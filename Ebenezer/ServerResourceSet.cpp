// ServerResourceSet.cpp : implementation file
//

#include "stdafx.h"
#include "ebenezer.h"
#include "ServerResourceSet.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerResourceSet

IMPLEMENT_DYNAMIC(CServerResourceSet, CRecordset)

CServerResourceSet::CServerResourceSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CServerResourceSet)
	m_nResourceID = 0;
	m_strResource = _T("");
	// dungeon work
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CServerResourceSet::GetDefaultConnect()
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

CString CServerResourceSet::GetDefaultSQL()
{
	return _T("[dbo].[SERVER_RESOURCE]");
}

void CServerResourceSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CServerResourceSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[nResourceID]"), m_nResourceID);
	RFX_Text(pFX, _T("[strResource]"), m_strResource, 255);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CServerResourceSet diagnostics

#ifdef _DEBUG
void CServerResourceSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CServerResourceSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
