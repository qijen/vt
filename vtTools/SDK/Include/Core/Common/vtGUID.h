/********************************************************************
	created:	2009/04/14
	created:	14:4:2009   10:06
	filename: 	x:\ProjectRoot\vtmodsvn\tools\VTCPPProjectPremakerSimple\SDK\Include\Core\Common\vtGUID.h
	file path:	x:\ProjectRoot\vtmodsvn\tools\VTCPPProjectPremakerSimple\SDK\Include\Core\Common 
	file base:	vtGUID
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Wrapper for CKGuid
*********************************************************************/

class vtGUID
{
private:
    CKGUID      guid;

public:

	CKGUID GetVirtoolsGUID()
	{
		return guid;
	}
    vtGUID( DWORD d1=0, DWORD d2=0 ):guid(d1,d2) {}

    XString ToString( void )
    {
        XString laid;

        laid << DecimalToHex( guid.d1 );
        laid << DecimalToHex( guid.d2 );

        return laid;
    }

    bool FromString( const XString laid )
    {

		if( laid.Length() != 16 )
			return false;

		XString d1(laid);
        XString d2(laid);

        d1.Crop( 0, 8 );
        d2.Crop( 8, 8 );

        HexToDecimalHI( d1.Str(), guid.d1 );
        HexToDecimalLO( d2.Str(), guid.d2 );

        return true;
    }

    vtGUID & operator =( const CKGUID& ckguid )
    {
        guid.d1 = ckguid.d1;
        guid.d2 = ckguid.d2;

        return *this;
    }

    operator XString() { return ToString(); }

private:
    XString DecimalToHex( int decimal )
    {
        XString     hexStr;
		char hexstring[17]; 
		itoa( decimal, hexstring, 16);

		int length = strlen(hexstring);
		if (length < 8)
		{
			int add = 8 - length;
			for (int i = 0; i < add; i++)
			{
				hexStr << "0";
			}
		}

		hexStr << hexstring;

		return hexStr;

    }

    bool HexToDecimalHI (char* HexNumber, unsigned int& Number)
    { 
        char* pStopString; 
        Number = strtol (HexNumber, &pStopString, 16);
        return (bool)(Number != LONG_MAX);
    }
	bool HexToDecimalLO(char* HexNumber, unsigned int& Number)
	{ 
		char* pStopString; 
		Number = strtoul(HexNumber, &pStopString, 16);
		return (bool)(Number != LONG_MAX);
	}
};