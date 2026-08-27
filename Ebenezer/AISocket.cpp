// AISocket.cpp: implementation of the CAISocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "AISocket.h"
#include "EbenezerDlg.h"
#include "define.h"
#include "PacketDefine.h"
#include "AiPacket.h"
#include "Npc.h"
#include "user.h"
#include "Map.h"
#include "Compress.h"

extern CRITICAL_SECTION g_LogFile_critical;

CAISocket::CAISocket(int zonenum)
{
	m_iZoneNum = zonenum;
}

CAISocket::~CAISocket()
{

}

void CAISocket::Initialize()
{
	m_pMain = (CEbenezerDlg*)AfxGetApp()->GetMainWnd();
	m_MagicProcess.m_pMain = m_pMain;
}

void CAISocket::Parsing( int len, char* pData )
{
	int index = 0;

	BYTE command = GetByte(pData, index);

	//printf("Parsing - command=%d, length = %d\n", command, len);

	switch( command )
	{
		case AG_CHECK_ALIVE_REQ:
			RecvCheckAlive(pData+index);
			break;
		case AI_SERVER_CONNECT:
			LoginProcess(pData+index);
			break;
		case AG_SERVER_INFO:
			RecvServerInfo(pData+index);
			break;
		case NPC_INFO_ALL:
//			Sleep(730);
			RecvNpcInfoAll(pData+index);
			break;
		case MOVE_RESULT:
			RecvNpcMoveResult(pData+1);
			break;
		case MOVE_END_RESULT:
			break;
		case AG_ATTACK_RESULT:
			RecvNpcAttack(pData+1);
			break;
		case AG_MAGIC_ATTACK_RESULT:
			RecvMagicAttackResult(pData+1);
			break;
		case AG_NPC_INFO:
			RecvNpcInfo(pData+1);
			break;
		case AG_USER_SET_HP:
			RecvUserHP(pData+1);
			break;
		case AG_USER_EXP:
			RecvUserExp(pData+1);
			break;
		case AG_SYSTEM_MSG:
			RecvSystemMsg(pData+1);
			break;
		case AG_NPC_GIVE_ITEM:
			RecvNpcGiveItem(pData+1);
			break;
		case AG_USER_FAIL:
			RecvUserFail(pData+1);
			break;
		case AG_COMPRESSED_DATA:
			RecvCompressedData(pData+1);
			break;
		case AG_NPC_GATE_DESTORY:
			RecvGateDestory(pData+1);
			break;
		case AG_DEAD:
			RecvNpcDead(pData+1);
			break;
		case AG_NPC_INOUT:
			RecvNpcInOut(pData+1);
			break;
		case AG_BATTLE_EVENT:
			RecvBattleEvent(pData+1);
			break;
		case AG_NPC_EVENT_ITEM:
			RecvNpcEventItem(pData+1);
			break;
		case AG_NPC_GATE_OPEN:
			RecvGateOpen(pData+1);
			break;
	}
}

void CAISocket::CloseProcess()
{
	CString logstr;
	CTime time = CTime::GetCurrentTime();
	logstr.Format("*** CloseProcess - socketID=%d...  ***  %d-%d-%d, %d:%d]\r\n", m_Sid, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
	LogFileAISocket( logstr );

	Initialize();

	CIOCPSocket2::CloseProcess();
}

// sungyong 2002.05.23
void CAISocket::LoginProcess( char* pBuf )
{
	int index = 0;
	float fReConnectEndTime = 0.0f;
	BYTE ver = GetByte(pBuf, index);
	BYTE byReConnect = GetByte(pBuf, index);	// 0 : 처음접속, 1 : 재접속
	CString logstr;

	if(ver == -1)	// zone Error if incorrect 
	{
		AfxMessageBox(_T("AI Server Version Fail!!"));
	}
	else			// Error if incorrect
	{
		logstr.Format("AI Server Connect Success!! - %d", ver);
		m_pMain->m_StatusList.AddString( logstr );
		if( byReConnect == 0 )	{
			m_pMain->m_sSocketCount++;
			if(m_pMain->m_sSocketCount == MAX_AI_SOCKET)	{
				m_pMain->m_bServerCheckFlag = TRUE;
				m_pMain->m_sSocketCount = 0;
				printf("*** Preparation stage for sending user information ****\n");
				m_pMain->SendAllUserInfo();
			}
		}
		else if( byReConnect == 1 )	{
			if(m_pMain->m_sReSocketCount == 0)
			m_pMain->m_fReConnectStart = TimeGet();
			m_pMain->m_sReSocketCount++;
			printf("**** ReConnect - zone=%d,  socket = %d ****\n ", ver, m_pMain->m_sReSocketCount);
			fReConnectEndTime = TimeGet();
			if(fReConnectEndTime > m_pMain->m_fReConnectStart+120)	{	// 2분안에 모든 소켓이 재접됐다면...
				printf("**** ReConnect - simple connection... socket = %d ****\n ", m_pMain->m_sReSocketCount);
				m_pMain->m_sReSocketCount = 0;
				m_pMain->m_fReConnectStart = 0.0f;
			}

			if(m_pMain->m_sReSocketCount == MAX_AI_SOCKET)	{
				fReConnectEndTime = TimeGet();
				if(fReConnectEndTime < m_pMain->m_fReConnectStart+60)	{	// 1분안에 모든 소켓이 재접됐다면...
					printf("**** ReConnect - All socket initialization complete socket = %d ****\n ", m_pMain->m_sReSocketCount);
					m_pMain->m_bServerCheckFlag = TRUE;
					m_pMain->m_sReSocketCount = 0;
					printf("*** Preparation stage for sending user information ****\n");
					m_pMain->SendAllUserInfo();
				}
				else	{								// 하나의 떨어진 소켓이라면...
					m_pMain->m_sReSocketCount = 0;
					m_pMain->m_fReConnectStart = 0.0f;
				}
			}
		}
	}
}
/*
void CAISocket::RecvServerInfo(char* pBuf)
{
	int index = 0;
	BYTE type = GetByte(pBuf, index);
	BYTE byZone = GetByte(pBuf, index);
	CString logstr;
	int size = m_pMain->m_ZoneArray.size();

	if(type == SERVER_INFO_START)	{	
		printf("몬스터의 정보를 받기 시작합니다..%d\n", byZone);
	}
	else if(type == SERVER_INFO_END)	{
		short sTotalMonster = 0;
		sTotalMonster = GetShort(pBuf, index);
		m_pMain->m_StatusList.AddString("All Monster info Received!!");
		//Sleep(100);

		m_pMain->m_sZoneCount++;

		printf("몬스터의 정보를 다 받았음....%d, total=%d, socketcount=%d\n", byZone, sTotalMonster, m_pMain->m_sZoneCount);

		if(m_pMain->m_sZoneCount == size)	{
			if(m_pMain->m_bFirstServerFlag == FALSE)	{
				m_pMain->UserAcceptThread();
				printf("+++ 몬스터의 모든 정보를 다 받았음, User AcceptThread Start ....%d, socketcount=%d\n", byZone, m_pMain->m_sZoneCount);
			}
			m_pMain->m_sZoneCount = 0;
			m_pMain->m_bFirstServerFlag = TRUE;
			m_pMain->m_bPointCheckFlag = TRUE;
			printf("몬스터의 모든 정보를 다 받았음, User AcceptThread Start ....%d, socketcount=%d\n", byZone, m_pMain->m_sZoneCount);
			// 여기에서 Event Monster의 포인터를 미리 할당 하도록 하장~~
			//InitEventMonster( sTotalMonster );
		}
	}
}
*/
BYTE GetByteSafe(char* pBuf, int& index, int bufSize)
{
	if (index + sizeof(BYTE) > bufSize) return 0;
	return pBuf[index++];
}

short GetShortSafe(char* pBuf, int& index, int bufSize)
{
	if (index + sizeof(short) > bufSize) return 0;
	short val = *(short*)(pBuf + index);
	index += sizeof(short);
	return val;
}
void CAISocket::RecvServerInfo(char* pBuf)
{
	int index = 0;
	int totalZones = (int)m_pMain->m_ZoneArray.size(); // Yüklenen toplam harita sayısı (20)

	BYTE type = GetByte(pBuf, index);
	BYTE byZone = GetByte(pBuf, index); // Bu Zone ID'sidir (1, 2, 201 vb.)

	if (type == SERVER_INFO_START) {
		printf("Ebenezer: NPC info receiving START zone=%d\n", byZone);
	}
	else if (type == SERVER_INFO_END) {
		short sTotalMonster = GetShort(pBuf, index);

		m_pMain->m_StatusList.AddString("All Monster info Received!!");

		// KRİTİK DÜZELTME: Zone ID'sine (201) bakma, sadece bir haritanın bittiğini say.
		m_pMain->m_sZoneCount++;

		printf("Ebenezer: all monsters received zone=%d, total=%d, socketcount=%d/%d\n",
			byZone, sTotalMonster, m_pMain->m_sZoneCount, totalZones);

		// Tüm haritaların NPC bilgileri geldiyse (veya sayaç dolduysa) girişi aç
		if (m_pMain->m_sZoneCount >= totalZones && !m_pMain->m_bFirstServerFlag) {
			printf("Ebenezer: All zones loaded (%d/%d). Starting UserAcceptThread...\n", m_pMain->m_sZoneCount, totalZones);
			m_pMain->UserAcceptThread();
			m_pMain->m_bFirstServerFlag = TRUE;
			m_pMain->m_bPointCheckFlag = TRUE;
		}
	}
}

// ai server에 처음 접속시 npc의 모든 정보를 받아온다..
/*
void CAISocket::RecvNpcInfoAll(char* pBuf)
{
	if (!m_pMain)
		return;

	int index = 0;

	BYTE byCount = GetByte(pBuf, index);

	printf("[EB] NPC ALL START count=%d\n", byCount);

	if (byCount == 0 || byCount > 200)
	{
		printf("[EB] PACKET CORRUPT count=%d\n", byCount);
		return;
	}

	for (int i = 0; i < byCount; i++)
	{
		BYTE byType = GetByte(pBuf, index);

		short nid = GetShort(pBuf, index);
		short sid = GetShort(pBuf, index);
		short sPid = GetShort(pBuf, index);
		short sSize = GetShort(pBuf, index);

		int iweapon_1 = GetDWORD(pBuf, index);
		int iweapon_2 = GetDWORD(pBuf, index);

		short sZone = GetShort(pBuf, index);
		short sZoneIndex = GetShort(pBuf, index);

		// =========================
		// NAME SAFE READ
		// =========================
		char szName[MAX_ID_SIZE + 1];
		memset(szName, 0, sizeof(szName));

		BYTE nameLen = GetByte(pBuf, index);

		if (nameLen > MAX_ID_SIZE)
			nameLen = MAX_ID_SIZE;

		if (nameLen > 0)
		{
			memcpy(szName, pBuf + index, nameLen);
			szName[nameLen] = 0;
			index += nameLen;
		}

		BYTE byGroup = GetByte(pBuf, index);
		BYTE byLevel = GetByte(pBuf, index);

		// =========================
		// POSITION
		// =========================
		short sPosX = GetShort(pBuf, index);
		short sPosZ = GetShort(pBuf, index);
		short sPosY = GetShort(pBuf, index);

		float fPosX = sPosX / 10.0f;
		float fPosZ = sPosZ / 10.0f;
		float fPosY = sPosY / 10.0f;

		if (fPosY <= 0.0f)
			fPosY = 1.0f;

		BYTE tNpcType = GetByte(pBuf, index);

		DWORD iMaxHP = GetDWORD(pBuf, index);
		DWORD iHP = GetDWORD(pBuf, index);

		BYTE byGateOpen = GetByte(pBuf, index);
		short sHitRate = GetShort(pBuf, index);
		BYTE byObjectType = GetByte(pBuf, index);

		printf("[EB] NPC RECV nid=%d name=%.20s pos(%.1f,%.1f,%.1f)\n",
			nid, szName, fPosX, fPosY, fPosZ);

		// =========================
		// VALIDATION
		// =========================
		if (nid <= 0 || sPid <= 0)
			continue;

		if (sZoneIndex < 0 || sZoneIndex >= (int)m_pMain->m_ZoneArray.size())
			continue;

		C3DMap* pMap = m_pMain->m_ZoneArray[sZoneIndex];
		if (!pMap)
			continue;

		// =========================
		// GET / CREATE NPC
		// =========================
		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);

		if (!pNpc)
		{
			pNpc = new CNpc();
			pNpc->Initialize();

			pNpc->m_pMain = m_pMain;
			pNpc->m_pMap = pMap;

			if (!m_pMain->m_arNpcArray.PutData(nid, pNpc))
			{
				delete pNpc;
				continue;
			}

			printf("[EB] NPC CREATED nid=%d\n", nid);
		}

		// =========================
		// ASSIGN DATA
		// =========================
		pNpc->m_sNid = nid;
		pNpc->m_sSid = sid;
		pNpc->m_sPid = sPid;
		pNpc->m_sSize = (sSize <= 0 ? 100 : sSize);

		pNpc->m_iWeapon_1 = iweapon_1;
		pNpc->m_iWeapon_2 = iweapon_2;

		// 🔥 SAFE NAME COPY (CRASH FIX)
		if (szName[0] == 0)
		{
			strncpy_s(pNpc->m_strName, MAX_ID_SIZE, "UNKNOWN", _TRUNCATE);
		}
		else
		{
			strncpy_s(pNpc->m_strName, MAX_ID_SIZE, szName, _TRUNCATE);
		}

		pNpc->m_byGroup = byGroup;
		pNpc->m_byLevel = byLevel;

		pNpc->m_sCurZone = sZone;
		pNpc->m_sZoneIndex = sZoneIndex;

		pNpc->m_fCurX = fPosX;
		pNpc->m_fCurZ = fPosZ;
		pNpc->m_fCurY = fPosY;

		pNpc->m_tNpcType = tNpcType;

		pNpc->m_iMaxHP = iMaxHP;
		pNpc->m_iHP = iHP;

		pNpc->m_byGateOpen = byGateOpen;
		pNpc->m_sHitRate = sHitRate;
		pNpc->m_byObjectType = byObjectType;

		pNpc->m_NpcState = NPC_LIVE;

		// =========================
		// REGION CALC
		// =========================
		int nRegX = (int)(fPosX / VIEW_DISTANCE);
		int nRegZ = (int)(fPosZ / VIEW_DISTANCE);

		// SAFE BOUND CHECK
		if (nRegX < 0) nRegX = 0;
		if (nRegZ < 0) nRegZ = 0;

		pNpc->m_sRegion_X = nRegX;
		pNpc->m_sRegion_Z = nRegZ;

		pMap->RegionNpcAdd(nRegX, nRegZ, nid);

		printf("[EB] REGION OK nid=%d reg(%d,%d)\n", nid, nRegX, nRegZ);
	}

	printf("[EB] NPC ALL END\n");
}
*/
/*
void CAISocket::RecvNpcInfoAll(char* pBuf)
{
	int index = 0;

	BYTE byCount = GetByte(pBuf, index);

	printf("RecvNpcInfoAll START count=%d\n", byCount);

	for (int i = 0; i < byCount; i++)
	{
		CNpc* pNpc = new CNpc();
		if (!pNpc)
			continue;

		pNpc->Initialize();

		// =====================================================
		// BASIC INFO (client NPCIn order)
		// =====================================================
		pNpc->m_NpcState = GetByte(pBuf, index);

		pNpc->m_sNid = GetShort(pBuf, index);
		pNpc->m_sSid = GetShort(pBuf, index);

		if (pNpc->m_sNid <= 0)
		{
			printf("INVALID NPC nid=%d SKIP\n", pNpc->m_sNid);
			delete pNpc;
			continue;
		}

		pNpc->m_sSize = GetShort(pBuf, index);

		pNpc->m_iWeapon_1 = GetDWORD(pBuf, index);
		pNpc->m_iWeapon_2 = GetDWORD(pBuf, index);

		// =====================================================
		// NAME (CLIENT SAFE)
		// =====================================================
		int name_len = GetByte(pBuf, index);

		if (name_len < 0 || name_len > MAX_ID_SIZE)
		{
			printf("NAME CORRUPT nid=%d len=%d\n", pNpc->m_sNid, name_len);
			delete pNpc;
			continue;
		}

		memset(pNpc->m_strName, 0, sizeof(pNpc->m_strName));
		GetString(pNpc->m_strName, pBuf, name_len, index);
		pNpc->m_strName[name_len] = 0;

		// =====================================================
		// LEVEL
		// =====================================================
		pNpc->m_byLevel = GetByte(pBuf, index);

		// =====================================================
		// POSITION (IMPORTANT)
		// =====================================================
		pNpc->m_fCurX = Getfloat(pBuf, index);
		pNpc->m_fCurZ = Getfloat(pBuf, index);
		pNpc->m_fCurY = Getfloat(pBuf, index);

		// =====================================================
		// CLIENT CORE DATA
		// =====================================================
		pNpc->m_byDirection = GetByte(pBuf, index);
		pNpc->m_tNpcType = GetByte(pBuf, index);

		pNpc->m_iSellingGroup = GetDWORD(pBuf, index);
		pNpc->m_iMaxHP = GetDWORD(pBuf, index);
		pNpc->m_iHP = GetDWORD(pBuf, index);

		pNpc->m_byGateOpen = GetByte(pBuf, index);

		float fHitRate = Getfloat(pBuf, index);
		float fEvasion = Getfloat(pBuf, index);

		pNpc->m_sAc = GetShort(pBuf, index);
		pNpc->m_sHitRate = GetShort(pBuf, index);

		pNpc->m_byObjectType = GetByte(pBuf, index);
		pNpc->m_byTrapNumber = GetByte(pBuf, index);
		pNpc->m_byMonster = GetByte(pBuf, index);

		pNpc->m_sSocketID = GetShort(pBuf, index);

		pNpc->m_sFireR = GetShort(pBuf, index);
		pNpc->m_sColdR = GetShort(pBuf, index);
		pNpc->m_sLightningR = GetShort(pBuf, index);
		pNpc->m_sMagicR = GetShort(pBuf, index);
		pNpc->m_sDiseaseR = GetShort(pBuf, index);
		pNpc->m_sPoisonR = GetShort(pBuf, index);

		// =====================================================
		// ZONE FIX (IMPORTANT)
		// =====================================================
		if (pNpc->m_sCurZone <= 0 || pNpc->m_sCurZone >= (int)m_pMain->m_ZoneArray.size())
		{
			printf("ZONE INVALID nid=%d zone=%d -> FIX=1\n",
				pNpc->m_sNid, pNpc->m_sCurZone);

			pNpc->m_sCurZone = 1;
		}

		pNpc->m_sZoneIndex = pNpc->m_sCurZone;
		pNpc->m_pMap = m_pMain->m_ZoneArray[pNpc->m_sCurZone];

		if (!pNpc->m_pMap)
		{
			printf("MAP NULL nid=%d zone=%d\n",
				pNpc->m_sNid, pNpc->m_sCurZone);

			delete pNpc;
			continue;
		}

		// =====================================================
		// REGION CALC
		// =====================================================
		int rx = (int)(pNpc->m_fCurX / VIEW_DISTANCE);
		int rz = (int)(pNpc->m_fCurZ / VIEW_DISTANCE);

		pNpc->m_sRegion_X = rx;
		pNpc->m_sRegion_Z = rz;

		// =====================================================
		// ADD NPC
		// =====================================================
		if (!m_pMain->m_arNpcArray.PutData(pNpc->m_sNid, pNpc))
		{
			delete pNpc;
			continue;
		}

		if (pNpc->m_NpcState == NPC_DEAD)
			continue;

		printf("NPC OK nid=%d name=%s zone=%d x=%.2f z=%.2f rx=%d rz=%d\n",
			pNpc->m_sNid,
			pNpc->m_strName,
			pNpc->m_sCurZone,
			pNpc->m_fCurX,
			pNpc->m_fCurZ,
			rx, rz);

		if (pNpc->m_pMap)
		{
			pNpc->m_pMap->RegionNpcAdd(rx, rz, pNpc->m_sNid);
		}
	}
}
*/

void CAISocket::RecvNpcInfoAll(char* pBuf)
{
	int index = 0;

	BYTE byCount = GetByte(pBuf, index);

	printf("RecvNpcInfoAll START count=%d\n", byCount);

	for (int i = 0; i < byCount; i++)
	{
		CNpc* pNpc = new CNpc();

		if (pNpc == NULL)
			continue;

		pNpc->Initialize();

		// =====================================================
		// STATE
		// =====================================================
		pNpc->m_NpcState = GetByte(pBuf, index);

		// =====================================================
		// NID
		// =====================================================
		pNpc->m_sNid = GetShort(pBuf, index);

		// =====================================================
		// SERVER SERIAL ID + CLIENT RESOURCE/PICTURE ID
		// AIServer sends both fields.  They must not be collapsed:
		// m_sSid identifies the server NPC instance, while m_sPid is
		// the 1298 client resource ID used by NPC_Looks.
		// =====================================================
		pNpc->m_sSid = GetShort(pBuf, index);
		pNpc->m_sPid = GetShort(pBuf, index);

		if (pNpc->m_sNid <= 0)
		{
			printf("INVALID NPC nid=%d\n", pNpc->m_sNid);

			delete pNpc;
			continue;
		}

		// =====================================================
		// NPC TYPE
		// =====================================================
		pNpc->m_tNpcType = GetByte(pBuf, index);

		// =====================================================
		// SELL GROUP
		// =====================================================
		pNpc->m_iSellingGroup = GetDWORD(pBuf, index);

		// =====================================================
		// SIZE
		// =====================================================
		pNpc->m_sSize = GetShort(pBuf, index);

		// =====================================================
		// WEAPONS
		// =====================================================
		pNpc->m_iWeapon_1 = GetDWORD(pBuf, index);
		pNpc->m_iWeapon_2 = GetDWORD(pBuf, index);

		// =====================================================
		// NAME
		// =====================================================
		int name_len = GetByte(pBuf, index);

		if (name_len <= 0 || name_len > MAX_ID_SIZE)
		{
			printf("NAME ERROR nid=%d len=%d\n",
				pNpc->m_sNid,
				name_len);

			delete pNpc;
			continue;
		}

		memset(pNpc->m_strName, 0, sizeof(pNpc->m_strName));

		GetString(pNpc->m_strName, pBuf, name_len, index);

		pNpc->m_strName[name_len] = 0;

		// =====================================================
		// GROUP
		// =====================================================
		pNpc->m_byGroup = GetByte(pBuf, index);

		// =====================================================
		// LEVEL
		// =====================================================
		pNpc->m_byLevel = GetByte(pBuf, index);

		// =====================================================
		// POSITION
		// =====================================================
		pNpc->m_fCurX = (float)GetShort(pBuf, index) / 10.0f;
		pNpc->m_fCurZ = (float)GetShort(pBuf, index) / 10.0f;
		pNpc->m_fCurY = (float)GetShort(pBuf, index) / 10.0f;

		// ZONE
		pNpc->m_sCurZone = GetShort(pBuf, index);
		pNpc->m_sZoneIndex = m_pMain->GetZoneIndex(pNpc->m_sCurZone);
		if (pNpc->m_sZoneIndex < 0)
		{
			printf("ZONE INDEX NOT FOUND nid=%d zone=%d\n", pNpc->m_sNid, pNpc->m_sCurZone);
			delete pNpc;
			continue;
		}

		// =====================================================
		// GATE OPEN
		// =====================================================
		pNpc->m_byGateOpen = (BYTE)GetDWORD(pBuf, index);

		// =====================================================
		// OBJECT TYPE
		// =====================================================
		pNpc->m_byObjectType = GetByte(pBuf, index);

		// =====================================================
		// UNKNOWNS
		// =====================================================
		GetShort(pBuf, index);
		GetShort(pBuf, index);

		// =====================================================
		// DIRECTION
		// =====================================================
		pNpc->m_byDirection = GetByte(pBuf, index);

		// =====================================================
		// DEFAULT VALUES
		// =====================================================
		pNpc->m_iHP = 100;
		pNpc->m_iMaxHP = 100;
		

		// =====================================================
		// MAP CHECK
		// =====================================================
		int zoneIndex = m_pMain->GetZoneIndex(pNpc->m_sCurZone);

		if (zoneIndex < 0)
		{
			printf("ZONE NOT FOUND FIX nid=%d zone=%d\n",
				pNpc->m_sNid,
				pNpc->m_sCurZone);

			delete pNpc;
			continue;
		}

		pNpc->m_pMap = m_pMain->m_ZoneArray[zoneIndex];

		if (pNpc->m_pMap == NULL)
		{
			printf("MAP NULL nid=%d zone=%d\n",
				pNpc->m_sNid,
				pNpc->m_sCurZone);

			delete pNpc;
			continue;
		}

		// =====================================================
		// REGION
		// =====================================================
		pNpc->m_sRegion_X = (int)(pNpc->m_fCurX / VIEW_DISTANCE);
		pNpc->m_sRegion_Z = (int)(pNpc->m_fCurZ / VIEW_DISTANCE);

		// =====================================================
		// NPC ARRAY
		// =====================================================
		if (!m_pMain->m_arNpcArray.PutData(pNpc->m_sNid, pNpc))
		{
			printf("NPC ARRAY FAIL nid=%d\n",
				pNpc->m_sNid);

			delete pNpc;
			continue;
		}

		// =====================================================
		// DEAD NPC SKIP
		// =====================================================
		if (pNpc->m_NpcState == NPC_DEAD)
			continue;

		// =====================================================
		// REGION ADD
		// =====================================================
		pNpc->m_pMap->RegionNpcAdd(
			pNpc->m_sRegion_X,
			pNpc->m_sRegion_Z,
			pNpc->m_sNid);

		// =====================================================
		// DEBUG
		// =====================================================
		printf(
			"NPC OK nid=%d sid=%d name=%s lvl=%d x=%.1f z=%.1f rx=%d rz=%d\n",
			pNpc->m_sNid,
			pNpc->m_sSid,
			pNpc->m_strName,
			pNpc->m_byLevel,
			pNpc->m_fCurX,
			pNpc->m_fCurZ,
			pNpc->m_sRegion_X,
			pNpc->m_sRegion_Z);
	}
}
void CAISocket::RecvNpcInfo(char* pBuf)
{

		int index = 0;

		BYTE Mode = GetByte(pBuf, index);

		short nid = GetShort(pBuf, index);
		short sid = GetShort(pBuf, index);
		short sPid = GetShort(pBuf, index);

		short sSize = GetShort(pBuf, index);

		int iWeapon_1 = GetDWORD(pBuf, index);
		int iWeapon_2 = GetDWORD(pBuf, index);

		// ❌ CLIENTTA YOK → REMOVE ZONE READ
		 short sZone = GetShort(pBuf, index);
		 short sZoneIndex = GetShort(pBuf, index);

		char szName[MAX_ID_SIZE + 1];
		memset(szName, 0, sizeof(szName));

		int nLength = GetByte(pBuf, index);
		if (nLength <= 0 || nLength > MAX_ID_SIZE)
			return;

		GetString(szName, pBuf, nLength, index);
		szName[nLength] = 0;

		BYTE byGroup = GetByte(pBuf, index);
		BYTE byLevel = GetByte(pBuf, index);

		// POSITION
		float fPosX = Getfloat(pBuf, index);
		float fPosZ = Getfloat(pBuf, index);
		float fPosY = Getfloat(pBuf, index);

		BYTE byDirection = GetByte(pBuf, index);
		BYTE tNpcKind = GetByte(pBuf, index);

		int iSellingGroup = GetDWORD(pBuf, index);

		int nMaxHP = GetDWORD(pBuf, index);
		int nHP = GetDWORD(pBuf, index);

		BYTE byGateOpen = GetByte(pBuf, index);

		float fHitRate = Getfloat(pBuf, index);
		float fEvasion = Getfloat(pBuf, index);

		short sAc = GetShort(pBuf, index);
		short sHitRate = GetShort(pBuf, index);

		BYTE byObjectType = GetByte(pBuf, index);

		// =====================================================
		// FIND NPC
		// =====================================================
		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);
		if (!pNpc)
			return;

		// =====================================================
		// UPDATE
		// =====================================================
		pNpc->m_NpcState = NPC_LIVE;

		pNpc->m_sNid = nid;
		pNpc->m_sSid = sid;
		pNpc->m_sSize = sSize;

		pNpc->m_iWeapon_1 = iWeapon_1;
		pNpc->m_iWeapon_2 = iWeapon_2;

		strcpy(pNpc->m_strName, szName);

		pNpc->m_byGroup = byGroup;
		pNpc->m_byLevel = byLevel;

		pNpc->m_fCurX = fPosX;
		pNpc->m_fCurZ = fPosZ;
		pNpc->m_fCurY = fPosY;

		pNpc->m_fDir = byDirection;

		pNpc->m_tNpcType = tNpcKind;

		pNpc->m_iSellingGroup = iSellingGroup;

		pNpc->m_iMaxHP = nMaxHP;
		pNpc->m_iHP = nHP;

		pNpc->m_byGateOpen = byGateOpen;
		pNpc->m_sHitRate = sHitRate;
		pNpc->m_byObjectType = byObjectType;

		// =====================================================
		// SAFE ZONE RESOLVE (CRITICAL FIX)
		// =====================================================
		if (pNpc->m_sCurZone <= 0)
		{
			// MAP'TEN AL
			if (pNpc->m_pMap)
				pNpc->m_sCurZone = pNpc->m_pMap->m_nZoneNumber;
			else
				pNpc->m_sCurZone = 1;
		}

		pNpc->m_sZoneIndex = pNpc->m_sCurZone;

		// =====================================================
		// MAP ASSIGN SAFE
		// =====================================================
		if (pNpc->m_sCurZone >= 0 && pNpc->m_sCurZone < (int)m_pMain->m_ZoneArray.size())
			pNpc->m_pMap = m_pMain->m_ZoneArray[pNpc->m_sCurZone];
		else
			pNpc->m_pMap = NULL;

		if (!pNpc->m_pMap)
			return;

		// =====================================================
		// REGION
		// =====================================================
		int rx = (int)(fPosX / VIEW_DISTANCE);
		int rz = (int)(fPosZ / VIEW_DISTANCE);

		pNpc->m_sRegion_X = rx;
		pNpc->m_sRegion_Z = rz;

		// =====================================================
		// ADD REGION
		// =====================================================
		pNpc->m_pMap->RegionNpcAdd(rx, rz, pNpc->m_sNid);
	}

/*
void CAISocket::RecvNpcInfo(char* pBuf)
{
	if (!m_pMain)
	{
		printf("[EB] ERROR: m_pMain NULL\n");
		return;
	}

	int index = 0;

	// =========================
	// HEADER
	// =========================
	BYTE Mode = GetByte(pBuf, index);

	short nid = GetShort(pBuf, index);
	short sid = GetShort(pBuf, index);
	short sPid = GetShort(pBuf, index);
	short sSize = GetShort(pBuf, index);

	int iWeapon_1 = GetDWORD(pBuf, index);
	int iWeapon_2 = GetDWORD(pBuf, index);

	short sZone = GetShort(pBuf, index);
	short sZoneIndex = GetShort(pBuf, index);

	printf("[EB] NPC RECV nid=%d sid=%d pid=%d zone=%d zoneIndex=%d mode=%d\n",
		nid, sid, sPid, sZone, sZoneIndex, Mode);

	// =========================
	// NAME
	// =========================
	char szName[MAX_ID_SIZE + 1];
	memset(szName, 0, sizeof(szName));

	BYTE nameLen = GetByte(pBuf, index);

	if (nameLen > MAX_ID_SIZE)
		nameLen = MAX_ID_SIZE;

	if (nameLen > 0)
	{
		memcpy(szName, pBuf + index, nameLen);
		index += nameLen;
	}

	BYTE byGroup = GetByte(pBuf, index);
	BYTE byLevel = GetByte(pBuf, index);

	// =========================
	// POSITION
	// =========================
	float fPosX = GetShort(pBuf, index) / 10.0f;
	float fPosZ = GetShort(pBuf, index) / 10.0f;
	float fPosY = GetShort(pBuf, index) / 10.0f;

	BYTE tState = GetByte(pBuf, index);
	BYTE tNpcKind = GetByte(pBuf, index);

	short sDummy1 = GetShort(pBuf, index);
	short sDummy2 = GetShort(pBuf, index);

	short sHitRate = GetShort(pBuf, index);
	BYTE byDirection = GetByte(pBuf, index);

	// =========================
	// DEBUG PACKET STATE
	// =========================
	printf("[EB] DATA name=%s pos=(%.1f,%.1f,%.1f) weapon1=%d weapon2=%d index=%d\n",
		szName, fPosX, fPosY, fPosZ, iWeapon_1, iWeapon_2, index);

	// =========================
	// VALIDATION
	// =========================
	if (sZoneIndex < 0 || sZoneIndex >= (short)m_pMain->m_ZoneArray.size())
	{
		printf("[EB][ERROR] INVALID ZONE INDEX=%d nid=%d\n", sZoneIndex, nid);
		return;
	}

	C3DMap* pMap = m_pMain->m_ZoneArray[sZoneIndex];

	if (!pMap)
	{
		printf("[EB][ERROR] MAP NULL zoneIndex=%d nid=%d\n", sZoneIndex, nid);
		return;
	}

	// =========================
	// NPC GET OR CREATE
	// =========================
	CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);

	if (!pNpc)
	{
		printf("[EB] CREATE NPC nid=%d name=%s\n", nid, szName);

		pNpc = new CNpc();
		m_pMain->m_arNpcArray.PutData(nid, pNpc);
	}

	// =========================
	// ASSIGN DATA
	// =========================
	pNpc->m_sNid = nid;
	pNpc->m_sSid = sid;
	pNpc->m_sPid = sPid;
	pNpc->m_sSize = (sSize <= 0 ? 100 : sSize);

	pNpc->m_iWeapon_1 = iWeapon_1;
	pNpc->m_iWeapon_2 = iWeapon_2;

	strcpy_s(pNpc->m_strName, szName);

	pNpc->m_byGroup = byGroup;
	pNpc->m_byLevel = byLevel;

	pNpc->m_sCurZone = sZone;
	pNpc->m_sZoneIndex = sZoneIndex;

	pNpc->m_fCurX = fPosX;
	pNpc->m_fCurZ = fPosZ;
	pNpc->m_fCurY = fPosY;

	pNpc->m_fDir = (float)byDirection;

	pNpc->m_NpcState = tState;
	pNpc->m_tNpcType = tNpcKind;

	pNpc->m_sHitRate = sHitRate;

	// =========================
	// REGION CALC
	// =========================
	pNpc->m_sRegion_X = (short)(fPosX / VIEW_DISTANCE);
	pNpc->m_sRegion_Z = (short)(fPosZ / VIEW_DISTANCE);

	printf("[EB] REGION nid=%d (%d,%d)\n",
		nid, pNpc->m_sRegion_X, pNpc->m_sRegion_Z);

	// =========================
	// ADD TO MAP REGION
	// =========================
	pMap->RegionNpcAdd(
		pNpc->m_sRegion_X,
		pNpc->m_sRegion_Z,
		nid
	);

	printf("[EB] NPC REGISTERED SUCCESS nid=%d name=%s\n",
		nid, szName);

	// =========================
	// IMPORTANT NOTE
	// =========================
	// ❌ Send_Region KESİNLİKLE YOK
	// ❌ AI already sends to client
	// ✔ Only region registration here
}
*/

// ~sungyong 2002.05.23

void CAISocket::RecvNpcMoveResult(char *pBuf)
{
	// sungyong tw
	char send_buff[256];	memset( send_buff, 0x00, 256 );
	int index = 0, send_index = 0;
	BYTE		flag;			// 01(INFO_MODIFY)	: NPC 정보 변경
								// 02(INFO_DELETE)	: NPC 정보 삭제
	short		nid;			// NPC index
	float		fPosX;			// X Position
	float		fPosZ;			// Z Position
	float		fPosY;			// Y Position
	float		fSecForMetor;	// Sec당 metor
	flag = GetByte(pBuf,index);
	nid = GetShort(pBuf,index);
	fPosX = Getfloat(pBuf, index);
	fPosZ = Getfloat(pBuf, index);
	fPosY = Getfloat(pBuf, index);
	fSecForMetor = Getfloat(pBuf, index);

	CNpc* pNpc = m_pMain->m_arNpcArray.GetData( nid );
	if(!pNpc)
		return;

	if( pNpc->m_NpcState == NPC_DEAD || pNpc->m_iHP <= 0 ) {	// Npc 상태 동기화 불량,, 재요청..
		SetByte( send_buff, AG_NPC_HP_REQ, send_index);
		SetShort( send_buff, nid, send_index );
		SetDWORD( send_buff, pNpc->m_iHP, send_index );
		Send( send_buff, send_index );
	}
	// ~sungyong tw
	
	pNpc->MoveResult(fPosX, fPosY, fPosZ, fSecForMetor);
}
/*
void CAISocket::RecvNpcMoveResult(char* pBuf)
{
	char send_buff[256];
	memset(send_buff, 0x00, sizeof(send_buff));

	int index = 0;
	int send_index = 0;

	BYTE	flag;

	short	nid;

	float	fPosX;
	float	fPosZ;
	float	fPosY;

	float	fSecForMetor;

	// packet read
	flag = GetByte(pBuf, index);

	nid = GetShort(pBuf, index);

	fPosX = Getfloat(pBuf, index);
	fPosZ = Getfloat(pBuf, index);
	fPosY = Getfloat(pBuf, index);

	fSecForMetor = Getfloat(pBuf, index);

	// invalid position protection
	if (fPosX < 0.0f || fPosZ < 0.0f)
	{
		TRACE(
			"RecvNpcMoveResult Invalid Position : nid=%d x=%.2f z=%.2f\n",
			nid,
			fPosX,
			fPosZ);

		return;
	}

	// invalid speed protection
	if (fSecForMetor <= 0.0f)
		fSecForMetor = 1.0f;

	// npc find
	CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);

	if (pNpc == NULL)
	{
		TRACE(
			"RecvNpcMoveResult NPC NULL : nid=%d\n",
			nid);

		return;
	}

	// dead npc
	if (pNpc->m_NpcState == NPC_DEAD ||
		pNpc->m_iHP <= 0)
	{
		SetByte(send_buff, AG_NPC_HP_REQ, send_index);

		SetShort(send_buff, nid, send_index);

		SetDWORD(send_buff, pNpc->m_iHP, send_index);

		Send(send_buff, send_index);

		return;
	}

	// move update
	pNpc->MoveResult(
		fPosX,
		fPosY,
		fPosZ,
		fSecForMetor);

#ifdef _DEBUG
	printf(
		"[NPC MOVE RESULT] nid=%d x=%.2f y=%.2f z=%.2f speed=%.2f\n",
		nid,
		fPosX,
		fPosY,
		fPosZ,
		fSecForMetor);
#endif
}
*/
void CAISocket::RecvNpcAttack(char* pBuf)
{
	int index = 0, send_index = 0;
	int sid = -1, tid = -1;

	BYTE type = 0;
	BYTE result = 0;
	BYTE byAttackType = 0;

	short damage = 0;
	int nHP = 0;

	CNpc* pNpc = NULL;
	CNpc* pMon = NULL;
	CUser* pUser = NULL;

	char pOutBuf[1024];
	memset(pOutBuf, 0x00, sizeof(pOutBuf));

	_OBJECT_EVENT* pEvent = NULL;

	int temp_damage = 0;

	// =========================
	// PACKET READ
	// =========================
	type = GetByte(pBuf, index);
	result = GetByte(pBuf, index);

	sid = GetShort(pBuf, index);
	tid = GetShort(pBuf, index);

	damage = GetShort(pBuf, index);
	nHP = GetDWORD(pBuf, index);

	byAttackType = GetByte(pBuf, index);

	// =========================================================
	// USER -> NPC
	// =========================================================
	if (type == 0x01)
	{
		pNpc = m_pMain->m_arNpcArray.GetData(tid);
		if (!pNpc)
			return;

		pNpc->m_iHP -= damage;
		if (pNpc->m_iHP < 0)
			pNpc->m_iHP = 0;

		// =========================
		// SEND ATTACK PACKET
		// =========================
		send_index = 0;
		SetByte(pOutBuf, WIZ_ATTACK, send_index);
		SetByte(pOutBuf, byAttackType, send_index);
		SetByte(pOutBuf, result, send_index);
		SetShort(pOutBuf, sid, send_index);
		SetShort(pOutBuf, tid, send_index);

		m_pMain->Send_Region(
			pOutBuf,
			send_index,
			pNpc->m_sCurZone,
			pNpc->m_sRegion_X,
			pNpc->m_sRegion_Z,
			NULL,
			false);

		// =========================
		// ITEM DAMAGE USER
		// =========================
		if (sid >= 0 && sid < MAX_USER)
		{
			CUser* pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sid];
			if (pUser)
			{
				pUser->SendTargetHP(0, tid, -damage);

				if (byAttackType != MAGIC_ATTACK && byAttackType != DURATION_ATTACK)
					pUser->ItemWoreOut(ATTACK, damage);
			}
		}

		// =========================
		// NPC DEAD
		// =========================
		if (result == 0x02 || result == 0x04)
		{
			int zoneIndex = m_pMain->GetZoneIndex(pNpc->m_sCurZone);

			if (zoneIndex >= 0)
			{
				C3DMap* pMap = m_pMain->m_ZoneArray[zoneIndex];

				if (pMap)
				{
					pMap->RegionNpcRemove(
						pNpc->m_sRegion_X,
						pNpc->m_sRegion_Z,
						tid);
				}
			}

			pNpc->m_sRegion_X = 0;
			pNpc->m_sRegion_Z = 0;
			pNpc->m_NpcState = NPC_DEAD;

			// SPECIAL OBJECT FIX
			if (pNpc->m_byObjectType == SPECIAL_OBJECT)
			{
				if (zoneIndex >= 0)
				{
					C3DMap* pMap = m_pMain->m_ZoneArray[zoneIndex];
					if (pMap)
					{
						pEvent = pMap->GetObjectEvent(pNpc->m_sSid);
						if (pEvent)
							pEvent->byLife = 0;
					}
				}
			}
		}
	}

	// =========================================================
	// NPC -> USER / MONSTER
	// =========================================================
	else if (type == 0x02)
	{
		pNpc = m_pMain->m_arNpcArray.GetData(sid);
		if (!pNpc)
			return;

		// =========================
		// USER TARGET
		// =========================
		if (tid >= USER_BAND && tid < NPC_BAND)
		{
			if (tid >= 0 && tid < MAX_USER)
				pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[tid];

			if (!pUser)
				return;

			pUser->HpChange(-damage, 1, true);
			pUser->ItemWoreOut(DEFENCE, damage);

			send_index = 0;
			SetByte(pOutBuf, WIZ_ATTACK, send_index);
			SetByte(pOutBuf, byAttackType, send_index);
			SetByte(pOutBuf, result, send_index);
			SetShort(pOutBuf, sid, send_index);
			SetShort(pOutBuf, tid, send_index);

			m_pMain->Send_Region(
				pOutBuf,
				send_index,
				pNpc->m_sCurZone,
				pNpc->m_sRegion_X,
				pNpc->m_sRegion_Z,
				NULL,
				false);
		}

		// =========================
		// MONSTER -> MONSTER
		// =========================
		else if (tid >= NPC_BAND)
		{
			pMon = m_pMain->m_arNpcArray.GetData(tid);
			if (!pMon)
				return;

			pMon->m_iHP -= damage;
			if (pMon->m_iHP < 0)
				pMon->m_iHP = 0;

			send_index = 0;
			SetByte(pOutBuf, WIZ_ATTACK, send_index);
			SetByte(pOutBuf, byAttackType, send_index);
			SetByte(pOutBuf, result, send_index);
			SetShort(pOutBuf, sid, send_index);
			SetShort(pOutBuf, tid, send_index);

			m_pMain->Send_Region(
				pOutBuf,
				send_index,
				pMon->m_sCurZone,
				pMon->m_sRegion_X,
				pMon->m_sRegion_Z,
				NULL,
				false);

			// =========================
			// MONSTER DEAD
			// =========================
			if (result == 0x02)
			{
				int zoneIndex = m_pMain->GetZoneIndex(pMon->m_sCurZone);

				if (zoneIndex >= 0)
				{
					C3DMap* pMap = m_pMain->m_ZoneArray[zoneIndex];

					if (pMap)
					{
						pMap->RegionNpcRemove(
							pMon->m_sRegion_X,
							pMon->m_sRegion_Z,
							tid);
					}
				}

				pMon->m_sRegion_X = 0;
				pMon->m_sRegion_Z = 0;
				pMon->m_NpcState = NPC_DEAD;

				if (pMon->m_byObjectType == SPECIAL_OBJECT)
				{
					if (zoneIndex >= 0)
					{
						C3DMap* pMap = m_pMain->m_ZoneArray[zoneIndex];
						if (pMap)
						{
							pEvent = pMap->GetObjectEvent(pMon->m_sSid);
							if (pEvent)
								pEvent->byLife = 0;
						}
					}
				}
			}
		}
	}
}

void CAISocket::RecvMagicAttackResult(char* pBuf)
{
	int index = 0, send_index = 1;
	int sid = -1, tid = -1, magicid=0;
	BYTE byCommand; 
	short data0, data1, data2, data3, data4, data5;

	CNpc* pNpc = NULL;
	CUser* pUser = NULL;
	char send_buff[1024];
	memset(send_buff, NULL, 1024);

	//byType = GetByte(pBuf,index);				// who ( 1:mon->user 2:mon->mon )
	//byAttackType = GetByte(pBuf,index);			// attack type ( 1:long attack, 2:magic attack
	byCommand = GetByte(pBuf,index);			// magic type ( 1:casting, 2:flying, 3:effecting, 4:fail )
	magicid = GetDWORD(pBuf,index);
	sid = GetShort(pBuf,index);
	tid = GetShort(pBuf,index);
	data0 = GetShort(pBuf,index);
	data1 = GetShort(pBuf,index);
	data2 = GetShort(pBuf,index);
	data3 = GetShort(pBuf,index);
	data4 = GetShort(pBuf,index);
	data5 = GetShort(pBuf,index);

	SetByte( send_buff, byCommand, send_index );
	SetDWORD( send_buff, magicid, send_index );
	SetShort( send_buff, sid, send_index );
	SetShort( send_buff, tid, send_index );
	SetShort( send_buff, data0, send_index );
	SetShort( send_buff, data1, send_index );
	SetShort( send_buff, data2, send_index );
	SetShort( send_buff, data3, send_index );
	SetShort( send_buff, data4, send_index );
	SetShort( send_buff, data5, send_index );

	if(byCommand == 0x01)	{		// casting
		pNpc = m_pMain->m_arNpcArray.GetData(sid);
		if(!pNpc)	return;
		index = 0;
		SetByte( send_buff, WIZ_MAGIC_PROCESS, index );
		m_pMain->Send_Region(send_buff, send_index, pNpc->m_sCurZone, pNpc->m_sRegion_X, pNpc->m_sRegion_Z, NULL, false);
	}
	else if(byCommand == 0x03)	{	// effecting
		//pNpc = m_pMain->m_arNpcArray.GetData(tid);
		//if(!pNpc)	return;
		if( sid >= USER_BAND && sid < NPC_BAND)	{
			pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sid];
			if(pUser == NULL || pUser->m_bResHpType == USER_DEAD)	return;
			index = 0;
			SetByte( send_buff, WIZ_MAGIC_PROCESS, index );
			m_pMain->Send_Region(send_buff, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ, NULL, false);
		}
		else if(sid >= NPC_BAND)	{
			if(tid >= NPC_BAND)	{
				pNpc = m_pMain->m_arNpcArray.GetData(tid);
				if(!pNpc)	return;
				index = 0;
				SetByte( send_buff, WIZ_MAGIC_PROCESS, index );
				m_pMain->Send_Region(send_buff, send_index, pNpc->m_sCurZone, pNpc->m_sRegion_X, pNpc->m_sRegion_Z, NULL, false);
				return;
			}
			memset(send_buff, NULL, 1024);	send_index = 0;
			SetByte( send_buff, byCommand, send_index );
			SetDWORD( send_buff, magicid, send_index );
			SetShort( send_buff, sid, send_index );
			SetShort( send_buff, tid, send_index );
			SetShort( send_buff, data0, send_index );
			SetShort( send_buff, data1, send_index );
			SetShort( send_buff, data2, send_index );
			SetShort( send_buff, data3, send_index );
			SetShort( send_buff, data4, send_index );
			SetShort( send_buff, data5, send_index );
			m_MagicProcess.MagicPacket(send_buff, send_index);
		}
	}
	
}


void CAISocket::RecvUserHP(char* pBuf)
{
	int index = 0, send_index = 0;
	int nid = 0;
	int nHP = 0, nMaxHP = 0;
	char send_buff[256];
	memset( send_buff, 0x00, 256 );

	nid = GetShort(pBuf,index);
	nHP = GetDWORD(pBuf,index);
	nMaxHP = GetDWORD(pBuf,index);

	if( nid >= USER_BAND && nid < NPC_BAND)	{
		CUser* pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[nid];
		if(pUser == NULL)		return;
		pUser->m_pUserData->m_sHp = nHP;
	}
	else if(nid >= NPC_BAND)	{
		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);
		if(!pNpc)	return;
		int nOldHP = pNpc->m_iHP;
		pNpc->m_iHP = nHP;
		pNpc->m_iMaxHP = nMaxHP;
//		printf("RecvNpcHP - (%d,%s), %d->%d\n", pNpc->m_sNid, pNpc->m_strName, nOldHP, pNpc->m_sHP);
	}
}

void CAISocket::RecvUserExp(char* pBuf)
{
	int index = 0;
	int nid = 0;
	short sExp = 0;
	short sLoyalty = 0;

	nid = GetShort(pBuf,index);
	sExp = GetShort(pBuf,index);
	sLoyalty = GetShort(pBuf,index);

	CUser* pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[nid];
	if(pUser == NULL)
		return;
	if(sExp < 0 || sLoyalty < 0)	{
		printf("#### AISocket - RecvUserExp : exp=%d, loyalty=%d,, 잘못된 경험치가 온다,, 수정해!!\n", sExp, sLoyalty);
		return;
	}
	pUser->m_pUserData->m_iLoyalty += sLoyalty;
	pUser->ExpChange(sExp);

	if( sLoyalty > 0 )	{
		char send_buff[128];  memset( send_buff, 0x00, 128 );
		int send_index = 0;
		SetByte( send_buff, WIZ_LOYALTY_CHANGE, send_index );
		SetDWORD( send_buff, pUser->m_pUserData->m_iLoyalty, send_index );
		pUser->Send( send_buff, send_index );
	}
}

void CAISocket::RecvSystemMsg(char* pBuf)
{
	int index = 0, send_index = 0;
	char send_buff[256];
	memset( send_buff, 0x00, 256 );
	char strSysMsg[256];
	memset( strSysMsg, 0x00, 256 );

	BYTE bType;
	short sWho, sLength;

	bType = GetByte(pBuf,index);
	sWho = GetShort(pBuf,index);
	sLength = GetShort(pBuf,index);
	GetString( strSysMsg, pBuf, sLength, index );

	//printf("RecvSystemMsg - type=%d, who=%d, len=%d, msg=%s\n", bType, sWho, sLength, strSysMsg);

	switch(sWho)
	{
	case SEND_ME:
		break;
	case SEND_REGION:
		break;
	case SEND_ALL:
		SetByte( send_buff, WIZ_CHAT, send_index );
		SetByte( send_buff, bType, send_index );
		SetByte( send_buff, 0x01, send_index );		// nation
		SetShort( send_buff, -1, send_index );		// sid
		SetShort( send_buff, sLength, send_index );
		SetString( send_buff, strSysMsg, sLength, send_index );
		m_pMain->Send_All( send_buff, send_index );
		break;
	case SEND_ZONE:
		break;
	}
	
}

void CAISocket::RecvNpcGiveItem(char* pBuf)
{
	int index = 0, send_index = 0, zoneindex = -1;
	char send_buff[1024];
	memset( send_buff, 0x00, 1024 );
	short sUid, sNid, sZone, regionx, regionz;
	float fX, fZ, fY;
	BYTE byCount;
	int nItemNumber[NPC_HAVE_ITEM_LIST];
	short sCount[NPC_HAVE_ITEM_LIST];
	_ZONE_ITEM* pItem = NULL;
	C3DMap* pMap = NULL;
	CUser* pUser = NULL;

	sUid = GetShort(pBuf,index);	// Item을 가져갈 사람의 아이디... (이것을 참조해서 작업하셈~)
	sNid = GetShort(pBuf,index);
	sZone = GetShort(pBuf, index);
	regionx = GetShort( pBuf, index );
	regionz = GetShort( pBuf, index );
	fX = Getfloat(pBuf,index);
	fZ = Getfloat(pBuf,index);
	fY = Getfloat(pBuf,index);
	byCount = GetByte(pBuf,index);
	for(int i=0; i<byCount; i++)
	{
		nItemNumber[i] = GetDWORD(pBuf, index);
		sCount[i] = GetShort(pBuf,index);
	}

	if( sUid < 0 || sUid >= MAX_USER ) return;
	zoneindex = m_pMain->GetZoneIndex( sZone );
	if( zoneindex == -1 )
		return;

	pItem = new _ZONE_ITEM;
	for(auto i=0; i<6; i++) {
		pItem->itemid[i] = 0;
		pItem->count[i] = 0;
	}
	pItem->bundle_index = m_pMain->m_ZoneArray[zoneindex]->m_wBundle;
	pItem->time = TimeGet();
	pItem->x = fX;
	pItem->z = fZ;
	pItem->y = fY;
	for(auto i=0; i<byCount; i++) {
		if( m_pMain->m_ItemtableArray.GetData(nItemNumber[i]) ) {
			pItem->itemid[i] = nItemNumber[i];
			pItem->count[i] = sCount[i];
		}
	}

	pMap = (C3DMap*)m_pMain->m_ZoneArray[zoneindex];
	if( !pMap ) {
		delete pItem;
		return;
	}
	if( pMap->RegionItemAdd( regionx, regionz, pItem ) == FALSE ) {
		delete pItem;
		return;
	}
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sUid];
	if( !pUser ) return;
	
	send_index = 0;
	memset( send_buff, 0x00, 1024 );

	SetByte( send_buff, WIZ_ITEM_DROP, send_index );
	SetShort( send_buff, sNid, send_index );
	SetDWORD( send_buff, pItem->bundle_index, send_index );
	if( pUser->m_sPartyIndex == -1 )
		pUser->Send( send_buff, send_index );
	else
		m_pMain->Send_PartyMember( pUser->m_sPartyIndex, send_buff, send_index );
}

void CAISocket::RecvUserFail(char* pBuf)
{
	short nid = 0, sid=0;
	int index = 0, send_index = 0;
	char pOutBuf[1024];
	memset(pOutBuf, NULL, 1024);

	nid = GetShort(pBuf,index);
	sid = GetShort(pBuf,index);

	CUser* pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[nid];
	if(pUser == NULL)
		return;

	// 여기에서 게임데이타의 정보를 AI서버에 보내보자...
/*	if(pUser->m_pUserData->m_sHp > 0 && pUser->m_bResHpType != USER_DEAD)	// 게임서버와 AI서버간의 데이타가 틀림..
	{
		SetByte(pOutBuf, AG_USER_FAIL, send_index);
		SetShort( pOutBuf, nid, send_index );
		SetShort( pOutBuf, sid, send_index );
		SetShort( pOutBuf, pUser->m_pUserData->m_sHp, send_index);
		Send( pOutBuf, send_index);
	}	*/

	pUser->HpChange(-10000, 1);

	BYTE type = 0x01;
	BYTE result = 0x02;
	float fDir = 0.0f;

	SetByte(pOutBuf, WIZ_ATTACK, send_index);
	SetByte( pOutBuf, type, send_index );
	SetByte( pOutBuf, result, send_index );
	SetShort( pOutBuf, sid, send_index );
	SetShort( pOutBuf, nid, send_index );

	printf("### AISocket - RecvUserFail : sid=%d, tid=%d, id=%s ####\n", sid, nid, pUser->m_pUserData->m_id);

	m_pMain->Send_Region(pOutBuf, send_index, pUser->m_pUserData->m_bZone, pUser->m_RegionX, pUser->m_RegionZ);

}

void CAISocket::RecvCompressedData(char* pBuf)
{
	int index = 0;
	short sCompLen, sOrgLen, sCompCount;
	char pTempBuf[10001];
	memset(pTempBuf, 0x00, 10001);
	DWORD dwCrcValue;
	sCompLen = GetShort(pBuf,index);	// 압축된 데이타길이얻기...
	sOrgLen = GetShort(pBuf,index);		// 원래데이타길이얻기...
	dwCrcValue = GetDWORD(pBuf,index);	// CRC값 얻기...
	sCompCount = GetShort(pBuf,index);	// 압축 데이타 수 얻기...
	// 압축 데이타 얻기...
	memcpy( pTempBuf, pBuf+index, sCompLen );
	index += sCompLen;

	CCompressMng	cmpMgrDecode;

	/// 압축 해제	
	cmpMgrDecode.PreUncompressWork(pTempBuf, sCompLen, sOrgLen);	// 압축 풀기... 

	if (cmpMgrDecode.Extract() == false) {
		cmpMgrDecode.Initialize();
		return;
	}

	if (cmpMgrDecode.m_nErrorOccurred != 0) {
		cmpMgrDecode.Initialize();
		return;
	}
	
	if (dwCrcValue != cmpMgrDecode.m_dwCrc) {
		cmpMgrDecode.Initialize();
		return;
	}

	if (sOrgLen != cmpMgrDecode.m_nOutputBufferCurPos) {
		cmpMgrDecode.Initialize();
		return;
	} 
	
	// 압축 풀린 데이타 읽기
	Parsing(sOrgLen, cmpMgrDecode.m_pOutputBuffer);

	// 압축 풀기 끝
	cmpMgrDecode.Initialize();
}

void CAISocket::InitEventMonster(int index)
{
	int count = index;
	if( count < 0 || count > NPC_BAND )	{
		printf("### InitEventMonster index Fail = %d ###\n", index);
		return;
	}

	int max_eventmop = count+EVENT_MONSTER;
	for( int i=count; i<max_eventmop; i++ )	{
		CNpc* pNpc = NULL;
		pNpc = new CNpc;
		if(pNpc == NULL) return;
		pNpc->Initialize();

		pNpc->m_sNid = i+NPC_BAND;
		//printf("InitEventMonster : uid = %d\n", pNpc->m_sNid);
		if( !m_pMain->m_arNpcArray.PutData( pNpc->m_sNid, pNpc) ) {
			printf("Npc PutData Fail - %d\n", pNpc->m_sNid);
			delete pNpc;
			pNpc = NULL;
		}	
	}

	count = m_pMain->m_arNpcArray.GetSize();
	printf("TotalMonster = %d\n", count);
}

void CAISocket::RecvCheckAlive(char* pBuf)
{
//	printf("CAISocket-RecvCheckAlive : zone_num=%d\n", m_iZoneNum);
	m_pMain->m_sErrorSocketCount = 0;
	int len = 0;
	char pSendBuf[256];		::ZeroMemory(pSendBuf, sizeof(pSendBuf));
	SetByte(pSendBuf, AG_CHECK_ALIVE_REQ, len);
	Send(pSendBuf, len);
}

void CAISocket::RecvGateDestory(char* pBuf)
{
	int index = 0, send_index = 0, cur_zone=0, rx=0, rz=0;
	int nid = 0, gate_status = 0;
	char send_buff[256];
	memset( send_buff, 0x00, 256 );

	nid = GetShort(pBuf,index);
	gate_status = GetByte(pBuf,index);
	cur_zone = GetShort(pBuf,index);
	rx = GetShort(pBuf,index);
	rz = GetShort(pBuf,index);

	if(nid >= NPC_BAND)		{
		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);
		if(!pNpc)	return;
		pNpc->m_byGateOpen = gate_status;
		printf("RecvGateDestory - (%d,%s), gate_status=%d\n", pNpc->m_sNid, pNpc->m_strName, pNpc->m_byGateOpen);
/*
		SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
		SetByte( send_buff, 1, send_index );					// type
		SetByte( send_buff, 1, send_index );
		SetShort( send_buff, nid, send_index );
		SetByte( send_buff, pNpc->m_byGateOpen, send_index );
		m_pMain->Send_Region( send_buff, send_index, cur_zone, rx, rz );	*/	
	}
}

void CAISocket::RecvNpcDead(char* pBuf)
{
	int index = 0, send_index = 0;
	int nid = 0;
	char send_buff[256];
	memset( send_buff, 0x00, 256 );
	_OBJECT_EVENT* pEvent = NULL;

	nid = GetShort(pBuf,index);


	if(nid >= NPC_BAND)		{
		CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);
		if(!pNpc)	return;

		if( pNpc->m_byObjectType == SPECIAL_OBJECT )	{
			pEvent = m_pMain->m_ZoneArray[pNpc->m_sZoneIndex]->GetObjectEvent( pNpc->m_sSid );
			if( pEvent )	pEvent->byLife = 0;
		}

		//pNpc->NpcInOut( NPC_OUT );
		//printf("RecvNpcDead - (%d,%s)\n", pNpc->m_sNid, pNpc->m_strName);

		C3DMap* pMap = m_pMain->m_ZoneArray[(int)pNpc->m_sZoneIndex];
		if( !pMap )	return;
		pMap->RegionNpcRemove(pNpc->m_sRegion_X, pNpc->m_sRegion_Z, nid);
		//printf("--- RecvNpcDead : Npc를 Region에서 삭제처리.. ,, zone=%d, region_x=%d, y=%d\n", pNpc->m_sZoneIndex, pNpc->m_sRegion_X, pNpc->m_sRegion_Z);

		SetByte( send_buff, WIZ_DEAD, send_index );
		SetShort( send_buff, nid, send_index );
		m_pMain->Send_Region(send_buff, send_index, pNpc->m_sCurZone, pNpc->m_sRegion_X, pNpc->m_sRegion_Z, NULL, false);

		pNpc->m_sRegion_X = 0;		pNpc->m_sRegion_Z = 0;
	}
}

void CAISocket::RecvNpcInOut(char* pBuf)
{
	int index = 0;

	BYTE nType = GetByte(pBuf, index);

	int nid = GetShort(pBuf, index);

	// AIServer -> Ebenezer AG_NPC_INOUT uses float coordinates.
	// Do not read SID/zone here: AIServer sends only type, NID and X/Z/Y.
	float fx = Getfloat(pBuf, index);
	float fz = Getfloat(pBuf, index);
	float fy = Getfloat(pBuf, index);

	if (nid < NPC_BAND)
		return;

	CNpc* pNpc = m_pMain->m_arNpcArray.GetData(nid);

	if (!pNpc)
	{
		printf("NPC_INOUT FAIL nid=%d\n", nid);
		return;
	}

	pNpc->NpcInOut(nType, fx, fz, fy);
}

void CAISocket::RecvBattleEvent(char* pBuf)
{
	int index = 0, send_index = 0, udp_index = 0, retvalue = 0;
	int nType = 0, nResult = 0, nLen = 0;
	char strMaxUserName[MAX_ID_SIZE+1];	memset( strMaxUserName, 0x00, MAX_ID_SIZE+1 );
	char strKnightsName[MAX_ID_SIZE+1];	memset( strKnightsName, 0x00, MAX_ID_SIZE+1 );
	char chatstr[1024]; memset( chatstr, NULL, 1024 );
	char finalstr[1024]; memset( finalstr, NULL, 1024 );
	char send_buff[1024]; memset( send_buff, NULL, 1024 );
	char udp_buff[1024]; memset( udp_buff, NULL, 1024 );
	CUser* pUser = NULL;
	CKnights* pKnights = NULL;

	std::string buff;
	std::string buff2;

	nType = GetByte( pBuf, index );
	nResult = GetByte(pBuf, index);

	if( nType == BATTLE_EVENT_OPEN )	{
	}
	else if( nType == BATTLE_MAP_EVENT_RESULT )	{
		if( m_pMain->m_byBattleOpen == NO_BATTLE )	{
			printf("#### RecvBattleEvent Fail : battleopen = %d, type = %d\n", m_pMain->m_byBattleOpen, nType);
			return;
		}
		if( nResult == KARUS )	{
			//printf("--> RecvBattleEvent : 카루스 땅으로 넘어갈 수 있어\n");
			m_pMain->m_byKarusOpenFlag = 1;		// 카루스 땅으로 넘어갈 수 있어
		}
		else if( nResult == ELMORAD )	{
			//printf("--> RecvBattleEvent : 엘모 땅으로 넘어갈 수 있어\n");
			m_pMain->m_byElmoradOpenFlag = 1;	// 엘모 땅으로 넘어갈 수 있어
		}

		SetByte( udp_buff, UDP_BATTLE_EVENT_PACKET, udp_index );
		SetByte( udp_buff, nType, udp_index );
		SetByte( udp_buff, nResult, udp_index );
	}
	else if( nType == BATTLE_EVENT_RESULT )	{
		if( m_pMain->m_byBattleOpen == NO_BATTLE )	{
			printf("#### RecvBattleEvent Fail : battleopen = %d, type=%d\n", m_pMain->m_byBattleOpen, nType);
			return;
		}
		if( nResult == KARUS )	{
			//printf("--> RecvBattleEvent : 카루스가 승리하였습니다.\n");
		}
		else if( nResult == ELMORAD )	{
			//printf("--> RecvBattleEvent : 엘모라드가 승리하였습니다.\n");
		}

		nLen = GetByte(pBuf, index);

		if( nLen > 0 && nLen < MAX_ID_SIZE+1 )	{
			GetString( strMaxUserName, pBuf, nLen, index );
			if( m_pMain->m_byBattleSave == 0 )	{
				memset( send_buff, NULL, 1024 );		send_index = 0;			// 승리국가를 sql에 저장
				SetByte( send_buff, WIZ_BATTLE_EVENT, send_index );
				SetByte( send_buff, nType, send_index );
				SetByte( send_buff, nResult, send_index );
				SetByte( send_buff, nLen, send_index );
				SetString( send_buff, strMaxUserName, nLen, send_index );
				retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
				if( retvalue >= SMQ_FULL ) {
					char logstr[1024]; memset( logstr, 0x00, 1024 );
					sprintf_s( logstr, "WIZ_BATTLE_EVENT Send Fail : %d, %d", retvalue, nType);
					m_pMain->m_StatusList.AddString(logstr);
				}
				m_pMain->m_byBattleSave = 1;
			}
		}

		m_pMain->m_bVictory = nResult;
		m_pMain->m_byOldVictory = nResult;
		m_pMain->m_byKarusOpenFlag = 0;		// 카루스 땅으로 넘어갈 수 없도록
		m_pMain->m_byElmoradOpenFlag = 0;	// 엘모 땅으로 넘어갈 수 없도록
		m_pMain->m_byBanishFlag = 1;

		SetByte( udp_buff, UDP_BATTLE_EVENT_PACKET, udp_index );	// udp로 다른서버에 정보 전달
		SetByte( udp_buff, nType, udp_index );
		SetByte( udp_buff, nResult, udp_index );
	}
	else if( nType == BATTLE_EVENT_MAX_USER )	{
		nLen = GetByte(pBuf, index);

		if( nLen > 0 && nLen < MAX_ID_SIZE+1 )	{
			GetString( strMaxUserName, pBuf, nLen, index );
			pUser = m_pMain->GetUserPtr( strMaxUserName, 0x02 );
			if( pUser )	{
				pKnights = m_pMain->m_KnightsArray.GetData( pUser->m_pUserData->m_bKnights );
				if( pKnights )	{
					strcpy_s( strKnightsName, pKnights->m_strName );
				}
			}
			//printf("--> RecvBattleEvent : 적국의 대장을 죽인 유저이름은? %s, len=%d\n", strMaxUserName, nResult);
			if( nResult == 1 )	{
				::_LoadStringFromResource(IDS_KILL_CAPTAIN, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
				
		/*		if( m_pMain->m_byBattleSave == 0 )	{
					memset( send_buff, NULL, 256 );		send_index = 0;			// 승리국가를 sql에 저장
					SetByte( send_buff, WIZ_BATTLE_EVENT, send_index );
					SetByte( send_buff, nType, send_index );
					SetByte( send_buff, m_pMain->m_bVictory, send_index );
					SetByte( send_buff, nLen, send_index );
					SetString( send_buff, strMaxUserName, nLen, send_index );
					retvalue = m_pMain->m_LoggerSendQueue.PutData( send_buff, send_index );
					if( retvalue >= SMQ_FULL ) {
						char logstr[256]; memset( logstr, 0x00, 256 );
						sprintf( logstr, "WIZ_BATTLE_EVENT Send Fail : %d, %d", retvalue, nType);
						m_pMain->m_StatusList.AddString(logstr);
					}
					m_pMain->m_byBattleSave = 1;
				}	*/
			}
			else	if( nResult == 2 )	{
				::_LoadStringFromResource(IDS_KILL_GATEKEEPER, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}
			else	if( nResult == 3 )	{
				::_LoadStringFromResource(IDS_KILL_KARUS_GUARD1, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}
			else	if( nResult == 4 )	{
				::_LoadStringFromResource(IDS_KILL_KARUS_GUARD2, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}
			else	if( nResult == 5 )	{
				::_LoadStringFromResource(IDS_KILL_ELMO_GUARD1, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}
			else	if( nResult == 6 )	{
				::_LoadStringFromResource(IDS_KILL_ELMO_GUARD2, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}
			else	if( nResult == 7 || nResult == 8 )	{
				::_LoadStringFromResource(IDS_KILL_GATEKEEPER, buff);
				sprintf_s( chatstr, buff.c_str(), strKnightsName, strMaxUserName );
			}

			memset( send_buff, NULL, 1024 );		send_index = 0;
			//sprintf( finalstr, "## 공지 : %s ##", chatstr );
			::_LoadStringFromResource(IDP_ANNOUNCEMENT, buff2);
			sprintf_s( finalstr, buff2.c_str(), chatstr );
			SetByte( send_buff, WIZ_CHAT, send_index );
			SetByte( send_buff, WAR_SYSTEM_CHAT, send_index );
			SetByte( send_buff, 1, send_index );
			SetShort( send_buff, -1, send_index );
			SetShort( send_buff, strlen(finalstr), send_index );
			SetString( send_buff, finalstr, strlen(finalstr), send_index );
			m_pMain->Send_All( send_buff, send_index );

			memset( send_buff, NULL, 1024 );		send_index = 0;
			SetByte( send_buff, WIZ_CHAT, send_index );
			SetByte( send_buff, PUBLIC_CHAT, send_index );
			SetByte( send_buff, 1, send_index );
			SetShort( send_buff, -1, send_index );
			SetShort( send_buff, strlen(finalstr), send_index );
			SetString( send_buff, finalstr, strlen(finalstr), send_index );
			m_pMain->Send_All( send_buff, send_index );

			SetByte( udp_buff, UDP_BATTLE_EVENT_PACKET, udp_index );
			SetByte( udp_buff, nType, udp_index );
			SetByte( udp_buff, nResult, udp_index );
			SetByte( udp_buff, nLen, udp_index );
			SetString( udp_buff, strMaxUserName, nLen, udp_index );
		}
	}

	m_pMain->Send_UDP_All( udp_buff, udp_index );
}


void CAISocket::RecvNpcEventItem( char* pBuf )
{
	int index = 0, send_index = 0, zoneindex = -1;
	char send_buff[1024];
	memset( send_buff, 0x00, 1024 );
	short sUid = 0, sNid = 0;
	int nItemNumber = 0, nCount = 0;
	CUser* pUser = NULL;

	sUid = GetShort(pBuf,index);	// Item을 가져갈 사람의 아이디... (이것을 참조해서 작업하셈~)
	sNid = GetShort(pBuf,index);
	nItemNumber = GetDWORD(pBuf, index);
	nCount = GetDWORD(pBuf,index);

	if( sUid < 0 || sUid >= MAX_USER ) return;
	pUser = (CUser*)m_pMain->m_Iocport.m_SockArray[sUid];
	if( !pUser ) return;
	pUser->EventMoneyItemGet( nItemNumber, nCount );
}

void CAISocket::RecvGateOpen( char* pBuf )
{
	int index = 0, send_index = 0, nNid = 0, nSid = 0, nGateFlag = 0;
	char send_buff[256];
	memset( send_buff, 0x00, 256 );
	CNpc* pNpc = NULL;
	_OBJECT_EVENT* pEvent = NULL;

	nNid = GetShort(pBuf,index);
	nSid = GetShort(pBuf,index);
	nGateFlag = GetByte(pBuf,index);

	pNpc = m_pMain->m_arNpcArray.GetData( nNid );
	if(!pNpc)	{
		printf("#### RecvGateOpen Npc Pointer null : nid=%d ####\n", nNid);
		return;
	}

	pNpc->m_byGateOpen = nGateFlag;

	pEvent = m_pMain->m_ZoneArray[pNpc->m_sZoneIndex]->GetObjectEvent( nSid );
	if( !pEvent )	{
		printf("#### RecvGateOpen Npc Object fail : nid=%d, sid=%d ####\n", nNid, nSid);
		return;
	}

	//printf("---> RecvGateOpen Npc Object fail : nid=%d, sid=%d, nGateFlag = %d ####\n", nNid, nSid, nGateFlag);

	if( pNpc->m_tNpcType == NPC_GATE || pNpc->m_tNpcType == NPC_PHOENIX_GATE || pNpc->m_tNpcType == NPC_SPECIAL_GATE )	{
		SetByte( send_buff, WIZ_OBJECT_EVENT, send_index );
		SetByte( send_buff, pEvent->sType, send_index );
		SetByte( send_buff, 0x01, send_index );
		SetShort( send_buff, nNid, send_index );
		SetByte( send_buff, pNpc->m_byGateOpen, send_index );
		m_pMain->Send_Region( send_buff, send_index, pNpc->m_sCurZone, pNpc->m_sRegion_X, pNpc->m_sRegion_Z, NULL, false);
	}
}