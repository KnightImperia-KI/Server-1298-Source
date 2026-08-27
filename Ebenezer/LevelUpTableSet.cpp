// LevelUpTableSet.cpp : implementation file
//

#include "stdafx.h"
#include "Ebenezer.h"
#include "LevelUpTableSet.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLevelUpTableSet

IMPLEMENT_DYNAMIC(CLevelUpTableSet, CRecordset)

CLevelUpTableSet::CLevelUpTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CLevelUpTableSet)
	m_level = 0;
	m_Exp = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CLevelUpTableSet::GetDefaultConnect()
{
	CIni inifile;
	inifile.SetPath("GameServer.ini");

	// INI'den deðerleri çekiyoruz
	// Sýnýfýnýz char* döndürdüðü için doðrudan CString'e atayabiliriz
	CString strDSN = inifile.GetProfileString("ODBC", "DSN", "kn_online");
	CString strUID = inifile.GetProfileString("ODBC", "UID", "knight");
	CString strPWD = inifile.GetProfileString("ODBC", "PWD", "knight");

	// Baðlantý dizesini oluþturuyoruz
	CString strConn;
	strConn.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s"), (LPCTSTR)strDSN, (LPCTSTR)strUID, (LPCTSTR)strPWD);

	return strConn;
}

CString CLevelUpTableSet::GetDefaultSQL()
{
	return _T("[dbo].[LEVEL_UP]");
}

void CLevelUpTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CLevelUpTableSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[level]"), m_level);
	RFX_Long(pFX, _T("[Exp]"), m_Exp);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CLevelUpTableSet diagnostics

#ifdef _DEBUG
void CLevelUpTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CLevelUpTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
