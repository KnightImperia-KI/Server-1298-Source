// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "versionmanager.h"
#include "versionmanagerdlg.h"
#include "User.h"

#pragma warning(disable : 4786)		// Visual C++ Only
#include <set>

CUser::CUser()
{

}

CUser::~CUser()
{

}

void CUser::Initialize()
{
	m_pMain = (CVersionManagerDlg*)AfxGetApp()->GetMainWnd();

	CIOCPSocket2::Initialize();
}

void CUser::CloseProcess()
{

	CIOCPSocket2::CloseProcess();
}

void CUser::Parsing(int len, char *pData)
{
	int index = 0, send_index = 0, i=0, client_version = 0;
	char buff[2048]; memset( buff, 0x00, 2048 );
	BYTE command = GetByte( pData, index );

	switch( command ) {
	case LS_VERSION_REQ:
		SetByte( buff, LS_VERSION_REQ, send_index );
		SetShort( buff, m_pMain->m_nLastVersion, send_index );
		Send( buff, send_index );
		m_pMain->m_OutputList.AddString("[ Version Information Sent... ]");
		break;
	case LS_WHATS_NEW:
	{
		int NewsLength = 0;
		SetByte(buff, LS_WHATS_NEW, send_index);
		SetShort(buff, strlen("Nero"), send_index);
		SetString(buff, "Nero", strlen("Nero"), send_index);
		for (i = 0; i < m_pMain->m_nNoticeCount; i++) {
			NewsLength = NewsLength + strlen(m_pMain->m_NoticeList[i]->strNotice) + strlen(m_pMain->m_NoticeList[i]->strTitle);
		}
		NewsLength += (m_pMain->m_nNoticeCount) * 6;
		SetShort(buff, NewsLength, send_index);
		for (i = 0; i < m_pMain->m_NoticeList.size(); i++) {
			SetString(buff, m_pMain->m_NoticeList[i]->strTitle, strlen(m_pMain->m_NoticeList[i]->strTitle), send_index);
			SetString(buff, "#", strlen("#"), send_index);
			SetShort(buff, 10, send_index);
			SetString(buff, m_pMain->m_NoticeList[i]->strNotice, strlen(m_pMain->m_NoticeList[i]->strNotice), send_index);
			SetShort(buff, 10, send_index);
			SetString(buff, "#", strlen("#"), send_index);
		}
	}
	Send(buff, send_index);
	m_pMain->m_OutputList.AddString("[ What is News Sent... ]");
	break;
	case LS_SERVERLIST:
		m_pMain->m_DBProcess.LoadUserCountList();		// 기범이가 ^^;
		SetByte( buff, LS_SERVERLIST, send_index );
		SetByte( buff, m_pMain->m_nServerCount, send_index );
		for(i=0; i<m_pMain->m_ServerList.size(); i++) {		
			SetShort( buff, strlen(m_pMain->m_ServerList[i]->strServerIP), send_index );
			SetString( buff, m_pMain->m_ServerList[i]->strServerIP, strlen(m_pMain->m_ServerList[i]->strServerIP), send_index );
			SetShort( buff, strlen(m_pMain->m_ServerList[i]->strServerName), send_index );
			SetString( buff, m_pMain->m_ServerList[i]->strServerName, strlen( m_pMain->m_ServerList[i]->strServerName ), send_index );			
			SetShort( buff, m_pMain->m_ServerList[i]->sUserCount, send_index);   // 기범이가 ^^;
		}
		Send( buff, send_index );
		m_pMain->m_OutputList.AddString("[ Server List Sent... ]");
		break;

	case LS_DOWNLOADINFO_REQ:
		client_version = GetShort( pData, index );
		SendDownloadInfo( client_version );
		m_pMain->m_OutputList.AddString("[ The version is old; update information has been sent.... ]");
		break;
	case LS_LOGIN_REQ:
		LogInReq( pData+index );
		break;
	case LS_MGAME_LOGIN:
		MgameLogin( pData+index );
		break;
	}
}
/*
void CUser::LogInReq(char* pBuf)
{
	int index = 0, idlen = 0, pwdlen = 0, send_index = 0, result = 0, serverno = 0;
	BOOL bCurrentuser = FALSE;
	char send_buff[256]; memset(send_buff, 0x00, 256);
	char serverip[20]; memset(serverip, 0x00, 20);
	//char accountid[MAX_ID_SIZE+1], pwd[13];
	char accountid[MAX_ID_SIZE + 1], pwd[13], strclientip[32];;
	char LBMessage[256]; memset(LBMessage, 0x00, 256);
	memset(accountid, NULL, MAX_ID_SIZE + 1);
	memset(pwd, NULL, 13);

	idlen = GetShort(pBuf, index);
	if (idlen > MAX_ID_SIZE || idlen <= 0)
		goto fail_return;
	GetString(accountid, pBuf, idlen, index);
	pwdlen = GetShort(pBuf, index);
	if (pwdlen > 12 || pwdlen < 0)
		goto fail_return;
	GetString(pwd, pBuf, pwdlen, index);

	result = m_pMain->m_DBProcess.AccountLogin(accountid, pwd);
	SetByte(send_buff, LS_LOGIN_REQ, send_index);
	if (result == 1) { // success 
		bCurrentuser = m_pMain->m_DBProcess.IsCurrentUser(accountid, serverip, serverno);
		if (bCurrentuser) {
			result = 0x05;		// Kick out
			SetByte(send_buff, result, send_index);
			SetShort(send_buff, strlen(serverip), send_index);
			SetString(send_buff, serverip, strlen(serverip), send_index);
			SetShort(send_buff, serverno, send_index);
			sprintf(LBMessage, "[ ID : %s / PW : %s -> The player is already in the game]", accountid, pwd);
		}
		else
			SetByte(send_buff, result, send_index);
		sprintf(LBMessage, "[ ID : %s / PW : %s -> Player logged in]", accountid, pwd);

		result = m_pMain->m_DBProcess.IsPremiumUser(accountid);
		if (result > 0) {
			SetShort(send_buff, result, send_index);
			sprintf(LBMessage, "[ ID : %s -> Premium Player logged in ]",
				accountid);
		}
	}
	else
	{
		SetByte(send_buff, result, send_index);
	sprintf(LBMessage, "[ ID : %s / PW : %s -> The player was unable to log in]", accountid, pwd);
}
	m_pMain->m_OutputList.AddString(LBMessage);
	Send(send_buff, send_index);

	return;
fail_return:
	SetByte( send_buff, LS_LOGIN_REQ, send_index );
	SetByte( send_buff, 0x02, send_index );				// id, pwd 이상...
	Send( send_buff, send_index );
}
*/
void CUser::LogInReq(char* pBuf)
{
	int index = 0, idlen = 0, pwdlen = 0, send_index = 0, result = 0, serverno = 0;
	BOOL bCurrentuser = FALSE;

	char send_buff[256];
	memset(send_buff, 0x00, sizeof(send_buff));

	char serverip[20];
	memset(serverip, 0x00, sizeof(serverip));

	char accountid[MAX_ID_SIZE + 1];
	char pwd[13];
	char strclientip[32];

	char LBMessage[256];
	memset(LBMessage, 0x00, sizeof(LBMessage));

	memset(accountid, 0x00, sizeof(accountid));
	memset(pwd, 0x00, sizeof(pwd));

	idlen = GetShort(pBuf, index);

	if (idlen > MAX_ID_SIZE || idlen <= 0)
		goto fail_return;

	GetString(accountid, pBuf, idlen, index);

	pwdlen = GetShort(pBuf, index);

	if (pwdlen > 12 || pwdlen < 0)
		goto fail_return;

	GetString(pwd, pBuf, pwdlen, index);

	if (!IsValidName(accountid))
	{
		sprintf(
			LBMessage,
			"[ LOGIN ERROR ] Invalid account id : %s",
			accountid);

		m_pMain->m_OutputList.AddString(LBMessage);

		goto fail_return;
	}

	if (!IsValidName(pwd))
	{
		sprintf(
			LBMessage,
			"[ LOGIN ERROR ] Invalid password attempt for ID : %s",
			accountid);

		m_pMain->m_OutputList.AddString(LBMessage);

		goto fail_return;
	}

	// ACCOUNT LOGIN
	result = m_pMain->m_DBProcess.AccountLogin(accountid, pwd);

	SetByte(send_buff, LS_LOGIN_REQ, send_index);

	if (result == 1) // LOGIN SUCCESS
	{
		// PREMIUM CONTROL
		int premium = m_pMain->m_DBProcess.IsPremiumUser(accountid);

		// PREMIUM YOK
		if (premium <= 0)
		{
			SetByte(send_buff, 0x04, send_index);

			sprintf(
				LBMessage,
				"[ ID : %s / PW : %s -> Premium not found ]",
				accountid,
				pwd);

			m_pMain->m_OutputList.AddString(LBMessage);

			Send(send_buff, send_index);
			return;
		}

		// CURRENT USER CONTROL
		bCurrentuser = m_pMain->m_DBProcess.IsCurrentUser(accountid, serverip, serverno);

		if (bCurrentuser)
		{
			result = 0x05; // already online

			SetByte(send_buff, result, send_index);

			SetShort(send_buff, strlen(serverip), send_index);
			SetString(send_buff, serverip, strlen(serverip), send_index);

			SetShort(send_buff, serverno, send_index);

			sprintf(
				LBMessage,
				"[ ID : %s / PW : %s -> The player is already in the game ]",
				accountid,
				pwd);
		}
		else
		{
			// LOGIN OK
			SetByte(send_buff, 1, send_index);

			// PREMIUM TYPE
			SetShort(send_buff, premium, send_index);

			sprintf(
				LBMessage,
				"[ ID : %s / PW : %s -> Premium Player logged in ]",
				accountid,
				pwd);
		}
	}
	else
	{
		// LOGIN FAILED
		SetByte(send_buff, result, send_index);

		sprintf(
			LBMessage,
			"[ ID : %s / PW : %s -> The player was unable to log in ]",
			accountid,
			pwd);
	}

	m_pMain->m_OutputList.AddString(LBMessage);

	Send(send_buff, send_index);

	return;

fail_return:

	SetByte(send_buff, LS_LOGIN_REQ, send_index);
	SetByte(send_buff, 0x02, send_index);

	sprintf(
		LBMessage,
		"[ LOGIN ERROR ] Invalid ID/PW packet");

	m_pMain->m_OutputList.AddString(LBMessage);

	Send(send_buff, send_index);
}
void CUser::MgameLogin(char *pBuf)
{
	int index = 0, idlen=0, pwdlen = 0, send_index = 0, result = 0;
	char send_buff[256]; memset( send_buff, 0x00, 256 );
	char accountid[MAX_ID_SIZE+1], pwd[13];
	memset( accountid, NULL, MAX_ID_SIZE+1 );
	memset( pwd, NULL, 13);

	idlen = GetShort( pBuf, index );
	if( idlen > MAX_ID_SIZE || idlen <= 0)
		goto fail_return;
	GetString( accountid, pBuf, idlen, index );
	pwdlen = GetShort( pBuf, index );
	if( pwdlen > 12 )
		goto fail_return;
	GetString( pwd, pBuf, pwdlen, index );

	result = m_pMain->m_DBProcess.MgameLogin( accountid, pwd );
	SetByte( send_buff, LS_MGAME_LOGIN, send_index );
	SetByte( send_buff, result, send_index );
	Send( send_buff, send_index );

	return;
fail_return:
	SetByte( send_buff, LS_MGAME_LOGIN, send_index );
	SetByte( send_buff, 0x02, send_index );				// login fail...
	Send( send_buff, send_index );
}

void CUser::SendDownloadInfo(int version)
{
	int send_index = 0, filecount = 0;
	_VERSION_INFO *pInfo = NULL;
	std::set <string>	downloadset;
	char buff[2048]; memset( buff, 0x00, 2048 );

	std::map <string, _VERSION_INFO*>::iterator	Iter1, Iter2;
	Iter1 = m_pMain->m_VersionList.m_UserTypeMap.begin();
	Iter2 = m_pMain->m_VersionList.m_UserTypeMap.end();
	for( ; Iter1 != Iter2; Iter1++ ) {
		pInfo = (*Iter1).second;
		if( pInfo->sVersion > version )
			downloadset.insert(pInfo->strCompName);
	}

	SetByte( buff, LS_DOWNLOADINFO_REQ, send_index );
	SetShort( buff, strlen( m_pMain->m_strFtpUrl), send_index );
	SetString( buff, m_pMain->m_strFtpUrl, strlen( m_pMain->m_strFtpUrl), send_index );
	SetShort( buff, strlen( m_pMain->m_strFilePath), send_index );
	SetString( buff, m_pMain->m_strFilePath, strlen( m_pMain->m_strFilePath), send_index );
	SetShort( buff, downloadset.size(), send_index );
	
	std::set <string>::iterator filenameIter1, filenameIter2;
	filenameIter1 = downloadset.begin();
	filenameIter2 = downloadset.end();
	for(; filenameIter1 != filenameIter2; filenameIter1++ ) {
		SetShort( buff, strlen( (*filenameIter1).c_str() ), send_index );
		SetString( buff, (char*)((*filenameIter1).c_str()), strlen( (*filenameIter1).c_str() ), send_index );
	}
	Send( buff, send_index );
}
