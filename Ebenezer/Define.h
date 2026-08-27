#ifndef _DEFINE_H
#define _DEFINE_H

#include <mmsystem.h>

#define _LISTEN_PORT		15000
#define _UDP_PORT			8888
#define AI_KARUS_SOCKET_PORT		10020
#define AI_ELMO_SOCKET_PORT			10030
#define AI_BATTLE_SOCKET_PORT		10040
#define MAX_USER			3000
#define CLIENT_SOCKSIZE		4096

//#define CLIENT_SOCKSIZE		100
#define MAX_AI_SOCKET		10			// sungyong~ 2002.05.22

#define MAX_TYPE3_REPEAT    20
#define MAX_TYPE4_BUFF		9

#define MAX_ITEM_COUNT		9999		// ÇÑ ½½·Ô¿¡ °¡Áö´Â ÃÖ´ë È­»ì/¼ÛÆí °³¼ö

#define MAX_ID_SIZE			20
#define MAX_NPC_SIZE		30
//#define MAX_NPC_NAME_SIZE	20
#define MAX_PW_SIZE			49
#define MAX_ITEM			28
#define VIEW_DISTANCE		48			// °¡½Ã°Å¸®

const uint8_t SLOT_MAX			= 14;		// Âø¿ë ¾ÆÅÛ MAX
const uint8_t HAVE_MAX			= 28;		// ¼ÒÀ¯ ¾ÆÅÛ MAX (ÀÎº¥Åä¸®Ã¢)
const uint8_t EXTRA_MAX		= 30;
const uint8_t WAREHOUSE_MAX	= 196;		// Ã¢°í ¾ÆÀÌÅÛ MAX

#define NPC_HAVE_ITEM_LIST	6
#define ZONEITEM_MAX		2100000000	// Á¸¿¡ ¶³¾îÁö´Â ÃÖ´ë ¾ÆÀÌÅÛ¼ö...

#define MAX_CLASS			26			// Á÷¾÷ MAX
#define MAX_LEVEL			80			// ÃÖ°í·¾...

#define SERVER_INFO_START			0X01
#define SERVER_INFO_END				0X02

#define MAX_SEND_SIZE		(MAX_PACKET_SIZE * 2)

//////////////  Quest °ü·Ã Define ////////////////////////////
/* tamer
 --³£Á¿-- 
CHECK_UNDER_WEIGHT			=		1
CHECK_OVER_WEIGHT			=		2
CHECK_SKILL_POINT			=		3
CHECK_EXIST_ITEM			=		4
CHECK_CLASS					=		5
CHECK_WEIGHT				=		6
CHECK_EDITBOX				=		7
RAND	    				=		8
CHECK_HOWMUCH_ITEM			=		9
CHECK_LV    				=		10
NOEXIST_COM_EVENT			=		11
EXIST_COM_EVENT				=		12
CHECK_NOAH					=		13
CHECK_NATION				=		14
CHECK_PPCARD_SERIAL			=		15
CHECK_PPCARD_TYPE			=		16
CHECK_EXIST_ITEM_AND			=	17
CHECK_EXIST_ITEM_OR				=	18
CHECK_NOEXIST_ITEM_AND			=	19
CHECK_NOEXIST_ITEM_OR			=	20
CHECK_EXIST_EVENT				=	21
CHECK_NOEXIST_EVENT				=	22
CHECK_PROMOTION_ELIGIBLE 		=	23
CHECK_EXCHANGE					=	24
CHECK_ITEMCHANGE_NUM			=	25
CHECK_NOCLASS					=	26
CHECK_LOYALTY					=	27
CHECK_CHIEF						=	28
CHECK_NO_CHIEF					=	29
CHECK_CLAN_GRADE				=	30
CHECK_KNIGHT					=	31
CHECK_DICE						=	32
CHECK_CLAN						=	33
CHECK_NO_CLAN					=	34
CHECK_MANNER					=	35
CHECK_MONSTER_CHALLENGE_TIME	=   36
CHECK_MONSTER_CHALLENGE_USERCOUNT = 37
CHECK_CASTLE					=	38
CHECK_NO_CASTLE					=	39
CHECK_SKILL_TOTAL				=	40
CHECK_STAT_TOTAL				=	41
CHECK_EMPTY_SLOT				=	42
CHECK_LOYALTY_RANK_MONTHLY 		=	43
CHECK_LOYALTY_RANK				=	44
CHECK_CLAN_RANKING				=	45
CHECK_MIDDLE_STATUE_CAPTURE		=   46
CHECK_MIDDLE_STATUE_NOCAPTURE 	=	47
CHECK_INPUT_COUNT				=	48
CHECK_EXIST_ITEM_INPUT_COUNT	=	49
CHECK_WEIGHT_INPUT_COUNT		=	50
CHECK_EXAM_COUNT				=	51
CHECK_BEEF_ROAST_KARUS_VICTORY	=   52
CHECK_BEEF_ROAST_ELMORAD_VICTORY =  53
CHECK_BEEF_ROAST_NO_VICTORY      =  54

SAY							  	=	 101
SELECT_MSG					 	=	 102
RUN_EVENT						=	 103
GIVE_ITEM						=	 104
ROB_ITEM						=	 105
RETURN							=	 106
OPEN_EDITBOX					=	 107
GIVE_NOAH						=	 108
LOG_COUPON_ITEM					=	 109
SAVE_COM_EVENT					=	 110
EXEC_ROB_NOAH					=	 111
REQUEST_REWARD					=    112
GIVE_PPCARD_ITEM				=    113
SAVE_EVENT						=    114
PROMOTE_USER					=    115
GIVE_PROMOTION_QUEST        	=    116
RUN_EXCHANGE                	=    117
KISS_USER                   	=    118
ZONE_CHANGE                 	=    119
PROMOTE_USER_NOVICE         	=    120
SKILL_POINT_DISTRIBUTE      	=    121
STAT_POINT_DISTRIBUTE       	=    122
LEVEL_UP                    	=    123
EXP_CHANGE                  	=    124
DESTROY_ITEM                	=    125
PROMOTE_KNIGHT              	=    126
CHANGE_POSITION             	=    127
ROOL_DICE                   	=    128
ZONE_CHANGE_CLAN             	=    129
CHANGE_LOYALTY                	= 	 130
SKILL_POINT_FREE               	= 	 131
STAT_POINT_FREE                	= 	 132
CHANGE_NAME                    	=    133
SEND_WEBPAGE_ADDRESS           	=    134
ROB_ALLITEM_PARTY             	=    135
ZONE_CHANGE_PARTY              	=    136
STATE_CHANGE                   	=    137
EMIGRATION_ACCEPT              	=    138
CHANGE_MANNER                  	=    139
SHOW_PCBANG_ITEM               	=    140
CHECK_PCBANG_ITEM              	=    141
GIVE_PCBANG_ITEM               	=    142
CHECK_PCBANG_OWNER             	=    143
REQUEST_PERSONAL_RANK_REWARD   	=    144
CHECK_KJWAR_ACCOUNT            	=    145
GIVE_KJWAR_ITEM                	=    146
CHECK_LOGTIME_ACCOUNT          	=    147
GIVE_LOGTIME_ITEM              	=    148
MOVE_MIDDLE_STATUE             	=    149
CHECK_OLYMPIC_ACCOUNT          	=    150
LOG_OLYMPIC_ACCOUNT            	=    151
OPEN_INPUT_COUNT               	=    152
ROB_ITEM_INPUT_COUNT           	=    153
GIVE_ITEM_INPUT_COUNT          	=    154
ROB_EXAM                       	=    155
*/
#define MAX_EVENT					2000
#define MAX_EVENT_SIZE				400
#define MAX_EVENT_NUM				2000
#define MAX_EXEC_INT				30
#define MAX_LOGIC_ELSE_INT			10
#define MAX_MESSAGE_EVENT			10
#define MAX_COUPON_ID_LENGTH		20
#define MAX_CURRENT_EVENT			20

// Áö±İ ¾²ÀÌ´Â°Í¸¸ Á¤ÀÇ ÇØ ³ù½À´Ï´Ù.
// logic°ü·Ã define
#define LOGIC_CHECK_UNDER_WEIGHT	0X01
#define LOGIC_CHECK_OVER_WEIGHT		0X02
#define LOGIC_CHECK_SKILL_POINT		0X03
#define LOGIC_EXIST_ITEM			0X04
#define LOGIC_CHECK_CLASS			0x05
#define LOGIC_CHECK_WEIGHT			0x06
#define LOGIC_CHECK_EDITBOX			0x07
#define LOGIC_RAND					0x08
#define LOGIC_HOWMUCH_ITEM			0x09 
#define	LOGIC_CHECK_LEVEL			0x0A
#define LOGIC_NOEXIST_COM_EVENT		0x0B
#define LOGIC_EXIST_COM_EVENT		0x0C
#define LOGIC_CHECK_NOAH			0x0D

/*
#define	LOGIC_CHECK_NATION			0X01
#define	LOGIC_CHECK_LEVEL			0X02
#define	LOGIC_NOEXIST_ITEM			0X03
#define	LOGIC_QUEST_END				0X04
#define	LOGIC_QUEST_LOG				0X05
#define	LOGIC_EXIST_ITEM			0X06
#define	LOGIC_CHECK_NOAH			0X07
#define LOGIC_CHECK_CLASS			0x08
#define LOGIC_CHECK_WEIGHT			0x09
#define LOGIC_CHECK_RACE			0x0A
#define LOGIC_CHECK_LOYALTY			0x0B
#define LOGIC_CHECK_AUTHORITY		0x0C
#define LOGIC_CHECK_STR				0X0D
#define LOGIC_CHECK_STA				0x0D
#define LOGIC_CHECK_DEX				0x0E
#define LOGIC_CHECK_INT				0x0F
#define LOGIC_CHECK_CHA				0x10
#define LOGIC_CHECK_SKILLPOINT		0x11
#define LOGIC_CHECK_DAY				0x12
*/

// ½ÇÇà°ü·Ã define
#define EXEC_SAY					0X01
#define EXEC_SELECT_MSG				0X02
#define EXEC_RUN_EVENT				0X03
#define EXEC_GIVE_ITEM				0X04
#define EXEC_ROB_ITEM				0X05
#define EXEC_RETURN					0X06
#define EXEC_OPEN_EDITBOX			0x07
#define EXEC_GIVE_NOAH				0x08
#define EXEC_LOG_COUPON_ITEM		0x09
#define EXEC_SAVE_COM_EVENT			0x0A
#define EXEC_ROB_NOAH				0x0B

/*
#define EXEC_SAY					0X01
#define EXEC_SELECT_MSG				0X02
#define EXEC_RUN_EVENT				0X03
#define EXEC_RETURN					0X04
#define EXEC_ROB_ITEM				0X05
#define EXEC_ROB_NOAH				0X06
#define EXEC_GIVE_ITEM				0X07
#define EXEC_GIVE_QUEST				0X08
#define EXEC_QUEST_END				0X09
#define EXEC_QUEST_SAVE				0X0A
#define EXEC_GIVE_NOAH				0x0B
*/

// EVENT ½ÃÀÛ ¹øÈ£µé :)
#define EVENT_POTION				1
#define EVENT_LOGOS_ELMORAD			1001
#define EVENT_LOGOS_KARUS			2001
#define EVENT_COUPON				3001




////////////////////////////////////////////////////////////

///////////////// BBS RELATED //////////////////////////////
#define MAX_BBS_PAGE			23
#define MAX_BBS_MESSAGE			40
#define MAX_BBS_TITLE			20
#define MAX_BBS_POST			500

#define BUY_POST_PRICE			500
#define SELL_POST_PRICE			1000

#define REMOTE_PURCHASE_PRICE	5000
#define BBS_CHECK_TIME			36000

///////////////// NPC  STATUS //////////////////////////////
#define NPC_DEAD				0X00
#define NPC_LIVE				0X01

///////////////// NPC TYPE /////////////////////////////////
#define NPC_MONSTER				00
#define NPC_GENERAL				01		//
#define NPC_BOSS				03		// Unique Mop
#define NPC_PATROL_GUARD		11		// °æºñº´
#define NPC_MERCHANT			21		// »óÀÎ
#define NPC_TINKER				22		// ´ëÀåÀåÀÌ
#define NPC_WAREHOUSE			31		// Ã¢°íÁö±â
#define NPC_CAPTAIN2			35		// ÀüÁ÷
#define NPC_OFFICER				36		// ±â»ç´Ü °ü¸® NPC
#define NPC_CLERIC				37		// ´ë»çÁ¦ NPC
#define NPC_HEALER				40		// Healer
#define NPC_WARP				41		// Warp Npc
#define NPC_GATE				50		// ¼º¹®ÇüÅÂ NPC 
#define NPC_PHOENIX_GATE		51		// ±úÁöÁö ¾Ê´Â ¹® (8->51)
#define NPC_SPECIAL_GATE		52		// ±úÁöÁö ¾Ê´Â ¹®	ÀÌ¸é¼­ 2ºĞ¸¶´Ù ¿­·È´Ù ´İÇû´Ù ÇÏ´Â ¹®
#define NPC_GATE_LEVER			55		// ¼º¹® ·¹¹ö...	(9->55)	
#define NPC_ARTIFACT			60		// °á°è¼® (7->60)
#define NPC_DESTORY_ARTIFACT	61		// ÆÄ±«µÇ´Â °á°è¼®
#define NPC_MONK_ELMORAD		71		// ºñ·¯¸Ó±Û ¼öµµ½Â >.<
#define NPC_MONK_KARUS			72		// ºñ·¯¸Ó±Û ¼öµµ½Â >.<	
#define NPC_DOMESTIC_ANIMAL		99		// °¡Ãà NPC
#define NPC_COUPON				100		// ºñ·¯¸Ó±Û º¹±Ç >.<

// SPECIAL TYPE
#define NPC_UPGRADE				24 // Magic anvil
#define NPC_CAPTAIN				34 // [Grand merchant] kaishan
#define NPC_SIEGE				44 // Arendil [Castle Siege Warfare]
#define NPC_CHAOTIC				162 // Chaotic generator (137)
#define NPC_TOWER				63 // Guard tower
#define NPC_RENTAL				78 // item kiralama npcsi
#define NPC_ELECTION			79 // Kral seçme npcsi
#define NPC_LORD				80 // kralın npcsi
#define NPC_KISS				32 // KissMe
#define NPC_ADVISOR				26 // clan match advisor
#define NPC_KJWAR				133 // duel kjwar
#define NPC_SIEGE2				134 // Joyce[siege war event]
//
#define NPC_ARTIFACT1			65 // Protective Artifact 
#define NPC_ARTIFACT2			66 // Guard Tower Artifact
#define NPC_ARTIFACT3			67 // Guard Artifact
#define NPC_ARTIFACT4			68 // Defense Artifact
#define NPC_MONUMENT			122 // El Morad Monument & Asga Village Monument & Raiba Village Monument & Doda Camp Monument & Elmorad Snowman
#define NPC_GATE2				150 // Doda Camp Gate 
#define NPC_VICTORY_GATE		53 // Victory Gate savaştan karşı zoneye geçiş gatesi
#define NPC_RECON				43 // guard tipi
#define NPC_ROYAL_GUARD			142 // guard tipi
#define NPC_ADVISOR2			149 // bilinmiyor
#define NPC_SPY					141 // guard tipi
#define NPC_ROYAL_CHEF			143 // guard tipi
#define NPC_GATE_GUARD			148 // guard tipi
#define NPC_ESLANT_WOMAN		144 // bilinmiyor
#define NPC_FARMER				145 // bilinmiyor
#define NPC_UNKNOWN1			155 // bilinmiyor
#define NPC_UNKNOWN2			156 // bilinmiyor
#define NPC_UNKNOWN3			157 // bilinmiyor
#define NPC_UNKNOWN4			158 // bilinmiyor
#define NPC_UNKNOWN5			159 // bilinmiyor
#define NPC_UNKNOWN6			160 // bilinmiyor
#define NPC_UNKNOWN7			161 // bilinmiyor
#define NPC_CRAFTSMAN			135 // craftsman boy

// QUEST TYPE
#define NPC_MOIRA				29 // Operator moira
#define NPC_ISAAC				33 // isaac event
#define NPC_LADY				38 // calamus lady event
#define NPC_QUEST				47 // Sentinel patrick
#define NPC_ENCAMPMENT			64 // encampment
#define NPC_BLACKMITH			77 // Blacksmith heppa
#define NPC_HERO_STATUS1		106 // Elmorad & Karus Hero statue & 1st place clan
#define NPC_HERO_STATUS2		107 // Elmorad & Karus Hero statue & 2st place clan
#define NPC_HERO_STATUS3		108 // Elmorad & Karus Hero statue & 3st place clan
#define NPC_MINERVA				131 // learth [Forgotten Temple 3] & priest iris
#define NPC_HUGOR				27 // captain hugor[maintain]
#define NPC_ARENA				43 // ijin arena
#define NPC_SABICE				25 // sabice mantle
#define NPC_NPC1				101
#define NPC_NPC2				102
#define NPC_NPC3				103
#define NPC_NPC4				104
#define NPC_NPC5				105
#define NPC_KEY1				111 // Sentinel of the Key
#define NPC_KEY2				112 // Watcher of the Key
#define NPC_KEY3				113 // Protector of the Key
#define NPC_KEY4				114 // Ranger of the Key
#define NPC_KEY5				115 // Patroller of the Key
#define NPC_KEY6				116 // Recon of the Key
#define NPC_KEY7				117 // Keeper of the Key
#define NPC_ADELIA				153 // Goddess Adelia[event]
#define NPC_LAEMITH1			129 // Laemith[Forgotten Temple 1]
#define NPC_LAEMITH2			130 // Laemith[Forgotten Temple 2]
#define NPC_LAEMITH3			131 // Learth[Forgotten Temple 3]
#define	NPC_ATHIAN				39 // Priest athian
//
#define NPC_ROBOS				118 // Robos
#define NPC_TRANSFER			123 // lillia server transfer
#define NPC_RANKING				124 // hardis ranking
#define NPC_LYONI				125 // lyoni
#define NPC_HELPER				127 // adine beginner helper


///////////////// NATION ///////////////////////////////////
#define UNIFY_NATION		0
#define KARUS               1
#define ELMORAD             2
#define BATTLE				3

#define BATTLE_ZONE			101

////////////////////////////////////////////////////////////

// Attack Type
#define DIRECT_ATTACK		0
#define LONG_ATTACK			1
#define MAGIC_ATTACK		2
#define DURATION_ATTACK		3

////////////////// ETC Define //////////////////////////////
// UserInOut //
#define USER_IN					0X01
#define USER_OUT				0X02
#define USER_REGENE				0X03	// UserinÇÏ°í °°Àº°ÍÀÎµ¥ È¿°ú¸¦ ÁÖ±âÀ§ÇØ¼­.. ºĞ¸®(°ÔÀÓ½ÃÀÛ, ¸®Á¨. ¼ÒÈ¯½Ã)
#define USER_WARP				0X04
#define USER_SUMMON				0X05
#define NPC_IN					0X01
#define NPC_OUT					0X02

////////////////// Resurrection related ////////////////////
#define BLINK_TIME				10
#define CLAN_SUMMON_TIME		180
////////////////////////////////////////////////////////////

// Socket Define
////////////////////////////////////////////////////////////
#define SOCKET_BUFF_SIZE	(1024*16)
#define MAX_PACKET_SIZE		(1024*8)
#define REGION_BUFF_SIZE	(1024*16)

#define PACKET_START1				0XAA
#define PACKET_START2				0X55
#define PACKET_END1					0X55
#define PACKET_END2					0XAA

// status
#define STATE_CONNECTED			0X01
#define STATE_DISCONNECTED		0X02
#define STATE_GAMESTART			0x03

// Socket type
#define TYPE_ACCEPT				0x01
#define TYPE_CONNECT			0x02

// Overlapped flag
#define OVL_RECEIVE				0X01
#define OVL_SEND				0X02
#define OVL_CLOSE				0X03
////////////////////////////////////////////////////////////

// ==================================================================
//	About Map Object
// ==================================================================
#define USER_BAND				0			// Map À§¿¡ À¯Àú°¡ ÀÖ´Ù.
#define NPC_BAND				10000		// Map À§¿¡ NPC(¸÷Æ÷ÇÔ)°¡ ÀÖ´Ù.
#define INVALID_BAND			30000		// Àß¸øµÈ ID BAND

#define EVENT_MONSTER			20			// Event monster ÃÑ ¼ö

///////////////// snow event define //////////////////////////////
#define SNOW_EVENT_MONEY		2000
#define SNOW_EVENT_SKILL		490043

//////////////////////////////////////////////////////////////////
// DEFINE Shared Memory Queue
//////////////////////////////////////////////////////////////////

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
#define SMQ_INVALID		10006

// DEFINE Shared Memory Costumizing

#define MAX_PKTSIZE		512
#define MAX_COUNT		4096
#define SMQ_LOGGERSEND	"KNIGHT_SEND"
#define SMQ_LOGGERRECV	"KNIGHT_RECV"

#define SMQ_ITEMLOGGER	"ITEMLOG_SEND"

// Reply packet define...

#define SEND_ME					0x01
#define SEND_REGION				0x02
#define SEND_ALL				0x03
#define SEND_ZONE				0x04

// Battlezone Announcement
#define BATTLEZONE_OPEN					0x00
#define BATTLEZONE_CLOSE				0x01           
#define DECLARE_WINNER					0x02
#define DECLARE_LOSER					0x03
#define DECLARE_BAN						0x04
#define KARUS_CAPTAIN_NOTIFY			0x05
#define ELMORAD_CAPTAIN_NOTIFY			0x06
#define KARUS_CAPTAIN_DEPRIVE_NOTIFY	0x07
#define ELMORAD_CAPTAIN_DEPRIVE_NOTIFY	0x08
#define SNOW_BATTLEZONE_OPEN			0x09
#define GAME_SHOUTDOWN			0x10

// Battle define
#define NO_BATTLE				0
#define NATION_BATTLE			1
#define SNOW_BATTLE				2

// Zone IDs
#define ZONE_KARUS				1
#define ZONE_ELMORAD			2
#define ZONE_KARUS_ESLANT		11
#define ZONE_ELMORAD_ESLANT		12
#define ZONE_MORADON			21
#define ZONE_DELOS				30
#define ZONE_BIFROST			31
#define ZONE_DESPERATION_ABYSS	32
#define ZONE_HELL_ABYSS			33
#define ZONE_DRAGON_CAVE		34
#define ZONE_DELOS_CASTELLAN	35
#define ZONE_ARENA				48
#define ZONE_ORC_ARENA			51
#define ZONE_BLOOD_DON_ARENA	52
#define ZONE_GOBLIN_ARENA		53
#define ZONE_CAITHAROS_ARENA	54
#define ZONE_FORGOTTEN_TEMPLE	55
#define ZONE_LOST_TEMPLE		56
#define ZONE_BATTLE				101
#define ZONE_SNOW_BATTLE		102
#define	ZONE_FRONTIER			201



#define MAX_BATTLE_ZONE_USERS	150

#define MAX_ZONE 255
#define MAX_ZONE_SERVER 255

//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
typedef union{
	WORD		w;
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

typedef union{
	__int64		i;
	uint8_t		b[8];
} MYINT64;

struct _REGION_BUFFER {
	int		iLength;
	uint8_t	bFlag;
	DWORD	dwThreadID;
	
	char	pDataBuff[REGION_BUFF_SIZE];
	_REGION_BUFFER() {
		iLength = 0;
		bFlag = E;
		dwThreadID = 0;
		memset(pDataBuff, 0x00, REGION_BUFF_SIZE);
	};
};

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

inline float Getfloat(char* buf, int& index)
{
	float val;
	memcpy(&val, buf + index, sizeof(float));
	index += sizeof(float);
	return val;
}
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

	if (nSize == sizeof(BYTE))	nLen = GetByte(sBuf, index);
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
	_splitpath_s(Buf,drive,dir,fname,ext);
	strcpy_s(Path, drive);
	strcat_s(Path, dir);		
	CString _Path = Path;
	return _Path;
};
inline void LogFileGameServer(LPCTSTR logstr)
{
	CString strLogFile;
	CFile file;
	int loglength;

	// Mevcut zamanı alıyoruz
	CTime cur = CTime::GetCurrentTime();

	// Klasör yolunu ve dosya adını Gün-Ay-Yıl formatında oluşturuyoruz
	// Not: Log klasörünün mevcut olduğundan emin olun, yoksa dosya açılmaz.
	strLogFile.Format("Log/EbenezerLog-%d-%d-%d.txt",
		GetProgPath(), cur.GetYear(), cur.GetMonth(), cur.GetDay());

	loglength = strlen(logstr);

	// Dosyayı açıyoruz (Erişim modları senin verdiğin örnekle birebir aynı)
	if (file.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		file.SeekToEnd();
		file.Write(logstr, loglength);
		// Satır sonuna otomatik alt satıra geçme eklemek istersen:
		// file.Write("\r\n", 2); 
		file.Close();
	}
};
inline void LogFileSpeedHack(LPCTSTR logstr)
{
	CString strLogFile;
	CFile file;
	int loglength;

	// Mevcut zamanı alıyoruz
	CTime cur = CTime::GetCurrentTime();

	// Klasör yolunu ve dosya adını Gün-Ay-Yıl formatında oluşturuyoruz
	// Not: Log klasörünün mevcut olduğundan emin olun, yoksa dosya açılmaz.
	strLogFile.Format("Log/SpeedHackLog-%d-%d-%d.txt",
		GetProgPath(), cur.GetYear(), cur.GetMonth(), cur.GetDay());

	loglength = strlen(logstr);

	// Dosyayı açıyoruz (Erişim modları senin verdiğin örnekle birebir aynı)
	if (file.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		file.SeekToEnd();
		file.Write(logstr, loglength);
		// Satır sonuna otomatik alt satıra geçme eklemek istersen:
		// file.Write("\r\n", 2); 
		file.Close();
	}
};
inline void LogFileAISocket(LPCTSTR logstr)
{
	CString strLogFile;
	CFile file;
	int loglength;

	// Mevcut zamanı alıyoruz
	CTime cur = CTime::GetCurrentTime();

	// Klasör yolunu ve dosya adını Gün-Ay-Yıl formatında oluşturuyoruz
	// Not: Log klasörünün mevcut olduğundan emin olun, yoksa dosya açılmaz.
	strLogFile.Format("Log/AIServerLog-%d-%d-%d.txt",
		GetProgPath(), cur.GetYear(), cur.GetMonth(), cur.GetDay());

	loglength = strlen(logstr);

	// Dosyayı açıyoruz (Erişim modları senin verdiğin örnekle birebir aynı)
	if (file.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		file.SeekToEnd();
		file.Write(logstr, loglength);
		// Satır sonuna otomatik alt satıra geçme eklemek istersen:
		// file.Write("\r\n", 2); 
		file.Close();
	}
};
inline void LogFileReportBug(LPCTSTR logstr)
{
	CString strLogFile;
	CFile file;
	int loglength;

	// Mevcut zamanı alıyoruz
	CTime cur = CTime::GetCurrentTime();

	// Klasör yolunu ve dosya adını Gün-Ay-Yıl formatında oluşturuyoruz
	// Not: Log klasörünün mevcut olduğundan emin olun, yoksa dosya açılmaz.
	strLogFile.Format("Log/ReportBugLog-%d-%d-%d.txt",
		GetProgPath(), cur.GetYear(), cur.GetMonth(), cur.GetDay());

	loglength = strlen(logstr);

	// Dosyayı açıyoruz (Erişim modları senin verdiğin örnekle birebir aynı)
	if (file.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		file.SeekToEnd();
		file.Write(logstr, loglength);
		// Satır sonuna otomatik alt satıra geçme eklemek istersen:
		// file.Write("\r\n", 2); 
		file.Close();
	}
};
inline void LogFileLogin(LPCTSTR logstr)
{
	CString strLogFile;
	CFile file;
	int loglength;

	// Mevcut zamanı alıyoruz
	CTime cur = CTime::GetCurrentTime();

	// Klasör yolunu ve dosya adını Gün-Ay-Yıl formatında oluşturuyoruz
	// Not: Log klasörünün mevcut olduğundan emin olun, yoksa dosya açılmaz.
	strLogFile.Format("Log/LoginLog-%d-%d-%d.txt",
		GetProgPath(), cur.GetYear(), cur.GetMonth(), cur.GetDay());

	loglength = strlen(logstr);

	// Dosyayı açıyoruz (Erişim modları senin verdiğin örnekle birebir aynı)
	if (file.Open(strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		file.SeekToEnd();
		file.Write(logstr, loglength);
		// Satır sonuna otomatik alt satıra geçme eklemek istersen:
		// file.Write("\r\n", 2); 
		file.Close();
	}
};
inline void LogFileWrite(LPCTSTR logstr)
{
	CString ProgPath, LogFileName;
	CFile file;

	ProgPath = GetProgPath();
	LogFileName.Format("%s\\Ebenezer.log", ProgPath);

	// Şu anki zamanı al
	CTime time = CTime::GetCurrentTime();

	// Format: [YYYY-MM-DD HH:MM:SS]
	CString timeStr;
	timeStr.Format("[%04d-%02d-%02d %02d:%02d:%02d] ",
		time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond());

	// Final log string
	CString finalLog;
	finalLog.Format("%s%s\r\n", timeStr, logstr);

	// Dosyaya yaz
	if (file.Open(LogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		file.SeekToEnd();
		file.Write(finalLog, finalLog.GetLength());
		file.Close();
	}
}

inline void DisplayErrorMsg(SQLHANDLE hstmt)
{
	SQLCHAR       SqlState[6], Msg[1024];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc2;

	i = 1;
	while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
	{
		printf("*** %s, %d, %s, %d ***\n", SqlState,NativeError,Msg,MsgLen);

		i++;
	}
};

inline int myrand( int min, int max )
{
	if( min == max ) return min;
	if( min > max )
	{
		int temp = min;
		min = max;
		max = temp;
	}

	double gap = max - min + 1;
	double rrr = (double)RAND_MAX / gap;

	double rand_result;

	rand_result = (double)rand() / rrr;

	if( (int)( min + (int)rand_result ) < min ) return min;
	if( (int)( min + (int)rand_result ) > max ) return max;

	return (int)( min + (int)rand_result );
};

inline float TimeGet()
{
	static bool bInit = false;
	static bool bUseHWTimer = FALSE;
	static LARGE_INTEGER nTime, nFrequency;
	
	if(bInit == false)
	{
		if(TRUE == ::QueryPerformanceCounter(&nTime))
		{
			::QueryPerformanceFrequency(&nFrequency);
			bUseHWTimer = TRUE;
		}
		else 
		{
			bUseHWTimer = FALSE;
		}

		bInit = true;
	}

	if(bUseHWTimer)
	{
		::QueryPerformanceCounter(&nTime);
		return (float)((double)(nTime.QuadPart)/(double)nFrequency.QuadPart);
	}

	return (float)timeGetTime();
};

inline void	Timeprintf(TCHAR* pMsg)
{
	CString szMsg = _T("");
	CTime time = CTime::GetCurrentTime();
	szMsg.Format("%s,,  time : %d-%d-%d, %d:%d]\n", pMsg, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
	printf(szMsg);
};
inline void SetKOString(char* buf, const char* str, int& index, int lenType /*0=byte,1=short*/)
{
	if (str == nullptr)
	{
		if (lenType == 1)
			SetShort(buf, 0, index);
		else
			SetByte(buf, 0, index);
		return;
	}

	int len = (int)strlen(str);

	if (lenType == 1)
		SetShort(buf, len, index); // 2 byte length
	else
		SetByte(buf, len, index);  // 1 byte length

	memcpy(buf + index, str, len);
	index += len;
}
inline float GetFloat(char* buf, int& index)
{
	float val;
	memcpy(&val, buf + index, sizeof(float));
	index += sizeof(float);
	return val;
}
inline void GetKOString(char* dest, char* src, int& index)
{
	int len = *(BYTE*)&src[index];
	index++;

	if (len <= 0 || len > 64) { // max name size
		dest[0] = 0;
		return;
	}

	memcpy(dest, &src[index], len);
	dest[len] = 0;

	index += len;
}
#endif