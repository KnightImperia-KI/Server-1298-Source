// User.h: interface for the CUser class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_)
#define AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "IOCPSocket2.h"
#include "define.h"
#include "GameDefine.h"
#include "MagicProcess.h"
#include "Npc.h"
#include "EVENT.h"
#include "EVENT_DATA.h"
#include "LOGIC_ELSE.h"
#include "EXEC.h"     

#include <list>
typedef	 std::list<_EXCHANGE_ITEM*>		ItemList;
typedef  list<int>				UserEventList;	// ÀÌ¹êÆ®¸¦ À§ÇÏ¿© ^^;

#define BANISH_DELAY_TIME    30

class CEbenezerDlg;
class CUser : public CIOCPSocket2  
{
public:
	C3DMap* GetMap();
	_USER_DATA*	m_pUserData;
	short m_sid; // Socket ID deðerini tutacak olan deðiþken
	char	m_strAccountID[MAX_ID_SIZE+1];	// Login -> Select Char ±îÁö ÇÑ½ÃÀûÀ¸·Î¸¸ ¾²´Âº¯¼ö. ÀÌ¿Ü¿¡´Â _USER_DATA ¾È¿¡ÀÖ´Â º¯¼ö¸¦ ¾´´Ù...agent ¿ÍÀÇ µ¥ÀÌÅÍ µ¿±âÈ­¸¦ À§ÇØ...
	
	short	m_RegionX;						// ÇöÀç ¿µ¿ª X ÁÂÇ¥
	short	m_RegionZ;						// ÇöÀç ¿µ¿ª Z ÁÂÇ¥

	int		m_iMaxExp;						// ´ÙÀ½ ·¹º§ÀÌ µÇ±â À§ÇØ ÇÊ¿äÇÑ Exp·®
	short	m_sMaxWeight;					// µé ¼ö ÀÖ´Â ÃÖ´ë ¹«°Ô
	uint8_t    m_sSpeed;						// ½ºÇÇµå

	short	m_sBodyAc;						// ¸Ç¸ö ¹æ¾î·Â

	short	m_sTotalHit;					// ÃÑ Å¸°Ý°ø°Ý·Â	
	short	m_sTotalAc;						// ÃÑ ¹æ¾î·Â
	float	m_sTotalHitrate;				// ÃÑ °ø°Ý¼º°ø ¹ÎÃ¸¼º
	float	m_sTotalEvasionrate;			// ÃÑ ¹æ¾î ¹ÎÃ¸¼º

	short   m_sItemMaxHp;                   // ¾ÆÀÌÅÛ ÃÑ ÃÖ´ë HP Bonus
	short   m_sItemMaxMp;                   // ¾ÆÀÌÅÛ ÃÑ ÃÖ´ë MP Bonus
	short	m_sItemWeight;					// ¾ÆÀÌÅÛ ÃÑ¹«°Ô
	short	m_sItemHit;						// ¾ÆÀÌÅÛ ÃÑÅ¸°ÝÄ¡
	short	m_sItemAc;						// ¾ÆÀÌÅÛ ÃÑ¹æ¾î·Â
	short	m_sItemStr;						// ¾ÆÀÌÅÛ ÃÑÈû º¸³Ê½º
	short	m_sItemSta;						// ¾ÆÀÌÅÛ ÃÑÃ¼·Â º¸³Ê½º
	short	m_sItemDex;						// ¾ÆÀÌÅÛ ÃÑ¹ÎÃ¸¼º º¸³Ê½º
	short	m_sItemIntel;					// ¾ÆÀÌÅÛ ÃÑÁö´É º¸³Ê½º
	short	m_sItemCham;					// ¾ÆÀÌÅÛ ÃÑ¸Å·Âº¸³Ê½º
	short	m_sItemHitrate;					// ¾ÆÀÌÅÛ ÃÑÅ¸°ÝÀ²
	short	m_sItemEvasionrate;				// ¾ÆÀÌÅÛ ÃÑÈ¸ÇÇÀ²

	uint8_t	m_bFireR;						// ºÒ ¸¶¹ý ÀúÇ×·Â
	uint8_t	m_bColdR;						// ¾óÀ½ ¸¶¹ý ÀúÇ×·Â
	uint8_t	m_bLightningR;					// Àü±â ¸¶¹ý ÀúÇ×·Â
	uint8_t	m_bMagicR;						// ±âÅ¸ ¸¶¹ý ÀúÇ×·Â
	uint8_t	m_bDiseaseR;					// ÀúÁÖ ¸¶¹ý ÀúÇ×·Â
	uint8_t	m_bPoisonR;						// µ¶ ¸¶¹ý ÀúÇ×·Â

	uint8_t    m_bMagicTypeLeftHand;			// The type of magic item in user's left hand  
	uint8_t    m_bMagicTypeRightHand;			// The type of magic item in user's right hand
	short   m_sMagicAmountLeftHand;         // The amount of magic item in user's left hand
	short	m_sMagicAmountRightHand;        // The amount of magic item in user's left hand

	short   m_sDaggerR;						// Resistance to Dagger
	short   m_sSwordR;						// Resistance to Sword
	short	m_sAxeR;						// Resistance to Axe
	short	m_sMaceR;						// Resistance to Mace
	short	m_sSpearR;						// Resistance to Spear
	short	m_sBowR;						// Resistance to Bow		

	short	m_iMaxHp;
	short	m_iMaxMp;
	
	short	m_iZoneIndex;

	float	m_fWill_x;
	float	m_fWill_z;
	float	m_fWill_y;

	uint8_t	m_bResHpType;					// HP È¸º¹Å¸ÀÔ
	uint8_t	m_bWarp;						// Á¸ÀÌµ¿Áß...
	uint8_t	m_bNeedParty;					// ÆÄÆ¼....±¸ÇØ¿ä

	short	m_sPartyIndex;
	short	m_sExchangeUser;				// ±³È¯ÁßÀÎ À¯Àú
	uint8_t	m_bExchangeOK;

	ItemList	m_ExchangeItemList;
	_ITEM_DATA	m_MirrorItem[HAVE_MAX+EXTRA_MAX];			// ±³È¯½Ã ¹é¾÷ ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ ¾´´Ù.

	short	m_sPrivateChatUser;
	uint8_t	m_sPrivateChat;

	DWORD	m_i8MerchantItem[8];

	float	m_fHPLastTimeNormal;					// For Automatic HP recovery. 
	float	m_fHPStartTimeNormal;
	short	m_bHPAmountNormal;
	uint8_t	m_bHPDurationNormal;
	uint8_t	m_bHPIntervalNormal;

	float	m_fHPLastTime[MAX_TYPE3_REPEAT];		// For Automatic HP recovery and Type 3 durational HP recovery.
	float	m_fHPStartTime[MAX_TYPE3_REPEAT];
	short	m_bHPAmount[MAX_TYPE3_REPEAT];
	uint8_t	m_bHPDuration[MAX_TYPE3_REPEAT];
	uint8_t	m_bHPInterval[MAX_TYPE3_REPEAT];
	short	m_sSourceID[MAX_TYPE3_REPEAT];
	BOOL	m_bType3Flag;

	float	m_fAreaLastTime;			// For Area Damage spells Type 3.
	float   m_fAreaStartTime;
	uint8_t    m_bAreaInterval;
	int     m_iAreaMagicID;

	uint8_t	m_bAttackSpeedAmount;		// For Character stats in Type 4 Durational Spells.
	uint8_t    m_bSpeedAmount;
	short   m_sACAmount;
	uint8_t    m_bAttackAmount;
	short	m_sMaxHPAmount;
	uint8_t	m_bHitRateAmount;
	short	m_sAvoidRateAmount;
	uint8_t	m_bStrAmount;
	uint8_t	m_bStaAmount;
	uint8_t	m_bDexAmount;
	uint8_t	m_bIntelAmount;
	uint8_t	m_bChaAmount;
	uint8_t	m_bFireRAmount;
	uint8_t	m_bColdRAmount;
	uint8_t	m_bLightningRAmount;
	uint8_t	m_bMagicRAmount;
	uint8_t	m_bDiseaseRAmount;
	uint8_t	m_bPoisonRAmount;
	
	short   m_sDuration1 ;  float   m_fStartTime1 ;
	short   m_sDuration2 ;  float   m_fStartTime2 ;
	short   m_sDuration3 ;  float   m_fStartTime3 ;
	short   m_sDuration4 ;  float   m_fStartTime4 ;
	short   m_sDuration5 ;  float   m_fStartTime5 ;
	short   m_sDuration6 ;  float   m_fStartTime6 ;
	short   m_sDuration7 ;  float   m_fStartTime7 ;
	short   m_sDuration8 ;  float   m_fStartTime8 ;
	short   m_sDuration9 ;  float   m_fStartTime9 ;

	uint8_t	m_bType4Buff[MAX_TYPE4_BUFF];
	BOOL	m_bType4Flag;
		
	CEbenezerDlg* m_pMain;
	CMagicProcess m_MagicProcess;

	float	m_fSpeedHackClientTime, m_fSpeedHackServerTime;
	uint8_t	m_bSpeedHackCheck;

	short	m_sFriendUser;				// who are you trying to make friends with?

	float	m_fBlinkStartTime;			// When did you start to blink?

	short	m_sAliveCount;

	uint8_t	m_bAbnormalType;			// Is the player normal,a giant, or a dwarf?

	short	m_sWhoKilledMe;				// Who killed me???
	int		m_iLostExp;					// Experience point that was lost when you died.

	float	m_fLastTrapAreaTime;		// The last moment you were in the trap area.

	BOOL	m_bZoneChangeFlag;			// ¼º¿ë¾¾ ¹Ì¿ö!!

	uint8_t	m_bRegeneType;				// Did you die and go home or did you type '/town'?

	float	m_fLastRegeneTime;			// The last moment you got resurrected.

	BOOL	m_bZoneChangeSameZone;		// Did the server change when you warped?

	// ÀÌ¹êÆ®¿ë °ü·Ã.... Á¤¾Ö¾¾ ÀÌ°Å º¸¸é ÄÚÄ«½º ½ò²²¿ä ^^;
//	int					m_iSelMsgEvent[5];	// ½ÇÇàÁßÀÎ ¼±ÅÃ ¸Þ¼¼Áö¹Ú½º ÀÌº¥Æ®
	int					m_iSelMsgEvent[MAX_MESSAGE_EVENT];	// ½ÇÇàÁßÀÎ ¼±ÅÃ ¸Þ¼¼Áö¹Ú½º ÀÌº¥Æ®
	short				m_sEventNid;		// ¸¶Áö¸·À¸·Î ¼±ÅÃÇÑ ÀÌº¥Æ® NPC ¹øÈ£
	UserEventList		m_arUserEvent;		// ½ÇÇàÇÑ ÀÌº¥Æ® ¸®½ºÆ®

	char	m_strCouponId[MAX_COUPON_ID_LENGTH];		// What was the number of the coupon?
	int		m_iEditBoxEvent;

	short	m_sEvent[MAX_CURRENT_EVENT];				// ÀÌ¹Ì ½ÇÇàµÈ ÀÌ¹êÆ® ¸®½ºÆ®µé :)
	

public:
	__forceinline bool isBanned() { return getAuthority() == AUTHORITY_BANNED; };
	__forceinline bool isMuted() { return getAuthority() == AUTHORITY_MUTED; };
	__forceinline bool isAttackDisabled() { return getAuthority() == AUTHORITY_ATTACK_DISABLED; };
	__forceinline bool isGM() { return getAuthority() == AUTHORITY_GAME_MASTER; };
	__forceinline bool isLimitedGM() { return getAuthority() == AUTHORITY_LIMITED_GAME_MASTER; };

	__forceinline bool isDead() { return m_bResHpType == USER_DEAD || m_pUserData->m_sHp <= 0; };

	__forceinline bool isInParty() { return m_sPartyIndex != -1; };
	__forceinline bool isInClan() { return m_pUserData->m_bKnights != -1; };
	__forceinline bool isTrading() { return m_sExchangeUser != -1; };
	

	__forceinline BYTE getNation() { return m_pUserData->m_bNation; };
	__forceinline BYTE getLevel() { return m_pUserData->m_bLevel; };
	__forceinline BYTE getZoneID() { return m_pUserData->m_bZone; };
	__forceinline BYTE getAuthority() { return m_pUserData->m_bAuthority; };
	

	void RecvDeleteChar( char* pBuf );
	BOOL ExistComEvent(int eventid);
	void SaveComEvent(int eventid);
	BOOL CheckItemCount(int itemid, short min, short max);
	void CouponEvent( char* pBuf );
	void LogCoupon(int itemid, int count);
	void RecvEditBox(char *pBuf);
	BOOL CheckCouponUsed();
	BOOL CheckRandom(short percent);
	void OpenEditBox(int message, int event);
	BOOL CheckEditBox();
	void NativeZoneReturn();
	void EventMoneyItemGet( int itemid, int count );
	void KickOutZoneUser(BOOL home = FALSE, int nZoneID = 21);;
	void TrapProcess();
	BOOL JobGroupCheck(short jobgroupid);
	void SelectMsg(EXEC* pExec);
	void SendNpcSay(EXEC* pExec);
	BOOL CheckClass(short class1, short class2, short class3, short class4, short class5, short class6);
	void Make_public_key();
	void RecvSelectMsg(char *pBuf);
	BOOL GiveItem(int itemid, short count);
	BOOL RobItem(int itemid, short count);
	BOOL CheckExistItem(int itemid, short count);
	BOOL CheckWeight(int itemid, short count);
	BOOL CheckSkillPoint(BYTE skillnum, BYTE min, BYTE max);
	BOOL GoldLose(int gold);
	void GoldGain(int gold);
	void SendItemWeight();
	void ItemLogToAgent(const char *srcid, const char *tarid, int type, __int64 serial, int itemid, int count, int durability);
	void TestPacket( char* pBuf );
	BOOL RunEvent(EVENT_DATA *pEventData);
	BOOL RunNpcEvent(CNpc* pNpc, EXEC* pExec);
	BOOL CheckEventLogic(EVENT_DATA* pEventData);
	void ClientEvent(char* pBuf);
	void KickOut( char* pBuf );
	void SetLogInInfoToDB(BYTE bInit);
	void BlinkTimeCheck(float currenttime);
	void MarketBBSSellPostFilter();
	void MarketBBSBuyPostFilter();
	void MarketBBSMessage(char *pBuf);
	void MarketBBSSellDelete(short index);
	void MarketBBSBuyDelete(short index);
	void MarketBBSUserDelete();
	void MarketBBSTimeCheck();
	void MarketBBSRemotePurchase(char *pBuf);
	void MarketBBSReport(char *pBuf, BYTE type);
	void MarketBBSDelete(char *pBuf);
	void MarketBBSRegister(char *pBuf);
	void MarketBBS(char *pBuf);
	void PartyBBSNeeded(char *pBuf, BYTE type);
	void PartyBBSDelete(char *pBuf);
	void PartyBBSRegister(char *pBuf);
	void PartyBBS(char *pBuf);
	void Corpse();
	void FriendAccept(char *pBuf);
	void FriendRequest(char *pBuf);
	void Friend(char *pBuf);
	BOOL WarpListObjectEvent(short objectindex, short nid);
	BOOL FlagObjectEvent(short objectindex, short nid);
	BOOL GateLeverObjectEvent(short objectindex, short nid);
	BOOL GateObjectEvent(short objectindex, short nid);
	BOOL BindObjectEvent(short objectindex, short nid);
	void InitType3();
	BOOL GetWarpList( int warp_group );
	void ServerChangeOk( char* pBuf );
	void ZoneConCurrentUsers( char* pBuf );
	void SelectWarpList( char* pBuf );
	void GoldChange(short tid, int gold);
	void AllSkillPointChange();
	void AllPointChange();
	void ClassChangeReq();
	void FriendReport( char* pBuf );
	CUser* GetItemRoutingUser(int itemid, short itemcount);
	void Home();
	bool GetStartPosition(short& x, short& y, BYTE bZone = 0);
	void ReportBug(char *pBuf);
	int GetEmptySlot( int itemid, int bCountable );
	void InitType4();
	void WarehouseProcess( char* pBuf );
	short GetACDamage(int damage, short tid);
	short GetMagicDamage(int damage, short tid);
	void Type3AreaDuration( float currenttime);
	void ServerStatusCheck();
	void SpeedHackTime( char* pBuf );
	void OperatorCommand( char* pBuf );
	void ItemRemove( char* pBuf );
	void SendAllKnightsID();
	BYTE ItemCountChange(int itemid, int type, int amount);
	void Type4Duration(float currenttime);
	void ItemRepair( char* pBuf );
	int ExchangeDone();
	void HPTimeChange( float currenttime );
	void HPTimeChangeType3( float currenttime );
	void ItemDurationChange( int slot, int maxvalue, int curvalue, int amount );
	void ItemWoreOut( int type, int damage );
	void Dead();
	void LoyaltyDivide( short tid );
	void UserDataSaveToAgent();
	void CountConcurrentUser();
	void SendUserInfo(char *temp_send, int &index);
	void ChatTargetSelect( char* pBuf );
	BOOL ItemEquipAvailable( _ITEM_TABLE* pTable );
	void ClassChange( char* pBuf );
	void MSpChange(int amount);
	void UpdateGameWeather( char* pBuf, BYTE type );
	void ObjectEvent( char* pBuf );
	void NewEvent( char* pBuf );
	void EventJoin( char* pBuf );
	void KingProcess( char* pBuf );
	void SkillPointChange( char* pBuf );
	BOOL ExecuteExchange();
	void InitExchange(BOOL bStart);
	void ExchangeCancel();
	void ExchangeDecide();
	void ExchangeAdd( char* pBuf );
	void ExchangeAgree( char* pBuf );
	void ExchangeReq( char* pBuf );
	void ExchangeProcess( char* pBuf );
	void PartyDelete();
	void PartyRemove( int memberid );
	void PartyInsert();
	void PartyCancel();
	void PartyRequest( int memberid, BOOL bCreate );
	void PartyProcess( char* pBuf );
	void SendNotice();
	void SendPremium();
	void UserLookChange( int pos, int itemid, int durability );
	void SpeedHackUser();
	void VersionCheck();
	void LoyaltyChange( short tid );
	void StateChange( char* pBuf );
	void PointChange( char* pBuf );
	void ZoneChange( int zone, float x, float z );
	void ItemGet( char* pBuf );
	BOOL IsValidName( char* name );
	void AllCharInfoToAgent();
	void SelNationToAgent( char* pBuf );
	void DelCharToAgent( char* pBuf );
	void NewCharToAgent( char* pBuf );
	void BundleOpenReq( char* pBuf );
	void SendTargetHP( BYTE echo, int tid, int damage = 0 );
	void ItemTrade( char* pBuf );
	void NpcEvent( char* pBuf );
	BOOL IsValidSlotPos( _ITEM_TABLE* pTable, int destpos );
	void ItemMove( char* pBuf );
	void Warp( char* pBuf );
	void RequestNpcIn( char* pBuf );
	void SetUserAbility();
	void LevelChange(short level, BYTE type=TRUE);	// type : TRUE => level up, FALSE => level down
	void HpChange(int amount, int type=0, bool attack=false);
	short GetDamage(short tid, int magicid);
	void SetSlotItemValue();
	BYTE GetHitRate(float rate);
	void RequestUserIn( char* pBuf );
	void InsertRegion(int del_x, int del_z);
	void RemoveRegion( int del_x, int del_z );
	void RegisterRegion();
	void SetDetailData();
	void SendTimeStatus();
	void Regene(char* pBuf, int magicid = 0);
	void SetMaxMp();
	void SetMaxHp(int iFlag=0); // 0:default, 1:hp¸¦ maxhp¸¸Å­ Ã¤¿öÁÖ±â
	void ExpChange(int iExp);
	void Chat( char* pBuf );
	void StartShutdownSequence();
	void LogOut();
	void SelCharToAgent( char* pBuf );

	void Helmet( char* pBuf );
	void MiningProcess( char* pBuf );
	void MerchantProcess( char* pBuf );
	void QuestProcess( char* pBuf );
	void LetterToAgent( char* pBuf );
	void CheckNewLetter( char* pBuf );

	void SendMyInfo();
	void SelectCharacter( char* pBuf );
	void Send2AI_UserUpdateInfo();
	void Attack( char* pBuf );
	void UserInOut( BYTE Type );
	void Initialize();
	void MoveProcess( char* pBuf );
	void Rotate( char* pBuf );
	void LoginProcess( char* pBuf );
	void Parsing( int len, char* pData );
	void CloseProcess();
	CUser();
	virtual ~CUser();
};

#endif // !defined(AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_)
