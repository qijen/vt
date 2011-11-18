// OGGReader.cpp: implementation of the OGGReader class.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
//#include "URLMon.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

/* debug context
CKContext *g_pContext;

CKERROR InitInstance(CKContext* context)
{
    g_pContext = context;
	return CK_OK;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////
// Construction

OGGReader::OGGReader()
{
// Init data members
    m_FormatInfo.cbSize = 0;
    m_FormatInfo.nAvgBytesPerSec = 0;
    m_FormatInfo.nBlockAlign = 0;
    m_FormatInfo.nChannels = 0;
    m_FormatInfo.nSamplesPerSec = 0;
    m_FormatInfo.wBitsPerSample = 0;
    m_FormatInfo.wFormatTag = 0;

    m_EOF = 0;
    m_CurrentSection = -1;

	m_pBuffer = new BYTE[BUFFERSIZE];
	m_BufferDataRead = 0;

	m_FileOpen = false;

}

//////////////////////////////////////////////////////////////////////////////////////////
// Destruction

OGGReader::~OGGReader()
{
    // Close Ogg file
    if(m_FileOpen)
		ov_clear(&m_VorbisFile);

	delete [] m_pBuffer;
}

//CKERROR OGGReader::ReadMemory(void* memory, int size)
//{
//	return CKSOUND_READER_GENERICERR;
//}

//////////////////////////////////////////////////////////////////////////////////////////
// Opens a file

bool isExist(const char* filepath)
{
  FILE* f = 0;
  if (filepath && (f=fopen(filepath,"r")) != 0)
    fclose(f);
  return f != 0;
}

CKERROR OGGReader::OpenFile(char *file)
{
    CKERROR fRtn = CK_OK;    // assume success
	int ovResult;

    if (!file || strlen(file) == 0)
        goto OPEN_ERROR;

	if(isExist(file) == FALSE)
		goto OPEN_ERROR;

    // Make a C-style file stream out of the cached path
    FILE *pOggFile;
    if (!(pOggFile = fopen(file, "rb")))
		goto OPEN_ERROR;

    // Open the vorbis file
    ovResult = ov_open(pOggFile, &m_VorbisFile, NULL, 0);

    if(ovResult < 0)
		goto OPEN_ERROR;
	
	m_FileOpen = true;
    vorbis_info *pVorbisInfo;
    if(!(pVorbisInfo = ov_info(&m_VorbisFile, -1))) {
       // g_pContext->OutputToConsole("Ogg Error", false);
        goto OPEN_ERROR;
    }

    m_FormatInfo.wFormatTag = 1;
    m_FormatInfo.nChannels = pVorbisInfo->channels;
    m_FormatInfo.nSamplesPerSec = pVorbisInfo->rate;
    m_FormatInfo.wBitsPerSample = BITSPERSAMPLE;
    m_FormatInfo.nBlockAlign = (pVorbisInfo->channels * m_FormatInfo.wBitsPerSample) / 8;
//    m_FormatInfo.nAvgBytesPerSec = ov_bitrate(m_VorbisFile, -1) / 8;
    m_FormatInfo.nAvgBytesPerSec = m_FormatInfo.nBlockAlign * m_FormatInfo.nSamplesPerSec;
    m_FormatInfo.cbSize = 0;

	/*
    sprintf(str, "Channels: %i", m_FormatInfo.nChannels);
    g_pContext->OutputToConsole(str, false);
    sprintf(str, "SamplesPerSec: %i", m_FormatInfo.nSamplesPerSec);
    g_pContext->OutputToConsole(str, false);
    sprintf(str, "BlockAlign: %i", m_FormatInfo.nBlockAlign);
    g_pContext->OutputToConsole(str, false);
    sprintf(str, "AvgBytesPerSec: %i", m_FormatInfo.nAvgBytesPerSec);
    g_pContext->OutputToConsole(str, false);
	*/

	//return CK_OK;
	return fRtn;


OPEN_ERROR:
    
	// Handle all errors here
    fRtn = CKSOUND_READER_GENERICERR;
    // Close Ogg file
    //ov_clear(&m_VorbisFile);
	return CKSOUND_READER_GENERICERR;
	
  }

//////////////////////////////////////////////////////////////////////////////////////////
// Decodes 'n' stuff

// Decodes next chunk of data, use get data buffer to get decoded data
CKERROR OGGReader::Decode() 
{

	m_BufferDataRead = 0;
    int startSection = m_CurrentSection;

  long result = ov_read(&m_VorbisFile,
                          (char *)m_pBuffer,
                          BUFFERSIZE,
                          0,
                          2,
                          1,
                          &m_CurrentSection);
//    char str[256];
//    sprintf(str, "ov_read result: %i", result);
//    g_pContext->OutputToConsole(str, true);

    // End Of File
    if (result == 0) {
        m_EOF = true;
        //g_pContext->OutputToConsole("E-O-F", true);
        return CKSOUND_READER_EOF;
    }
    // Error in the bitstram
    else if (result < 0) {
        //g_pContext->OutputToConsole("Error in the bitstram in Decode", false);
        return CKSOUND_READER_GENERICERR;
    }

    // Successfully read some stuff
    m_BufferDataRead = result;

    if (m_CurrentSection != startSection) {
        vorbis_info *pVorbisInfo;

        m_FormatInfo.wFormatTag = 1;
        m_FormatInfo.nChannels = pVorbisInfo->channels;
        m_FormatInfo.nSamplesPerSec = pVorbisInfo->rate;
        m_FormatInfo.wBitsPerSample = BITSPERSAMPLE;
        m_FormatInfo.nBlockAlign = (pVorbisInfo->channels * m_FormatInfo.wBitsPerSample) / 8;
//        m_FormatInfo.nAvgBytesPerSec = ov_bitrate(m_VorbisFile, -1) / 8;
        m_FormatInfo.nAvgBytesPerSec = m_FormatInfo.nBlockAlign * m_FormatInfo.nSamplesPerSec;
        m_FormatInfo.cbSize = 0;
    }

    return CK_OK;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Gets the last decoded buffer

CKERROR OGGReader::GetDataBuffer(BYTE **buf, int *size)
{
	*buf = m_pBuffer;
	*size = m_BufferDataRead;

	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gets the ogg format of decoded datas

CKERROR OGGReader::GetWaveFormat(CKWaveFormat *wfe)
{
	memcpy(wfe, &m_FormatInfo, sizeof(CKWaveFormat));
	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gets whole decoded data size

int OGGReader::GetDataSize()
{
	return (int)(ov_pcm_total(&m_VorbisFile, -1) * m_FormatInfo.nBlockAlign); // * m_FormatInfo.nSamplesPerSec * m_FormatInfo.nBlockAlign);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gets the play time length

int OGGReader::GetDuration()
{
	return (int)(ov_time_total(&m_VorbisFile, -1) * 1000);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Seek some shit

CKERROR OGGReader::Seek(int pos)
{
    if(ov_seekable(&m_VorbisFile)) {
        ov_time_seek(&m_VorbisFile, pos);
    }
    else {
        //g_pContext->OutputToConsole("Tried to seek a non-seekable file!", false);
        return CKSOUND_READER_GENERICERR;
    }
   
	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Play

CKERROR OGGReader::Play()
{
	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Stop

CKERROR OGGReader::Stop()
{
	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Release

void OGGReader::Release()
{
	delete this;
}
