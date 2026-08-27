#ifndef _GAMEDEFINE_H
#define _GAMEDEFINE_H

#define __VERSION			1298

#define INOUT_IN  1
#define INOUT_OUT 2

//////////////////// Á÷¾÷º° Define ////////////////////
#define KARUWARRRIOR		101		// Ä«·çÀü»ç
#define KARUROGUE			102		// Ä«·ç·Î±×
#define KARUWIZARD			103		// Ä«·ç¸¶¹ı
#define KARUPRIEST			104		// Ä«·ç»çÁ¦
#define BERSERKER			105		// ¹ö¼­Ä¿
#define GUARDIAN			106		// °¡µğ¾ğ
#define HUNTER				107		// ÇåÅÍ
#define PENETRATOR			108		// Æä³ÊÆ®·¹ÀÌÅÍ
#define SORSERER			109		// ¼Ò¼­·¯
#define NECROMANCER			110		// ³×Å©·Î¸Ç¼­
#define SHAMAN				111		// »ş¸¸
#define DARKPRIEST			112		// ´ÙÅ©ÇÁ¸®½ºÆ®

#define ELMORWARRRIOR		201		// ¿¤¸ğÀü»ç
#define ELMOROGUE			202		// ¿¤¸ğ·Î±×
#define ELMOWIZARD			203		// ¿¤¸ğ¸¶¹ı
#define ELMOPRIEST			204		// ¿¤¸ğ»çÁ¦
#define BLADE				205		// ºí·¹ÀÌµå
#define PROTECTOR			206		// ÇÁ·ÎÅØÅÍ
#define RANGER				207		// ·¹ÀÎÁ®
#define ASSASSIN			208		// ¾î½Ø½Å
#define MAGE				209		// ¸ŞÀÌÁö
#define ENCHANTER			210		// ¿£Ã¾ÅÍ
#define CLERIC				211		// Å¬·¹¸¯
#define DRUID				212		// µå·çÀÌµå
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// Race Define
/////////////////////////////////////////////////////
#define KARUS_BIG			1
#define KARUS_MIDDLE		2
#define KARUS_SMALL			3
#define KARUS_WOMAN			4
#define BABARIAN			11
#define ELMORAD_MAN			12
#define ELMORAD_WOMAN		13

/////////////////////////////////////////////////////
// ITEM_SLOT DEFINE
const uint8_t RIGHTEAR			= 0;
const uint8_t HEAD				= 1;
const uint8_t LEFTEAR			= 2;
const uint8_t NECK				= 3;
const uint8_t BREAST			= 4;
const uint8_t SHOULDER			= 5;
const uint8_t RIGHTHAND		= 6;
const uint8_t WAIST			= 7;
const uint8_t LEFTHAND			= 8;
const uint8_t RIGHTRING		= 9;
const uint8_t LEG				= 10;
const uint8_t LEFTRING			= 11;
const uint8_t GLOVE			= 12;
const uint8_t FOOT				= 13;
const uint8_t RESERVED			= 14;

/////////////////////////////////////////////////////
// COSPRE ITEM
const uint8_t CWING			= 43;
const uint8_t CHELMET			= 44;
const uint8_t CLEFT			= 45;
const uint8_t CRIGHT			= 46;
const uint8_t CTOP				= 47;
const uint8_t BAG1				= 48;
const uint8_t BAG2				= 49;

/////////////////////////////////////////////////////

// Å¸°İºñº° ¼º°ø·ü //
#define GREAT_SUCCESS			0X01		// ´ë¼º°ø
#define SUCCESS					0X02		// ¼º°ø
#define NORMAL					0X03		// º¸Åë
#define	FAIL					0X04		// ½ÇÆĞ 

// Item Move Direction Define 
#define ITEM_INVEN_SLOT			0x01
#define ITEM_SLOT_INVEN			0x02
#define ITEM_INVEN_INVEN		0x03
#define ITEM_SLOT_SLOT			0x04
#define ITEM_INVEN_ZONE			0x05
#define ITEM_ZONE_INVEN			0x06

// Item Weapon Type Define
#define WEAPON_DAGGER			1
#define WEAPON_SWORD			2
#define WEAPON_AXE				3
#define WEAPON_MACE				4
#define WEAPON_SPEAR			5
#define WEAPON_SHIELD			6
#define WEAPON_BOW				7
#define WEAPON_LONGBOW			8
#define WEAPON_LAUNCHER			10
#define WEAPON_STAFF			11
#define WEAPON_ARROW			12	// ½ºÅ³ ¾øÀ½
#define WEAPON_JAVELIN			13	// ½ºÅ³ ¾øÀ½
#define WEAPON_WORRIOR_AC		21	// ½ºÅ³ ¾øÀ½
#define WEAPON_LOG_AC			22	// ½ºÅ³ ¾øÀ½
#define WEAPON_WIZARD_AC		23	// ½ºÅ³ ¾øÀ½
#define WEAPON_PRIEST_AC		24	// ½ºÅ³ ¾øÀ½
////////////////////////////////////////////////////////////
// User Status //
#define USER_STANDING			0X01		// ¼­ ÀÖ´Ù.
#define USER_SITDOWN			0X02		// ¾É¾Æ ÀÖ´Ù.
#define USER_DEAD				0x03		// µà°Å¶°
#define USER_BLINKING			0x04		// ¹æ±İ »ì¾Æ³µ¾î!!!
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Magic State
#define NONE				0x01
#define CASTING				0x02
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Durability Type
#define ATTACK				0x01
#define DEFENCE				0x02
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Knights Authority Type
/*
#define CHIEF				0x06
#define VICECHIEF			0x05*/
#define OFFICER				0x04
#define KNIGHT				0x03
//#define TRAINEE				0x02
#define PUNISH				0x01	

#define CHIEF				0x01	// ´ÜÀå
#define VICECHIEF			0x02	// ºÎ´ÜÀå
#define TRAINEE				0x05	// ¸â¹ö
#define COMMAND_CAPTAIN		100		// ÁöÈÖ±ÇÀÚ
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// COMMUNITY TYPE DEFINE
#define CLAN_TYPE			0x01
#define KNIGHTS_TYPE		0x02
////////////////////////////////////////////////////////////

#define MAX_CLAN			24
#define MAX_KNIGHTS_BANK	200
#define MAX_KNIGHTS_MARK	512

#define ITEM_GOLD			900000000	// µ· ¾ÆÀÌÅÛ ¹øÈ£...
#define ITEM_NO_TRADE		900000001	// °Å·¡ ºÒ°¡ ¾ÆÀÌÅÛµé.... ºñ·¯¸Ó±Û Å©¸®½º¸¶½º ÀÌ¹êÆ® >.<		

////////////////////////////////////////////////////////////
// EVENT TYPE DEFINE
#define ZONE_CHANGE			0x01
#define ZONE_TRAP_DEAD		0x02
#define ZONE_TRAP_AREA		0x03

////////////////////////////////////////////////////////////
// EVENT MISCELLANOUS DATA DEFINE
#define ZONE_TRAP_INTERVAL	   1		// Interval is one second right now.
#define ZONE_TRAP_DAMAGE	   10		// HP Damage is 10 for now :)

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// USER POINT DEFINE
#define STR					0x01
#define STA					0x02
#define DEX					0x03
#define INTEL				0x04
#define CHA					0x05

#define ORDER_SKILL			0x01
#define MANNER_SKILL		0X02
#define LANGUAGE_SKILL		0x03
#define BATTLE_SKILL		0x04
#define PRO_SKILL1			0x05
#define PRO_SKILL2			0x06
#define PRO_SKILL3			0x07
#define PRO_SKILL4			0x08

/////////////////////////////////////////////////////////////
// ITEM TYPE DEFINE
#define ITEM_TYPE_FIRE				0x01
#define ITEM_TYPE_COLD				0x02
#define ITEM_TYPE_LIGHTNING			0x03
#define ITEM_TYPE_POISON			0x04
#define ITEM_TYPE_HP_DRAIN			0x05
#define ITEM_TYPE_MP_DAMAGE			0x06
#define ITEM_TYPE_MP_DRAIN			0x07
#define ITEM_TYPE_MIRROR_DAMAGE		0x08

/////////////////////////////////////////////////////////////
// ITEM LOG TYPE 
#define ITEM_MERCHANT_BUY			0x01
#define ITEM_MERCHANT_SELL			0x02
#define ITEM_MONSTER_GET			0x03
#define ITEM_EXCHANGE_PUT			0x04
#define ITEM_EXCHANGE_GET			0x05
#define ITEM_DESTROY				0x06
#define ITEM_WAREHOUSE_PUT			0x07
#define ITEM_WAREHOUSE_GET			0x08
#define ITEM_UPGRADE				0x09

/////////////////////////////////////////////////////////////
// JOB GROUP TYPES
#define GROUP_WARRIOR				1
#define GROUP_ROGUE					2
#define GROUP_MAGE					3
#define GROUP_CLERIC				4
#define GROUP_ATTACK_WARRIOR		5
#define GROUP_DEFENSE_WARRIOR		6
#define GROUP_ARCHERER				7
#define GROUP_ASSASSIN				8
#define GROUP_ATTACK_MAGE			9
#define GROUP_PET_MAGE				10
#define GROUP_HEAL_CLERIC			11
#define GROUP_CURSE_CLERIC			12

//////////////////////////////////////////////////////////////
// USER ABNORMAL STATUS TYPES
#define ABNORMAL_NORMAL			0x01
#define ABNORMAL_GIANT			0x02
#define ABNORMAL_DWARF			0x03
#define ABNORMAL_BLINKING		0x04

//////////////////////////////////////////////////////////////
// Object Type
#define NORMAL_OBJECT		0
#define SPECIAL_OBJECT		1

//////////////////////////////////////////////////////////////
// REGENE TYPES
#define REGENE_NORMAL		0
#define REGENE_MAGIC		1
#define REGENE_ZONECHANGE	2

//////////////////////////////////////////////////////////////
// TYPE 3 ATTRIBUTE TYPES
#define ATTRIBUTE_FIRE				1
#define ATTRIBUTE_ICE				2
#define ATTRIBUTE_LIGHTNING			3

//extern CRITICAL_SECTION g_LogFile_critical;

struct _CLASS_COEFFICIENT
{
	short	sClassNum;
	char	strClassName[30];
	float	ShortSword;
	float	Sword;
	float	Axe;
	float	Club;
	float	Spear;
	float	Pole;
	float	Staff;
	float	Bow;
	float	HP;
	float	MP;
	float	SP;
	float	AC;
	float	Hitrate;
	float	Evasionrate;
};

struct _ZONE_ITEM {		// Bundle unit
	DWORD bundle_index;
	int itemid[6];
	short count[6];
	float x;
	float z;
	float y;
	float time;
};

struct	_ITEM_DATA
{
	int		nNum;		// item ¹øÈ£
	short	sDuration;	// item ³»±¸·Â
	short	sCount;		// item °¹¼ö or item Ãàº¹ ¼Ó¼º¿¡ ÇØ´ç °ª
	__int64	nSerialNum;	// item serial code
};

struct	_EXCHANGE_ITEM
{
	int itemid;
	int count;
	short duration;
	BYTE pos;			//  ±³È¯ÈÄ µé¾î°¥ ÀÚ¸®..
	__int64	nSerialNum;	// item serial code
};

struct _ITEM_TABLE
{
	int   m_iNum;				// item num
	char  m_strName[50];		// item Name
	uint8_t  m_bKind;				// item Á¾·ù
	uint8_t  m_bSlot;				// ÀåÂøÀ§Ä¡
	uint8_t  m_bRace;				// »ç¿ë °¡´ÉÇÑ Á¾Á·
	uint8_t  m_bClass;				// »ç¿ë °¡´ÉÇÑ Class
	short m_sDamage;			// ÃÖ´ë Å¸°İÄ¡
	short m_sDelay;				// °ø°İ½Ã°£
	short m_sRange;				// ¹üÀ§(À¯È¿°Å¸®)
	short m_sWeight;			// ¹«°Ô
	short m_sDuration;			// ³»±¸¼º
	int	  m_iBuyPrice;			// À¯Àú°¡ »ç´Â°¡°İ
	int	  m_iSellPrice;			// À¯Àú°¡ ÆÄ´Â°¡°İ
	short m_sAc;				// ¹æ¾î·Â
	uint8_t  m_bCountable;			// °³¼ö °³³ä ¾ÆÀÌÅÛ
	int	  m_iEffect1;			// ¸ÅÁ÷ ÀÌÆåÆ®1
	int	  m_iEffect2;			// ¸ÅÁ÷ ÀÌÆåÆ®2
	uint8_t  m_bReqLevel;			// ¿ä±¸ ·¹º§
	uint8_t  m_bReqRank;			// ¿ä±¸ ÀÛÀ§
	uint8_t  m_bReqTitle;			// ¿ä±¸ ÁöÀ§
	uint8_t  m_bReqStr;			// ¿ä±¸ Èû
	uint8_t  m_bReqSta;			// ¿ä±¸ Ã¼·Â
	uint8_t  m_bReqDex;			// ¿ä±¸ ¹ÎÃ¸
	uint8_t  m_bReqIntel;			// ¿ä±¸ Áö´É
	uint8_t  m_bReqCha;			// ¿ä±¸ ¸Å·Â
	uint8_t  m_bSellingGroup;		// »óÀÎ Ãë±Ş ¹°Ç°
	uint8_t  m_ItemType;			// ¸ÅÁ÷¾ÆÀÌÅÛ ¶Ç´Â ·¹¾î¾ÆÀÌÅÛ
	short m_sHitrate;			// Å¸°İ·ü
	short m_sEvarate;			// È¸ÇÇÀ²
	short m_sDaggerAc;			// ¹æ¾î·Â1
	short m_sSwordAc;			// ¹æ¾î·Â2
	short m_sMaceAc;			// ¹æ¾î·Â3
	short m_sAxeAc;				// ¹æ¾î·Â4
	short m_sSpearAc;			// ¹æ¾î·Â5
	short m_sBowAc;				// ¹æ¾î·Â6
	uint8_t  m_bFireDamage;		// ºÒ ¼Ó¼º
	uint8_t  m_bIceDamage;			// ³Ã±â ¼Ó¼º
	uint8_t  m_bLightningDamage;	// Àü°İ ¼Ó¼º
	uint8_t  m_bPoisonDamage;		// µ¶ ¼Ó¼º
	uint8_t  m_bHPDrain;			// HP Èí¼ö
	uint8_t  m_bMPDamage;			// MP Å¸°İ
	uint8_t  m_bMPDrain;			// MP Èí¼ö
	uint8_t  m_bMirrorDamage;		// ¹İ»ç Å¸°İ
	uint8_t  m_bDroprate;			// µå·Ó ºñÀ²
	uint8_t  m_bStrB;				// Èû º¸³Ê½º
	uint8_t  m_bStaB;				// Ã¼·Â º¸³Ê½º
	uint8_t  m_bDexB;				// ¹ÎÃ¸¼º º¸³Ê½º
	uint8_t  m_bIntelB;			// Áö´É º¸³Ê½º
	uint8_t  m_bChaB;				// ¸Å·Â º¸³Ê½º
	short m_MaxHpB;				// MaxHP add
	short m_MaxMpB;				// MaxMP add
	uint8_t  m_bFireR;				// ºÒ ¸¶¹ı ÀúÇ×·Â
	uint8_t  m_bColdR;				// ¾óÀ½ ¸¶¹ı ÀúÇ×·Â
	uint8_t  m_bLightningR;		// Àü±â ¸¶¹ı ÀúÇ×·Â
	uint8_t  m_bMagicR;			// ±âÅ¸ ¸¶¹ı ÀúÇ×·Â
	uint8_t  m_bPoisonR;			// µ¶ ¸¶¹ı ÀúÇ×·Â
	uint8_t  m_bCurseR;			// ÀúÁÖ ¸¶¹ı ÀúÇ×·Â
};

#pragma pack(push, 1)
struct _USER_DATA
{
	char	m_id[MAX_ID_SIZE + 1];
	char	m_Accountid[MAX_ID_SIZE + 1];

	uint8_t	m_bZone;
	float	m_curx;
	float	m_curz;
	float	m_cury;

	uint8_t	m_bNation;
	uint8_t	m_bRace;
	short	m_sClass;
	uint8_t	m_bHairColor;
	uint8_t	m_bRank;
	uint8_t	m_bTitle;
	uint8_t	m_bLevel;
	int		m_iExp;
	int		m_iLoyalty;

	// 🔥 BURADAKİ m_iLoyaltyMonthly SİLİNDİ (Aujard'da en sonda olduğu için)

	uint8_t	m_bFace;
	uint8_t	m_bCity;
	short	m_bKnights;
	uint8_t	m_bFame;
	short	m_sHp;
	short	m_sMp;
	short	m_sSp;
	uint8_t	m_bStr;
	uint8_t	m_bSta;
	uint8_t	m_bDex;
	uint8_t	m_bIntel;
	uint8_t	m_bCha;
	uint8_t	m_bAuthority;
	uint8_t	m_bPoints;
	int		m_iGold;
	short	m_sBind;
	int		m_iBank;

	uint8_t	m_bstrSkill[10]; // 🔥 Aujard ile aynı olması için 10 yapıldı

	_ITEM_DATA m_sItemArray[HAVE_MAX + SLOT_MAX + EXTRA_MAX];
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];

	uint8_t	m_bLogout;
	uint8_t	m_bWarehouse;
	DWORD	m_dwTime;

	// --- AUJARD İLE AYNI SIRADA OLMASI İÇİN EN SONA EKLENDİ ---
	short	m_sQuestCount;
	uint8_t	m_strQuest[400];
	int		m_iMannerPoint;
	int		m_iLoyaltyMonthly;
};

#pragma pack(pop)

struct _LEVELUP
{
	short	m_sLevel;
	int		m_iExp;
};

struct	_PARTY_GROUP
{
	WORD wIndex;
	short uid[8];		// ÇÏ³ªÀÇ ÆÄÆ¼¿¡ 8¸í±îÁö °¡ÀÔ°¡´É
	short sMaxHp[8];
	short sHp[8];
	uint8_t bLevel[8];
	short sClass[8];
	uint8_t bItemRouting;
	_PARTY_GROUP() {
		for(int i=0;i<8;i++) {
			uid[i] = -1; sMaxHp[i] = 0; sHp[i] = 0; bLevel[i] = 0; sClass[i] = 0;
		}
		bItemRouting = 0;
	};
};

struct _OBJECT_EVENT
{
	uint8_t byLife;			// 1:»ì¾ÆÀÖ´Ù, 0:ÄÊ,, Á×À½
	int sBelong;			// ¼Ò¼Ó
	short sIndex;			// 100 ¹ø´ë - Ä«·ç½º ¹ÙÀÎµå Æ÷ÀÎÆ® | 200 ¹ø´ë ¿¤¸ğ¶óµå ¹ÙÀÎµå Æ÷ÀÎÆ® | 1100 ¹ø´ë - Ä«·ç½º ¼º¹®µé 1200 - ¿¤¸ğ¶óµå ¼º¹®µé
	short sType;			// 0 - ¹ÙÀÎµå Æ÷ÀÎÆ®, 1 - ÁÂ¿ì·Î ¿­¸®´Â ¼º¹®, 2 - »óÇÏ·Î ¿­¸®´Â ¼º¹®, 3 - ·¹¹ö, 4 - ±ê¹ß·¹¹ö, 6:Ã¶Ã¢, 7-±úÁö´Â ºÎÈ°ºñ¼®
	short sControlNpcID;	// Á¶Á¾ÇÒ NPC ID (Á¶Á¾ÇÒ Object Index), Type-> 5 : Warp Group ID
	short sStatus;			// status
	float fPosX;			// À§Ä¡°ª
	float fPosY;
	float fPosZ;
};

struct _REGENE_EVENT
{
	int	  sRegenePoint;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ª ¹øÈ£
	float fRegenePosX;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ªÀÇ ¿Ş¾Æ·¡ÂÊ ±¸¼® ÁÂÇ¥ X
	float fRegenePosY;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ªÀÇ ¿Ş¾Æ·¡ÂÊ ±¸¼® ÁÂÇ¥ Y
	float fRegenePosZ;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ªÀÇ ¿Ş¾Æ·¡ÂÊ ±¸¼® ÁÂÇ¥ Z
	float fRegeneAreaZ;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ªÀÇ Z Ãà ±æÀÌ 
	float fRegeneAreaX;		// Ä³¸¯ÅÍ ³ªÅ¸³ª´Â Áö¿ªÀÇ X Ãà ±æÀÌ
};

struct _KNIGHTS_USER
{
	uint8_t    byUsed;								// »ç¿ëÁß : 1, ºñ»ç¿ëÁß : 0
	char	strUserName[MAX_ID_SIZE+1];			// Ä³¸¯ÅÍÀÇ ÀÌ¸§
};

struct _MAGIC_TABLE
{
	int		iNum;
	short	sFlyingEffect;
	uint8_t	bMoral;
	uint8_t	bSkillLevel;
	short	sSkill;
	short	sMsp;
	short   sHP;
	BYTE	bItemGroup;
	int		iUseItem;
	uint8_t	bCastTime;
	uint8_t	bReCastTime;
	uint8_t	bSuccessRate;
	uint8_t	bType1;
	uint8_t	bType2;
	short   sRange;
	uint8_t	bEtc;
};

struct _MAGIC_TYPE8
{
	int     iNum;
	BYTE    bTarget;
	short   sRadius;
	BYTE    bWarpType;
	short   sExpRecover;
};

struct _MAGIC_TYPE5
{
	int		iNum;
	BYTE	bType;
	BYTE	bExpRecover;
	short	sNeedStone;
};

struct _MAGIC_TYPE4
{
	int     iNum;
	short   sMSP;
	BYTE    bBuffType;
	BYTE    bRadius;
	short   sDuration;
	BYTE    bAttackSpeed;
	BYTE    bSpeed;
	short   sAC;
	BYTE    bAttack;
	short   sMaxHP;
	BYTE    bHitRate;
	short   sAvoidRate;
	BYTE    bStr;
	BYTE    bSta;
	BYTE    bDex;
	BYTE    bIntel;
	BYTE    bCha;
	BYTE    bFireR;
	BYTE    bColdR;
	BYTE    bLightningR;
	BYTE    bMagicR;
	BYTE    bDiseaseR;
	BYTE    bPoisonR;
};

struct _MAGIC_TYPE3
{
	int		iNum;
	BYTE	bRadius;
	short	sAngle;
	short	sFirstDamage;
	short	sEndDamage;
	short	sTimeDamage;
	BYTE	bDirectType;
	short	sDuration;
	BYTE	bAttribute;
};

struct _MAGIC_TYPE2
{
	int     iNum;
	BYTE    bHitType;
	short   sHitRate;
	short	sAddDamage;
	short   sAddRange;
	BYTE    bNeedArrow;
};

struct _MAGIC_TYPE1
{
	int		iNum;
	BYTE	bHitType;
	short	sHitRate;
	short	sHit;
	BYTE	bDelay;
	BYTE	bComboType;
	BYTE	bComboCount;
	short	sComboDamage;
	short	sRange;
};

struct _ZONE_SERVERINFO
{
	short		sServerNo;
	short		sPort;
	char		strServerIP[20];
	_ZONE_SERVERINFO() {memset(strServerIP, 0x00, 20);};
};

#pragma pack(push, 1)
// 1298/9 WARP_INFO must match the client SMD layout byte-for-byte.
// The client uses a 256-byte announcement field followed by padding fields.
// The old server struct used 258 bytes here and read the following fields at
// different offsets, which can corrupt warp ID/zone/price/coordinate data when
// loading the SMD warp table.
struct _WARP_INFO
{
	short	sWarpID;
	char	strWarpName[32];
	char	strAnnounce[256];
	unsigned short	sUnk0;
	unsigned long	dwPay;
	short	sZone;
	unsigned short	sUnk1;
	float	fX;
	float	fY;
	float	fZ;
	float	fR;
	short	sNation;
	unsigned short	sUnk2;

	_WARP_INFO() {
		memset(this, 0, sizeof(_WARP_INFO));
	};
};
#pragma pack(pop)

static_assert(sizeof(_WARP_INFO) == 320, "1298 WARP_INFO layout mismatch");

struct _START_POSITION
{
	int   ZoneID;
	short sKarusX;
	short sKarusZ;
	short sElmoradX;
	short sElmoradZ;
	short sKarusGateX;
	short sKarusGateZ;
	short sElmoradGateX;
	short sElmoradGateZ;
	uint8_t bRangeX;
	uint8_t bRangeZ;
};

struct _SERVER_RESOURCE
{
	int nResourceID;
	char strResource[255];
	_SERVER_RESOURCE()
	{
		memset(strResource, NULL, 255);
	};
};

struct _HOME_INFO
{
	uint8_t	bNation;
	float	ElmoZoneX;
	float	ElmoZoneZ;
	uint8_t	ElmoZoneLX;
	uint8_t	ElmoZoneLZ;
	float	KarusZoneX;
	float	KarusZoneZ;
	uint8_t	KarusZoneLX;
	uint8_t	KarusZoneLZ;
	float	FreeZoneX;
	float	FreeZoneZ;
	uint8_t	FreeZoneLX;
	uint8_t	FreeZoneLZ;
//
	float	BattleZoneX;
	float	BattleZoneZ;
	uint8_t	BattleZoneLX;
	uint8_t	BattleZoneLZ;
//
};

enum AuthorityTypes
{
	AUTHORITY_GAME_MASTER = 0,
	AUTHORITY_PLAYER = 1,
	AUTHORITY_MUTED = 11,
	AUTHORITY_ATTACK_DISABLED = 12,
	AUTHORITY_LIMITED_GAME_MASTER = 250,
	AUTHORITY_BANNED = 255
};


#endif