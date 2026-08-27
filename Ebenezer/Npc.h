// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_)
#define AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"


class CEbenezerDlg;
class C3DMap;
class CNpc  
{
public:
	CEbenezerDlg* m_pMain;
	C3DMap* m_pMap;

	short	m_sNid;				// NPC (서버상의)일련번호
	short	m_sSid;				// NPC 테이블 참조번호
	short	m_sCurZone;			// Current Zone;
	short	m_sZoneIndex;		// NPC 가 존재하고 있는 존의 인덱스
	float	m_fCurX;			// Current X Pos;
	float	m_fCurY;			// Current Y Pos;
	float	m_fCurZ;			// Current Z Pos;
	float	m_fDir;				// NPC가 보고 있는 방향
	short	m_sPid;				// MONSTER(NPC) Picture ID
	short	m_sSize;			// MONSTER(NPC) Size
	int		m_iWeapon_1;
	int		m_iWeapon_2;
	char	m_strName[MAX_ID_SIZE];		// MONSTER(NPC) Name
	int		m_iMaxHP;			// 최대 HP
	int		m_iHP;				// 현재 HP
	uint8_t	m_byState;			// 몬스터 (NPC) 상태
	uint8_t	m_byGroup;			// 소속 집단
	uint8_t	m_byLevel;			// 레벨
	uint8_t	m_tNpcType;			// NPC Type
	// 0 : Normal Monster
	// 1 : NPC
	// 2 : 각 입구,출구 NPC
	// 3 : 경비병
	int   m_iSellingGroup;		// ItemGroup
	//	DWORD	m_dwStepDelay;		

	short m_sRegion_X;			// region x position
	short m_sRegion_Z;			// region z position
	uint8_t	m_NpcState;			// NPC의 상태 - 살았다, 죽었다, 서있다 등등...
	uint8_t	m_byGateOpen;		// Gate Npc Status -> 1 : open 0 : close
	short   m_sHitRate;			// 공격 성공률
	uint8_t    m_byObjectType;     // 보통은 0, object타입(성문, 레버)은 1
	uint8_t    m_byDirection;
	short   m_byEvent;		    // This is for the quest. 

	int     m_sAtk;
	int     m_sAc;

	short   m_sEvadeRate;
	short   m_sDamage;
	short   m_sAttackDelay;

	int     m_iExp;
	int     m_iLoyalty;
	short   m_sMpPoint;
	uint8_t  m_byTrapNumber;
	uint8_t  m_byMonster;
	short m_sSocketID;

	short m_sFireR;
	short m_sColdR;
	short m_sLightningR;
	short m_sMagicR;
	short m_sDiseaseR;
	short m_sPoisonR;
	short m_sLightR;
	//char m_strName[MAX_ID_SIZE];

public:
	CNpc();
	virtual ~CNpc();

	void Initialize();
	void MoveResult(float xpos, float ypos, float zpos, float speed);
	
	void NpcInOut(BYTE Type, float fx, float fz, float fy);
	void RegisterRegion();
	void RemoveRegion(int del_x, int del_z);
	void InsertRegion(int del_x, int del_z);
	int GetRegionNpcList(int region_x, int region_z, char *buff, int &t_count);
	void SendInOut(BYTE type, float fx, float fz, float fy);
	
	BYTE GetState()		{ return m_byState; };
	__forceinline C3DMap* GetMap() { return m_pMap; };
	

};

#endif // !defined(AFX_NPC_H__1DE71CDD_4040_4479_828D_E8EE07BD7A67__INCLUDED_)
