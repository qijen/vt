// OGGReader.h: interface for the OGGReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGGReader_H__1ACADF42_5A21_11D3_BA37_00105A669BB5__INCLUDED_)
#define AFX_OGGReader_H__1ACADF42_5A21_11D3_BA37_00105A669BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

// buffer and size
#define BUFFERSIZE 16384
#define BITSPERSAMPLE 16

//CKERROR InitInstance(CKContext *context);

class OGGReader : public CKSoundReader  
{
public:
	OGGReader();
	virtual ~OGGReader();

	virtual void Release();

	virtual CKPluginInfo * GetReaderInfo();

	// Get Options Count
	virtual int GetOptionsCount() { return 0; }
	// Get Options Description
	virtual CKSTRING GetOptionDescription(int i) {return NULL; }
	
	// Get Flags
	virtual CK_DATAREADER_FLAGS GetFlags() { return (CK_DATAREADER_FLAGS) (CK_DATAREADER_FILELOAD | CK_DATAREADER_MEMORYLOAD); }

public:
	// Opens a file
	virtual CKERROR OpenFile(char *file);

	//virtual CKERROR ReadMemory(void* memory, int size);
	
	// Decodes next chunk of data, use get data buffer to get decoded data
	virtual CKERROR Decode();

	// Gets the last decoded buffer
	virtual CKERROR GetDataBuffer(BYTE **buf, int *size);
	
	// Gets the wave format of decoded datas
	virtual CKERROR GetWaveFormat(CKWaveFormat *wfe);

	// Gets whole decoded data size
	virtual int GetDataSize();
	
	// Gets the play time length
	virtual int GetDuration();

	// Seek
	virtual CKERROR Seek(int pos);
	// Play;
	virtual CKERROR Play();
	// Stop
	virtual CKERROR Stop();

	virtual CKERROR Pause(){return CK_OK;}
	virtual CKERROR Resume(){return CK_OK;}

protected:
/*
	int DataRead(int count, char* buf); 
	int DataRead(long pos, int count, char* buf);
	int DataSeek(long pos);
*/
    OggVorbis_File m_VorbisFile;
    CKWaveFormat m_FormatInfo;
    bool m_EOF;
	bool m_FileOpen;
    int m_CurrentSection;
/*
    UINT m_nBlockAlign;         // ogg data block alignment spec
    UINT m_nAvgDataRate;        // average ogg data rate
    UINT m_nBytesPlayed;        // offset into data chunk
	BOOL m_IsPcm;

	unsigned long m_OutDataSize;	// size of in  data chunk
	unsigned long m_InDataSize;		// size of out data chunk
	
	unsigned long m_InDataCursor;
*/
// PCM Data

	BYTE *m_pBuffer;		// Data Buffer;
	int m_BufferDataRead;

	
};

#endif // !defined(AFX_OGGReader_H__1ACADF42_5A21_11D3_BA37_00105A669BB5__INCLUDED_)
