// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ebenezer.h"
#include "Npc.h"
#include "EbenezerDlg.h"
#include "Map.h"
#include "PacketDefine.h"
#include "User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION g_region_critical;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{

}

CNpc::~CNpc()
{

}
/*
void CNpc::Initialize()
{
	m_pMain = (CEbenezerDlg*)AfxGetMainWnd();

	m_sNid = -1;				// NPC (¼­¹ö»óÀÇ)ÀÏ·Ã¹øÈ£
	m_sSid = 0;
	m_sZoneIndex = -1;			// Current Zone Index(¹è¿­)
	m_sCurZone = -1;			// Current Zone number
	m_fCurX = 0;			// Current X Pos;
	m_fCurY = 0;			// Current Y Pos;
	m_fCurZ = 0;			// Current Z Pos;
	m_sPid = 0;				// MONSTER(NPC) Picture ID
	m_sSize = 100;				// MONSTER(NPC) Size
	m_iMaxHP = 0;				// ÃÖ´ë HP
	m_iHP = 0;					// ÇöÀç HP
	//m_byState = 0;			// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó
	m_tNpcType = 0;				// NPC Type
								// 0 : Normal Monster
								// 1 : NPC
								// 2 : °¢ ÀÔ±¸,Ãâ±¸ NPC
								// 3 : °æºñº´
	m_byGroup = 0;
	m_byLevel = 0;
	m_iSellingGroup = 0;
//	m_dwStepDelay = 0;		

	m_sRegion_X = 0;			// region x position
	m_sRegion_Z = 0;			// region z position
	m_fDir = 0.0f;				// npcÀÇ ¹æÇâ,,
	m_iWeapon_1 = 0;
	m_iWeapon_2 = 0;
	m_NpcState = NPC_LIVE;
	m_byGateOpen = 1;
	m_sHitRate = 0;
	m_byObjectType = NORMAL_OBJECT;

	m_byEvent = -1;				//  This is for the event.
}
*/
void CNpc::Initialize()
{
	// Main pointer (MFC yerine global pointer daha stabil)
	m_pMain = g_pMain;

	// IDs
	m_sNid = -1;
	m_sSid = 0;
	m_sPid = 0;

	// Zone / Map
	m_sZoneIndex = 0;
	m_sCurZone = 0;

	// Position
	m_fCurX = 0.0f;
	m_fCurY = 0.0f;
	m_fCurZ = 0.0f;
	m_fDir = 180.0f;

	// Size / Model
	m_sSize = 100;

	// HP (CRITICAL: 0 HP NPC = invisible / not spawned bug)
	m_iMaxHP = 100;
	m_iHP = 100;

	// State
	m_byState = 1;          // alive state safe default
	m_NpcState = NPC_LIVE;

	// Type
	m_tNpcType = 0;         // monster default

	// Group / Level (CRITICAL: 0 causes filter bugs in some logic)
	m_byGroup = 1;
	m_byLevel = 1;

	// Economy / AI
	m_iSellingGroup = 0;

	// Region
	m_sRegion_X = 0;
	m_sRegion_Z = 0;

	// Weapons
	m_iWeapon_1 = 0;
	m_iWeapon_2 = 0;

	// Gate / Object
	m_byGateOpen = 1;
	m_byObjectType = NORMAL_OBJECT;

	// Combat
	m_sHitRate = 0;

	// Event system (0 safer than -1 in many checks)
	m_byEvent = 0;

	m_byDirection = 0;
}
/*
void CNpc::MoveResult(float xpos, float ypos, float zpos, float speed)
{
	m_fCurX = xpos;
	m_fCurZ = zpos;
	m_fCurY = ypos;

	// 🔥 REGION FIX
	int old_x = m_sRegion_X;
	int old_z = m_sRegion_Z;

	m_sRegion_X = (short)(m_fCurX / VIEW_DISTANCE);
	m_sRegion_Z = (short)(m_fCurZ / VIEW_DISTANCE);

	if (old_x != m_sRegion_X || old_z != m_sRegion_Z)
	{
		m_pMain->m_ZoneArray[m_sZoneIndex]->RegionNpcRemove(old_x, old_z, m_sNid);
		m_pMain->m_ZoneArray[m_sZoneIndex]->RegionNpcAdd(m_sRegion_X, m_sRegion_Z, m_sNid);
	}

	int send_index = 0;
	char pOutBuf[128];
	memset(pOutBuf, 0, sizeof(pOutBuf));

	SetByte(pOutBuf, WIZ_NPC_MOVE, send_index);
	SetShort(pOutBuf, m_sNid, send_index);

	// 🔥 FIXED COORDS
	SetShort(pOutBuf, (short)(m_fCurX * 10), send_index);
	SetShort(pOutBuf, (short)(m_fCurZ * 10), send_index);
	SetShort(pOutBuf, (short)(m_fCurY * 10), send_index);
	SetShort(pOutBuf, (short)(speed * 10), send_index);

	m_pMain->Send_Region(pOutBuf, send_index, m_sCurZone, m_sRegion_X, m_sRegion_Z);

	printf("RecvNpcMove ==> nid=%d zone=%d x=%.2f z=%.2f\n",
		m_sNid, m_sCurZone, m_fCurX, m_fCurZ);
}
*/
void CNpc::MoveResult(float xpos, float ypos, float zpos, float speed)
{
	// Position update
	m_fCurX = xpos;
	m_fCurY = ypos;
	m_fCurZ = zpos;

	// Map validation
	if (m_pMain == nullptr)
		return;

	C3DMap* pMap = m_pMain->m_ZoneArray[m_sZoneIndex];

	if (pMap == nullptr)
		return;

	// Region backup
	int old_region_x = m_sRegion_X;
	int old_region_z = m_sRegion_Z;

	// New region calculate
	m_sRegion_X = (short)(m_fCurX / VIEW_DISTANCE);
	m_sRegion_Z = (short)(m_fCurZ / VIEW_DISTANCE);

	// Region change
	if (old_region_x != m_sRegion_X || old_region_z != m_sRegion_Z)
	{
		// remove old region
		if (old_region_x >= 0 && old_region_z >= 0)
		{
			pMap->RegionNpcRemove(
				old_region_x,
				old_region_z,
				m_sNid);
		}

		// add new region
		pMap->RegionNpcAdd(
			m_sRegion_X,
			m_sRegion_Z,
			m_sNid);
	}

	// Invalid speed protection
	if (speed < 0.1f)
		speed = 1.0f;

	// Packet build
	int send_index = 0;

	char pOutBuf[128];
	memset(pOutBuf, 0, sizeof(pOutBuf));

	// opcode
	SetByte(pOutBuf, WIZ_NPC_MOVE, send_index);

	// npc id
	SetShort(pOutBuf, m_sNid, send_index);

	// client reads uint16
	SetShort(
		pOutBuf,
		(uint16_t)(m_fCurX * 10.0f),
		send_index);

	SetShort(
		pOutBuf,
		(uint16_t)(m_fCurZ * 10.0f),
		send_index);

	// client reads int16
	SetShort(
		pOutBuf,
		(int16_t)(m_fCurY * 10.0f),
		send_index);

	SetShort(
		pOutBuf,
		(int16_t)(speed * 10.0f),
		send_index);

	// send region
	m_pMain->Send_Region(
		pOutBuf,
		send_index,
		m_sCurZone,
		m_sRegion_X,
		m_sRegion_Z);

#ifdef _DEBUG
	printf(
		"[NPC MOVE] nid=%d zone=%d region=(%d,%d) pos=(%.2f, %.2f, %.2f) speed=%.2f\n",
		m_sNid,
		m_sCurZone,
		m_sRegion_X,
		m_sRegion_Z,
		m_fCurX,
		m_fCurY,
		m_fCurZ,
		speed);
#endif
}
void CNpc::NpcInOut(BYTE Type, float fx, float fz, float fy)
{
	int send_index = 0;

	char buff[1024];
	memset(buff, 0x00, sizeof(buff));

	C3DMap* pMap = m_pMain->m_ZoneArray[m_sZoneIndex];

	if (pMap == nullptr)
		return;

	// OUT
	if (Type == NPC_OUT)
	{
		if (m_sRegion_X >= 0 && m_sRegion_Z >= 0)
		{
			pMap->RegionNpcRemove(
				m_sRegion_X,
				m_sRegion_Z,
				m_sNid);
		}
	}
	else
	{
		// position update
		m_fCurX = fx;
		m_fCurZ = fz;
		m_fCurY = fy;

		// region update
		m_sRegion_X = (short)(m_fCurX / VIEW_DISTANCE);
		m_sRegion_Z = (short)(m_fCurZ / VIEW_DISTANCE);

		pMap->RegionNpcAdd(
			m_sRegion_X,
			m_sRegion_Z,
			m_sNid);
	}

	// opcode
	SetByte(buff, WIZ_NPC_INOUT, send_index);

	// type
	SetByte(buff, Type, send_index);

	// npc id
	SetShort(buff, m_sNid, send_index);

	// OUT packet end
	if (Type == NPC_OUT)
	{
		m_pMain->Send_Region(
			buff,
			send_index,
			m_sCurZone,
			m_sRegion_X,
			m_sRegion_Z);

		return;
	}

	// name length limit
	int len = (int)strlen(m_strName);

	if (len > 50)
		len = 50;

	// sid/resource id
	SetShort(buff, m_sPid, send_index);

	// npc type
	SetByte(buff, m_tNpcType, send_index);

	// selling group
	SetDWORD(buff, m_iSellingGroup, send_index);

	// scale
	SetShort(buff, m_sSize, send_index);

	// weapons
	SetDWORD(buff, m_iWeapon_1, send_index);
	SetDWORD(buff, m_iWeapon_2, send_index);

	// name length
	SetByte(buff, len, send_index);

	// name
	SetString(buff, m_strName, len, send_index);

	// nation/group
	SetByte(buff, m_byGroup, send_index);

	// level
	SetByte(buff, (BYTE)m_byLevel, send_index);

	// client reads uint16
	SetShort(
		buff,
		(uint16_t)(m_fCurX * 10.0f),
		send_index);

	SetShort(
		buff,
		(uint16_t)(m_fCurZ * 10.0f),
		send_index);

	// client reads int16
	SetShort(
		buff,
		(int16_t)(m_fCurY * 10.0f),
		send_index);

	// status
	SetDWORD(buff, (DWORD)m_byGateOpen, send_index);

	// object type
	SetByte(buff, m_byObjectType, send_index);

	// unknown values (client expects these)
	SetShort(buff, 0, send_index);
	SetShort(buff, 0, send_index);

	// direction
	SetByte(buff, 0, send_index);

	// send
	m_pMain->Send_Region(
		buff,
		send_index,
		m_sCurZone,
		m_sRegion_X,
		m_sRegion_Z);

#ifdef _DEBUG
	printf(
		"[NPC INOUT] type=%d nid=%d sid=%d zone=%d x=%.2f z=%.2f\n",
		Type,
		m_sNid,
		m_sPid,
		m_sCurZone,
		m_fCurX,
		m_fCurZ);
#endif
}



void CNpc::RegisterRegion()
{
	int iRegX = 0, iRegZ = 0, old_region_x = 0, old_region_z = 0;
	iRegX = (int)(m_fCurX / VIEW_DISTANCE);
	iRegZ = (int)(m_fCurZ / VIEW_DISTANCE);

	if(m_sRegion_X != iRegX || m_sRegion_Z != iRegZ)
	{
		C3DMap* pMap = m_pMain->m_ZoneArray[m_sZoneIndex];
		if( !pMap )
			return;
		
		old_region_x = m_sRegion_X;	old_region_z = m_sRegion_Z;
		pMap->RegionNpcRemove(m_sRegion_X, m_sRegion_Z, m_sNid);
		m_sRegion_X = iRegX;		m_sRegion_Z = iRegZ;
		pMap->RegionNpcAdd(m_sRegion_X, m_sRegion_Z, m_sNid);

		RemoveRegion( old_region_x - m_sRegion_X, old_region_z - m_sRegion_Z );	// delete npc ´Â °è»ê ¹æÇâÀÌ ÁøÇà¹æÇâÀÇ ¹İ´ë...
		InsertRegion( m_sRegion_X - old_region_x, m_sRegion_Z - old_region_z );	// add npc ´Â °è»ê ¹æÇâÀÌ ÁøÇà¹æÇâ...
	}
}

void CNpc::RemoveRegion(int del_x, int del_z)
{
	C3DMap* pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if (!pMap || m_sNid <= 0)
		return;

	char buff[32];
	int send_index = 0;
	memset(buff, 0, sizeof(buff));

	// 1298 client NPC_OUT reads only: opcode, type, NID.
	SetByte(buff, WIZ_NPC_INOUT, send_index);
	SetByte(buff, NPC_OUT, send_index);
	SetShort(buff, m_sNid, send_index);

	auto SendSafe = [&](int x, int z)
	{
		if (x < 0 || z < 0 ||
			x >= pMap->GetXRegionMax() ||
			z >= pMap->GetZRegionMax())
			return;

		m_pMain->Send_UnitRegion(buff, send_index, m_sZoneIndex, x, z);
	};

	const int x = m_sRegion_X + del_x;
	const int z = m_sRegion_Z + del_z;

	SendSafe(x, z);
	SendSafe(x - 1, z);
	SendSafe(x + 1, z);
	SendSafe(x, z - 1);
	SendSafe(x, z + 1);
}

void CNpc::InsertRegion(int del_x, int del_z)
{
	C3DMap* pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if (!pMap || m_sNid <= 0)
		return;

	char buff[1024];
	int send_index = 0;
	memset(buff, 0, sizeof(buff));

	// Build exactly the same NPC_IN payload used by NpcInOut().
	// The 1298 client expects: NID, ResourceID, Type, TradeID, Scale,
	// Weapon0, Weapon1, BYTE name length, name, Nation, Level, X/Z/Y,
	// Status, ObjectType, two SHORTs and Direction.
	SetByte(buff, WIZ_NPC_INOUT, send_index);
	SetByte(buff, NPC_IN, send_index);
	SetShort(buff, m_sNid, send_index);
	SetShort(buff, m_sPid, send_index);
	SetByte(buff, m_tNpcType, send_index);
	SetDWORD(buff, m_iSellingGroup, send_index);
	SetShort(buff, (m_sSize <= 0 ? 100 : m_sSize), send_index);
	SetDWORD(buff, m_iWeapon_1, send_index);
	SetDWORD(buff, m_iWeapon_2, send_index);

	int len = (int)strlen(m_strName);
	if (len > 50) len = 50;
	SetByte(buff, (BYTE)len, send_index);
	if (len > 0)
		SetString(buff, m_strName, len, send_index);

	SetByte(buff, m_byGroup, send_index);
	SetByte(buff, (BYTE)m_byLevel, send_index);
	SetShort(buff, (uint16_t)(m_fCurX * 10.0f), send_index);
	SetShort(buff, (uint16_t)(m_fCurZ * 10.0f), send_index);
	SetShort(buff, (int16_t)(m_fCurY * 10.0f), send_index);
	SetDWORD(buff, (DWORD)m_byGateOpen, send_index);
	SetByte(buff, m_byObjectType, send_index);
	SetShort(buff, 0, send_index);
	SetShort(buff, 0, send_index);
	SetByte(buff, 0, send_index);

	auto SendSafe = [&](int x, int z)
	{
		if (x < 0 || z < 0 ||
			x >= pMap->GetXRegionMax() ||
			z >= pMap->GetZRegionMax())
			return;

		m_pMain->Send_UnitRegion(buff, send_index, m_sZoneIndex, x, z);
	};

	if (del_x != 0)
	{
		SendSafe(m_sRegion_X + del_x, m_sRegion_Z - 1);
		SendSafe(m_sRegion_X + del_x, m_sRegion_Z);
		SendSafe(m_sRegion_X + del_x, m_sRegion_Z + 1);
	}

	if (del_z != 0)
	{
		SendSafe(m_sRegion_X, m_sRegion_Z + del_z);

		if (del_x < 0)
			SendSafe(m_sRegion_X + 1, m_sRegion_Z + del_z);
		else if (del_x > 0)
			SendSafe(m_sRegion_X - 1, m_sRegion_Z + del_z);
		else
		{
			SendSafe(m_sRegion_X - 1, m_sRegion_Z + del_z);
			SendSafe(m_sRegion_X + 1, m_sRegion_Z + del_z);
		}
	}
}

int CNpc::GetRegionNpcList(int region_x, int region_z, char* buff, int& t_count)
{
	if (m_pMain->m_bPointCheckFlag == FALSE)	return 0;	// 포인터 참조하면 안됨

	int buff_index = 0, i = 0, j = 0;
	int user_count = 0, nid = -1;
	C3DMap* pMap = NULL;
	CNpc* pNpc = NULL;

	pMap = (C3DMap*)m_pMain->m_ZoneArray[m_sZoneIndex];
	if (!pMap)
		return 0;
	if (region_x<0 || region_z<0 || region_x>pMap->GetXRegionMax() || region_z>pMap->GetZRegionMax())
		return 0;

	EnterCriticalSection(&g_region_critical);

	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionNpcArray.m_UserTypeMap.end();

	for (; Iter1 != Iter2; Iter1++) {
		nid = *((*Iter1).second);
		if (nid < 0)
			continue;
		pNpc = (CNpc*)m_pMain->m_arNpcArray.GetData(nid);
		if (pNpc) {
			SetShort(buff, pNpc->m_sNid, buff_index);
			t_count++;
		}
	}

	LeaveCriticalSection(&g_region_critical);

	return buff_index;
}
void CNpc::SendInOut(BYTE type, float fx, float fz, float fy)
{
	// Keep the legacy entry point on the same packet implementation used by
	// the AI socket path.  This prevents two different NPC_INOUT formats
	// from being emitted by Ebenezer.
	NpcInOut(type, fx, fz, fy);
}
