#ifndef _DEFINE_H
#define _DEFINE_H

#define MAX_USER			3000
#define MAX_ID_SIZE			20
#define MAX_PW_SIZE			50
#define MAX_ITEM			59

////////////////////////////////////////////////////////////
// Socket Define
////////////////////////////////////////////////////////////
#define SOCKET_BUFF_SIZE	(1024*8)
#define MAX_PACKET_SIZE		(1024*2)

#define PACKET_START1				0XAA
#define PACKET_START2				0X55
#define PACKET_END1					0X55
#define PACKET_END2					0XAA
//#define PROTOCOL_VER				0X01

// status
#define STATE_CONNECTED			0X01
#define STATE_DISCONNECTED		0X02
#define STATE_GAMESTART			0x03

/////////////////////////////////////////////////////
// ITEM_SLOT DEFINE
#define RIGHTEAR			0
#define HEAD				1
#define LEFTEAR				2
#define NECK				3
#define BREAST				4
#define SHOULDER			5
#define RIGHTHAND			6
#define WAIST				7
#define LEFTHAND			8
#define RIGHTRING			9
#define LEG					10
#define LEFTRING			11
#define GLOVE				12
#define FOOT				13
/////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

typedef union{
	short int	i;
	BYTE		b[2];
} MYSHORT;

typedef union{
	int			i;
	BYTE		b[4];
} MYINT;

typedef union{
	DWORD		w;
	BYTE		b[4];
} MYDWORD;


// DEFINE MACRO PART...
#define BufInc(x) (x)++;(x) %= SOCKET_BUF_SIZE;

// DEFINE Shared Memory Queue Flag

#define E	0x00
#define R	0x01
#define W	0x02
#define WR	0x03

// DEFINE Shared Memory Queue Return VALUE

#define SMQ_BROKEN		10000
#define SMQ_FULL		10001
#define SMQ_EMPTY		10002
#define SMQ_PKTSIZEOVER	10003
#define SMQ_WRITING		10004
#define SMQ_READING		10005

// DEFINE Shared Memory Costumizing

#define MAX_PKTSIZE		512
#define MAX_COUNT		4096
#define SMQ_LOGGERSEND	"KNIGHT_SEND"
#define SMQ_LOGGERRECV	"KNIGHT_RECV"

// Packet Define...
#define WIZ_LOGIN				0x01	// Account Login
#define WIZ_NEW_CHAR			0x02	// Create Character DB
#define WIZ_DEL_CHAR			0x03	// Delete Character DB
#define WIZ_SEL_CHAR			0x04	// Select Character
#define WIZ_SEL_NATION			0x05	// Select Nation
#define WIZ_ALLCHAR_INFO_REQ	0x0C	// Account All Character Info Request
#define WIZ_LOGOUT				0x0F	// Request Logout
#define WIZ_DATASAVE			0x37	// User GameData DB Save Request
#define WIZ_KNIGHTS_PROCESS		0x3C	// Knights Related Packet..
#define WIZ_CLAN_PROCESS		0x4E	// Clans Related Packet..
#define WIZ_LOGIN_INFO			0x50	// define for DBAgent Communication
#define WIZ_KICKOUT				0x51	// Account ID forbid duplicate connection
#define WIZ_BATTLE_EVENT		0x57	// Battle Event Result
#define DB_COUPON_EVENT			0x10	// coupon event
#define WIZ_LETTER				0x6A	// Letter system

		#define CHECK_COUPON_EVENT		0x01
		#define UPDATE_COUPON_EVENT		0x02

////////////////////////////////////////////////////////////////
// Knights Packet sub define 
////////////////////////////////////////////////////////////////
#define KNIGHTS_CREATE			0x11		// »ý¼º
#define KNIGHTS_JOIN			0x12		// °¡ÀÔ
#define KNIGHTS_WITHDRAW		0x13		// Å»Åð
#define KNIGHTS_REMOVE			0x14		// ¸â¹ö »èÁ¦
#define KNIGHTS_DESTROY			0x15		// »Ç°³±â
#define KNIGHTS_ADMIT			0x16		// ¸â¹ö °¡ÀÔ Çã°¡
#define KNIGHTS_REJECT			0x17		// ¸â¹ö °¡ÀÔ °ÅÀý
#define KNIGHTS_PUNISH			0x18		// ¸â¹ö Â¡°è
#define KNIGHTS_CHIEF			0x19		// ´ÜÀå ÀÓ¸í
#define KNIGHTS_VICECHIEF		0x1A		// ºÎ´ÜÀå ÀÓ¸í
#define KNIGHTS_OFFICER			0x1B		// Àå±³ÀÓ¸í
#define KNIGHTS_ALLLIST_REQ		0x1C		// ¸®½ºÆ®¸¦ 10°³ ´ÜÀ§·Î Page ¿äÃ»
#define KNIGHTS_MEMBER_REQ		0x1D		// ¸ðµç ¸â¹ö ¿äÃ»
#define KNIGHTS_CURRENT_REQ		0x1E		// ÇöÀç Á¢¼Ó ¸®½ºÆ®
#define KNIGHTS_STASH			0x1F		// ±â»ç´Ü Ã¢°í
#define KNIGHTS_MODIFY_FAME		0x20		// ¸â¹öÀÇ Á÷À§ º¯°æ.. ÇØ´ç ¸â¹ö¿¡°Ô °£´Ù
#define KNIGHTS_JOIN_REQ		0x21		// ÇØ´ç¸â¹ö¿¡°Ô °¡ÀÔ¿äÃ»À» ÇÑ´Ù
#define KNIGHTS_LIST_REQ		0x22		// ±â»ç´Ü ¸®½ºÆ®¸¦  ¿äÃ» ( index °Ë»ö )

////////////////////////////////////////////////////////////////
// Clan Packet sub define
////////////////////////////////////////////////////////////////
#define CLAN_CREATE				0x01
#define CLAN_JOIN				0x02

////////////////////////////////////////////////////////////////
// Letter Packet sub define 
////////////////////////////////////////////////////////////////
#define LETTER_L2				0x02 // Karakter seçildiðinde kullanýlýyor
#define	LETTER_L5				0x05 // Letteri açtýðýnda
#define	LETTER_L6				0x06 // Letteri açtýðýnda

////////////////////////////////////////////////////////////////
// Update User Data type define
////////////////////////////////////////////////////////////////
#define UPDATE_LOGOUT			0x01
#define UPDATE_ALL_SAVE			0x02
#define UPDATE_PACKET_SAVE		0x03

// Reply packet define...

#define SEND_ME					0x01
#define SEND_REGION				0x02
#define SEND_ALL				0x03

#define SLOT_MAX			14		// Âø¿ë ¾ÆÅÛ MAX
#define HAVE_MAX			28		// ¼ÒÀ¯ ¾ÆÅÛ MAX (ÀÎº¥Åä¸®Ã¢)	28
#define EXTRA_MAX			30
#define ITEMCOUNT_MAX		9999	// ¼Ò¸ð ¾ÆÀÌÅÛ ¼ÒÀ¯ ÇÑ°è°ª
#define WAREHOUSE_MAX		196		// Ã¢°í ¾ÆÀÌÅÛ MAX

/////////////////////////////////////////////////////////////////////////////////
// Structure Define
/////////////////////////////////////////////////////////////////////////////////
struct	_ITEM_DATA
{
	int		nNum;		// item ¹øÈ£
	short	sDuration;	// item ³»±¸·Â
	short	sCount;		// item °¹¼ö or item Ãàº¹ ¼Ó¼º¿¡ ÇØ´ç °ª
	__int64	nSerialNum;	// item serial code
};

struct _ITEM_TABLE
{
	int   m_iNum;				// item num
	char  m_strName[50];		// item Name
	BYTE  m_bKind;				// item Á¾·ù
	BYTE  m_bSlot;				// ÀåÂøÀ§Ä¡
	BYTE  m_bRace;				// »ç¿ë °¡´ÉÇÑ Á¾Á·
	BYTE  m_bClass;				// »ç¿ë °¡´ÉÇÑ Class
	short m_sDamage;			// ÃÖ´ë Å¸°ÝÄ¡
	short m_sDelay;				// °ø°Ý½Ã°£
	short m_sRange;				// ¹üÀ§(À¯È¿°Å¸®)
	short m_sWeight;			// ¹«°Ô
	short m_sDuration;			// ³»±¸¼º
	int	  m_iBuyPrice;			// À¯Àú°¡ »ç´Â°¡°Ý
	int	  m_iSellPrice;			// À¯Àú°¡ ÆÄ´Â°¡°Ý
	short m_sAc;				// ¹æ¾î·Â
	BYTE  m_bCountable;			// °³¼ö °³³ä ¾ÆÀÌÅÛ
	int	  m_iEffect1;			// ¸ÅÁ÷ ÀÌÆåÆ®1
	int	  m_iEffect2;			// ¸ÅÁ÷ ÀÌÆåÆ®2
	BYTE  m_bReqLevel;			// ¿ä±¸ ·¹º§
	BYTE  m_bReqRank;			// ¿ä±¸ ÀÛÀ§
	BYTE  m_bReqTitle;			// ¿ä±¸ ÁöÀ§
	BYTE  m_bReqStr;			// ¿ä±¸ Èû
	BYTE  m_bReqSta;			// ¿ä±¸ Ã¼·Â
	BYTE  m_bReqDex;			// ¿ä±¸ ¹ÎÃ¸
	BYTE  m_bReqIntel;			// ¿ä±¸ Áö´É
	BYTE  m_bReqCha;			// ¿ä±¸ ¸Å·Â
	BYTE  m_bSellingGroup;		// »óÀÎ Ãë±Þ ¹°Ç°
	BYTE  m_ItemType;			// ¸ÅÁ÷¾ÆÀÌÅÛ ¶Ç´Â ·¹¾î¾ÆÀÌÅÛ
	short m_sHitrate;			// Å¸°Ý·ü
	short m_sEvarate;			// È¸ÇÇÀ²
	short m_sDaggerAc;			// ¹æ¾î·Â1
	short m_sSwordAc;			// ¹æ¾î·Â2
	short m_sMaceAc;			// ¹æ¾î·Â3
	short m_sAxeAc;				// ¹æ¾î·Â4
	short m_sSpearAc;			// ¹æ¾î·Â5
	short m_sBowAc;				// ¹æ¾î·Â6
	BYTE  m_bFireDamage;		// ºÒ ¼Ó¼º
	BYTE  m_bIceDamage;			// ³Ã±â ¼Ó¼º
	BYTE  m_bLightningDamage;	// Àü°Ý ¼Ó¼º
	BYTE  m_bPoisonDamage;		// µ¶ ¼Ó¼º
	BYTE  m_bHPDrain;			// HP Èí¼ö
	BYTE  m_bMPDamage;			// MP Å¸°Ý
	BYTE  m_bMPDrain;			// MP Èí¼ö
	BYTE  m_bMirrorDamage;		// ¹Ý»ç Å¸°Ý
	BYTE  m_bDroprate;			// µå·Ó ºñÀ²
	BYTE  m_bStrB;				// Èû º¸³Ê½º
	BYTE  m_bStaB;				// Ã¼·Â º¸³Ê½º
	BYTE  m_bDexB;				// ¹ÎÃ¸¼º º¸³Ê½º
	BYTE  m_bIntelB;			// Áö´É º¸³Ê½º
	BYTE  m_bChaB;				// ¸Å·Â º¸³Ê½º
	short m_MaxHpB;				// MaxHP add
	short m_MaxMpB;				// MaxMP add
	BYTE  m_bFireR;				// ºÒ ¸¶¹ý ÀúÇ×·Â
	BYTE  m_bColdR;				// ¾óÀ½ ¸¶¹ý ÀúÇ×·Â
	BYTE  m_bLightningR;		// Àü±â ¸¶¹ý ÀúÇ×·Â
	BYTE  m_bMagicR;			// ±âÅ¸ ¸¶¹ý ÀúÇ×·Â
	BYTE  m_bPoisonR;			// µ¶ ¸¶¹ý ÀúÇ×·Â
	BYTE  m_bCurseR;			// ÀúÁÖ ¸¶¹ý ÀúÇ×·Â
};

#pragma pack(push, 1)
struct _USER_DATA
{
	char m_id[MAX_ID_SIZE + 1];
	char m_Accountid[MAX_ID_SIZE + 1];

	BYTE m_bZone;
	float m_curx;
	float m_curz;
	float m_cury;

	BYTE m_bNation;
	BYTE m_bRace;
	short m_sClass;
	BYTE m_bHairColor;
	BYTE m_bRank;
	BYTE m_bTitle;
	BYTE m_bLevel;

	int m_iExp;
	int m_iLoyalty;

	BYTE m_bFace;
	BYTE m_bCity;
	short m_bKnights;
	BYTE m_bFame;

	short m_sHp;
	short m_sMp;
	short m_sSp;

	BYTE m_bStr;
	BYTE m_bSta;
	BYTE m_bDex;
	BYTE m_bIntel;
	BYTE m_bCha;

	BYTE m_bAuthority;
	BYTE m_bPoints;

	int m_iGold;
	BYTE m_bZoneIndex;
	short m_sBind;

	int m_iBank;

	BYTE m_bstrSkill[9];

	_ITEM_DATA m_sItemArray[HAVE_MAX + SLOT_MAX];
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];

	BYTE m_bLogout;
	BYTE m_bWarehouse;
	DWORD m_dwTime;

	// extras (SP uyumlu)
	char m_strQuest[400];
	short m_sQuestCount;
	int m_iMannerPoint;
	int m_iLoyaltyMonthly;
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	Global Function Define
//

inline void GetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf, sBuf+index, len);
	index += len;
};

inline BYTE GetByte(char* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (BYTE)(*(sBuf+t_index));
};

inline int GetShort(char* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf+index-2);
};

inline DWORD GetDWORD(char* sBuf, int& index)
{
	index += 4;
	return *(DWORD*)(sBuf+index-4);
};

inline float Getfloat(char* sBuf, int& index)
{
	index += 4;
	return *(float*)(sBuf+index-4);
};

inline __int64 GetInt64(char* sBuf, int& index)
{
	index += 8;
	return *(__int64*)(sBuf+index-8);
};

inline void SetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf+index, sBuf, len);
	index += len;
};

inline void SetByte(char* tBuf, BYTE sByte, int& index)
{
	*(tBuf+index) = (char)sByte;
	index++;
};

inline void SetShort(char* tBuf, int sShort, int& index)
{
	short temp = (short)sShort;

	CopyMemory( tBuf+index, &temp, 2);
	index += 2;
};

inline void SetDWORD(char* tBuf, DWORD sDWORD, int& index)
{
	CopyMemory( tBuf+index, &sDWORD, 4);
	index += 4;
};

inline void Setfloat ( char* tBuf, float sFloat, int& index )
{
	CopyMemory( tBuf+index, &sFloat, 4);
	index += 4;
};

inline void SetInt64 ( char* tBuf, __int64 nInt64, int& index )
{
	CopyMemory( tBuf+index, &nInt64, 8);
	index += 8;
};
// sungyong 2001.11.06
inline int GetVarString(TCHAR* tBuf, TCHAR* sBuf, int nSize, int& index)
{
	int nLen = 0;
	
	if(nSize == sizeof(BYTE))	nLen = GetByte(sBuf, index);
	else nLen = GetShort(sBuf, index);

	GetString(tBuf, sBuf, nLen, index);
	*(tBuf + nLen) = 0;

	return nLen;
};

inline void SetVarString(TCHAR *tBuf, TCHAR* sBuf, int len, int &index)
{
	*(tBuf+index) = (BYTE)len;
	index ++;

	CopyMemory(tBuf+index, sBuf, len);
	index += len;
};
// ~sungyong 2001.11.06
inline int ParseSpace( char* tBuf, char* sBuf)
{
	int i = 0, index = 0;
	BOOL flag = FALSE;
	
	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	while(sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] !=(BYTE) 0){
		tBuf[i++] = sBuf[index++];
		flag = TRUE;
	}
	tBuf[i] = 0;

	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	if(!flag) return 0;	
	return index;
};

inline CString GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString _Path = Path;
	return _Path;
};

inline void LogFileWrite( LPCTSTR logstr )
{
	CString ProgPath, LogFileName;
	CFile file;
	int loglength;

	ProgPath = GetProgPath();
	loglength = strlen( logstr );

	LogFileName.Format("%s\\Aujard.log", ProgPath);
	
	file.Open( LogFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );

	file.SeekToEnd();
	file.Write(logstr, loglength);
	file.Close();
};

inline int DisplayErrorMsg(SQLHANDLE hstmt)
{
	SQLCHAR       SqlState[6], Msg[1024];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc2;
	char		  logstr[512];
	memset( logstr, NULL, 512 );

	i = 1;
	while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
	{
		sprintf( logstr, "*** %s, %d, %s, %d ***\r\n", SqlState,NativeError,Msg,MsgLen );
		LogFileWrite( logstr );

		i++;
	}

	if( strcmp((char *)SqlState, "08S01") == 0 )
		return -1;
	else
		return 0;
};

#endif