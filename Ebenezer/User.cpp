// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "EbenezerDlg.h"
#include "User.h"
#include "AiPacket.h"
#include "Map.h"
#include "PacketDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION g_region_critical;
extern CRITICAL_SECTION g_LogFile_critical;
extern BYTE g_serverdown_flag;

// Cryption


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{
}

CUser::~CUser()
{
}

void CUser::Initialize()
{
	// m_pMain atamasını en güvenli şekilde yapıyoruz
	m_pMain = (CEbenezerDlg*)AfxGetApp()->m_pMainWnd;
	if (m_pMain == NULL) m_pMain = (CEbenezerDlg*)AfxGetMainWnd();

	// Cryption
	Make_public_key();
	jct.SetPublicKey(m_Public_key);
	jct.SetPrivateKey(g_private_key);
	jct.Init();

	m_CryptionFlag = 0; // 0 olmalı (Şifreleme başlamasın)
	m_Sen_val = 0;
	m_Rec_val = 0;
	m_Public_key = 0;



	// Magic süreci için m_pMain aktarımı
	if (m_pMain != NULL) {
		m_MagicProcess.m_pMain = m_pMain;
	}
	m_MagicProcess.m_pSrcUser = this;

	// Değişken sıfırlamaları
	m_RegionX = -1;
	m_RegionZ = -1;
	m_sBodyAc = 0;
	m_sTotalHit = 0;
	m_sTotalAc = 0;
	m_sTotalHitrate = 0;
	m_sTotalEvasionrate = 0;
	m_sItemMaxHp = 0;
	m_sItemMaxMp = 0;
	m_sItemWeight = 0;
	m_sItemHit = 0;
	m_sItemAc = 0;
	m_sItemStr = 0;
	m_sItemSta = 0;
	m_sItemDex = 0;
	m_sItemIntel = 0;
	m_sItemCham = 0;
	m_sItemHitrate = 100;
	m_sItemEvasionrate = 100;
	m_sSpeed = 0;
	m_iMaxHp = 0;
	m_iMaxMp = 1;
	m_iMaxExp = 0;
	m_sMaxWeight = 0;
	m_bFireR = 0;
	m_bColdR = 0;
	m_bLightningR = 0;
	m_bMagicR = 0;
	m_bDiseaseR = 0;
	m_bPoisonR = 0;
	m_sDaggerR = 0;
	m_sSwordR = 0;
	m_sAxeR = 0;
	m_sMaceR = 0;
	m_sSpearR = 0;
	m_sBowR = 0;
	m_bMagicTypeLeftHand = 0;
	m_bMagicTypeRightHand = 0;
	m_sMagicAmountLeftHand = 0;
	m_sMagicAmountRightHand = 0;
	m_iZoneIndex = 0;
	m_bResHpType = USER_STANDING;
	m_bWarp = 0x00;
	m_sPartyIndex = -1;
	m_sExchangeUser = -1;
	m_bExchangeOK = 0x00;
	m_sPrivateChatUser = -1;
	m_sPrivateChat = 0x00;
	m_bNeedParty = 0x01;
	m_fHPLastTimeNormal = 0.0f;
	m_fHPStartTimeNormal = 0.0f;
	m_bHPAmountNormal = 0;
	m_bHPDurationNormal = 0;
	m_bHPIntervalNormal = 5;
	m_fAreaLastTime = 0.0f;
	m_fAreaStartTime = 0.0f;
	m_bAreaInterval = 5;
	m_iAreaMagicID = 0;
	m_sFriendUser = -1;

	InitType3();
	InitType4();

	m_fSpeedHackClientTime = 0.0f;
	m_fSpeedHackServerTime = 0.0f;
	m_bSpeedHackCheck = 0;
	m_fBlinkStartTime = 0.0f;
	m_sAliveCount = 0;
	m_bAbnormalType = 1;
	m_sWhoKilledMe = -1;
	m_iLostExp = 0;
	m_fLastTrapAreaTime = 0.0f;

	memset(m_strAccountID, 0x00, MAX_ID_SIZE + 1);

	for (int i = 0; i < MAX_MESSAGE_EVENT; i++) {
		m_iSelMsgEvent[i] = -1;
	}

	m_sEventNid = -1;
	m_bZoneChangeFlag = FALSE;
	m_bRegeneType = 0;
	m_fLastRegeneTime = 0.0f;
	m_bZoneChangeSameZone = FALSE;

	memset(m_strCouponId, 0x00, MAX_COUPON_ID_LENGTH);
	m_iEditBoxEvent = -1;

	for (int j = 0; j < MAX_CURRENT_EVENT; j++) {
		m_sEvent[j] = -1;
	}

	while (!m_arUserEvent.empty())
		m_arUserEvent.pop_back();

	CIOCPSocket2::Initialize();
}


// Cryption
void CUser::Make_public_key()
{
	BYTE rand1, rand2, rand3, rand4, rand5, rand6, rand7, rand8;

	int out_flag = 0;
	do
	{
		rand1 = rand();
		rand2 = rand();
		rand3 = rand();
		rand4 = rand();
		rand5 = rand();
		rand6 = rand();
		rand7 = rand();
		rand8 = rand();

		m_Public_key = rand1;
		m_Public_key <<= 8;

		m_Public_key |= rand2;
		m_Public_key <<= 8;

		m_Public_key |= rand3;
		m_Public_key <<= 8;

		m_Public_key |= rand4;
		m_Public_key <<= 8;

		m_Public_key |= rand5;
		m_Public_key <<= 8;

		m_Public_key |= rand6;
		m_Public_key <<= 8;

		m_Public_key |= rand7;
		m_Public_key <<= 8;

		m_Public_key |= rand8;

		if (m_Public_key != 0)
			out_flag = 1;

	} while (!out_flag);
}

///~

void CUser::CloseProcess()
{
	UserInOut( USER_OUT );
	if( m_sPartyIndex != -1 )
		PartyRemove(m_Sid);
	if( m_sExchangeUser != -1 )
		ExchangeCancel();

/* ºÎµğ Àß ÀÛµ¿ÇÏ±æ ¤Ğ.¤Ğ
	if (!m_bZoneChangeFlag) {
		if (m_pUserData->m_bZone == ZONE_BATTLE || (m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3) ) {	
			_HOME_INFO* pHomeInfo = NULL;	// Send user back home in case it was the battlezone.
			pHomeInfo = m_pMain->m_HomeArray.GetData(m_pUserData->m_bNation);
			if (!pHomeInfo) return;

			m_pUserData->m_bZone = m_pUserData->m_bNation;

			if (m_pUserData->m_bNation == KARUS) {
				m_pUserData->m_curx = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
				m_pUserData->m_curz = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ); 
			}
			else {
				m_pUserData->m_curx = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
				m_pUserData->m_curz = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ); 
			}
		}
		printf("º»±¹À¸·Î Àß ÀúÀåµÇ¾úÀ»°Å¾ß. °ÆÁ¤¸¶!!!\r\n");
	}
*/

	MarketBBSUserDelete();
	LogOut();
	Initialize();
	CIOCPSocket2::CloseProcess();
}

void CUser::Parsing(int len, char *pData)
{
	int index = 0;
	float	currenttime = FLT_MIN;

	BYTE command = GetByte(pData, index);

	switch( command )
	{
	case WIZ_LOGIN:
		LoginProcess( pData+index );
		break;
	case WIZ_SEL_NATION:
		SelNationToAgent( pData+index );
		break;
	case WIZ_NEW_CHAR:
		NewCharToAgent( pData+index );
		break;
	case WIZ_DEL_CHAR:
		DelCharToAgent( pData+index );
		break;
	case WIZ_SEL_CHAR:
		SelCharToAgent( pData+index );
		break;
	case WIZ_GAMESTART:
			{
				int index = 0, send_index = 0, knightsindex = 0;
				char send_buff[2048]; memset( send_buff, NULL, 2048 );
				char knightstr[1024]; memset( knightstr, NULL, 1024 );

				if( m_State == STATE_GAMESTART )
					break;
				m_State = STATE_GAMESTART;

				SendMyInfo();
				UserInOut( USER_REGENE );
				m_pMain->UserInOutForMe(this);
				m_pMain->NpcInOutForMe(this);
				SendPremium();
				SendNotice();
				SendTimeStatus();

				SetByte( send_buff, WIZ_GAMESTART, send_index );
				Send( send_buff, send_index );

				printf("GAMESTART: %s..%d\n", m_pUserData->m_id, m_Sid);

				if( m_pUserData->m_bKnights != 0 ) {
					send_index = 0;
					knightsindex = m_pUserData->m_bKnights;

					sprintf( knightstr, "%s is Online.", m_pUserData->m_id );

					SetByte( send_buff, WIZ_CHAT, send_index );
					SetByte( send_buff, KNIGHTS_CHAT, send_index );
					SetByte( send_buff, 1, send_index );
					SetByte( send_buff, -1, send_index );
					SetByte( send_buff, -1, send_index );
					SetByte( send_buff, 0x00, send_index );
					SetShort( send_buff, strlen(knightstr), send_index );
					SetString( send_buff, knightstr, strlen(knightstr), send_index );
					m_pMain->Send_KnightsMember( knightsindex, send_buff, send_index );
				}
			}
		break;
	case WIZ_MOVE:
		MoveProcess( pData+index );
		break;
	case WIZ_ROTATE:
		Rotate( pData+index );
		break;
	case WIZ_ATTACK:
		Attack( pData+index );
		break;
	case WIZ_ALLCHAR_INFO_REQ:
		AllCharInfoToAgent();
		break;
	case WIZ_CHAT:
		Chat( pData+index );
		break;
	case WIZ_CHAT_TARGET:
		ChatTargetSelect( pData+index );
		break;
	case WIZ_REGENE:	
		InitType3();	// Init Type 3.....
		InitType4();	// Init Type 4.....
//		Corpse();
		Regene( pData+index );
//		InitType3();	// Init Type 3.....
//		InitType4();	// Init Type 4.....
		break;
	case WIZ_REQ_USERIN:
		RequestUserIn( pData+index );
		break;
	case WIZ_REQ_NPCIN:
		RequestNpcIn( pData+index );
		break;
	case WIZ_WARP:
//		if( m_pUserData->m_bAuthority == 0 || m_pUserData->m_bAuthority == 1) {
		if( m_pUserData->m_bAuthority == 0 ) {
			Warp( pData+index );
		}
		break;
	case WIZ_ITEM_MOVE:
		ItemMove( pData+index );
		break;
	case WIZ_NPC_EVENT:
		NpcEvent( pData+index );
		break;
	case WIZ_ITEM_TRADE:
		ItemTrade( pData+index );
		break;
	case WIZ_TARGET_HP:
		{
			int uid = GetShort( pData, index );
			BYTE echo = GetByte( pData, index );
			SendTargetHP(echo, uid);
		}
		break;
	case WIZ_BUNDLE_OPEN_REQ:
		BundleOpenReq( pData+index );
		break;
	case WIZ_ITEM_GET:
		ItemGet( pData+index );
		break;
	case WIZ_ZONE_CHANGE:
		//UserInOut( USER_IN );
		UserInOut( USER_REGENE );
		m_pMain->RegionUserInOutForMe(this);
		m_pMain->RegionNpcInfoForMe(this);
		m_bWarp = 0x00;
		break;
	case WIZ_POINT_CHANGE:
		PointChange( pData+index );
		break;
	case WIZ_STATE_CHANGE:
		StateChange( pData+index );
		break;
	case WIZ_VERSION_CHECK:
		VersionCheck();
		break;
	//case WIZ_SPEEDHACK_USED:
	//	SpeedHackUser();
	//	break;
	case WIZ_PARTY:
		PartyProcess( pData+index );
		break;
	case WIZ_EXCHANGE:
		ExchangeProcess( pData+index );
		break;
	case WIZ_MAGIC_PROCESS:
		m_MagicProcess.MagicPacket( pData+index, len );
		break;
	case WIZ_SKILLPT_CHANGE:
		SkillPointChange( pData+index );
		break;
	case WIZ_OBJECT_EVENT:
		ObjectEvent( pData+index );
		break;
	case WIZ_WEATHER:
	case WIZ_TIME:
		//if (m_pMain->m_bBattleOpen == FALSE) {	// Check if it's time to open the Battlezone.
		//	m_pMain->BattleZoneOpenTimer();
		//}
			
	/*	if (m_pMain->m_bVictory) {		// If victory was declared....
			if (currenttime > (m_pMain->m_bBanishDelayStart + BANISH_DELAY_TIME)) {
				m_pMain->BanishLosers();
				m_pMain->ResetBattleZone();
			}							
		}	*/
		UpdateGameWeather( pData+index, command );
		break;
	case WIZ_CLASS_CHANGE:
		ClassChange( pData+index );
		break;
	case WIZ_CONCURRENTUSER:
		CountConcurrentUser();
		break;
	case WIZ_DATASAVE:
		UserDataSaveToAgent();
		break;
	case WIZ_ITEM_REPAIR:
		ItemRepair( pData+index );
		break;
	case WIZ_KNIGHTS_PROCESS:
		m_pMain->m_KnightsManager.PacketProcess( this, pData+index );
		break;
	case WIZ_ITEM_REMOVE:
		ItemRemove( pData+index );
		break;
	case WIZ_OPERATOR:
		OperatorCommand( pData+index );
		break;
	case WIZ_SPEEDHACK_CHECK:
		SpeedHackTime( pData+index );
		m_sAliveCount = 0;
		break;
	case WIZ_SERVER_CHECK:
		ServerStatusCheck();
		break;
	case WIZ_WAREHOUSE:
		WarehouseProcess( pData+index );
		break;
	case WIZ_REPORT_BUG:
		ReportBug(pData+index);
		break;
	case WIZ_HOME:
		Home();
		break; 
/*	case WIZ_FRIEND_REPORT:
		FriendReport( pData+index );
//		Friend(pData+index);
		break; */
	case WIZ_WARP_LIST:
		SelectWarpList( pData+index );
		break;
	case WIZ_ZONE_CONCURRENT:
		ZoneConCurrentUsers( pData+index );
		break;
	case WIZ_VIRTUAL_SERVER:	// ÀÎ¿øÃ¼Å©°¡ ³¡³ª°í ÇØ´ç ¼­¹ö·Î °¡µµ ÁÁ´Ù´Â Çã¶ôÀ» ¹Ş¾Ò´Ù
		ServerChangeOk( pData+index );
		break;
	case WIZ_PARTY_BBS:
		PartyBBS( pData+index );
		break;
	case WIZ_MARKET_BBS:
		MarketBBS( pData+index );
		break;
	case WIZ_KICKOUT:
		KickOut( pData+index );
		break;
	case WIZ_CLIENT_EVENT:
		ClientEvent( pData+index );
		break;
	case WIZ_TEST_PACKET:
		TestPacket( pData+index );
		break;
	case WIZ_SELECT_MSG:
		RecvSelectMsg( pData+index );
		break;
	case WIZ_EDIT_BOX:
		RecvEditBox( pData+index );
		break;

	case WIZ_MERCHANT:
		MerchantProcess( pData+index );
		break;
	case WIZ_LETTER:
	//	LetterToAgent( pData+index );
		break;
	case WIZ_HELMET:
		Helmet( pData+index );
		break;
	case WIZ_NEW_EVENT:
		NewEvent( pData+index );
		break;
	case WIZ_MINE:
		MiningProcess( pData+index );
		break;
	case WIZ_TEMPLE:
		EventJoin( pData+index );
		break;
	case WIZ_KING:
		KingProcess( pData+index );                                        
		break;
/*	case WIZ_COMBAT:
		// Battle yollayınca
		// SEND-->WIZ_UNKNOW (75)-->750109546F7567684C756E6B
		// RECV-->WIZ_UNKNOW (75)-->750509546F7567684C756E6B

		// Karşıdaki iptal edince
		// RECV-->WIZ_UNKNOW (75)-->7504
		// Kabul edince
		//UNKNOW (5E)-->5E010000010A00
		//UNKNOW (27)-->27033000B0047E048C0002
		//--moradona geçince
		//UNKNOW (5E)-->5E010100011400
		//UNKNOW (27)-->27031500B61C7C15000002
		break; */
	}

	currenttime = TimeGet();

	if( command == WIZ_GAMESTART ) {
		m_fHPLastTimeNormal = currenttime;

		for (int h = 0 ; h < MAX_TYPE3_REPEAT ; h++) {
			m_fHPLastTime[h] = currenttime;
		}
	}	

	if( m_fHPLastTimeNormal != 0.0f && (currenttime - m_fHPLastTimeNormal) > m_bHPIntervalNormal && m_bAbnormalType != ABNORMAL_BLINKING) {
		HPTimeChange( currenttime );	// For Sitdown/Standup HP restoration.
	}

	if (m_bType3Flag) {     // For Type 3 HP Duration.
		for (int i = 0 ; i < MAX_TYPE3_REPEAT ; i++) {	
			if( m_fHPLastTime[i] != 0.0f && (currenttime - m_fHPLastTime[i]) > m_bHPInterval[i] ) {
				HPTimeChangeType3(currenttime);	
				break;
			}
		}
	} 

	if (m_bType4Flag) {		// For Type 4 Stat Duration.
		Type4Duration(currenttime);
	}
		
	if (m_bAbnormalType == ABNORMAL_BLINKING) {		// Should you stop blinking?
		BlinkTimeCheck(currenttime);
	}
}

void CUser::VersionCheck()
{
	int send_index = 0;
	char send_buff[128];
	memset(send_buff, 0x00, 128);

	// 1. Paket Header (WIZ_VERSION_CHECK)
	SetByte(send_buff, WIZ_VERSION_CHECK, send_index);

	// 2. Versiyon (1298)
	SetShort(send_buff, (short)__VERSION, send_index);
	
	// 3. Cryption
	SetInt64(send_buff, m_Public_key, send_index);

	// 4. Paketi Gönder
	// ÖNEMLİ: Paket henüz ŞİFRESİZ gitmeli!
	Send(send_buff, send_index);

	// 5. Şifrelemeyi ŞİMDİ aktif et
	// Bu satırdan sonra gelecek tüm paketler şifreli olacak.

}



void CUser::LoginProcess(char* pBuf)
{
	
	if (this == NULL || m_pMain == NULL) return;

	int index = 0, idlen = 0, pwdlen = 0, send_index = 0, retvalue = 0;
	char accountid[MAX_ID_SIZE + 1];
	char password[MAX_PW_SIZE + 1];
	char send_buff[MAX_PKTSIZE];

	memset(accountid, 0x00, sizeof(accountid));
	memset(password, 0x00, sizeof(password));
	memset(send_buff, 0x00, sizeof(send_buff));

	// 1. ADIM: Paket Okuma
	idlen = GetShort(pBuf, index);

	// idlen hala 100+ geliyorsa sorun Decryption (Şifre Çözme) aşamasındadır.
	if (idlen > MAX_ID_SIZE || idlen <= 0) {
		printf("--- Login Hatasi: Gelen ID Uzunlugu Gecersiz! (%d) ---\n", idlen);
		goto fail_return;
	}
	GetString(accountid, pBuf, idlen, index);

	pwdlen = GetShort(pBuf, index);
	if (pwdlen > MAX_PW_SIZE || pwdlen <= 0) {
		printf("--- Login Hatasi: Gelen Sifre Uzunlugu Gecersiz! (%d) ---\n", pwdlen);
		goto fail_return;
	}
	GetString(password, pBuf, pwdlen, index);

	// 2. ADIM: Kullanıcı Kontrolü
	CUser* pUser = m_pMain->GetUserPtr(accountid, 0x01);
	if (pUser && (pUser->m_Sid != m_Sid)) {
		pUser->UserDataSaveToAgent();
		pUser->Close();
		goto fail_return;
	}

	// 3. ADIM: Paketi Aujard'a Göndermek İçin Hazırla
	send_index = 0;
	SetByte(send_buff, WIZ_LOGIN, send_index);
	SetShort(send_buff, m_Sid, send_index);
	SetShort(send_buff, (short)idlen, send_index);
	SetString(send_buff, accountid, idlen, send_index);
	SetShort(send_buff, (short)pwdlen, send_index);
	SetString(send_buff, password, pwdlen, send_index);

	// 4. ADIM: Kuyruğa Gönder (Aujard'a Veri Gönderimi)
	// m_pHeader kontrolü kaldırıldı, doğrudan PutData sonucu kontrol ediliyor.
	retvalue = m_pMain->m_LoggerSendQueue.PutData(send_buff, send_index);

	if (retvalue >= SMQ_FULL) {
		char logstr[256];
		sprintf(logstr, "Aujard Kuyruk Hatasi (ID: %s) - Kod: %d", accountid, retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		goto fail_return;
	}

	// Başarılıysa ID'yi sakla
	strncpy(m_strAccountID, accountid, MAX_ID_SIZE);
	return;

fail_return:
	send_index = 0;
	SetByte(send_buff, WIZ_LOGIN, send_index);
	SetByte(send_buff, 0xFF, send_index); // 0xFF = Fail / Login Error
	Send(send_buff, send_index);
}




void CUser::NewCharToAgent(char *pBuf)
{
	int index = 0, idlen = 0, send_index = 0, retvalue = 0;
	int charindex = 0, race = 0, Class = 0, hair = 0, face = 0, str = 0, sta = 0, dex = 0, intel = 0, cha = 0;
	char charid[MAX_ID_SIZE+1];
	memset( charid, NULL, MAX_ID_SIZE+1 );
	char send_buff[256];
	memset( send_buff, NULL, 256);
	BYTE result;
	int sum = 0;
	_CLASS_COEFFICIENT* p_TableCoefficient = NULL;

	charindex = GetByte( pBuf, index );
	idlen = GetShort( pBuf, index );

	if( idlen > MAX_ID_SIZE || idlen <= 0 ) {
		result = 0x05;
		goto fail_return;
	}
	
	GetString( charid, pBuf, idlen, index );
	race = GetByte( pBuf, index );
	Class = GetShort( pBuf, index );
	face = GetByte( pBuf, index );
	hair = GetByte( pBuf, index );
	str = GetByte( pBuf, index );
	sta = GetByte( pBuf, index );
	dex = GetByte( pBuf, index );
	intel = GetByte( pBuf, index );
	cha = GetByte( pBuf, index );

	if( charindex > 4 || charindex < 0 ) {
		result = 0x01;
		goto fail_return;
	}

	if( !IsValidName( charid ) ) {
		result = 0x05;
		goto fail_return;
	}

	p_TableCoefficient = m_pMain->m_CoefficientArray.GetData( Class );
	if( !p_TableCoefficient ) {
		result = 0x02;
		goto fail_return;
	}

	sum = str + sta + dex + intel + cha;
	if( sum > 300 ) {
		result = 0x02;
		goto fail_return;
	}

	if (str < 50 || sta < 50 || dex < 50 || intel < 50 || cha < 50) {
		result = 0x11;
		goto fail_return;		
	}

	SetByte( send_buff, WIZ_NEW_CHAR, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetByte( send_buff, charindex, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, charid, idlen, send_index );
	SetByte( send_buff, race, send_index );
	SetShort( send_buff, Class, send_index );
	SetByte( send_buff, face, send_index );
	SetByte( send_buff, hair, send_index );
	SetByte( send_buff, str, send_index );
	SetByte( send_buff, sta, send_index );
	SetByte( send_buff, dex, send_index );
	SetByte( send_buff, intel, send_index );
	SetByte( send_buff, cha, send_index );
	
	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "NewChar Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		goto fail_return;
	}

	return;

fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_NEW_CHAR, send_index );
	SetByte( send_buff, result, send_index );
	Send( send_buff, send_index );
}

void CUser::DelCharToAgent(char *pBuf)
{
	int index = 0, idlen = 0, send_index = 0, retvalue = 0;
	int charindex = 0, soclen = 0;
	char charid[MAX_ID_SIZE+1];
	char socno[15];
	memset( charid, NULL, MAX_ID_SIZE+1 );
	memset( socno, NULL, 15 );
	char send_buff[256];
	memset( send_buff, NULL, 256);

	charindex = GetByte( pBuf, index );
	if( charindex > 4 )	goto fail_return;
	idlen = GetShort( pBuf, index );
	if( idlen > MAX_ID_SIZE || idlen <= 0 )	goto fail_return;
	GetString( charid, pBuf, idlen, index );
	soclen = GetShort( pBuf, index );
	// sungyong tw
	//if( soclen != 14 ) goto fail_return;
	if( soclen > 14 || soclen <= 0 ) goto fail_return;
	// ~sungyong tw
	GetString( socno, pBuf, soclen, index );

	// ´ÜÀåÀº Ä³¸¯ »èÁ¦°¡ ¾ÈµÇ°Ô, ¸ÕÀú Å¬·£À» Å»Åğ ÈÄ »èÁ¦°¡ µÇµµ·Ï,,
	if( m_pUserData->m_bKnights > 0 && m_pUserData->m_bFame == CHIEF)	goto fail_return;	

	SetByte( send_buff, WIZ_DEL_CHAR, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetByte( send_buff, charindex, send_index );
	SetShort( send_buff, idlen, send_index );
	SetString( send_buff, charid, idlen, send_index );
	SetShort( send_buff, soclen, send_index );
	SetString( send_buff, socno, soclen, send_index );

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "DelChar Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		goto fail_return;
	}

	return;

fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_DEL_CHAR, send_index );
	SetByte( send_buff, 0x00, send_index );
	SetByte( send_buff, 0xFF, send_index );
	Send( send_buff, send_index );
}

void CUser::SelNationToAgent(char *pBuf)
{
	int index = 0, send_index = 0, retvalue = 0;
	int nation = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256);

	nation = GetByte( pBuf, index );
	if( nation > 2 )
		goto fail_return;

	SetByte( send_buff, WIZ_SEL_NATION, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetByte( send_buff, nation, send_index );

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "Nation Sel Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		goto fail_return;
	}

	return;
fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_SEL_NATION, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::SelCharToAgent(char *pBuf)
{
	int index = 0, idlen1 = 0, idlen2 = 0, send_index = 0, retvalue = 0, zone = 0, zoneindex = 0;
	char userid[MAX_ID_SIZE+1], accountid[MAX_ID_SIZE+1];
	memset( userid, NULL, MAX_ID_SIZE+1 );
	memset( accountid, NULL, MAX_ID_SIZE+1 );
	char send_buff[256];
	memset( send_buff, NULL, 256);
	CUser* pUser = NULL;
	C3DMap* pMap = NULL;
	_ZONE_SERVERINFO *pInfo	= NULL;
	CTime t = CTime::GetCurrentTime();
	BYTE	bInit = 0x01;

	idlen1 = GetShort( pBuf, index );
	if( idlen1 > MAX_ID_SIZE || idlen1 <= 0 )
		goto fail_return;
	GetString( accountid, pBuf, idlen1, index );
	
	idlen2 = GetShort( pBuf, index );
	if( idlen2 > MAX_ID_SIZE || idlen2 <= 0 )
		goto fail_return;
	GetString( userid, pBuf, idlen2, index );
	bInit = GetByte( pBuf, index );
	zone = GetByte( pBuf, index );
	
	if( _strnicmp( accountid, m_strAccountID, MAX_ID_SIZE ) != 0 ) {
		pUser = m_pMain->GetUserPtr( accountid, 0x01 );
		if( pUser && (pUser->m_Sid != m_Sid) ) {
			pUser->Close();
			goto fail_return;
		}
		strcpy( m_strAccountID, accountid );	// Á¸ÀÌµ¿ ÇÑ °æ¿ì´Â ·Î±×ÀÎ ÇÁ·Î½ÃÁ®°¡ ¾øÀ¸¹Ç·Î...
	}

	pUser = m_pMain->GetUserPtr( userid, 0x02 );
	if( pUser && (pUser->m_Sid != m_Sid) ) {
		pUser->Close();
		goto fail_return;
	}

	// À½³É,, ¿©±â¼­ Á¸À» ºñ±³,,,
	if( zone <= 0 )	{
		printf("### SelCharToAgent zone Fail : zone=%d\n", zone);
		goto fail_return;
	}

	zoneindex = m_pMain->GetZoneIndex(zone);
	if( zoneindex < 0 || zoneindex >= m_pMain->m_ZoneArray.size() )	{
		printf("### SelCharToAgent zoneindex Fail : zone=%d, zoneindex=%d\n", zone, zoneindex);
		goto fail_return;
	}

	pMap = m_pMain->m_ZoneArray[zoneindex];
	if( !pMap )	{
		printf("### SelCharToAgent map load Fail : zoneindex=%d\n", zoneindex);
		goto fail_return;
	}

	if( m_pMain->m_nServerNo != pMap->m_nServerNo ) {
		pInfo = m_pMain->m_ServerArray.GetData( pMap->m_nServerNo );
		if( !pInfo )	{
			printf("### SelCharToAgent server info Fail : server=%d\n", pMap->m_nServerNo);
			goto fail_return;
		}

		SetByte( send_buff, WIZ_SERVER_CHANGE, send_index );
		SetShort( send_buff, strlen( pInfo->strServerIP ), send_index );
		SetString( send_buff, pInfo->strServerIP, strlen( pInfo->strServerIP ), send_index );
		SetShort( send_buff, pInfo->sPort, send_index );
		SetByte( send_buff, bInit, send_index );
		SetByte( send_buff, zone, send_index );
		SetByte( send_buff, m_pMain->m_byOldVictory, send_index );
		Send( send_buff, send_index );
		printf("--> SelCharToAgent server change : userid=%s, ip=%s, binit=%d\n", userid, pInfo->strServerIP, bInit);
		return;
	}


	SetByte( send_buff, WIZ_SEL_CHAR, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetShort( send_buff, idlen2, send_index );
	SetString( send_buff, userid, idlen2, send_index );
	SetByte( send_buff, bInit, send_index );
	SetDWORD( send_buff, m_pMain->m_iPacketCount, send_index );

	char logfile[256];	memset(logfile, 0x00, 256);
	wsprintf( logfile, "[SelCharToAgent : %d:%d:%d] - acname=%s, name=%s, count=%d, TH: %lu, Rear : %d\r\n", t.GetHour(), t.GetMinute(), t.GetSecond(), m_strAccountID, userid, m_pMain->m_iPacketCount, ::GetCurrentThreadId(), m_pMain->m_LoggerSendQueue.GetRearPointer());
	EnterCriticalSection( &g_LogFile_critical );
	m_pMain->m_LogFile.Write( logfile, strlen(logfile) );
	LeaveCriticalSection( &g_LogFile_critical );

	m_pMain->m_iPacketCount++;

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "SelChar Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		goto fail_return;
	}

	m_pMain->m_iSendPacketCount++;
	//printf(" select char to agent ,, acname=%s, userid=%s\n", m_strAccountID, userid);

	return;

fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_SEL_CHAR, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::SelectCharacter(char* pBuf)
{
	int index = 0, send_index = 0, zoneindex = -1, retvalue = 0;
	char send_buff[256];
	memset(send_buff, NULL, 256);
	BYTE result, bInit;
	C3DMap* pMap = NULL;
	_ZONE_SERVERINFO* pInfo = NULL;
	CKnights* pKnights = NULL;

	result = GetByte(pBuf, index);
	bInit = GetByte(pBuf, index);

	m_pMain->m_iRecvPacketCount++;

	if (result == 0x00) goto fail_return;
	if (m_pUserData->m_bZone == 0)	goto fail_return;

	zoneindex = m_pMain->GetZoneIndex(m_pUserData->m_bZone);
	if (zoneindex < 0 || zoneindex >= m_pMain->m_ZoneArray.size()) goto fail_return;
	pMap = m_pMain->m_ZoneArray[zoneindex];
	if (!pMap)
		goto fail_return;

	if (m_pMain->m_nServerNo != pMap->m_nServerNo) {
		pInfo = m_pMain->m_ServerArray.GetData(pMap->m_nServerNo);
		if (!pInfo)
			goto fail_return;

		SetByte(send_buff, WIZ_SERVER_CHANGE, send_index);
		SetShort(send_buff, strlen(pInfo->strServerIP), send_index);
		SetString(send_buff, pInfo->strServerIP, strlen(pInfo->strServerIP), send_index);
		SetShort(send_buff, pInfo->sPort, send_index);
		SetByte(send_buff, bInit, send_index);
		SetByte(send_buff, m_pUserData->m_bZone, send_index);
		SetByte(send_buff, m_pMain->m_byOldVictory, send_index);
		Send(send_buff, send_index);
		return;
	}

	if (m_pUserData->m_bAuthority == 0xff) {
		Close();
		return;
	}

	// ÀüÀïÁßÀÌ ¾Æ´Ñ»óÅÂ¿¡¼­´Â ´ëÀåÀ¯Àú°¡->ÀÏ¹İ´ÜÀåÀ¸·Î
	if (m_pMain->m_byBattleOpen == NO_BATTLE && m_pUserData->m_bFame == COMMAND_CAPTAIN) {
		m_pUserData->m_bFame = CHIEF;
	}
	//
	if (m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3 && !m_pMain->m_byBattleOpen) {
		NativeZoneReturn();
		Close();
		return;
	}

	if (m_pUserData->m_bZone == ZONE_BATTLE && (m_pMain->m_byBattleOpen != NATION_BATTLE)) {
		NativeZoneReturn();
		Close();
		return;
	}
	if (m_pUserData->m_bZone == ZONE_SNOW_BATTLE && (m_pMain->m_byBattleOpen != SNOW_BATTLE)) {
		NativeZoneReturn();
		Close();
		return;
	}

	if (m_pUserData->m_bZone == ZONE_FRONTIER && m_pMain->m_byBattleOpen) {
		NativeZoneReturn();
		Close();
		return;
	}
	//
	SetLogInInfoToDB(bInit);	// Write User Login Info To DB for Kicking out or Billing

	SetByte(send_buff, WIZ_SEL_CHAR, send_index);
	SetByte(send_buff, result, send_index);
	SetByte(send_buff, m_pUserData->m_bZone, send_index);
	SetShort(send_buff, (WORD)m_pUserData->m_curx * 10, send_index);
	SetShort(send_buff, (WORD)m_pUserData->m_curz * 10, send_index);
	SetShort(send_buff, (short)m_pUserData->m_cury * 10, send_index);
	SetByte(send_buff, m_pMain->m_byOldVictory, send_index);
	Send(send_buff, send_index);

	SetDetailData();	// µğºñ¿¡ ¾ø´Â µ¥ÀÌÅÍ ¼ÂÆÃ...

	//printf("SelectCharacter 111 - id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);

	// sungyong ,, zone server : Ä«·ç½º¿Í ÀüÀïÁ¸À» ÇÕÄ¡¹Ç·Î ÀÎÇØ¼­,,
	// ÀüÀïÁ¸ÀÏ¶§ ... 
	if (m_pUserData->m_bZone > 2)
	{
		// ³ªÀÇ ±â»ç´Ü ¸®½ºÆ®¿¡¼­ ³»°¡ ±â»ç´Ü Á¤º¸¿¡ ÀÖ´ÂÁö¸¦ °Ë»öÇØ¼­ ¸¸¾à ¾øÀ¸¸é 
		// Ãß°¡ÇÑ´Ù(´Ù¸¥Á¸¿¡¼­ ±â»ç´Ü¿¡ °¡ÀÔµÈ °æ¿ì)
		if (m_pUserData->m_bKnights == -1) {	// Ãß¹æµÈ À¯Àú
			m_pUserData->m_bKnights = 0;
			m_pUserData->m_bFame = 0;
			printf("SelectCharacter - id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);
			return;
		}
		else if (m_pUserData->m_bKnights != 0) {
			/*	memset( send_buff, 0x00, 256);	send_index = 0;
				SetByte( send_buff, WIZ_KNIGHTS_PROCESS, send_index );
				SetByte( send_buff, KNIGHTS_LIST_REQ+0x10, send_index );
				SetShort( send_buff, GetSocketID(), send_index );
				SetShort( send_buff, m_pUserData->m_bKnights, send_index );
				retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
				if( retvalue >= SMQ_FULL ) {
					//goto fail_return;
					m_pMain->m_StatusList.AddString("KNIGHTS_LIST_REQ Packet Drop!!!");
				}	*/

			pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);
			if (pKnights) {
				m_pMain->m_KnightsManager.SetKnightsUser(m_pUserData->m_bKnights, m_pUserData->m_id);
			}
			else {
				printf("SelectCharacter - ±â»ç´Ü ¸®½ºÆ® ¿äÃ»,, id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);
				memset(send_buff, 0x00, 256);	send_index = 0;
				SetByte(send_buff, WIZ_KNIGHTS_PROCESS, send_index);
				SetByte(send_buff, KNIGHTS_LIST_REQ + 0x10, send_index);
				SetShort(send_buff, GetSocketID(), send_index);
				SetShort(send_buff, m_pUserData->m_bKnights, send_index);
				retvalue = m_pMain->m_LoggerSendQueue.PutData(send_buff, send_index);
				if (retvalue >= SMQ_FULL) {
					//goto fail_return;
					m_pMain->m_StatusList.AddString("KNIGHTS_LIST_REQ Packet Drop!!!");
				}

				pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);
				if (pKnights) {
					printf("SelectCharacter - ±â»ç´Ü ¸®½ºÆ® Ãß°¡,, id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);
					m_pMain->m_KnightsManager.SetKnightsUser(m_pUserData->m_bKnights, m_pUserData->m_id);
				}
			}
		}
	}
	else {
		// ³ªÀÇ ±â»ç´Ü ¸®½ºÆ®¿¡¼­ ³»°¡ ±â»ç´Ü Á¤º¸¿¡ ÀÖ´ÂÁö¸¦ °Ë»öÇØ¼­ ¸¸¾à ¾øÀ¸¸é 
		// Ãß°¡ÇÑ´Ù(´Ù¸¥Á¸¿¡¼­ ±â»ç´Ü¿¡ °¡ÀÔµÈ °æ¿ì)
		if (m_pUserData->m_bKnights == -1) {	// Ãß¹æµÈ À¯Àú
			m_pUserData->m_bKnights = 0;
			m_pUserData->m_bFame = 0;
			printf("SelectCharacter - id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);
			return;
		}
		else if (m_pUserData->m_bKnights != 0) {
			pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);
			if (pKnights) {
				m_pMain->m_KnightsManager.SetKnightsUser(m_pUserData->m_bKnights, m_pUserData->m_id);
			}
			else {			// ±â»ç´ÜÀÌ ÆÄ±«µÇ¾î ÀÖÀ½À¸·Î.. 
				m_pUserData->m_bKnights = 0;
				m_pUserData->m_bFame = 0;
			}
		}
	}

	memset(send_buff, NULL, 256); send_index = 0;
	SetByte(send_buff, WIZ_DATASAVE, send_index);
	SetShort(send_buff, strlen(m_pUserData->m_Accountid), send_index);
	SetString(send_buff, m_pUserData->m_Accountid, strlen(m_pUserData->m_Accountid), send_index);
	SetShort(send_buff, strlen(m_pUserData->m_id), send_index);
	SetString(send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index);
	SetByte(send_buff, 0x01, send_index);	// login...
	SetByte(send_buff, m_pUserData->m_bLevel, send_index);
	SetDWORD(send_buff, m_pUserData->m_iExp, send_index);
	SetDWORD(send_buff, m_pUserData->m_iLoyalty, send_index);
	SetDWORD(send_buff, m_pUserData->m_iGold, send_index);

	retvalue = m_pMain->m_ItemLoggerSendQ.PutData(send_buff, send_index);
	if (retvalue >= SMQ_FULL) {
		char logstr[256]; memset(logstr, 0x00, 256);
		sprintf(logstr, "Login Logger Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
	printf("SelectCharacter - id=%s, knights=%d, fame=%d\n", m_pUserData->m_id, m_pUserData->m_bKnights, m_pUserData->m_bFame);

	return;

fail_return:
	SetByte(send_buff, WIZ_SEL_CHAR, send_index);
	SetByte(send_buff, 0x00, send_index);
	Send(send_buff, send_index);
}



void CUser::AllCharInfoToAgent()
{
	int send_index = 0, retvalue = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256);

	SetByte( send_buff, WIZ_ALLCHAR_INFO_REQ, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		memset( send_buff, NULL, 256); send_index = 0;
		SetByte( send_buff, WIZ_ALLCHAR_INFO_REQ, send_index );
		SetByte( send_buff, 0xFF, send_index );
		
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "All CharInfo Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
}

void CUser::UserDataSaveToAgent()
{
	int send_index = 0, retvalue = 0;
	char send_buff[256];	memset( send_buff, NULL, 256);

	if( strlen(m_pUserData->m_id) == 0 )
		return;
	if( strlen(m_pUserData->m_Accountid) == 0 )
		return;

	SetByte( send_buff, WIZ_DATASAVE, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_pUserData->m_Accountid), send_index );
	SetString( send_buff, m_pUserData->m_Accountid, strlen(m_pUserData->m_Accountid), send_index );
	SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
	SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "DataSave Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}

	memset( send_buff, NULL, 256); send_index = 0;
	SetByte( send_buff, WIZ_DATASAVE, send_index );
	SetShort( send_buff, strlen(m_pUserData->m_Accountid), send_index );
	SetString( send_buff, m_pUserData->m_Accountid, strlen(m_pUserData->m_Accountid), send_index );
	SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
	SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
	SetByte( send_buff, 0x02, send_index );
	SetByte( send_buff, m_pUserData->m_bLevel, send_index );
	SetDWORD( send_buff, m_pUserData->m_iExp, send_index );
	SetDWORD( send_buff, m_pUserData->m_iLoyalty, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );

	retvalue = m_pMain->m_ItemLoggerSendQ.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf( logstr, "Exp Logger Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
}
void CUser::LogOut()
{
	// GÜVENLİK: Pointer kontrolleri (Crash engelleme)
	if (this == NULL || m_pMain == NULL || m_pUserData == NULL)
		return;

	// Veri kontrolü: ID'ler boşsa işlem yapma
	if (m_pUserData->m_Accountid[0] == '\0' || m_pUserData->m_id[0] == '\0')
		return;

	int index = 0, send_index = 0, count = 0;
	CUser* pUser = NULL;
	char send_buf[256]; 	memset(send_buf, 0x00, 256);

	// Log Yazma Süreci
	CTime t = CTime::GetCurrentTime();
	char logfile[256];	memset(logfile, 0x00, 256);
	wsprintf(logfile, "[%s : %s Logout : %02d:%02d:%02d]\r\n",
		m_pUserData->m_Accountid, m_pUserData->m_id, t.GetHour(), t.GetMinute(), t.GetSecond());

	EnterCriticalSection(&g_LogFile_critical);
	// Dosyanın geçerli ve m_pMain'in hala hayatta olduğunu kontrol et
	if (m_pMain != NULL && m_pMain->m_LogFile.m_hFile != CFile::hFileNull)
	{
		m_pMain->m_LogFile.Write(logfile, (UINT)strlen(logfile));
	}



	LeaveCriticalSection(&g_LogFile_critical);

	// Kullanıcı Pointer Kontrolü
	pUser = m_pMain->GetUserPtr(m_pUserData->m_Accountid, 0x01);
	if (pUser && (pUser->m_Sid != m_Sid))
	{
		printf("%s : %s Logout: Sid farkli...\n", m_pUserData->m_Accountid, m_pUserData->m_id);
		return;
	}

	// Paket Hazırlama (WIZ_LOGOUT)
	SetByte(send_buf, WIZ_LOGOUT, send_index);
	SetShort(send_buf, m_Sid, send_index);
	SetShort(send_buf, (short)strlen(m_pUserData->m_Accountid), send_index);
	SetString(send_buf, m_pUserData->m_Accountid, (int)strlen(m_pUserData->m_Accountid), send_index);
	SetShort(send_buf, (short)strlen(m_pUserData->m_id), send_index);
	SetString(send_buf, m_pUserData->m_id, (int)strlen(m_pUserData->m_id), send_index);

	// Logger Queue'ya ekle
	do {
		if (m_pMain->m_LoggerSendQueue.PutData(send_buf, send_index) == 1)
			break;
		else
			count++;
	} while (count < 30);

	if (count > 29) {
		char logstr[256]; memset(logstr, 0x00, 256);
		sprintf(logstr, "Logout Send Fail : acname=%s, charid=%s ", m_pUserData->m_Accountid, m_pUserData->m_id);
		m_pMain->m_StatusList.AddString(logstr);
	}

	// AI Server'a Bildir
	char ai_buf[256]; memset(ai_buf, 0x00, 256);
	int ai_index = 0;
	SetByte(ai_buf, AG_USER_LOG_OUT, ai_index);
	m_pMain->Send_AIServer(m_pUserData->m_bZone, ai_buf, ai_index);

	// DB Save Paketi (WIZ_DATASAVE)
	memset(send_buf, 0x00, 256); send_index = 0;
	SetByte(send_buf, WIZ_DATASAVE, send_index);
	SetShort(send_buf, (short)strlen(m_pUserData->m_Accountid), send_index);
	SetString(send_buf, m_pUserData->m_Accountid, (int)strlen(m_pUserData->m_Accountid), send_index);
	SetShort(send_buf, (short)strlen(m_pUserData->m_id), send_index);
	SetString(send_buf, m_pUserData->m_id, (int)strlen(m_pUserData->m_id), send_index);
	SetByte(send_buf, 0x03, send_index); // Logout tipi save
	SetByte(send_buf, m_pUserData->m_bLevel, send_index);
	SetDWORD(send_buf, m_pUserData->m_iExp, send_index);
	SetDWORD(send_buf, m_pUserData->m_iLoyalty, send_index);
	SetDWORD(send_buf, m_pUserData->m_iGold, send_index);

	// Item Logger'a gönder
	int retvalue = m_pMain->m_ItemLoggerSendQ.PutData(send_buf, send_index);
	if (retvalue >= 10000) { // SMQ_FULL kontrolü
		char logstr[256]; memset(logstr, 0x00, 256);
		sprintf(logstr, "Logout Logger Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}

	return;
}




void CUser::MoveProcess(char *pBuf )
{
	if( m_bWarp ) return;
		
	int index = 0, send_index = 0, region = 0;
	WORD will_x, will_z;
	short will_y, speed=0;
	float real_x, real_z, real_y;
	BYTE echo;
	char send_buf[1024];
	memset( send_buf, 0x00, 1024 );

	will_x = GetShort( pBuf, index );
	will_z = GetShort( pBuf, index );
	will_y = GetShort( pBuf, index );

	speed = GetShort( pBuf, index );
	echo = GetByte( pBuf, index );

	real_x = will_x/10.0f; real_z = will_z/10.0f; real_y = will_y/10.0f;
	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
	if( m_pMain->m_ZoneArray[m_iZoneIndex]->IsValidPosition( real_x, real_z, real_y ) == FALSE ) return;

//	real_y = m_pMain->m_ZoneArray[m_iZoneIndex]->GetHeight(	real_x, real_y, real_z );

//	if( speed > 60 ) {	// client ¿¡¼­ ÀÌ¼öÄ¡º¸´Ù Å©°Ô º¸³¾¶§°¡ ¸¹À½...
//		if( m_bSpeedAmount == 100 )
//			speed = 0;
//	}

	if( m_bResHpType == USER_DEAD || m_pUserData->m_sHp == 0 )	{
		if( speed != 0 )
			printf("### MoveProcess Fail : name=%s(%d), m_bResHpType=%d, hp=%d, speed=%d, x=%d, z=%d ###\n", m_pUserData->m_id, m_Sid, m_bResHpType, m_pUserData->m_sHp, speed, (int)m_pUserData->m_curx, (int)m_pUserData->m_curz);
	}

	if( speed != 0 ) {
		m_pUserData->m_curx = m_fWill_x;	// °¡Áö°í ÀÖ´ø ´ÙÀ½ÁÂÇ¥¸¦ ÇöÀçÁÂÇ¥·Î ¼ÂÆÃ...
		m_pUserData->m_curz = m_fWill_z;
		m_pUserData->m_cury = m_fWill_y;

		m_fWill_x = will_x/10.0f;	// ´ÙÀ½ÁÂÇ¥¸¦ ±â¾ï....
		m_fWill_z = will_z/10.0f;
		m_fWill_y = will_y/10.0f;
	}
	else {
		m_pUserData->m_curx = m_fWill_x = will_x/10.0f;	// ´ÙÀ½ÁÂÇ¥ == ÇöÀç ÁÂÇ¥...
		m_pUserData->m_curz = m_fWill_z = will_z/10.0f;
		m_pUserData->m_cury = m_fWill_y = will_y/10.0f;
	}

	SetByte( send_buf, WIZ_MOVE, send_index );
	SetShort( send_buf, m_Sid, send_index );
	SetShort( send_buf, will_x, send_index );
	SetShort( send_buf, will_z, send_index );
	SetShort( send_buf, will_y, send_index );
	SetShort( send_buf, speed, send_index );
	SetByte( send_buf, echo, send_index );

	RegisterRegion();
	m_pMain->Send_Region( send_buf, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );

	if( m_iZoneIndex >= 0 && m_iZoneIndex < m_pMain->m_ZoneArray.size() ) 
		m_pMain->m_ZoneArray[m_iZoneIndex]->CheckEvent( real_x, real_z, this );

	int  ai_send_index = 0;
	char ai_send_buff[256];
	memset( ai_send_buff, NULL, 256);

	SetByte( ai_send_buff, AG_USER_MOVE, ai_send_index );
	SetShort( ai_send_buff, m_Sid, ai_send_index );
	Setfloat( ai_send_buff, m_fWill_x, ai_send_index );
	Setfloat( ai_send_buff, m_fWill_z, ai_send_index );
	Setfloat( ai_send_buff, m_fWill_y, ai_send_index );
	SetShort( ai_send_buff, speed, ai_send_index );
	
	m_pMain->Send_AIServer(m_pUserData->m_bZone, ai_send_buff, ai_send_index);
}

void CUser::UserInOut(BYTE Type)
{
	int send_index = 0, iLength = 0;
	// KRİTİK: Buffer boyutunu 256'dan 512'ye çıkardık (Eşya verileri sığmıyor olabilir)
	char buff[512];
	memset(buff, 0x00, 512);

	// 1. GÜVENLİK: m_pMain ve ZoneArray geçerli mi? (0xA8 hatasını önler)
	if (m_pMain == NULL || (DWORD_PTR)m_pMain < 0x1000) return;

	// 2. GÜVENLİK: m_ZoneArray bozulmuş mu? (std::vector::size çökmesini önler)
	try {
		if (m_iZoneIndex < 0 || m_iZoneIndex >= (int)m_pMain->m_ZoneArray.size())
			return;
	}
	catch (...) {
		return; // Bellek bozulmuşsa sessizce çık
	}

	C3DMap* pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if (!pMap) return;

	CKnights* pKnights = NULL;
	pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);

	// Bölge işlemleri
	if (Type == USER_OUT)
		pMap->RegionUserRemove(m_RegionX, m_RegionZ, m_Sid);
	else
		pMap->RegionUserAdd(m_RegionX, m_RegionZ, m_Sid);

	memset(buff, 0x00, 512);		send_index = 0;

	// Paket Hazırlama
	SetByte(buff, WIZ_USER_INOUT, send_index);
	SetShort(buff, Type, send_index);
	SetShort(buff, m_Sid, send_index);

	if (Type == USER_OUT) {
		m_pMain->Send_Region(buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, this);

		// AI Server Bilgilendirme
		send_index = 0;
		memset(buff, 0x00, 512);
		SetByte(buff, AG_USER_INOUT, send_index);
		SetByte(buff, Type, send_index);
		SetShort(buff, m_Sid, send_index);
		iLength = strlen(m_pUserData->m_id);
		SetShort(buff, (short)iLength, send_index);
		SetString(buff, m_pUserData->m_id, iLength, send_index);
		Setfloat(buff, m_pUserData->m_curx, send_index);
		Setfloat(buff, m_pUserData->m_curz, send_index);
		m_pMain->Send_AIServer(m_pUserData->m_bZone, buff, send_index);
		return;
	}

	// USER_IN Detayları
	iLength = strlen(m_pUserData->m_id);
	SetByte(buff, (BYTE)iLength, send_index);
	SetString(buff, m_pUserData->m_id, iLength, send_index);
	SetByte(buff, m_pUserData->m_bNation, send_index);
	SetShort(buff, m_pUserData->m_bKnights, send_index);
	SetShort(buff, m_pUserData->m_bFame, send_index);

	// Clan Bilgileri
	if (m_pUserData->m_bKnights == 0) {
		for (int i = 0; i < 11; i++) SetByte(buff, 0x00, send_index); // Padding
	}
	else {
		pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);
		if (pKnights) {
			iLength = strlen(pKnights->m_strName);
			SetByte(buff, pKnights->m_byRanking, send_index);
			SetByte(buff, (BYTE)iLength, send_index);
			SetString(buff, pKnights->m_strName, iLength, send_index);
			SetByte(buff, pKnights->m_byGrade, send_index);
			SetByte(buff, 0x01, send_index);
			SetByte(buff, 0x01, send_index);
			SetByte(buff, 0x00, send_index);
			SetByte(buff, 0x40, send_index);
			SetByte(buff, 0x00, send_index);
			SetByte(buff, 0xFF, send_index);
			SetByte(buff, 0x00, send_index);
			SetByte(buff, 0xFF, send_index);
		}
		else {
			for (int i = 0; i < 11; i++) SetByte(buff, 0x00, send_index);
		}
	}

	// Karakter Statları ve Görünüm
	SetByte(buff, m_pUserData->m_bRank, send_index);
	SetByte(buff, m_pUserData->m_bTitle, send_index);
	SetByte(buff, m_pUserData->m_bLevel, send_index);
	SetByte(buff, m_pUserData->m_bRace, send_index);
	SetShort(buff, m_pUserData->m_sClass, send_index);
	SetShort(buff, (WORD)(m_pUserData->m_curx * 10), send_index);
	SetShort(buff, (WORD)(m_pUserData->m_curz * 10), send_index);
	SetShort(buff, (short)(m_pUserData->m_cury * 10), send_index);
	SetByte(buff, m_pUserData->m_bFace, send_index);
	SetByte(buff, m_pUserData->m_bHairColor, send_index);
	SetByte(buff, m_bResHpType, send_index);
	SetByte(buff, m_bAbnormalType, send_index);
	SetByte(buff, 0x00, send_index);
	SetByte(buff, 0x00, send_index);
	SetByte(buff, 0x00, send_index);
	SetByte(buff, m_bNeedParty, send_index);
	SetByte(buff, m_pUserData->m_bAuthority, send_index);
	for (int i = 0; i < 7; i++) SetByte(buff, 0x00, send_index); // Extra padding
	SetByte(buff, -1, send_index);
	SetByte(buff, -1, send_index);

	// Item Listesi (Kritik Alan - Taşma Genelde Burada Olur)
	for (int i = 0; i < 8; i++) { // BREAST'den LEFTHAND'a kadar ana itemlar
		SetDWORD(buff, m_pUserData->m_sItemArray[i].nNum, send_index);
		SetShort(buff, m_pUserData->m_sItemArray[i].sDuration, send_index);
		SetByte(buff, 0x00, send_index);
	}

	// Ekstra Slotlar (Boş gönderiliyor)
	for (int i = 0; i < 4; i++) {
		SetDWORD(buff, 0x00, send_index);
		SetShort(buff, 0x00, send_index);
		SetByte(buff, 0x00, send_index);
	}

	SetByte(buff, m_pUserData->m_bZone, send_index);

	// Paketleri Gönder
	m_pMain->Send_Region(buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, this);

	if (m_bAbnormalType != ABNORMAL_BLINKING) {
		send_index = 0;
		memset(buff, 0x00, 512);
		SetByte(buff, AG_USER_INOUT, send_index);
		SetByte(buff, Type, send_index);
		SetShort(buff, m_Sid, send_index);
		iLength = strlen(m_pUserData->m_id);
		SetShort(buff, (short)iLength, send_index);
		SetString(buff, m_pUserData->m_id, iLength, send_index);
		Setfloat(buff, m_pUserData->m_curx, send_index);
		Setfloat(buff, m_pUserData->m_curz, send_index);
		m_pMain->Send_AIServer(m_pUserData->m_bZone, buff, send_index);
	}
}


void CUser::Rotate( char* pBuf )
{
	int index = 0, send_index = 0;
	int uid = -1;
	BYTE type = 0x00;
	char buff[256];
	memset( buff, NULL, 256 );
	short dir;

	dir = GetShort( pBuf, index );

	SetByte( buff, WIZ_ROTATE, send_index );
	SetShort( buff, m_Sid, send_index );
	SetShort( buff, dir, send_index );

	m_pMain->Send_Region( buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );
}

void CUser::Attack(char *pBuf)
{
	int index = 0, send_index = 0;
	int sid = -1, tid = -1, damage = 0; float delaytime = 0.0f ; float distance = 0.0f ;	
	BYTE type, result;	
	char buff[256]; memset( buff, 0x00, 256 );


//	CUser* pUser = NULL;
	CUser* pTUser = NULL;
	CNpc* pNpc  = NULL;
	_ITEM_TABLE* pTable = NULL;

	type = GetByte( pBuf, index );
	result = GetByte( pBuf, index );
//	sid = GetShort( pBuf, index );
	tid = GetShort( pBuf, index );
// ºñ·¯¸Ó±Û ÇØÅ·Åø À¯Àú --;
	delaytime = GetShort( pBuf, index );
	distance = GetShort( pBuf,index );
//

//	delaytime = delaytime / 100.0f;
//	distance = distance / 10.0f;	// 'Coz the server multiplies it by 10 before they send it to you.

/*
	if( sid < 0 || sid >= MAX_USER || tid < 0) return;

	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sid];
	if(!pUser || (pUser->m_Sid != m_Sid ) || pUser->m_bResHpType == USER_BLINKING) return;
*/
	if (m_bAbnormalType == ABNORMAL_BLINKING) return;

	if( m_bResHpType == USER_DEAD || m_pUserData->m_sHp == 0 )	{
		printf("### Attack Fail : name=%s(%d), m_bResHpType=%d, hp=%d###\n", m_pUserData->m_id, m_Sid, m_bResHpType, m_pUserData->m_sHp);
		return;
	}
// ºñ·¯¸Ó±Û ÇØÅ·Åø À¯Àú --;
	pTable = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[RIGHTHAND].nNum);	// This checks if such an item exists.
	if(!pTable && m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0) return;
	
	if (pTable) {	// If you're holding a weapon, do a delay check.
//		printf("Delay time : %f  ,  Table Delay Time : %f \r\n", delaytime, pTable->m_sDelay / 100.0f);
//		if (delaytime + 0.01f < (pTable->m_sDelay / 100.0f)) {
		if (delaytime < pTable->m_sDelay) {
			return;	
		}
	}
	else {		// Empty handed.
//		if (delaytime + 0.01f < 1.0f) { 
		if (delaytime < 100) {
			return;			
		}
	}
//
	if(tid < NPC_BAND) {	// USER
		if( tid >= MAX_USER || tid < 0 ) return;
		pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];
 
		if( !pTUser || pTUser->m_bResHpType == USER_DEAD || pTUser->m_bAbnormalType == ABNORMAL_BLINKING
		    || pTUser->m_pUserData->m_bNation == m_pUserData->m_bNation ) 
			result = 0x00;
		else {
// ºñ·¯¸Ó±Û ÇØÅ·Åø À¯Àú --;
			if (pTable) {	// Check if the user is holding a weapon!!! No null pointers allowed!!!
//				printf("Distance : %f  , Table Distance : %f  \r\n", distance, pTable->m_sRange / 10.0f);
//				if ( distance > (pTable->m_sRange / 10.0f)) {
				if ( distance > pTable->m_sRange ) {
					return;
				}
			}
//
			damage = GetDamage(tid, 0);

			// ´«½Î¿òÀüÀïÁ¸¿¡¼­ ´«½Î¿òÁßÀÌ¶ó¸é °ø°İÀº ´«À» ´øÁö´Â °Í¸¸ °¡´ÉÇÏµµ·Ï,,,
			if( m_pUserData->m_bZone == ZONE_SNOW_BATTLE && m_pMain->m_byBattleOpen == SNOW_BATTLE )	{
				damage = 0;		
			}

			if( damage <= 0 )
				result = 0x00;
			else {
				pTUser->HpChange( -damage, 0, true );
				ItemWoreOut( ATTACK, damage );
				pTUser->ItemWoreOut( DEFENCE, damage );
				//printf("%s - HP:%d, (damage-%d, t_hit-%d)\n", pTUser->m_pUserData->m_id, pTUser->m_pUserData->m_sHp, damage, m_sTotalHit);
				if( pTUser->m_pUserData->m_sHp == 0) {
					result = 0x02;
					pTUser->m_bResHpType = USER_DEAD;

					// sungyong work : loyalty					
					if( m_sPartyIndex == -1 ) {    // Something regarding loyalty points.
						LoyaltyChange(tid);
					}
					else {
						LoyaltyDivide(tid);
					}

					GoldChange(tid, 0);

					// ±â¹üÀÌÀÇ ¿Ïº®ÇÑ º¸È£ ÄÚµù!!!
					pTUser->InitType3();	// Init Type 3.....
					pTUser->InitType4();	// Init Type 4.....

					memset( buff, 0x00, 256 );	send_index = 0;
					if( pTUser->m_pUserData->m_bFame == COMMAND_CAPTAIN )	{	// ÁöÈÖ±ÇÇÑÀÌ ÀÖ´Â À¯Àú°¡ Á×´Â´Ù¸é,, ÁöÈÖ ±ÇÇÑ ¹ÚÅ»
						pTUser->m_pUserData->m_bFame = CHIEF;
						SetByte( buff, WIZ_AUTHORITY_CHANGE, send_index );
						SetByte( buff, COMMAND_AUTHORITY, send_index );
						SetShort( buff, pTUser->GetSocketID(), send_index );
						SetByte( buff, pTUser->m_pUserData->m_bFame, send_index );
						m_pMain->Send_Region( buff, send_index, pTUser->m_pUserData->m_bZone, pTUser->m_RegionX, pTUser->m_RegionZ );

						Send( buff, send_index );
						//printf("---> UserAttack Dead Captain Deprive - %s\n", pTUser->m_pUserData->m_id);
						if( pTUser->m_pUserData->m_bNation == KARUS )			m_pMain->Announcement( KARUS_CAPTAIN_DEPRIVE_NOTIFY, KARUS );
						else if( pTUser->m_pUserData->m_bNation == ELMORAD )	m_pMain->Announcement( ELMORAD_CAPTAIN_DEPRIVE_NOTIFY, ELMORAD );
					}

					pTUser->m_sWhoKilledMe = m_Sid;		// You killed me, you.....
//
					if( pTUser->m_pUserData->m_bZone != pTUser->m_pUserData->m_bNation && pTUser->m_pUserData->m_bZone < 3) {
						pTUser->ExpChange(-pTUser->m_iMaxExp / 100);
						//printf("Á¤¸»·Î 1%¸¸ ±ï¿´´Ù´Ï±î¿ä ¤Ğ.¤Ğ\r\n");
					}
//
				}
				SendTargetHP( 0, tid, -damage );
			}
		}
	}
	else if(tid >= NPC_BAND) { // NPC
		if( m_pMain->m_bPointCheckFlag == FALSE)	return;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ
		pNpc = m_pMain->m_arNpcArray.GetData(tid);		
		if( pNpc && pNpc->m_NpcState != NPC_DEAD && pNpc->m_iHP > 0 ) {	// Npc »óÅÂ Ã¼Å©..
// ºñ·¯¸Ó±Û ÇØÅ·Åø À¯Àú --;
			if (pTable) {	// Check if the user is holding a weapon!!! No null pointers allowed!!!
//				printf("Distance : %f  , Table Distance : %f  \r\n", distance, pTable->m_sRange / 10.0f);
//				if ( distance > (pTable->m_sRange / 10.0f)) {
				if ( distance > pTable->m_sRange ) {
					return;
				}
//				printf("Success!!! \r\n");
			}
//
			memset( buff, 0x00, 256 );	send_index = 0;
			SetByte( buff, AG_ATTACK_REQ, send_index );
			SetByte( buff, type, send_index );
			SetByte( buff, result, send_index ); 
//			SetShort( buff, sid, send_index );
			SetShort( buff, m_Sid, send_index );
			SetShort( buff, tid, send_index );
			SetShort( buff, m_sTotalHit * m_bAttackAmount / 100, send_index );   // Ç¥½Ã
			SetShort( buff, m_sTotalAc + m_sACAmount, send_index );   // Ç¥½Ã
			Setfloat( buff, m_sTotalHitrate, send_index );
			Setfloat( buff, m_sTotalEvasionrate, send_index ); 
			SetShort( buff, m_sItemAc, send_index);
			SetByte( buff, m_bMagicTypeLeftHand, send_index);
			SetByte( buff, m_bMagicTypeRightHand, send_index);
			SetShort( buff, m_sMagicAmountLeftHand, send_index);
			SetShort( buff, m_sMagicAmountRightHand, send_index);
			m_pMain->Send_AIServer(m_pUserData->m_bZone, buff, send_index);	// AI ServerÂÊÀ¸·Î Á¤º¸ Àü¼Û..
			return;
		}
	}

	memset( buff, 0x00, 256 );	send_index = 0;
	SetByte( buff, WIZ_ATTACK, send_index );
	SetByte( buff, type, send_index );
	SetByte( buff, result, send_index );
//	SetShort( buff, sid, send_index );
	SetShort( buff, m_Sid, send_index );
	SetShort( buff, tid, send_index );
	m_pMain->Send_Region( buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );

	if( tid < NPC_BAND )	{
		if( result == 0x02 )	{
			if( pTUser )	{
				// À¯Àú¿¡°Ô´Â ¹Ù·Î µ¥µå ÆĞÅ¶À» ³¯¸²... (ÇÑ ¹ø ´õ º¸³¿, À¯·ÉÀ» ¾ø¾Ö±â À§ÇØ¼­)
				pTUser->Send( buff, send_index );
				memset(buff, 0x00, 256);
				wsprintf(buff, "*** User Attack Dead, id=%s, result=%d, type=%d, HP=%d", pTUser->m_pUserData->m_id, result, pTUser->m_bResHpType, pTUser->m_pUserData->m_sHp);
				Timeprintf(buff);
			}
		}
	}
}

void CUser::SendMyInfo()
{
	C3DMap* pMap = nullptr;

	if (m_pMain == nullptr
		|| m_pUserData == nullptr)
		return;

	pMap = m_pMain->m_ZoneArray[m_iZoneIndex];

	if (pMap == nullptr)
		return;

	//---------------------------------------------------
	// POSITION FIX
	//---------------------------------------------------
	if (!pMap->IsValidPosition(
		m_pUserData->m_curx,
		m_pUserData->m_curz,
		0.0f))
	{
		short x = 0, z = 0;

		GetStartPosition(x, z);

		m_pUserData->m_curx = (float)x;
		m_pUserData->m_curz = (float)z;
	}

	//---------------------------------------------------
	// MAIN PACKET
	//---------------------------------------------------
	char send_buff[32768];
	memset(send_buff, 0, sizeof(send_buff));

	int send_index = 0;

	SetByte(send_buff, WIZ_MYINFO, send_index);

	SetShort(send_buff, m_Sid, send_index);

	//---------------------------------------------------
	// NAME
	//---------------------------------------------------
	int id_len = strlen(m_pUserData->m_id);

	SetByte(send_buff, id_len, send_index);

	SetString(send_buff,
		m_pUserData->m_id,
		id_len,
		send_index);

	//---------------------------------------------------
	// POSITION
	//---------------------------------------------------
	SetShort(send_buff,
		(uint16_t)(m_pUserData->m_curx * 10),
		send_index);

	SetShort(send_buff,
		(uint16_t)(m_pUserData->m_curz * 10),
		send_index);

	SetShort(send_buff,
		(int16_t)(m_pUserData->m_cury * 10),
		send_index);

	//---------------------------------------------------
	// BASIC INFO
	//---------------------------------------------------
	SetByte(send_buff,
		m_pUserData->m_bNation,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bRace,
		send_index);

	SetShort(send_buff,
		m_pUserData->m_sClass,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bFace,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bHairColor,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bRank,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bTitle,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bLevel,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bPoints,
		send_index);

	//---------------------------------------------------
	// EXP / NP
	//---------------------------------------------------
	SetDWORD(send_buff,
		m_iMaxExp,
		send_index);

	SetDWORD(send_buff,
		m_pUserData->m_iExp,
		send_index);

	SetDWORD(send_buff,
		m_pUserData->m_iLoyalty,
		send_index);

	// monthly np
	SetDWORD(send_buff,
		m_pUserData->m_iLoyaltyMonthly,
		send_index);

	//---------------------------------------------------
	// CITY
	//---------------------------------------------------
	SetByte(send_buff,
		m_pUserData->m_bCity,
		send_index);

	//---------------------------------------------------
	// CLAN
	//---------------------------------------------------
	CKnights* pKnights = m_pMain->m_KnightsArray.GetData(m_pUserData->m_bKnights);

	SetShort(send_buff, m_pUserData->m_bKnights, send_index);
	SetByte(send_buff, m_pUserData->m_bFame, send_index);

	if (!pKnights)
	{
		SetShort(send_buff, 0, send_index); // Alliance
		SetByte(send_buff, 0, send_index);  // Flag
		SetByte(send_buff, 0, send_index);  // Name len
		SetByte(send_buff, 0, send_index);  // Grade
		SetByte(send_buff, 0, send_index);  // Rank
		SetShort(send_buff, 0, send_index); // MarkVersion
		SetShort(send_buff, 0, send_index); // Cape
	}
	else
	{
		int len = strlen(pKnights->m_strName);

		SetShort(send_buff, pKnights->m_sAlliance, send_index);
		SetByte(send_buff, pKnights->m_byFlag, send_index);

		SetByte(send_buff, len, send_index);
		SetString(send_buff, pKnights->m_strName, len, send_index);

		SetByte(send_buff, pKnights->m_byGrade, send_index);
		SetByte(send_buff, pKnights->m_byRanking, send_index);

		SetShort(send_buff, pKnights->m_sMarkVersion, send_index);
		SetShort(send_buff, pKnights->m_sCape, send_index);
	}

	//---------------------------------------------------
	// HP / MP
	//---------------------------------------------------
	SetShort(send_buff, m_iMaxHp, send_index);
	SetShort(send_buff, m_pUserData->m_sHp, send_index);

	SetShort(send_buff, m_iMaxMp, send_index);
	SetShort(send_buff, m_pUserData->m_sMp, send_index);

	SetShort(send_buff, m_sMaxWeight, send_index);
	SetShort(send_buff, m_sItemWeight, send_index);

	//---------------------------------------------------
	// STATS
	//---------------------------------------------------
	SetByte(send_buff, m_pUserData->m_bStr, send_index);
	SetByte(send_buff, m_sItemStr, send_index);

	SetByte(send_buff, m_pUserData->m_bSta, send_index);
	SetByte(send_buff, m_sItemSta, send_index);

	SetByte(send_buff, m_pUserData->m_bDex, send_index);
	SetByte(send_buff, m_sItemDex, send_index);

	SetByte(send_buff, m_pUserData->m_bIntel, send_index);
	SetByte(send_buff, m_sItemIntel, send_index);

	SetByte(send_buff, m_pUserData->m_bCha, send_index);
	SetByte(send_buff, m_sItemCham, send_index);

	//---------------------------------------------------
	// ATTACK / AC
	//---------------------------------------------------
	SetShort(send_buff, m_sTotalHit, send_index);
	SetShort(send_buff, m_sTotalAc, send_index);

	//---------------------------------------------------
	// RESIST
	//---------------------------------------------------
	SetByte(send_buff, m_bFireR, send_index);
	SetByte(send_buff, m_bColdR, send_index);
	SetByte(send_buff, m_bLightningR, send_index);
	SetByte(send_buff, m_bMagicR, send_index);
	SetByte(send_buff, m_bDiseaseR, send_index);
	SetByte(send_buff, m_bPoisonR, send_index);

	//---------------------------------------------------
	// GOLD
	//---------------------------------------------------
	SetDWORD(send_buff,
		m_pUserData->m_iGold,
		send_index);

	SetByte(send_buff,
		m_pUserData->m_bAuthority,
		send_index);

	//---------------------------------------------------
	// NATIONAL/PERSONAL RANK
	//---------------------------------------------------
	SetByte(send_buff, 0, send_index);
	SetByte(send_buff, 0, send_index);

	//---------------------------------------------------
	// SKILLS
	//---------------------------------------------------
	for (int i = 0; i < 9; i++)
	{
		SetByte(send_buff,
			m_pUserData->m_bstrSkill[i],
			send_index);
	}

	//---------------------------------------------------
	// SLOT ITEMS
	//---------------------------------------------------
	for (int i = 0; i < SLOT_MAX; i++)
	{
		_ITEM_DATA* pItem =
			&m_pUserData->m_sItemArray[i];

		SetDWORD(send_buff,
			pItem->nNum,
			send_index);

		SetShort(send_buff,
			pItem->sDuration,
			send_index);

		SetShort(send_buff,
			pItem->sCount,
			send_index);

		// rental
		SetByte(send_buff, 0, send_index);
		SetShort(send_buff, 0, send_index);
	}

	//---------------------------------------------------
	// INVENTORY
	//---------------------------------------------------
	for (int i = 0; i < HAVE_MAX; i++)
	{
		_ITEM_DATA* pItem =
			&m_pUserData->m_sItemArray[SLOT_MAX + i];

		SetDWORD(send_buff,
			pItem->nNum,
			send_index);

		SetShort(send_buff,
			pItem->sDuration,
			send_index);

		SetShort(send_buff,
			pItem->sCount,
			send_index);

		// rental
		SetByte(send_buff, 0, send_index);
		SetShort(send_buff, 0, send_index);
	}

	//---------------------------------------------------
	// CHICKEN / MANNER
	//---------------------------------------------------
	SetByte(send_buff, 0, send_index);
	SetByte(send_buff, 0, send_index);
	SetShort(send_buff, 0, send_index);

	SetByte(send_buff, 0, send_index);

	SetDWORD(send_buff, 0, send_index);

	Send(send_buff, send_index);



	// AI Server쪽으로 정보 전송..
	int  ai_send_index = 0;
	char ai_send_buff[256];
	memset(ai_send_buff, NULL, 256);

	SetByte(ai_send_buff, AG_USER_INFO, ai_send_index);
	SetShort(ai_send_buff, m_Sid, ai_send_index);
	SetShort(ai_send_buff, strlen(m_pUserData->m_id), ai_send_index);
	SetString(ai_send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), ai_send_index);
	SetByte(ai_send_buff, m_pUserData->m_bZone, ai_send_index);
	SetShort(ai_send_buff, m_iZoneIndex, ai_send_index);
	SetByte(ai_send_buff, m_pUserData->m_bNation, ai_send_index);
	SetByte(ai_send_buff, m_pUserData->m_bLevel, ai_send_index);
	SetShort(ai_send_buff, m_pUserData->m_sHp, ai_send_index);
	SetShort(ai_send_buff, m_pUserData->m_sMp, ai_send_index);
	SetShort(ai_send_buff, m_sTotalHit * m_bAttackAmount / 100, ai_send_index);  // 표시
	SetShort(ai_send_buff, m_sTotalAc + m_sACAmount, ai_send_index);  // 표시
	Setfloat(ai_send_buff, m_sTotalHitrate, ai_send_index);
	Setfloat(ai_send_buff, m_sTotalEvasionrate, ai_send_index);

	// Yookozuna
	SetShort(ai_send_buff, m_sItemAc, ai_send_index);
	SetByte(ai_send_buff, m_bMagicTypeLeftHand, ai_send_index);
	SetByte(ai_send_buff, m_bMagicTypeRightHand, ai_send_index);
	SetShort(ai_send_buff, m_sMagicAmountLeftHand, ai_send_index);
	SetShort(ai_send_buff, m_sMagicAmountRightHand, ai_send_index);
	SetByte(ai_send_buff, m_pUserData->m_bAuthority, ai_send_index);
	//
	m_pMain->Send_AIServer(m_pUserData->m_bZone, ai_send_buff, ai_send_index);
}


void CUser::Chat(char *pBuf)
{
	int index = 0, chatlen = 0, send_index = 0, tid = -1;
	BYTE type;
	CUser* pUser = NULL;
	char chatstr[1024]; memset( chatstr, NULL, 1024 );
	char finalstr[1024]; memset( finalstr, NULL, 1024 );
	char send_buff[1024]; memset( send_buff, NULL, 1024 );

	std::string buff;

	if( m_pUserData->m_bAuthority == 2 ) return;		// this user refused chatting
	type = GetByte( pBuf, index );
	chatlen = GetShort( pBuf, index );
	if( chatlen > 512 || chatlen <= 0 )
		return;
	GetString( chatstr, pBuf, chatlen, index );

	if( type == PUBLIC_CHAT ) { // /notice
		if( m_pUserData->m_bAuthority != 0 ) return;
		//sprintf( finalstr, "#### °øÁö : %s ####", chatstr );
		::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff);
		sprintf( finalstr, buff.c_str(), chatstr );
	}
	else if( type == TOP_PUBLIC_CHAT ) { // 0x11 : /noticeall
			if( m_pUserData->m_bAuthority != 0 ) return;
			type = WAR_SYSTEM_CHAT; // WAR_SYSTEM_CHAT
			::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff);
			sprintf( finalstr, buff.c_str(), chatstr );
	}
////// GM Commands
	/*
	else if( _strnicmp( "+zonechange", chatstr, 11 ) == 0 ) {
		if( m_pUserData->m_bAuthority != 0 ) return;
		CString zonestr;
		zonestr = chatstr;
		zonestr = zonestr.Mid(12,6);
		int zoneno = atoi( zonestr );

		m_iZoneIndex = zoneno;

		if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		C3DMap* pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
		if( !pMap ) return;

		ZoneChange( zoneno, m_pUserData->m_curx, m_pUserData->m_curz );
	}*/
	else if (_strnicmp("+zonechange", chatstr, 11) == 0)
	{
		if (m_pUserData->m_bAuthority != 0)
			return;

		// -----------------------------
		// 1. SAFE PARSE
		// -----------------------------
		CString zonestr = chatstr;
		zonestr.Delete(0, 11); // "+zonechange "

		zonestr.Trim();

		int zoneno = atoi(zonestr);
		if (zoneno <= 0)
		{
			printf("[ZONE CMD] Invalid input: %s\n", chatstr);
			return;
		}

		printf("[ZONE CMD] request zone=%d user=%s\n",
			zoneno, m_pUserData->m_id);

		// -----------------------------
		// 2. ZONE INDEX CHECK
		// -----------------------------
		int zoneindex = m_pMain->GetZoneIndex(zoneno);
		if (zoneindex < 0 || zoneindex >= (int)m_pMain->m_ZoneArray.size())
		{
			printf("[ZONE CMD] ZoneIndex not found: %d\n", zoneno);
			return;
		}

		C3DMap* pMap = m_pMain->m_ZoneArray[zoneindex];
		if (!pMap)
		{
			printf("[ZONE CMD] Map null zone=%d index=%d\n", zoneno, zoneindex);
			return;
		}

		// -----------------------------
		// 3. TOWN / SAFE ZONE CHECK
		// -----------------------------
	

		// -----------------------------
		// 4. BATTLE RULES
		// -----------------------------
		if (m_pMain->m_byBattleOpen == NATION_BATTLE)
		{
			if (pMap->m_bType == 1 &&
				m_pUserData->m_bNation != zoneno)
			{
				printf("[ZONE BLOCK] Nation mismatch user=%d zone=%d\n",
					m_pUserData->m_bNation, zoneno);
				return;
			}
		}

		// -----------------------------
		// 5. SAVE TARGET ZONE (CRITICAL FIX)
		// -----------------------------
		m_iZoneIndex = zoneindex;

		// -----------------------------
		// 6. EXECUTE ZONE CHANGE
		// -----------------------------
		ZoneChange(zoneno,
			m_pUserData->m_curx,
			m_pUserData->m_curz);
	}

	else if (_strnicmp("+shutdown", chatstr, 9) == 0)
	{
		if (m_pUserData->m_bAuthority != 0) return;
		StartShutdownSequence();
		
	}
	else if( _strnicmp( "+tp_all", chatstr, 7 ) == 0 ) {
		if( m_pUserData->m_bAuthority != 0 ) return;
		CString zonestr;
		zonestr = chatstr;
		zonestr = zonestr.Mid(8,6);
		int zoneno = atoi( zonestr ); int i = 0;

		m_iZoneIndex = zoneno;

		if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		C3DMap* pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
		if( !pMap ) return;

		for(i=0; i<MAX_USER; i++ ) {
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
			if( pUser && pUser->GetState() == STATE_GAMESTART ) {
				pUser->ZoneChange( zoneno, pUser->m_pUserData->m_curx, pUser->m_pUserData->m_curz );
			}
		}
	}
	else if( _strnicmp( "+giveitem", chatstr, 9 ) == 0 ) { // item yolla
		if( m_pUserData->m_bAuthority != 0 ) return;
		CString itemnum;
		itemnum = chatstr;
		itemnum = itemnum.Mid(10,9);
		int itemno = atoi( itemnum );

		GiveItem( itemno , 1);
	}
//////
	else {
		sprintf( finalstr, "%s : %s", m_pUserData->m_id, chatstr );
	}

	SetByte( send_buff, WIZ_CHAT, send_index );
	SetByte( send_buff, type, send_index );
	SetByte( send_buff, m_pUserData->m_bNation, send_index );
	SetShort( send_buff, m_Sid, send_index );
	if( type == PRIVATE_CHAT ) {
		SetByte( send_buff, strlen(m_pUserData->m_id), send_index );
		SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
		SetShort( send_buff, strlen(chatstr), send_index );
		SetString( send_buff, chatstr, strlen(chatstr), send_index );
	} else {
		SetByte( send_buff, 0x00, send_index );
		SetShort( send_buff, strlen(finalstr), send_index );
		SetString( send_buff, finalstr, strlen(finalstr), send_index );
	}

	switch(type) {
		case GENERAL_CHAT:
			m_pMain->Send_NearRegion( send_buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, m_pUserData->m_curx, m_pUserData->m_curz );
			break;
		case PRIVATE_CHAT:
//			if( m_sPrivateChatUser < 0 || m_sPrivateChatUser >= MAX_USER ) break;
//			if( m_sPrivateChatUser == m_Sid) break;		// ÀÌ°Ç ³»°¡ Ãß°¡ÇßÁö·Õ :P
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sPrivateChatUser];
			if( !pUser || (pUser->GetState() != STATE_GAMESTART) ) break;
			pUser->Send( send_buff, send_index );
			break;
		case PARTY_CHAT:
			m_pMain->Send_PartyMember( m_sPartyIndex, send_buff, send_index );
			break;
		case FORCE_CHAT:
			break;
		case SHOUT_CHAT:
			if( m_pUserData->m_sMp < (m_iMaxMp/5) ) break;
			MSpChange( -(m_iMaxMp/5) );
			m_pMain->Send_Region( send_buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );
			break;
		case KNIGHTS_CHAT:
			m_pMain->Send_KnightsMember( m_pUserData->m_bKnights, send_buff, send_index, m_pUserData->m_bZone );
			break;
		case PUBLIC_CHAT:
			m_pMain->Send_All( send_buff, send_index );
			break;
		case COMMAND_CHAT:
			if( m_pUserData->m_bFame == COMMAND_CAPTAIN )		// ÁöÈÖ±ÇÀÚ¸¸ Ã¤ÆÃÀÌ µÇµµ·Ï
				m_pMain->Send_CommandChat( send_buff, send_index, m_pUserData->m_bNation, this );
			break;
		case MERCHANT_CHAT:
			sprintf( finalstr, "%s : %s(%f,%f)", m_pUserData->m_id, chatstr, m_pUserData->m_curx, m_pUserData->m_curz );
			m_pMain->Send_Region( send_buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );
			break;
		case WAR_SYSTEM_CHAT:
			m_pMain->Send_All( send_buff, send_index );
			break;
	}
}

void CUser::StartShutdownSequence()
{
	int send_index = 0;

	char chatstr[1024]; memset(chatstr, NULL, 1024);
	char finalstr[1024]; memset(finalstr, NULL, 1024);
	char send_buff[1024]; memset(send_buff, NULL, 1024);

	std::string buff;
	std::string buff2;

	for (int i = 5; i >= 1; i--)
	{
		::_LoadStringFromResource(IDS_SERVER_SHOUTDOWN, buff);
		sprintf(chatstr, buff.c_str());
		Sleep(1000);
	}

	g_serverdown_flag = TRUE;
	::SuspendThread(g_pMain->m_Iocport.m_hAcceptThread);
	g_pMain->KickOutAllUsers();   // server loop break için
}

void CUser::SetMaxHp(int iFlag)
{
	_CLASS_COEFFICIENT* p_TableCoefficient = NULL;
	p_TableCoefficient = m_pMain->m_CoefficientArray.GetData( m_pUserData->m_sClass );
	if( !p_TableCoefficient ) return;

	int temp_sta = 0;
	temp_sta = m_pUserData->m_bSta + m_sItemSta + m_bStaAmount;
//	if( temp_sta > 255 ) temp_sta = 255;

	if( m_pUserData->m_bZone == ZONE_SNOW_BATTLE && iFlag == 0 )	{
		m_iMaxHp = 100;
		//printf("--> SetMaxHp - name=%s, max=%d, hp=%d\n", m_pUserData->m_id, m_iMaxHp, m_pUserData->m_sHp);
	}
	else	{
		m_iMaxHp = (short)(((p_TableCoefficient->HP * m_pUserData->m_bLevel * m_pUserData->m_bLevel * temp_sta ) 
		      + (0.1 * m_pUserData->m_bLevel * temp_sta ) + (temp_sta / 5)) + m_sMaxHPAmount + m_sItemMaxHp);
		if( iFlag == 1 )	m_pUserData->m_sHp = m_iMaxHp + 20;		// Á¶±İ ´õ hp¸¦ ÁÖ¸é ÀÚµ¿À¸·Î hpchange()ÇÔ¼ö°¡ ½ÇÇàµÊ,, ²Ç¼ö^^*
		else if( iFlag == 2 )	m_iMaxHp = 100;
		//printf("<-- SetMaxHp - name=%s, max=%d, hp=%d\n", m_pUserData->m_id, m_iMaxHp, m_pUserData->m_sHp);
	}

	if(m_iMaxHp < m_pUserData->m_sHp) {
		m_pUserData->m_sHp = m_iMaxHp;
		HpChange( m_pUserData->m_sHp );
	}
	if( m_pUserData->m_sHp < 5 )
		m_pUserData->m_sHp = 5;
}

void CUser::SetMaxMp()
{
	_CLASS_COEFFICIENT* p_TableCoefficient = NULL;
	p_TableCoefficient = m_pMain->m_CoefficientArray.GetData( m_pUserData->m_sClass );
	if( !p_TableCoefficient ) return;

	int temp_intel = 0, temp_sta = 0;
	temp_intel = m_pUserData->m_bIntel + m_sItemIntel + m_bIntelAmount + 30;
//	if( temp_intel > 255 ) temp_intel = 255;
	temp_sta = m_pUserData->m_bSta + m_sItemSta + m_bStaAmount;
//	if( temp_sta > 255 ) temp_sta = 255;

	if( p_TableCoefficient->MP != 0)
	{
		m_iMaxMp = (short)((p_TableCoefficient->MP * m_pUserData->m_bLevel * m_pUserData->m_bLevel * temp_intel)
				  + (0.1f * m_pUserData->m_bLevel * 2 * temp_intel) + (temp_intel / 5));
		m_iMaxMp += m_sItemMaxMp;		
		m_iMaxMp += 20;		 // ¼º·¡¾¾ ¿äÃ»
	}
	else if( p_TableCoefficient->SP != 0)
	{
		m_iMaxMp = (short)((p_TableCoefficient->SP * m_pUserData->m_bLevel * m_pUserData->m_bLevel * temp_sta )
			  + (0.1f * m_pUserData->m_bLevel * temp_sta) + (temp_sta / 5));
		m_iMaxMp += m_sItemMaxMp;
	}

	if(m_iMaxMp < m_pUserData->m_sMp) {
		m_pUserData->m_sMp = m_iMaxMp;
		MSpChange( m_pUserData->m_sMp );
	}
}

void CUser::Regene(char *pBuf, int magicid)	// ³Ê¹« °³ÆÇÀÌ¶ó ³ªÁß¿¡ ¹İµå½Ã ¼öÁ¤ÇØ¾ß ÇÒ ÇÔ¼ö.... 
{
//	Corpse();		// Get rid of the corpse ~ ¶Ç »ç°íÄ¥»· ÇßÀÚ³ª ÀÌ ¹Ùº¸¾ß!!!

	InitType3();
	InitType4();

	CUser* pUser = NULL;
	_OBJECT_EVENT* pEvent = NULL;
	_HOME_INFO* pHomeInfo = NULL;
	_MAGIC_TYPE5* pType = NULL;

	int index = 0; BYTE regene_type = 0;

	regene_type = GetByte(pBuf, index);

	if (regene_type != 1 && regene_type != 2) {
		regene_type = 1;
	}

	if (regene_type == 2) {
		magicid = 490041;	// The Stone of Ressurection magic ID

		if(ItemCountChange(379006000, 1, 3 * m_pUserData->m_bLevel) < 2) {
			return;	// Subtract resurrection stones.
		}

		if (m_pUserData->m_bLevel <= 5) {
			return;	// 5 level minimum.
		}
	}

	pHomeInfo = m_pMain->m_HomeArray.GetData(m_pUserData->m_bNation);
	if (!pHomeInfo) return;

	int send_index = 0; char send_buff[1024];
	memset( send_buff, NULL, 1024 );

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;

	UserInOut(USER_OUT);	// ¿ø·¡´Â ÀÌ ÇÑÁÙ¹Û¿¡ ¾ø¾úÀ½ --;

	float x = 0.0f, z = 0.0f;
	x = (float)(myrand( 0, 400 )/100.0f);	z = (float)(myrand( 0, 400 )/100.0f);
	if( x < 2.5f )	x = 1.5f + x;
	if( z < 2.5f )	z = 1.5f + z;

	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent(m_pUserData->m_sBind);	

	if (magicid == 0) {
		if( pEvent && pEvent->byLife == 1 ) {		// Bind Point
			m_pUserData->m_curx = m_fWill_x = pEvent->fPosX + x;
			m_pUserData->m_curz = m_fWill_z = pEvent->fPosZ + z;
			m_pUserData->m_cury = 0;
		}
		else if( m_pUserData->m_bNation != m_pUserData->m_bZone) {	// Free Zone or Opposite Zone
			if(m_pUserData->m_bZone > 200) {		// Frontier Zone...
				x = pHomeInfo->FreeZoneX + myrand(0, pHomeInfo->FreeZoneLX);
				z = pHomeInfo->FreeZoneZ + myrand(0, pHomeInfo->FreeZoneLZ);
			}
//
			else if(m_pUserData->m_bZone > 100 && m_pUserData->m_bZone < 200) {		// Battle Zone...
/*
				m_bResHpType = USER_STANDING;
				HpChange( m_iMaxHp );
				KickOutZoneUser();	// Go back to your own zone!
				return;
*/
				x = pHomeInfo->BattleZoneX + myrand(0, pHomeInfo->BattleZoneLX);
				z = pHomeInfo->BattleZoneZ + myrand(0, pHomeInfo->BattleZoneLZ);
// ºñ·¯¸Ó±Û °³Ã´Á¸ ¹Ù²Ù¾îÄ¡±â >.<
				if (m_pUserData->m_bZone == ZONE_SNOW_BATTLE) {
					x = pHomeInfo->FreeZoneX + myrand(0, pHomeInfo->FreeZoneLX);
					z = pHomeInfo->FreeZoneZ + myrand(0, pHomeInfo->FreeZoneLZ);					
				}
//
			}
// ºñ·¯¸Ó±Û ´ºÁ¸ >.<
			else if (m_pUserData->m_bZone > 10 && m_pUserData->m_bZone < 20) {
				x = 527 + myrand(0, 10);
				z = 543 + myrand(0, 10);			
			}
//
			else if (m_pUserData->m_bZone < 3) {	// Specific Lands...
				if (m_pUserData->m_bNation == KARUS) {
					x = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
					z = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ);			
				}
				else if (m_pUserData->m_bNation == ELMORAD) {
					x = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
					z = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ);	
				}		
				else return;
			}

			m_pUserData->m_curx = x;
			m_pUserData->m_curz = z;
		}
		else {		//  ÃßÈÄ¿¡ Warp ¶û ÇÕÃÄ¾ß ÇÒ°Í °°À½...
			if (m_pUserData->m_bNation == KARUS) {
				x = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
				z = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ);			
			}
			else if (m_pUserData->m_bNation == ELMORAD) {			
				x = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
				z = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ);
			}		
			else return;		

			m_pUserData->m_curx = x;
			m_pUserData->m_curz = z;
		}
	}

	SetByte( send_buff, WIZ_REGENE, send_index );
//	SetShort( send_buff, m_Sid, send_index );    //
	SetShort(send_buff, (short)(m_pUserData->m_curx * 10), send_index);
	SetShort(send_buff, (short)(m_pUserData->m_curz * 10), send_index);
	SetShort(send_buff, (short)(m_pUserData->m_cury * 10), send_index);
	Send( send_buff, send_index );
//	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false ); //
	
	if (magicid > 0) {	// Clerical Resurrection.
		pType = m_pMain->m_Magictype5Array.GetData(magicid);     
		if ( !pType ) return;

		m_bAbnormalType = ABNORMAL_BLINKING;
		m_bResHpType = USER_STANDING;
		m_fBlinkStartTime = TimeGet();			
		MSpChange(-m_iMaxMp);					// Empty out MP.

		if (m_sWhoKilledMe == -1 && regene_type == 1) {		
			ExpChange((m_iLostExp * pType->bExpRecover) / 100);		// Restore Target Experience.
		}

		m_bRegeneType = REGENE_MAGIC;
	}
	else {		// Normal Regene.
//
		m_bAbnormalType = ABNORMAL_BLINKING;
		m_fBlinkStartTime = TimeGet();			
//
		m_bResHpType = USER_STANDING;	
//		HpChange( m_iMaxHp );
		m_bRegeneType = REGENE_NORMAL;
	}

//	ºñ·¯¸Ó±Û Å¬·£ ¼ÒÈ¯!!!
	m_fLastRegeneTime = TimeGet();
//
	m_sWhoKilledMe = -1;
	m_iLostExp = 0;
//	
	if (m_bAbnormalType != ABNORMAL_BLINKING) {
		// AI_server·Î regeneÁ¤º¸ Àü¼Û...	
		memset( send_buff, NULL, 1024 ); send_index=0;
		SetByte( send_buff, AG_USER_REGENE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetShort( send_buff, m_pUserData->m_sHp, send_index );
		m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
	}
//
	memset( send_buff, NULL, 1024 ); send_index=0;	// ÀÌ send_index´Â ¿Ö ¾ø¾úÀ»±î??? --;
	wsprintf(send_buff, "<------ User Regene ,, nid=%d, name=%s, type=%d ******", m_Sid, m_pUserData->m_id, m_bResHpType);
	//Timeprintf(send_buff);
	memset( send_buff, NULL, 1024 ); send_index=0;	// ÀÌ°Å È®ÀÎ»ç»ì·Î Ãß°¡Çß¾î¿ä!!!!
		
	m_RegionX = (int)(m_pUserData->m_curx / VIEW_DISTANCE);
	m_RegionZ = (int)(m_pUserData->m_curz / VIEW_DISTANCE);

	UserInOut(USER_REGENE);		

	m_pMain->RegionUserInOutForMe(this);
	m_pMain->RegionNpcInfoForMe(this);
	m_pMain->NpcInOutForMe(this);

	memset( send_buff, NULL, 1024); send_index = 0 ;		// Send Blinking State Change.....
	SetByte(send_buff, 3, send_index);
	SetByte(send_buff, m_bAbnormalType, send_index);
	StateChange(send_buff); 
	memset( send_buff, NULL, 1024); send_index = 0 ;	
//
	// Send Party Packet.....
	if (m_sPartyIndex != -1 && m_bType3Flag == FALSE) {
		memset( send_buff, NULL, 128); send_index = 0 ;
		SetByte( send_buff, WIZ_PARTY, send_index );
		SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetByte( send_buff, 1, send_index );
		SetByte( send_buff, 0x00, send_index);
		m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
	}
	//  end of Send Party Packet.....  //
//
//
	// Send Party Packet for Type 4
	if (m_sPartyIndex != -1 && m_bType4Flag == FALSE) {
		memset( send_buff, NULL, 128); send_index = 0 ;
		SetByte( send_buff, WIZ_PARTY, send_index );
		SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetByte( send_buff, 2, send_index );
		SetByte( send_buff, 0x00, send_index);
		m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
	}
	//  end of Send Party Packet.....  //
//
}

void CUser::ZoneChange(int zone, float x, float z)
{
	if (m_bZoneChangeFlag)
		return;

	m_bZoneChangeFlag = TRUE;

	int send_index = 0;
	int zoneindex = -1;

	char send_buff[256];
	memset(send_buff, 0x00, sizeof(send_buff));

	C3DMap* pMap = NULL;
	_ZONE_SERVERINFO* pInfo = NULL;

	//---------------------------------------------------
	// SERVER DOWN
	//---------------------------------------------------
	if (g_serverdown_flag)
		goto FAIL_RETURN;

	//---------------------------------------------------
	// ZONE INDEX
	//---------------------------------------------------
	zoneindex = m_pMain->GetZoneIndex(zone);

	if (zoneindex < 0 ||
		zoneindex >= (int)m_pMain->m_ZoneArray.size())
	{
		printf("[ZoneChange] Invalid zoneindex=%d zone=%d\n",
			zoneindex, zone);
		goto FAIL_RETURN;
	}

	pMap = m_pMain->m_ZoneArray[zoneindex];

	if (!pMap)
	{
		printf("[ZoneChange] Null map zone=%d\n", zone);
		goto FAIL_RETURN;
	}

	printf("[ZoneChange] user=%s zone=%d index=%d nation=%d type=%d\n",
		m_pUserData->m_id,
		zone,
		zoneindex,
		m_pUserData->m_bNation,
		pMap->m_bType);

	//---------------------------------------------------
	// FRONTIER LEVEL CHECK
	//---------------------------------------------------
	if (pMap->m_bType == 2)
	{
		if (m_pUserData->m_bLevel < 20
			&& m_pMain->m_byBattleOpen != SNOW_BATTLE)
		{
			goto FAIL_RETURN;
		}
	}

	//---------------------------------------------------
	// NATION CHECK
	//---------------------------------------------------
	if (zone == ZONE_KARUS &&
		m_pUserData->m_bNation != KARUS)
	{
		goto FAIL_RETURN;
	}

	if (zone == ZONE_ELMORAD &&
		m_pUserData->m_bNation != ELMORAD)
	{
		goto FAIL_RETURN;
	}

	//---------------------------------------------------
	// FRONTIER BLOCK
	//---------------------------------------------------
	if (m_pMain->m_byBattleOpen == NATION_BATTLE)
	{
		if (zone == ZONE_FRONTIER)
		{
			int temp_index = 0;
			char temp_buff[128];
			memset(temp_buff, 0x00, sizeof(temp_buff));

			SetByte(temp_buff, WIZ_WARP_LIST, temp_index);
			SetByte(temp_buff, 2, temp_index);
			SetByte(temp_buff, 0, temp_index);

			Send(temp_buff, temp_index);

			goto FAIL_RETURN;
		}
	}

	//---------------------------------------------------
	// USER OUT
	//---------------------------------------------------
	m_bWarp = 0x01;

	UserInOut(USER_OUT);

	//---------------------------------------------------
	// SNOW BATTLE HP FIX
	//---------------------------------------------------
	if (m_pUserData->m_bZone == ZONE_SNOW_BATTLE)
		SetMaxHp(1);

	//---------------------------------------------------
	// UPDATE USER DATA
	//---------------------------------------------------
	m_iZoneIndex = zoneindex;

	m_pUserData->m_bZone = zone;

	m_pUserData->m_curx = m_fWill_x = x;
	m_pUserData->m_curz = m_fWill_z = z;

	//---------------------------------------------------
	// REGION UPDATE
	//---------------------------------------------------
	m_RegionX = (int)(x / VIEW_DISTANCE);
	m_RegionZ = (int)(z / VIEW_DISTANCE);

	//---------------------------------------------------
	// PARTY REMOVE
	//---------------------------------------------------
	PartyRemove(m_Sid);

	//---------------------------------------------------
	// SERVER CHANGE
	//---------------------------------------------------
	if (m_pMain->m_nServerNo != pMap->m_nServerNo)
	{
		pInfo = m_pMain->m_ServerArray.GetData(
			pMap->m_nServerNo);

		if (!pInfo)
			goto FAIL_RETURN;

		UserDataSaveToAgent();

		m_pUserData->m_bLogout = 2;

		send_index = 0;

		SetByte(send_buff, WIZ_SERVER_CHANGE, send_index);

		SetShort(send_buff,
			strlen(pInfo->strServerIP),
			send_index);

		SetString(send_buff,
			pInfo->strServerIP,
			strlen(pInfo->strServerIP),
			send_index);

		SetShort(send_buff,
			pInfo->sPort,
			send_index);

		SetByte(send_buff, 0x02, send_index);

		SetByte(send_buff,
			m_pUserData->m_bZone,
			send_index);

		SetByte(send_buff,
			m_pMain->m_byOldVictory,
			send_index);

		Send(send_buff, send_index);

		m_bZoneChangeFlag = FALSE;
		return;
	}

	//---------------------------------------------------
	// CLIENT ZONE CHANGE
	//---------------------------------------------------
	send_index = 0;

	SetByte(send_buff, WIZ_ZONE_CHANGE, send_index);
	SetByte(send_buff, 0x03, send_index);

	SetShort(send_buff,
		(short)m_pUserData->m_bZone,
		send_index);

	SetShort(send_buff,
		(short)(m_pUserData->m_curx * 10.0f),
		send_index);

	SetShort(send_buff,
		(short)(m_pUserData->m_curz * 10.0f),
		send_index);

	SetShort(send_buff,
		(short)(m_pUserData->m_cury * 10.0f),
		send_index);

	SetByte(send_buff,
		m_pMain->m_byOldVictory,
		send_index);

	Send(send_buff, send_index);

	//---------------------------------------------------
	// GAMESTART
	//---------------------------------------------------
	send_index = 0;

	SetByte(send_buff, WIZ_GAMESTART, send_index);

	Send(send_buff, send_index);

	//---------------------------------------------------
	// REGEN RESET
	//---------------------------------------------------
	if (!m_bZoneChangeSameZone)
	{
		m_sWhoKilledMe = -1;
		m_iLostExp = 0;
		m_bRegeneType = 0;
		m_fLastRegeneTime = 0.0f;

		m_pUserData->m_sBind = -1;

		InitType3();
		InitType4();
	}
	else
	{
		m_bZoneChangeSameZone = FALSE;
	}

	//---------------------------------------------------
	// AI SERVER
	//---------------------------------------------------
	{
		int ai_send_index = 0;

		char ai_send_buff[128];
		memset(ai_send_buff, 0x00, sizeof(ai_send_buff));

		SetByte(ai_send_buff,
			AG_ZONE_CHANGE,
			ai_send_index);

		SetShort(ai_send_buff,
			m_Sid,
			ai_send_index);

		SetShort(ai_send_buff,
			(short)m_iZoneIndex,
			ai_send_index);

		SetShort(ai_send_buff,
			(short)m_pUserData->m_bZone,
			ai_send_index);

		m_pMain->Send_AIServer(
			m_pUserData->m_bZone,
			ai_send_buff,
			ai_send_index);
	}

	m_bZoneChangeFlag = FALSE;
	return;

FAIL_RETURN:

	m_bZoneChangeFlag = FALSE;
	return;
}

void CUser::Warp(char *pBuf)
{
	if( m_bWarp ) return;
//	if( m_pUserData->m_bAuthority != 0 ) return;

	int index = 0, send_index = 0;
	WORD warp_x, warp_z;
	float real_x, real_z;
	char	send_buff[128];
	memset( send_buff, NULL, 128 );
	C3DMap* pMap = NULL;

	warp_x = GetShort( pBuf, index );
	warp_z = GetShort( pBuf, index );

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	return;

	real_x = warp_x/10.0f; real_z = warp_z/10.0f;
	if( !pMap->IsValidPosition( real_x, real_z, 0.0f ) ) return;

	SetByte( send_buff, WIZ_WARP, send_index );
	SetShort( send_buff, warp_x, send_index );
	SetShort( send_buff, warp_z, send_index );
	Send( send_buff, send_index );

	UserInOut( USER_OUT );

	m_pUserData->m_curx = m_fWill_x = real_x;
	m_pUserData->m_curz = m_fWill_z = real_z;

	m_RegionX = (int)(m_pUserData->m_curx / VIEW_DISTANCE);
	m_RegionZ = (int)(m_pUserData->m_curz / VIEW_DISTANCE);

	//printf(" Warp ,, name=%s, x=%.2f, z=%.2f\n", m_pUserData->m_id, m_pUserData->m_curx, m_pUserData->m_curz);

	//UserInOut( USER_IN );
	UserInOut( USER_WARP );
	m_pMain->RegionUserInOutForMe(this);
	m_pMain->RegionNpcInfoForMe(this);
}

void CUser::SendTimeStatus()
{
	char	send_buff[256];
	memset( send_buff, NULL, 256 );
	int send_index = 0;

	SetByte( send_buff, WIZ_TIME, send_index );
	SetShort( send_buff, m_pMain->m_nYear, send_index );
	SetShort( send_buff, m_pMain->m_nMonth, send_index );
	SetShort( send_buff, m_pMain->m_nDate, send_index );
	SetShort( send_buff, m_pMain->m_nHour, send_index );
	SetShort( send_buff, m_pMain->m_nMin, send_index );
	Send( send_buff, send_index );

	send_index = 0;
	memset( send_buff, NULL, 256 );
	SetByte( send_buff, WIZ_WEATHER, send_index );
	SetByte( send_buff, (BYTE)m_pMain->m_nWeather, send_index );
	SetShort( send_buff, m_pMain->m_nAmount, send_index );
	Send( send_buff, send_index );
}

void CUser::SetDetailData()
{
	C3DMap* pMap = NULL;

	SetSlotItemValue();
	SetUserAbility();

	if (m_pUserData->m_bLevel >= MAX_LEVEL ) {
		Close();
	}

	m_iMaxExp = m_pMain->m_LevelUpArray[m_pUserData->m_bLevel-1]->m_iExp;
	m_sMaxWeight = (m_pUserData->m_bStr + m_sItemStr) * 50;

	m_iZoneIndex = m_pMain->GetZoneIndex( m_pUserData->m_bZone );

	if( m_iZoneIndex == -1 ) {	// ÀÌ ¼­¹ö¿¡ ¾ø´Â Á¸....
		Close();
	}

	m_fWill_x = m_pUserData->m_curx;
	m_fWill_z = m_pUserData->m_curz;
	m_fWill_y = m_pUserData->m_cury;

	m_RegionX = (int)(m_pUserData->m_curx / VIEW_DISTANCE);
	m_RegionZ = (int)(m_pUserData->m_curz / VIEW_DISTANCE);
}

void CUser::RegisterRegion()
{
	int iRegX = 0, iRegZ = 0, old_region_x = 0, old_region_z = 0;
	iRegX = (int)(m_pUserData->m_curx / VIEW_DISTANCE);
	iRegZ = (int)(m_pUserData->m_curz / VIEW_DISTANCE);

	if(m_RegionX != iRegX || m_RegionZ != iRegZ)
	{
		if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		C3DMap* pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
		if( !pMap )
			return;
		
		old_region_x = m_RegionX;	old_region_z = m_RegionZ;
		pMap->RegionUserRemove(m_RegionX, m_RegionZ, m_Sid);
		m_RegionX = iRegX;		m_RegionZ = iRegZ;
		pMap->RegionUserAdd(m_RegionX, m_RegionZ, m_Sid);

		if( m_State == STATE_GAMESTART ) {
			RemoveRegion( old_region_x - m_RegionX, old_region_z - m_RegionZ );	// delete user ´Â °è»ê ¹æÇâÀÌ ÁøÇà¹æÇâÀÇ ¹İ´ë...
			InsertRegion( m_RegionX - old_region_x, m_RegionZ - old_region_z );	// add user ´Â °è»ê ¹æÇâÀÌ ÁøÇà¹æÇâ...
			m_pMain->RegionNpcInfoForMe(this);
			m_pMain->NpcInOutForMe(this);
			m_pMain->RegionUserInOutForMe(this);
		}
//		printf("User¸¦ Region¿¡ µî·Ï,, region_x=%d, y=%d\n", m_RegionX, m_RegionZ);
	}
}

void CUser::RemoveRegion(int del_x, int del_z)
{
	int send_index = 0, i=0, iLength = 0;
	int region_x = -1, region_z = -1, uid = -1;
	char buff[256];
	memset( buff, NULL, 256 );
	C3DMap* pMap = NULL;
	CUser* pUser = NULL;
	CKnights* pKnights = NULL;

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )
		return;

	SetByte( buff, WIZ_USER_INOUT, send_index );
	SetByte( buff, USER_OUT, send_index );
	SetByte( buff, 0x00, send_index );
	SetShort( buff, m_Sid, send_index );
/*
/////////
	SetShort( buff, strlen(m_pUserData->m_id), send_index );
	SetString( buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
	SetByte( buff, m_pUserData->m_bNation, send_index );
	SetShort( buff, m_pUserData->m_bKnights, send_index );
	SetByte( buff, m_pUserData->m_bFame, send_index );

	if( m_pUserData->m_bKnights == 0 )	{
		SetShort( buff, 0, send_index );
		SetByte( buff, 0, send_index );
		SetByte( buff, 0, send_index );
	}
	else {
		pKnights = m_pMain->m_KnightsArray.GetData( m_pUserData->m_bKnights );
		if( pKnights )	{
			iLength = strlen(pKnights->strName);
			SetShort( buff, iLength, send_index );
			SetString( buff, pKnights->strName, iLength, send_index );
			SetByte( buff, pKnights->bGrade, send_index );  // knights grade
			SetByte( buff, pKnights->bRanking, send_index );  // knights grade
			//printf("removeregion knights index = %d, kname=%s, name=%s\n" , iLength, pKnights->strName, m_pUserData->m_id);
		}
		else	{
			SetShort( buff, 0, send_index );
			SetByte( buff, 0, send_index );
			SetByte( buff, 0, send_index );
		}
	}

	SetByte( buff, m_pUserData->m_bLevel, send_index );
	SetByte( buff, m_pUserData->m_bRace, send_index );
	SetShort( buff, m_pUserData->m_sClass, send_index );
	SetShort( buff, (WORD)m_pUserData->m_curx*10, send_index );
	SetShort( buff, (WORD)m_pUserData->m_curz*10, send_index );
	SetShort( buff, (short)m_pUserData->m_cury*10, send_index );
	SetByte( buff, m_pUserData->m_bFace, send_index );
	SetByte( buff, m_pUserData->m_bHairColor, send_index );
	SetByte( buff, m_bResHpType, send_index );
// ºñ·¯¸Ó±Û ¼ö´É...
	SetByte( buff, m_bAbnormalType, send_index );
//
	SetByte( buff, m_bNeedParty, send_index );
	SetByte( buff, m_pUserData->m_bAuthority, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[BREAST].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[BREAST].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[LEG].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[LEG].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[HEAD].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[HEAD].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[GLOVE].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[GLOVE].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[FOOT].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[FOOT].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[SHOULDER].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[SHOULDER].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[RIGHTHAND].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[RIGHTHAND].sDuration, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[LEFTHAND].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[LEFTHAND].sDuration, send_index );

*/	//////
	if( del_x != 0 ) {	// x ÃàÀ¸·Î ÀÌµ¿µÇ¾úÀ»¶§...
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x*2, m_RegionZ+del_z-1 );
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x*2, m_RegionZ+del_z );
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x*2, m_RegionZ+del_z+1 );
//		printf("Remove : (%d %d), (%d %d), (%d %d)\n", m_RegionX+del_x*2, m_RegionZ+del_z-1, m_RegionX+del_x*2, m_RegionZ+del_z, m_RegionX+del_x*2, m_RegionZ+del_z+1 );
	}
	if( del_z != 0 ) {	// z ÃàÀ¸·Î ÀÌµ¿µÇ¾úÀ»¶§...
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x, m_RegionZ+del_z*2 );
		if( del_x < 0 ) // x, z Ãà µÑ´Ù ÀÌµ¿µÇ¾úÀ»¶§ °ãÄ¡´Â ºÎºĞ ÇÑ¹ø¸¸ º¸³½´Ù..
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x+1, m_RegionZ+del_z*2 );
		else if( del_x > 0 )
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x-1, m_RegionZ+del_z*2 );
		else {
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x-1, m_RegionZ+del_z*2 );
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x+1, m_RegionZ+del_z*2 );
//			printf("Remove : (%d %d), (%d %d), (%d %d)\n", m_RegionX+del_x-1, m_RegionZ+del_z*2, m_RegionX+del_x, m_RegionZ+del_z*2, m_RegionX+del_x+1, m_RegionZ+del_z*2 );
		}
	}
}

void CUser::InsertRegion(int del_x, int del_z)
{
	int send_index = 0, buff_index = 0, uid_sendindex = 0, i=0;
	int region_x = -1, region_z = -1, user_count = 0, uid = -1, iLength=0;
	char buff[256];
	memset( buff, NULL, 256 );
	C3DMap* pMap = NULL;
	CUser* pUser = NULL;
	CKnights* pKnights = NULL;

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )
		return;

// TAMER
	SetByte( buff, WIZ_USER_INOUT, send_index );
	SetShort( buff, USER_IN, send_index );
	SetShort( buff, m_Sid, send_index );

	SetByte( buff, strlen(m_pUserData->m_id), send_index );
	SetString( buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );

	SetByte( buff, m_pUserData->m_bNation, send_index );
	SetShort( buff, m_pUserData->m_bKnights, send_index );
	SetShort( buff, m_pUserData->m_bFame, send_index );

	if( m_pUserData->m_bKnights == 0 )	{
		SetByte( buff, 0x00, send_index );
		SetByte( buff, 0x00, send_index );

		SetByte( buff, 0x00, send_index );

		SetByte( buff, 0x00, send_index );
		SetByte( buff, 0x00, send_index );
		SetByte( buff, 0x00, send_index );

		SetByte( buff, -1, send_index );
		SetByte( buff, -1, send_index );

		SetByte( buff, 0x00, send_index );
		SetByte( buff, 0x00, send_index );
		SetByte( buff, 0x00, send_index );
	}
	else {
		pKnights = m_pMain->m_KnightsArray.GetData( m_pUserData->m_bKnights );
		if( pKnights )	{
			iLength = strlen(pKnights->m_strName);
			SetByte( buff, pKnights->m_byRanking, send_index ); // Knights ranking
			SetByte( buff, iLength, send_index );
			SetString( buff, pKnights->m_strName, iLength, send_index );
			SetByte( buff, pKnights->m_byGrade, send_index ); // Knights grade

			SetByte( buff, 0x01, send_index );
			SetByte( buff, 0x01, send_index );
			SetByte( buff, 0x00, send_index );

			SetByte( buff, 0x40, send_index ); // Knights cloak_c
			SetByte( buff, 0x00, send_index ); // Knights cloak_m

			SetByte( buff, 0xFF, send_index ); // R
			SetByte( buff, 0x00, send_index ); // G
			SetByte( buff, 0xFF, send_index ); // B
		}
		else	{
			SetByte( buff, 0x00, send_index );
			SetByte( buff, 0x00, send_index );

			SetByte( buff, 0x00, send_index );

			SetByte( buff, 0x00, send_index );
			SetByte( buff, 0x00, send_index );
			SetByte( buff, 0x00, send_index );

			SetByte( buff, -1, send_index );
			SetByte( buff, -1, send_index );

			SetByte( buff, 0x00, send_index );
			SetByte( buff, 0x00, send_index );
			SetByte( buff, 0x00, send_index );
		}
	}

	SetByte( buff, m_pUserData->m_bRank, send_index );
	SetByte( buff, m_pUserData->m_bTitle, send_index );
	SetByte( buff, m_pUserData->m_bLevel, send_index );
	SetByte( buff, m_pUserData->m_bRace, send_index );
	SetShort( buff, m_pUserData->m_sClass, send_index );
	SetShort( buff, (WORD)m_pUserData->m_curx*10, send_index );
	SetShort( buff, (WORD)m_pUserData->m_curz*10, send_index );
	SetShort( buff, (short)m_pUserData->m_cury*10, send_index );

	SetByte( buff, m_pUserData->m_bFace, send_index );
	SetByte( buff, m_pUserData->m_bHairColor, send_index );
	SetByte( buff, m_bResHpType, send_index );
	SetByte( buff, m_bAbnormalType, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, m_bNeedParty, send_index );
	SetByte( buff, m_pUserData->m_bAuthority, send_index );

	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );

	SetByte( buff, -1, send_index );
	SetByte( buff, -1, send_index );

	SetDWORD( buff, m_pUserData->m_sItemArray[BREAST].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[BREAST].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[LEG].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[LEG].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[HEAD].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[HEAD].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[GLOVE].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[GLOVE].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[FOOT].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[FOOT].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[SHOULDER].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[SHOULDER].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[RIGHTHAND].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[RIGHTHAND].sDuration, send_index );
	SetByte( buff, 0x00, send_index );
	SetDWORD( buff, m_pUserData->m_sItemArray[LEFTHAND].nNum, send_index );
	SetShort( buff, m_pUserData->m_sItemArray[LEFTHAND].sDuration, send_index );
	SetByte( buff, 0x00, send_index );

	SetDWORD( buff, 0x00, send_index );
	SetShort( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );

	SetDWORD( buff, 0x00, send_index );
	SetShort( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );

	SetDWORD( buff, 0x00, send_index );
	SetShort( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );

	SetDWORD( buff, 0x00, send_index );
	SetShort( buff, 0x00, send_index );
	SetByte( buff, 0x00, send_index );

	SetByte( buff, m_pUserData->m_bZone, send_index );

	if( del_x != 0 ) {	// x ÃàÀ¸·Î ÀÌµ¿µÇ¾úÀ»¶§...
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x, m_RegionZ-1 );
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x, m_RegionZ );
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+del_x, m_RegionZ+1 );

//		printf("Insert : (%d %d), (%d %d), (%d %d)\n", m_RegionX+del_x, m_RegionZ-1, m_RegionX+del_x, m_RegionZ, m_RegionX+del_x, m_RegionZ+1 );
	}
	if( del_z != 0 ) {	// z ÃàÀ¸·Î ÀÌµ¿µÇ¾úÀ»¶§...
		m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX, m_RegionZ+del_z );
		
		if( del_x < 0 )	// x, z Ãà µÑ´Ù ÀÌµ¿µÇ¾úÀ»¶§ °ãÄ¡´Â ºÎºĞ ÇÑ¹ø¸¸ º¸³½´Ù..
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+1, m_RegionZ+del_z );		
		else if( del_x > 0 )
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX-1, m_RegionZ+del_z );
		else {
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX-1, m_RegionZ+del_z );
			m_pMain->Send_UnitRegion( buff, send_index, m_iZoneIndex, m_RegionX+1, m_RegionZ+del_z );
//			printf("Insert : (%d %d), (%d %d), (%d %d)\n", m_RegionX-1, m_RegionZ+del_z, m_RegionX, m_RegionZ+del_z, m_RegionX+1, m_RegionZ+del_z );
		}
	}
}

void CUser::RequestUserIn(char* pBuf)
{
	if (m_pMain->m_bPointCheckFlag == FALSE) return;

	int index = 0, uid = -1, user_count = 0, buff_index = 0, t_count = 0;
	CUser* pUser = NULL;
	CKnights* pKnights = NULL;

	// Stack taşmasını önlemek için static veya heap kullanımı önerilir
	static char buff[204800];
	memset(buff, 0x00, 204800);

	buff_index = 3; // İlk 3 byte: [WIZ_REQ_USERIN (1)] + [UserCount (2)]
	user_count = GetShort(pBuf, index);

	for (int i = 0; i < user_count; i++) {
		uid = GetShort(pBuf, index);
		if (uid < 0 || uid >= MAX_USER) continue;
		if (t_count >= 1000) break; // Maksimum paket sınırı

		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[uid];
		if (!pUser || pUser->GetState() != STATE_GAMESTART) continue;

		// --- PAKET BAŞLANGICI ---
		SetByte(buff, 0x00, buff_index); // Type (0x00: User)
		SetShort(buff, pUser->GetSocketID(), buff_index);

		// İsim
		int id_len = (int)strlen(pUser->m_pUserData->m_id);
		SetByte(buff, (BYTE)id_len, buff_index);
		SetString(buff, pUser->m_pUserData->m_id, id_len, buff_index);

		SetByte(buff, pUser->m_pUserData->m_bNation, buff_index);
		SetShort(buff, pUser->m_pUserData->m_bKnights, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bFame, buff_index);

		// --- CLAN BİLGİLERİ ---
		pKnights = m_pMain->m_KnightsArray.GetData(pUser->m_pUserData->m_bKnights);
		if (pKnights) {
			SetByte(buff, pKnights->m_byRanking, buff_index);
			int clan_len = (int)strlen(pKnights->m_strName);
			SetByte(buff, (BYTE)clan_len, buff_index);
			SetString(buff, pKnights->m_strName, clan_len, buff_index);
			SetByte(buff, pKnights->m_byGrade, buff_index);
			SetShort(buff, 0, buff_index); // Symbol
		}
		else {
			SetByte(buff, 0, buff_index); // Ranking 0
			SetByte(buff, 0, buff_index); // Clan Name Len 0
			SetByte(buff, 0, buff_index); // Grade 0
			SetShort(buff, 0, buff_index); // Symbol 0
		}
		SetShort(buff, -1, buff_index); // Cape (Pelerin)

		// --- STATLAR ---
		SetByte(buff, pUser->m_pUserData->m_bRank, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bTitle, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bLevel, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bRace, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sClass, buff_index);

		// Koordinatlar (*10)
		SetShort(buff, (short)(pUser->m_pUserData->m_curx * 10), buff_index);
		SetShort(buff, (short)(pUser->m_pUserData->m_curz * 10), buff_index);
		SetShort(buff, (short)(pUser->m_pUserData->m_cury * 10), buff_index);

		SetByte(buff, pUser->m_pUserData->m_bFace, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bHairColor, buff_index);

		SetByte(buff, pUser->m_bResHpType, buff_index);
		SetByte(buff, pUser->m_bAbnormalType, buff_index);
		SetByte(buff, pUser->m_bNeedParty, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bAuthority, buff_index);

		// --- ITEMLAR (Kıyafet ve Silah Slotları) ---
		// Standart 1298 Sırası: Breast, Leg, Head, Glove, Foot, Shoulder, RightHand, LeftHand
		int slots[] = { BREAST, LEG, HEAD, GLOVE, FOOT, SHOULDER, RIGHTHAND, LEFTHAND };
		for (int k = 0; k < 8; k++) {
			SetDWORD(buff, pUser->m_pUserData->m_sItemArray[slots[k]].nNum, buff_index);
			SetShort(buff, pUser->m_pUserData->m_sItemArray[slots[k]].sDuration, buff_index);
			// 🔥 Buradaki SetByte(buff, 0, buff_index) bazı özel exelerde gerekebilir. 
			// Eğer kıyafetler hala kayıksa her itemdan sonra 1 byte 0x00 ekle.
		}

		SetByte(buff, pUser->m_pUserData->m_bZone, buff_index);
		t_count++;
	}

	// Başlık verilerini paketin en başına yazıyoruz
	int temp_index = 0;
	SetByte(buff, WIZ_REQ_USERIN, temp_index);
	SetShort(buff, (short)t_count, temp_index);

	if (t_count > 0)
		Send(buff, buff_index);
}

void CUser::RequestNpcIn(char* pBuf)
{
	if (!m_pMain || !m_pMain->m_bPointCheckFlag)
		return;

	int index = 0;
	char buff[20480];
	memset(buff, 0, sizeof(buff));

	int send_index = 0;
	SetByte(buff, WIZ_REQ_NPCIN, send_index);
	int count_index = send_index;
	SetShort(buff, 0, send_index);

	int t_count = 0;
	short npc_count = GetShort(pBuf, index);
	if (npc_count < 0 || npc_count > 1000)
		return;

	for (int i = 0; i < npc_count; i++)
	{
		int nid = GetShort(pBuf, index);
		if (nid <= 0)
			continue;

		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);
		if (!pNpc)
			continue;

		// Exact 1298 Client MsgRecv_NPCIn order:
		// NID, Resource/Picture ID, Type, Trade ID, Scale, Weapon0, Weapon1,
		// BYTE name length, name, Nation, Level, X, Z, Y, Status, ObjectType,
		// two SHORTs and Direction.
		SetShort(buff, pNpc->m_sNid, send_index);
		SetShort(buff, pNpc->m_sPid, send_index);
		SetByte(buff, pNpc->m_tNpcType, send_index);
		SetDWORD(buff, pNpc->m_iSellingGroup, send_index);
		SetShort(buff, (pNpc->m_sSize <= 0 ? 100 : pNpc->m_sSize), send_index);
		SetDWORD(buff, pNpc->m_iWeapon_1, send_index);
		SetDWORD(buff, pNpc->m_iWeapon_2, send_index);

		int len = (int)strlen(pNpc->m_strName);
		if (len > 50) len = 50;
		SetByte(buff, (BYTE)len, send_index);
		if (len > 0)
			SetString(buff, pNpc->m_strName, len, send_index);

		SetByte(buff, pNpc->m_byGroup, send_index);
		SetByte(buff, pNpc->m_byLevel, send_index);
		SetShort(buff, (uint16_t)(pNpc->m_fCurX * 10.0f), send_index);
		SetShort(buff, (uint16_t)(pNpc->m_fCurZ * 10.0f), send_index);
		SetShort(buff, (int16_t)(pNpc->m_fCurY * 10.0f), send_index);
		SetDWORD(buff, (DWORD)pNpc->m_byGateOpen, send_index);
		SetByte(buff, pNpc->m_byObjectType, send_index);
		SetShort(buff, 0, send_index);
		SetShort(buff, 0, send_index);
		SetByte(buff, pNpc->m_byDirection, send_index);

		t_count++;

		if (send_index > (int)sizeof(buff) - 256)
			break;
	}

	int temp = count_index;
	SetShort(buff, t_count, temp);

	if (t_count > 0)
		Send(buff, send_index);
}


BYTE CUser::GetHitRate(float rate)
{
	BYTE result;
	int random = 0;
	random = myrand(1, 10000);

	if( rate >= 5.0 )
	{
		if( random >= 1 && random <= 3500)
			result = GREAT_SUCCESS;
		else if( random >= 3501 && random <= 7500)
			result = SUCCESS;
		else if( random >= 7501 && random <= 9800)
			result = NORMAL;
		else if( random >= 9801 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 5.0 && rate >= 3.0)
	{
		if( random >= 1 && random <= 2500)
			result = GREAT_SUCCESS;
		else if( random >= 2501 && random <= 6000)
			result = SUCCESS;
		else if( random >= 6001 && random <= 9600)
			result = NORMAL;
		else if( random >= 9601 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 3.0 && rate >= 2.0)
	{
		if( random >= 1 && random <= 2000)
			result = GREAT_SUCCESS;
		else if( random >= 2001 && random <= 5000)
			result = SUCCESS;
		else if( random >= 5001 && random <= 9400)
			result = NORMAL;
		else if( random >= 9401 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 2.0 && rate >= 1.25)
	{
		if( random >= 1 && random <= 1500)
			result = GREAT_SUCCESS;
		else if( random >= 1501 && random <= 4000)
			result = SUCCESS;
		else if( random >= 4001 && random <= 9200)
			result = NORMAL;
		else if( random >= 9201 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 1.25 && rate >= 0.8)
	{
		if( random >= 1 && random <= 1000)
			result = GREAT_SUCCESS;
		else if( random >= 1001 && random <= 3000)
			result = SUCCESS;
		else if( random >= 3001 && random <= 9000)
			result = NORMAL;
		else if( random >= 9001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}	
	else if ( rate < 0.8 && rate >= 0.5)
	{
		if( random >= 1 && random <= 800)
			result = GREAT_SUCCESS;
		else if( random >= 801 && random <= 2500)
			result = SUCCESS;
		else if( random >= 2501 && random <= 8000)
			result = NORMAL;
		else if( random >= 8001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.5 && rate >= 0.33)
	{
		if( random >= 1 && random <= 600)
			result = GREAT_SUCCESS;
		else if( random >= 601 && random <= 2000)
			result = SUCCESS;
		else if( random >= 2001 && random <= 7000)
			result = NORMAL;
		else if( random >= 7001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else if ( rate < 0.33 && rate >= 0.2)
	{
		if( random >= 1 && random <= 400)
			result = GREAT_SUCCESS;
		else if( random >= 401 && random <= 1500)
			result = SUCCESS;
		else if( random >= 1501 && random <= 6000)
			result = NORMAL;
		else if( random >= 6001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	else
	{
		if( random >= 1 && random <= 200)
			result = GREAT_SUCCESS;
		else if( random >= 201 && random <= 1000)
			result = SUCCESS;
		else if( random >= 1001 && random <= 5000)
			result = NORMAL;
		else if( random >= 5001 && random <= 10000)
			result = FAIL;
		else
			result = FAIL;
	}
	
	return result;
}

void CUser::SetSlotItemValue()	// Âø¿ëÇÑ ¾ÆÀÌÅÛÀÇ °ª(Å¸°İ·ü, È¸ÇÇÀ², µ¥¹ÌÁö)À» ±¸ÇÑ´Ù.
{
	_ITEM_TABLE* pTable = NULL;
	int item_hit = 0, item_ac = 0;

	m_sItemMaxHp = 0; m_sItemMaxMp = 0;
	m_sItemHit = 0; m_sItemAc = 0; m_sItemStr = 0; m_sItemSta = 0; m_sItemDex = 0; m_sItemIntel = 0;
	m_sItemCham = 0; m_sItemHitrate = 100; m_sItemEvasionrate = 100; m_sItemWeight = 0;	

	m_bFireR = 0; m_bColdR = 0; m_bLightningR = 0; m_bMagicR = 0; m_bDiseaseR = 0; m_bPoisonR = 0;
	
	m_sDaggerR = 0; m_sSwordR = 0; m_sAxeR = 0; m_sMaceR = 0; m_sSpearR = 0; m_sBowR = 0;
	m_bMagicTypeLeftHand = 0; m_bMagicTypeRightHand = 0; m_sMagicAmountLeftHand = 0; m_sMagicAmountRightHand = 0;       

	for(int i=0; i<SLOT_MAX; i++)  {
		if(m_pUserData->m_sItemArray[i].nNum <= 0)
			continue;
		pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[i].nNum );
		if( !pTable )
			continue;
		if( m_pUserData->m_sItemArray[i].sDuration == 0 ) {
			item_hit = pTable->m_sDamage / 2;
			item_ac = pTable->m_sAc / 2;
		}
		else {
			item_hit = pTable->m_sDamage;
			item_ac = pTable->m_sAc;
		}
		if( i == RIGHTHAND ) 	// ItemHit Only Hands
			m_sItemHit += item_hit;
		if( i == LEFTHAND ) {
			if( ( m_pUserData->m_sClass == BERSERKER || m_pUserData->m_sClass == BLADE ) )
	//			m_sItemHit += item_hit * (double)( m_pUserData->m_bstrSkill[PRO_SKILL1] / 60.0 );    // ¼º·¡¾¾ ¿äÃ» ^^;
				m_sItemHit += item_hit * 0.5f;
		}

		m_sItemMaxHp += pTable->m_MaxHpB;
		m_sItemMaxMp += pTable->m_MaxMpB;
		m_sItemAc += item_ac;
		m_sItemStr += pTable->m_bStrB;
		m_sItemSta += pTable->m_bStaB;
		m_sItemDex += pTable->m_bDexB;
		m_sItemIntel += pTable->m_bIntelB;
		m_sItemCham += pTable->m_bChaB;
		m_sItemHitrate += pTable->m_sHitrate;
		m_sItemEvasionrate += pTable->m_sEvarate;
//		m_sItemWeight += pTable->m_sWeight;

		m_bFireR += pTable->m_bFireR;
		m_bColdR += pTable->m_bColdR;
		m_bLightningR += pTable->m_bLightningR;
		m_bMagicR += pTable->m_bMagicR;
		m_bDiseaseR += pTable->m_bCurseR;
		m_bPoisonR += pTable->m_bPoisonR;

		m_sDaggerR += pTable->m_sDaggerAc;
		m_sSwordR += pTable->m_sSwordAc;
		m_sAxeR += pTable->m_sAxeAc;
		m_sMaceR += pTable->m_sMaceAc;
		m_sSpearR += pTable->m_sSpearAc;
		m_sBowR += pTable->m_sBowAc;
	}

// Also add the weight of items in the inventory....
	for(int i=0 ; i < HAVE_MAX+SLOT_MAX+EXTRA_MAX ; i++)  {
		if(m_pUserData->m_sItemArray[i].nNum <= 0) continue;

		pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[i].nNum );
		if( !pTable ) continue;

		if (pTable->m_bCountable == 0) {	// Non-countable items.
			m_sItemWeight += pTable->m_sWeight;
		}
		else {	// Countable items.
			m_sItemWeight += pTable->m_sWeight * m_pUserData->m_sItemArray[i].sCount;
		}
	}
//	
	if( m_sItemHit < 3 )
		m_sItemHit = 3;

	// For magical items..... by Yookozuna 2002.7.10
	_ITEM_TABLE* pLeftHand = NULL;			// Get item info for left hand.
	pLeftHand = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[LEFTHAND].nNum);
	if (pLeftHand) {
		if (pLeftHand->m_bFireDamage) {
			m_bMagicTypeLeftHand = 1;
			m_sMagicAmountLeftHand = pLeftHand->m_bFireDamage;
		}

		if (pLeftHand->m_bIceDamage) {
			m_bMagicTypeLeftHand = 2;
			m_sMagicAmountLeftHand = pLeftHand->m_bIceDamage;
		}

		if (pLeftHand->m_bLightningDamage) {
			m_bMagicTypeLeftHand = 3;
			m_sMagicAmountLeftHand = pLeftHand->m_bLightningDamage;
		}

		if (pLeftHand->m_bPoisonDamage) {
			m_bMagicTypeLeftHand = 4;
			m_sMagicAmountLeftHand = pLeftHand->m_bPoisonDamage;
		}

		if (pLeftHand->m_bHPDrain) {
			m_bMagicTypeLeftHand = 5;
			m_sMagicAmountLeftHand = pLeftHand->m_bHPDrain;
		}

		if (pLeftHand->m_bMPDamage) {
			m_bMagicTypeLeftHand = 6;
			m_sMagicAmountLeftHand = pLeftHand->m_bMPDamage;
		}

		if (pLeftHand->m_bMPDrain) {
			m_bMagicTypeLeftHand = 7;
			m_sMagicAmountLeftHand = pLeftHand->m_bMPDrain;
		}

		if (pLeftHand->m_bMirrorDamage)	{
			m_bMagicTypeLeftHand = 8;
			m_sMagicAmountLeftHand = pLeftHand->m_bMirrorDamage;	
		}
	}

	_ITEM_TABLE* pRightHand = NULL;			// Get item info for right hand.
	pRightHand = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[RIGHTHAND].nNum);
	if (pRightHand) {
		if (pRightHand->m_bFireDamage) {
			m_bMagicTypeRightHand = 1;
			m_sMagicAmountRightHand = pRightHand->m_bFireDamage;
		}

		if (pRightHand->m_bIceDamage) {
			m_bMagicTypeRightHand = 2;
			m_sMagicAmountRightHand = pRightHand->m_bIceDamage;
		}

		if (pRightHand->m_bLightningDamage) {
			m_bMagicTypeRightHand = 3;
			m_sMagicAmountRightHand = pRightHand->m_bLightningDamage;
		}

		if (pRightHand->m_bPoisonDamage) {
			m_bMagicTypeRightHand = 4;
			m_sMagicAmountRightHand = pRightHand->m_bPoisonDamage;
		}

		if (pRightHand->m_bHPDrain) {
			m_bMagicTypeRightHand = 5;
			m_sMagicAmountRightHand = pRightHand->m_bHPDrain;
		}

		if (pRightHand->m_bMPDamage) {
			m_bMagicTypeRightHand = 6;
			m_sMagicAmountRightHand = pRightHand->m_bMPDamage;
		}

		if (pRightHand->m_bMPDrain) {
			m_bMagicTypeRightHand = 7;
			m_sMagicAmountRightHand = pRightHand->m_bMPDrain;
		}

		if (pRightHand->m_bMirrorDamage) {
			m_bMagicTypeRightHand = 8;
			m_sMagicAmountRightHand = pRightHand->m_bMirrorDamage;	
		}		
	}
}

short CUser::GetDamage(short tid, int magicid)
{
	short damage = 0;
	int random = 0;
	short common_damage = 0, temp_hit = 0, temp_ac = 0, temp_hit_B = 0;
	BYTE result;

	_MAGIC_TABLE* pTable = NULL;
	_MAGIC_TYPE1* pType1 = NULL; 
	_MAGIC_TYPE2* pType2 = NULL;

	if( tid < 0 || tid >= MAX_USER) return -1;     // Check if target id is valid.

	CUser* pTUser = NULL;              
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];	   // Get target info.
	if( !pTUser || pTUser->m_bResHpType == USER_DEAD ) return -1;

	temp_ac = pTUser->m_sTotalAc + pTUser->m_sACAmount;    // Ç¥½Ã   
	temp_hit_B = (int)( (m_sTotalHit* m_bAttackAmount * 200 / 100) / (temp_ac + 240) ) ;   // Ç¥½Ã

	if (magicid > 0) {    // Skill/Arrow hit.    
		pTable = m_pMain->m_MagictableArray.GetData( magicid );     // Get main magic table.
		if( !pTable ) return -1; 
		
		if (pTable->bType1 == 1) {	// SKILL HIT!			                                
			pType1 = m_pMain->m_Magictype1Array.GetData( magicid );	    // Get magic skill table type 1.
			if( !pType1 ) return -1;     	                                

			if(pType1->bHitType) {    // Non-relative hit.
				random = myrand(0,100) ;       
				if (pType1->sHitRate <= random)
					result = FAIL;
				else 
					result = SUCCESS;
			}
			else {     // Relative hit.
				result = GetHitRate( (m_sTotalHitrate / pTUser->m_sTotalEvasionrate) * (pType1->sHitRate / 100.0f) );			
			}
			temp_hit = temp_hit_B * (pType1->sHit / 100.0f);
		}
		else if (pTable->bType1 == 2) {   // ARROW HIT!
			pType2 = m_pMain->m_Magictype2Array.GetData( magicid );	    // Get magic skill table type 1.
			if( !pType2 ) return -1; 
			
			if(pType2->bHitType == 1 || pType2->bHitType == 2 ) {    // Non-relative/Penetration hit.
				random = myrand(0,100) ; 
				
				if (pType2->sHitRate <= random)
					result = FAIL;
				else 
					result = SUCCESS;
			}
			else     // Relative hit/Arc hit.
				result = GetHitRate( (m_sTotalHitrate / pTUser->m_sTotalEvasionrate) * (pType2->sHitRate / 100.0f) );
			
			if(pType2->bHitType == 1 /* || pType2->bHitType == 2 */)  {
				temp_hit = m_sTotalHit * m_bAttackAmount * (pType2->sAddDamage / 100.0f) / 100 ;   // Ç¥½Ã
			}
			else {
				temp_hit = temp_hit_B * (pType2->sAddDamage / 100.0f);
			}
		}
	}
	else {    // Normal Hit.     
		temp_hit = m_sTotalHit * m_bAttackAmount / 100 ;	// Ç¥½Ã
		result = GetHitRate( m_sTotalHitrate / pTUser->m_sTotalEvasionrate ); 
	}
	
	switch(result) {						// 1. Magical item damage....
		case GREAT_SUCCESS:
		case SUCCESS:
		case NORMAL:
			if( magicid > 0 ) {	 // Skill Hit.
				damage = (short)temp_hit;
				random = myrand(0, damage);
				if (pTable->bType1 == 1) {
					damage = (short)((temp_hit + 0.3f * random) + 0.99f);
				}
				else {
					damage = (short)(((temp_hit * 0.6f) + 1.0f * random) + 0.99f);
				}
			}
			else {	// Normal Hit.	
				damage = (short)temp_hit_B;
				random = myrand(0, damage);
				damage = (short)((0.85f * temp_hit_B) + 0.3f * random);
			}		
			
			break;
		case FAIL:
			damage = 0;
			break;
	}	

	damage = GetMagicDamage(damage, tid);	// 2. Magical item damage....	
	damage = GetACDamage(damage, tid);		// 3. Additional AC calculation....	
//	damage = damage / 2;	// ¼º·¡¾¾ Ãß°¡ ¿äÃ»!!!!
	damage = damage / 3;	// ¼º·¡¾¾ Ãß°¡ ¿äÃ»!!!!  

	return damage;	  
}

short CUser::GetMagicDamage(int damage, short tid)
{
	short total_r = 0;
	short temp_damage = 0;

	CUser* pTUser = NULL;   
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];	   // Get target info.
	if( !pTUser || pTUser->m_bResHpType == USER_DEAD ) return damage;	

	// RIGHT HAND!!! by Yookozuna
	if (m_bMagicTypeRightHand > 4 && m_bMagicTypeRightHand < 8) {
		temp_damage = damage * m_sMagicAmountRightHand / 100;
	}

	switch (m_bMagicTypeRightHand) {	// RIGHT HAND!!!
		case ITEM_TYPE_FIRE :	// Fire Damage
			total_r = pTUser->m_bFireR + pTUser->m_bFireRAmount;
			break;
		case ITEM_TYPE_COLD :	// Ice Damage
			total_r = pTUser->m_bColdR + pTUser->m_bColdRAmount;
			break;
		case ITEM_TYPE_LIGHTNING :	// Lightning Damage
			total_r = pTUser->m_bLightningR + pTUser->m_bLightningRAmount;
			break;
		case ITEM_TYPE_POISON :	// Poison Damage
			total_r = pTUser->m_bPoisonR + pTUser->m_bPoisonRAmount;
			break;
		case ITEM_TYPE_HP_DRAIN :	// HP Drain		
			HpChange(temp_damage, 0);			
			break;
		case ITEM_TYPE_MP_DAMAGE :	// MP Damage		
			pTUser->MSpChange(-temp_damage);
			break;
		case ITEM_TYPE_MP_DRAIN :	// MP Drain		
			MSpChange(temp_damage);
			break;
		case 0:
			break;
	}

	if (m_bMagicTypeRightHand > 0 && m_bMagicTypeRightHand < 5) {
		if (total_r > 200) total_r = 200;
		temp_damage = m_sMagicAmountRightHand - m_sMagicAmountRightHand * total_r / 200;
		damage = damage + temp_damage;
	}

	total_r = 0 ;		// Reset all temporary data.
	temp_damage = 0 ;

	// LEFT HAND!!! by Yookozuna
	if (m_bMagicTypeLeftHand > 4 && m_bMagicTypeLeftHand < 8) {
		temp_damage = damage * m_sMagicAmountLeftHand / 100;
	}

	switch (m_bMagicTypeLeftHand) {	// LEFT HAND!!!
		case ITEM_TYPE_FIRE :	// Fire Damage
			total_r = pTUser->m_bFireR + pTUser->m_bFireRAmount;
			break;
		case ITEM_TYPE_COLD :	// Ice Damage
			total_r = pTUser->m_bColdR + pTUser->m_bColdRAmount;
			break;
		case ITEM_TYPE_LIGHTNING :	// Lightning Damage
			total_r = pTUser->m_bLightningR + pTUser->m_bLightningRAmount;
			break;
		case ITEM_TYPE_POISON :	// Poison Damage
			total_r = pTUser->m_bPoisonR + pTUser->m_bPoisonRAmount;
			break;
		case ITEM_TYPE_HP_DRAIN :	// HP Drain		
			HpChange(temp_damage, 0);			
			break;
		case ITEM_TYPE_MP_DAMAGE :	// MP Damage		
			pTUser->MSpChange(-temp_damage);
			break;
		case ITEM_TYPE_MP_DRAIN :	// MP Drain		
			MSpChange(temp_damage);
			break;
		case 0:
			break;
	}

	if (m_bMagicTypeLeftHand > 0 && m_bMagicTypeLeftHand < 5) {
		if (total_r > 200) total_r = 200;
		temp_damage = m_sMagicAmountLeftHand - m_sMagicAmountLeftHand * total_r / 200;
		damage = damage + temp_damage;
	}

	total_r = 0 ;		// Reset all temporary data.
	temp_damage = 0 ;

	// Mirror Attack Check routine.
	if (pTUser->m_bMagicTypeLeftHand == ITEM_TYPE_MIRROR_DAMAGE) {
		temp_damage = damage * pTUser->m_sMagicAmountLeftHand / 100;
		HpChange(-temp_damage);		// Reflective Hit.
	}

	return damage;
}

short CUser::GetACDamage(int damage, short tid)
{
	_ITEM_TABLE* pLeftHand = NULL;
	_ITEM_TABLE* pRightHand = NULL;

	CUser* pTUser = NULL;   
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];	   // Get target info.
	if( !pTUser || pTUser->m_bResHpType == USER_DEAD ) return damage;	

	if( m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 ) {
		pRightHand = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[RIGHTHAND].nNum );
		if( pRightHand ) {
			switch(pRightHand->m_bKind/10) {		// Weapon Type Right Hand....
				case WEAPON_DAGGER:		
					damage = damage - damage * pTUser->m_sDaggerR / 200 ;
					break;
				case WEAPON_SWORD:
					damage = damage - damage * pTUser->m_sSwordR / 200 ;				
					break;
				case WEAPON_AXE:
					damage = damage - damage * pTUser->m_sAxeR / 200 ;				
					break;
				case WEAPON_MACE:
					damage = damage - damage * pTUser->m_sMaceR / 200 ;				
					break;
				case WEAPON_SPEAR:
					damage = damage - damage * pTUser->m_sSpearR / 200 ;			
					break;
				case WEAPON_BOW:
					damage = damage - damage * pTUser->m_sBowR / 200 ;			
					break;
			}
		}
	}

	if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0) {
		pLeftHand = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEFTHAND].nNum );	
		if( pLeftHand ) {
			switch(pLeftHand->m_bKind/10) {			// Weapon Type Right Hand....
				case WEAPON_DAGGER:		
					damage = damage - damage * pTUser->m_sDaggerR / 200 ;
					break;
				case WEAPON_SWORD:
					damage = damage - damage * pTUser->m_sSwordR / 200 ;				
					break;
				case WEAPON_AXE:
					damage = damage - damage * pTUser->m_sAxeR / 200 ;				
					break;
				case WEAPON_MACE:
					damage = damage - damage * pTUser->m_sMaceR / 200 ;				
					break;
				case WEAPON_SPEAR:
					damage = damage - damage * pTUser->m_sSpearR / 200 ;			
					break;
				case WEAPON_BOW:
					damage = damage - damage * pTUser->m_sBowR / 200 ;			
					break;
			}
		}
	}

	return damage;
}

void CUser::ExpChange(int iExp)
{	
	char buff[256];
	memset( buff, 0x00, 256 );
	int send_index = 0;

	if( m_pUserData->m_bLevel < 6 && iExp < 0 )
		return;
//	
	if( m_pUserData->m_bZone == ZONE_BATTLE && iExp < 0 ) 
		return;
//

/*
	if( m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3 && iExp < 0 ) {
		iExp = iExp / 10;
	}
*/
	m_pUserData->m_iExp += iExp;

	if( m_pUserData->m_iExp < 0 ) {
		if( m_pUserData->m_bLevel > 5) {
			m_pUserData->m_bLevel--;
			m_pUserData->m_iExp = m_pMain->m_LevelUpArray[m_pUserData->m_bLevel-1]->m_iExp + m_pUserData->m_iExp;
			LevelChange( m_pUserData->m_bLevel, FALSE );
			return;
		}
	}
	else if( m_pUserData->m_iExp >= m_iMaxExp ) {
		if( m_pUserData->m_bLevel >= MAX_LEVEL ) {
			m_pUserData->m_iExp = m_iMaxExp;
			return;
		}

		m_pUserData->m_iExp = m_pUserData->m_iExp - m_iMaxExp;
		m_pUserData->m_bLevel++;
		LevelChange( m_pUserData->m_bLevel );
		return;
	}

	SetByte( buff, WIZ_EXP_CHANGE, send_index );
	SetByte( buff, 0x01, send_index );//Fix Exp Add
	SetDWORD( buff, m_pUserData->m_iExp, send_index );
	Send( buff, send_index);
//	
	if (iExp < 0) {
		m_iLostExp = -iExp;
	}
//
}

void CUser::LevelChange(short level, BYTE type )
{
	if( level < 1 || level > MAX_LEVEL )
		return;

	char buff[256];
	memset( buff, 0x00, 256 );
	int send_index = 0;

	if( type ) {
		if( (m_pUserData->m_bPoints+m_pUserData->m_bSta+m_pUserData->m_bStr+m_pUserData->m_bDex+m_pUserData->m_bIntel+m_pUserData->m_bCha) < (300+3*(level-1)) )
			m_pUserData->m_bPoints += 3;
		if( level > 9 && (m_pUserData->m_bstrSkill[0]+m_pUserData->m_bstrSkill[1]+m_pUserData->m_bstrSkill[2]+m_pUserData->m_bstrSkill[3]+m_pUserData->m_bstrSkill[4]
			+m_pUserData->m_bstrSkill[5]+m_pUserData->m_bstrSkill[6]+m_pUserData->m_bstrSkill[7]+m_pUserData->m_bstrSkill[8]) < (2*(level-9)) )
			m_pUserData->m_bstrSkill[0] += 2;	// Skill Points up
	}

	m_iMaxExp = m_pMain->m_LevelUpArray[level-1]->m_iExp;
	
	SetSlotItemValue();
	SetUserAbility();

	m_pUserData->m_sMp = m_iMaxMp;
	HpChange( m_iMaxHp );

	Send2AI_UserUpdateInfo();

	memset( buff, 0x00, 256 ); send_index = 0;
	SetByte( buff, WIZ_LEVEL_CHANGE, send_index );
	SetShort( buff, m_Sid, send_index );
	SetByte( buff, m_pUserData->m_bLevel, send_index );
	SetByte( buff, m_pUserData->m_bPoints, send_index );
	SetByte( buff, m_pUserData->m_bstrSkill[0], send_index );
	SetDWORD( buff, m_iMaxExp, send_index );
	SetDWORD( buff, m_pUserData->m_iExp, send_index );
	SetShort( buff, m_iMaxHp, send_index );
	SetShort( buff, m_pUserData->m_sHp, send_index );
	SetShort( buff, m_iMaxMp, send_index );
	SetShort( buff, m_pUserData->m_sMp, send_index );
	SetShort( buff, m_sMaxWeight, send_index );	
	SetShort( buff, m_sItemWeight, send_index );
	m_pMain->Send_Region( buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ );
	if( m_sPartyIndex != -1 ) {
		memset( buff, 0x00, 256 ); send_index = 0;
		SetByte( buff, WIZ_PARTY, send_index );
		SetByte( buff, PARTY_LEVELCHANGE, send_index );
		SetShort( buff, m_Sid, send_index );
		SetByte( buff, m_pUserData->m_bLevel, send_index );
		m_pMain->Send_PartyMember(m_sPartyIndex, buff, send_index);
	}
}

void CUser::PointChange(char *pBuf)
{
	int index = 0, send_index = 0, value = 0;
	BYTE type = 0x00;
	char send_buff[512];
	memset( send_buff, NULL, 512 );

	type = GetByte( pBuf, index );
	value = GetShort( pBuf, index );
	if( type > 5 || abs(value) > 1 ) return;
	if( m_pUserData->m_bPoints < 1 ) return;
	switch( type ) {
	case STR:
		if( m_pUserData->m_bStr == 0xFF ) return;
		break;
	case STA:
		if( m_pUserData->m_bSta == 0xFF ) return;
		break;
	case DEX:
		if( m_pUserData->m_bDex == 0xFF ) return;
		break;
	case INTEL:
		if( m_pUserData->m_bIntel == 0xFF ) return;
		break;
	case CHA:
		if( m_pUserData->m_bCha == 0xFF ) return;
		break;
	}

	m_pUserData->m_bPoints -= value;

	SetByte( send_buff, WIZ_POINT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	switch( type ) {
	case STR:
		m_pUserData->m_bStr++;
		SetShort( send_buff, m_pUserData->m_bStr, send_index );
		SetUserAbility();
		break;
	case STA:
		m_pUserData->m_bSta++;
		SetShort( send_buff, m_pUserData->m_bSta, send_index );
		SetMaxHp();
		SetMaxMp();
		break;
	case DEX:
		m_pUserData->m_bDex++;
		SetShort( send_buff, m_pUserData->m_bDex, send_index );
		SetUserAbility();
		break;
	case INTEL:
		m_pUserData->m_bIntel++;
		SetShort( send_buff, m_pUserData->m_bIntel, send_index );
		SetMaxMp();
		break;
	case CHA:
		m_pUserData->m_bCha++;
		SetShort( send_buff, m_pUserData->m_bCha, send_index );
		break;
	}
	SetShort( send_buff, 0x00, send_index ); //
	SetShort( send_buff, m_iMaxHp, send_index );
	SetShort( send_buff, m_iMaxMp, send_index );
	SetShort( send_buff, m_sTotalHit, send_index );
	SetShort( send_buff, m_sMaxWeight, send_index );
	SetShort( send_buff, 0x00, send_index ); //
	Send( send_buff, send_index );
}

void CUser::HpChange(int amount, int type, bool attack)		// type : Received From AIServer -> 1, The Others -> 0
{															// attack : Direct Attack(true) or Other Case(false)
	char buff[256];
	memset( buff, 0x00, 256 );
	int send_index = 0;

	m_pUserData->m_sHp += amount;
	if( m_pUserData->m_sHp < 0 )
		m_pUserData->m_sHp = 0;
	else if ( m_pUserData->m_sHp > m_iMaxHp )
		m_pUserData->m_sHp = m_iMaxHp;

	SetByte( buff, WIZ_HP_CHANGE, send_index );
	SetShort( buff, m_iMaxHp, send_index );
	SetShort( buff, m_pUserData->m_sHp, send_index );
	Send( buff, send_index);

	if(type == 0) {
		send_index = 0; memset( buff, 0x00, 256 );

		SetByte( buff, AG_USER_SET_HP, send_index );
		SetShort( buff, m_Sid, send_index );
		SetDWORD( buff, m_pUserData->m_sHp, send_index );
		m_pMain->Send_AIServer( m_pUserData->m_bZone, buff, send_index);
	}

	if( m_sPartyIndex != -1 ) {
		send_index = 0; memset( buff, 0x00, 256 );

		SetByte( buff, WIZ_PARTY, send_index );
		SetByte( buff, PARTY_HPCHANGE, send_index );
		SetShort( buff, m_Sid, send_index );
		SetShort( buff, m_iMaxHp, send_index );
		SetShort( buff, m_pUserData->m_sHp, send_index );
		SetShort( buff, m_iMaxMp, send_index );
		SetShort( buff, m_pUserData->m_sMp, send_index );
		m_pMain->Send_PartyMember(m_sPartyIndex, buff, send_index);
	}
	
	if( m_pUserData->m_sHp == 0 && attack == false) // Á÷Á¢ °¡°İÇØ¼­ Á×´Â°æ¿ì´Â Dead Packet ¾øÀ½
		Dead();
}

void CUser::MSpChange(int amount)
{
	char buff[256];
	memset( buff, 0x00, 256 );
	int send_index = 0;

	m_pUserData->m_sMp += amount;
	if( m_pUserData->m_sMp < 0 )
		m_pUserData->m_sMp = 0;
	else if ( m_pUserData->m_sMp > m_iMaxMp )
		m_pUserData->m_sMp = m_iMaxMp;
	
	SetByte( buff, WIZ_MSP_CHANGE, send_index );
	SetShort( buff, m_iMaxMp, send_index );
	SetShort( buff, m_pUserData->m_sMp, send_index );
	Send( buff, send_index);

	if( m_sPartyIndex != -1 ) {
		send_index = 0; memset( buff, 0x00, 256 );

		SetByte( buff, WIZ_PARTY, send_index );
		SetByte( buff, PARTY_HPCHANGE, send_index );
		SetShort( buff, m_Sid, send_index );
		SetShort( buff, m_iMaxHp, send_index );
		SetShort( buff, m_pUserData->m_sHp, send_index );
		SetShort( buff, m_iMaxMp, send_index );
		SetShort( buff, m_pUserData->m_sMp, send_index );
		m_pMain->Send_PartyMember(m_sPartyIndex, buff, send_index);
	}
}

void CUser::Send2AI_UserUpdateInfo()
{
	int  send_index = 0;
	char send_buf[1024];

	SetByte(send_buf, AG_USER_UPDATE, send_index );
	SetShort( send_buf, m_Sid, send_index );
	SetByte( send_buf, m_pUserData->m_bLevel, send_index );
	SetShort( send_buf, m_pUserData->m_sHp, send_index );
	SetShort( send_buf, m_pUserData->m_sMp, send_index );
	SetShort( send_buf, m_sTotalHit * m_bAttackAmount / 100 , send_index ); // Ç¥½Ã
	SetShort( send_buf, m_sTotalAc + m_sACAmount, send_index );  // Ç¥½Ã
	Setfloat( send_buf, m_sTotalHitrate, send_index );
	Setfloat( send_buf, m_sTotalEvasionrate, send_index );

//
	SetShort( send_buf, m_sItemAc, send_index );
	SetByte( send_buf, m_bMagicTypeLeftHand, send_index );
	SetByte( send_buf, m_bMagicTypeRightHand, send_index );
	SetShort( send_buf, m_sMagicAmountLeftHand, send_index );
	SetShort( send_buf, m_sMagicAmountRightHand, send_index );
//

	m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buf, send_index);
}

void CUser::SetUserAbility()
{
	_CLASS_COEFFICIENT* p_TableCoefficient = NULL;
	_ITEM_TABLE* pItem = NULL;
	BOOL bHaveBow = FALSE;
	p_TableCoefficient = m_pMain->m_CoefficientArray.GetData( m_pUserData->m_sClass );
	if( !p_TableCoefficient ) return;
	
	float hitcoefficient = 0.0f;
	if( m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 ) {
		pItem = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[RIGHTHAND].nNum );
		if( pItem ) {
			switch(pItem->m_bKind/10) {	// ¹«±â Å¸ÀÔ....
			case WEAPON_DAGGER:
				hitcoefficient = p_TableCoefficient->ShortSword;
				break;
			case WEAPON_SWORD:
				hitcoefficient = p_TableCoefficient->Sword;
				break;
			case WEAPON_AXE:
				hitcoefficient = p_TableCoefficient->Axe;
				break;
			case WEAPON_MACE:
				hitcoefficient = p_TableCoefficient->Club;
				break;
			case WEAPON_SPEAR:
				hitcoefficient = p_TableCoefficient->Spear;
				break;
			case WEAPON_SHIELD:
				break;
			case WEAPON_BOW:
			case WEAPON_LONGBOW:
			case WEAPON_LAUNCHER:
				hitcoefficient = p_TableCoefficient->Bow;
				bHaveBow = TRUE;
				break;
			case WEAPON_STAFF:
				hitcoefficient = p_TableCoefficient->Staff;
				break;
			case WEAPON_ARROW:
				break;
			case WEAPON_JAVELIN:
				break;
			case WEAPON_WORRIOR_AC:
				break;
			case WEAPON_LOG_AC:
				break;
			case WEAPON_WIZARD_AC:
				break;
			case WEAPON_PRIEST_AC:
				break;
			}
		}
	}
	if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 && hitcoefficient == 0.0f ) {
		pItem = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEFTHAND].nNum );	// ¿Ş¼Õ ¹«±â : È° Àû¿ëÀ» À§ÇØ
		if( pItem ) {
			switch(pItem->m_bKind/10) {	// ¹«±â Å¸ÀÔ....
			case WEAPON_BOW:
			case WEAPON_LONGBOW:
			case WEAPON_LAUNCHER:
				hitcoefficient = p_TableCoefficient->Bow;
				bHaveBow = TRUE;
				break;
			}
		}
	}

	int temp_str = 0, temp_dex = 0;

	temp_str = m_pUserData->m_bStr+m_bStrAmount+m_sItemStr;
//	if( temp_str > 255 ) temp_str = 255;

	temp_dex = m_pUserData->m_bDex+m_bDexAmount+m_sItemDex;
//	if( temp_dex > 255 ) temp_dex = 255;

	m_sBodyAc = m_pUserData->m_bLevel;
	m_sMaxWeight = (m_pUserData->m_bStr + m_sItemStr ) * 50;
/*
	±Ã¼ö °ø°İ·Â = 0.005 * È° °ø°İ·Â * (Dex + 40) + Á÷¾÷°è¼ö * È° °ø°İ·Â * È­»ì°ø°İ·Â * ·¹º§ * Dex
	Àü»ç °ø°İ·Â = 0.005 * ¹«±â °ø°İ·Â * (Str + 40) + Á÷¾÷°è¼ö * È° °ø°İ·Â * È­»ì°ø°İ·Â * ·¹º§ * Dex
*/
	if( bHaveBow ) 
//		m_sTotalHit = (short)((((0.005 * pItem->m_sDamage * temp_dex) + ( hitcoefficient * pItem->m_sDamage * m_pUserData->m_bLevel * temp_dex )) + 3) * (m_bAttackAmount/100)); 
//		m_sTotalHit = (short)((((0.005f * pItem->m_sDamage * (temp_dex + 40)) + ( hitcoefficient * pItem->m_sDamage * m_pUserData->m_bLevel * temp_dex )))); 
//		m_sTotalHit = (short)((((0.005 * pItem->m_sDamage * (temp_dex + 40)) + ( hitcoefficient * pItem->m_sDamage * m_pUserData->m_bLevel * temp_dex )) + 3) * (m_bAttackAmount/100)); 
		m_sTotalHit = (short)((((0.005 * pItem->m_sDamage * (temp_dex + 40)) + ( hitcoefficient * pItem->m_sDamage * m_pUserData->m_bLevel * temp_dex )) + 3));
	else
//		m_sTotalHit = (short)((((0.005 * m_sItemHit * temp_str) + ( hitcoefficient * m_sItemHit * m_pUserData->m_bLevel * temp_str )) + 3) * (m_bAttackAmount/100)); 
//		m_sTotalHit = (short)((((0.005f * m_sItemHit * (temp_str + 40)) + ( hitcoefficient * m_sItemHit * m_pUserData->m_bLevel * temp_dex ))));
//		m_sTotalHit = (short)((((0.005 * m_sItemHit * (temp_str + 40)) + ( hitcoefficient * m_sItemHit * m_pUserData->m_bLevel * temp_str )) + 3) * (m_bAttackAmount/100)); 	
		m_sTotalHit = (short)((((0.005f * m_sItemHit * (temp_str + 40)) + ( hitcoefficient * m_sItemHit * m_pUserData->m_bLevel * temp_str )) + 3)); 	


//	m_sTotalAc = (short)(((p_TableCoefficient->AC * (m_sBodyAc+m_sItemAc) * temp_str )) + m_sACAmount );
//	m_sTotalAc = (short)(((p_TableCoefficient->AC * (m_sBodyAc + m_sItemAc + m_sACAmount) * temp_str )));

	// ÅäÅ» AC = Å×ÀÌºí ÄÚ¿¡ÇÇ¼ÇÆ® * (·¹º§ + ¾ÆÀÌÅÛ AC + Å×ÀÌºí 4ÀÇ AC)
//	m_sTotalAc = (short)(p_TableCoefficient->AC * (m_sBodyAc + m_sItemAc + m_sACAmount));
	m_sTotalAc = (short)(p_TableCoefficient->AC * (m_sBodyAc + m_sItemAc));
	m_sTotalHitrate = ((1 + p_TableCoefficient->Hitrate * m_pUserData->m_bLevel *  temp_dex ) * m_sItemHitrate/100 ) * (m_bHitRateAmount/100);

	m_sTotalEvasionrate = ((1 + p_TableCoefficient->Evasionrate * m_pUserData->m_bLevel * temp_dex ) * m_sItemEvasionrate/100) * (m_sAvoidRateAmount/100);

	SetMaxHp();
	SetMaxMp();
}

void CUser::ItemMove(char *pBuf)
{
	int index = 0, itemid = 0, srcpos = -1, destpos = -1;
	int send_index = 0;
	char send_buff[128];
	memset( send_buff, NULL, 128 );
	_ITEM_TABLE* pTable = NULL;
	BYTE dir;

	dir = GetByte( pBuf, index );
	itemid = GetDWORD( pBuf, index );
	srcpos = GetByte( pBuf, index );
	destpos = GetByte( pBuf, index );

	if( m_sExchangeUser != -1 ) goto fail_return;
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable )
		goto fail_return;
//	if( dir == ITEM_INVEN_SLOT && ((pTable->m_sWeight + m_sItemWeight) > m_sMaxWeight) )
//		goto fail_return;
	if( dir > 0x04 || srcpos >= SLOT_MAX+HAVE_MAX+EXTRA_MAX || destpos >= SLOT_MAX+HAVE_MAX+EXTRA_MAX )
		goto fail_return;
	if( (dir == ITEM_INVEN_SLOT || dir == ITEM_SLOT_SLOT ) && destpos > SLOT_MAX )
		goto fail_return;
	if( dir == ITEM_SLOT_INVEN && srcpos > SLOT_MAX )
		goto fail_return;
	if( dir == ITEM_INVEN_SLOT && destpos == RESERVED )
		goto fail_return;
	if( dir == ITEM_SLOT_INVEN && srcpos == RESERVED )
		goto fail_return;
	if( dir == ITEM_INVEN_SLOT || dir == ITEM_SLOT_SLOT ) {
		if( pTable->m_bRace != 0 ) {
			if( pTable->m_bRace != m_pUserData->m_bRace )
				goto fail_return;
		}
		if( !ItemEquipAvailable( pTable ) )
			goto fail_return;
	}

	switch( dir ) {
	case ITEM_INVEN_SLOT:
		if( itemid != m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum )
			goto fail_return;
		if( !IsValidSlotPos( pTable, destpos ) )
			goto fail_return;
		else if( pTable->m_bSlot == 0x01 || (pTable->m_bSlot == 0x00 && destpos == RIGHTHAND) ) {	// ¿À¸¥¼ÕÀü¿ë ¹«±â(¶Ç´Â ¾ç¼Õ¾µ¼ö ÀÖ°í ÀåÂøÇÏ·Á´Â À§Ä¡°¡ ¿À¸¥¼Õ) ÀÎµ¥ ´Ù¸¥¼Õ¿¡ µÎ¼Õ¾²´Â °æ¿ì Ã¼Å©
			if(m_pUserData->m_sItemArray[LEFTHAND].nNum != 0) {
				_ITEM_TABLE* pTable2 = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEFTHAND].nNum );
				if( pTable2 ) {
					if( pTable2->m_bSlot == 0x04 ) {
						m_pUserData->m_sItemArray[RIGHTHAND].nNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum;	// ¿À¸¥¼Õ¿¡ ³Ö±¸..
						m_pUserData->m_sItemArray[RIGHTHAND].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						m_pUserData->m_sItemArray[RIGHTHAND].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
						m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
						if( m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum == 0 )
							m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[LEFTHAND].nNum; // ¿Ş¼Õ¹«±â¸¦ ÀÎº¥À¸·Î ³Ö¾îÁØ´Ù.
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[LEFTHAND].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[LEFTHAND].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[LEFTHAND].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[LEFTHAND].nNum = 0;			// ¿Ş¼ÕÀº ºñ¾îÀÖ°Ô µÇÁö...
						m_pUserData->m_sItemArray[LEFTHAND].sDuration = 0;
						m_pUserData->m_sItemArray[LEFTHAND].sCount = 0;
						m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = 0;
					}
					else {
						short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[destpos].nNum = itemid;
						m_pUserData->m_sItemArray[destpos].sDuration = duration;
						m_pUserData->m_sItemArray[destpos].sCount = 1;
						m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
						if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 ) 
							m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
					}
				}
			}
			else {
				short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[destpos].nNum = itemid;
				m_pUserData->m_sItemArray[destpos].sDuration = duration;
				m_pUserData->m_sItemArray[destpos].sCount = 1;
				m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
				if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
		else if( pTable->m_bSlot == 0x02 || (pTable->m_bSlot == 0x00 && destpos == LEFTHAND) ) {	// ¿Ş¼ÕÀü¿ë ¹«±â(¶Ç´Â ¾ç¼Õ¾µ¼ö ÀÖ°í ÀåÂøÇÏ·Á´Â À§Ä¡°¡ ¿Ş¼Õ) ÀÎµ¥ ´Ù¸¥¼Õ¿¡ µÎ¼Õ¾²´Â °æ¿ì Ã¼Å©
			if(m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0) {
				_ITEM_TABLE* pTable2 = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[RIGHTHAND].nNum);
				if( pTable2 ) {
					if( pTable2->m_bSlot == 0x03 ) {
						m_pUserData->m_sItemArray[LEFTHAND].nNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum;
						m_pUserData->m_sItemArray[LEFTHAND].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						m_pUserData->m_sItemArray[LEFTHAND].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
						m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
						if( m_pUserData->m_sItemArray[LEFTHAND].nSerialNum == 0 )
							m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[RIGHTHAND].nNum; // ¿À¸¥¼Õ¹«±â¸¦ ÀÎº¥À¸·Î ³Ö¾îÁØ´Ù.
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[RIGHTHAND].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[RIGHTHAND].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 ) 
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[RIGHTHAND].nNum = 0;
						m_pUserData->m_sItemArray[RIGHTHAND].sDuration = 0;
						m_pUserData->m_sItemArray[RIGHTHAND].sCount = 0;
						m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = 0;
					}
					else {
						short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[destpos].nNum = itemid;
						m_pUserData->m_sItemArray[destpos].sDuration = duration;
						m_pUserData->m_sItemArray[destpos].sCount = 1;
						m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
						if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
					}
				}
			}
			else {
				short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[destpos].nNum = itemid;
				m_pUserData->m_sItemArray[destpos].sDuration = duration;
				m_pUserData->m_sItemArray[destpos].sCount = 1;
				m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
				if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
		else if( pTable->m_bSlot == 0x03 ) {	// µÎ¼Õ »ç¿ëÇÏ°í ¿À¸¥¼Õ ¹«±â
			if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 && m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 )
				goto fail_return;
			else if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 ) {
				m_pUserData->m_sItemArray[RIGHTHAND].nNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum;
				m_pUserData->m_sItemArray[RIGHTHAND].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				m_pUserData->m_sItemArray[RIGHTHAND].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
				m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
				if( m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum == 0 )
					m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[LEFTHAND].nNum;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[LEFTHAND].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[LEFTHAND].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[LEFTHAND].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[LEFTHAND].nNum = 0;
				m_pUserData->m_sItemArray[LEFTHAND].sDuration = 0;
				m_pUserData->m_sItemArray[LEFTHAND].sCount = 0;
				m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = 0;
			}
			else {
				short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 ) 
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[destpos].nNum = itemid;
				m_pUserData->m_sItemArray[destpos].sDuration = duration;
				m_pUserData->m_sItemArray[destpos].sCount = 1;
				m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
				if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
		else if ( pTable->m_bSlot == 0x04 ) {	// µÎ¼Õ »ç¿ëÇÏ°í ¿Ş¼Õ ¹«±â
			if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 && m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 )
				goto fail_return;
			else if( m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 ) {
				m_pUserData->m_sItemArray[LEFTHAND].nNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum;
				m_pUserData->m_sItemArray[LEFTHAND].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				m_pUserData->m_sItemArray[LEFTHAND].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
				m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
				if( m_pUserData->m_sItemArray[LEFTHAND].nSerialNum == 0 )
					m_pUserData->m_sItemArray[LEFTHAND].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[RIGHTHAND].nNum;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[RIGHTHAND].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[RIGHTHAND].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[RIGHTHAND].nNum = 0;
				m_pUserData->m_sItemArray[RIGHTHAND].sDuration = 0;
				m_pUserData->m_sItemArray[RIGHTHAND].sCount = 0;
				m_pUserData->m_sItemArray[RIGHTHAND].nSerialNum = 0;
			}
			else {
				short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[destpos].nNum = itemid;
				m_pUserData->m_sItemArray[destpos].sDuration = duration;
				m_pUserData->m_sItemArray[destpos].sCount = 1;
				m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
				if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
// COSPRE ITEM
	// CRIGHT & CLEFT
		else if( pTable->m_bSlot == 100 ) {
			if(m_pUserData->m_sItemArray[CLEFT].nNum != 0) {
				_ITEM_TABLE* pTable2 = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[CLEFT].nNum );
				if( pTable2 ) {
//					if( pTable2->m_bSlot == 0x04 ) {
						m_pUserData->m_sItemArray[CRIGHT].nNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum;
						m_pUserData->m_sItemArray[CRIGHT].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						m_pUserData->m_sItemArray[CRIGHT].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
						m_pUserData->m_sItemArray[CRIGHT].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
						if( m_pUserData->m_sItemArray[CRIGHT].nSerialNum == 0 )
							m_pUserData->m_sItemArray[CRIGHT].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[CLEFT].nNum;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[CLEFT].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[CLEFT].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[CLEFT].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[CLEFT].nNum = 0;
						m_pUserData->m_sItemArray[CLEFT].sDuration = 0;
						m_pUserData->m_sItemArray[CLEFT].sCount = 0;
						m_pUserData->m_sItemArray[CLEFT].nSerialNum = 0;
					}
					else {
						short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
						__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
						m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
						if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
							m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
						m_pUserData->m_sItemArray[destpos].nNum = itemid;
						m_pUserData->m_sItemArray[destpos].sDuration = duration;
						m_pUserData->m_sItemArray[destpos].sCount = 1;
						m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
						if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 ) 
							m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
					}
//				}
			}
			else {
				short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
				__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
				if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
				m_pUserData->m_sItemArray[destpos].nNum = itemid;
				m_pUserData->m_sItemArray[destpos].sDuration = duration;
				m_pUserData->m_sItemArray[destpos].sCount = 1;
				m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
				if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
					m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
//
		else {
			short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
			__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;

			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;	// Swaping
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;
			if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != 0 && m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 )
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
			m_pUserData->m_sItemArray[destpos].nNum = itemid;
			m_pUserData->m_sItemArray[destpos].sDuration = duration;
			m_pUserData->m_sItemArray[destpos].sCount = 1;
			m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
			if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
				m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
		}
		break;
	case ITEM_SLOT_INVEN:
		printf("ITEM_SLOT_INVEN");

		if( itemid != m_pUserData->m_sItemArray[srcpos].nNum )
			goto fail_return;
		if( m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum != 0 )
			goto fail_return; 

		m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum = m_pUserData->m_sItemArray[srcpos].nNum;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration = m_pUserData->m_sItemArray[srcpos].sDuration;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount = m_pUserData->m_sItemArray[srcpos].sCount;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = m_pUserData->m_sItemArray[srcpos].nSerialNum;
		if( m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum == 0 ) 
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = m_pMain->GenerateItemSerial();
		m_pUserData->m_sItemArray[srcpos].nNum = 0;
		m_pUserData->m_sItemArray[srcpos].sDuration = 0;
		m_pUserData->m_sItemArray[srcpos].sCount = 0;
		m_pUserData->m_sItemArray[srcpos].nSerialNum = 0;
		break;
	case ITEM_INVEN_INVEN:
		printf("ITEM_INVEN_INVEN");

		if( itemid != m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum )
			goto fail_return; 
		{
			short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
			short itemcount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
			__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
			_ITEM_TABLE* pTable2 = NULL;

			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum;
			if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum == 0 ) {
				pTable2 = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum);
				if( pTable && pTable->m_bCountable == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pMain->GenerateItemSerial();
			}

			m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum = itemid;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration = duration;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount = itemcount;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = serial;
			if( m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum == 0 ) {
				pTable2 = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum);
				if( pTable && pTable->m_bCountable == 0 )
					m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = m_pMain->GenerateItemSerial();
			}
		}
		break;
	case ITEM_SLOT_SLOT:
		printf("ITEM_SLOT_SLOT");

		if( itemid != m_pUserData->m_sItemArray[srcpos].nNum )
			goto fail_return;
		if( !IsValidSlotPos( pTable, destpos ) )
			goto fail_return;

		if( m_pUserData->m_sItemArray[destpos].nNum != 0 ) {
			_ITEM_TABLE* pTable2 = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[destpos].nNum );	// dest slot exist some item
			if( pTable2 ) {
				if( pTable2->m_bSlot != 0x00 )
					goto fail_return;
				else {
					short duration = m_pUserData->m_sItemArray[srcpos].sDuration;
					short count = m_pUserData->m_sItemArray[srcpos].sCount;
					__int64 serial = m_pUserData->m_sItemArray[srcpos].nSerialNum;
					m_pUserData->m_sItemArray[srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;				// Swaping
					m_pUserData->m_sItemArray[srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;		// Swaping
					m_pUserData->m_sItemArray[srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;			// Swaping
					m_pUserData->m_sItemArray[srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;	// Swaping
					if( m_pUserData->m_sItemArray[srcpos].nSerialNum == 0 )
						m_pUserData->m_sItemArray[srcpos].nSerialNum = m_pMain->GenerateItemSerial();
					m_pUserData->m_sItemArray[destpos].nNum = itemid;
					m_pUserData->m_sItemArray[destpos].sDuration = duration;
					m_pUserData->m_sItemArray[destpos].sCount = count;
					m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
					if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
						m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
				}
			}
		}
		else {
			short duration = m_pUserData->m_sItemArray[srcpos].sDuration;
			short count = m_pUserData->m_sItemArray[srcpos].sCount;
			__int64 serial = m_pUserData->m_sItemArray[srcpos].nSerialNum;
			m_pUserData->m_sItemArray[srcpos].nNum = m_pUserData->m_sItemArray[destpos].nNum;				// Swaping
			m_pUserData->m_sItemArray[srcpos].sDuration = m_pUserData->m_sItemArray[destpos].sDuration;		// Swaping
			m_pUserData->m_sItemArray[srcpos].sCount = m_pUserData->m_sItemArray[destpos].sCount;			// Swaping
			m_pUserData->m_sItemArray[srcpos].nSerialNum = m_pUserData->m_sItemArray[destpos].nSerialNum;	// Swaping
			m_pUserData->m_sItemArray[destpos].nNum = itemid;
			m_pUserData->m_sItemArray[destpos].sDuration = duration;
			m_pUserData->m_sItemArray[destpos].sCount = count;
			m_pUserData->m_sItemArray[destpos].nSerialNum = serial;
			if( m_pUserData->m_sItemArray[destpos].nSerialNum == 0 )
				m_pUserData->m_sItemArray[destpos].nSerialNum = m_pMain->GenerateItemSerial();
		}
		break;
	}

	if( dir != ITEM_INVEN_INVEN ) {	// ÀåÂøÀÌ ¹Ù²î´Â °æ¿ì¿¡¸¸ °è»ê..
		SetSlotItemValue();
		SetUserAbility();
	}
/*
	SetByte( send_buff, WIZ_ITEM_MOVE, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, m_sTotalHit, send_index );
	SetShort( send_buff, m_sTotalAc, send_index );
	SetShort( send_buff, m_sMaxWeight, send_index );
	SetShort( send_buff, m_iMaxHp, send_index );
	SetShort( send_buff, m_iMaxMp, send_index );
	SetByte( send_buff, m_sItemStr, send_index );
	SetByte( send_buff, m_sItemSta, send_index );
	SetByte( send_buff, m_sItemDex, send_index );
	SetByte( send_buff, m_sItemIntel, send_index );
	SetByte( send_buff, m_sItemCham, send_index );
	SetByte( send_buff, m_bFireR, send_index );
	SetByte( send_buff, m_bColdR, send_index );
	SetByte( send_buff, m_bLightningR, send_index );
	SetByte( send_buff, m_bMagicR, send_index );
	SetByte( send_buff, m_bDiseaseR, send_index );
	SetByte( send_buff, m_bPoisonR, send_index );
	Send( send_buff, send_index );

	BYTE	m_bStrAmount;
	BYTE	m_bStaAmount;
	BYTE	m_bDexAmount;
	BYTE	m_bIntelAmount;
	BYTE	m_bChaAmount;
*/

//	ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<
	SetByte( send_buff, WIZ_ITEM_MOVE, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetShort( send_buff, m_sTotalHit, send_index );
	SetShort( send_buff, m_sTotalAc, send_index );
	SetShort( send_buff, m_sMaxWeight, send_index );
	SetShort( send_buff, 0x00, send_index );
	SetShort( send_buff, m_iMaxHp, send_index );
	SetShort( send_buff, m_iMaxMp, send_index );

	SetShort( send_buff, m_sItemStr + m_bStrAmount, send_index );		// ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<
	SetShort( send_buff, m_sItemSta + m_bStaAmount, send_index );		// ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<
	SetShort( send_buff, m_sItemDex + m_bDexAmount, send_index );		// ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<
	SetShort( send_buff, m_sItemIntel + m_bIntelAmount, send_index );	// ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<
	SetShort( send_buff, m_sItemCham + m_bChaAmount, send_index );		// ºñ·¯¸Ó±Û »çÀÚÀÇ Èû >.<

	SetShort( send_buff, m_bFireR, send_index );
	SetShort( send_buff, m_bColdR, send_index );
	SetShort( send_buff, m_bLightningR, send_index );
	SetShort( send_buff, m_bMagicR, send_index );
	SetShort( send_buff, m_bDiseaseR, send_index );
	SetShort( send_buff, m_bPoisonR, send_index );
	Send( send_buff, send_index );
//
	SendItemWeight();

// COSPRE ITEM 
	if( (dir == ITEM_INVEN_SLOT ) && ( destpos == HEAD || destpos == BREAST || destpos == SHOULDER || destpos == LEFTHAND || destpos == RIGHTHAND || destpos == LEG || destpos == GLOVE || destpos == FOOT || destpos == CWING || destpos == CLEFT || destpos == CRIGHT || destpos == CTOP || destpos == CHELMET ) ) 
		UserLookChange( destpos, itemid, m_pUserData->m_sItemArray[destpos].sDuration );	// ÀåÂø
	if( (dir == ITEM_SLOT_INVEN ) && ( srcpos == HEAD || srcpos == BREAST || srcpos == SHOULDER || srcpos == LEFTHAND || srcpos == RIGHTHAND || srcpos == LEG || srcpos == GLOVE || srcpos == FOOT || destpos == CWING || destpos == CLEFT || destpos == CRIGHT || destpos == CTOP || destpos == CHELMET ) ) 
		UserLookChange( srcpos, 0, 0 );		// ÇØÁ¦

	// AI Server¿¡ ¹Ù…Î µ¥ÀÌÅ¸ Àü¼Û....
	Send2AI_UserUpdateInfo();

	return;

fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_ITEM_MOVE, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

BOOL CUser::IsValidSlotPos(_ITEM_TABLE* pTable, int destpos)
{
	if( !pTable )
		return FALSE;

	switch( pTable->m_bSlot ) {
	case 0:
		if( destpos != RIGHTHAND && destpos != LEFTHAND )
			return FALSE;
		break;
	case 1:
	case 3:
		if( destpos != RIGHTHAND )
			return FALSE;
		break;
	case 2:
	case 4:
		if( destpos != LEFTHAND )
			return FALSE;
		break;
	case 5:
		if( destpos != BREAST )
			return FALSE;
		break;
	case 6:
		if( destpos != LEG )
			return FALSE;
		break;
	case 7:
		if( destpos != HEAD )
			return FALSE;
		break;
	case 8:
		if( destpos != GLOVE )
			return FALSE;
		break;
	case 9:
		if( destpos != FOOT )
			return FALSE;
		break;
	case 10:
		if( destpos != RIGHTEAR && destpos != LEFTEAR )
			return FALSE;
		break;
	case 11:
		if( destpos != NECK )
			return FALSE;
		break;
	case 12:
		if( destpos != RIGHTRING && destpos != LEFTRING )
			return FALSE;
		break;
	case 13:
		if( destpos != SHOULDER )
			return FALSE;
		break;
// COSPRE ITEM
/*	case 43:
		if( destpos != CWING )
			return FALSE;
		break;
	case 44:
		if( destpos != CHELMET )
			return FALSE;
		break;
	case 45:
		if( destpos != CLEFT )
			return FALSE;
		break;
	case 46:
		if( destpos != CRIGHT )
			return FALSE;
		break;
	case 47:
		if( destpos != CTOP )
			return FALSE;
		break;
	case 48:
		if( destpos != BAG1 )
			return FALSE;
		break;
	case 49:
		if( destpos != BAG2 )
			return FALSE;
		break; */
//
	}
	return TRUE;
}

void CUser::NpcEvent(char *pBuf)
{
	if( m_pMain->m_bPointCheckFlag == FALSE)	return;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ

	int index = 0, send_index = 0, nid = 0, i=0, temp_index = 0;
	char send_buf[2048];
	memset( send_buf, NULL, 2048);
	CNpc* pNpc = NULL;

	nid = GetShort( pBuf, index );
	pNpc = m_pMain->m_arNpcArray.GetData(nid);
	if( !pNpc ) return;

	switch( pNpc->m_tNpcType ) {
		// pelerin npcsi
		// SEND-->WIZ_NPC_EVENT-->20F334
		// RECV-->WIZ_KNIGHTS_PROCESS-->3C1B00000000
		// kral seçim npcsi
		// SEND-->WIZ_NPC_EVENT-->20C72B
		// RECV-->WIZ_UNKNOW (78)-->7805027632
		// kral özel npcsi
		// SEND-->WIZ_NPC_EVENT-->205829
		// RECV-->WIZ_UNKNOW (78)-->7803010100319FCD3B84E55262
		case NPC_CHAOTIC:
			SetByte( send_buf, WIZ_NEW_EVENT, send_index );
			SetByte( send_buf, 0x04, send_index ); // Chaotic Generator
			SetShort( send_buf, nid, send_index );
			Send( send_buf, send_index );
			break;
		case NPC_RENTAL:
			SetByte( send_buf, WIZ_RENTAL, send_index );
			SetByte( send_buf, 0x03, send_index ); // NPC
			SetByte( send_buf, -1, send_index );
			SetByte( send_buf, -1, send_index );
			SetByte( send_buf, 0x58, send_index );
			SetByte( send_buf, 0x86, send_index );
			SetShort( send_buf, 0x03, send_index );
			Send( send_buf, send_index );
			break;
		case NPC_MERCHANT:
			SetByte( send_buf, WIZ_TRADE_NPC, send_index );
			SetDWORD( send_buf, pNpc->m_iSellingGroup, send_index );
			Send( send_buf, send_index );
			break;
		case NPC_TINKER:
			SetByte( send_buf, WIZ_REPAIR_NPC, send_index );
			SetDWORD( send_buf, pNpc->m_iSellingGroup, send_index );
			Send( send_buf, send_index );
			break;
		case NPC_CAPTAIN:
			SetByte( send_buf, WIZ_CLASS_CHANGE, send_index );
			SetByte( send_buf, CLASS_CHANGE_REQ, send_index );
			Send( send_buf, send_index );
			break;
		case NPC_OFFICER:
			SetShort( send_buf, 0, send_index );	// default 0 page
			m_pMain->m_KnightsManager.AllKnightsList( this, send_buf );
			break;
		case NPC_WAREHOUSE:
			SetByte( send_buf, WIZ_WAREHOUSE, send_index );	
			SetByte( send_buf, WAREHOUSE_REQ, send_index );
			Send( send_buf, send_index );
	/*
			SetByte( send_buf, WIZ_WAREHOUSE, send_index );
			SetByte( send_buf, WAREHOUSE_OPEN, send_index );
			SetDWORD( send_buf, m_pUserData->m_iBank, send_index );
			for( i=0; i<WAREHOUSE_MAX; i++ ) {
				SetDWORD( send_buf, m_pUserData->m_sWarehouseArray[i].nNum, send_index );
				SetShort( send_buf, m_pUserData->m_sWarehouseArray[i].sDuration, send_index );
				SetShort( send_buf, m_pUserData->m_sWarehouseArray[i].sCount, send_index );
			}
			SendCompressingPacket( send_buf, send_index );	*/
			break;

		case NPC_WARP:
			break;

		case NPC_CLERIC:	// ºñ·¯¸Ó±Û Äù½ºÆ® °ü·Ã NPCµé >.<....
		case NPC_COUPON:
		case NPC_MONK_KARUS:
		case NPC_MONK_ELMORAD:
			SetShort( send_buf, nid, send_index );
			ClientEvent( send_buf );
			break;
		default:
				SetByte( send_buf, WIZ_QUEST, send_index );
				SetByte( send_buf, 0x07, send_index ); // msg
				switch( (int)pNpc->m_sSid ) {
					case 16085: // [Peddler]Hemes
							SetShort( send_buf, 4131, send_index );
						break;
					case 16047: // [Operator] Moira
							SetShort( send_buf, 4031, send_index );
						break;
					case 14301: // [Blacksmith] Heppa
							SetShort( send_buf, 242, send_index );
						break;
					case 13013: // [Sentinel]Patrick
							SetShort( send_buf, 167, send_index );
						break;
					case 16079: // [Grand Merchant's kaishan] Menissiah
							SetShort( send_buf, 193, send_index );
						break;
					default:
							return;
						break;
				}
				SetShort( send_buf, 0x00, send_index );
				SetShort( send_buf, pNpc->m_sSid, send_index );
				Send( send_buf, send_index );
			break;
	}   
}

void CUser::ItemTrade(char *pBuf)
{
	int index = 0, send_index = 0, itemid = 0, money = 0, count = 0, group = 0, npcid = 0;
	_ITEM_TABLE* pTable = NULL;
	char send_buf[128];
	CNpc* pNpc = NULL;
	memset( send_buf, NULL, 128);
	BYTE type, pos, destpos, result;

	DWORD OldGold; // add
	OldGold = m_pUserData->m_iGold; // add

	// »ó°Å·¡ ¾ÈµÇ°Ô...
	if( m_bResHpType == USER_DEAD || m_pUserData->m_sHp == 0 )	{
		printf("### ItemTrade Fail : name=%s(%d), m_bResHpType=%d, hp=%d, x=%d, z=%d ###\n", m_pUserData->m_id, m_Sid, m_bResHpType, m_pUserData->m_sHp, (int)m_pUserData->m_curx, (int)m_pUserData->m_curz);
		result = 0x01;
		goto fail_return;
	}

	type = GetByte( pBuf, index );
	if( type == 0x01 ) {// item buy
		group = GetDWORD( pBuf, index );
		npcid = GetShort( pBuf, index );
	}
	itemid = GetDWORD( pBuf, index );
	pos = GetByte( pBuf, index );
	if( type == 0x03 ) 	// item move only
		destpos = GetByte( pBuf, index );
	else
		count = GetShort( pBuf, index );
//	ºñ·¯¸Ó±Û Å©¸®½º¸¶½º ÀÌ¹êÆ® >.<
	if (itemid >= ITEM_NO_TRADE) goto fail_return;
//
	if( type == 0x03 ) {	// item inven to inven move
		if( pos >= HAVE_MAX || destpos >= HAVE_MAX ) {
			SetByte( send_buf, WIZ_ITEM_TRADE, send_index );
			SetByte( send_buf, 0x04, send_index );
			Send( send_buf, send_index );
			return;
		}
		if( itemid != m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum ) {
			SetByte( send_buf, WIZ_ITEM_TRADE, send_index );
			SetByte( send_buf, 0x04, send_index );
			Send( send_buf, send_index );
			return;
		}
		short duration = m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration;
		short itemcount = m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum = itemid;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration = duration;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount = itemcount;
		SetByte( send_buf, WIZ_ITEM_TRADE, send_index );
		SetByte( send_buf, 0x03, send_index );
		Send( send_buf, send_index );
		return;
	}

	if( m_sExchangeUser != -1 ) goto fail_return;
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) {
		result = 0x01;
		goto fail_return;
	}
	if( pos >= HAVE_MAX ) {
		result = 0x02;
		goto fail_return;
	}

	if( count <= 0 || count > MAX_ITEM_COUNT) {
		result = 0x02;
		goto fail_return;
	}

	if( type == 0x01 ) {	// buy sequence
		if( m_pMain->m_bPointCheckFlag == FALSE)	{
			result = 0x01;
			goto fail_return;
		}	

		pNpc = m_pMain->m_arNpcArray.GetData(npcid);
		if( !pNpc ) {
			result = 0x01;
			goto fail_return;
		}
		if( pNpc->m_iSellingGroup != group ) {
			result = 0x01;
			goto fail_return;
		}

		if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != 0 ) {
			if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum == itemid ) {
				if( !pTable->m_bCountable || count <= 0 ) {
					result = 0x02;
					goto fail_return;
				}

				if( pTable->m_bCountable && (count+m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount) > MAX_ITEM_COUNT ) {
					result = 0x04;
					goto fail_return;				
				}
			}
			else {
				result = 0x02;
				goto fail_return;
			}
		}
		if( m_pUserData->m_iGold < (pTable->m_iBuyPrice*count) ) {
			result = 0x03;
			goto fail_return;
		}
//
		if (pTable->m_bCountable) {	// Check weight of countable item.
			if (((pTable->m_sWeight * count) + m_sItemWeight) > m_sMaxWeight) {			
				result = 0x04;
				goto fail_return;
			}
		}
		else {	// Check weight of non-countable item.
			if ((pTable->m_sWeight + m_sItemWeight) > m_sMaxWeight) {
				result = 0x04;
				goto fail_return;
			}
		}		
//
		m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = itemid;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = pTable->m_sDuration;
		if( pTable->m_bCountable && count > 0 ) {	// count °³³äÀÌ ÀÖ´Â ¾ÆÀÌÅÛ
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount += count;
			m_pUserData->m_iGold -= (pTable->m_iBuyPrice * count);
		}
		else {
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 1;
			m_pUserData->m_iGold -= pTable->m_iBuyPrice;
			m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum = m_pMain->GenerateItemSerial();
		}

		SendItemWeight();
		ItemLogToAgent( m_pUserData->m_id, pNpc->m_strName, ITEM_MERCHANT_BUY, m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum, itemid, count, pTable->m_sDuration );
	}
	else {		// sell sequence
		if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != itemid ) {
			result = 0x02;
			goto fail_return;
		}
		if( m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount < count ) {
			result = 0x03;
			goto fail_return;
		}
		int durability = m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration;

		if( pTable->m_bCountable && count > 0 ) {
			m_pUserData->m_iGold += (pTable->m_iSellPrice * count);
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount -= count;
			if( m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount <= 0 ) {
				m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = 0;
				m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = 0;
				m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 0;
			}
		}
		else {
			m_pUserData->m_iGold += pTable->m_iSellPrice;
			m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = 0;
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = 0;
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 0;
		}

		SendItemWeight();
		ItemLogToAgent( m_pUserData->m_id, "MERCHANT SELL", ITEM_MERCHANT_SELL, 0, itemid, count, durability );
	}
	
	DWORD qGold; // add
	qGold = m_pUserData->m_iGold - OldGold; // add

	SetByte( send_buf, WIZ_ITEM_TRADE, send_index );
	SetShort( send_buf, 0x01, send_index ); // byte
	SetDWORD( send_buf, m_pUserData->m_iGold, send_index );
	SetDWORD( send_buf, qGold, send_index ); // add
	Send( send_buf, send_index );
	return;

fail_return:
	send_index = 0;
	SetByte( send_buf, WIZ_ITEM_TRADE, send_index );
	SetByte( send_buf, 0x00, send_index );
	SetByte( send_buf, result, send_index );
	Send( send_buf, send_index );
}

void CUser::SendTargetHP( BYTE echo, int tid, int damage )
{
	int send_index = 0, hp = 0, maxhp = 0;
	char buff[256];
	memset( buff, 0x00, 256 );
	CUser* pTUser = NULL;
	CNpc* pNpc = NULL;

	if( tid < 0 )
		return;
	if( tid >= NPC_BAND ) {
		if( m_pMain->m_bPointCheckFlag == FALSE)	return;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ
		pNpc = m_pMain->m_arNpcArray.GetData(tid);
		if( !pNpc )
			return;
		hp = pNpc->m_iHP;	maxhp = pNpc->m_iMaxHP;
	}
	else {
		if( tid >= MAX_USER )
			return;
		pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];
		if( !pTUser || pTUser->m_bResHpType == USER_DEAD ) 
			return;
		hp = pTUser->m_pUserData->m_sHp;	maxhp = pTUser->m_iMaxHp;
	}

	SetByte( buff, WIZ_TARGET_HP, send_index );
	SetShort( buff, tid, send_index );
	SetByte( buff, echo, send_index );
	SetDWORD( buff, maxhp, send_index );
	SetDWORD( buff, hp, send_index );
	SetShort( buff, damage, send_index );
	Send( buff, send_index );
}

void CUser::BundleOpenReq(char *pBuf)
{
	int index = 0, send_index = 0, bundle_index = 0;
	char send_buff[256];
	memset(send_buff, NULL, 256 );
	_ZONE_ITEM* pItem = NULL;
	C3DMap* pMap = NULL;
	CRegion* pRegion = NULL;

	bundle_index = GetDWORD( pBuf, index );
	if( bundle_index < 1 )	return;
	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	return;
	if( m_RegionX < 0 || m_RegionZ < 0 || m_RegionX > pMap->GetXRegionMax() || m_RegionZ > pMap->GetZRegionMax() ) return;
	pRegion = &(pMap->m_ppRegion[m_RegionX][m_RegionZ]);
	if( !pRegion ) return;

	pItem = (_ZONE_ITEM*)pRegion->m_RegionItemArray.GetData( bundle_index );
	if( !pItem ) return;

	SetByte( send_buff, WIZ_BUNDLE_OPEN_REQ, send_index );
	for(int i=0; i<6; i++ ) {
		SetDWORD( send_buff, pItem->itemid[i], send_index );
		SetShort( send_buff, pItem->count[i], send_index );
	}
	Send( send_buff, send_index );
}
/*
BOOL CUser::IsValidName(char *name)
{
	// sungyong tw
	char* szInvalids[] = {	"~", "`", "!", "@", "#", "$", "%", "^", "&", "*",
							"(", ")", "-", "+", "=", "|", "\\", "<", ">", ",",
							".", "?", "/", "{", "[", "}", "]", "\"", "\'", " ",
							"¡¡", "¿î¿µÀÚ", "³ªÀÌÆ®", "µµ¿ì¹Ì", "Knight", "Noahsystem", "Wizgate", "Mgame", "³ë¾Æ½Ã½ºÅÛ", "À§Áî°ÔÀÌÆ®", "¿¥°ÔÀÓ"};
	// taiwan version
/*	char* szInvalids[] = {	"~", "`", "!", "@", "#", "$", "%", "^", "&", "*",
							"(", ")", "-", "+", "=", "|", "\\", "<", ">", ",",
							".", "?", "/", "{", "[", "}", "]", "\"", "\'", " ",	"¡¡" };	


	BOOL bInvalidStr = FALSE;
	
	for(int i = 0; i < 41; i++)		// korea version
	//for(int i = 0; i < 31; i++)	// taiwan version
	{
		if(strstr(name, szInvalids[i]))
		{
			bInvalidStr = TRUE;
			break;
		}
	}
	// ~sungyong tw

	return !bInvalidStr;
}
*/
BOOL CUser::IsValidName(char* name)
{
	CString upperName = name;
	upperName.MakeUpper();

	for (BlockNameArray::iterator itr = m_pMain->m_BlockNameArray.begin(); itr != m_pMain->m_BlockNameArray.end(); itr++)
	{
		if (strstr(upperName, *itr))
			return FALSE;
	}

	return TRUE;
}

void CUser::ItemGet(char *pBuf)
{
	int index = 0, send_index = 0, bundle_index = 0, itemid = 0, count = 0, i=0;
	BYTE pos;
	_ITEM_TABLE* pTable = NULL;
	char send_buff[256];	memset( send_buff, NULL, 256 );
	_ZONE_ITEM* pItem = NULL;
	C3DMap* pMap = NULL;
	CRegion* pRegion = NULL;
	CUser* pUser = NULL;
	CUser* pGetUser = NULL;
	_PARTY_GROUP* pParty = NULL;

	bundle_index = GetDWORD( pBuf, index );
	if( bundle_index < 1 )	goto fail_return;
	if( m_sExchangeUser != -1 ) goto fail_return;
	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	goto fail_return;
	if( m_RegionX < 0 || m_RegionZ < 0 || m_RegionX > pMap->GetXRegionMax() || m_RegionZ > pMap->GetZRegionMax() ) goto fail_return;
	pRegion = &(pMap->m_ppRegion[m_RegionX][m_RegionZ]);
	if( !pRegion )	goto fail_return;
	pItem = (_ZONE_ITEM*)pRegion->m_RegionItemArray.GetData( bundle_index );
	if( !pItem ) goto fail_return;

	itemid = GetDWORD( pBuf, index );

	for(i=0; i<6; i++) {
		if( pItem->itemid[i] == itemid )
			break;
	}
	if( i == 6 ) goto fail_return;
	count = pItem->count[i];

	if( pMap->RegionItemRemove( m_RegionX, m_RegionZ, bundle_index, pItem->itemid[i], pItem->count[i] ) == FALSE )
		goto fail_return;
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) goto fail_return;

	if( m_sPartyIndex != -1 && itemid != ITEM_GOLD ) 
		pGetUser = GetItemRoutingUser(itemid, count);
	else
		pGetUser = this;
		
	if( !pGetUser ) goto fail_return;
	pos = pGetUser->GetEmptySlot( itemid, pTable->m_bCountable );

	if( pos != 0xFF ) {	// Common Item
		if( pos >= HAVE_MAX ) goto fail_return;

		if( pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != 0 ) {	
			if( pTable->m_bCountable != 1 ) goto fail_return;	
			else if( pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != itemid ) goto fail_return;
		}
//
		if (pTable->m_bCountable) {	// Check weight of countable item.
			if ((pTable->m_sWeight * count + pGetUser->m_sItemWeight) > pGetUser->m_sMaxWeight) {			
				send_index = 0; memset( send_buff, NULL, 256 );
				SetByte( send_buff, WIZ_ITEM_GET, send_index );
				SetByte( send_buff, 0x06, send_index );
				pGetUser->Send( send_buff, send_index );
				return;
			}
		}
		else {	// Check weight of non-countable item.
			if ((pTable->m_sWeight + pGetUser->m_sItemWeight) > pGetUser->m_sMaxWeight) {
				send_index = 0; memset( send_buff, NULL, 256 );
				SetByte( send_buff, WIZ_ITEM_GET, send_index );
				SetByte( send_buff, 0x06, send_index );
				pGetUser->Send( send_buff, send_index );			
				return;
			}
		}
//
		pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = itemid;	// Add item to inventory. 

		if( pTable->m_bCountable ) {	// Apply number of item.		
			pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount += count;
			if( pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount > MAX_ITEM_COUNT ) {
				pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = MAX_ITEM_COUNT;
			}
		}
		else {
			pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 1;
			pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum = m_pMain->GenerateItemSerial();
		}

		pGetUser->SendItemWeight();
		pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = pTable->m_sDuration;
		pGetUser->ItemLogToAgent( pGetUser->m_pUserData->m_id, "MONSTER", ITEM_MONSTER_GET, pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum, itemid, count, pTable->m_sDuration );
	}
	else {	// Gold
		if( itemid != ITEM_GOLD ) goto fail_return;
		if( count > 0 && count < 32767 ) {
			if( m_sPartyIndex == -1 ) {
				m_pUserData->m_iGold += count;
				SetByte( send_buff, WIZ_ITEM_GET, send_index );
				SetByte( send_buff, 0x01, send_index );
				SetByte( send_buff, pos, send_index );
				SetDWORD( send_buff, itemid, send_index );
				SetShort( send_buff, count, send_index );
				SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
				Send( send_buff, send_index );
			}
			else {
				pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
				if( !pParty ) goto fail_return;
				int usercount = 0, money = 0, levelsum = 0;
				for( int i=0; i<8; i++ ) {
					if( pParty->uid[i] != -1 ) {
						usercount++;
						levelsum += pParty->bLevel[i];
					}
				}
				if( usercount == 0 ) goto fail_return;
				for( i=0; i<8; i++ ) {
					if( pParty->uid[i] != -1 || pParty->uid[i] >= MAX_USER ) {
						pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[i]];
						if( !pUser ) continue;

						money = count * (float)( pUser->m_pUserData->m_bLevel/(float)levelsum );    
						pUser->m_pUserData->m_iGold += money;

						send_index = 0; memset( send_buff, 0x00, 256 );
						SetByte( send_buff, WIZ_ITEM_GET, send_index );
						SetByte( send_buff, 0x02, send_index );
						SetByte( send_buff, 0xff, send_index );			// gold -> pos : 0xff
						SetDWORD( send_buff, itemid, send_index );
						SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
						pUser->Send( send_buff, send_index );
					}
				}
			}
		}
		return;
	}
	
	SetByte( send_buff, WIZ_ITEM_GET, send_index );
	if( pGetUser == this )
		SetByte( send_buff, 0x01, send_index );
	else
		SetByte( send_buff, 0x05, send_index );
	SetByte( send_buff, pos, send_index );
	SetDWORD( send_buff, itemid, send_index );
	SetShort( send_buff, pGetUser->m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount, send_index );
	SetDWORD( send_buff, pGetUser->m_pUserData->m_iGold, send_index );
	pGetUser->Send( send_buff, send_index );

	if( m_sPartyIndex != -1 ) {
		memset( send_buff, NULL, 256 ); send_index = 0;
		SetByte( send_buff, WIZ_ITEM_GET, send_index );
		SetByte( send_buff, 0x03, send_index );
		SetDWORD( send_buff, itemid, send_index );
		SetShort( send_buff, strlen(pGetUser->m_pUserData->m_id), send_index );
		SetString( send_buff, pGetUser->m_pUserData->m_id, strlen(pGetUser->m_pUserData->m_id), send_index );
		m_pMain->Send_PartyMember( m_sPartyIndex, send_buff, send_index );
		if( pGetUser != this ) {
			memset( send_buff, NULL, 256 ); send_index = 0;
			SetByte( send_buff, WIZ_ITEM_GET, send_index );
			SetByte( send_buff, 0x04, send_index );
			Send( send_buff, send_index );
		}
	} 

	return;

fail_return:
	send_index = 0;
	SetByte( send_buff, WIZ_ITEM_GET, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::StateChange(char *pBuf)
{
	int index = 0, send_index = 0;

	BYTE type = 0x00;
	BYTE buff = 0x00;

	char send_buff[128];
	memset( send_buff, NULL, send_index );

	type = GetByte( pBuf, index );
	buff = GetByte( pBuf, index );

//	if( type > 5 ) return;
	if( type == 5 && m_pUserData->m_bAuthority != 0) return;	//  Operators only!!!
	
	if (type == 1) {
		m_bResHpType = buff;
	}
	else if (type == 2) {
		m_bNeedParty = buff;
	}
	else if (type == 3) {
		m_bAbnormalType = buff;
	}

	SetByte( send_buff, WIZ_STATE_CHANGE, send_index );
	SetShort( send_buff, m_Sid, send_index );

	SetByte( send_buff, type, send_index );
	if (type == 1) {
		SetByte( send_buff, m_bResHpType, send_index );
	}
	else if (type == 2) {
		SetByte( send_buff, m_bNeedParty, send_index );
	}
//
	else if (type == 3) {
		SetByte( send_buff, m_bAbnormalType, send_index );
	}	
//
	else {		// Just plain echo :)
		SetByte( send_buff, buff, send_index );
//		N3_SP_STATE_CHANGE_ACTION = 0x04,			// 1 - ÀÎ»ç, 11 - µµ¹ß
//		N3_SP_STATE_CHANGE_VISIBLE = 0x05 };		// Åõ¸í 0 ~ 255
	}

	SetByte( send_buff, 0x00, send_index );
	SetShort( send_buff, 0x00, send_index );
	SetDWORD( send_buff, 0x00, send_index );

	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ );
}

void CUser::LoyaltyChange(short tid)
{
	int send_index = 0; char send_buff[256]; memset( send_buff, NULL, 256 );	
	short level_difference = 0; short loyalty_source = 0; short loyalty_target = 0;

	CUser* pTUser = NULL ;									  // Pointer initialization!		
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];     // Get target info.  
	if( !pTUser ) return;									  // Check if target exists and not already dead.
	
	if (pTUser->m_pUserData->m_bNation != m_pUserData->m_bNation) {		// Different nations!!!
		level_difference = pTUser->m_pUserData->m_bLevel - m_pUserData->m_bLevel ;	// Calculate difference!

		if (pTUser->m_pUserData->m_iLoyalty <= 0) {		// No cheats allowed...
			loyalty_source = 0;
			loyalty_target = 0;
		}
		else if (level_difference > 5) {	// Target at least six levels lower...
			loyalty_source = 50;
			loyalty_target = -25;
		}
		else if (level_difference < -5) {	// Target at least six levels higher...
			loyalty_source =  10; 
			loyalty_target = -5;
		}
		else {								// Target within the 5 and -5 range...
			loyalty_source =  30;
			loyalty_target = -15;
		}
	}
	else {		// Same Nations!!!
		if (pTUser->m_pUserData->m_iLoyalty >= 0) {		
			loyalty_source  = -1000;
			loyalty_target = -15;			
		}
		else {
			loyalty_source  =  100;
			loyalty_target = -15;
		}
	}
//
	if (m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3) { 
		loyalty_source  = 2 * loyalty_source;
	}
//

	//printf("LoyaltyChange 222 - user1=%s, %d,, user2=%s, %d\n", m_pUserData->m_id,  m_pUserData->m_iLoyalty, pTUser->m_pUserData->m_id, pTUser->m_pUserData->m_iLoyalty);
	
	m_pUserData->m_iLoyalty += loyalty_source;			// Recalculations of Loyalty...
	pTUser->m_pUserData->m_iLoyalty += loyalty_target;  

	if (m_pUserData->m_iLoyalty < 0) m_pUserData->m_iLoyalty = 0;	// Cannot be less than zero.
	if (pTUser->m_pUserData->m_iLoyalty < 0) pTUser->m_pUserData->m_iLoyalty = 0;

	//printf("LoyaltyChange 222 - user1=%s, %d,, user2=%s, %d\n", m_pUserData->m_id,  m_pUserData->m_iLoyalty, pTUser->m_pUserData->m_id, pTUser->m_pUserData->m_iLoyalty);

	SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );	// Send result to source.
	SetDWORD( send_buff, m_pUserData->m_iLoyalty, send_index );
	Send( send_buff, send_index );

	memset( send_buff, NULL, 256 ); send_index = 0;		// Send result to target.
	SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );
	SetDWORD( send_buff, pTUser->m_pUserData->m_iLoyalty, send_index );
	pTUser->Send( send_buff, send_index );

//	This is for the Event Battle on Wednesday :(
	if (m_pMain->m_byBattleOpen) {
		if (m_pUserData->m_bZone == ZONE_BATTLE ) { // || m_pUserData->m_bZone == ZONE_SNOW_BATTLE) {
			if (pTUser->m_pUserData->m_bNation == KARUS) {
				m_pMain->m_sKarusDead++;
				//printf("++ LoyaltyChange - ka=%d, el=%d\n", m_pMain->m_sKarusDead, m_pMain->m_sElmoradDead);
			}
			else if (pTUser->m_pUserData->m_bNation == ELMORAD) {
				m_pMain->m_sElmoradDead++;
				//printf("++ LoyaltyChange - ka=%d, el=%d\n", m_pMain->m_sKarusDead, m_pMain->m_sElmoradDead);
			}
		}
	}
//
}

void CUser::SpeedHackUser()
{
	if( strlen(m_pUserData->m_id) == 0 )
		return;

	char logstr[256];
	memset( logstr, NULL, 256 );
	sprintf( logstr, "%s Speed Hack Used\r\n", m_pUserData->m_id);
	LogFileSpeedHack( logstr );
	
	if( m_pUserData->m_bAuthority != 0 )
		m_pUserData->m_bAuthority = -1;

	Close();
}

void CUser::UserLookChange(int pos, int itemid, int durability)
{
	int send_index = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256 );

	if( pos >= SLOT_MAX )
		return;

	SetByte( send_buff, WIZ_USERLOOK_CHANGE, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetByte( send_buff, (BYTE)pos, send_index );
	SetDWORD( send_buff, itemid, send_index );
	SetShort( send_buff, durability, send_index );
	m_pMain->Send_Region( send_buff, send_index, (int)m_pUserData->m_bZone, m_RegionX, m_RegionZ, this );
}

void CUser::SendNotice()
{
	int send_index = 0, buff_index = 0, new_index = 0, count = 0, ecount = 0;
	char send_buff[2048], new_buff[2024], buff[2024];
	memset( buff, NULL, 2024 );
	memset( new_buff, NULL, 2048 );
	memset( send_buff, NULL, 2048 );

	// OLD
	SetByte( send_buff, WIZ_NOTICE, send_index );
	for( int i=0; i<20; i++ ) {
		if( !strlen(m_pMain->m_ppNotice[i]) )
			continue;
		SetByte( buff, strlen(m_pMain->m_ppNotice[i]), buff_index );
		SetString( buff, m_pMain->m_ppNotice[i], strlen( m_pMain->m_ppNotice[i]), buff_index );
		count++;
	}
	SetByte( send_buff, 0x01, send_index ); // Type
	SetByte( send_buff, count, send_index );
	SetString( send_buff, buff, buff_index, send_index );
	Send( send_buff, send_index );

	// CLAN
	if( m_pUserData->m_bKnights != 0 ) {
		send_index = 0, buff_index = 0;
		SetByte( send_buff, WIZ_NOTICE, send_index );
		SetShort( buff, strlen("Clan Notice"), buff_index );
		SetString( buff, "Clan Notice", strlen("Clan Notice"), buff_index );
		SetShort( buff, strlen("NERO"), buff_index );
		SetString( buff, "NERO", strlen("NERO"), buff_index );
		SetByte( send_buff, 0x04, send_index ); // Type
		SetByte( send_buff, 0x01, send_index ); // Count
		SetString( send_buff, buff, buff_index, send_index );
		Send( send_buff, send_index );
	}

	// NEW
	send_index = 0, buff_index = 0;
	SetByte( send_buff, WIZ_NOTICE, send_index );
	for( int e=0; e<20; e++ ) {
		if( !strlen(m_pMain->m_epNotice[e]) )
			continue;
		SetShort( buff, strlen(m_pMain->m_epNotice[e]), buff_index );
		SetString( buff, m_pMain->m_epNotice[e], strlen( m_pMain->m_epNotice[e]), buff_index );
		SetShort( buff, strlen(m_pMain->m_upNotice[e]), buff_index );
		SetString( buff, m_pMain->m_upNotice[e], strlen( m_pMain->m_upNotice[e]), buff_index );
		ecount++;
	}
	SetByte( send_buff, 0x02, send_index ); // Type
	SetByte( send_buff, ecount, send_index );
	SetString( send_buff, buff, buff_index, send_index );
	Send( send_buff, send_index );
}

void CUser::PartyProcess(char *pBuf)
{
	int index = 0, idlength = 0, memberid = -1;
	char strid[MAX_ID_SIZE+1]; memset( strid, 0x00, MAX_ID_SIZE+1 );
	BYTE subcommand, result;
	CUser* pUser = NULL;

	subcommand = GetByte( pBuf, index );
	switch( subcommand ) {
	case PARTY_CREATE:
		idlength = GetShort( pBuf, index );
		if (idlength <= 0 || idlength > MAX_ID_SIZE) return ;
		GetString( strid, pBuf, idlength, index );
		pUser = m_pMain->GetUserPtr( strid, 0x02 );
		if( pUser ) {
			memberid = pUser->GetSocketID();
			PartyRequest( memberid, TRUE );
		}
		break;
	case PARTY_PERMIT:
		result = GetByte( pBuf, index );
		if( result ) 
			PartyInsert();
		else									// °ÅÀıÇÑ °æ¿ì
			PartyCancel();
		break;
	case PARTY_INSERT:
		idlength = GetShort( pBuf, index );
		if (idlength <= 0 || idlength > MAX_ID_SIZE) return ;
		GetString( strid, pBuf, idlength, index );
		pUser = m_pMain->GetUserPtr( strid, 0x02 );
		if( pUser ) {
			memberid = pUser->GetSocketID();
			PartyRequest( memberid, FALSE );
		}
		break;
	case PARTY_REMOVE:
		memberid = GetShort( pBuf, index );
		PartyRemove( memberid );
		break;
	case PARTY_DELETE:
		PartyDelete();
		break;
	}
}

void CUser::PartyCancel()	// °ÅÀıÇÑ »ç¶÷ÇÑÅ× ¿Â´Ù... ¸®´õ¸¦ Ã£¾Æ¼­ ¾Ë·ÁÁÖ´Â ÇÔ¼ö
{
	int send_index = 0, leader_id = -1, count = 0;
	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;
	char send_buff[256]; memset( send_buff, 0x00, 256 );

	if( m_sPartyIndex == -1 ) return;
	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) {				// ÀÌ»óÇÑ °æ¿ì
		m_sPartyIndex = -1;
		return;
	}

	m_sPartyIndex = -1;
	
	leader_id = pParty->uid[0];
	if( leader_id < 0 || leader_id >= MAX_USER ) return;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[leader_id];
	if( !pUser ) return;

	for( int i=0; i<8; i++ ) {		// ÆÄÆ¼ »ı¼º½Ã Ãë¼ÒÇÑ°Å¸é..ÆÄÆ¼¸¦ »Ç°³¾ßÁã...
		if( pParty->uid[i] >= 0 )
			count++;
	}

	if( count == 1 )
		pUser->PartyDelete();				// ¸®´õ È¥ÀÚÀÌ¸é ÆÄÆ¼ ±úÁü

	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_INSERT, send_index );
	SetShort( send_buff, -1, send_index );
	pUser->Send( send_buff, send_index );
}

void CUser::PartyRequest(int memberid, BOOL bCreate)	//¸®´õ¿¡°Ô ÆĞÅ¶ÀÌ ¿Â°Å´Ù..
{
	int index = 0, send_index = 0, result = -1, i=0;
	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;
	char send_buff[256]; memset( send_buff, 0x00, 256 );

	if( memberid < 0 || memberid >= MAX_USER ) goto fail_return;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[memberid];
	if( !pUser ) goto fail_return;
	if( pUser->m_sPartyIndex != -1 ) goto fail_return;//ÀÌ¹Ì ÆÄÆ¼ °¡ÀÔµÇ¾î ÀÖÀ¸¸é ¾ÈµÇÂ¡...

	if( m_pUserData->m_bNation != pUser->m_pUserData->m_bNation ) {
		result = -3;
		goto fail_return;
	}

	if( !(   ( pUser->m_pUserData->m_bLevel <= (int)(m_pUserData->m_bLevel * 1.5) && pUser->m_pUserData->m_bLevel >= (int)(m_pUserData->m_bLevel * 1.5)) 
		  || ( pUser->m_pUserData->m_bLevel <= (m_pUserData->m_bLevel+8) && pUser->m_pUserData->m_bLevel >= ((int)(m_pUserData->m_bLevel)-8) ) 
		 )
	  )  {
		result = -2;
		goto fail_return;
	}

	if( !bCreate ) {	// ±âÁ¸ÀÇ ÆÄÆ¼¿¡ Ãß°¡ÇÏ´Â °æ¿ì
		pParty = m_pMain->m_PartyArray.GetData(m_sPartyIndex);
		if( !pParty ) goto fail_return;
		for(i=0; i<8; i++) {
			if( pParty->uid[i] < 0 ) 
				break;
		}
		if( i==8 ) goto fail_return;	// ÆÄÆ¼ ÀÎ¿ø Full
	}

	if( bCreate ) {
		if( m_sPartyIndex != -1 ) goto fail_return;	// (»ı¼ºÀÚ)¸®´õ°¡ ÀÌ¹Ì ÆÄÆ¼ °¡ÀÔµÇ¾î ÀÖÀ¸¸é ¾ÈµÇÂ¡...
		m_sPartyIndex = m_pMain->m_sPartyIndex++;
		if( m_pMain->m_sPartyIndex == 32767 )	
			m_pMain->m_sPartyIndex = 0;

		EnterCriticalSection( &g_region_critical );
		
		pParty = new _PARTY_GROUP;
		pParty->wIndex = m_sPartyIndex;
		pParty->uid[0] = m_Sid;
		pParty->sMaxHp[0] = m_iMaxHp;
		pParty->sHp[0] = m_pUserData->m_sHp;
		pParty->bLevel[0] = m_pUserData->m_bLevel;
		pParty->sClass[0] = m_pUserData->m_sClass;
		if( !m_pMain->m_PartyArray.PutData( pParty->wIndex, pParty ) ) {
			delete pParty;
			m_sPartyIndex = -1;
			LeaveCriticalSection( &g_region_critical );
			goto fail_return;
		}
		LeaveCriticalSection( &g_region_critical );
		// AI Server
		send_index = 0; memset( send_buff, 0x00, 256 );
		SetByte( send_buff, AG_USER_PARTY, send_index );
		SetByte( send_buff, PARTY_CREATE, send_index );
		SetShort( send_buff, pParty->wIndex, send_index );
		SetShort( send_buff, pParty->uid[0], send_index );
		//SetShort( send_buff, pParty->sHp[0], send_index );
		//SetByte( send_buff, pParty->bLevel[0], send_index );
		//SetShort( send_buff, pParty->sClass[0], send_index );
		m_pMain->Send_AIServer(m_pUserData->m_bZone, send_buff, send_index);
	}

	pUser->m_sPartyIndex = m_sPartyIndex;

/*	ÆÄÆ¼ BBS¸¦ À§ÇØ Ãß°¡...
	if (pUser->m_bNeedParty == 2 && pUser->m_sPartyIndex != -1) {
		pUser->m_bNeedParty = 1;	// ³­ ´õ ÀÌ»ó ÆÄÆ¼°¡ ÇÊ¿äÇÏÁö ¾Ê¾Æ ^^;
		memset( send_buff, 0x00, 256 ); send_index = 0;	
		SetByte(send_buff, 2, send_index);
		SetByte(send_buff, pUser->m_bNeedParty, send_index);
		pUser->StateChange(send_buff);
	}

	if (m_bNeedParty == 2 && m_sPartyIndex != -1) {
		m_bNeedParty = 1;	// ³­ ´õ ÀÌ»ó ÆÄÆ¼°¡ ÇÊ¿äÇÏÁö ¾Ê¾Æ ^^;
		memset( send_buff, 0x00, 256 ); send_index = 0;	
		SetByte(send_buff, 2, send_index);
		SetByte(send_buff, m_bNeedParty, send_index);
		StateChange(send_buff);
	}	
*/
	send_index = 0; memset( send_buff, 0x00, 256 );
	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_PERMIT, send_index );
	SetShort( send_buff, m_Sid, send_index );
// ¿ø°Å¸®°¡ ¾ÊµÈµ¥ÀÚ³ª ¾¾~
	SetShort(send_buff, strlen(m_pUserData->m_id), send_index);	// Create packet.
	SetString(send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index);
//
	pUser->Send( send_buff, send_index );
	return;

fail_return:
	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_INSERT, send_index );
	SetShort( send_buff, result, send_index );
	Send( send_buff, send_index );
}

void CUser::PartyInsert()	// º»ÀÎÀÌ Ãß°¡ µÈ´Ù.  ¸®´õ¿¡°Ô ÆĞÅ¶ÀÌ °¡´Â°ÍÀÌ ¾Æ´Ô
{
	int send_index = 0;
	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;
	char send_buff[256]; memset( send_buff, 0x00, 256 );
	if( m_sPartyIndex == -1 ) return;

	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) {				// ÀÌ»óÇÑ °æ¿ì
		m_sPartyIndex = -1;
		return;
	}
	
	for(int i=0; i<8; i++) {	// Send your info to the rest of the party members.
		if( pParty->uid[i] == m_Sid ) continue;
		if( pParty->uid[i] < 0 || pParty->uid[i] >= MAX_USER ) continue;
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[i]];
		if( !pUser ) continue;

		memset( send_buff, 0x00, 256 ); send_index = 0;
		SetByte( send_buff, WIZ_PARTY, send_index );
		SetByte( send_buff, PARTY_INSERT, send_index );
		SetShort( send_buff, pParty->uid[i], send_index );
		SetShort( send_buff, strlen(pUser->m_pUserData->m_id), send_index );
		SetString( send_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), send_index );
		SetShort( send_buff, pParty->sMaxHp[i], send_index );
		SetShort( send_buff, pParty->sHp[i], send_index );
		SetByte( send_buff, pParty->bLevel[i], send_index );
		SetShort( send_buff, pParty->sClass[i], send_index );
		SetShort( send_buff, pUser->m_iMaxMp, send_index );
		SetShort( send_buff, pUser->m_pUserData->m_sMp, send_index );
		Send( send_buff, send_index );	// Ãß°¡µÈ »ç¶÷¿¡°Ô ±âÁ¸ ÀÎ¿ø ´Ù ¹Ş°ÔÇÔ..
	}
/*
	for(int i=0; i<8; i++ ) {
		if( pParty->uid[i] == -1 ) {		// Party Structure ¿¡ Ãß°¡
			pParty->uid[i] = m_Sid;
			pParty->sMaxHp[i] = m_iMaxHp;
			pParty->sHp[i] = m_pUserData->m_sHp;
			pParty->bLevel[i] = m_pUserData->m_bLevel;
			pParty->sClass[i] = m_pUserData->m_sClass;
			break;
		}
	}*/
	int insertIndex = -1;

	for (int i = 0; i < 8; i++) {
		if (pParty->uid[i] == -1) {
			pParty->uid[i] = m_Sid;
			pParty->sMaxHp[i] = m_iMaxHp;
			pParty->sHp[i] = m_pUserData->m_sHp;
			pParty->bLevel[i] = m_pUserData->m_bLevel;
			pParty->sClass[i] = m_pUserData->m_sClass;

			insertIndex = i; // 🔥 BURASI ÇOK ÖNEMLİ
			break;
		}
	}
// ÆÄÆ¼ BBS¸¦ À§ÇØ Ãß°¡...	´ëÀåÆÇ!!!
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[0]];
	if( !pUser ) return;

	if (pUser->m_bNeedParty == 2 && pUser->m_sPartyIndex != -1) {	// ÀÌ°Ç ÆÄÆ¼Àå °Í...
		pUser->m_bNeedParty = 1;	// ³­ ´õ ÀÌ»ó ÆÄÆ¼°¡ ÇÊ¿äÇÏÁö ¾Ê¾Æ ^^;
		memset( send_buff, 0x00, 256 ); send_index = 0;	
		SetByte(send_buff, 2, send_index);
		SetByte(send_buff, pUser->m_bNeedParty, send_index);
		pUser->StateChange(send_buff);
	}
// Ãß°¡ ³¡................

// ÆÄÆ¼ BBS¸¦ À§ÇØ Ãß°¡...	ÂÌµû±¸ÆÇ!!!
	if (m_bNeedParty == 2 && m_sPartyIndex != -1) {		// ÀÌ°Ç ½ÇÁ¦·Î Ãß°¡µÈ »ç¶÷ °Í...
		m_bNeedParty = 1;	// ³­ ´õ ÀÌ»ó ÆÄÆ¼°¡ ÇÊ¿äÇÏÁö ¾Ê¾Æ ^^;
		memset( send_buff, 0x00, 256 ); send_index = 0;	
		SetByte(send_buff, 2, send_index);
		SetByte(send_buff, m_bNeedParty, send_index);
		StateChange(send_buff);
	}
// Ãß°¡ ³¡................

	memset( send_buff, 0x00, 256 ); send_index = 0;
	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_INSERT, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
	SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
	SetShort( send_buff, m_iMaxHp, send_index );		
	SetShort( send_buff, m_pUserData->m_sHp, send_index );
	SetByte( send_buff, m_pUserData->m_bLevel, send_index );
	SetShort( send_buff, m_pUserData->m_sClass, send_index );
	SetShort( send_buff, m_iMaxMp, send_index );
	SetShort( send_buff, m_pUserData->m_sMp, send_index );
	m_pMain->Send_PartyMember( m_sPartyIndex, send_buff, send_index );	// Ãß°¡µÈ ÀÎ¿øÀ» ºê·ÎµåÄ³½ºÆÃ..

	
	// AI Server
	if (insertIndex == -1)
		return; // party doluysa çık

	BYTE byIndex = insertIndex;

	send_index = 0;
	memset(send_buff, 0x00, 256);

	SetByte(send_buff, AG_USER_PARTY, send_index);
	SetByte(send_buff, PARTY_INSERT, send_index);
	SetShort(send_buff, pParty->wIndex, send_index);
	SetByte(send_buff, byIndex, send_index);
	SetShort(send_buff, pParty->uid[insertIndex], send_index);

	m_pMain->Send_AIServer(m_pUserData->m_bZone, send_buff, send_index);
}

void CUser::PartyRemove(int memberid)
{
	int index = 0, send_index = 0, count = 0;
	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;

	if( m_sPartyIndex == -1 ) return;
	if( memberid < 0 || memberid >= MAX_USER ) return;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[memberid];	// Á¦°ÅµÉ »ç¶÷...
	if( !pUser ) return;
	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) {					// ÀÌ»óÇÑ °æ¿ì
		pUser->m_sPartyIndex = -1;
		m_sPartyIndex = -1;
		return;
	}

	if( memberid != m_Sid ) {					// ÀÚ±âÀÚ½Å Å»Åğ°¡ ¾Æ´Ñ°æ¿ì
		if( pParty->uid[0] != m_Sid ) return;	// ¸®´õ¸¸ ¸â¹ö »èÁ¦ ÇÒ¼ö ÀÖÀ½..
	}
	else {
		if( pParty->uid[0] == memberid ) {		// ¸®´õ°¡ Å»ÅğÇÏ¸é ÆÄÆ¼ ±úÁü
			PartyDelete();
			return;
		}
	}

	for( int i=0; i<8; i++ ) {
		if( pParty->uid[i] != -1 ) {
			if( pParty->uid[i] == memberid ) {
				count--;
			}
			count++;
		}
	}
	if( count == 1 ) {		// ¸®´õ È¥ÀÚ ³²Àº °æ¿ì ÆÄÆ¼ ±úÁü
		PartyDelete();
		return;
	}

	char send_buff[256]; memset( send_buff, 0x00, 256 );
	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_REMOVE, send_index );
	SetShort( send_buff, memberid, send_index );
	m_pMain->Send_PartyMember( m_sPartyIndex, send_buff, send_index );	// »èÁ¦µÈ ÀÎ¿øÀ» ºê·ÎµåÄ³½ºÆÃ..Á¦°ÅµÉ »ç¶÷ÇÑÅ×µÎ ÆĞÅ¶ÀÌ °£´Ù.

	for(int i=0; i<8; i++ ) {			// ÆÄÆ¼°¡ À¯È¿ÇÑ °æ¿ì ¿¡´Â ÆÄÆ¼ ¸®½ºÆ®¿¡¼­ »«´Ù.
		if( pParty->uid[i] != -1 ) {
			if( pParty->uid[i] == memberid ) {
				pParty->uid[i] = -1;
				pParty->sHp[i] = 0;
				pParty->bLevel[i] = 0;
				pParty->sClass[i] = 0;
				pUser->m_sPartyIndex = -1;
			}
		}
	}
	// AI Server
	send_index = 0; memset( send_buff, 0x00, 256 );
	SetByte( send_buff, AG_USER_PARTY, send_index );
	SetByte( send_buff, PARTY_REMOVE, send_index );
	SetShort( send_buff, pParty->wIndex, send_index );
	SetShort( send_buff, memberid, send_index );
	m_pMain->Send_AIServer(m_pUserData->m_bZone, send_buff, send_index);
}

void CUser::PartyDelete()
{
	int send_index = 0;
	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;
	if( m_sPartyIndex == -1 ) return;

	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) {
		m_sPartyIndex = -1;
		return;
	}
	for( int i=0; i<8; i++ ) {
		if( pParty->uid[i] < 0 || pParty->uid[i] >= MAX_USER ) continue;
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[i]];
		if( !pUser ) continue;
		pUser->m_sPartyIndex = -1;
	}

	char send_buff[256]; memset( send_buff, 0x00, 256 );
	SetByte( send_buff, WIZ_PARTY, send_index );
	SetByte( send_buff, PARTY_DELETE, send_index );
	m_pMain->Send_PartyMember( pParty->wIndex, send_buff, send_index );	// »èÁ¦µÈ ÀÎ¿øÀ» ºê·ÎµåÄ³½ºÆÃ..

	// AI Server
	send_index = 0; memset( send_buff, 0x00, 256 );
	SetByte( send_buff, AG_USER_PARTY, send_index );
	SetByte( send_buff, PARTY_DELETE, send_index );
	SetShort( send_buff, pParty->wIndex, send_index );
	m_pMain->Send_AIServer(m_pUserData->m_bZone, send_buff, send_index);

	EnterCriticalSection( &g_region_critical );

	m_pMain->m_PartyArray.DeleteData( pParty->wIndex );

	LeaveCriticalSection( &g_region_critical );
}

void CUser::ExchangeProcess(char *pBuf)
{
	int index = 0;
	BYTE subcommand = GetByte( pBuf, index );

	switch( subcommand ) {
	case EXCHANGE_REQ:
		ExchangeReq( pBuf+index );
		break;
	case EXCHANGE_AGREE:
		ExchangeAgree( pBuf+index );
		break;
	case EXCHANGE_ADD:
		ExchangeAdd( pBuf+index );
		break;
	case EXCHANGE_DECIDE:
		ExchangeDecide();
		break;
	case EXCHANGE_CANCEL:
		ExchangeCancel();
		break;
	}
}

void CUser::ExchangeReq(char *pBuf)
{
	int index = 0, destid = -1, send_index = 0, type = 0;
	CUser* pUser = NULL;
	char buff[256];	memset( buff, 0x00, 256 );

	destid = GetShort( pBuf, index );
	if( destid < 0 || destid >= MAX_USER ) goto fail_return;

	// ±³È¯ ¾ÈµÇ°Ô.....
	if( m_bResHpType == USER_DEAD || m_pUserData->m_sHp == 0 )	{
		printf("### ExchangeProcess Fail : name=%s(%d), m_bResHpType=%d, hp=%d, x=%d, z=%d ###\n", m_pUserData->m_id, m_Sid, m_bResHpType, m_pUserData->m_sHp, (int)m_pUserData->m_curx, (int)m_pUserData->m_curz);
		goto fail_return;
	}

	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[destid];
	if( !pUser ) goto fail_return;
	if( pUser->m_sExchangeUser != -1 ) goto fail_return;
	if( pUser->m_pUserData->m_bNation != m_pUserData->m_bNation ) goto fail_return;

	m_sExchangeUser = destid;
	pUser->m_sExchangeUser = m_Sid;

	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_REQ, send_index );
	SetShort( buff, m_Sid, send_index );
	pUser->Send( buff, send_index );
	
	return;

fail_return:
	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_CANCEL, send_index );
	Send( buff, send_index );
}

void CUser::ExchangeAgree(char* pBuf)
{
	int index = 0, destid = -1, send_index = 0;
	CUser* pUser = NULL;
	char buff[256];	memset( buff, 0x00, 256 );

	BYTE result = GetByte( pBuf, index );

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) {
		m_sExchangeUser = -1;
		return;
	}
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) {
		m_sExchangeUser = -1;
		return;
	}
	if( result == 0x00 ) {		// °ÅÀı
		m_sExchangeUser = -1;
		pUser->m_sExchangeUser = -1;
	}
	else {
		InitExchange(TRUE);
		pUser->InitExchange(TRUE);
	}

	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_AGREE, send_index );
	SetShort( buff, result, send_index );
	pUser->Send( buff, send_index );
}

void CUser::ExchangeAdd(char *pBuf)
{
	int index = 0, send_index = 0, count = 0, itemid = 0, duration = 0;
	CUser* pUser = NULL;
	_EXCHANGE_ITEM* pItem = NULL;
	_ITEM_TABLE* pTable = NULL;
	list<_EXCHANGE_ITEM*>::iterator	Iter;
	char buff[256];	memset( buff, 0x00, 256 );
	BYTE pos;
	BOOL bAdd = TRUE, bGold = FALSE;

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) {
		ExchangeCancel();
		return;
	}
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) {
		ExchangeCancel();
		return;
	}

	pos = GetByte( pBuf, index );
	itemid = GetDWORD( pBuf, index );
	count = GetDWORD( pBuf, index );
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable )
		goto add_fail;
	if( itemid != ITEM_GOLD && pos >= HAVE_MAX )
		goto add_fail;
	if( m_bExchangeOK )
		goto add_fail;
	if( itemid == ITEM_GOLD ) {
		if( count > m_pUserData->m_iGold ) goto add_fail;
		if( count <= 0 ) goto add_fail;
		for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
			if( (*Iter)->itemid == ITEM_GOLD ) {
				(*Iter)->count += count;
				m_pUserData->m_iGold -= count;
				bAdd = FALSE;
				break;
			}
		}
		if( bAdd )
			m_pUserData->m_iGold -= count;
	}
	else if( m_MirrorItem[pos].nNum == itemid ) {
		if( m_MirrorItem[pos].sCount < count ) goto add_fail;
		if( pTable->m_bCountable ) {		// Áßº¹Çã¿ë ¾ÆÀÌÅÛ
			for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
				if( (*Iter)->itemid == itemid ) {
					(*Iter)->count += count;
					m_MirrorItem[pos].sCount -= count;
					bAdd = FALSE;
					break;
				}
			}
		}
		if( bAdd )
			m_MirrorItem[pos].sCount -= count;
	
		duration = m_MirrorItem[pos].sDuration;
		if( m_MirrorItem[pos].sCount <= 0 || pTable->m_bCountable == 0 ) {
			m_MirrorItem[pos].nNum = 0;
			m_MirrorItem[pos].sDuration = 0;
			m_MirrorItem[pos].sCount = 0;
			m_MirrorItem[pos].nSerialNum = 0;
		}
	}
	else
		goto add_fail;
	for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
		if( (*Iter)->itemid == ITEM_GOLD ) {
			bGold = TRUE;
			break;
		}
	}
	if( m_ExchangeItemList.size() > ( (bGold) ? 13 : 12 ) )
		goto add_fail;

	if( bAdd ) {		// Gold °¡ Áßº¹µÇ¸é Ãß°¡ÇÏÁö ¾Ê´Â´ô..
		pItem = new _EXCHANGE_ITEM;
		pItem->itemid = itemid;
		pItem->duration = duration;
		pItem->count = count;
		pItem->nSerialNum = m_MirrorItem[pos].nSerialNum;
		m_ExchangeItemList.push_back(pItem);
	}

	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_ADD, send_index );
	SetByte( buff, 0x01, send_index );
	Send( buff, send_index );
	
	send_index = 0;
	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_OTHERADD, send_index );
	SetDWORD( buff, itemid, send_index );
	SetDWORD( buff, count, send_index );
	SetShort( buff, duration, send_index );
	pUser->Send( buff, send_index );

	return;

add_fail:
	SetByte( buff, WIZ_EXCHANGE, send_index );
	SetByte( buff, EXCHANGE_ADD, send_index );
	SetByte( buff, 0x00, send_index );
	Send( buff, send_index );
}

void CUser::ExchangeDecide()
{
	int send_index = 0, getmoney = 0, putmoney = 0;
	CUser* pUser = NULL;
	_EXCHANGE_ITEM* pItem = NULL;
	char buff[256];	memset( buff, 0x00, 256 );
	BOOL bSuccess = TRUE;

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) {
		ExchangeCancel();
		return;
	}
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) {
		ExchangeCancel();
		return;
	}
	if( !pUser->m_bExchangeOK ) {
		m_bExchangeOK = 0x01;
		SetByte( buff, WIZ_EXCHANGE, send_index );
		SetByte( buff, EXCHANGE_OTHERDECIDE, send_index );
		pUser->Send( buff, send_index );
	}
	else {										// µÑ´Ù ½ÂÀÎÇÑ °æ¿ì
		list<_EXCHANGE_ITEM*>::iterator	Iter;
		if( !ExecuteExchange() || !pUser->ExecuteExchange() ) {				// ±³È¯ ½ÇÆĞ
			for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
				if( (*Iter)->itemid == ITEM_GOLD ) {
					m_pUserData->m_iGold += (*Iter)->count;		// µ·¸¸ ¹é¾÷
					break;
				}
			}
			
			for( Iter = pUser->m_ExchangeItemList.begin(); Iter != pUser->m_ExchangeItemList.end(); Iter++ ) {
				if( (*Iter)->itemid == ITEM_GOLD ) {
					pUser->m_pUserData->m_iGold += (*Iter)->count;		// µ·¸¸ ¹é¾÷
					break;
				}
			}

			bSuccess = FALSE;
		}
		if( bSuccess ) {
			getmoney = ExchangeDone();						// ½ÇÁ¦ µ¥ÀÌÅÍ ±³È¯...
			putmoney = pUser->ExchangeDone();
			if( getmoney > 0 )
				ItemLogToAgent( m_pUserData->m_id, pUser->m_pUserData->m_id, ITEM_EXCHANGE_GET, 0, ITEM_GOLD, getmoney, 0 );
			if( putmoney > 0 )
				ItemLogToAgent( m_pUserData->m_id, pUser->m_pUserData->m_id, ITEM_EXCHANGE_PUT, 0, ITEM_GOLD, putmoney, 0 );
			
			SetByte( buff, WIZ_EXCHANGE, send_index );
			SetByte( buff, EXCHANGE_DONE, send_index );
			SetByte( buff, 0x01, send_index );
			SetDWORD( buff, m_pUserData->m_iGold, send_index );
			SetShort( buff, pUser->m_ExchangeItemList.size(), send_index );
			for( Iter = pUser->m_ExchangeItemList.begin(); Iter != pUser->m_ExchangeItemList.end(); Iter++ ) {
				SetByte( buff, (*Iter)->pos, send_index );		// »õ·Î µé¾î°¥ ÀÎº¥Åä¸® À§Ä¡
				SetDWORD( buff, (*Iter)->itemid, send_index );
				SetShort( buff, (*Iter)->count, send_index );
				SetShort( buff, (*Iter)->duration, send_index );

				ItemLogToAgent( m_pUserData->m_id, pUser->m_pUserData->m_id, ITEM_EXCHANGE_GET, (*Iter)->nSerialNum, (*Iter)->itemid, (*Iter)->count, (*Iter)->duration );
			}
			Send( buff, send_index );		// ³ªÇÑÅ× º¸³»°í...

			memset( buff, 0x00, 256 ); send_index = 0;
			SetByte( buff, WIZ_EXCHANGE, send_index );
			SetByte( buff, EXCHANGE_DONE, send_index );
			SetByte( buff, 0x01, send_index );
			SetDWORD( buff, pUser->m_pUserData->m_iGold, send_index );
			SetShort( buff, m_ExchangeItemList.size(), send_index );
			for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
				SetByte( buff, (*Iter)->pos, send_index );		// »õ·Î µé¾î°¥ ÀÎº¥Åä¸® À§Ä¡
				SetDWORD( buff, (*Iter)->itemid, send_index );
				SetShort( buff, (*Iter)->count, send_index );
				SetShort( buff, (*Iter)->duration, send_index );

				ItemLogToAgent( m_pUserData->m_id, pUser->m_pUserData->m_id, ITEM_EXCHANGE_PUT, (*Iter)->nSerialNum, (*Iter)->itemid, (*Iter)->count, (*Iter)->duration );
			}
			pUser->Send( buff, send_index );	// »ó´ë¹æµµ º¸³»ÁØ´Ù. 

			SendItemWeight();
			pUser->SendItemWeight();
		}
		else {
			SetByte( buff, WIZ_EXCHANGE, send_index );
			SetByte( buff, EXCHANGE_DONE, send_index );
			SetByte( buff, 0x00, send_index );
			Send( buff, send_index );
			pUser->Send( buff, send_index );
		}
		InitExchange(FALSE);
		pUser->InitExchange(FALSE);
	}
}

void CUser::ExchangeCancel()
{
	int send_index = 0;
	char buff[256];	memset( buff, 0x00, 256 );
	CUser* pUser = NULL;
	BOOL bFind = TRUE;

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) bFind = FALSE;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) bFind = FALSE;

	list<_EXCHANGE_ITEM*>::iterator	Iter;
	for( Iter = m_ExchangeItemList.begin(); Iter != m_ExchangeItemList.end(); Iter++ ) {
		if( (*Iter)->itemid == ITEM_GOLD ) {
			m_pUserData->m_iGold += (*Iter)->count;		// µ·¸¸ ¹é¾÷
			break;
		}
	}

	InitExchange(FALSE);

	if( bFind ) {
		pUser->ExchangeCancel();
		SetByte( buff, WIZ_EXCHANGE, send_index );
		SetByte( buff, EXCHANGE_CANCEL, send_index );
		pUser->Send( buff, send_index );
	}
}

void CUser::InitExchange(BOOL bStart)
{
	_EXCHANGE_ITEM* pItem = NULL;

	while(m_ExchangeItemList.size()) {
		pItem = m_ExchangeItemList.front();
		if( pItem )
			delete pItem;
		m_ExchangeItemList.pop_front();
	}
	m_ExchangeItemList.clear();

	if( bStart ) {						// ±³È¯ ½ÃÀÛ½Ã ¹é¾÷
		for(int i=0; i<HAVE_MAX; i++ ) {
			m_MirrorItem[i].nNum = m_pUserData->m_sItemArray[SLOT_MAX+i].nNum;
			m_MirrorItem[i].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+i].sDuration;
			m_MirrorItem[i].sCount = m_pUserData->m_sItemArray[SLOT_MAX+i].sCount;
			m_MirrorItem[i].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+i].nSerialNum;
		}
	}
	else {								// ±³È¯ Á¾·á½Ã Å¬¸®¾î
		m_sExchangeUser = -1;
		m_bExchangeOK = 0x00;
		for(int i=0; i<HAVE_MAX; i++ ) {
			m_MirrorItem[i].nNum = 0;
			m_MirrorItem[i].sDuration = 0;
			m_MirrorItem[i].sCount = 0;
			m_MirrorItem[i].nSerialNum = 0;
		}
	}
}

BOOL CUser::ExecuteExchange()
{
	_ITEM_TABLE* pTable = NULL;
	CUser* pUser = NULL;
	DWORD money = 0;
	short weight = 0, i=0;

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) return FALSE;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) return FALSE;

	list<_EXCHANGE_ITEM*>::iterator	Iter;
	int iCount = pUser->m_ExchangeItemList.size(); 
	for( Iter = pUser->m_ExchangeItemList.begin(); Iter != pUser->m_ExchangeItemList.end(); Iter++ ) {
//	ºñ·¯¸Ó±Û Å©¸®½º¸¶½º ÀÌ¹êÆ® >.<
		if( (*Iter)->itemid >= ITEM_NO_TRADE) {
			return FALSE;
		}
		else if( (*Iter)->itemid == ITEM_GOLD ) {
//
//		if( (*Iter)->itemid == ITEM_GOLD ) {
			money = (*Iter)->count;
		}
		else {
			pTable = m_pMain->m_ItemtableArray.GetData( (*Iter)->itemid );
			if( !pTable ) continue;
			for( i=0; i<HAVE_MAX; i++ ) {
				if( m_MirrorItem[i].nNum == 0  && pTable->m_bCountable == 0 ) {  // Áõº¹Çã¿ë ¾ÊµÇ´Â ¾ÆÀÌÅÛ!!!
					m_MirrorItem[i].nNum = (*Iter)->itemid;
					m_MirrorItem[i].sDuration = (*Iter)->duration;
					m_MirrorItem[i].sCount = (*Iter)->count;
					m_MirrorItem[i].nSerialNum = (*Iter)->nSerialNum;
					(*Iter)->pos = i;							// ÆĞÅ¶¿ë µ¥ÀÌÅÍ...
					weight += pTable->m_sWeight;
					break;
				}
				else if( m_MirrorItem[i].nNum == (*Iter)->itemid && pTable->m_bCountable == 1 ) {	// Áõº¹Çã¿ë ¾ÆÀÌÅÛ!!!				
					m_MirrorItem[i].sCount += (*Iter)->count;
					if( m_MirrorItem[i].sCount > MAX_ITEM_COUNT )
						m_MirrorItem[i].sCount = MAX_ITEM_COUNT;
					weight += ( pTable->m_sWeight * (*Iter)->count );
					(*Iter)->pos = i;							// ÆĞÅ¶¿ë µ¥ÀÌÅÍ...
					break;
				}
			}

			if( i == HAVE_MAX && pTable->m_bCountable == 1 ) {	// Áßº¹ Çã¿ë ¾ÆÀÌÅÛÀÎµ¥ ±âÁ¸¿¡ °¡Áö°í ÀÖÁö ¾ÊÀº °æ¿ì ºó°÷¿¡ Ãß°¡			
				for( i=0; i<HAVE_MAX; i++ ) {
					if( m_MirrorItem[i].nNum == 0 ) {
						m_MirrorItem[i].nNum = (*Iter)->itemid;
						m_MirrorItem[i].sDuration = (*Iter)->duration;
						m_MirrorItem[i].sCount = (*Iter)->count;
						(*Iter)->pos = i;							// ÆĞÅ¶¿ë µ¥ÀÌÅÍ...
						weight += ( pTable->m_sWeight * (*Iter)->count );
						break;
					}
				}
			}
			
			if( Iter != pUser->m_ExchangeItemList.end() && i == HAVE_MAX )
				return FALSE;		// ÀÎº¥Åä¸® °ø°£ ºÎÁ·...
		}
	}

	if( (weight + m_sItemWeight) > m_sMaxWeight ) return FALSE;		// Too much weight! 

	return TRUE;
}

int CUser::ExchangeDone()
{
	int money = 0;
	CUser* pUser = NULL;
	_ITEM_TABLE* pTable = NULL;

	if( m_sExchangeUser < 0 || m_sExchangeUser >= MAX_USER ) return 0;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sExchangeUser];
	if( !pUser ) return 0;

	list<_EXCHANGE_ITEM*>::iterator	Iter;
	for( Iter = pUser->m_ExchangeItemList.begin(); Iter != pUser->m_ExchangeItemList.end(); ) {
		if( (*Iter)->itemid == ITEM_GOLD ) {
			money = (*Iter)->count;
			delete (*Iter);
			Iter = pUser->m_ExchangeItemList.erase(Iter);
			continue;
		}

		Iter++;
	}
	
	if( money > 0 ) 
		m_pUserData->m_iGold += money;		// »ó´ë¹æÀÌ ÁØ µ·.
	for( int i=0; i<HAVE_MAX; i++ ) {			// ¼º°ø½Ã ¸®½ºÅä¾î..
		m_pUserData->m_sItemArray[SLOT_MAX+i].nNum = m_MirrorItem[i].nNum;
		m_pUserData->m_sItemArray[SLOT_MAX+i].sDuration = m_MirrorItem[i].sDuration;
		m_pUserData->m_sItemArray[SLOT_MAX+i].sCount = m_MirrorItem[i].sCount;
		m_pUserData->m_sItemArray[SLOT_MAX+i].nSerialNum = m_MirrorItem[i].nSerialNum;

		pTable = m_pMain->m_ItemtableArray.GetData(m_pUserData->m_sItemArray[SLOT_MAX+i].nNum);
		if( !pTable ) continue;
		if( pTable->m_bCountable == 0 && m_pUserData->m_sItemArray[SLOT_MAX+i].nSerialNum == 0 )
			m_pUserData->m_sItemArray[SLOT_MAX+i].nSerialNum = m_pMain->GenerateItemSerial();
	}

	return money;
}

void CUser::SkillPointChange(char *pBuf)
{
	int index = 0, send_index = 0, value = 0;
	BYTE type = 0x00;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	type = GetByte( pBuf, index );
	if( type > 0x08 ) goto fail_return;
	if( m_pUserData->m_bstrSkill[0] < 1 ) goto fail_return;
	if( (m_pUserData->m_bstrSkill[type]+1) > m_pUserData->m_bLevel ) goto fail_return;

	m_pUserData->m_bstrSkill[0] -= 1;
	m_pUserData->m_bstrSkill[type] += 1;

	return;

fail_return:
	SetByte( send_buff, WIZ_SKILLPT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	SetByte( send_buff, m_pUserData->m_bstrSkill[type], send_index );
	Send( send_buff, send_index );
}

void CUser::UpdateGameWeather(char *pBuf, BYTE type)
{
	int index = 0, send_index = 0, year = 0, month = 0, date = 0;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	if( m_pUserData->m_bAuthority != 0 )	// is this user administrator?
		return;

	if( type == WIZ_WEATHER ) {
		m_pMain->m_nWeather = GetByte( pBuf, index );
		m_pMain->m_nAmount = GetShort( pBuf, index );

		SetByte( send_buff, WIZ_WEATHER, send_index );
		SetByte( send_buff, (BYTE)m_pMain->m_nWeather, send_index );
		SetShort( send_buff, m_pMain->m_nAmount, send_index );
		m_pMain->Send_All( send_buff, send_index );
	}
	else if( type == WIZ_TIME ) {
		year = GetShort( pBuf, index );
		month = GetShort( pBuf, index );
		date = GetShort( pBuf, index );
		m_pMain->m_nHour = GetShort( pBuf, index );
		m_pMain->m_nMin = GetShort( pBuf, index );

		SetByte( send_buff, WIZ_TIME, send_index );
		SetShort( send_buff, year, send_index );
		SetShort( send_buff, month, send_index );
		SetShort( send_buff, date, send_index );
		SetShort( send_buff, m_pMain->m_nHour, send_index );
		SetShort( send_buff, m_pMain->m_nMin, send_index );
		m_pMain->Send_All( send_buff, send_index );
	}
}

void CUser::ClassChange(char *pBuf)
{
	int index = 0, classcode = 0, send_index = 0, type=0, sub_type = 0, money = 0, old_money=0;
	char send_buff[128]; memset( send_buff, NULL, 128 );
	BOOL bSuccess = FALSE;

	type = GetByte( pBuf, index );

	if(type == CLASS_CHANGE_REQ)	{		// ÀüÁ÷¿äÃ»
		ClassChangeReq();
		return;
	}
	else if(type == ALL_POINT_CHANGE)	{	// Æ÷ÀÎÆ® ÃÊ±âÈ­
		AllPointChange();
		return;
	}
	else if(type == ALL_SKILLPT_CHANGE)	{   // ½ºÅ³ ÃÊ±âÈ­
		AllSkillPointChange();
		return;
	}
	else if(type == CHANGE_MONEY_REQ)	{   // Æ÷ÀÎÆ® & ½ºÅ³ ÃÊ±âÈ­¿¡ µ·ÀÌ ¾ó¸¶ÀÎÁö¸¦ ¹¯´Â ¼­ºê ÆĞÅ¶
		sub_type = GetByte( pBuf, index );

		money = pow(( m_pUserData->m_bLevel * 2 ), 3.4);
		money = ( money / 100 )*100;
		if( m_pUserData->m_bLevel < 30)		money = money * 0.4;
		else if( m_pUserData->m_bLevel >= 30 && m_pUserData->m_bLevel < 60 ) money = money * 1;
		else if( m_pUserData->m_bLevel >= 60 && m_pUserData->m_bLevel <= 90 ) money = money * 1.5;

		if( sub_type == 1 )		{			// ´É·ÂÄ¡ Æ÷ÀÎÆ®
			if( m_pMain->m_sDiscount == 1 && m_pMain->m_byOldVictory == m_pUserData->m_bNation )		{	// ÇÒÀÎ½ÃÁ¡ÀÌ°í ½Â¸®±¹ÀÌ¶ó¸é
				old_money = money;
				money = money * 0.5;
				//printf("^^ ClassChange -  point Discount ,, money=%d->%d\n", old_money, money);
			}

			if( m_pMain->m_sDiscount == 2 )		{	
				old_money = money;
				money = money * 0.5;
			}

			SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
			SetByte( send_buff, CHANGE_MONEY_REQ, send_index );
			SetDWORD( send_buff, money, send_index );
			Send( send_buff, send_index );
		}
		else if( sub_type == 2 )		{	// skill Æ÷ÀÎÆ®
			money = money * 1.5;			// ½ºÅ³Àº ÇÑ¹ø ´õ 
			if( m_pMain->m_sDiscount == 1 && m_pMain->m_byOldVictory == m_pUserData->m_bNation )		{	// ÇÒÀÎ½ÃÁ¡ÀÌ°í ½Â¸®±¹ÀÌ¶ó¸é
				old_money = money;
				money = money * 0.5;
				//printf("^^ ClassChange -  skillpoint Discount ,, money=%d->%d\n", old_money, money);
			}

			if( m_pMain->m_sDiscount == 2 )		{	
				old_money = money;
				money = money * 0.5;
			}
			
			SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
			SetByte( send_buff, CHANGE_MONEY_REQ, send_index );
			SetDWORD( send_buff, money, send_index );
			Send( send_buff, send_index );
		}
		return;
	}

	classcode = GetByte( pBuf, index );

	switch( m_pUserData->m_sClass ) {
	case KARUWARRRIOR:
		if( classcode == BERSERKER || classcode == GUARDIAN )
			bSuccess = TRUE;
		break;
	case KARUROGUE:
		if( classcode == HUNTER || classcode == PENETRATOR )
			bSuccess = TRUE;
		break;
	case KARUWIZARD:
		if( classcode == SORSERER || classcode == NECROMANCER )
			bSuccess = TRUE;
		break;
	case KARUPRIEST:
		if( classcode == SHAMAN || classcode == DARKPRIEST )
			bSuccess = TRUE;
		break;
	case ELMORWARRRIOR:
		if( classcode == BLADE || classcode == PROTECTOR )
			bSuccess = TRUE;
		break;
	case ELMOROGUE:
		if( classcode == RANGER || classcode == ASSASSIN )
			bSuccess = TRUE;
		break;
	case ELMOWIZARD:
		if( classcode == MAGE || classcode == ENCHANTER )
			bSuccess = TRUE;
		break;
	case ELMOPRIEST:
		if( classcode == CLERIC || classcode == DRUID )
			bSuccess = TRUE;
		break;
	}

	memset( send_buff, NULL, 128 );	send_index = 0;
	if( !bSuccess ) {
		SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
		SetByte( send_buff, CLASS_CHANGE_RESULT, send_index );
		SetByte( send_buff, 0x00, send_index );
		Send( send_buff, send_index );
	}
	else {
		m_pUserData->m_sClass = classcode;
		if( m_sPartyIndex != -1 ) {
			SetByte( send_buff, WIZ_PARTY, send_index );
			SetByte( send_buff, PARTY_CLASSCHANGE, send_index );
			SetShort( send_buff, m_Sid, send_index );
			SetShort( send_buff, m_pUserData->m_sClass, send_index );
			m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
		}
	}
}


BOOL CUser::ItemEquipAvailable(_ITEM_TABLE *pTable)
{
	if( !pTable )
		return FALSE;
//	if( pTable->m_bReqLevel > m_pUserData->m_bLevel )
//		return FALSE;
	if( pTable->m_bReqRank > m_pUserData->m_bRank )
		return FALSE;
	if( pTable->m_bReqTitle > m_pUserData->m_bTitle )
		return FALSE;
	if( pTable->m_bReqStr > m_pUserData->m_bStr )
		return FALSE;
	if( pTable->m_bReqSta > m_pUserData->m_bSta )
		return FALSE;
	if( pTable->m_bReqDex > m_pUserData->m_bDex )
		return FALSE;
	if( pTable->m_bReqIntel > m_pUserData->m_bIntel )
		return FALSE;
	if( pTable->m_bReqCha > m_pUserData->m_bCha )
		return FALSE;

	return TRUE;
}

void CUser::ChatTargetSelect(char *pBuf)
{
	int index = 0, send_index = 0, i=0, idlen = 0;
	CUser* pUser = NULL;
	char chatid[MAX_ID_SIZE+1]; memset( chatid, 0x00, MAX_ID_SIZE+1 );
	char send_buff[128]; memset( send_buff, 0x00, 128 );

	BYTE type;
	type = GetByte( pBuf, index );

	SetByte( send_buff, WIZ_CHAT_TARGET, send_index );
	SetByte( send_buff, type, send_index );
		switch( type )
		{
			case 0x01: // WHISPER
					idlen = GetShort( pBuf, index );
				//	if( idlen > MAX_ID_SIZE || idlen < 0 ) return;
					GetString( chatid, pBuf, idlen, index );

					for(i=0; i<MAX_USER; i++ ) {
						pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
						if( pUser && pUser->GetState() == STATE_GAMESTART ) {
							if( _strnicmp( chatid, pUser->m_pUserData->m_id, MAX_ID_SIZE ) == 0 ) {
								m_sPrivateChatUser = i;
								break;
							}
						}
					}

					if( i == MAX_USER ) {
						SetShort( send_buff, 0x00, send_index );
					} else if( pUser->m_sPrivateChat == 0x01 ) {
						SetByte( send_buff, -1, send_index );
						SetByte( send_buff, -1, send_index );
						SetShort( send_buff, strlen( chatid ), send_index );
						SetString( send_buff, chatid, strlen( chatid ), send_index );
					} else if( pUser->m_sPrivateChat == 0x00 ) {
						SetShort( send_buff, 0x01, send_index );
						SetShort( send_buff, strlen( chatid ), send_index );
						SetString( send_buff, chatid, strlen( chatid ), send_index );
					}
				break;
			case 0x02: // WHISPER BLOCK
					BYTE whisper;
					whisper =  GetByte( pBuf, index );

					if(whisper == 0x00) { // OPEN
						m_sPrivateChat = 0x00;
					} else if(whisper == 0x01) { // BLOCK
						m_sPrivateChat = 0x01;
					} return;
				break;
			case 0x03: // ask question
				// SEND-->WIZ_CHAT_TARGET-->350302026171
					return;
				break;
			default:
				return;
		}
	Send( send_buff, send_index );
}

// AI server¿¡ UserÁ¤º¸¸¦ ÀüºÎ Àü¼Û...
void CUser::SendUserInfo(char *temp_send, int &index)
{
	SetShort( temp_send, m_Sid, index );
	SetShort( temp_send, strlen(m_pUserData->m_id), index );
	SetString( temp_send, m_pUserData->m_id, strlen(m_pUserData->m_id), index );
	SetByte( temp_send, m_pUserData->m_bZone, index );
	SetShort( temp_send, m_iZoneIndex, index );
	SetByte( temp_send, m_pUserData->m_bNation, index );
	SetByte( temp_send, m_pUserData->m_bLevel, index );
	SetShort( temp_send, m_pUserData->m_sHp, index );
	SetShort( temp_send, m_pUserData->m_sMp, index );
	SetShort( temp_send, m_sTotalHit * m_bAttackAmount / 100, index );    // Ç¥½Ã
	SetShort( temp_send, m_sTotalAc + m_sACAmount , index );	// Ç¥½Ã
	Setfloat( temp_send, m_sTotalHitrate, index );
	Setfloat( temp_send, m_sTotalEvasionrate, index );
	SetShort( temp_send, m_sPartyIndex, index );
	SetByte( temp_send, m_pUserData->m_bAuthority, index );
}

void CUser::CountConcurrentUser()
{
	if( m_pUserData->m_bAuthority != 0 )
		return;
	int usercount = 0, send_index = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++ ) {
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if( !pUser )
			continue;
		if( pUser->GetState() == STATE_GAMESTART )
			usercount++;
	}

	SetByte( send_buff, WIZ_CONCURRENTUSER, send_index );
	SetShort( send_buff, usercount, send_index );
	Send( send_buff, send_index );
}

void CUser::LoyaltyDivide(short tid)
{
	int send_index = 0;
	char send_buff[256]; memset( send_buff, NULL, 256 );	

	int levelsum = 0, individualvalue = 0;
	short temp_loyalty = 0;
	short level_difference = 0; short loyalty_source = 0; short loyalty_target = 0; 
	BYTE total_member = 0; short average_level = 0;

	CUser* pUser = NULL;

	_PARTY_GROUP* pParty = NULL;		// Party Pointer Initialization!
	if( m_sPartyIndex < 0 ) return;
	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) return;

	CUser* pTUser = NULL ;									  // Target Pointer initialization!		
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];     // Get target info.  
	if( !pTUser ) return;									  // Check if target exists and not already dead.		

	for( int i = 0; i < 8; i++ ) {		// Get total level and number of members in party.
		if( pParty->uid[i] != -1 ) {
			levelsum += pParty->bLevel[i];
			total_member ++;			
		}
	}

	if (levelsum <= 0) return;		// Protection codes.
	if (total_member <= 0) return;

	average_level = levelsum / total_member;	// Calculate average level.

	//	This is for the Event Battle on Wednesday :(
	if (m_pMain->m_byBattleOpen) {
		if (m_pUserData->m_bZone == ZONE_BATTLE) {
			if (pTUser->m_pUserData->m_bNation == KARUS) {
				m_pMain->m_sKarusDead++;
				//printf("++ LoyaltyDivide - ka=%d, el=%d\n", m_pMain->m_sKarusDead, m_pMain->m_sElmoradDead);
			}
			else if (pTUser->m_pUserData->m_bNation == ELMORAD) {
				m_pMain->m_sElmoradDead++;
				//printf("++ LoyaltyDivide - ka=%d, el=%d\n", m_pMain->m_sKarusDead, m_pMain->m_sElmoradDead);
			}
		}
	}
		
	if (pTUser->m_pUserData->m_bNation != m_pUserData->m_bNation) {		// Different nations!!!
		level_difference = pTUser->m_pUserData->m_bLevel - average_level;	// Calculate difference!

		if (pTUser->m_pUserData->m_iLoyalty <= 0) {	   // No cheats allowed...
			loyalty_source = 0;
			loyalty_target = 0;
		}
		else if (level_difference > 5) {	// At least six levels higher...
			loyalty_source  = 50;
			loyalty_target = -25;
		}
		else if (level_difference < -5) {	// At least six levels lower...
			loyalty_source  = 10; 
			loyalty_target = -5;
		}
		else {		// Within the 5 and -5 range...
			loyalty_source  =  30;
			loyalty_target = -15;
		}
	}
	else {		// Same Nation!!! 
		individualvalue = -1000 ;

		for (int j = 0 ; j < 8 ; j++) {		// Distribute loyalty amongst party members.
			if( pParty->uid[j] != -1 || pParty->uid[j] >= MAX_USER ) {
				pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[j]];
				if( !pUser ) continue;

				//printf("LoyaltyDivide 111 - user1=%s, %d\n", pUser->m_pUserData->m_id, pUser->m_pUserData->m_iLoyalty);
			
				pUser->m_pUserData->m_iLoyalty += individualvalue;	
				if (pUser->m_pUserData->m_iLoyalty < 0) pUser->m_pUserData->m_iLoyalty = 0;	// Cannot be less than zero.

				//printf("LoyaltyDivide 222 - user1=%s, %d\n", pUser->m_pUserData->m_id, pUser->m_pUserData->m_iLoyalty);

				memset( send_buff, NULL, 256 ); send_index = 0;	
				SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );	// Send result to source.
				SetDWORD( send_buff, pUser->m_pUserData->m_iLoyalty, send_index );
				pUser->Send( send_buff, send_index );			
			}
		}		
		
		return;
	}
//
	if (m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3) { 
		loyalty_source  = 2 * loyalty_source;
	}
//
	for (int j = 0 ; j < 8 ; j++) {		// Distribute loyalty amongst party members.
		if( pParty->uid[j] != -1 || pParty->uid[j] >= MAX_USER ) {
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[j]];
			if( !pUser ) continue;

			//printf("LoyaltyDivide 333 - user1=%s, %d\n", pUser->m_pUserData->m_id, pUser->m_pUserData->m_iLoyalty);
			individualvalue = pUser->m_pUserData->m_bLevel * loyalty_source / levelsum ;
			pUser->m_pUserData->m_iLoyalty += individualvalue;	
			if (pUser->m_pUserData->m_iLoyalty < 0) pUser->m_pUserData->m_iLoyalty = 0;

			//printf("LoyaltyDivide 444 - user1=%s, %d\n", pUser->m_pUserData->m_id, pUser->m_pUserData->m_iLoyalty);

			memset( send_buff, NULL, 256 ); send_index = 0;	
			SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );	// Send result to source.
			SetDWORD( send_buff, pUser->m_pUserData->m_iLoyalty, send_index );
			pUser->Send( send_buff, send_index );

			individualvalue = 0;
		}
	}

	pTUser->m_pUserData->m_iLoyalty += loyalty_target;	// Recalculate target loyalty.
	if (pTUser->m_pUserData->m_iLoyalty < 0) pTUser->m_pUserData->m_iLoyalty = 0;

	//printf("LoyaltyDivide 555 - user1=%s, %d\n", pTUser->m_pUserData->m_id, pTUser->m_pUserData->m_iLoyalty);
	
	memset( send_buff, NULL, 256 ); send_index = 0;		// Send result to target.
	SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );
	SetDWORD( send_buff, pTUser->m_pUserData->m_iLoyalty, send_index );
	pTUser->Send( send_buff, send_index );
}

void CUser::Dead()
{
	int send_index = 0;
	char chatstr[1024]; memset( chatstr, NULL, 1024 );
	char finalstr[1024]; memset( finalstr, NULL, 1024 );
	char send_buff[1024]; memset( send_buff, 0x00, 1024 );
	char strKnightsName[MAX_ID_SIZE+1];		memset( strKnightsName, 0x00, MAX_ID_SIZE+1 );
	CKnights* pKnights = NULL;
	std::string buff;
	std::string buff2;

	SetByte( send_buff, WIZ_DEAD, send_index );
	SetShort( send_buff, m_Sid, send_index );
	m_pMain->Send_Region( send_buff, send_index , m_pUserData->m_bZone, m_RegionX, m_RegionZ );

	m_bResHpType = USER_DEAD;

	Send( send_buff, send_index );		// À¯Àú¿¡°Ô´Â ¹Ù·Î µ¥µå ÆĞÅ¶À» ³¯¸²... (ÇÑ ¹ø ´õ º¸³¿, À¯·ÉÀ» ¾ø¾Ö±â À§ÇØ¼­)

	memset( send_buff, NULL, 1024 );
	wsprintf(send_buff, "----> User Dead ,, nid=%d, name=%s, type=%d, x=%d, z=%d ******", m_Sid, m_pUserData->m_id, m_bResHpType, (int)m_pUserData->m_curx, (int)m_pUserData->m_curz);
	//Timeprintf(send_buff);

	memset( send_buff, NULL, 1024 );		send_index = 0;
	if( m_pUserData->m_bFame == COMMAND_CAPTAIN )	{	// ÁöÈÖ±ÇÇÑÀÌ ÀÖ´Â À¯Àú°¡ Á×´Â´Ù¸é,, ÁöÈÖ ±ÇÇÑ ¹ÚÅ»
		m_pUserData->m_bFame = CHIEF;
		SetByte( send_buff, WIZ_AUTHORITY_CHANGE, send_index );
		SetByte( send_buff, COMMAND_AUTHORITY, send_index );
		SetShort( send_buff, GetSocketID(), send_index );
		SetByte( send_buff, m_pUserData->m_bFame, send_index );
		m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ );
		Send( send_buff, send_index );

		pKnights = m_pMain->m_KnightsArray.GetData( m_pUserData->m_bKnights );
		if( pKnights )		strcpy( strKnightsName, pKnights->m_strName );
		else				strcpy( strKnightsName, "*" );
		//printf("---> Dead Captain Deprive - %s\n", m_pUserData->m_id);
		if( m_pUserData->m_bNation == KARUS )	{
			//m_pMain->Announcement( KARUS_CAPTAIN_DEPRIVE_NOTIFY, KARUS );
			::_LoadStringFromResource(IDS_KARUS_CAPTAIN_DEPRIVE, buff);
			sprintf( chatstr, buff.c_str(), strKnightsName, m_pUserData->m_id );
		}
		else if( m_pUserData->m_bNation == ELMORAD )	{
			//m_pMain->Announcement( ELMORAD_CAPTAIN_DEPRIVE_NOTIFY, ELMORAD );
			::_LoadStringFromResource(IDS_ELMO_CAPTAIN_DEPRIVE, buff);
			sprintf( chatstr, buff.c_str(), strKnightsName, m_pUserData->m_id );
		}

		memset( send_buff, NULL, 1024 );		send_index = 0;
		::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff2);
		sprintf( finalstr, buff2.c_str(), chatstr );
		//sprintf( finalstr, "## °øÁö : %s ##", chatstr );
		SetByte( send_buff, WIZ_CHAT, send_index );
		SetByte( send_buff, WAR_SYSTEM_CHAT, send_index );
		SetByte( send_buff, 1, send_index );
		SetByte( send_buff, -1, send_index );
		SetByte( send_buff, -1, send_index );
		SetByte( send_buff, 0x00, send_index );
		SetShort( send_buff, strlen(finalstr), send_index );
		SetString( send_buff, finalstr, strlen(finalstr), send_index );
		m_pMain->Send_All( send_buff, send_index, NULL, m_pUserData->m_bNation );
	}
}

void CUser::ItemWoreOut(int type, int damage)
{
	_ITEM_TABLE* pTable = NULL;
	int worerate = sqrt(damage/10.0);
	if( worerate == 0 ) return;

	if( type == ATTACK ) {
		if( m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[RIGHTHAND].nNum );
			if( pTable ) {
				if( pTable->m_bSlot != 2 )	{// 2 == DEFENCE ITEM
					if( m_pUserData->m_sItemArray[RIGHTHAND].sDuration != 0 ) {
						m_pUserData->m_sItemArray[RIGHTHAND].sDuration -= worerate;
						ItemDurationChange( RIGHTHAND, pTable->m_sDuration, m_pUserData->m_sItemArray[RIGHTHAND].sDuration, worerate );
					}
				}
			}
		}
		if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEFTHAND].nNum );
			if( pTable ) {
				if( pTable->m_bSlot != 2 ) {
					if( m_pUserData->m_sItemArray[LEFTHAND].sDuration != 0 ) {
						m_pUserData->m_sItemArray[LEFTHAND].sDuration -= worerate;
						ItemDurationChange( LEFTHAND, pTable->m_sDuration, m_pUserData->m_sItemArray[LEFTHAND].sDuration, worerate );
					}
				}
			}
		}
	}
	else if ( type == DEFENCE ) {
		if( m_pUserData->m_sItemArray[HEAD].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[HEAD].nNum );
			if( pTable ) {
				if( m_pUserData->m_sItemArray[HEAD].sDuration != 0 ) {
					m_pUserData->m_sItemArray[HEAD].sDuration -= worerate;
					ItemDurationChange( HEAD, pTable->m_sDuration, m_pUserData->m_sItemArray[HEAD].sDuration, worerate );
				}
			}
		}
		if( m_pUserData->m_sItemArray[BREAST].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[BREAST].nNum );
			if( pTable ) {
				if( m_pUserData->m_sItemArray[BREAST].sDuration != 0 ) {
					m_pUserData->m_sItemArray[BREAST].sDuration -= worerate;
					ItemDurationChange( BREAST, pTable->m_sDuration, m_pUserData->m_sItemArray[BREAST].sDuration, worerate );
				}
			}
		}
		if( m_pUserData->m_sItemArray[LEG].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEG].nNum );
			if( pTable ) {
				if( m_pUserData->m_sItemArray[LEG].sDuration != 0 ) {
					m_pUserData->m_sItemArray[LEG].sDuration -= worerate;
					ItemDurationChange( LEG, pTable->m_sDuration, m_pUserData->m_sItemArray[LEG].sDuration, worerate );
				}
			}
		}
		if( m_pUserData->m_sItemArray[GLOVE].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[GLOVE].nNum );
			if( pTable ) {
				if( m_pUserData->m_sItemArray[GLOVE].sDuration != 0 ) {
					m_pUserData->m_sItemArray[GLOVE].sDuration -= worerate;
					ItemDurationChange( GLOVE, pTable->m_sDuration, m_pUserData->m_sItemArray[GLOVE].sDuration, worerate );
				}
			}
		}
		if( m_pUserData->m_sItemArray[FOOT].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[FOOT].nNum );
			if( pTable ) {
				if( m_pUserData->m_sItemArray[FOOT].sDuration != 0 ) {
					m_pUserData->m_sItemArray[FOOT].sDuration -= worerate;
					ItemDurationChange( FOOT, pTable->m_sDuration, m_pUserData->m_sItemArray[FOOT].sDuration, worerate );
				}
			}
		}
		if( m_pUserData->m_sItemArray[RIGHTHAND].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[RIGHTHAND].nNum );
			if( pTable ) {
				if( pTable->m_bSlot == 2 ) {	// ¹æÆĞ?
					if( m_pUserData->m_sItemArray[RIGHTHAND].sDuration != 0 ) {
						m_pUserData->m_sItemArray[RIGHTHAND].sDuration -= worerate;
						ItemDurationChange( RIGHTHAND, pTable->m_sDuration, m_pUserData->m_sItemArray[RIGHTHAND].sDuration, worerate );
					}
				}
			}
		}
		if( m_pUserData->m_sItemArray[LEFTHAND].nNum != 0 ) {
			pTable = m_pMain->m_ItemtableArray.GetData( m_pUserData->m_sItemArray[LEFTHAND].nNum );
			if( pTable ) {
				if( pTable->m_bSlot == 2 ) {	// ¹æÆĞ?
					if( m_pUserData->m_sItemArray[LEFTHAND].sDuration != 0 ) {
						m_pUserData->m_sItemArray[LEFTHAND].sDuration -= worerate;
						ItemDurationChange( LEFTHAND, pTable->m_sDuration, m_pUserData->m_sItemArray[LEFTHAND].sDuration, worerate );
					}
				}
			}
		}
	}
}

void CUser::ItemDurationChange(int slot, int maxvalue, int curvalue, int amount)
{
	if( maxvalue <= 0 ) return;
	if( slot < 0 || slot > SLOT_MAX ) return;
	
	int curpercent = 0, beforepercent = 0, curbasis = 0, beforebasis = 0;
	int send_index = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	
	if( m_pUserData->m_sItemArray[slot].sDuration <= 0 ) {
		m_pUserData->m_sItemArray[slot].sDuration = 0;
		SetByte( send_buff, WIZ_DURATION, send_index );
		SetByte( send_buff, slot, send_index );
		SetShort( send_buff, 0, send_index );
		Send( send_buff, send_index );
		
		SetSlotItemValue();
		SetUserAbility();

		memset( send_buff, 0x00, 128 ); send_index = 0;
		SetByte( send_buff, WIZ_ITEM_MOVE, send_index );	// durability º¯°æ¿¡ µû¸¥ ¼öÄ¡ ÀçÁ¶Á¤...
		SetByte( send_buff, 0x01, send_index );
		SetShort( send_buff, m_sTotalHit, send_index );
		SetShort( send_buff, m_sTotalAc, send_index );
		SetShort( send_buff, m_sItemWeight, send_index );
		SetShort( send_buff, m_iMaxHp, send_index );
		SetShort( send_buff, m_iMaxMp, send_index );
		SetByte( send_buff, m_sItemStr, send_index );
		SetByte( send_buff, m_sItemSta, send_index );
		SetByte( send_buff, m_sItemDex, send_index );
		SetByte( send_buff, m_sItemIntel, send_index );
		SetByte( send_buff, m_sItemCham, send_index );
		SetByte( send_buff, m_bFireR, send_index );
		SetByte( send_buff, m_bColdR, send_index );
		SetByte( send_buff, m_bLightningR, send_index );
		SetByte( send_buff, m_bMagicR, send_index );
		SetByte( send_buff, m_bDiseaseR, send_index );
		SetByte( send_buff, m_bPoisonR, send_index );
		Send( send_buff, send_index );
		return;
	}
	curpercent = ( curvalue / (double)maxvalue) * 100;
	beforepercent = ( (curvalue+amount) / (double)maxvalue ) * 100;
	
	curbasis = curpercent / 5;
	beforebasis = beforepercent / 5;

	if( curbasis != beforebasis ) {
		SetByte( send_buff, WIZ_DURATION, send_index );
		SetByte( send_buff, slot, send_index );
		SetShort( send_buff, curvalue, send_index );
		Send( send_buff, send_index );

		if( curpercent >= 65 && curpercent < 70 )
			UserLookChange( slot, m_pUserData->m_sItemArray[slot].nNum, curvalue );
		if( curpercent >= 25 && curpercent < 30 )
			UserLookChange( slot, m_pUserData->m_sItemArray[slot].nNum, curvalue );
	}
}

void CUser::HPTimeChange(float currenttime)
{
	int send_index = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	BOOL bFlag = FALSE;

	m_fHPLastTimeNormal = currenttime;

	if( m_bResHpType == USER_DEAD ) return;

	//wsprintf(send_buff, "HPTimeChange ,, nid=%d, name=%s, hp=%d, type=%d ******", m_Sid, m_pUserData->m_id, m_pUserData->m_sHp, m_bResHpType);
	//Timeprintf( send_buff );
	memset( send_buff, 0x00, 128 );

	if( m_pUserData->m_bZone == ZONE_SNOW_BATTLE && m_pMain->m_byBattleOpen == SNOW_BATTLE )	{
		if( m_pUserData->m_sHp < 1 ) return;
		HpChange( 5 );
		return;
	}

	if( m_bResHpType == USER_STANDING ) {
		if( m_pUserData->m_sHp < 1 ) return;
		if( m_iMaxHp != m_pUserData->m_sHp )
			HpChange( (int)((m_pUserData->m_bLevel*(1+m_pUserData->m_bLevel/60.0) + 1)*0.2)+3 );

		if( m_iMaxMp != m_pUserData->m_sMp )
			MSpChange( (int)((m_pUserData->m_bLevel*(1+m_pUserData->m_bLevel/60.0) + 1)*0.2)+3 );
	}
	else if ( m_bResHpType == USER_SITDOWN ) {
		if( m_pUserData->m_sHp < 1 ) return;
		if( m_iMaxHp != m_pUserData->m_sHp ) {
			HpChange( (int)(m_pUserData->m_bLevel*(1+m_pUserData->m_bLevel/30.0) ) + 3 );
		}
		if( m_iMaxMp != m_pUserData->m_sMp ) {
			MSpChange((int)((m_iMaxMp * 5) / ((m_pUserData->m_bLevel - 1) + 30 )) + 3 ) ;
		}
	}

	/*
	³ªÁß¿¡ ¶Ç °íÄ¥°Í¿¡ ´ëºñÇØ¼­ ¿©±â¿¡ µÎ±â·Î Çß½À´Ï´Ù :

	HP(MP)°¡ ¸ğµÎ Â÷´Â µ¥ °É¸®´Â ½Ã°£ A = (·¹º§ - 1) + 30
	HP(MP)°¡ ¸ğµÎ Â÷´Â µ¥ °É¸®´Â È½¼ö B = A/5
	ÇÑ¹ø¿¡ Â÷´Â HP(MP)ÀÇ ¾ç = Max HP / B
	*/
}

void CUser::HPTimeChangeType3(float currenttime)
{
	int send_index = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );

	for (int g = 0 ; g < MAX_TYPE3_REPEAT ; g++) {	// Get the current time for all the last times...
		m_fHPLastTime[g] = currenttime;
	}

	if( m_bResHpType != USER_DEAD ) {	// Make sure the user is not dead first!!!
		for (int h = 0 ; h < MAX_TYPE3_REPEAT ; h++) {
			HpChange(m_bHPAmount[h]);	// Reduce HP...
			CUser* pUser = NULL;

			if (m_sSourceID[h] >= 0 && m_sSourceID[h] < MAX_USER) {	// Send report to the source...
				pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sSourceID[h]];
				if (pUser) {
					pUser->SendTargetHP( 0, m_Sid, m_bHPAmount[h] );
				}
			}	// ...End of : Send report to the source...

			if ( m_pUserData->m_sHp == 0) {     // Check if the target is dead.	
				// sungyong work : loyalty
				m_bResHpType = USER_DEAD;	// Officially declare the user DEAD!!!!!

				if (m_sSourceID[h] >= NPC_BAND) {	// If the killer was a NPC
//
					if( m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3) {
						ExpChange(-m_iMaxExp / 100);
						//printf("Á¤¸»·Î 1%¸¸ ±ï¿´´Ù´Ï±î¿ä ¤Ğ.¤Ğ\r\n");
					}
					else {
//
						ExpChange(-m_iMaxExp/20 );     // Reduce target experience.		
//
					}
//
				}
				else {	// You got killed by another player
					if (pUser) {	// (No more pointer mistakes....)
						if( pUser->m_sPartyIndex == -1 ) {     // Something regarding loyalty points.
							pUser->LoyaltyChange(m_Sid);
						}
						else {
							pUser->LoyaltyDivide(m_Sid);
						}
						
						pUser->GoldChange(m_Sid, 0);
					}
				}				
				// ±â¹üÀÌÀÇ ¿Ïº®ÇÑ º¸È£ ÄÚµù!!!
				InitType3();	// Init Type 3.....
				InitType4();	// Init Type 4.....
				
				if (m_sSourceID[h] >= 0 && m_sSourceID[h] < MAX_USER) {
					m_sWhoKilledMe = m_sSourceID[h];	// Who the hell killed me?
//
					if( m_pUserData->m_bZone != m_pUserData->m_bNation && m_pUserData->m_bZone < 3) {
						ExpChange(-m_iMaxExp / 100);
						//printf("Á¤¸»·Î 1%¸¸ ±ï¿´´Ù´Ï±î¿ä ¤Ğ.¤Ğ\r\n");
					}
//
				}
				
				break;	// Exit the for loop :)
			}	// ...End of : Check if the target is dead.
		}
	}
	else return;

	for (int i = 0 ; i < MAX_TYPE3_REPEAT ; i++) {	// Type 3 Cancellation Process.
		if( m_bHPDuration[i] > 0 ) {
			if( ((currenttime - m_fHPStartTime[i]) >= m_bHPDuration[i]) || m_bResHpType == USER_DEAD) {
				/*	Send Party Packet.....
				if (m_sPartyIndex != -1) {
					SetByte( send_buff, WIZ_PARTY, send_index );
					SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
					SetShort( send_buff, m_Sid, send_index );
					SetByte( send_buff, 1, send_index );
					SetByte( send_buff, 0x00, send_index);
					m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
					memset( send_buff, NULL, 128); send_index = 0 ;
				}
				//  end of Send Party Packet.....*/ 

				SetByte( send_buff, WIZ_MAGIC_PROCESS, send_index );
				SetByte( send_buff, MAGIC_TYPE3_END, send_index );	

				if (m_bHPAmount[i] > 0) {
					SetByte( send_buff, 100, send_index );
				}
				else {
					SetByte( send_buff, 200, send_index );
				}

				Send( send_buff, send_index ); 
				memset( send_buff, NULL, 128); send_index = 0 ;
				
				m_fHPStartTime[i] = 0.0f;
				m_fHPLastTime[i] = 0.0f;
				m_bHPAmount[i] = 0;
				m_bHPDuration[i] = 0;				
				m_bHPInterval[i] = 5;
				m_sSourceID[i] = -1; 
			}
		}
	}

	int buff_test = 0;
	for (int j = 0 ; j < MAX_TYPE3_REPEAT ; j++) {
		buff_test += m_bHPDuration[j];
	}
	if (buff_test == 0) m_bType3Flag = FALSE;
//
	BOOL bType3Test = TRUE;
	for (int k = 0 ; k < MAX_TYPE3_REPEAT ; k++) {
		if (m_bHPAmount[k] < 0) {
			bType3Test = FALSE;
			break;
		}
	}

	// Send Party Packet.....
	if (m_sPartyIndex != -1 && bType3Test) {
		memset( send_buff, NULL, 128); send_index = 0 ;
		SetByte( send_buff, WIZ_PARTY, send_index );
		SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetByte( send_buff, 1, send_index );
		SetByte( send_buff, 0x00, send_index);
		m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
	}
	//  end of Send Party Packet.....  //
//
}

void CUser::ItemRepair(char *pBuf)
{
	int index = 0, send_index = 0, money = 0, quantity = 0;
	int itemid = 0, pos = 0, slot = -1, durability = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	_ITEM_TABLE* pTable = NULL;

	pos = GetByte( pBuf, index );
	slot = GetByte( pBuf, index );
	itemid = GetDWORD( pBuf, index );
	if( pos == 1 ) {	// SLOT
		if( slot >= SLOT_MAX ) goto fail_return;
		if( m_pUserData->m_sItemArray[slot].nNum != itemid ) goto fail_return;
	}
	else if ( pos == 2 ) {	// INVEN
		if( slot >= HAVE_MAX ) goto fail_return;
		if( m_pUserData->m_sItemArray[SLOT_MAX+slot].nNum != itemid ) goto fail_return;
	}
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) goto fail_return;
	durability = pTable->m_sDuration;
	if( durability == 1 ) goto fail_return;
	if( pos == 1 )
		quantity = pTable->m_sDuration - m_pUserData->m_sItemArray[slot].sDuration;
	else if( pos == 2 ) 
		quantity = pTable->m_sDuration - m_pUserData->m_sItemArray[SLOT_MAX+slot].sDuration;
	
	money = (int)( ((pTable->m_iBuyPrice-10)/10000.0f) + pow(pTable->m_iBuyPrice, 0.75)) * quantity / (double)durability;
	if( money > m_pUserData->m_iGold ) goto fail_return;

	m_pUserData->m_iGold -= money;
	if( pos == 1 )
		m_pUserData->m_sItemArray[slot].sDuration = durability;
	else if( pos == 2 )
		m_pUserData->m_sItemArray[SLOT_MAX+slot].sDuration = durability;

	SetByte( send_buff, WIZ_ITEM_REPAIR, send_index );
	SetByte( send_buff, 0x01, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, WIZ_ITEM_REPAIR, send_index );
	SetByte( send_buff, 0x00, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );
}

void CUser::Type4Duration(float currenttime)
{
	int send_index = 0 ;
	char send_buff[128] ;
	memset( send_buff, 0x00, 128 ) ;
	BYTE buff_type = 0 ;					

	if (m_sDuration1 && buff_type == 0) {
		if (currenttime > (m_fStartTime1 + m_sDuration1)) {
			m_sDuration1 = 0;		
			m_fStartTime1 = 0.0f;
			m_sMaxHPAmount = 0;
			buff_type = 1 ;			
		}
	}

	if (m_sDuration2 && buff_type == 0) {
		if (currenttime > (m_fStartTime2 + m_sDuration2)) {
			m_sDuration2 = 0;		
			m_fStartTime2 = 0.0f;
			m_sACAmount = 0;
			buff_type = 2 ;
		}
	}
//
	if (m_sDuration3 && buff_type == 0) {
		if (currenttime > (m_fStartTime3 + m_sDuration3)) {
			m_sDuration3 = 0;		
			m_fStartTime3 = 0.0f;
			buff_type = 3 ;
			
			memset( send_buff, NULL, 128); send_index = 0 ;
			SetByte(send_buff, 3, send_index);	// You are now normal again!!!
			SetByte(send_buff, ABNORMAL_NORMAL, send_index);
			StateChange(send_buff);					
			memset( send_buff, NULL, 128); send_index = 0 ;
		}
	}
//
	if (m_sDuration4 && buff_type == 0) {
		if (currenttime > (m_fStartTime4 + m_sDuration4)){
			m_sDuration4 = 0;		
			m_fStartTime4 = 0.0f;
			m_bAttackAmount = 100;
			buff_type = 4 ;
		}
	}

	if (m_sDuration5 && buff_type == 0) {
		if (currenttime > (m_fStartTime5 + m_sDuration5)){
			m_sDuration5 = 0;		
			m_fStartTime5 = 0.0f;
			m_bAttackSpeedAmount = 100;	
			buff_type = 5 ;
		}
	}

	if (m_sDuration6 && buff_type == 0) {
		if (currenttime > (m_fStartTime6 + m_sDuration6)){
			m_sDuration6 = 0;		
			m_fStartTime6 = 0.0f;
			m_bSpeedAmount = 100;
			buff_type = 6 ;
		}
	}

	if (m_sDuration7 && buff_type == 0) {
		if (currenttime > (m_fStartTime7 + m_sDuration7)){
			m_sDuration7 = 0;		
			m_fStartTime7 = 0.0f;
			m_bStrAmount = 0;
			m_bStaAmount = 0;
			m_bDexAmount = 0;
			m_bIntelAmount = 0;
			m_bChaAmount = 0;
			buff_type = 7 ;
		}
	}

	if (m_sDuration8 && buff_type == 0) {
		if (currenttime > (m_fStartTime8 + m_sDuration8)){
			m_sDuration8 = 0;		
			m_fStartTime8 = 0.0f;
			m_bFireRAmount = 0;
			m_bColdRAmount = 0;
			m_bLightningRAmount = 0;
			m_bMagicRAmount = 0;
			m_bDiseaseRAmount = 0;
			m_bPoisonRAmount = 0;
			buff_type = 8 ;
		}
	}

	if (m_sDuration9 && buff_type == 0) {
		if (currenttime > (m_fStartTime9 + m_sDuration9)){
			m_sDuration9 = 0;		
			m_fStartTime9 = 0.0f;
			m_bHitRateAmount = 100;
			m_sAvoidRateAmount = 100;
			buff_type = 9 ;
		}
	}

	if (buff_type) {
		m_bType4Buff[buff_type - 1] = 0;

		SetSlotItemValue();
		SetUserAbility();
		Send2AI_UserUpdateInfo();	// AI Server¿¡ ¹Ù…Î µ¥ÀÌÅ¸ Àü¼Û....		

		/*	Send Party Packet.....
		if (m_sPartyIndex != -1) {
			SetByte( send_buff, WIZ_PARTY, send_index );
			SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
			SetShort( send_buff, m_Sid, send_index );
//			if (buff_type != 5 && buff_type != 6) {
//				SetByte( send_buff, 3, send_index );
//			}
//			else {
			SetByte( send_buff, 2, send_index );
//			}
			SetByte( send_buff, 0x00, send_index);
			m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
			memset( send_buff, NULL, 128); send_index = 0 ;
		}
		//  end of Send Party Packet.....  */

		SetByte( send_buff, WIZ_MAGIC_PROCESS, send_index );
		SetByte( send_buff, MAGIC_TYPE4_END, send_index );	
		SetByte( send_buff, buff_type, send_index ); 
		Send( send_buff, send_index ); 
	}

	int buff_test = 0;
	for (int i = 0 ; i < MAX_TYPE4_BUFF ; i++) {
		buff_test += m_bType4Buff[i];
	}
	if (buff_test == 0) m_bType4Flag = FALSE;

	BOOL bType4Test = TRUE ;
	for (int j = 0 ; j < MAX_TYPE4_BUFF ; j++) {
		if (m_bType4Buff[j] == 1) {
			bType4Test = FALSE;
			break;
		}
	}
//
	// Send Party Packet.....
	if (m_sPartyIndex != -1 && bType4Test) {
		memset( send_buff, NULL, 128); send_index = 0 ;
		SetByte( send_buff, WIZ_PARTY, send_index );
		SetByte( send_buff, PARTY_STATUSCHANGE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetByte( send_buff, 2, send_index );
		SetByte( send_buff, 0x00, send_index);
		m_pMain->Send_PartyMember(m_sPartyIndex, send_buff, send_index);
	}
	//  end of Send Party Packet.....  //
//
}

BYTE CUser::ItemCountChange(int itemid, int type, int amount)	// 0 : Requested item not available.
{																// 1 : Amount is greater than current item count.
	int send_index = 0, result = 0, slot = -1 ;					// 2 : Success. Current item count updated or deleted.
	char send_buff[128]; memset( send_buff, 0x00, 128 ) ;

	_ITEM_TABLE* pTable = NULL;				// This checks if such an item exists.
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) {
		result = 0;
		return result;	
	}

	for ( int i = SLOT_MAX * type ; i < SLOT_MAX + HAVE_MAX * type ; i++ ) {
		if( m_pUserData->m_sItemArray[i].nNum == itemid ) {		
// 
			if (pTable->m_bReqDex > (m_pUserData->m_bDex + m_sItemDex + m_bDexAmount) && pTable->m_bReqDex != 0) {
				return result;
			}

			if (pTable->m_bReqStr > (m_pUserData->m_bStr + m_sItemStr + m_bStrAmount) && pTable->m_bReqStr != 0) {
				return result;
			}

			if (pTable->m_bReqSta > (m_pUserData->m_bSta + m_sItemSta + m_bStaAmount) && pTable->m_bReqSta != 0) {
				return result;
			}

			if (pTable->m_bReqIntel > (m_pUserData->m_bIntel + m_sItemIntel + m_bIntelAmount) && pTable->m_bReqIntel != 0) {
				return result;
			}

			if (pTable->m_bReqCha > (m_pUserData->m_bCha + m_sItemCham + m_bChaAmount) && pTable->m_bReqCha !=0) {
				return result;
			}
//
			if (!pTable->m_bCountable) {	// This checks if the user actually has that item.
				result = 2;
				return result;
			}

			slot = i ;
			m_pUserData->m_sItemArray[i].sCount -= amount ;

			if (m_pUserData->m_sItemArray[i].sCount == 0) {
				m_pUserData->m_sItemArray[i].nNum = 0 ;
				result = 2 ;	// You have just ran out of items.
				break;
			}
			else if (m_pUserData->m_sItemArray[i].sCount < 0) {
				m_pUserData->m_sItemArray[i].sCount += amount ;
				result = 1 ;	// Insufficient number of items.
				break;
			}

			result = 2 ;		// No error detected....
		}		
	}

	if ( result < 2 ) return result ;	// Something happened :(

	SendItemWeight();

	SetByte( send_buff, WIZ_ITEM_COUNT_CHANGE, send_index );	
	SetShort( send_buff, 1, send_index );	// The number of for-loops
	SetByte( send_buff, type, send_index );
	SetByte( send_buff, slot - type * SLOT_MAX , send_index );
	SetDWORD( send_buff, itemid, send_index );	// The ID of item.
	SetDWORD( send_buff, m_pUserData->m_sItemArray[slot].sCount, send_index );

	Send( send_buff, send_index );

	return result ;		// Success :) 
}

void CUser::SendAllKnightsID()
{
	int send_index = 0, count = 0, buff_index = 0;
	char send_buff[40960]; memset( send_buff, 0x00, 40960 );
	char temp_buff[40960]; memset( temp_buff, 0x00, 40960 );
	map < int, CKnights*>::iterator Iter1, Iter2;
	CKnights* pKnights = NULL;

	Iter1 = m_pMain->m_KnightsArray.m_UserTypeMap.begin();
	Iter2 = m_pMain->m_KnightsArray.m_UserTypeMap.end();
	
	for( ; Iter1 != Iter2; Iter1++ ) {
		pKnights = (*Iter1).second;
		if( !pKnights ) continue;
		//if( pKnights->bFlag != KNIGHTS_TYPE ) continue;
		SetShort( temp_buff, pKnights->m_sIndex, buff_index );
		SetShort( temp_buff, strlen(pKnights->m_strName), buff_index );
		SetString( temp_buff, pKnights->m_strName, strlen(pKnights->m_strName), buff_index );
		count++;
	}

	SetByte( send_buff, WIZ_KNIGHTS_LIST, send_index );
	SetByte( send_buff, 0x01, send_index );					// All List 
	SetShort( send_buff, count, send_index );
	SetString( send_buff, temp_buff, buff_index, send_index );
//	SendCompressingPacket( send_buff, send_index );
	Send( send_buff, send_index );
}

void CUser::ItemRemove(char *pBuf)
{
	int index = 0, send_index = 0, slot = 0, pos = 0, itemid = 0, count = 0, durability = 0;
	__int64 serial = 0;
	char send_buff[128]; memset( send_buff, 0x00, send_index );

	slot = GetByte( pBuf, index );
	pos = GetByte( pBuf, index );
	itemid = GetDWORD( pBuf, index );

	if( slot == 1 ) {
		if( pos > SLOT_MAX ) goto fail_return;
		if( m_pUserData->m_sItemArray[pos].nNum != itemid ) goto fail_return;
		count = m_pUserData->m_sItemArray[pos].sCount;
		durability = m_pUserData->m_sItemArray[pos].sDuration;
		serial = m_pUserData->m_sItemArray[pos].nSerialNum;
		m_pUserData->m_sItemArray[pos].nNum = 0;
		m_pUserData->m_sItemArray[pos].sCount = 0;
		m_pUserData->m_sItemArray[pos].sDuration = 0;
		m_pUserData->m_sItemArray[pos].nSerialNum = 0;
	}
	else {
		if( pos > HAVE_MAX ) goto fail_return;
		if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != itemid ) goto fail_return;
		count = m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount;
		durability = m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration;
		serial = m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = 0;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 0;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = 0;
		m_pUserData->m_sItemArray[SLOT_MAX+pos].nSerialNum = 0;
	}

	SendItemWeight();

	SetByte( send_buff, WIZ_ITEM_REMOVE, send_index );
	SetByte( send_buff, 0x01, send_index );
	Send( send_buff, send_index );

	ItemLogToAgent( m_pUserData->m_id, "DESTROY", ITEM_DESTROY, serial, itemid, count, durability );
	return;
fail_return:
	SetByte( send_buff, WIZ_ITEM_REMOVE, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::OperatorCommand(char *pBuf)
{
	int index = 0, idlen = 0;
	char userid[MAX_ID_SIZE+1]; memset( userid, 0x00, MAX_ID_SIZE+1 );
	CUser* pUser = NULL;

	if( m_pUserData->m_bAuthority != 0 ) return;	// Is this user`s authority operator?
	
	BYTE command = GetByte( pBuf, index );
	idlen = GetShort( pBuf, index );
	if( idlen < 0 || idlen > MAX_ID_SIZE ) return;
	GetString( userid, pBuf, idlen, index );

	pUser = m_pMain->GetUserPtr( userid, 0x02 );
	if( !pUser ) return;

/* COMMANDS
SEND-->WIZ_OPERATOR-->40 + 01 + 0200 + 6171 ; /arrest
SEND-->WIZ_OPERATOR-->40 + 07 + 0200 + 6171 ; /forbiduser
SEND-->WIZ_OPERATOR-->40 + 08 + 0200 + 6171 ; /attackdisable
SEND-->WIZ_OPERATOR-->40 + 09 + 0200 + 6171 ; /attackenable
SEND-->WIZ_UNKNOW (7A)-->7A + 01 + 0200 + 6171 ; /plc
SEND-->WIZ_UNKNOW (7C)-->7C + 03 + 02 + 6171 ; /goout
*/
	switch( command ) {
		case OPERATOR_ARREST:
			ZoneChange( pUser->m_pUserData->m_bZone, pUser->m_pUserData->m_curx, pUser->m_pUserData->m_curz );
			break;
		case OPERATOR_KILL:
			pUser->m_pUserData->m_bAuthority = 255;
			pUser->Close();
			break;
		case OPERATOR_NOTCHAT:
			pUser->m_pUserData->m_bAuthority = 2;
			break;
		case OPERATOR_CHAT:
			pUser->m_pUserData->m_bAuthority = 1;
			break;
	}
}

void CUser::SpeedHackTime(char* pBuf)
{
	BYTE b_first = 0x00;
	int index = 0;
	float servertime = 0.0f, clienttime = 0.0f, client_gap = 0.0f, server_gap = 0.0f;

	b_first = GetByte( pBuf, index );
	clienttime = Getfloat( pBuf, index );

	if( b_first ) {
		m_fSpeedHackClientTime = clienttime;
		m_fSpeedHackServerTime = TimeGet();
	}
	else {
		servertime = TimeGet();

		server_gap = servertime - m_fSpeedHackServerTime;
		client_gap = clienttime - m_fSpeedHackClientTime;

		if( client_gap - server_gap > 10.0f ) {
			char logstr[256];
			memset( logstr, NULL, 256 );
			sprintf( logstr, "%s SpeedHack User Checked By Server Time\r\n", m_pUserData->m_id);
			LogFileSpeedHack( logstr );

			Close();
		}
		else if( client_gap - server_gap < 0.0f ) {
			m_fSpeedHackClientTime = clienttime;
			m_fSpeedHackServerTime = TimeGet();
		}
	}

/*	float currenttime;
	if( m_fSpeedHackTime == 0.0f )
		m_fSpeedHackTime = TimeGet();
	else {
		currenttime = TimeGet();
		if( (currenttime - m_fSpeedHackTime) < 48.0f ) { 
			char logstr[256];
			memset( logstr, NULL, 256 );
			sprintf( logstr, "%s SpeedHack User Checked By Server Time\r\n", m_pUserData->m_id);
			LogFileWrite( logstr );
			
//			if( m_pUserData->m_bAuthority != 0 )
//				m_pUserData->m_bAuthority = -1;

			Close();
		}
	}

	m_fSpeedHackTime = TimeGet();
*/
}

// serverÀÇ »óÅÂ¸¦ Ã¼Å©..
void CUser::ServerStatusCheck()
{
	int send_index = 0;
	char send_buff[256];		memset(send_buff, 0x00, 256);
	SetByte(send_buff, WIZ_SERVER_CHECK, send_index );
	SetShort(send_buff, m_pMain->m_sErrorSocketCount, send_index);
	Send( send_buff, send_index );
}

void CUser::Type3AreaDuration(float currenttime)
{
	int send_index = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );

	CMagicProcess	magic_process;

	_MAGIC_TYPE3* pType = NULL;
	pType = m_pMain->m_Magictype3Array.GetData(m_iAreaMagicID);      // Get magic skill table type 3.
	if( !pType ) return;

	if ( m_fAreaLastTime != 0.0f && (currenttime - m_fAreaLastTime) > m_bAreaInterval) {    // Did one second pass?
		m_fAreaLastTime = currenttime ;
		if( m_bResHpType == USER_DEAD ) return;
		
		for (int i = 0 ; i < MAX_USER ; i++) {	// Actual damage procedure.
			if (magic_process.UserRegionCheck(m_Sid, i, m_iAreaMagicID, pType->bRadius)) {	// Region check.
				CUser* pTUser = NULL ;     		
				pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];     
				if (!pTUser) continue;

				SetByte( send_buff, WIZ_MAGIC_PROCESS, send_index );	// Set packet.
				SetByte( send_buff, MAGIC_EFFECTING, send_index );
				SetDWORD( send_buff, m_iAreaMagicID, send_index );
				SetShort( send_buff, m_Sid, send_index );
				SetShort( send_buff, i, send_index );
				SetShort( send_buff, 0, send_index );	
				SetShort( send_buff, 0, send_index );	
				SetShort( send_buff, 0, send_index );	
				m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );
			}
		}

		if ( (( currenttime - m_fAreaStartTime) >= pType->sDuration) || m_bResHpType == USER_DEAD) { // Did area duration end? 			
			m_bAreaInterval = 5;
			m_fAreaStartTime = 0.0f;
			m_fAreaLastTime = 0.0f;
			m_iAreaMagicID = 0;
		}
	}	


	SetByte( send_buff, WIZ_MAGIC_PROCESS, send_index );	// Set packet.
	SetByte( send_buff, MAGIC_EFFECTING, send_index );
	SetDWORD( send_buff, m_iAreaMagicID, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, 0, send_index );	
	SetShort( send_buff, 0, send_index );	
	SetShort( send_buff, 0, send_index );	
	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );	// Send packet to region.
}

void CUser::WarehouseProcess(char *pBuf)
{
	int index = 0, send_index = 0, itemid = 0, srcpos = -1, destpos = -1, page = -1, reference_pos = -1, count = 0;
	char send_buff[20480]; memset( send_buff, 0x00, 20480 );
	_ITEM_TABLE* pTable = NULL;
	BYTE command = 0;
	command = GetByte( pBuf, index );

	// Ã¢°í ¾ÈµÇ°Ô...
	if( m_bResHpType == USER_DEAD || m_pUserData->m_sHp == 0 )	{
		printf("### WarehouseProcess Fail : name=%s(%d), m_bResHpType=%d, hp=%d, x=%d, z=%d ###\n", m_pUserData->m_id, m_Sid, m_bResHpType, m_pUserData->m_sHp, (int)m_pUserData->m_curx, (int)m_pUserData->m_curz);
		return;
	}
	if( m_sExchangeUser != -1 ) goto fail_return;

	if( command == WAREHOUSE_OPEN )	{
		SetByte( send_buff, WIZ_WAREHOUSE, send_index );
		SetByte( send_buff, WAREHOUSE_OPEN, send_index );
		SetDWORD( send_buff, m_pUserData->m_iBank, send_index );
/*
		for(int i=0; i<WAREHOUSE_MAX; i++ ) {
			SetDWORD( send_buff, m_pUserData->m_sWarehouseArray[i].nNum, send_index );
			SetShort( send_buff, m_pUserData->m_sWarehouseArray[i].sDuration, send_index );
			SetShort( send_buff, m_pUserData->m_sWarehouseArray[i].sCount, send_index );
		}
*/
		for(int i=0; i<WAREHOUSE_MAX; i++ ) {
			SetDWORD( send_buff, m_pUserData->m_sWarehouseArray[i].nNum, send_index );
			SetShort( send_buff, m_pUserData->m_sWarehouseArray[i].sDuration, send_index );
			SetShort( send_buff, m_pUserData->m_sWarehouseArray[i].sCount, send_index );
			// Byte 4 Seal Item, 0 Normal Item || Short Rental minutes || DWORD 0x00 || DWORD Expiration Time
			SetDWORD( send_buff, m_pUserData->m_sWarehouseArray[i].nSerialNum, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
			SetByte( send_buff, 0x00, send_index );
		}

//		SendCompressingPacket( send_buff, send_index );
		Send( send_buff, send_index );	
		return;
	}

	itemid = GetDWORD( pBuf, index );
	page = GetByte( pBuf, index );
	srcpos = GetByte( pBuf, index );
	destpos = GetByte( pBuf, index );
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) goto fail_return;
	reference_pos = 24 * page;

	switch( command ) {
	case WAREHOUSE_INPUT:
		count = GetDWORD( pBuf, index );
		if( itemid == ITEM_GOLD ) {
			if( m_pUserData->m_iBank+count > 2100000000 ) goto fail_return;
			if( m_pUserData->m_iGold-count < 0 ) goto fail_return;
			m_pUserData->m_iBank += count;
			m_pUserData->m_iGold -= count;
			break;
		}
		if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum != itemid ) goto fail_return;
		if( reference_pos+destpos > WAREHOUSE_MAX ) goto fail_return;
		if( m_pUserData->m_sWarehouseArray[reference_pos+destpos].nNum && !pTable->m_bCountable ) goto fail_return;
		if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount < count ) goto fail_return;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].nNum = itemid;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
		if( pTable->m_bCountable == 0 && m_pUserData->m_sWarehouseArray[reference_pos+destpos].nSerialNum == 0 )
			m_pUserData->m_sWarehouseArray[reference_pos+destpos].nSerialNum = m_pMain->GenerateItemSerial();

		if( pTable->m_bCountable ) {
			m_pUserData->m_sWarehouseArray[reference_pos+destpos].sCount += count;
		}
		else {
			m_pUserData->m_sWarehouseArray[reference_pos+destpos].sCount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
		}

		if( !pTable->m_bCountable ) {
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = 0;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = 0;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = 0;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = 0;
		}
		else {
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount -= count;
			if( m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount <= 0 ) {
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = 0;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = 0;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = 0;
				m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = 0;
			}
		}

		SendItemWeight();
		ItemLogToAgent( m_pUserData->m_Accountid, m_pUserData->m_id, ITEM_WAREHOUSE_PUT, 0, itemid, count, m_pUserData->m_sWarehouseArray[reference_pos+destpos].sDuration );
		break;
	case WAREHOUSE_OUTPUT:
		count = GetDWORD( pBuf, index );

		if( itemid == ITEM_GOLD ) {
			if( m_pUserData->m_iGold+count > 2100000000 ) goto fail_return;
			if( m_pUserData->m_iBank-count < 0 ) goto fail_return;
			m_pUserData->m_iGold += count;
			m_pUserData->m_iBank -= count;
			break;
		}
//
		if (pTable->m_bCountable) {	// Check weight of countable item.
			if (((pTable->m_sWeight * count)   + m_sItemWeight) > m_sMaxWeight) {			
				goto fail_return;
			}
		}
		else {	// Check weight of non-countable item.
			if ((pTable->m_sWeight + m_sItemWeight) > m_sMaxWeight) {
				goto fail_return;
			}
		}		
//
		if( reference_pos+srcpos > WAREHOUSE_MAX ) goto fail_return;
		if( m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nNum != itemid ) goto fail_return;
		if( m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum && !pTable->m_bCountable ) goto fail_return;
		if( m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount < count ) goto fail_return;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum = itemid;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sDuration;
		m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nSerialNum;
		if( pTable->m_bCountable )
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount += count;
		else {
			if( m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum == 0 )
				m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = m_pMain->GenerateItemSerial();
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount;
		}
		if( !pTable->m_bCountable ) {
			m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nNum = 0;
			m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sDuration = 0;
			m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount = 0;
			m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nSerialNum = 0;
		}
		else {
			m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount -= count;
			if( m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount <= 0 ) {
				m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nNum = 0;
				m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sDuration = 0;
				m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount = 0;
				m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nSerialNum = 0;
			}
		}

		SendItemWeight();		
		ItemLogToAgent( m_pUserData->m_id, m_pUserData->m_Accountid, ITEM_WAREHOUSE_GET, 0, itemid, count, m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration );
		//printf("WARE OUTPUT : %s %s %d %d %d %d %d", m_pUserData->m_id, m_pUserData->m_Accountid, ITEM_WAREHOUSE_GET, 0, itemid, count, m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration );
		break;
	case WAREHOUSE_MOVE:
		if( reference_pos+srcpos > WAREHOUSE_MAX ) goto fail_return;
		if( m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nNum != itemid ) goto fail_return;
		if( m_pUserData->m_sWarehouseArray[reference_pos+destpos].nNum ) goto fail_return;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].nNum = itemid;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].sDuration = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sDuration;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].sCount = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount;
		m_pUserData->m_sWarehouseArray[reference_pos+destpos].nSerialNum = m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nSerialNum;

		m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nNum = 0;
		m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sDuration = 0;
		m_pUserData->m_sWarehouseArray[reference_pos+srcpos].sCount = 0;
		m_pUserData->m_sWarehouseArray[reference_pos+srcpos].nSerialNum = 0;
		break;
	case WAREHOUSE_INVENMOVE:
		if( itemid != m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum )
			goto fail_return;
		{
			short duration = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration;
			short itemcount = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount;
			__int64 serial = m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nNum = m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sDuration = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].sCount = m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount;
			m_pUserData->m_sItemArray[SLOT_MAX+srcpos].nSerialNum = m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum;

			m_pUserData->m_sItemArray[SLOT_MAX+destpos].nNum = itemid;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sDuration = duration;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].sCount = itemcount;
			m_pUserData->m_sItemArray[SLOT_MAX+destpos].nSerialNum = serial;
		}
		break;
	}

	m_pUserData->m_bWarehouse = 1;

	SetByte( send_buff, WIZ_WAREHOUSE, send_index );
	SetByte( send_buff, command, send_index );
	SetByte( send_buff, 0x01, send_index );
	Send( send_buff, send_index );
	return;
fail_return:
	SetByte( send_buff, WIZ_WAREHOUSE, send_index );
	SetByte( send_buff, command, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::InitType4()
{
	m_bAttackSpeedAmount = 100;		// this is for the duration spells Type 4
    m_bSpeedAmount = 100;
    m_sACAmount = 0;
    m_bAttackAmount = 100;
	m_sMaxHPAmount = 0;
	m_bHitRateAmount = 100;
	m_sAvoidRateAmount = 100;
	m_bStrAmount = 0;
	m_bStaAmount = 0;
	m_bDexAmount = 0;
	m_bIntelAmount = 0;
	m_bChaAmount = 0;
	m_bFireRAmount = 0;
	m_bColdRAmount = 0;
	m_bLightningRAmount = 0;
	m_bMagicRAmount = 0;
	m_bDiseaseRAmount = 0;
	m_bPoisonRAmount = 0;		
// ºñ·¯¸Ó±Û ¼ö´É
	m_bAbnormalType = 1;
//
	m_sDuration1 = 0 ;  m_fStartTime1 = 0.0f ;		// Used for Type 4 Durational Spells.
	m_sDuration2 = 0 ;  m_fStartTime2 = 0.0f ;
	m_sDuration3 = 0 ;  m_fStartTime3 = 0.0f ;
	m_sDuration4 = 0 ;  m_fStartTime4 = 0.0f ;
	m_sDuration5 = 0 ;  m_fStartTime5 = 0.0f ;
	m_sDuration6 = 0 ;  m_fStartTime6 = 0.0f ;
	m_sDuration7 = 0 ;  m_fStartTime7 = 0.0f ;
	m_sDuration8 = 0 ;  m_fStartTime8 = 0.0f ;
	m_sDuration9 = 0 ;  m_fStartTime9 = 0.0f ;

	for (int h = 0 ; h < MAX_TYPE4_BUFF ; h++) {
		m_bType4Buff[h] = 0 ;
	}

	m_bType4Flag = FALSE;
}

int CUser::GetEmptySlot(int itemid, int bCountable)	// item ¸ÔÀ»¶§ ºñ¾îÀÕ´Â ½½·ÔÀ» Ã£¾Æ¾ßµÇ...
{
	int pos = 255, i=0;
	
	_ITEM_TABLE* pTable = NULL;

	if (bCountable == -1) {
		pTable = m_pMain->m_ItemtableArray.GetData( itemid );
		if (!pTable) return pos;
		
		bCountable = pTable->m_bCountable;
	}

	if( itemid == ITEM_GOLD ) return pos;
	for(i=0; i<HAVE_MAX; i++) {
		if( m_pUserData->m_sItemArray[SLOT_MAX+i].nNum != 0 )
			continue;
		else {
			pos = i;
			break;
		}
	}
	if( bCountable ){
		for(i=0; i<HAVE_MAX; i++) {
			if( m_pUserData->m_sItemArray[SLOT_MAX+i].nNum == itemid )
				return i;
		}
		if( i == HAVE_MAX )
			return pos;
	}
	return pos;
}

void CUser::ReportBug(char *pBuf)
{		
	Beep(3000, 200);	// Let's hear a beep from the speaker.

	int index = 0, chatlen = 0, send_index = 0;
	char chatstr[1024]; memset( chatstr, NULL, 1024 );
	char logstr[1024]; memset( logstr, NULL, 1024 );

	chatlen = GetShort( pBuf, index );
	if( chatlen > 512 || chatlen <= 0 ) return;
	GetString( chatstr, pBuf, chatlen, index );

//	printf( " Short : %d   String : %s  \n ", chatlen, chatstr);
	if( strlen(m_pUserData->m_id) == 0 ) return;	
	sprintf_s( logstr, "%s -> ERROR : %s\r\n", m_pUserData->m_id, chatstr);
	LogFileReportBug( logstr );
}
/*
void CUser::Home()
{
	int send_index = 0 ;    
	char send_buff[128] ;
	memset( send_buff, NULL, 128) ;
	
	short x = 0, z = 0 ;		// The point where you will be warped to.

	_HOME_INFO* pHomeInfo = NULL;
	pHomeInfo = m_pMain->m_HomeArray.GetData(m_pUserData->m_bNation);
	if (!pHomeInfo) return;

	if( m_pUserData->m_bNation != m_pUserData->m_bZone && m_pUserData->m_bZone > 200) {		// Frontier Zone...
		x = pHomeInfo->FreeZoneX + myrand(0, pHomeInfo->FreeZoneLX);
		z = pHomeInfo->FreeZoneZ + myrand(0, pHomeInfo->FreeZoneLZ);
	}
//
	else if( m_pUserData->m_bNation != m_pUserData->m_bZone && m_pUserData->m_bZone > 100 && m_pUserData->m_bZone < 200) {	// Battle Zone...
		x = pHomeInfo->BattleZoneX + myrand(0, pHomeInfo->BattleZoneLX);
		z = pHomeInfo->BattleZoneZ + myrand(0, pHomeInfo->BattleZoneLZ);

// ºñ·¯¸Ó±Û °³Ã´Áö´ë ¹Ù²ãÄ¡±â --;
		if (m_pUserData->m_bZone == ZONE_SNOW_BATTLE) {
			x = pHomeInfo->FreeZoneX + myrand(0, pHomeInfo->FreeZoneLX);
			z = pHomeInfo->FreeZoneZ + myrand(0, pHomeInfo->FreeZoneLZ);
		}
//

/*
		KickOutZoneUser();
		return;
*/
/*
	}
//
	else if (m_pUserData->m_bNation != m_pUserData->m_bZone && m_pUserData->m_bZone < 3) {	// Specific Lands...
		if (m_pUserData->m_bNation == KARUS) {
			x = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
			z = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ);			
		}
		else if (m_pUserData->m_bNation == ELMORAD) {
			x = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
			z = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ);	
		}		
		else return;
	}
// ºñ·¯¸Ó±Û ´ºÁ¸ >.<
	else if (m_pUserData->m_bZone > 10 && m_pUserData->m_bZone < 20) {
		x = 527 + myrand(0, 10);
		z = 543 + myrand(0, 10);			
	}
//
	else {	// Your own nation...
		if (m_pUserData->m_bNation == KARUS) {
			x = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
			z = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ);			
		}
		else if (m_pUserData->m_bNation == ELMORAD) {			
			x = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
			z = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ);
		}		
		else return;
	}
	
	if ( m_pUserData->m_bZone == 21 ) { // Moradon
		x = 817;
		z = 432; // Town
	}

	SetShort(send_buff, (WORD)(x * 10), send_index);	
	SetShort(send_buff, (WORD)(z * 10), send_index);
	Warp(send_buff);
}
*/
void CUser::Home()
{
	// The point where you will be warped to.
	short x = 0, z = 0;

	// Forgotten Temple
	if (getZoneID() == 55)
	{
		KickOutZoneUser(TRUE);
		return;
	}
	// Prevent /town'ing in quest arenas
	else if ((getZoneID() / 10) == 5
		|| !GetStartPosition(x, z))
		return;

	char send_buff[4]; int send_index = 0;
	SetShort(send_buff, (WORD)(x * 10), send_index);
	SetShort(send_buff, (WORD)(z * 10), send_index);
	Warp(send_buff);
}
bool CUser::GetStartPosition(short& x, short& z, BYTE bZone /*= 0 */)
{
	// Get start position data for current zone (unless we specified a zone).
	int nZoneID = (bZone == 0 ? getZoneID() : bZone);
	_START_POSITION* pData = m_pMain->GetStartPosition(nZoneID);
	if (pData == NULL)
		return false;

	// TO-DO: Allow for Delos/CSW.

	// NOTE: This is how mgame does it.
	// This only allows for positive randomisation; we should really allow for the full range...
	if (getNation() == KARUS)
	{
		x = pData->sKarusX + myrand(0, pData->bRangeX);
		z = pData->sKarusZ + myrand(0, pData->bRangeZ);
	}
	else
	{
		x = pData->sElmoradX + myrand(0, pData->bRangeX);
		z = pData->sElmoradZ + myrand(0, pData->bRangeZ);
	}

	return true;
}

CUser* CUser::GetItemRoutingUser(int itemid, short itemcount)
{
	if( m_sPartyIndex == -1 ) return NULL;

	CUser* pUser = NULL;
	_PARTY_GROUP* pParty = NULL;
	int select_user = -1, count = 0;
 
	pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
	if( !pParty ) return NULL;
	if(	pParty->bItemRouting > 7 ) return NULL;
//
	_ITEM_TABLE* pTable = NULL;
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) return NULL;
//
	while(count<8) {
		select_user = pParty->uid[pParty->bItemRouting];
		if( select_user >= 0 && select_user < MAX_USER ) {
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[select_user];
			if( pUser ) {
//	ÀÌ°Å ¾ÊµÇµµ Àú¸¦ ³Ê¹« ¹Ì¿öÇÏÁö ¸¶¼¼¿ä ¤Ğ.¤Ğ
				if (pTable->m_bCountable) {	// Check weight of countable item.
					if ((pTable->m_sWeight * count + pUser->m_sItemWeight) <= pUser->m_sMaxWeight) {			
						pParty->bItemRouting++;
						if( pParty->bItemRouting > 6 )
							pParty->bItemRouting = 0;
						return pUser;	// Áï, À¯ÀúÀÇ Æ÷ÀÎÅÍ¸¦ ¸®ÅÏÇÑ´Ù :)
					}
				}
				else {	// Check weight of non-countable item.
					if ((pTable->m_sWeight + pUser->m_sItemWeight) <= pUser->m_sMaxWeight) {
						pParty->bItemRouting++;
						if( pParty->bItemRouting > 6 )
							pParty->bItemRouting = 0;
						return pUser;	// Áï, À¯ÀúÀÇ Æ÷ÀÎÅÍ¸¦ ¸®ÅÏÇÑ´Ù :)
					}
				}
//

/*
				pParty->bItemRouting++;
				if( pParty->bItemRouting > 6 )
					pParty->bItemRouting = 0;
				return pUser;	// Áï, À¯ÀúÀÇ Æ÷ÀÎÅÍ¸¦ ¸®ÅÏÇÑ´Ù :)
*/
			}

		}
		if( pParty->bItemRouting > 6 )
			pParty->bItemRouting = 0;
		else
			pParty->bItemRouting++;
		count++;
	}

	return NULL;
}

void CUser::FriendReport(char *pBuf)
{
	int index = 0; short usercount = 0, idlen = 0;		// Basic Initializations.
	int send_index = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256);
	char userid[MAX_ID_SIZE+1];
	memset( userid, NULL, MAX_ID_SIZE+1 );
	CUser* pUser = NULL;

	usercount = GetShort( pBuf, index );	// Get usercount packet.
	if( usercount >= 30 || usercount < 0) return;
	
	SetByte( send_buff, WIZ_FRIEND_REPORT, send_index );
	SetShort( send_buff, usercount, send_index);

	for (int k = 0 ; k < usercount ; k++) {
		idlen = GetShort( pBuf, index );
		if( idlen > MAX_ID_SIZE ) {
			SetShort(send_buff, strlen(userid), send_index);
			SetString( send_buff, userid, strlen(userid), send_index );
			SetShort(send_buff, -1, send_index);
			SetByte( send_buff, 0, send_index);
			continue;
		}
		GetString( userid, pBuf, idlen, index );

		pUser = m_pMain->GetUserPtr( userid, 0x02 );

		SetShort(send_buff, idlen, send_index);
		SetString( send_buff, userid, idlen, send_index );

		if (!pUser) { // No such user
			SetShort(send_buff, -1, send_index);
			SetByte( send_buff, 0, send_index);
		}
		else {
			SetShort(send_buff, pUser->m_Sid, send_index);
			if (pUser->m_sPartyIndex >=0) {
				SetByte( send_buff, 3, send_index);
			}
			else {
				SetByte( send_buff, 1, send_index);
			}
		}
	}

	Send( send_buff, send_index );
}

void CUser::ClassChangeReq()
{
	char send_buff[128]; memset( send_buff, NULL, 128 );
	int send_index = 0;

	SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
	SetByte( send_buff, CLASS_CHANGE_RESULT, send_index );
	if( m_pUserData->m_bLevel < 10 )
		SetByte( send_buff, 0x02, send_index );
	else if ( (m_pUserData->m_sClass % 100) > 4 )
		SetByte( send_buff, 0x03, send_index );
	else
		SetByte( send_buff, 0x01, send_index );
	Send( send_buff, send_index );
}

void CUser::AllSkillPointChange()
{
	// µ·À» ¸ÕÀú ±ï°í.. ¸¸¾à,, µ·ÀÌ ºÎÁ·ÇÏ¸é.. ¿¡·¯...
	int index = 0, send_index = 0, skill_point = 0, money = 0, i=0, j=0, temp_value = 0, old_money = 0;
	BYTE type = 0x00;    // 0:µ·ÀÌ ºÎÁ·, 1:¼º°ø, 2:ÃÊ±âÈ­ÇÒ ½ºÅ³ÀÌ ¾øÀ»¶§..
	char send_buff[128]; memset( send_buff, NULL, 128 );

	temp_value = pow(( m_pUserData->m_bLevel * 2 ), 3.4);
	temp_value = ( temp_value / 100 )*100;
	if( m_pUserData->m_bLevel < 30)		temp_value = temp_value * 0.4;
	else if( m_pUserData->m_bLevel >= 30 && m_pUserData->m_bLevel < 60 ) temp_value = temp_value * 1;
	else if( m_pUserData->m_bLevel >= 60 && m_pUserData->m_bLevel <= 90 ) temp_value = temp_value * 1.5;

	// ½ºÅ³Àº ÇÑ¹ø ´õ 
	temp_value = temp_value * 1.5;

	if( m_pMain->m_sDiscount == 1 && m_pMain->m_byOldVictory == m_pUserData->m_bNation )		{	// ÇÒÀÎ½ÃÁ¡ÀÌ°í ½Â¸®±¹ÀÌ¶ó¸é
		old_money = temp_value;
		temp_value = temp_value * 0.5;
		//printf("^^ AllSkillPointChange - Discount ,, money=%d->%d\n", old_money, temp_value);
	}

	if( m_pMain->m_sDiscount == 2  )		{	
		old_money = temp_value;
		temp_value = temp_value * 0.5;
		//printf("^^ AllSkillPointChange - Discount ,, money=%d->%d\n", old_money, temp_value);
	}

	money = m_pUserData->m_iGold - temp_value;
	//money = m_pUserData->m_iGold - 100;

	if(money < 0)	goto fail_return;
	if( m_pUserData->m_bLevel < 10 )	goto fail_return;

	for(i=1; i<9; i++)	{
		skill_point += m_pUserData->m_bstrSkill[i];
	}

	if(skill_point <= 0) {
		type = 2;
		goto fail_return;
	}

	// ¹®Á¦µÉ ¼ÒÁö°¡ ¸¹À½ : °¡¿ë½ºÅ³ÀÌ 255À» ³Ñ´Â »óÈ²ÀÌ ¹ß»ıÇÒ È®À²ÀÌ ³ôÀ½..
	//m_pUserData->m_bstrSkill[0] += skill_point;		
	m_pUserData->m_bstrSkill[0] = (m_pUserData->m_bLevel - 9) * 2;
	for(j=1; j<9; j++)	
		m_pUserData->m_bstrSkill[j] = 0;
	m_pUserData->m_iGold = money;
	type = 1;

	SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
	SetByte( send_buff, ALL_SKILLPT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	SetByte( send_buff, m_pUserData->m_bstrSkill[0], send_index );
	Send( send_buff, send_index );

	return;

fail_return:
	SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
	SetByte( send_buff, ALL_SKILLPT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	SetDWORD( send_buff, temp_value, send_index );
	Send( send_buff, send_index );
}

void CUser::AllPointChange()
{
	// µ·À» ¸ÕÀú ±ï°í.. ¸¸¾à,, µ·ÀÌ ºÎÁ·ÇÏ¸é.. ¿¡·¯...
	int index = 0, send_index = 0, total_point = 0, money = 0, classcode=0, temp_money = 0, old_money=0;
	double dwMoney = 0;
	BYTE type = 0x00;
	char send_buff[128];
	memset( send_buff, NULL, 128 );
	int i = 0 ;

	if( m_pUserData->m_bLevel > 80 ) goto fail_return;

	temp_money = pow(( m_pUserData->m_bLevel * 2 ), 3.4);
	temp_money = (temp_money/100)*100;
	if( m_pUserData->m_bLevel < 30)		temp_money = temp_money * 0.4;
	else if( m_pUserData->m_bLevel >= 30 && m_pUserData->m_bLevel < 60 ) temp_money = temp_money * 1;
	else if( m_pUserData->m_bLevel >= 60 && m_pUserData->m_bLevel <= 90 ) temp_money = temp_money * 1.5;

	if( m_pMain->m_sDiscount == 1 && m_pMain->m_byOldVictory == m_pUserData->m_bNation )		{	// ÇÒÀÎ½ÃÁ¡ÀÌ°í ½Â¸®±¹ÀÌ¶ó¸é
		temp_money = temp_money * 0.5;
		//printf("^^ AllPointChange - Discount ,, money=%d->%d\n", old_money, temp_money);
	}

	if( m_pMain->m_sDiscount == 2  )		{	
		temp_money = temp_money * 0.5;
	}

	money = m_pUserData->m_iGold - temp_money;
	if(money < 0)	goto fail_return;

	// ÀåÂø¾ÆÀÌÅÛÀÌ ÇÏ³ª¶óµµ ÀÖÀ¸¸é ¿¡·¯Ã³¸® 
	for (i = 0 ; i < SLOT_MAX ; i++) {
		if (m_pUserData->m_sItemArray[i].nNum) {
			type = 0x04;
			goto fail_return;
		}
	}
	
	switch( m_pUserData->m_bRace ) {
	case KARUS_BIG:	
		if( m_pUserData->m_bStr == 65 && m_pUserData->m_bSta == 65 && m_pUserData->m_bDex == 60 && m_pUserData->m_bIntel == 50 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 65;
		m_pUserData->m_bSta = 65;
		m_pUserData->m_bDex = 60;
		m_pUserData->m_bIntel = 50;
		m_pUserData->m_bCha = 50;
		break;
	case KARUS_MIDDLE:
		if( m_pUserData->m_bStr == 65 && m_pUserData->m_bSta == 65 && m_pUserData->m_bDex == 60 && m_pUserData->m_bIntel == 50 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 65;
		m_pUserData->m_bSta = 65;
		m_pUserData->m_bDex = 60;
		m_pUserData->m_bIntel = 50;
		m_pUserData->m_bCha = 50;
		break;
	case KARUS_SMALL:
		if( m_pUserData->m_bStr == 50 && m_pUserData->m_bSta == 50 && m_pUserData->m_bDex == 70 && m_pUserData->m_bIntel == 70 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 50;
		m_pUserData->m_bSta = 50;
		m_pUserData->m_bDex = 70;
		m_pUserData->m_bIntel = 70;
		m_pUserData->m_bCha = 50;
		break;
	case KARUS_WOMAN:
		if( m_pUserData->m_bStr == 50 && m_pUserData->m_bSta == 60 && m_pUserData->m_bDex == 60 && m_pUserData->m_bIntel == 70 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 50;
		m_pUserData->m_bSta = 60;
		m_pUserData->m_bDex = 60;
		m_pUserData->m_bIntel = 70;
		m_pUserData->m_bCha = 50;
		break;
	case BABARIAN:
		if( m_pUserData->m_bStr == 65 && m_pUserData->m_bSta == 65 && m_pUserData->m_bDex == 60 && m_pUserData->m_bIntel == 50 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 65;
		m_pUserData->m_bSta = 65;
		m_pUserData->m_bDex = 60;
		m_pUserData->m_bIntel = 50;
		m_pUserData->m_bCha = 50;
		break;
	case ELMORAD_MAN:
		if( m_pUserData->m_bStr == 60 && m_pUserData->m_bSta == 60 && m_pUserData->m_bDex == 70 && m_pUserData->m_bIntel == 50 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 60;
		m_pUserData->m_bSta = 60;
		m_pUserData->m_bDex = 70;
		m_pUserData->m_bIntel = 50;
		m_pUserData->m_bCha = 50;
		break;
	case ELMORAD_WOMAN:
		if( m_pUserData->m_bStr == 50 && m_pUserData->m_bSta == 50 && m_pUserData->m_bDex == 70 && m_pUserData->m_bIntel == 70 && m_pUserData->m_bCha == 50 )	{
			type = 0x02;	
			goto fail_return;
		}
		m_pUserData->m_bStr = 50;
		m_pUserData->m_bSta = 50;
		m_pUserData->m_bDex = 70;
		m_pUserData->m_bIntel = 70;
		m_pUserData->m_bCha = 50;
		break;
	}

	m_pUserData->m_bPoints = (m_pUserData->m_bLevel-1) * 3 + 10;
	m_pUserData->m_iGold = money;

	SetUserAbility();
	Send2AI_UserUpdateInfo();

	type = 0x01;
	SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
	SetByte( send_buff, ALL_POINT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	SetShort( send_buff, m_pUserData->m_bStr, send_index );
	SetShort( send_buff, m_pUserData->m_bSta, send_index );
	SetShort( send_buff, m_pUserData->m_bDex, send_index );
	SetShort( send_buff, m_pUserData->m_bIntel, send_index );
	SetShort( send_buff, m_pUserData->m_bCha, send_index );
	SetShort( send_buff, m_iMaxHp, send_index );
	SetShort( send_buff, m_iMaxMp, send_index );
	SetShort( send_buff, m_sTotalHit, send_index );
	SetShort( send_buff, m_sMaxWeight, send_index );
	SetShort( send_buff, m_pUserData->m_bPoints, send_index );
	Send( send_buff, send_index );


fail_return:
	SetByte( send_buff, WIZ_CLASS_CHANGE, send_index );
	SetByte( send_buff, ALL_POINT_CHANGE, send_index );
	SetByte( send_buff, type, send_index );
	SetDWORD( send_buff, temp_money, send_index );
	Send( send_buff, send_index );

}

void CUser::GoldChange(short tid, int gold)
{
//
	if (m_pUserData->m_bZone < 3) return;	// Money only changes in Frontier zone and Battle zone!!!
//
//
	if (m_pUserData->m_bZone == ZONE_SNOW_BATTLE) return;
//
	if( tid >= MAX_USER || tid < 0 ) return;	// Users ONLY!!!

	int s_temp_gold = 0; int t_temp_gold = 0;
	int send_index = 0;
	BYTE s_type = 0; BYTE t_type = 0;    // 1 -> Get gold    2 -> Lose gold

	char send_buff[256];
	memset( send_buff, 0x00, 256 );	

	CUser* pTUser = NULL;	
	pTUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];
	if( !pTUser ) return;
	
	if ( pTUser->m_pUserData->m_iGold <= 0 ) return;

	if (gold == 0) {		// Reward money in battle zone!!!
		if (m_sPartyIndex == -1) {	// Source is NOT in a party.
			s_type = 1 ; t_type = 2 ;

			s_temp_gold = (pTUser->m_pUserData->m_iGold * 4) / 10;
			t_temp_gold = pTUser->m_pUserData->m_iGold / 2;

			m_pUserData->m_iGold += s_temp_gold;
			pTUser->m_pUserData->m_iGold -= t_temp_gold;			
		}
		else {	// When the source is in a party.			
			_PARTY_GROUP* pParty = NULL;
			pParty = m_pMain->m_PartyArray.GetData( m_sPartyIndex );
			if( !pParty ) return;			

			s_type = 1 ; t_type = 2 ;

			s_temp_gold = (pTUser->m_pUserData->m_iGold * 4) / 10;
			t_temp_gold = pTUser->m_pUserData->m_iGold / 2;
			
			pTUser->m_pUserData->m_iGold -= t_temp_gold;		

			SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// First the victim...
			SetByte( send_buff, t_type, send_index );
			SetDWORD( send_buff, t_temp_gold, send_index );
			SetDWORD( send_buff, pTUser->m_pUserData->m_iGold, send_index );
			pTUser->Send( send_buff, send_index );	

			int usercount = 0, money = 0, levelsum = 0, count = 0;		// For the loot sharing procedure...
			count = s_temp_gold ;

			for( int i=0; i<8; i++ ) {
				if( pParty->uid[i] != -1 ) {
					usercount++;
					levelsum += pParty->bLevel[i];
				}
			}

			if( usercount == 0 ) return;

			for(int i=0; i<8; i++ ) {
				if( pParty->uid[i] != -1 || pParty->uid[i] >= MAX_USER ) {
					CUser * pUser = NULL;
					pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[pParty->uid[i]];
					if( !pUser ) continue;

					money = count * (float)( pUser->m_pUserData->m_bLevel/(float)levelsum );
					pUser->m_pUserData->m_iGold += money;

					send_index = 0; memset( send_buff, 0x00, 256 );
					SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );		// Now the party members...
					SetByte( send_buff, 1, send_index );
					SetDWORD( send_buff, money, send_index );
					SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
					pUser->Send( send_buff, send_index );
				}
			}			
			return ;
		}
	}
	else {	// When actual values are provided.
		if (gold > 0 ) {	// Source gains money.
			s_type = 1; t_type = 2;
			s_temp_gold = gold; t_temp_gold = gold;

			m_pUserData->m_iGold += s_temp_gold;
			pTUser->m_pUserData->m_iGold -= t_temp_gold;
		}
		else {	// Source loses money.
			s_type = 2; t_type = 1;
			s_temp_gold = gold; t_temp_gold = gold;

			m_pUserData->m_iGold -= s_temp_gold;
			pTUser->m_pUserData->m_iGold += t_temp_gold;
		}
	}

	SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// First the source...
	SetByte( send_buff, s_type, send_index );
	SetDWORD( send_buff, s_temp_gold, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );	

	send_index = 0;
	memset( send_buff, 0x00, 256 );	

	SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// Now the target
	SetByte( send_buff, t_type, send_index );
	SetDWORD( send_buff, t_temp_gold, send_index );
	SetDWORD( send_buff, pTUser->m_pUserData->m_iGold, send_index );
	pTUser->Send( send_buff, send_index );	
}

void CUser::SelectWarpList(char *pBuf)
{
	int index = 0, send_index = 0, warpid = 0, zoneindex = 0;
	_WARP_INFO* pWarp = NULL;
	_ZONE_SERVERINFO *pInfo	= NULL;
	C3DMap* pMap = NULL;
	char send_buff[128]; memset( send_buff, 0x00, 128 );

// ºñ·¯¸Ó±Û ¼ø°£ÀÌµ¿ >.<
	BYTE type = 2 ;
//		
	warpid = GetShort( pBuf, index );

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
	pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	return; 

	pWarp = pMap->m_WarpArray.GetData( warpid );
	if( !pWarp ) return;

	zoneindex = m_pMain->GetZoneIndex( pWarp->sZone );
	if( zoneindex < 0 ) return;

	pMap = m_pMain->m_ZoneArray[zoneindex];
	pInfo = m_pMain->m_ServerArray.GetData( pMap->m_nServerNo );
	if( !pInfo ) return;

	float rx = 0.0f, rz = 0.0f;
	rx = (float)myrand( 0, (int)pWarp->fR*2 );
	if( rx < pWarp->fR ) rx = -rx;
	rz = (float)myrand( 0, (int)pWarp->fR*2 );
	if( rz < pWarp->fR ) rz = -rz;

// ºñ·¯¸Ó±Û ¼ø°£ÀÌµ¿ >.<
/*
	SetByte( send_buff, WIZ_WARP_LIST, send_index );
	SetByte( send_buff, type, send_index );
	SetByte( send_buff, 1, send_index );
	Send(send_buff, send_index);
*/	

	if (m_pUserData->m_bZone == pWarp->sZone) {
		m_bZoneChangeSameZone = TRUE;

		SetByte( send_buff, WIZ_WARP_LIST, send_index );
		SetByte( send_buff, type, send_index );
		SetByte( send_buff, 1, send_index );
		Send(send_buff, send_index);
	}
//
	ZoneChange( pWarp->sZone, pWarp->fX + rx, pWarp->fZ + rz );
	
/*	SetByte( send_buff, WIZ_VIRTUAL_SERVER, send_index );
	SetShort( send_buff, strlen( pInfo->strServerIP ), send_index );
	SetString( send_buff, pInfo->strServerIP, strlen( pInfo->strServerIP ), send_index );
	SetShort( send_buff, pInfo->sPort, send_index );
	Send( send_buff, send_index );	*/
}

void CUser::ZoneConCurrentUsers(char *pBuf)
{
	int index = 0, send_index = 0, zone = 0, usercount = 0, nation = 0;
	char send_buff[128]; memset( send_buff, 0x00, 128 );
	CUser* pUser = NULL;

	zone = GetShort( pBuf, index );
	nation = GetByte( pBuf, index );

	for(int i=0; i<MAX_USER; i++ ) {
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if( !pUser )
			continue;
		if( pUser->m_pUserData->m_bZone == zone && pUser->m_pUserData->m_bNation == nation)
			usercount++;
	}

	SetByte( send_buff, WIZ_ZONE_CONCURRENT, send_index );
	SetShort( send_buff, usercount, send_index );
	Send( send_buff, send_index );
}

void CUser::ServerChangeOk(char *pBuf)
{
	int index = 0, warpid = 0;
	_WARP_INFO* pWarp = NULL;
	C3DMap* pMap = NULL;
	float rx = 0.0f, rz = 0.0f;
/* ºñ·¯¸Ó±Û ¼ø°£ÀÌµ¿ >.<
	int send_index = 0;
	char send_buff[128]; memset(send_buff, 0x00, 128);
	BYTE type = 2 ;
*/	
	warpid = GetShort( pBuf, index );

	if( m_iZoneIndex < 0 || m_iZoneIndex >= (int)m_pMain->m_ZoneArray.size() ) return;
	pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	return;

	pWarp = pMap->m_WarpArray.GetData( warpid );
	if( !pWarp ) return;

	rx = (float)myrand( 0, (int)pWarp->fR*2 );
	if( rx < pWarp->fR ) rx = -rx;
	rz = (float)myrand( 0, (int)pWarp->fR*2 );
	if( rz < pWarp->fR ) rz = -rz;

/* ºñ·¯¸Ó±Û ¼ø°£ÀÌµ¿ >.<
	SetByte( send_buff, WIZ_WARP_LIST, send_index );
	SetByte( send_buff, type, send_index );
	SetByte( send_buff, 1, send_index );
	Send(send_buff, send_index);
*/
	ZoneChange( pWarp->sZone, pWarp->fX + rx, pWarp->fZ + rz );
}

BOOL CUser::GetWarpList(int warp_group)
{
	_WARP_INFO* pWarp = NULL;
	C3DMap* pMap = NULL;
	int warpid = 0, send_index = 0;	// Çì´õ¿Í Ä«¿îÆ®¸¦ ³ªÁß¿¡ ÆĞÅ·...
	int zoneindex = -1, temp_index = 0, count = 0;
	char buff[81920]; memset(buff, 0x00, 81920);
	char send_buff[81920]; memset(send_buff, 0x00, 81920);
// ºñ·¯¸Ó±Û ¸¶À» ÀÌ¸§ Ç¥½Ã >.<
	BYTE type = 1;		// 1ÀÌ¸é ÀÏ¹İ, 2ÀÌ¸é ¿öÇÁ ¼º°øÇß´ÂÁö ¾ÊÇß´ÂÁö ^^;
//
	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return FALSE;
	pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )
		return FALSE;

	map < int, _WARP_INFO* >::iterator		Iter1;
	map < int, _WARP_INFO* >::iterator		Iter2;
	
	Iter1 = pMap->m_WarpArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_WarpArray.m_UserTypeMap.end();

	for( ; Iter1 != Iter2; Iter1++ ) {
		pWarp = (*Iter1).second;
		if( !pWarp ) continue;
		if( (pWarp->sWarpID / 10) != warp_group ) continue;

		SetShort( buff, pWarp->sWarpID, send_index );

		SetShort( buff, strlen(pWarp->strWarpName), send_index );
		SetString( buff, pWarp->strWarpName, strlen(pWarp->strWarpName), send_index );

		SetShort( buff, strlen(pWarp->strAnnounce), send_index );
		SetString( buff, pWarp->strAnnounce, strlen(pWarp->strAnnounce), send_index );

		SetShort( buff, pWarp->sZone, send_index );

		// 1298 client expects MAX USER immediately after the destination zone.
		// Omitting this SHORT shifts gold/X/Z/Y and corrupts the warp entry.
		zoneindex = m_pMain->GetZoneIndex( pWarp->sZone );
		if( zoneindex >= 0 && zoneindex < (int)m_pMain->m_ZoneArray.size() && m_pMain->m_ZoneArray[zoneindex] != NULL )
			SetShort( buff, m_pMain->m_ZoneArray[zoneindex]->m_sMaxUser, send_index );
		else
			SetShort( buff, 0, send_index );
		SetDWORD( buff, pWarp->dwPay, send_index );

		SetShort( buff, (short)(pWarp->fX*10), send_index );
		SetShort( buff, (short)(pWarp->fZ*10), send_index );
		SetShort( buff, (short)(pWarp->fY*10), send_index );
		count++;
	}

	SetByte( send_buff, WIZ_WARP_LIST, temp_index );
// ºñ·¯¸Ó±Û ¸¶À» ÀÌ¸§ Ç¥½Ã >.<
	SetByte( send_buff, type, temp_index );
//
	SetShort( send_buff, count, temp_index );
	SetString( send_buff, buff, send_index, temp_index );
	Send( send_buff, temp_index );

	return TRUE;
}

void CUser::InitType3()
{
	for (int i = 0 ; i < MAX_TYPE3_REPEAT ; i++) {     // This is for the duration spells Type 3.
		m_fHPStartTime[i] = 0.0f;		
		m_fHPLastTime[i] = 0.0f;
		m_bHPAmount[i] = 0;
		m_bHPDuration[i] = 0;
		m_bHPInterval[i] = 5;
		m_sSourceID[i] = -1;
	}

	m_bType3Flag = FALSE;
}

BOOL CUser::BindObjectEvent(short objectindex, short nid)
{
	int send_index = 0, result = 0;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	_OBJECT_EVENT* pEvent = NULL;
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) return FALSE;

	if( pEvent->sBelong != 0 && pEvent->sBelong != m_pUserData->m_bNation ) {
		result = 0;
	}
	else {
		m_pUserData->m_sBind = pEvent->sIndex;
		result = 1;
	}

	SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
	SetByte( send_buff, pEvent->sType, send_index );
	SetByte( send_buff, result, send_index );
	Send( send_buff, send_index );	

	return TRUE;
}

BOOL CUser::GateObjectEvent(short objectindex, short nid)
{
	if( m_pMain->m_bPointCheckFlag == FALSE)	return FALSE;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ

	int  send_index = 0, result = 0 ;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	_OBJECT_EVENT* pEvent = NULL;
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) return FALSE;

	CNpc* pNpc = NULL;
	pNpc = m_pMain->m_arNpcArray.GetData( nid );
	if( !pNpc )	return FALSE;
	
	if( pNpc->m_tNpcType == NPC_GATE || pNpc->m_tNpcType == NPC_PHOENIX_GATE || pNpc->m_tNpcType == NPC_SPECIAL_GATE ) {
		pNpc->m_byGateOpen = !pNpc->m_byGateOpen;
		result = 1;

		memset( send_buff, NULL, 128 );	send_index = 0;
		SetByte( send_buff, AG_NPC_GATE_OPEN, send_index );
		SetShort( send_buff, nid, send_index );
		SetByte( send_buff, pNpc->m_byGateOpen, send_index );
		m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
	}
	else {
		result = 0;
	}	
	
	memset( send_buff, NULL, 128 );	send_index = 0;		
	SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
	SetByte( send_buff, pEvent->sType, send_index );
	SetByte( send_buff, result, send_index );
	SetShort( send_buff, nid, send_index );
	SetByte( send_buff, pNpc->m_byGateOpen, send_index );
	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );
	
	return TRUE;
}

BOOL CUser::GateLeverObjectEvent(short objectindex, short nid)
{
	if( m_pMain->m_bPointCheckFlag == FALSE)	return FALSE;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ

	int send_index = 0, result = 0 ;
	char send_buff[128]; memset( send_buff, NULL, 128 );
	
	_OBJECT_EVENT* pEvent = NULL;
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) return FALSE;

	CNpc* pNpc = NULL;
	pNpc = m_pMain->m_arNpcArray.GetData( nid );
	if( !pNpc ) return FALSE;

	_OBJECT_EVENT* pGateEvent = NULL;	
	pGateEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( pEvent->sControlNpcID );
	if( !pGateEvent ) return FALSE;

	CNpc* pGateNpc = NULL;
	pGateNpc = m_pMain->GetNpcPtr(pEvent->sControlNpcID, m_pUserData->m_bZone);
	if( !pGateNpc ) {
		result = 0;
	}
	else {
		if( pGateNpc->m_tNpcType == NPC_GATE || pGateNpc->m_tNpcType == NPC_PHOENIX_GATE || pGateNpc->m_tNpcType == NPC_SPECIAL_GATE ) {
			if(pNpc->m_byGroup != m_pUserData->m_bNation && pNpc->m_byGroup != 0)	{
				if(pNpc->m_byGateOpen == 0)	return FALSE;
			}
			pNpc->m_byGateOpen = !pNpc->m_byGateOpen;
			result = 1;
			memset( send_buff, NULL, 128 );		send_index = 0;
			SetByte( send_buff, AG_NPC_GATE_OPEN, send_index );
			SetShort( send_buff, nid, send_index );
			SetByte( send_buff, pNpc->m_byGateOpen, send_index );
			m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);

			pGateNpc->m_byGateOpen = !pGateNpc->m_byGateOpen;
			result = 1;
			memset( send_buff, NULL, 128 );		send_index = 0;
			SetByte( send_buff, AG_NPC_GATE_OPEN, send_index );
			SetShort( send_buff, pGateNpc->m_sNid, send_index );
			SetByte( send_buff, pGateNpc->m_byGateOpen, send_index );
			m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);

			memset( send_buff, NULL, 128 );		send_index = 0;
			SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
			SetByte( send_buff, pGateEvent->sType, send_index );
			SetByte( send_buff, result, send_index );
			SetShort( send_buff, pGateNpc->m_sNid, send_index );
			SetByte( send_buff, pGateNpc->m_byGateOpen, send_index );
			m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );	
		}
		else	result = 0;
	}

	memset( send_buff, NULL, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
	SetByte( send_buff, pEvent->sType, send_index );
	SetByte( send_buff, result, send_index );
	SetShort( send_buff, nid, send_index );
	SetByte( send_buff, pNpc->m_byGateOpen, send_index );
	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );	
	
	return TRUE;
}

BOOL CUser::FlagObjectEvent(short objectindex, short nid)
{
	if( m_pMain->m_bPointCheckFlag == FALSE)	return FALSE;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ

	int  send_index = 0, result = 0;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	_OBJECT_EVENT* pEvent = NULL;
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) return FALSE;

	CNpc* pNpc = NULL;
	pNpc = m_pMain->m_arNpcArray.GetData( nid );
	if( !pNpc ) return FALSE;

	_OBJECT_EVENT* pFlagEvent = NULL;
	pFlagEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( pEvent->sControlNpcID );
	if ( !pFlagEvent ) return FALSE;

	CNpc* pFlagNpc = NULL;
	pFlagNpc = m_pMain->GetNpcPtr(pEvent->sControlNpcID, m_pUserData->m_bZone);
	if ( !pFlagNpc ) {
		result = 0;
	}
	else {
		if( pFlagNpc->m_tNpcType == NPC_GATE || pFlagNpc->m_tNpcType == NPC_PHOENIX_GATE || pFlagNpc->m_tNpcType == NPC_SPECIAL_GATE ) {
			if (m_pMain->m_bVictory > 0) return FALSE;
			if (pNpc->m_byGateOpen == 0) return FALSE;
	//			if (pNpc->m_byGroup != 0 && pFlagNpc->m_byGroup != 0) goto fail_return;
			
			result = 1;

	//			pNpc->m_byGroup = m_pUserData->m_bNation;		
	//			pNpc->m_byGateOpen = !pNpc->m_byGateOpen;	
			pNpc->m_byGateOpen = 0;	// LEVER !!!

			memset( send_buff, NULL, 128 );		send_index = 0;
			SetByte( send_buff, AG_NPC_GATE_OPEN, send_index );
			SetShort( send_buff, nid, send_index );
			SetByte( send_buff, pNpc->m_byGateOpen, send_index );
			m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
			memset( send_buff, NULL, 128 );		send_index = 0;

	//			pFlagNpc->m_byGroup = m_pUserData->m_bNation;		
	//			pFlagNpc->m_byGateOpen = !pFlagNpc->m_byGateOpen;	// FLAG !!!
			pFlagNpc->m_byGateOpen = 0;

			SetByte( send_buff, AG_NPC_GATE_OPEN, send_index );		// (Send to AI....)
			SetShort( send_buff, pFlagNpc->m_sNid, send_index );
			SetByte( send_buff, pFlagNpc->m_byGateOpen, send_index );
			m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
			memset( send_buff, NULL, 128 );		send_index = 0;

			SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );		// (Send to Region...)
			SetByte( send_buff, pFlagEvent->sType, send_index );
			SetByte( send_buff, result, send_index );
			SetShort( send_buff, pFlagNpc->m_sNid, send_index );
			SetByte( send_buff, pFlagNpc->m_byGateOpen, send_index );
			m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );	

			if (m_pUserData->m_bNation == KARUS) {		// ADD FLAG SCORE !!!
				m_pMain->m_bKarusFlag++;
			}
			else if (m_pUserData->m_bNation == ELMORAD) {
				m_pMain->m_bElmoradFlag++;
			}
			
			m_pMain->BattleZoneVictoryCheck();		// Did one of the teams win?
		}
		else	result = 0;
	}

	memset( send_buff, NULL, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
	SetByte( send_buff, pEvent->sType, send_index );
	SetByte( send_buff, result, send_index );
	SetShort( send_buff, nid, send_index );
	SetByte( send_buff, pNpc->m_byGateOpen, send_index );
	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );

	return TRUE;
}

BOOL CUser::WarpListObjectEvent(short objectindex, short nid)
{
	int send_index = 0, result = 0;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	_OBJECT_EVENT* pEvent = NULL;	
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) return FALSE;

	if( !GetWarpList( pEvent->sControlNpcID ) ) return FALSE;
			
	return TRUE;
}
 
void CUser::ObjectEvent(char *pBuf)
{
	int index = 0, objectindex = 0, send_index = 0, result = 0, nid = 0;
	char send_buff[128]; memset( send_buff, NULL, 128 );

	_OBJECT_EVENT* pEvent = NULL;

	objectindex = GetShort( pBuf, index );
	nid = GetShort( pBuf, index );

	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) goto fail_return;	
	pEvent = m_pMain->m_ZoneArray[m_iZoneIndex]->GetObjectEvent( objectindex );
	if( !pEvent ) goto fail_return;

	// Magic Anvil
	if(objectindex == 5001) {
		SetByte( send_buff, WIZ_NEW_EVENT, send_index );
		SetByte( send_buff, 0x01, send_index ); // Magic Anvil
		SetShort( send_buff, nid, send_index );
		Send( send_buff, send_index );
	} else { 
		switch( pEvent->sType ) {
//			case 8:		// Magic Anvil
//				if (!UpgradeObjectEvent(objectindex, nid)) goto fail_return;
//				break;
			case 0:		// Bind Point
			case 7:		// Destory Bind Point
				if (!BindObjectEvent(objectindex, nid)) goto fail_return;
				break;
			case 1:		// Gate Object : »ç¿ëÄ¡ ¾ÊÀ½ : 2002.12.23
			case 2:
				//if (!GateObjectEvent(objectindex, nid)) goto fail_return; 
				break;
			case 3:		// Gate lever Object
				if (!GateLeverObjectEvent(objectindex, nid)) goto fail_return;
				break;
			case 4:		// Flag Lever Object
				if (!FlagObjectEvent(objectindex, nid)) goto fail_return;
				break;
			case 5:		// Warp List
				if(!WarpListObjectEvent(objectindex, nid)) goto fail_return;
				break;
		}
	}
	return;

fail_return:
	SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
	SetByte( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::Friend(char *pBuf)
{
	int index = 0;
	BYTE subcommand = GetByte( pBuf, index );

	switch( subcommand ) {
		case FRIEND_REQUEST:
			FriendRequest(pBuf+index);
			break;
		case FRIEND_ACCEPT:
			FriendAccept(pBuf+index);
			break;
		case FRIEND_REPORT:
			FriendReport(pBuf+index);
			break;
	}
}

void CUser::FriendRequest(char *pBuf)
{
	int index = 0, destid = -1, send_index = 0;

	CUser* pUser = NULL;
	char buff[256];	memset( buff, 0x00, 256 );

	destid = GetShort( pBuf, index );
	if( destid < 0 || destid >= MAX_USER ) goto fail_return;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[destid];

	if( !pUser ) goto fail_return;
	if( pUser->m_sFriendUser != -1 ) goto fail_return;
	if( pUser->m_pUserData->m_bNation != m_pUserData->m_bNation ) goto fail_return;

	m_sFriendUser = destid;
	pUser->m_sFriendUser = m_Sid;

	SetByte( buff, WIZ_FRIEND_REPORT, send_index );
	SetByte( buff, FRIEND_REQUEST, send_index );
	SetShort( buff, m_Sid, send_index );
	pUser->Send( buff, send_index );	
	return;

fail_return:
	SetByte( buff, WIZ_FRIEND_REPORT, send_index );
	SetByte( buff, FRIEND_CANCEL, send_index );
	Send( buff, send_index );
}

void CUser::FriendAccept(char *pBuf)
{
	int index = 0, destid = -1, send_index = 0;
	CUser* pUser = NULL;
	char buff[256];	memset( buff, 0x00, 256 );

	BYTE result = GetByte( pBuf, index );

	if( m_sFriendUser < 0 || m_sFriendUser >= MAX_USER ) {
		m_sFriendUser = -1;
		return;
	}

	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_sFriendUser];

	if( !pUser ) {
		m_sFriendUser = -1;
		return;
	}

	m_sFriendUser = -1;
	pUser->m_sFriendUser = -1;

	SetByte( buff, WIZ_FRIEND_REPORT, send_index );
	SetByte( buff, FRIEND_ACCEPT, send_index );
	SetByte( buff, result, send_index );
	pUser->Send( buff, send_index );
}

void CUser::Corpse()
{
	int send_index = 0;
	char send_buff[256]; memset( send_buff, 0x00, 256 );

	SetByte( send_buff, WIZ_CORPSE, send_index );
	SetShort( send_buff, m_Sid, send_index );
	m_pMain->Send_Region( send_buff, send_index, m_pUserData->m_bZone, m_RegionX, m_RegionZ, NULL, false );	
}

void CUser::PartyBBS(char *pBuf)
{
	int index = 0;
	BYTE subcommand = GetByte( pBuf, index );
		
	switch( subcommand ) {
		case PARTY_BBS_REGISTER :	// When you register a message on the Party BBS.
			PartyBBSRegister(pBuf+index);
			break;
		case PARTY_BBS_DELETE :		// When you delete your message on the Party BBS.
			PartyBBSDelete(pBuf+index);
			break;
		case PARTY_BBS_NEEDED :		// Get the 'needed' messages from the Party BBS.
			PartyBBSNeeded(pBuf+index, PARTY_BBS_NEEDED);
			break;
	}
}

void CUser::PartyBBSRegister(char *pBuf)
{
	CUser* pUser = NULL;
	int index = 0, send_index = 0;	// Basic Initializations. 			
	BYTE result = 0; short bbs_len = 0;
	char send_buff[256]; memset(send_buff, NULL, 256);
	int i = 0, counter = 0;

	if (m_sPartyIndex != -1) goto fail_return;	// You are already in a party!
	if (m_bNeedParty == 2) goto fail_return;	// You are already on the BBS!

	m_bNeedParty = 2;	// Success! Now you officially need a party!!!
	result = 1;

	SetByte(send_buff, 2, send_index);	// Send new 'Need Party Status' to region!!!
	SetByte(send_buff, m_bNeedParty, send_index);
	StateChange(send_buff);

	send_index = 0; memset(send_buff, NULL, 256);	// Now, let's find out which page the user is on.
	for (i = 0 ; i < MAX_USER ; i++) {
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if (!pUser) continue;				// Protection codes.
		if ( pUser->m_pUserData->m_bNation != m_pUserData->m_bNation) continue;
		if ( pUser->m_bNeedParty == 1 ) continue;
		if( !(   ( pUser->m_pUserData->m_bLevel <= (int)(m_pUserData->m_bLevel * 1.5) && pUser->m_pUserData->m_bLevel >= (int)(m_pUserData->m_bLevel * 1.5)) 
			  || ( pUser->m_pUserData->m_bLevel <= (m_pUserData->m_bLevel+8) && pUser->m_pUserData->m_bLevel >= ((int)(m_pUserData->m_bLevel)-8) ) 
		) ) continue;

		if (pUser->m_Sid == m_Sid) break;
		counter++;		
	}

	SetShort(send_buff, counter / MAX_BBS_PAGE, send_index);
	PartyBBSNeeded(send_buff, PARTY_BBS_REGISTER);
	return;

fail_return:
	SetByte(send_buff, WIZ_PARTY_BBS, send_index);
	SetByte(send_buff, PARTY_BBS_REGISTER, send_index);
	SetByte(send_buff, result, send_index);
	Send(send_buff, send_index);
	return;
}

void CUser::PartyBBSDelete(char *pBuf)
{
	int send_index = 0;	// Basic Initializations. 			
	BYTE result = 0; 
	char send_buff[256]; memset( send_buff, NULL, 256);

	if (m_bNeedParty == 1) goto fail_return;	// You don't need anymore 

	m_bNeedParty = 1;	// Success! You no longer need a party !!!
	result = 1;

	SetByte(send_buff, 2, send_index);	// Send new 'Need Party Status' to region!!!
	SetByte(send_buff, m_bNeedParty, send_index);
	StateChange(send_buff);

	send_index = 0; memset(send_buff, NULL, 256);	// Now, let's find out which page the user is on.
	SetShort(send_buff, 0, send_index);
	PartyBBSNeeded(send_buff, PARTY_BBS_DELETE);
	return;

fail_return:
	SetByte(send_buff, WIZ_PARTY_BBS, send_index);
	SetByte(send_buff, PARTY_BBS_DELETE, send_index);
	SetByte(send_buff, result, send_index);
	Send(send_buff, send_index);
	return;	
}

void CUser::PartyBBSNeeded(char *pBuf, BYTE type)
{
	CUser* pUser = NULL;	// Basic Initializations. 	
	int index = 0, send_index = 0;				
	BYTE result = 0; short bbs_len = 0;
	char send_buff[256]; memset( send_buff, NULL, 256 );
	short page_index = 0; short start_counter = 0; BYTE valid_counter = 0 ;
	int  i = 0, j = 0; short BBS_Counter = 0;
	
	page_index = GetShort(pBuf, index);
	start_counter = page_index * MAX_BBS_PAGE;

	if ( start_counter < 0 ) goto fail_return;
	if ( start_counter > MAX_USER ) goto fail_return;

	result = 1;

	SetByte(send_buff, WIZ_PARTY_BBS, send_index);
	SetByte(send_buff, type, send_index);
	SetByte(send_buff, result, send_index);

	for (i = 0 ; i < MAX_USER ; i++) {
		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
		if (!pUser) continue;				// Protection codes.

		if ( pUser->m_pUserData->m_bNation != m_pUserData->m_bNation) continue;
		if ( pUser->m_bNeedParty == 1) continue;
		if( !(   ( pUser->m_pUserData->m_bLevel <= (int)(m_pUserData->m_bLevel * 1.5) && pUser->m_pUserData->m_bLevel >= (int)(m_pUserData->m_bLevel * 1.5)) 
			  || ( pUser->m_pUserData->m_bLevel <= (m_pUserData->m_bLevel+8) && pUser->m_pUserData->m_bLevel >= ((int)(m_pUserData->m_bLevel)-8) ) 
		) ) continue;

		BBS_Counter++;

		if (i < start_counter) continue;	// Range check codes.
		if (valid_counter >= MAX_BBS_PAGE) continue;

		SetShort(send_buff, strlen(pUser->m_pUserData->m_id), send_index);	// Create packet.
		SetString(send_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), send_index);
		SetByte(send_buff, pUser->m_pUserData->m_bLevel, send_index);
		SetShort(send_buff, pUser->m_pUserData->m_sClass, send_index);

		valid_counter++;		// Increment counters.
//		BBS_Counter++;		
	}

	if ( valid_counter < MAX_BBS_PAGE ) {	// You still need to fill up ten slots.
//		for (j = 0 ; j < MAX_BBS_PAGE ; j++) {
		for (j = valid_counter ; j < MAX_BBS_PAGE ; j++) {
			SetShort(send_buff, 0, send_index);
			SetString(send_buff, NULL, 0, send_index);
			SetByte(send_buff, 0, send_index);
			SetShort(send_buff, 0, send_index);
		}
	}

	SetShort(send_buff, page_index, send_index);
	SetShort(send_buff, BBS_Counter, send_index);
	Send(send_buff, send_index);
	return;

fail_return:
	SetByte(send_buff, WIZ_PARTY_BBS, send_index);
	SetByte(send_buff, PARTY_BBS_NEEDED, send_index);
	SetByte(send_buff, result, send_index);
	Send(send_buff, send_index);
	return;		
}

void CUser::MarketBBS(char *pBuf)
{
	int index = 0;
	BYTE subcommand = GetByte( pBuf, index );
		
	MarketBBSBuyPostFilter();		// Get rid of empty slots.
	MarketBBSSellPostFilter();

	switch( subcommand ) {
		case MARKET_BBS_REGISTER :			// When you register a message on the Market BBS.
			MarketBBSRegister(pBuf+index);
			break;
		case MARKET_BBS_DELETE :			// When you delete your message on the Market BBS.
			MarketBBSDelete(pBuf+index);
			break;
		case MARKET_BBS_REPORT :			// Get the 'needed' messages from the Market BBS.
			MarketBBSReport(pBuf+index, MARKET_BBS_REPORT);
			break;
		case MARKET_BBS_OPEN:				// When you first open the Market BBS.
			MarketBBSReport(pBuf+index, MARKET_BBS_OPEN);		
			break;
		case MARKET_BBS_REMOTE_PURCHASE:	// When you agree to spend money on remote bartering.
			MarketBBSRemotePurchase(pBuf+index);
			break;
		case MARKET_BBS_MESSAGE:			// USE ONLY IN EMERGENCY!!!
			MarketBBSMessage(pBuf+index);	
			break;
	}
}

void CUser::MarketBBSRegister(char *pBuf)
{
	CUser* pUser = NULL;	// Basic Initializations. 	
	int index = 0, send_index = 0; BYTE result = 0;	BYTE sub_result = 1;			 
	char send_buff[256]; memset( send_buff, NULL, 256 );
	short title_len = 0; short message_len = 0; BYTE buysell_index = 0; 
	int  i = 0, j = 0; int page_index = 0;

	buysell_index = GetByte(pBuf, index);	// Buy or sell?

	if (buysell_index == MARKET_BBS_BUY) {
		if (m_pUserData->m_iGold < BUY_POST_PRICE) {
			sub_result = 2;
			goto fail_return;
		}
	}
	else if (buysell_index == MARKET_BBS_SELL) {
		if (m_pUserData->m_iGold < SELL_POST_PRICE) {
			sub_result = 2;
			goto fail_return;
		}
	}
		
	for (i = 0 ; i < MAX_BBS_POST ; i++) {
		if (buysell_index == MARKET_BBS_BUY) {			// Buy 
			if (m_pMain->m_sBuyID[i] == -1) {
				m_pMain->m_sBuyID[i] = m_Sid;
				title_len = GetShort(pBuf, index);		
				GetString(m_pMain->m_strBuyTitle[i], pBuf, title_len, index);		
				message_len = GetShort(pBuf, index);
				GetString(m_pMain->m_strBuyMessage[i], pBuf, message_len, index);
				m_pMain->m_iBuyPrice[i] = GetDWORD(pBuf, index);
				m_pMain->m_fBuyStartTime[i] = TimeGet();
				result = 1;
				break;
			}
		}
		else if (buysell_index == MARKET_BBS_SELL) {	// Sell
			if (m_pMain->m_sSellID[i] == -1) {
				m_pMain->m_sSellID[i] = m_Sid;
				title_len = GetShort(pBuf, index);	
				GetString(m_pMain->m_strSellTitle[i], pBuf, title_len, index);		
				message_len = GetShort(pBuf, index);
				GetString(m_pMain->m_strSellMessage[i], pBuf, message_len, index);
				m_pMain->m_iSellPrice[i] = GetDWORD(pBuf, index);				
				m_pMain->m_fSellStartTime[i] = TimeGet();
				result = 1;
				break;
			}
		}
		else goto fail_return;							// Error 
	}
	
	if (result == 0) goto fail_return;	// No spaces available

	SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );		// Money removal packet...
	SetByte( send_buff, 0x02, send_index );
	if (buysell_index == MARKET_BBS_BUY) {
		m_pUserData->m_iGold -= BUY_POST_PRICE;
		SetDWORD( send_buff, BUY_POST_PRICE, send_index );
	}
	else if (buysell_index == MARKET_BBS_SELL) {
		m_pUserData->m_iGold -= SELL_POST_PRICE;		
		SetDWORD( send_buff, SELL_POST_PRICE, send_index );	
	}
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );

	page_index = i / MAX_BBS_PAGE;
	send_index = 0; memset( send_buff, NULL, 256 );
	SetByte(send_buff, buysell_index, send_index);
	SetShort(send_buff, page_index, send_index);
	MarketBBSReport(send_buff, MARKET_BBS_REGISTER);	
	return;

fail_return:
	send_index = 0; memset( send_buff, NULL, 256 );
	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_REGISTER, send_index);
	SetByte(send_buff, buysell_index, send_index);
	SetByte(send_buff, result, send_index);
	SetByte(send_buff, sub_result, send_index);
	Send(send_buff, send_index);
	return;			
}

void CUser::MarketBBSDelete(char *pBuf)
{
	CUser* pUser = NULL;	// Basic Initializations. 	
	int index = 0, send_index = 0; BYTE result = 0;	BYTE sub_result = 1;			 
	char send_buff[256]; memset( send_buff, NULL, 256 );
	short delete_id = 0; BYTE buysell_index = 0; 
	int  i = 0, j = 0; 

	buysell_index = GetByte(pBuf, index);	// Buy or sell?
	delete_id = GetShort(pBuf, index);		// Which message should I delete? 

	if (buysell_index == MARKET_BBS_BUY) {	// Buy 
		if (m_pMain->m_sBuyID[delete_id] == m_Sid || m_pUserData->m_bAuthority == 0) {
			MarketBBSBuyDelete(delete_id);
			result = 1;	
		}
		else goto fail_return;
	}
	else if (buysell_index == MARKET_BBS_SELL) {	// Sell
		if (m_pMain->m_sSellID[delete_id] == m_Sid || m_pUserData->m_bAuthority == 0) {
			MarketBBSSellDelete(delete_id);
			result = 1;
		}
		else goto fail_return;
	}
	else goto fail_return;			// Error 
	
	SetShort(send_buff, buysell_index, send_index);
	SetShort(send_buff, 0, send_index);
	MarketBBSReport(send_buff, MARKET_BBS_DELETE);
	return;

fail_return:
	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_DELETE, send_index);
	SetByte(send_buff, buysell_index, send_index);
	SetByte(send_buff, result, send_index);
	SetByte(send_buff, sub_result, send_index);
	Send(send_buff, send_index);
	return;	
}

void CUser::MarketBBSReport(char *pBuf, BYTE type)
{
	CUser* pUser = NULL;	// Basic Initializations. 	
	int index = 0, send_index = 0;				
	BYTE result = 0; BYTE sub_result = 1; short bbs_len = 0;
	char send_buff[8192]; memset(send_buff, NULL, 8192);

	short page_index = 0; BYTE buysell_index = 0;
	short start_counter = 0; short valid_counter = 0 ;
	int  i = 0, j = 0; short BBS_Counter = 0;

	short title_length = 0;
	short message_length = 0;
	
	buysell_index = GetByte(pBuf, index);	// Buy or sell?
	page_index = GetShort(pBuf, index);		// Which message should I delete? 

	start_counter = page_index * MAX_BBS_PAGE;
	
	if (type == MARKET_BBS_OPEN) {
		start_counter = 0;
		page_index = 0;
	}

	if ( start_counter < 0 ) goto fail_return;
	if ( start_counter > MAX_BBS_POST ) goto fail_return;

	result = 1;

	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, type, send_index);
	SetByte(send_buff, buysell_index, send_index);
	SetByte(send_buff, result, send_index);

	for (i = 0 ; i < MAX_BBS_POST ; i++) {
		if (buysell_index == MARKET_BBS_BUY) {
			if (m_pMain->m_sBuyID[i] == -1) continue;

			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sBuyID[i]];
			if (!pUser) {		// Delete info!!!
				MarketBBSBuyDelete(i);
				continue ;	
			}

			BBS_Counter++;		// Increment number of total messages.
				
			if (i < start_counter) continue;	// Range check codes.
			if (valid_counter >= MAX_BBS_PAGE) continue;

			SetShort(send_buff, m_pMain->m_sBuyID[i], send_index);

			SetShort(send_buff, strlen(pUser->m_pUserData->m_id), send_index);	
			SetString(send_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), send_index);

			title_length = strlen(m_pMain->m_strBuyTitle[i]);
			if (title_length > MAX_BBS_TITLE) {
				title_length = MAX_BBS_TITLE;
			}

			SetShort(send_buff, title_length, send_index);
			SetString(send_buff, m_pMain->m_strBuyTitle[i], title_length, send_index);
//			SetShort(send_buff, strlen(m_pMain->m_strBuyTitle[i]), send_index);
//			SetString(send_buff, m_pMain->m_strBuyTitle[i], strlen(m_pMain->m_strBuyTitle[i]), send_index);

			message_length = strlen(m_pMain->m_strBuyMessage[i]);
			if (message_length > MAX_BBS_MESSAGE) {
				message_length = MAX_BBS_MESSAGE ;
			}

			SetShort(send_buff, message_length, send_index);
			SetString(send_buff, m_pMain->m_strBuyMessage[i], message_length, send_index);
//			SetShort(send_buff, strlen(m_pMain->m_strBuyMessage[i]), send_index);
//			SetString(send_buff, m_pMain->m_strBuyMessage[i], strlen(m_pMain->m_strBuyMessage[i]), send_index);

			SetDWORD(send_buff, m_pMain->m_iBuyPrice[i], send_index);
			SetShort(send_buff, i, send_index);

			valid_counter++;			
		}
		else if (buysell_index == MARKET_BBS_SELL) {
			if (m_pMain->m_sSellID[i] == -1) continue;

			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sSellID[i]];
			if (!pUser) {	// Delete info!!!	
				MarketBBSSellDelete(i);
				continue ;	
			}

			BBS_Counter++;
			
			if (i < start_counter) continue;	// Range check codes.
			if (valid_counter >= MAX_BBS_PAGE) continue;
		
			SetShort(send_buff, m_pMain->m_sSellID[i], send_index);

			SetShort(send_buff, strlen(pUser->m_pUserData->m_id), send_index);	
			SetString(send_buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), send_index);

			title_length = strlen(m_pMain->m_strSellTitle[i]);
			if (title_length > MAX_BBS_TITLE) {
				title_length = MAX_BBS_TITLE;
			}

			SetShort(send_buff, title_length, send_index);
			SetString(send_buff, m_pMain->m_strSellTitle[i], title_length, send_index);
//			SetShort(send_buff, strlen(m_pMain->m_strSellTitle[i]), send_index);
//			SetString(send_buff, m_pMain->m_strSellTitle[i], strlen(m_pMain->m_strSellTitle[i]), send_index);

			message_length = strlen(m_pMain->m_strSellMessage[i]);
			if (message_length > MAX_BBS_MESSAGE) {
				message_length = MAX_BBS_MESSAGE ;
			}

			SetShort(send_buff, message_length, send_index);
			SetString(send_buff, m_pMain->m_strSellMessage[i], message_length, send_index);
//			SetShort(send_buff, strlen(m_pMain->m_strSellMessage[i]), send_index);
//			SetString(send_buff, m_pMain->m_strSellMessage[i], strlen(m_pMain->m_strSellMessage[i]), send_index);

			SetDWORD(send_buff, m_pMain->m_iSellPrice[i], send_index);
			SetShort(send_buff, i, send_index);

			valid_counter++;	// Increment number of messages on the requested page			
		}	
	}
//
	if (valid_counter == 0 && page_index > 0) {
		goto fail_return1;
	}
//
	if (valid_counter < MAX_BBS_PAGE) {	// You still need to fill up slots.
		for (j = valid_counter ; j < MAX_BBS_PAGE ; j++) {		
			SetShort(send_buff, -1, send_index);
			SetShort(send_buff, 0, send_index);	
			SetString(send_buff, NULL, 0, send_index);
			SetShort(send_buff, 0, send_index);
			SetString(send_buff, NULL, 0, send_index);
			SetShort(send_buff, 0, send_index);
			SetString(send_buff, NULL, 0, send_index);
			SetDWORD(send_buff, 0, send_index);
			SetShort(send_buff, -1, send_index);

			valid_counter++;
		}
	}

	SetShort(send_buff, page_index, send_index);
	SetShort(send_buff, BBS_Counter, send_index);
	Send(send_buff, send_index);
	return;

fail_return:
	send_index = 0; memset(send_buff, NULL, 8192);
	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_REPORT, send_index);
	SetByte(send_buff, buysell_index, send_index);
	SetByte(send_buff, result, send_index);
	SetByte(send_buff, sub_result, send_index);
	Send(send_buff, send_index);
	return;

fail_return1:
	send_index = 0; memset(send_buff, NULL, 8192); 	
	SetShort(send_buff, buysell_index, send_index);
	SetShort(send_buff, page_index - 1, send_index);
	MarketBBSReport(send_buff, type);	
	return;
}

void CUser::MarketBBSRemotePurchase(char *pBuf)
{
	CUser* pUser = NULL;	
	int send_index = 0;	
	BYTE buysell_index = 0; short message_index = -1;
	BYTE result = 0; BYTE sub_result = 1; short tid = -1 ; int index = 0;
	char send_buff[256]; memset(send_buff, NULL, 256); int i = 0;

	buysell_index = GetByte(pBuf, index);		// Buy or sell?
	message_index = GetShort(pBuf, index);		// Which message should I retrieve? 

	if (buysell_index != MARKET_BBS_BUY && buysell_index != MARKET_BBS_SELL) goto fail_return;

	if (buysell_index == MARKET_BBS_BUY) {
		if (m_pMain->m_sBuyID[message_index] == -1) {
			sub_result = 3;
			goto fail_return;
		}

		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sBuyID[message_index]];
		if (!pUser) {	// Something wrong with the target ID.
			sub_result = 1;
			goto fail_return;
		}
	}
	else if (buysell_index == MARKET_BBS_SELL) {
		if (m_pMain->m_sSellID[message_index] == -1) {
			sub_result = 3;
			goto fail_return;
		}

		pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sSellID[message_index]];
		if (!pUser) {	// Something wrong with the target ID.
			sub_result = 1;
			goto fail_return;
		}
	}
	
	if ( m_pUserData->m_iGold >= REMOTE_PURCHASE_PRICE) {	// Check if user has gold.
		m_pUserData->m_iGold -= REMOTE_PURCHASE_PRICE;
		
		SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	
		SetByte( send_buff, 0x02, send_index );
		SetDWORD( send_buff, REMOTE_PURCHASE_PRICE, send_index );
		SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
		Send( send_buff, send_index );

		result = 1;
	}
	else {	// User does not have gold.
		sub_result = 2;
	}

fail_return:
	memset(send_buff, NULL, 256); send_index = 0;
	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_REMOTE_PURCHASE, send_index);
	SetByte(send_buff, buysell_index, send_index);
	SetByte(send_buff, result, send_index);

	if (result == 0) { // Only on errors!!!
		SetByte(send_buff, sub_result, send_index);
	}

	Send(send_buff, send_index);
}

void CUser::MarketBBSTimeCheck()
{
	CUser* pUser = NULL;	// Basic Initializations. 	
	int send_index = 0;			
	char send_buff[256]; memset(send_buff, NULL, 256);	
	float currenttime = 0.0f;
	int price = 0;

	currenttime = TimeGet();

	for (int i = 0 ; i < MAX_BBS_POST ; i++) {		
		if (m_pMain->m_sBuyID[i] != -1) {	// BUY!!!
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sBuyID[i]];			
			if (!pUser) {
				MarketBBSBuyDelete(i);
				continue;
			}
			
			if (m_pMain->m_fBuyStartTime[i] + BBS_CHECK_TIME < currenttime) {
				if (pUser->m_pUserData->m_iGold >= BUY_POST_PRICE) {
					pUser->m_pUserData->m_iGold -= BUY_POST_PRICE ;
					m_pMain->m_fBuyStartTime[i] = TimeGet();

					memset(send_buff, NULL, 256); send_index = 0;	
					SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// Now the target
					SetByte( send_buff, 0x02, send_index );
					SetDWORD( send_buff, BUY_POST_PRICE, send_index );
					SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
					pUser->Send( send_buff, send_index );	
				}
				else {
					MarketBBSBuyDelete(i);
				}
			}
		}
		
		if (m_pMain->m_sSellID[i] != -1) {	// SELL!!!
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[m_pMain->m_sSellID[i]];
			if (!pUser) {
				MarketBBSSellDelete(i);
				continue;
			}
		
			if (m_pMain->m_fSellStartTime[i] + BBS_CHECK_TIME < currenttime) {
				if (pUser->m_pUserData->m_iGold >= SELL_POST_PRICE) {
					pUser->m_pUserData->m_iGold -= SELL_POST_PRICE ;
					m_pMain->m_fSellStartTime[i] = TimeGet();

					memset(send_buff, NULL, 256); send_index = 0;
					SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// Now the target
					SetByte( send_buff, 0x02, send_index );
					SetDWORD( send_buff, SELL_POST_PRICE, send_index );
					SetDWORD( send_buff, pUser->m_pUserData->m_iGold, send_index );
					pUser->Send( send_buff, send_index );	
				}
				else {
					MarketBBSSellDelete(i);
				}
			}
		}
	}
}

void CUser::MarketBBSUserDelete()
{
	// 1. GÜVENLİK: m_pMain veya m_Sid kontrolü
	if (m_pMain == NULL || m_Sid < 0)
		return;

	for (int i = 0; i < MAX_BBS_POST; i++) {
		// 2. GÜVENLİK: Dizi sınır kontrolü (i'nin MAX_BBS_POST dışına çıkmadığından emin olun)
		if (i >= MAX_BBS_POST) break;

		if (m_pMain->m_sBuyID[i] == m_Sid) {	// BUY!!!
			MarketBBSBuyDelete(i);
		}

		if (m_pMain->m_sSellID[i] == m_Sid) {	// SELL!!
			MarketBBSSellDelete(i);
		}
	}
}


void CUser::MarketBBSBuyDelete(short index)
{
	m_pMain->m_sBuyID[index] = -1;
	memset( m_pMain->m_strBuyTitle[index], NULL, MAX_BBS_TITLE);
	memset( m_pMain->m_strBuyMessage[index], NULL, MAX_BBS_MESSAGE);
	m_pMain->m_iBuyPrice[index] = 0;
	m_pMain->m_fBuyStartTime[index] = 0.0f;	
}

void CUser::MarketBBSSellDelete(short index)
{
	m_pMain->m_sSellID[index] = -1;
	memset( m_pMain->m_strSellTitle[index], NULL, MAX_BBS_TITLE);
	memset( m_pMain->m_strSellMessage[index], NULL, MAX_BBS_MESSAGE);
	m_pMain->m_iSellPrice[index] = 0;
	m_pMain->m_fSellStartTime[index] = 0.0f;
}

void CUser::MarketBBSMessage(char *pBuf)
{
	int index = 0, send_index = 0;				
	BYTE result = 0; BYTE sub_result = 1; 
	char send_buff[256]; memset(send_buff, NULL, 256);

	short message_index = 0; BYTE buysell_index = 0;
	short message_length = 0;

	buysell_index = GetByte(pBuf, index);		// Buy or sell?
	message_index = GetShort(pBuf, index);		// Which message should I retrieve? 

	if (buysell_index != MARKET_BBS_BUY && buysell_index != MARKET_BBS_SELL) goto fail_return;

	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_MESSAGE, send_index);
	SetByte(send_buff, result, send_index);

	switch(buysell_index) {
		case MARKET_BBS_BUY:
			if (m_pMain->m_sBuyID[message_index] == -1) goto fail_return;

			message_length = strlen(m_pMain->m_strBuyMessage[message_index]);
			if (message_length > MAX_BBS_MESSAGE) {
				message_length = MAX_BBS_MESSAGE ;
			}

			SetShort(send_buff, message_length, send_index);
			SetString(send_buff, m_pMain->m_strBuyMessage[message_index], message_length, send_index);
			break;

		case MARKET_BBS_SELL:
			if (m_pMain->m_sSellID[message_index] == -1) goto fail_return;

			message_length = strlen(m_pMain->m_strSellMessage[message_index]);
			if (message_length > MAX_BBS_MESSAGE) {
				message_length = MAX_BBS_MESSAGE ;
			}

			SetShort(send_buff, message_length, send_index);
			SetString(send_buff, m_pMain->m_strSellMessage[message_index], message_length, send_index);
			break;
	}

	Send(send_buff, send_index);
	return;

fail_return:
	memset(send_buff, NULL, 256); send_index = 0;
	SetByte(send_buff, WIZ_MARKET_BBS, send_index);
	SetByte(send_buff, MARKET_BBS_MESSAGE, send_index);
	SetByte(send_buff, result, send_index);
	SetByte(send_buff, sub_result, send_index);
	Send(send_buff, send_index);
}

void CUser::MarketBBSBuyPostFilter()
{
	int empty_counter = 0;

	for (int i = 0 ; i < MAX_BBS_POST ; i++) {
		if (m_pMain->m_sBuyID[i] == -1) {	// BUY!!!
			empty_counter++;	
			continue;
		}

		if (empty_counter > 0) {
			if (m_pMain->m_sBuyID[i] != -1) {
				m_pMain->m_sBuyID[i - empty_counter] = m_pMain->m_sBuyID[i] ;
				strcpy_s(m_pMain->m_strBuyTitle[i- empty_counter], m_pMain->m_strBuyTitle[i]);
				strcpy_s(m_pMain->m_strBuyMessage[i- empty_counter], m_pMain->m_strBuyMessage[i]);
				m_pMain->m_iBuyPrice[i- empty_counter] = m_pMain->m_iBuyPrice[i];
				m_pMain->m_fBuyStartTime[i- empty_counter] = m_pMain->m_fBuyStartTime[i];	

				MarketBBSBuyDelete(i);
			}
		}
	}
}

void CUser::MarketBBSSellPostFilter()
{
	int empty_counter = 0;

	for (int i = 0 ; i < MAX_BBS_POST ; i++) {
		if (m_pMain->m_sSellID[i] == -1) {	// BUY!!!
			empty_counter++;	
			continue;
		}

		if (empty_counter > 0) {
			if (m_pMain->m_sSellID[i] != -1) {
				m_pMain->m_sSellID[i - empty_counter] = m_pMain->m_sSellID[i];
				strcpy_s( m_pMain->m_strSellTitle[i- empty_counter], m_pMain->m_strSellTitle[i]);
				strcpy_s( m_pMain->m_strSellMessage[i- empty_counter], m_pMain->m_strSellMessage[i]);
				m_pMain->m_iSellPrice[i- empty_counter] = m_pMain->m_iSellPrice[i];
				m_pMain->m_fSellStartTime[i- empty_counter] = m_pMain->m_fSellStartTime[i];	

				MarketBBSSellDelete(i);
			}
		}
	}
}

void CUser::BlinkTimeCheck(float currenttime)
{
	int send_index = 0;				
	char send_buff[256]; memset(send_buff, NULL, 256);

	if (BLINK_TIME < (currenttime - m_fBlinkStartTime)) {
		m_fBlinkStartTime = 0.0f;
		
		m_bAbnormalType = ABNORMAL_NORMAL;
//
		if (m_bRegeneType == REGENE_MAGIC) {
			HpChange(m_iMaxHp / 2);					// Refill HP.	
		}
		else {
			HpChange(m_iMaxHp);	
		}

		m_bRegeneType = REGENE_NORMAL;
//
		SetByte(send_buff, 3, send_index);
		SetByte(send_buff, m_bAbnormalType, send_index);
		StateChange(send_buff); 

		//printf("?? BlinkTimeCheck : name=%s(%d), type=%d ??\n", m_pUserData->m_id, m_Sid, m_bAbnormalType);
//
		// AI_server·Î regeneÁ¤º¸ Àü¼Û...	
		memset( send_buff, NULL, 256 ); send_index=0;
		SetByte( send_buff, AG_USER_REGENE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetShort( send_buff, m_pUserData->m_sHp, send_index );
		m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
//
//
		memset( send_buff, 0x00, 256 ); send_index=0;		// To AI Server....
		SetByte( send_buff, AG_USER_INOUT, send_index );
		SetByte( send_buff, USER_REGENE, send_index );
		SetShort( send_buff, m_Sid, send_index );
		SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
		SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
		Setfloat( send_buff, m_pUserData->m_curx, send_index );
		Setfloat( send_buff, m_pUserData->m_curz, send_index );
		m_pMain->Send_AIServer( m_pUserData->m_bZone, send_buff, send_index);
//
	}
}

void CUser::SetLogInInfoToDB(BYTE bInit)
{
	int index = 0, send_index = 0, retvalue = 0, addrlen = 20;
	char send_buff[256], strClientIP[20];
	memset( send_buff, NULL, 256); memset( strClientIP, 0x00, 20 );
	_ZONE_SERVERINFO *pInfo	= NULL;
	struct sockaddr_in addr;

	pInfo = m_pMain->m_ServerArray.GetData( m_pMain->m_nServerNo );
	if( !pInfo ) {
		CString logstr;
		logstr.Format("%d Server Info Invalid User Closed...\r\n", m_pMain->m_nServerNo );
		LogFileLogin( logstr );
		Close();
	}

	getpeername(m_Socket, (struct sockaddr*)&addr, &addrlen );
	strcpy_s( strClientIP, inet_ntoa(addr.sin_addr) );

	SetByte( send_buff, WIZ_LOGIN_INFO, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
	SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
	SetShort( send_buff, strlen(pInfo->strServerIP), send_index );
	SetString( send_buff, pInfo->strServerIP, strlen(pInfo->strServerIP), send_index );
	SetShort( send_buff, pInfo->sPort, send_index );
	SetShort( send_buff, strlen(strClientIP), send_index );
	SetString( send_buff, strClientIP, strlen(strClientIP), send_index );
	SetByte( send_buff, bInit, send_index );

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf_s( logstr, "UserInfo Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
}

void CUser::KickOut(char *pBuf)
{
	int idlen = 0, index = 0, send_index = 0;
	char accountid[MAX_ID_SIZE+1]; memset( accountid, 0x00, MAX_ID_SIZE+1 );
	char send_buff[256];		   memset( send_buff, 0x00, 256 );
	CUser* pUser = NULL;
	idlen = GetShort( pBuf, index );
	if( idlen > MAX_ID_SIZE || idlen <= 0)
		return;
	GetString( accountid, pBuf, idlen, index );

	pUser = m_pMain->GetUserPtr( accountid, 0x01 );
	if( pUser ) {
		pUser->UserDataSaveToAgent();
		pUser->Close();
	}
	else {
		SetByte( send_buff, WIZ_KICKOUT, send_index );
		SetShort( send_buff, idlen, send_index );
		SetString( send_buff, accountid, idlen, send_index );
		m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	}
}
// ¿©±â¼­ ºÎÅÍ Á¤¾Ö¾¾°¡ °í»ıÇÏ¸é¼­ ÇØÁÖ½Å Äù½ºÆ® ºÎºĞ....
void CUser::ClientEvent(char *pBuf)		// The main function for the quest procedures!!!
{										// (actually, this only takes care of the first event :(  )
	if( m_pMain->m_bPointCheckFlag == FALSE)	return;	// Æ÷ÀÎÅÍ ÂüÁ¶ÇÏ¸é ¾ÈµÊ           //

	int index = 0;
	CNpc* pNpc = NULL;
	int nid = 0, eventnum = 0;
	BYTE code = 0x00;
	EVENT* pEvent = NULL;
	EVENT_DATA* pEventData = NULL;
	int eventid = -1;

	nid = GetShort( pBuf, index );
	pNpc = m_pMain->m_arNpcArray.GetData(nid);
	if( !pNpc )	return;	   // Better to check than not to check ;)

	m_sEventNid = nid;     // ÀÌ°Ç ³ªÁß¿¡ ³»°¡ Ãß°¡ÇÑ °Å¿´½¿....

//	if( pNpc->m_byEvent < 0 ) return;      // ÀÌ°Å ÀÏ´Ü ÁÖ¼® Ã³¸® Àı´ë »©Áö¸¶!!		//

	pEvent = m_pMain->m_Event.GetData(m_pUserData->m_bZone);	                    //
	if(!pEvent)	return;																//

	pEventData = pEvent->m_arEvent.GetData(pNpc->m_byEvent);						//

	switch(pNpc->m_tNpcType) {
		case NPC_CLERIC:
			eventid = EVENT_POTION;
			break;

		case NPC_COUPON:
			eventid = EVENT_COUPON;
			break;

		case NPC_MONK_ELMORAD:
			eventid = EVENT_LOGOS_ELMORAD;
			break;

		case NPC_MONK_KARUS:
			eventid = EVENT_LOGOS_KARUS;
			break;
	}


//	pEventData = pEvent->m_arEvent.GetData(eventid);		// Make sure you change this later!!!	
	if(!pEventData) return;								                           
	
	if( !CheckEventLogic(pEventData) ) return;	// Check if all 'A's meet the requirements in Event #1

	list<EXEC*>::iterator	Iter;		        // Execute the 'E' events in Event #1
	for( Iter = pEventData->m_arExec.begin(); Iter != pEventData->m_arExec.end(); Iter++ ) {
		if( !RunNpcEvent( pNpc, (*Iter) ) ){
			return;
		}
	}
}

BOOL CUser::CheckEventLogic(EVENT_DATA *pEventData) 	// This part reads all the 'A' parts and checks if the 
{                                                       // requirements for the 'E' parts are met.
	if( !pEventData ) return FALSE;

	BOOL bExact = TRUE;

	list<LOGIC_ELSE*>::iterator	Iter;
	for( Iter = pEventData->m_arLogicElse.begin(); Iter != pEventData->m_arLogicElse.end(); Iter++ ) {
		bExact = FALSE;

		LOGIC_ELSE* pLE = (*Iter);
		if( !pLE ) return FALSE;

		switch( pLE->m_LogicElse ) {
		case	LOGIC_CHECK_UNDER_WEIGHT:
			if( pLE->m_LogicElseInt[0]+m_sItemWeight >= m_sMaxWeight )
				bExact = TRUE;
			break;

		case	LOGIC_CHECK_OVER_WEIGHT:
			if( pLE->m_LogicElseInt[0]+m_sItemWeight < m_sMaxWeight )
				bExact = TRUE;
			break;

		case	LOGIC_CHECK_SKILL_POINT:
			if( CheckSkillPoint(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) )
				bExact = TRUE;
			break;

		case	LOGIC_EXIST_ITEM:
			if ( CheckExistItem(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1]) )	bExact = TRUE;
			break;

		case	LOGIC_CHECK_CLASS:		
			if (CheckClass( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2],
				pLE->m_LogicElseInt[3], pLE->m_LogicElseInt[4], pLE->m_LogicElseInt[5])) {
				bExact = TRUE;
			}
			break;

		case	LOGIC_CHECK_WEIGHT:	
			if (!CheckWeight( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1])) {				
				bExact = TRUE;
			}
			break;	
// ºñ·¯¸Ó±Û º¹±Ç >.<		
		case	LOGIC_CHECK_EDITBOX:
			if (!CheckEditBox()) {
				bExact = TRUE;
			}
			break;

		case	LOGIC_RAND:
			if (CheckRandom(pLE->m_LogicElseInt[0])) {
				bExact = TRUE;
			}
			break;
//
// ºñ·¯¸Ó±Û ¿¢¼¿ >.<
		case	LOGIC_CHECK_LEVEL:		
			if( m_pUserData->m_bLevel >= pLE->m_LogicElseInt[0] && m_pUserData->m_bLevel <= pLE->m_LogicElseInt[1] ) {
				bExact = TRUE;
			}
			break;

		case	LOGIC_NOEXIST_COM_EVENT:
			if (!ExistComEvent(pLE->m_LogicElseInt[0])) {
				bExact = TRUE;
			}	
			break;

		case	LOGIC_EXIST_COM_EVENT:
			if (ExistComEvent(pLE->m_LogicElseInt[0])) {
				bExact = TRUE;
			}	
			break;

		case	LOGIC_HOWMUCH_ITEM:
			if ( CheckItemCount(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) ) {
				bExact = TRUE;
			}
			break;

		case	LOGIC_CHECK_NOAH:
			if ( m_pUserData->m_iGold >= pLE->m_LogicElseInt[0] && m_pUserData->m_iGold <= pLE->m_LogicElseInt[1] ) {
				bExact = TRUE;	
			}
			break;
//

//
/*
			case LOGIC_CHECK_NATION:
				if( pLE->m_LogicElseInt[0] == m_pUserData->m_bNation ) {
					bExact = TRUE;
				}
				break;

			case LOGIC_CHECK_LEVEL:		
				if( m_pUserData->m_bLevel >= pLE->m_LogicElseInt[0] && m_pUserData->m_bLevel <= pLE->m_LogicElseInt[1] ) {
					bExact = TRUE;
				}
				break;

			case LOGIC_NOEXIST_ITEM:	
				if (ItemCountChange(pLE->m_LogicElseInt[0], 1, 0) < 2) {
					bExact = TRUE;
				}
				break;

			case LOGIC_QUEST_END:	
				break;

			case LOGIC_QUEST_LOG:
				break;

			case LOGIC_CHECK_NOAH:
				if(m_pUserData->m_iGold >= pLE->m_LogicElseInt[0]) {
					bExact = TRUE;
				}
				break;

			case LOGIC_CHECK_RACE:
				if (pLE->m_LogicElseInt[0] == m_pUserData->m_bRace) {
					bExact = TRUE;
				}
				break;
///////// These logics are for the test quest.
			case LOGIC_EXIST_ITEM:
				if (CheckExistItem(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1])) {
					bExact = TRUE;
				}
				break;

			case LOGIC_CHECK_CLASS:		
				if (CheckClass( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2],
					pLE->m_LogicElseInt[3], pLE->m_LogicElseInt[4], pLE->m_LogicElseInt[5])) {
					bExact = TRUE;
				}
				break;

			case LOGIC_CHECK_WEIGHT:	
				if (CheckWeight( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1])) {				
					bExact = TRUE;
				}
				break;

			case LOGIC_CHECK_SKILLPOINT:
				if (CheckSkillPoint(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2])) {	
					bExact = TRUE;
				}
				break;
*/

			default:
				return FALSE;
		}

		if( !pLE->m_bAnd ) {	// OR Á¶°ÇÀÏ °æ¿ì bExact°¡ TRUEÀÌ¸é ÀüÃ¼°¡ TRUEÀÌ´Ù
			if(bExact) return TRUE;
		}
		else {					// AND Á¶°ÇÀÏ °æ¿ì bExact°¡ FALSEÀÌ¸é ÀüÃ¼°¡ FALSEÀÌ´Ù
			if(!bExact) return FALSE;
		}
	}

	return bExact;
}

BOOL CUser::RunNpcEvent(CNpc *pNpc, EXEC *pExec)	// This part executes all the 'E' lines!
{
	switch( pExec->m_Exec ) {
	case EXEC_SAY:
		SendNpcSay( pExec );
		break;

	case	EXEC_SELECT_MSG:
		SelectMsg( pExec );
		break;

	case	EXEC_RUN_EVENT:
		{
			EVENT* pEvent = NULL;
			EVENT_DATA* pEventData = NULL;				

			pEvent = m_pMain->m_Event.GetData(m_pUserData->m_bZone);		if(!pEvent)	break;
			pEventData = pEvent->m_arEvent.GetData(pExec->m_ExecInt[0]);	if(!pEventData) break;

			if( !CheckEventLogic(pEventData) )	break;

			list<EXEC*>::iterator	Iter;
			for( Iter = pEventData->m_arExec.begin(); Iter != pEventData->m_arExec.end(); Iter++ ) 
			{
				if( !RunNpcEvent( pNpc, (*Iter) ) )
				{
					return FALSE;
				}
			}
		}
		break;

	case	EXEC_GIVE_ITEM:
		if ( !GiveItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1]) )		return FALSE;
		break;

	case	EXEC_ROB_ITEM:
		if ( !RobItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1]) )		return FALSE;
		break;
//	ºñ·¯¸Ó±Û º¹±Ç >.<
	case	EXEC_OPEN_EDITBOX:
		OpenEditBox(pExec->m_ExecInt[1], pExec->m_ExecInt[2]);
		break;

	case	EXEC_GIVE_NOAH:
		GoldGain(pExec->m_ExecInt[0]);
		break;

	case	EXEC_LOG_COUPON_ITEM:
		LogCoupon(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		break;
//
// ºñ·¯¸Ó±Û ¿¢¼¿ >.<
	case	EXEC_SAVE_COM_EVENT:
		SaveComEvent(pExec->m_ExecInt[0]);
		break;
//
	case	EXEC_RETURN:
		return FALSE;
		break;
/*
		case EXEC_SAY:		
			break;

		case EXEC_SELECT_MSG:
			SelectMsg( pExec );
			break;

		case EXEC_RUN_EVENT:
			{
				EVENT* pEvent = NULL;
				pEvent = m_pMain->m_Quest.GetData(m_pUserData->m_bZone);		
				if(!pEvent)	break;

				EVENT_DATA* pEventData = NULL;
				pEventData = pEvent->m_arEvent.GetData(pExec->m_ExecInt[0]);
				if(!pEventData) break;

				if( !CheckEventLogic(pEventData) )	break;

				list<EXEC*>::iterator	Iter;
				for( Iter = pEventData->m_arExec.begin(); Iter != pEventData->m_arExec.end(); Iter++ ) {
					if( !RunNpcEvent( pNpc, (*Iter) ) ){
						return FALSE;
					}
				}
			}
			break;

		case EXEC_ROB_NOAH:
			GoldLose(pExec->m_ExecInt[0]);
			break;

		case EXEC_GIVE_QUEST:
			break;

		case EXEC_QUEST_END:		
			break;

		case EXEC_QUEST_SAVE:
			break;

		case EXEC_RETURN:
			return FALSE;

		case EXEC_GIVE_NOAH:
			GoldGain(pExec->m_ExecInt[0]);
/////// These events are for the test quest. ///////
		case EXEC_ROB_ITEM:		
			if (!RobItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1])) {
				return FALSE;	
			}
			break;

		case EXEC_GIVE_ITEM:	
			if (!GiveItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1])) {
				return FALSE;
			}
			break;
*/

		default:
			break;
	}

	return TRUE;
}

BOOL CUser::RunEvent(EVENT_DATA *pEventData)
{
	EXEC* pExec = NULL;
	list<EXEC*>::iterator	Iter;

	for( Iter = pEventData->m_arExec.begin(); Iter != pEventData->m_arExec.end(); Iter++ ) 
	{
		pExec = (*Iter);
		if( !pExec ) break;

		switch(pExec->m_Exec){
			case EXEC_SAY:
				SendNpcSay( pExec );
				break;

			case	EXEC_SELECT_MSG:
				SelectMsg( pExec );
				break;

			case	EXEC_RUN_EVENT:
				{
					EVENT* pEvent = NULL;
					EVENT_DATA* pEventData = NULL;				

					pEvent = m_pMain->m_Event.GetData(m_pUserData->m_bZone);
					if(!pEvent)	break;

					pEventData = pEvent->m_arEvent.GetData(pExec->m_ExecInt[0]);
					if(!pEventData) break;

					if( !CheckEventLogic(pEventData) )	break;

					if( !RunEvent(pEventData) ){
						return FALSE;
					}
				}
				break;

			case	EXEC_GIVE_ITEM:
				if ( !GiveItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1]) )
					return FALSE;
				break;

			case	EXEC_ROB_ITEM:
				if ( !RobItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1]) )
					return FALSE;
				break;
//	ºñ·¯¸Ó±Û º¹±Ç >.<
			case	EXEC_OPEN_EDITBOX:
				OpenEditBox(pExec->m_ExecInt[1], pExec->m_ExecInt[2]);
				break;

			case	EXEC_GIVE_NOAH:
				GoldGain(pExec->m_ExecInt[0]);
				break;

			case	EXEC_LOG_COUPON_ITEM:
				LogCoupon(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
				break;
//
// ºñ·¯¸Ó±Û ¿¢¼¿ >.<
			case	EXEC_SAVE_COM_EVENT:
				SaveComEvent(pExec->m_ExecInt[0]);
				break;

			case	EXEC_ROB_NOAH:
				GoldLose(pExec->m_ExecInt[0]);
				break;
//
			case	EXEC_RETURN:
				return FALSE;
				break;

/*
			case EXEC_SAY:		
				break;

			case EXEC_SELECT_MSG:
				SelectMsg( pExec );
				break;

			case EXEC_RUN_EVENT:
				{								
					EVENT* pEvent = NULL;
					pEvent = m_pMain->m_Quest.GetData(m_pUserData->m_bZone);
					if(!pEvent)	break;

					EVENT_DATA* pEventData = NULL;
					pEventData = pEvent->m_arEvent.GetData(pExec->m_ExecInt[0]);
					if(!pEventData) break;

					if( !CheckEventLogic(pEventData) )	break;

					if( !RunEvent(pEventData) ) {
						return FALSE;
					}
				}
				break;

			case EXEC_ROB_NOAH:
				break;

			case EXEC_GIVE_QUEST:
				break;

			case EXEC_QUEST_END:		
				break;

			case EXEC_QUEST_SAVE:
				break;

			case EXEC_RETURN:
				return FALSE;
/////// These events are for the test quest. ///////
			case EXEC_ROB_ITEM:
				if (!RobItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1])) {
					return FALSE;	
				}
				break;

			case EXEC_GIVE_ITEM:
				if (!GiveItem(pExec->m_ExecInt[0], pExec->m_ExecInt[1])) {
					return FALSE;
				}
				break;
*/

			default:
				break;
		}
	}

	return TRUE;
}
// Á¤¾Ö¾¾°¡ °í»ıÇÏ¸é¼­ ÇØÁÖ½Å Äù½ºÆ® ºÎºĞ ³¡
void CUser::TestPacket( char* pBuf )
{
	// npcÀÇ ¸®½ºÆ®¸¦ Àç ¿äÃ»ÇÏ´Â ±º,,,,,
	m_pMain->RegionNpcInfoForMe(this, 1);
	m_pMain->SyncTest(2);
}

void CUser::ItemLogToAgent(const char *srcid, const char *tarid, int type, __int64 serial, int itemid, int count, int durability)
{
	int send_index = 0, retvalue = 0;
	char send_buff[1024]; memset( send_buff, 0x00, 1024 );

	SetByte( send_buff, WIZ_ITEM_LOG, send_index );
	SetShort( send_buff, strlen(srcid), send_index );
	SetString( send_buff, (char*)srcid, strlen(srcid), send_index );
	SetShort( send_buff, strlen(tarid), send_index );
	SetString( send_buff, (char*)tarid, strlen(tarid), send_index );
	SetByte( send_buff, type, send_index );
	SetInt64( send_buff, serial, send_index );
	SetDWORD( send_buff, itemid, send_index );
	SetShort( send_buff, count, send_index );
	SetShort( send_buff, durability, send_index );

	retvalue = m_pMain->m_ItemLoggerSendQ.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf_s( logstr, "ItemLog Write Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
}

void CUser::SendItemWeight()
{
	int send_index = 0 ;
	char send_buff[256]; memset(send_buff, 0x00, 256);

	SetSlotItemValue();
	SetByte( send_buff, WIZ_WEIGHT_CHANGE, send_index );
	SetShort( send_buff, m_sItemWeight, send_index );
	Send(send_buff, send_index);
}

void CUser::GoldGain(int gold)	// 1 -> Get gold    2 -> Lose gold
{
	int send_index = 0;
	char send_buff[256]; memset( send_buff, 0x00, 256 );	
	
	m_pUserData->m_iGold += gold;	// Add gold.

	SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// First the source...
	SetByte( send_buff, 0x01, send_index );
	SetDWORD( send_buff, gold, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );	
}

BOOL CUser::GoldLose(int gold)
{
	int send_index = 0;
	char send_buff[256]; memset( send_buff, 0x00, 256 );	
	
	if (m_pUserData->m_iGold < gold) return FALSE;	// Insufficient gold!
	
	m_pUserData->m_iGold -= gold;	// Subtract gold.

	SetByte( send_buff, WIZ_GOLD_CHANGE, send_index );	// First the source...
	SetByte( send_buff, 0x02, send_index );
	SetDWORD( send_buff, gold, send_index );
	SetDWORD( send_buff, m_pUserData->m_iGold, send_index );
	Send( send_buff, send_index );	

	return TRUE;
}

BOOL CUser::CheckSkillPoint(BYTE skillnum, BYTE min, BYTE max)
{
	if (skillnum < 5 || skillnum > 8) return FALSE;

	if (m_pUserData->m_bstrSkill[skillnum] >= min && m_pUserData->m_bstrSkill[skillnum] <= max) {
		return TRUE;
	}
	else {
		return FALSE;
	}		

	return FALSE;
}

BOOL CUser::CheckWeight(int itemid, short count)
{
	_ITEM_TABLE* pTable = NULL;
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if (!pTable) return FALSE;

	if (!pTable->m_bCountable) {
		if ((m_sItemWeight + pTable->m_sWeight ) <= m_sMaxWeight) {		// Check weight first!
			if (GetEmptySlot(itemid, 0) != 0xFF) {		// Now check empty slots :P
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	else {
		if (((pTable->m_sWeight * count) + m_sItemWeight) <= m_sMaxWeight) {	// Check weight first!
			if (GetEmptySlot(itemid, pTable->m_bCountable) != 0xFF) {	// Now check empty slots :P
				return TRUE;
			}
			else {
				return FALSE;
			}			
		}
		else {
			return FALSE;
		}
	}

	return FALSE;
}

BOOL CUser::CheckExistItem(int itemid, short count)
{
	_ITEM_TABLE* pTable = NULL;				// This checks if such an item exists.
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) return FALSE;	

	for ( int i = 0 ; i < SLOT_MAX + HAVE_MAX ; i++ ) {		// Check every slot in this case.....
		if( m_pUserData->m_sItemArray[i].nNum == itemid ) {		
			if (!pTable->m_bCountable) {	// Non-countable item. Automatically return TRUE				
				return TRUE;
			}
			else {
				if (m_pUserData->m_sItemArray[i].sCount >= count) {	// Countable items. Make sure the amount is 
					return TRUE;                                    // same or higher.
				}
				else {
					return FALSE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CUser::RobItem(int itemid, short count)
{
	int send_index = 0;
	char send_buff[256]; memset(send_buff, 0x00, 256);
	BYTE type = 1;

	int i; // 🔥 BURAYA ALDIK

	_ITEM_TABLE* pTable = NULL;
	pTable = m_pMain->m_ItemtableArray.GetData(itemid);
	if (!pTable) return FALSE;

	for (i = SLOT_MAX; i < SLOT_MAX + HAVE_MAX * type; i++) {
		if (m_pUserData->m_sItemArray[i].nNum == itemid) {
			if (!pTable->m_bCountable) {
				m_pUserData->m_sItemArray[i].nNum = 0;
				m_pUserData->m_sItemArray[i].sCount = 0;
				m_pUserData->m_sItemArray[i].sDuration = 0;
				goto success_return;
			}
			else {
				if (m_pUserData->m_sItemArray[i].sCount >= count) {
					m_pUserData->m_sItemArray[i].sCount -= count;

					if (m_pUserData->m_sItemArray[i].sCount == 0) {
						m_pUserData->m_sItemArray[i].nNum = 0;
						m_pUserData->m_sItemArray[i].sCount = 0;
						m_pUserData->m_sItemArray[i].sDuration = 0;
					}
					goto success_return;
				}
				else {
					return FALSE;
				}
			}
		}
	}

	return FALSE;

success_return:
	SendItemWeight();
	SetByte(send_buff, WIZ_ITEM_COUNT_CHANGE, send_index);
	SetShort(send_buff, 0x01, send_index);
	SetByte(send_buff, 0x01, send_index);
	SetByte(send_buff, i - SLOT_MAX, send_index);
	SetDWORD(send_buff, itemid, send_index);
	SetDWORD(send_buff, m_pUserData->m_sItemArray[i].sCount, send_index);
	Send(send_buff, send_index);

	return TRUE;
}

BOOL CUser::GiveItem(int itemid, short count)
{
	int pos = 255;
	int send_index = 0 ;					
	char send_buff[128]; memset( send_buff, 0x00, 128 ) ;

	_ITEM_TABLE* pTable = NULL;				// This checks if such an item exists.
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) return FALSE;	
	
	pos = GetEmptySlot( itemid, pTable->m_bCountable );

	if( pos != 0xFF ) {	// Common Item
		if( pos >= HAVE_MAX ) return FALSE;

		if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != 0 ) {	
			if( pTable->m_bCountable != 1) return FALSE;
			else if( m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum != itemid ) return FALSE;
		}
/*	ÀÌ°Ç ÇÊ¿äÇÒ ¶§ ÁÖ¼® »©µµ·Ï....
		if (pTable->m_bCountable) {	// Check weight of countable item.
			if (((pTable->m_sWeight * count) + m_sItemWeight) > m_sMaxWeight) {			
				return FALSE;
			}
		}
		else {	// Check weight of non-countable item.
			if ((pTable->m_sWeight + m_sItemWeight) > m_sMaxWeight) {
				return FALSE;
			}
		}
*/
		m_pUserData->m_sItemArray[SLOT_MAX+pos].nNum = itemid;	// Add item to inventory. 

		if( pTable->m_bCountable) {	// Apply number of items to a countable item.
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount += count;
			if( m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount > MAX_ITEM_COUNT ) {
				m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = MAX_ITEM_COUNT;
			}
		}
		else {		// Just add uncountable item to inventory.
			m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount = 1;
		}
		
		m_pUserData->m_sItemArray[SLOT_MAX+pos].sDuration = pTable->m_sDuration;	// Apply duration to item.
	}
	else {
		return FALSE;	// No empty slots.
	}

	SendItemWeight();	// Change weight first :)
	SetByte( send_buff, WIZ_ITEM_COUNT_CHANGE, send_index );	
	SetShort( send_buff, 0x01, send_index );	// The number of for-loops
	SetByte( send_buff, 0x01, send_index );
	SetByte( send_buff, pos, send_index );
	SetDWORD( send_buff, itemid, send_index );	// The ID of item.
	SetDWORD( send_buff, m_pUserData->m_sItemArray[SLOT_MAX+pos].sCount, send_index );
	Send( send_buff, send_index );
	return TRUE;
}

BOOL CUser::CheckClass(short class1, short class2, short class3, short class4, short class5, short class6)
{
/*
	if (m_pUserData->m_sClass == class1 || m_pUserData->m_sClass == class2 || m_pUserData->m_sClass == class3 ||
		m_pUserData->m_sClass == class4 || m_pUserData->m_sClass == class5 || m_pUserData->m_sClass == class6 ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
*/
	if (JobGroupCheck(class1)) return TRUE;
	if (JobGroupCheck(class2)) return TRUE;
	if (JobGroupCheck(class3)) return TRUE;
	if (JobGroupCheck(class4)) return TRUE;
	if (JobGroupCheck(class5)) return TRUE;
	if (JobGroupCheck(class6)) return TRUE;
	return FALSE;
}

void CUser::RecvSelectMsg(char *pBuf)	// Receive menu reply from client.
{
	int index = 0, selevent, selnum;
	EVENT* pEvent = NULL;
	EVENT_DATA* pEventData = NULL;

	selnum = GetByte( pBuf, index );
//	if( selnum < 0 || selnum > 4 )
	if( selnum < 0 || selnum > MAX_MESSAGE_EVENT )		// ºñ·¯¸Ó±Û Äù½ºÆ® >.<
		goto fail_return;

	selevent = m_iSelMsgEvent[selnum];	// Get the event number that needs to be processed next.

	pEvent = m_pMain->m_Event.GetData(m_pUserData->m_bZone);
	if(!pEvent)	goto fail_return;

	pEventData = pEvent->m_arEvent.GetData(selevent);
	if(!pEventData) goto fail_return;

	if( !CheckEventLogic(pEventData) )	goto fail_return;

	if( !RunEvent(pEventData) )
	{
		goto fail_return;
	}

	return;

fail_return:
/*
	m_iSelMsgEvent[0] = -1;
	m_iSelMsgEvent[1] = -1;
	m_iSelMsgEvent[2] = -1;
	m_iSelMsgEvent[3] = -1;
	m_iSelMsgEvent[4] = -1;
*/

// ºñ·¯¸Ó±Û Äù½ºÆ® >.<
	for (int i = 0 ; i < MAX_MESSAGE_EVENT ; i++) {
		m_iSelMsgEvent[i] = -1;
	}
//
}

void CUser::SendNpcSay(EXEC *pExec)
{
	int i, send_index = 0;
	char send_buf[128];	memset(send_buf, NULL, 128);

	if( !pExec ) return;

	SetByte( send_buf, WIZ_NPC_SAY, send_index );
	for( i=0; i<10; i++) {	// It will be TEN for now!!!
		SetDWORD( send_buf, pExec->m_ExecInt[i], send_index );
	}

	Send( send_buf, send_index );
}

void CUser::SelectMsg(EXEC *pExec)
{
	int i, chat, send_index = 0;
	char send_buf[128];		memset(send_buf, NULL, 128);

	if( !pExec ) return;

	SetByte( send_buf, WIZ_SELECT_MSG, send_index );
	SetShort( send_buf, m_sEventNid, send_index );
//	SetByte( send_buf, (BYTE)pExec->m_ExecInt[0], send_index );		// NPC Á÷¾÷
	SetDWORD( send_buf, pExec->m_ExecInt[1], send_index );			// Áö¹® ¹øÈ£

	chat = 2;

//	for( i=0; i<4; i++ )	
	for( i = 0 ; i < MAX_MESSAGE_EVENT ; i++ ) {	// ºñ·¯¸Ó±Û Äù½ºÆ® >.<
		SetDWORD( send_buf, pExec->m_ExecInt[chat], send_index );
		chat += 2;
	}

	Send( send_buf, send_index );

/*
	m_iSelMsgEvent[0] = pExec->m_ExecInt[3];
	m_iSelMsgEvent[1] = pExec->m_ExecInt[5];
	m_iSelMsgEvent[2] = pExec->m_ExecInt[7];
	m_iSelMsgEvent[3] = pExec->m_ExecInt[9];
	m_iSelMsgEvent[4] = pExec->m_ExecInt[11];	
*/

// ºñ·¯¸Ó±Û Äù½ºÆ® >.<
	for (int j = 0 ; j < MAX_MESSAGE_EVENT ; j++) {
		m_iSelMsgEvent[j] = pExec->m_ExecInt[(2 * j) + 3];
	}
//
}

BOOL CUser::JobGroupCheck(short jobgroupid)
{
	if (jobgroupid < 100) {	// Check job groups
		switch(jobgroupid) {
			case GROUP_WARRIOR :
				if (m_pUserData->m_sClass == 101 || m_pUserData->m_sClass == 105 || m_pUserData->m_sClass == 106 || 
					m_pUserData->m_sClass == 201 || m_pUserData->m_sClass == 205 || m_pUserData->m_sClass == 206)
					return TRUE;
				break;
			case GROUP_ROGUE :	
				if (m_pUserData->m_sClass == 102 || m_pUserData->m_sClass == 107 || m_pUserData->m_sClass == 108 ||
					m_pUserData->m_sClass == 202 || m_pUserData->m_sClass == 207 || m_pUserData->m_sClass == 208)
					return TRUE;
				break;
			case GROUP_MAGE :			
				if (m_pUserData->m_sClass == 103 || m_pUserData->m_sClass == 109 || m_pUserData->m_sClass == 110 ||
					m_pUserData->m_sClass == 203 || m_pUserData->m_sClass == 209 || m_pUserData->m_sClass == 210)
					return TRUE;
				break;
			case GROUP_CLERIC :	
				if (m_pUserData->m_sClass == 104 || m_pUserData->m_sClass == 111 || m_pUserData->m_sClass == 112 ||
					m_pUserData->m_sClass == 204 || m_pUserData->m_sClass == 211 || m_pUserData->m_sClass == 212)
					return TRUE;
				break;
			case GROUP_ATTACK_WARRIOR :
				if (m_pUserData->m_sClass == 105 || m_pUserData->m_sClass == 205)
					return TRUE;
				break;
			case GROUP_DEFENSE_WARRIOR :
				if (m_pUserData->m_sClass == 106 || m_pUserData->m_sClass == 206)
					return TRUE;
				break;
			case GROUP_ARCHERER	:
				if (m_pUserData->m_sClass == 107 || m_pUserData->m_sClass == 207)
					return TRUE;
				break;
			case GROUP_ASSASSIN	:
				if (m_pUserData->m_sClass == 108 || m_pUserData->m_sClass == 208)
					return TRUE;
				break;
			case GROUP_ATTACK_MAGE :
				if (m_pUserData->m_sClass == 109 || m_pUserData->m_sClass == 209)
					return TRUE;
				break;
			case GROUP_PET_MAGE	:
				if (m_pUserData->m_sClass == 110 || m_pUserData->m_sClass == 210)
					return TRUE;
				break;
			case GROUP_HEAL_CLERIC :
				if (m_pUserData->m_sClass == 111 || m_pUserData->m_sClass == 211)
					return TRUE;
				break;
			case GROUP_CURSE_CLERIC	:			
				if (m_pUserData->m_sClass == 112 || m_pUserData->m_sClass == 212)
					return TRUE;
				break;
		}
	}
	else {	// Just check if the class is the same as the player's class
		if (m_pUserData->m_sClass == jobgroupid) {
			return TRUE;
		}
	}

	return FALSE;
}

void CUser::TrapProcess()		// À×...¼º¿ë¾¾ ¹Ì¿ö!!! ÈæÈæÈæ ¤Ğ.¤Ğ
{
	float currenttime = 0.0f;
	currenttime = TimeGet();

	if (ZONE_TRAP_INTERVAL < (currenttime - m_fLastTrapAreaTime)) {	// Time interval has passed :)
		HpChange( -ZONE_TRAP_DAMAGE );     // Reduce target health point.

		if( m_pUserData->m_sHp == 0) {    // Check if the target is dead.
			m_bResHpType = USER_DEAD;     // Target status is officially dead now.
		
			InitType3();	// Init Type 3.....
			InitType4();	// Init Type 4.....

			m_sWhoKilledMe = -1;		// Who the hell killed me?
		}
	} 

	m_fLastTrapAreaTime = currenttime;		// Update Last Trap Area time :)
}

void CUser::KickOutZoneUser(BOOL home, int nZoneID /*= 21 */)
{
	int zoneindex = -1, yourmama=0, random = 0;
	zoneindex = m_pMain->GetZoneIndex( m_pUserData->m_bNation );
	if( zoneindex < 0 ) return;

	_REGENE_EVENT* pRegene = NULL;

	C3DMap* pMap = NULL;
	pMap = m_pMain->m_ZoneArray[zoneindex];
	if( !pMap ) return;

	if(home) {
//		ZoneChange( pMap->m_nZoneNumber, pMap->m_fInitX, pMap->m_fInitZ );

// ºñ·¯¸Ó±Û ¹öÆÛ
		int random = myrand(0, 9000) ;
		if( random >= 0 && random < 3000 )			yourmama = 0;
		else if( random >= 3000 && random < 6000 )	yourmama = 1;
		else if( random >= 6000 && random < 9001 )	yourmama = 2;

		pRegene = m_pMain->m_ZoneArray[zoneindex]->GetRegeneEvent(yourmama) ;	
		if (!pRegene) {
			printf("### KickOutZoneUser Fail - user=%s, reg=%d, zoneindex=%d\n", m_pUserData->m_id, yourmama, zoneindex);
			KickOutZoneUser()	;
			return;
		}

		//printf("KickOutZoneUser - user=%s, reg=%d\n", m_pUserData->m_id, yourmama);

		int yourmama_x = myrand(0, pRegene->fRegeneAreaX) ;
		int yourmama_z = myrand(0, pRegene->fRegeneAreaZ) ;

		int x = pRegene->fRegenePosX + yourmama_x ;
		int y = pRegene->fRegenePosZ + yourmama_z ;

		ZoneChange(pMap->m_nZoneNumber, x, y);			
//
	}
	else {
		if (m_pUserData->m_bNation == KARUS) {
			ZoneChange( pMap->m_nZoneNumber, 1335, 83);	// Move user to native zone.
		}
		else {
			ZoneChange( pMap->m_nZoneNumber, 445, 1950 );	// Move user to native zone.
		}
	}
}

void CUser::EventMoneyItemGet( int itemid, int count )
{
/*
	int index = 0, send_index = 0, bundle_index = 0, itemid = 0, count = 0, i=0;
	BYTE pos;
	char send_buff[256];	memset( send_buff, NULL, 256 );
	C3DMap* pMap = NULL;
	CUser* pUser = NULL;
	CUser* pGetUser = NULL;
	_PARTY_GROUP* pParty = NULL;

	if( m_sExchangeUser != -1 ) goto fail_return;
	if( m_iZoneIndex < 0 || m_iZoneIndex >= m_pMain->m_ZoneArray.size() ) return;
		pMap = m_pMain->m_ZoneArray[m_iZoneIndex];
	if( !pMap )	goto fail_return;

	pGetUser = this;
*/
}

void CUser::NativeZoneReturn()
{
	_HOME_INFO* pHomeInfo = NULL;	// Send user back home in case it was the battlezone.
	pHomeInfo = m_pMain->m_HomeArray.GetData(m_pUserData->m_bNation);
	if (!pHomeInfo) return;

	m_pUserData->m_bZone = m_pUserData->m_bNation;

	if (m_pUserData->m_bNation == KARUS) {
		m_pUserData->m_curx = pHomeInfo->KarusZoneX + myrand(0, pHomeInfo->KarusZoneLX);
		m_pUserData->m_curz = pHomeInfo->KarusZoneZ + myrand(0, pHomeInfo->KarusZoneLZ); 
	}
	else {
		m_pUserData->m_curx = pHomeInfo->ElmoZoneX + myrand(0, pHomeInfo->ElmoZoneLX);
		m_pUserData->m_curz = pHomeInfo->ElmoZoneZ + myrand(0, pHomeInfo->ElmoZoneLZ); 
	}
}

BOOL CUser::CheckEditBox()
{
	char notepadid[MAX_COUPON_ID_LENGTH]; memset(notepadid, 0x00, MAX_COUPON_ID_LENGTH);
	char postitid[MAX_COUPON_ID_LENGTH]; memset(postitid, 0x00, MAX_COUPON_ID_LENGTH); 

	std::string buff; 
	::_LoadStringFromResource(IDS_COUPON_NOTEPAD_ID, buff);
	sprintf_s(notepadid, buff.c_str());
	
//	if( _strnicmp( notepadid, m_strCouponId, MAX_COUPON_ID_LENGTH ) == 0 ) {
	if ( !strcmp( notepadid, m_strCouponId ) ) {
		return TRUE;	
	}

	std::string buff1;
	::_LoadStringFromResource(IDS_COUPON_POSTIT_ID, buff1);
	sprintf_s(postitid, buff1.c_str());

//	if( _strnicmp( postitid, m_strCouponId, MAX_COUPON_ID_LENGTH ) == 0 ) {
	if ( !strcmp( postitid, m_strCouponId ) ) {
		return TRUE;	
	}

	return FALSE;
}

void CUser::OpenEditBox(int message, int event)
{
	//if( !CheckCouponUsed() ) return;	// ÀÌ°ÍÀº ±â¼úÁö¿ø ÇÊ¿äÇÔ ¤Ğ.¤Ğ

	// ÀÌ°ÍÀº ±â¼úÁö¿ø ÇÊ¿äÇÔ ¤Ğ.¤Ğ
	int send_index = 0, retvalue = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256);

	SetByte( send_buff, DB_COUPON_EVENT, send_index );
	SetByte( send_buff, CHECK_COUPON_EVENT, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetDWORD( send_buff, event, send_index );
//	ºñ·¯¸Ó±Û ´ë»ç¹® >.<
	SetDWORD( send_buff, message, send_index );
//	
	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf_s( logstr, "Coupon Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		//goto fail_return;
	}
/*
	return TRUE;

fail_return:
	send_index = 0;
	return FALSE;

	int i, send_index = 0;
	char send_buf[128];	memset(send_buf, NULL, 128);

	m_iEditBoxEvent = event;	// What will the next event be when an answer is given?

	SetByte( send_buf, WIZ_EDIT_BOX, send_index );
	Send( send_buf, send_index );	*/	
}

BOOL CUser::CheckRandom(short percent)
{
	if (percent < 0 || percent > 1000) return FALSE;

	int rand = 0;
	rand = myrand(0, 1000);
	
	if (percent > rand) return TRUE;

	return FALSE;
}

void CUser::RecvEditBox(char *pBuf)
{
	EVENT* pEvent = NULL;
	EVENT_DATA* pEventData = NULL;

	int index = 0; int selevent = -1; short coupon_length = 0; // char coupon_id[MAX_COUPON_ID_LENGTH];
	char send_buff[256]; memset( send_buff, NULL, 256 );

	coupon_length = GetShort(pBuf, index);		// Get length of coupon number.
	if (coupon_length < 0 || coupon_length > MAX_COUPON_ID_LENGTH) return;

	GetString(m_strCouponId, pBuf, coupon_length, index);		// Get actual coupon number.

	selevent = m_iEditBoxEvent;

	pEvent = m_pMain->m_Event.GetData(m_pUserData->m_bZone);	// ¿©±â¼­ ºÎÅÍ Áß¿äÇÔ --;
	if(!pEvent)	goto fail_return;

	pEventData = pEvent->m_arEvent.GetData(selevent);
	if(!pEventData) goto fail_return;

	if( !CheckEventLogic(pEventData) )	goto fail_return;

	if( !RunEvent(pEventData) )
	{
		goto fail_return;
	}

	return;	

fail_return:
	m_iEditBoxEvent = -1;
	memset(m_strCouponId, NULL, MAX_COUPON_ID_LENGTH);
}

BOOL CUser::CheckCouponUsed()
{
	// ÀÌ°ÍÀº ±â¼úÁö¿ø ÇÊ¿äÇÔ ¤Ğ.¤Ğ
	return TRUE;

}

void CUser::LogCoupon(int itemid, int count)
{
	// Âü°í·Î ÄíÆù ¹øÈ£´Â : m_iEditboxEvent
	// ÀÌ°ÍÀº ±â¼úÁö¿ø ÇÊ¿äÇÔ ¤Ğ.¤Ğ	
	int send_index = 0, retvalue = 0;
	char send_buff[256];
	memset( send_buff, NULL, 256);

	SetByte( send_buff, DB_COUPON_EVENT, send_index );
	SetByte( send_buff, UPDATE_COUPON_EVENT, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, strlen(m_strAccountID), send_index );
	SetString( send_buff, m_strAccountID, strlen(m_strAccountID), send_index );
	SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
	SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
	SetShort( send_buff, strlen(m_strCouponId), send_index );
	SetString( send_buff, m_strCouponId, strlen(m_strCouponId), send_index );
	SetDWORD( send_buff, itemid, send_index );
	SetDWORD( send_buff, count, send_index );
	
	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf_s( logstr, "Coupon Send Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
		//goto fail_return;
	}

}

void CUser::CouponEvent( char* pBuf )
{
	int index = 0, nEventNum=0, nItemCount=0, nResult = 0, nMessageNum = 0; 
	nResult = GetByte( pBuf, index );
	nEventNum = GetDWORD( pBuf, index );
// ºñ·¯¸Ó±Û ´ë»ç >.<
	nMessageNum = GetDWORD( pBuf, index );
//

	if( nResult == 0 ) return;

	// ¾Ë¾Æ¼­ »ç¿ë
	int send_index = 0;
	char send_buf[128];	memset(send_buf, NULL, 128);
	m_iEditBoxEvent = nEventNum;	// What will the next event be when an answer is given?
	SetByte( send_buf, WIZ_EDIT_BOX, send_index );
// ºñ·¯¸Ó±Û ´ë»ç >.<
	SetDWORD( send_buf, nMessageNum, send_index );
//
	Send( send_buf, send_index );	
}

BOOL CUser::CheckItemCount(int itemid, short min, short max)
{
	_ITEM_TABLE* pTable = NULL;				// This checks if such an item exists.
	pTable = m_pMain->m_ItemtableArray.GetData( itemid );
	if( !pTable ) return FALSE;	

	for ( int i = 0 ; i < SLOT_MAX + HAVE_MAX ; i++ ) {		// Check every slot in this case.....
		if( m_pUserData->m_sItemArray[i].nNum == itemid ) {		
			if (!pTable->m_bCountable) {	// Non-countable item. Automatically return TRUE				
				return FALSE;	// Let's return false in this case.
			}
			else {
				if (m_pUserData->m_sItemArray[i].sCount >= min && m_pUserData->m_sItemArray[i].sCount <= max) {	// Countable items. Make sure the amount is 
					return TRUE;                                    // same or higher.
				}
				else {
					return FALSE;
				}
			}
		}
	}

	return FALSE;		
}

void CUser::SaveComEvent(int eventid)
{
	for (int i = 0 ; i < MAX_CURRENT_EVENT ; i++) {
		if (m_sEvent[i] != eventid) {
			m_sEvent[i] = eventid;
			break;
		}
	}
}

BOOL CUser::ExistComEvent(int eventid)
{
	for (int i = 0 ; i < MAX_CURRENT_EVENT ; i++) {
		if (m_sEvent[i] == eventid) {
			return TRUE;
		}
	}	

	return FALSE;
}

void CUser::RecvDeleteChar( char* pBuf )
{
	int nResult = 0, nLen = 0, index = 0, send_index = 0, char_index = 0, nKnights = 0;
	char strCharID[MAX_ID_SIZE+1];	memset( strCharID, 0x00, MAX_ID_SIZE+1 );
	char send_buff[256];			memset( send_buff, 0x00, 256 );

	nResult = GetByte( pBuf, index );
	char_index = GetByte( pBuf, index );
	nKnights = GetShort( pBuf, index );
	nLen = GetShort( pBuf, index );
	GetString( strCharID, pBuf, nLen, index );

	if( nResult == 1 && nKnights != 0 )	{
		m_pMain->m_KnightsManager.RemoveKnightsUser( nKnights, strCharID );
		printf("RecvDeleteChar ==> name=%s, knights=%d\n", strCharID, nKnights );

		memset( send_buff, 0x00, 128 );		send_index = 0;
		SetByte( send_buff, UDP_KNIGHTS_PROCESS, send_index );
		SetByte( send_buff, KNIGHTS_WITHDRAW, send_index );
		SetShort( send_buff, nKnights, send_index );
		SetShort( send_buff, nLen, send_index );
		SetString( send_buff, strCharID, nLen, send_index );
		if( m_pMain->m_nServerGroup == 0 )
			m_pMain->Send_UDP_All( send_buff, send_index );
		else
			m_pMain->Send_UDP_All( send_buff, send_index, 1 );
	}

	memset( send_buff, 0x00, 128 );		send_index = 0;
	SetByte( send_buff, WIZ_DEL_CHAR, send_index );
	SetByte( send_buff, nResult, send_index );					// ¼º°ø½Ã ±¹°¡ Á¤º¸
	SetByte( send_buff, char_index, send_index );

	Send( send_buff, send_index );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void CUser::MerchantProcess(char *pBuf)
{
	int index = 0, i=0, send_index = 0, mLength = 0, chatlen = 0;
	char mText[40]; memset( mText, 0x00, 40 );
	char send_buff[1024]; memset( send_buff, NULL, send_index );

	BYTE type = 0x00;
	BYTE islot = 0x00;
	BYTE mslot = 0x00;
	BYTE bcount = 0x00;

	DWORD item = 0x00;
	DWORD gold = 0x00;

	SHORT count = 0x00;
	SHORT unknown = 0x00;
	SHORT duration = 0x00;

	type = GetByte( pBuf, index );
	switch( type ) {
		case MERCHANT_OPEN: // 0x01
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_OPEN, send_index );

			if( m_pUserData->m_bLevel < 30 )
				SetShort( send_buff, 30, send_index ); 
			else
				SetShort( send_buff, 1, send_index ); 

			Send( send_buff, send_index );
			break;
		case MERCHANT_CLOSE: // 0x02
			for(i=0; i<8; i++ ) {
				m_i8MerchantItem[i] = 0x00;
			}

			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_CLOSE, send_index );
			SetByte( send_buff, -1, send_index ); // 0x01
			SetByte( send_buff, -1, send_index );
			Send( send_buff, send_index );
			break;
		case MERCHANT_ITEM_ADD: // 0x03
			item = GetDWORD( pBuf, index );
			count = GetShort( pBuf, index );
			gold = GetDWORD( pBuf, index );
			islot = GetByte( pBuf, index );
			mslot = GetByte( pBuf, index );

			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_ITEM_ADD, send_index );

			m_i8MerchantItem[(int)mslot] = item;

			SetShort( send_buff, 0x01, send_index );
			SetDWORD( send_buff, item, send_index );
			SetShort( send_buff, count, send_index );
			SetShort( send_buff, duration, send_index );
			SetDWORD( send_buff, gold, send_index );
			SetByte( send_buff, islot, send_index );
			SetByte( send_buff, mslot, send_index );
			Send( send_buff, send_index );
			// UNKNOW (68)-->6803EEFF > Sealed items cannot be traded.
			break;
		case MERCHANT_ITEM_CANCEL: // 0x04
			mslot = GetByte( pBuf, index );

			m_i8MerchantItem[(int)mslot] = 0x00;

			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_ITEM_CANCEL, send_index );
			SetShort( send_buff, 0x01, send_index );
			SetByte( send_buff, mslot, send_index );
			Send( send_buff, send_index );
			break;
		case MERCHANT_ITEM_LIST: // 0x05
			// açıldığında görünecek itemlerin okutulması gerek
		/*	unknown = GetByte( pBuf, index );
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_ITEM_LIST, send_index );
			SetShort( send_buff, 0x01, send_index );
			SetShort( send_buff, unknown, send_index );
			for(i=0; i<12; i++ ) {
				SetDWORD( send_buff, item, send_index ); // m_4MerchantItem[i].num
				SetShort( send_buff, count, send_index ); // m_4MerchantItem[i].count
				SetShort( send_buff, duration, send_index ); // 4m_MerchantItem[i].duration
				SetDWORD( send_buff, gold, send_index ); // m_4MerchantItem[i].gold
				SetDWORD( send_buff, 0x00, send_index );
			}
			Send( send_buff, send_index ); */
			break;
		case MERCHANT_INSERT: // 0x07
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_INSERT, send_index );
			SetShort( send_buff, 0x01, send_index );

			mLength = GetShort( pBuf, index );
			GetString( mText, pBuf, mLength, index );

			SetShort( send_buff, mLength, send_index );
			SetString( send_buff, mText, mLength, send_index);

			SetShort( send_buff, m_Sid, send_index );
			SetByte( send_buff, 0x01, send_index ); // 0: Normal - 1: Gold

			for(i=0; i<8; i++ ) {
				SetDWORD( send_buff, m_i8MerchantItem[i], send_index );
			}

			Send( send_buff, send_index );
			break;
		case MERCHANT_CANCEL: // 0x08
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, MERCHANT_CANCEL, send_index );
			SetByte( send_buff, -1, send_index ); // 0x01
			SetByte( send_buff, -1, send_index );
			Send( send_buff, send_index );
			break;
		case BUY_MERCHANT_OPEN: // 0x21
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, BUY_MERCHANT_OPEN, send_index );
			SetByte( send_buff, 0x01, send_index );
			Send( send_buff, send_index );
			break;
		case BUY_MERCHANT_CREATE: // 0x22
			// 2 item > itemid > count > gold
			// SEND-->68+22+02+51934F09+0100+6F000000+F0D70D16+5000+6F000000
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, BUY_MERCHANT_CREATE, send_index );
			SetByte( send_buff, 0x01, send_index );
			Send( send_buff, send_index );
			break;
		case BUY_MERCHANT_CLOSE: // 0x27
			SetByte( send_buff, WIZ_MERCHANT, send_index );
			SetByte( send_buff, BUY_MERCHANT_CLOSE, send_index );
			SetByte( send_buff, -1, send_index ); // 0x01
			SetByte( send_buff, -1, send_index );
			Send( send_buff, send_index );
			break;
	}
}

void CUser::Helmet(char *pBuf)
{
	int index = 0, send_index = 0, type = 0;
	char send_buff[128]; memset( send_buff, NULL, 128);

	type = GetByte( pBuf, index );

	SetByte( send_buff, WIZ_HELMET, send_index );
	SetByte( send_buff, type, send_index );
	SetShort( send_buff, m_Sid, send_index );
	SetShort( send_buff, 0x00, send_index );
	Send( send_buff, send_index );
}

void CUser::MiningProcess(char *pBuf)
{
	int rate = 0, random = 0;
	rate = myrand(1, 100);
	random = myrand(1, 10000);

	int index = 0, send_index = 0, type = 0;
	char send_buff[1024]; memset( send_buff, NULL, 1024);

	type = GetByte( pBuf, index );

	switch( type ) {
		/////////////////////////////////////////////////
		// MINING
		case 0x01: // Mining Start
			SetByte( send_buff, WIZ_MINE, send_index );
			SetByte( send_buff, type, send_index );
			SetShort( send_buff, 0x01, send_index ); // 1- Continue 2- You are mining already
			SetShort( send_buff, m_Sid, send_index );
			Send( send_buff, send_index );
			break;
		case 0x02: // Mining Processing
			SetByte( send_buff, WIZ_MINE, send_index );
			SetByte( send_buff, type, send_index );
			SetShort( send_buff, 0x01, send_index );
			SetShort( send_buff, m_Sid, send_index );

			if( rate <= 50 && random <= 5000 ) {
				ExpChange( 1 );
				SetShort( send_buff, 13082, send_index );
			} else if( rate >= 50 && rate <= 75 && random <= 7500 ) { 
				GiveItem(389043000, 1); // Sling
				SetShort( send_buff, 13081, send_index );
			} else if( rate >= 75 && rate <= 100 && random <= 10000 ) {
				GiveItem(399210000, 1); // Mysterious Ore
				SetShort( send_buff, 13081, send_index );
			}

			Send( send_buff, send_index );
			break;
		case 0x03: // Mining Stop
			SetByte( send_buff, WIZ_MINE, send_index );
			SetByte( send_buff, type, send_index );
			SetShort( send_buff, 0x01, send_index );
			SetShort( send_buff, m_Sid, send_index );
			Send( send_buff, send_index );
			break;
		/////////////////////////////////////////////////
		// SOCCER
		case 0x10:
			// RECV-->WIZ_UNKNOW (86)-->86+10+03+00+00000000 ; start
			// RECV-->WIZ_UNKNOW (86)-->86+10+01+4704+01+02+02+00 ; 2 ye 0
			// RECV-->WIZ_UNKNOW (86)-->86+10+01+4704+01+02+0A+00 ; 10 a 0
			break;
	}
}

void CUser::SendPremium()
{
    int send_index = 0;
    char send_buff[256];
    memset( send_buff, NULL, 256);

    SetByte (send_buff,WIZ_PREMIUM,send_index);
	/*
	1- Days
	2- Premium Pc Room
	*/
	SetByte( send_buff, 1, send_index ); // Type
	/*
	1- Normal Premium
	2- Ultra Premium
	3- Bronze Premium
	4- Silver Premium
	5- Gold Premium
	6- Prime Premium
	7- Platinum Premium
	8- Royal Premium
	*/
	SetByte( send_buff, 7, send_index ); // pType

	SetDWORD( send_buff, 3, send_index ); // pDays

    Send( send_buff, send_index );
}

void CUser::EventJoin(char *pBuf)
{
	int index = 0, send_index = 0, type = 0, event = 0;
	char send_buff[1024]; memset( send_buff, NULL, send_index );
/* 5F
-- chaos geldi
RECV-->WIZ_UNKNOW (5F)-->5F+07+1800+5802
-- tıkladım
SEND-->WIZ_UNKNOW (5F)-->5F+08+1800
-- map eksik oldugundan hata
RECV-->WIZ_UNKNOW (5F)-->5F+08+03+1800 (You can not enter)

-- bdw geldi
RECV-->WIZ_UNKNOW (5F)-->5F+07+0400+5802

-- iptal
07 : açıldı
08 : katıl
09 : iptal
03 : type
0300 : event
SEND-->WIZ_UNKNOW (5F)-->5F+09+0300
RECV-->WIZ_UNKNOW (5F)-->5F+09+03+0300

1 ile 7 arası
*/
	type = GetByte( pBuf, index ); // -- 7 : Send Event -- 8 : Join Event -- 9 : Leave Event
	event = GetShort( pBuf, index ); // -- 4 : BDW -- 24 : Chaos
	switch( type ) {
		case 0x08: // Join
		case 0x09: // Leave
			SetByte( send_buff, WIZ_TEMPLE, send_index );
			SetByte( send_buff, type, send_index );
			SetByte( send_buff, 0x01, send_index ); // -- 1 : Join -- 3 : You can not enter, need map
			SetShort( send_buff, event, send_index );
			Send( send_buff, send_index );
			break;
	}
}

void CUser::KingProcess(char *pBuf)
{
	int index = 0, send_index = 0, type = 0, command = 0, i = 0;
	char send_buff[1024]; memset( send_buff, NULL, send_index );
	CUser* pUser = NULL;

// PRIZE
	int gold = 0, prizelen = 0;
	char prizeid[1024]; memset( prizeid, NULL, 1024 );
//
	char finalstr[1024]; memset( finalstr, NULL, 1024 );

	std::string buff;

	type = GetByte( pBuf, index );
	command = GetByte( pBuf, index );
	switch( type ) {
		case 0x04: // King Commands
			switch( command ) {
				case 0x03: // Prize
					if( m_pUserData->m_bTitle == 1 ) {
						gold = GetDWORD( pBuf, index );
						prizelen = GetByte( pBuf, index );
						if( prizelen <= 0 )
							return;
						GetString( prizeid, pBuf, prizelen, index );

						for(i=0; i<MAX_USER; i++ ) {
							pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[i];
							if( pUser && pUser->GetState() == STATE_GAMESTART ) {
								if( _strnicmp( prizeid, pUser->m_pUserData->m_id, MAX_ID_SIZE ) == 0 ) {

								if( m_pUserData->m_bNation == 1 )
									::_LoadStringFromResource(IDS_KING_KARUS_PRIZE_EVENT_MESSAGE, buff);
								else if( m_pUserData->m_bNation == 2 )
									::_LoadStringFromResource(IDS_KING_ELMO_PRIZE_EVENT_MESSAGE, buff);

								sprintf_s( finalstr, buff.c_str(), prizeid, gold );

								SetByte( send_buff, WIZ_CHAT, send_index );
								SetByte( send_buff, WAR_SYSTEM_CHAT, send_index );
								SetByte( send_buff, m_pUserData->m_bNation, send_index );
								SetByte( send_buff, -1, send_index );
								SetByte( send_buff, -1, send_index );
								SetByte( send_buff, 0x00, send_index );
								SetShort( send_buff, strlen(finalstr), send_index );
								SetString( send_buff, finalstr, strlen(finalstr), send_index );
								m_pMain->Send_All( send_buff, send_index );

								if( pUser->m_pUserData->m_iGold + gold >= sizeof(int) ) return;
								pUser->GoldGain( gold ); 

								}
							}
						}

					} else {
						SetByte( send_buff, WIZ_KING, send_index );
						SetByte( send_buff, type, send_index );
						SetByte( send_buff, -1, send_index );
						SetByte( send_buff, -1, send_index );
						Send( send_buff, send_index );
					}
					break;
			}
			break;
	}

/*
// prize
SEND-->WIZ_UNKNOW (78)-->7804031F000000046E65726F
RECV-->WIZ_GOLD_CHANGE-->4A011F000000AF323101
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF00460023232323204E4F54494345203A2054686520456C204D6F726164204B696E67206861732070616964204E65726F203331204E6F616873206173207265776172642E2023232323
// SEND-->WIZ_UNKNOW (78)-->78040300000000 + 05 + 74616D6572
// RECV-->WIZ_UNKNOW (78)-->7804FFFF (hata; user charla denedim cünkü)

////////// RATE
// experiencepoint (10 (percent)) [10 - 30 - 50]
SEND-->WIZ_UNKNOW (78)-->7804020A
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF00650023232323204E4F54494345203A2054686520456C204D6F726164204B696E672068617320696E6974696174656420746865203130202870657263656E742920696E6372656173656420657870657269656E636520706F696E74206576656E742E2023232323

// droprate (3 (percent) [1 ~ 3]
SEND-->WIZ_UNKNOW (78)-->78040103
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF00620023232323204E4F54494345203A2054686520456C204D6F726164204B696E672068617320696E69746961746564207468652033202870657263656E742920696E63726561736564206974656D2064726F702072617465206576656E742E2023232323

////////// HAVA
// clear (100)
SEND-->WIZ_UNKNOW (78)-->7804050164
RECV-->WIZ_WEATHER[WEATHER_FINE]-->14016400
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF003D0023232323204E4F54494345203A2054686520456C204D6F726164204B696E672068617320636C65617265642074686520776561746865722E2023232323
SEND-->WIZ_SPEEDHACK_CHECK-->410066E1C843

// rain (100)
// hata
SEND-->WIZ_UNKNOW (78)-->7804050200
RECV-->WIZ_UNKNOW (78)-->780405FEFF
// tama
SEND-->WIZ_UNKNOW (78)-->7804050264
RECV-->WIZ_WEATHER[WEATHER_RAIN]-->14026400
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF00420023232323204E4F54494345203A2054686520456C204D6F726164204B696E67206861732073756D6D6F6E656420746865207261696E20636C6F7564732E2023232323

// snow (100)
SEND-->WIZ_UNKNOW (78)-->7804050364
RECV-->WIZ_WEATHER[WEATHER_SNOW]-->14036400
RECV-->WIZ_CHAT[WAR_SYSTEM_CHAT]-->100801FFFF00410023232323204E4F54494345203A2054686520456C204D6F726164204B696E67206861732073756D6D6F6E65642074686520736E6F772073746F726D2E2023232323

/////////
// oylama npcsi acılırken Roban[Election Committee]
RECV-->WIZ_UNKNOW (78)-->7805+02+7632 (The current King is v2.)

// check election day
SEND-->WIZ_UNKNOW (78)-->780101
RECV-->WIZ_UNKNOW (78)-->78010101041B010F
// election for a king > nominate a king
SEND-->WIZ_UNKNOW (78)-->780101
RECV-->WIZ_UNKNOW (78)-->78010101041B010F
// election for a king > turn down nomination
SEND-->WIZ_UNKNOW (78)-->780105
RECV-->WIZ_UNKNOW (78)-->780105FDFF
// election for a king > nominee board
SEND-->WIZ_UNKNOW (78)-->78010305
RECV-->WIZ_UNKNOW (78)-->78010305FFFF (Its not time for voting)
// election for a king > election
SEND-->WIZ_UNKNOW (78)-->78010401
RECV-->WIZ_UNKNOW (78)-->78010401FFFF (Its not time for voting)

// impeachment > propose impeachment > propose impeachment
SEND-->WIZ_UNKNOW (78)-->780201
RECV-->WIZ_UNKNOW (78)-->780201FFFF (This is for senetaros only)
// impeachment > propose impeachment > impeachment result
SEND-->WIZ_UNKNOW (78)-->780208
RECV-->WIZ_UNKNOW (78)-->780208FFFF (Not taking votes for impeachment at the monument)
// impeachment > list of users in favor of i
SEND-->WIZ_UNKNOW (78)-->780203
RECV-->WIZ_UNKNOW (78)-->780203FFFF (Its not time for voting)
// impeachment > impeachment vote
SEND-->WIZ_UNKNOW (78)-->780209
RECV-->WIZ_UNKNOW (78)-->780209FFFF (Not taking votes for impeachment at the monument)

// kralın özel npcsi acılırken Raisa[Lord]
RECV-->WIZ_UNKNOW (78)-->7803010100319FCD3B84E55262
// kral olmayan charla açılırken
RECV-->WIZ_UNKNOW (78)-->78030102005C2C5700

// change national treasury
1.649.599.876 coins -- paket yok

// change tax rate
SEND-->WIZ_UNKNOW (78)-->780303
RECV-->WIZ_UNKNOW (78)-->780303010000
// tax rate ok (10%)
SEND-->WIZ_UNKNOW (78)-->7803040A
RECV-->WIZ_UNKNOW (78)-->78030401000A02
// ok tan sonra degisti tabi tekrar actıgımda tax rateyi
SEND-->WIZ_UNKNOW (78)-->780303
RECV-->WIZ_UNKNOW (78)-->78030301000A

// kings fund
1.003.331.337 
// kings fund ok
SEND-->WIZ_UNKNOW (78)-->780302
RECV-->WIZ_UNKNOW (78)-->78030201008058003D319FCD3B

// king item
SEND-->WIZ_UNKNOW (78)-->780307
RECV-->WIZ_WEIGHT_CHANGE-->54A402
RECV-->WIZ_ITEM_COUNT_CHANGE-->3D01000107C7A13E360100000000204E
RECV-->WIZ_UNKNOW (78)-->7803070100 ; item has been issued

214	IDS_KING_ELMO_FUGITIVE_EVENT_MESSAGE_1            	The El Morad King has paid %s %d Noahs as reward.                                                   
215	IDS_KING_KARUS_WEATHER_FINE_EVENT                 	The Karus King has cleared the weather.                                                             
216	IDS_KING_KARUS_WEATHER_RAIN_EVENT                 	The Karus King has summoned the rain clouds.                                                        
217	IDS_KING_KARUS_WEATHER_SNOW_EVENT                 	The Karus King has summoned the snow storm.                                                         
218	IDS_KING_ELMO_WEATHER_FINE_EVENT                  	The El Morad King has cleared the weather.                                                          
219	IDS_KING_ELMO_WEATHER_RAIN_EVENT                  	The El Morad King has summoned the rain clouds.                                                     
220	IDS_KING_ELMO_WEATHER_SNOW_EVENT                  	The El Morad King has summoned the snow storm.                                                      
221	IDS_KING_KARUS_NOAH_BONUS_EVENT                   	The Karus King has initiated the %d (percent) increased item drop rate event.                       
222	IDS_KING_KARUS_EXP_BONUS_EVENT                    	The Karus King has initiated the %d (percent) increased experience point event.                     
223	IDS_KING_ELMO_NOAH_BONUS_EVENT                    	The El Morad King has initiated the %d (percent) increased item drop rate event.                    
224	IDS_KING_ELMO_EXP_BONUS_EVENT                     	The El Morad King has initiated the %d (percent) increased experience point event.                  
225	IDS_KING_RECOMMEND_REQUEST_MESSAGE                	User %s can nominate a King. Please nominate a King.                                                
226	IDS_KING_CANDIDACY_RECOMMEND_MESSAGE              	%s has nominated %s as a King.                                                                      
227	IDS_KING_PERIOD_OF_RECOMMEND_MESSAGE              	This is a period for nominating a King. Users with nomination rights should nominate a King as soon 
228	IDS_KING_PERIOD_OF_ELECTION_MESSAGE               	This is an election period for selecting a King. Users that have not voted yet should vote for a Kin
229	IDS_KING_ELECTION_RESULT_MESSAGE                  	%d(percent) of the vote was won by %s to become a King.                                             
230	IDS_KING_EXP_BONUS_EVENT_STOP                     	The increased experience point event has ended.                                                     
231	IDS_KING_ITEMDROP_BONUS_EVENT_STOP                	The increased item drop rate event has ended.                                                       
232	IDS_KING_IMPEACHMENT_REQUEST_MESSAGE              	NULL
233	IDS_KING_IMPEACHMENT_PASS_MESSAGE                 	NULL
234	IDS_KING_IMPEACHMENT_REJECT_MESSAGE               	NULL
235	IDS_KING_IMPEACHMENT_ELECTION_MESSAGE             	NULL
236	IDS_KING_IMPEACHMENT_ELECTION_YES_RESULT_MESSAGE  	NULL
237	IDS_KING_IMPEACHMENT_ELECTION_NO_RESULT_MESSAGE   	NULL
204	IDS_KING_RECOMMEND_TIME                           	Please nominate a King.                                                                             
205	IDS_KING_RECOMMEND_FINISH_TIME                    	The period for nomination has ended.                                                                
206	IDS_KING_ELECTION_TIME                            	Election in progress for selecting a King.                                                          
207	IDS_KING_IMPEACHMENT_REQUEST_TIME                 	A request for an impeachment of King has been made.                                                 
208	IDS_KING_IMPEACHMENT_ELECTION_TIME                	election in progress for impeaching a King.                                                         
209	IDS_KING_REIGN_TIME                               	A new King has been appointed.                                                                      
210	IDS_KING_KARUS_PRIZE_EVENT_MESSAGE                	The Karus King has paid %s %d Noahs as reward.                                                      
211	IDS_KING_KARUS_FUGITIVE_EVENT_MESSAGE_1           	The Karus King has paid %s %d Noahs as reward.                                                      
212	IDS_KING_FUGITIVE_EVENT_MESSAGE_2                 	%s killed %s and received %d coins as a reward.                                                     
213	IDS_KING_ELMO_PRIZE_EVENT_MESSAGE                 	The El Morad King has paid %s %d Noahs as reward. */    
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void CUser::NewEvent(char *pBuf)
{
	int index = 0, send_index = 0, buff_index = 0, type = 0, command = 0, count = 0, i = 0;
	char send_buff[2048*4]; memset( send_buff, NULL, 2048*4);
	char buff[2048*4]; memset( buff, NULL, 2048*4);

	int itemaq = 0, itemid = 0, itemsl = 0, pwlen = 0;
	char pw[8]; memset( pw, NULL, 8 );

	type = GetByte( pBuf, index );
	switch( type ) {
		case 0x01:
			// Magic Anvil NPC
			break;
		case 0x02: // Item UPGRADE
			//
			break;
		case 0x03: // Compound UPGRADE
			//
			break;
		case 0x04:
			// Chaotic Generator NPC
			break;
		case 0x06: // Familiar hatching
			/*
			SEND-->WIZ_UNKNOW (5B)-->5B+06D832E849C32300+0500+74616D6572 (tamer)
			SEND-->WIZ_UNKNOW (5B)-->5B08FFFF0102030401
			RECV-->WIZ_UNKNOW (5B)-->5B+06+01+68E05B24+0042230000+0500+74616D6572+650100002823
			*/
			break;
		case 0x07: // Rebirth UPGRADE

			break;
		case 0x08: // Item seal/cancel
			command = GetByte( pBuf, index );
			switch( command ) {			
				case 0x01: // Item Seal
				case 0x02: // Item Unseal
					itemaq = GetShort( pBuf, index );
					itemid = GetDWORD( pBuf, index );
					itemsl = GetByte( pBuf, index );
					pwlen = GetShort( pBuf, index );
					GetString( pw, pBuf, pwlen, index );

					SetByte( send_buff, WIZ_NEW_EVENT, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );

				//	if( pwlen < 0 )
						SetByte( send_buff, 0x04, send_index ); // Citizen number error
				//	else
				//		SetByte( send_buff, 0x01, send_index ); // Success 

					SetDWORD( send_buff, itemid, send_index );
					Send( send_buff, send_index );
					break;
				case 0x03: // Krowaz item seal
					// SEND-->WIZ_UNKNOW (5B)-->5B0803+0000+F033C80E+(00)+0100+31 // (00) max_ slotu
					// item degisince eski iconu bugda kalıyor
					itemaq = GetShort( pBuf, index );
					itemid = GetDWORD( pBuf, index );
					itemsl = GetByte( pBuf, index );

					SetByte( send_buff, WIZ_NEW_EVENT, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );

					SetByte( send_buff, 0x01, send_index ); // Success 

					SetDWORD( send_buff, itemid, send_index );
					Send( send_buff, send_index );
					break;
			}
			break;	
		case 0x09: // Character seal
			// 01 = Seal aşama 1 (SEND-->WIZ_UNKNOW (5B)-->5B09010000)
			// 02 = Seal aşama 2 (SEND-->WIZ_UNKNOW (5B)-->5B0902+FFFF+0698B9B02F0300065441+0800+3030303030303030)
			
			//// Sistem
			// item konuldu gönderildi
			// 3 char geldi (id - level - class)
			// char seçildi yollandı ve şifre ile onaylanması geldi
			// şifre ile onayladıktan sonra cyber ring geldi
			command = GetByte( pBuf, index );
			switch( command ) {
				case 0x01: // Character list
					SetByte( send_buff, WIZ_NEW_EVENT, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, 0x01, send_index );

					// BOZUK
					for(i=0; i<3; i++ ) {
						SetShort( send_buff, strlen(m_pUserData->m_id), send_index );
						SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
						SetByte( send_buff, m_pUserData->m_bRace, send_index );
						SetByte( send_buff, m_pUserData->m_bLevel, send_index );
						SetShort( send_buff, m_pUserData->m_sClass, send_index );
					}

					Send( send_buff, send_index );
					break;
				case 0x04: // Cypher Ring
					// SEND-->WIZ_UNKNOW (5B)-->5B0904+370F+0000
					SetByte( send_buff, WIZ_NEW_EVENT, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, 0x01, send_index );
					SetByte( send_buff, strlen(m_pUserData->m_id), send_index );
					SetString( send_buff, m_pUserData->m_id, strlen(m_pUserData->m_id), send_index );
					SetByte( send_buff, m_pUserData->m_bNation, send_index );
					SetByte( send_buff, m_pUserData->m_bRace, send_index );
					SetShort( send_buff, m_pUserData->m_sClass, send_index );
					SetByte( send_buff, m_pUserData->m_bLevel, send_index );

					SetByte( send_buff, m_pUserData->m_bStr, send_index );
					SetByte( send_buff, m_pUserData->m_bSta, send_index );
					SetByte( send_buff, m_pUserData->m_bDex, send_index );
					SetByte( send_buff, m_pUserData->m_bIntel, send_index );
					SetByte( send_buff, m_pUserData->m_bCha, send_index );

					SetDWORD( send_buff, m_pUserData->m_iGold, send_index );

					for(i=0; i<9; i++ )
						SetByte( send_buff, m_pUserData->m_bstrSkill[i], send_index );

					for(i=0; i<SLOT_MAX+HAVE_MAX; i++ ) {
						SetDWORD( send_buff, m_pUserData->m_sItemArray[i].nNum, send_index );
						SetShort( send_buff, m_pUserData->m_sItemArray[i].sDuration, send_index );
						SetShort( send_buff, m_pUserData->m_sItemArray[i].sCount, send_index );
						SetByte( send_buff, 0x00, send_index );
					}

					Send( send_buff, send_index );
					break;
			}
			break;
	}
}

void CUser::QuestProcess(char *pBuf)
{
	int index = 0, send_index = 0, type = 0, command = 0;
	char send_buff[1024]; memset( send_buff, NULL, 1024);

	type = GetByte( pBuf, index );
	// 1, 8 ?
	switch( type ) {
		case 0x02: // Quest Seed

			break;
		case 0x05: // Quest Delete
				/*
				-- spool of silk
				SEND-->WIZ_UNKNOW (64)-->6405+07000000
				RECV-->WIZ_UNKNOW (64)-->6402+640000
				-- worm hunt
				SEND-->WIZ_UNKNOW (64)-->6405+31000000
				RECV-->WIZ_UNKNOW (64)-->6402+650000
				RECV-->WIZ_UNKNOW (64)-->6409+01000000000000
				-- item upgrade
				SEND-->WIZ_UNKNOW (64)-->6405+48000000
				RECV-->WIZ_UNKNOW (64)-->6402+680000
				*/
			break;
		case 0x07: // Quest Select
				/*
				SEND-->WIZ_UNKNOW (64)-->64073A0D0000 // werewolf
				SEND-->WIZ_UNKNOW (64)-->6407440D0000 // silan
				*/
			break;
		case 0x09: // Quest Change

			break;
	}
}

void CUser::LetterToAgent(char *pBuf)
{
	int send_index = 0, type = 0, command = 0, index = 0, i = 0, retvalue = 0;
	char send_buff[1024]; memset( send_buff, NULL, 1024);

	int id = 0, mailcount = 0, sIdLen = 0, titleLen = 0, itemId = 0, unknown1 = 0, unknown2 = 0, gold = 0, contentLen = 0;
	char sId[21]; char title[32]; char content[130]; char idm[20];
	memset( sId, NULL, 21 ); memset( title, NULL, 32 ); memset( content, NULL, 130 ); memset( idm, NULL, 20 );

	type = GetByte( pBuf, index );

	switch( type ) {
		case 0x01: // Power-up Store
			//
			break;
		case LETTER_L2:
			SetByte( send_buff, WIZ_LETTER, send_index );
			SetShort( send_buff, m_Sid, send_index );
			SetByte( send_buff, type, send_index );
			break;	
		case LETTER_L6:
			command = GetByte( pBuf, index );
			switch( command ) {
				case 0x01: // Control
				case 0x02: // New letter refresh
				case 0x03: // Saved letter refresh
					SetByte( send_buff, WIZ_LETTER, send_index );
					SetShort( send_buff, m_Sid, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					break;
				case 0x04: // Accept mail
				case 0x05: // Show mail
					id = GetDWORD( pBuf, index );

					SetByte( send_buff, WIZ_LETTER, send_index );
					SetShort( send_buff, m_Sid, send_index );
					SetByte( send_buff, LETTER_L6, send_index );
					SetByte( send_buff, command, send_index );
					SetDWORD( send_buff, id, send_index );
					break;
				case 0x06: // Send mail
//					sIdLen = GetByte( pBuf, index );
//					GetString( sId, pBuf, sIdLen, index );
//					titleLen = GetByte( pBuf, index );
//					GetString( title, pBuf, titleLen, index );
//					unknown1 = GetByte( pBuf, index );
//					itemId = GetDWORD( pBuf, index );
//					unknown2 = GetByte( pBuf, index );
//					gold = GetDWORD( pBuf, index );
//					contentLen = GetByte( pBuf, index );
//					GetString( content, pBuf, contentLen, index );

					SetByte( send_buff, WIZ_LETTER, send_index );
					SetShort( send_buff, m_Sid, send_index );
					SetByte( send_buff, LETTER_L6, send_index );
					SetByte( send_buff, command, send_index );

//					SetByte( send_buff, sIdLen, send_index);
//					SetString( send_buff, sId, sIdLen, send_index );
//					SetByte( send_buff, titleLen, send_index);
//					SetString( send_buff, title, titleLen, send_index );
//					SetByte( send_buff, unknown1, send_index);
//					SetDWORD( send_buff, itemId, send_index);
//					SetByte( send_buff, unknown2, send_index);
//					SetDWORD( send_buff, gold, send_index);
//					SetByte( send_buff, contentLen, send_index);
//					SetString( send_buff, content, contentLen, send_index );
					break;
				case 0x07: // Delete mail
					mailcount = GetByte( pBuf, index );
					for( i=0; i<mailcount; i++ ) {
						idm[i] = GetDWORD( pBuf, index );
					}

					SetByte( send_buff, WIZ_LETTER, send_index );
					SetShort( send_buff, m_Sid, send_index );
					SetByte( send_buff, LETTER_L6, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, mailcount, send_index );
					for( i=0; i<mailcount; i++ ) {
						SetDWORD( send_buff, idm[i], send_index );
					}
					break;
			}
			break;
	}

	retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
	if( retvalue >= SMQ_FULL ) {
		memset( send_buff, NULL, 256); send_index = 0;
		char logstr[256]; memset( logstr, 0x00, 256 );
		sprintf_s( logstr, "LetterToAgent Fail : %d", retvalue);
		m_pMain->m_StatusList.AddString(logstr);
	}
}

void CUser::CheckNewLetter(char *pBuf)
{
	int send_index = 0, type = 0, command = 0, index = 0, i = 0;
	char send_buff[1024]; memset( send_buff, NULL, 1024);

	int recv = 0, recv2 = 0, recv3 = 0;

	type = GetByte( pBuf, index );

	switch( type ) {
		case LETTER_L2:
			SetByte( send_buff, WIZ_LETTER, send_index );
			SetByte( send_buff, type, send_index );
			Send( send_buff, send_index );
			break;
		case LETTER_L6:
			command = GetByte( pBuf, index );
			switch( command ) {
				case 0x01: // Control
					recv = GetByte( pBuf, index );
					SetByte( send_buff, WIZ_LETTER, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, recv, send_index );
					Send( send_buff, send_index );
					break;
				case 0x02: // New letter refresh
				case 0x03: // Saved letter refresh
					recv = GetByte( pBuf, index );
					recv2 = GetByte( pBuf, index );
					SetByte( send_buff, WIZ_LETTER, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, recv, send_index );
					SetByte( send_buff, recv2, send_index ); // Mail count
					Send( send_buff, send_index );
					break;
				case 0x04: // Accept mail
					// RECV-->WIZ_UNKNOW (6A)-->6A06+04+01
					break;
				case 0x05: // Show mail
					// RECV-->WIZ_UNKNOW (6A)-->6A06+05+01+02000000+1B+Welcome to the test server!
					break;
				case 0x06: // Send mail
					recv = GetByte( pBuf, index );
					SetByte( send_buff, WIZ_LETTER, send_index );
					SetByte( send_buff, type, send_index );
					SetByte( send_buff, command, send_index );
					SetByte( send_buff, recv, send_index );
					Send( send_buff, send_index );
					// RECV-->WIZ_UNKNOW (6A)-->6A060601
					break;
				case 0x07: // Delete mail
					// RECV-->WIZ_UNKNOW (6A)-->6A060701
					break;
			}
			break;
	}
}
C3DMap* CUser::GetMap()
{
	if (m_pMain == nullptr)
		return nullptr;

	return m_pMain->GetZoneByID(m_pUserData->m_bZone);
}
/*
void SetKOString(char* buffer, const char* str, int& index, int lenType /*0=short,1=byte)
{
	int len = (int)strlen(str);

	if (lenType == 1)
		SetByte(buffer, (BYTE)len, index);
	else
		SetShort(buffer, (short)len, index);

	memcpy(buffer + index, str, len);
	index += len;
}
*/