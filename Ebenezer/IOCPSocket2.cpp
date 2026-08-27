// IOCPSocket2.cpp: implementation of the CIOCPSocket2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IOCPSocket2.h"
#include "CircularBuffer.h"
#include "Compress.h"
#include "Packetdefine.h"
#include "define.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void bb() {};		// nop function

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIOCPSocket2::CIOCPSocket2()
{
	m_pBuffer = new CCircularBuffer(SOCKET_BUFF_SIZE);
	m_pRegionBuffer = new _REGION_BUFFER;
	m_pCompressMng = new CCompressMng;
	m_Socket = INVALID_SOCKET;

	m_pIOCPort = NULL;
	m_Type = TYPE_ACCEPT;

	// Cryption
	m_CryptionFlag = 0;
}

CIOCPSocket2::~CIOCPSocket2()
{
	delete m_pBuffer;
	delete m_pRegionBuffer;
	delete m_pCompressMng;
}


BOOL CIOCPSocket2::Create( UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)
{
	int ret;
	struct linger lingerOpt;

	m_Socket = socket( AF_INET, nSocketType/*SOCK_STREAM*/, 0 );
	if( m_Socket == INVALID_SOCKET ) {
		ret = WSAGetLastError();
		printf("Socket Create Fail! - %d\n", ret);
		return FALSE;
	}

	m_hSockEvent = WSACreateEvent();
	if( m_hSockEvent == WSA_INVALID_EVENT ) {
		ret = WSAGetLastError();
		printf("Event Create Fail! - %d\n", ret);
		return FALSE;
	}

	// Linger off -> close socket immediately regardless of existance of data 
	//
	lingerOpt.l_onoff = 0;
	lingerOpt.l_linger = 0;

	setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char *)&lingerOpt, sizeof(lingerOpt));

	int socklen;

	socklen = SOCKET_BUFF_SIZE*4;
	setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socklen, sizeof(socklen));
	
	socklen = SOCKET_BUFF_SIZE*4;
	setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socklen, sizeof(socklen));
	
	return TRUE;
}

BOOL CIOCPSocket2::Connect( CIOCPort* pIocp, LPCTSTR lpszHostAddress, UINT nHostPort )
{
	struct sockaddr_in addr;

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(lpszHostAddress);
	addr.sin_port = htons(nHostPort);

	int result = connect( m_Socket,(struct sockaddr *)&addr,sizeof(addr) );
	if ( result == SOCKET_ERROR )
	{
		int err = WSAGetLastError();
		printf("CONNECT FAIL : %d\n", err);
		closesocket( m_Socket );
		return FALSE;
	}

	ASSERT( pIocp );

	InitSocket( pIocp );

	m_Sid = m_pIOCPort->GetClientSid();
	if( m_Sid < 0 )
		return FALSE;

	m_pIOCPort->m_ClientSockArray[m_Sid] = this;
	
	if ( !m_pIOCPort->Associate(this, m_pIOCPort->m_hClientIOCPort) )
	{
		printf("Socket Connecting Fail - Associate\n");
		return FALSE;
	}

	m_ConnectAddress = lpszHostAddress;
	m_State = STATE_CONNECTED;
	m_Type = TYPE_CONNECT;

	Receive();

	return TRUE;
}
/*
int CIOCPSocket2::Send(char *pBuf, long length, int dwFlag)
{
	int ret_value = 0;
	WSABUF out;
	DWORD sent = 0;
	OVERLAPPED *pOvl;
	HANDLE	hComport = NULL;

	if( length > MAX_PACKET_SIZE )
		return 0;

	BYTE pTBuf[MAX_PACKET_SIZE];
	BYTE pTIBuf[MAX_PACKET_SIZE];
	BYTE pTOutBuf[MAX_PACKET_SIZE];
	memset( pTBuf, 0x00, MAX_PACKET_SIZE );
	memset( pTIBuf, 0x00, MAX_PACKET_SIZE );
	memset( pTOutBuf, 0x00, MAX_PACKET_SIZE );
	int index = 0;

	if( m_CryptionFlag )
	{
		unsigned short len = length + sizeof(WORD)+1+1;

		m_Sen_val++;
		m_Sen_val &= 0x00ffffff;

		pTIBuf[0] = 0xfc; // ¾ÏÈ£°¡ Á¤È®ÇÑÁö
		memcpy( pTIBuf+1, &m_Sen_val, sizeof(WORD)+1 );
		memcpy( pTIBuf+4, pBuf, length );
		jct.JvEncryptionFast( len, pTIBuf, pTOutBuf );
		
		pTBuf[index++] = (BYTE)PACKET_START1;
		pTBuf[index++] = (BYTE)PACKET_START2;
		memcpy( pTBuf+index, &len, 2 );
		index += 2;
		memcpy( pTBuf+index, pTOutBuf, len );
		index += len;
		pTBuf[index++] = (BYTE)PACKET_END1;
		pTBuf[index++] = (BYTE)PACKET_END2;
	}
	else
	{
		pTBuf[index++] = (BYTE)PACKET_START1;
		pTBuf[index++] = (BYTE)PACKET_START2;
		memcpy( pTBuf+index, &length, 2 );
		index += 2;
		memcpy( pTBuf+index, pBuf, length );
		index += length;
		pTBuf[index++] = (BYTE)PACKET_END1;
		pTBuf[index++] = (BYTE)PACKET_END2;
	}

	out.buf = (char*)pTBuf;
	out.len = index;
	
	pOvl = &m_SendOverlapped;
	pOvl->Offset = OVL_SEND;
	pOvl->OffsetHigh = out.len;

	ret_value = WSASend( m_Socket, &out, 1, &sent, dwFlag, pOvl, NULL);
	
	if ( ret_value == SOCKET_ERROR )
	{
		int last_err;
		last_err = WSAGetLastError();

		if ( last_err == WSA_IO_PENDING ) {
			printf("SEND : IO_PENDING[SID=%d]\n", m_Sid);
			m_nPending++;
			if( m_nPending > 3 )
				goto close_routine;
			sent = length; 
		}
		else if ( last_err == WSAEWOULDBLOCK )
		{
			printf("SEND : WOULDBLOCK[SID=%d]\n", m_Sid);

			m_nWouldblock++;
			if( m_nWouldblock > 3 )
				goto close_routine;
			return 0;
		}
		else
		{
			printf("SEND : ERROR [SID=%d] - %d\n", m_Sid, last_err);
//			char logstr[1024]; memset( logstr, NULL, 1024 );
//			sprintf( logstr, "SEND : ERROR [SID=%d] - %d\r\n", m_Sid, last_err);
//			LogFileWrite( logstr );
			m_nSocketErr++;
			goto close_routine;
		}
	}
	else if ( !ret_value )
	{
		m_nPending = 0;
		m_nWouldblock = 0;
		m_nSocketErr = 0;
	}

	return sent;

close_routine:
	pOvl = &m_RecvOverlapped;
	pOvl->Offset = OVL_CLOSE;
	
	if( m_Type == TYPE_ACCEPT )
		hComport = m_pIOCPort->m_hServerIOCPort;
	else
		hComport = m_pIOCPort->m_hClientIOCPort;
	
	PostQueuedCompletionStatus( hComport, (DWORD)0, (DWORD)m_Sid, pOvl );
	
	return -1;
}
*/
int CIOCPSocket2::Send(char* pBuf, long length, int dwFlag)
{
	int ret_value = 0;
	WSABUF out;
	DWORD sent = 0;
	OVERLAPPED* pOvl;
	HANDLE	hComport = NULL;

	if (length + 5 /* crypto */ >= MAX_SEND_SIZE)
		return 0;

	BYTE pTIBuf[MAX_SEND_SIZE], pTOutBuf[MAX_SEND_SIZE];
	memset(pTIBuf, 0x00, sizeof(pTIBuf));
	memset(pTOutBuf, 0x00, sizeof(pTOutBuf));
	int index = 0;

	if (m_CryptionFlag)
	{
		unsigned short len = (unsigned short)(length + sizeof(WORD) + 2 + 1);

		m_Sen_val++;
		m_Sen_val &= 0x00ffffff;

		pTIBuf[0] = 0xfc;
		pTIBuf[1] = 0x1e;
		memcpy(&pTIBuf[2], &m_Sen_val, sizeof(WORD));
		memcpy(&pTIBuf[5], pBuf, length);
		jct.JvEncryptionFast(len, pTIBuf, pTOutBuf);

		pTIBuf[index++] = (BYTE)PACKET_START1;
		pTIBuf[index++] = (BYTE)PACKET_START2;
		memcpy(pTIBuf + index, &len, 2);
		index += 2;
		memcpy(pTIBuf + index, pTOutBuf, len);
		index += len;
		pTIBuf[index++] = (BYTE)PACKET_END1;
		pTIBuf[index++] = (BYTE)PACKET_END2;
	}
	else
	{
		pTIBuf[index++] = (BYTE)PACKET_START1;
		pTIBuf[index++] = (BYTE)PACKET_START2;
		memcpy(pTIBuf + index, &length, 2);
		index += 2;
		memcpy(pTIBuf + index, pBuf, length);
		index += length;
		pTIBuf[index++] = (BYTE)PACKET_END1;
		pTIBuf[index++] = (BYTE)PACKET_END2;
	}

	out.buf = (char*)pTIBuf;
	out.len = index;

	pOvl = &m_SendOverlapped;
	pOvl->Offset = OVL_SEND;
	pOvl->OffsetHigh = out.len;

	ret_value = WSASend(m_Socket, &out, 1, &sent, dwFlag, pOvl, NULL);

	if (ret_value == SOCKET_ERROR)
	{
		int last_err;
		last_err = WSAGetLastError();

		if (last_err == WSA_IO_PENDING) {
			printf("SEND : IO_PENDING[SID=%d]\n", m_Sid);
			m_nPending++;
			if (m_nPending > 3)
				goto close_routine;
			sent = length;
		}
		else if (last_err == WSAEWOULDBLOCK)
		{
			printf("SEND : WOULDBLOCK[SID=%d]\n", m_Sid);

			m_nWouldblock++;
			if (m_nWouldblock > 3)
				goto close_routine;
			return 0;
		}
		else
		{
			printf("SEND : ERROR [SID=%d] - %d\n", m_Sid, last_err);
			//			char logstr[1024]; memset( logstr, NULL, 1024 );
			//			sprintf( logstr, "SEND : ERROR [SID=%d] - %d\r\n", m_Sid, last_err);
			//			LogFileWrite( logstr );
			m_nSocketErr++;
			goto close_routine;
		}
	}
	else if (!ret_value)
	{
		m_nPending = 0;
		m_nWouldblock = 0;
		m_nSocketErr = 0;
	}

	return sent;

close_routine:
	pOvl = &m_RecvOverlapped;
	pOvl->Offset = OVL_CLOSE;

	if (m_Type == TYPE_ACCEPT)
		hComport = m_pIOCPort->m_hServerIOCPort;
	else
		hComport = m_pIOCPort->m_hClientIOCPort;

	PostQueuedCompletionStatus(hComport, (DWORD)0, (DWORD)m_Sid, pOvl);

	return -1;
}
int CIOCPSocket2::Receive()
{
	int RetValue;
	WSABUF in;
	DWORD insize, dwFlag=0;
	OVERLAPPED *pOvl;
	HANDLE	hComport = NULL;

	memset(m_pRecvBuff, NULL, MAX_PACKET_SIZE );
	in.len = MAX_PACKET_SIZE;
	in.buf = m_pRecvBuff;

	pOvl = &m_RecvOverlapped;
	pOvl->Offset = OVL_RECEIVE;

	RetValue = WSARecv( m_Socket, &in, 1, &insize, &dwFlag, pOvl, NULL );

 	if ( RetValue == SOCKET_ERROR )
	{
		int last_err;
		last_err = WSAGetLastError();

		if ( last_err == WSA_IO_PENDING ) {
//			printf("RECV : IO_PENDING[SID=%d]\n", m_Sid);
//			m_nPending++;
//			if( m_nPending > 3 )
//				goto close_routine;
			return 0;
		}
		else if ( last_err == WSAEWOULDBLOCK )
		{
			printf("RECV : WOULDBLOCK[SID=%d]\n", m_Sid);

			m_nWouldblock++;
			if( m_nWouldblock > 3 )
				goto close_routine;
			return 0;
		}
		else
		{
			printf("RECV : ERROR [SID=%d] - %d\n", m_Sid, last_err);

			// Fixing count down attacks
			//if( m_State == STATE_CONNECTED ) { //
			//	if( m_State == STATE_GAMESTART ) { //

					m_nSocketErr++;
					if( m_nSocketErr == 2 )
						goto close_routine;
					return -1;

			//	} //
			//} //
		}
	}

	return (int)insize;

close_routine:
	pOvl = &m_RecvOverlapped;
	pOvl->Offset = OVL_CLOSE;
	
	if( m_Type == TYPE_ACCEPT )
		hComport = m_pIOCPort->m_hServerIOCPort;
	else
		hComport = m_pIOCPort->m_hClientIOCPort;
	
	PostQueuedCompletionStatus( hComport, (DWORD)0, (DWORD)m_Sid, pOvl );
	
	return -1;
}

void CIOCPSocket2::ReceivedData(int length)
{
	if(!length) return;

	int len = 0;

	if( !strlen(m_pRecvBuff) )		// ÆÐÅ¶±æÀÌ´Â Á¸ÀçÇÏ³ª ½Ç µ¥ÀÌÅÍ°¡ ¾ø´Â °æ¿ì°¡ ¹ß»ý...
		return;
	m_pBuffer->PutData(m_pRecvBuff, length);		// ¹ÞÀº Data¸¦ ¹öÆÛ¿¡ ³Ö´Â´Ù
	
	char *pData = NULL;
	char *pDecData = NULL;

	while (PullOutCore(pData, len))
	{
		if(pData)
		{
			Parsing(len, pData);//		½ÇÁ¦ ÆÄ½Ì ÇÔ¼ö...

			delete[] pData;
			pData = NULL;
		}
	}
}
/*
BOOL CIOCPSocket2::PullOutCore(char *&data, int &length)
{
	BYTE		*pTmp;
	BYTE		*pBuff;
	int			len;
	BOOL		foundCore;
	MYSHORT		slen;
	DWORD		wSerial = 0;
	int index = 1, recv_packet = 0;

	len = m_pBuffer->GetValidCount();

	if(len == 0 || len < 0) return FALSE;

	pTmp = new BYTE[len];

	m_pBuffer->GetData((char*)pTmp, len);

	foundCore = FALSE;

	int	sPos=0, ePos = 0;

	for (int i = 0; i < len && !foundCore; i++)
	{
		if (i+2 >= len) break;

		if (pTmp[i] == PACKET_START1 && pTmp[i+1] == PACKET_START2)
		{
//			if( m_wPacketSerial >= wSerial ) goto cancelRoutine;
			sPos = i+2;

			slen.b[0] = pTmp[sPos];
			slen.b[1] = pTmp[sPos + 1];

			length = slen.w;

			if( length < 0 ) goto cancelRoutine;
			if( length > len ) goto cancelRoutine;

			ePos = sPos+length + 2;

			if( (ePos + 2) > len ) goto cancelRoutine;
//			ASSERT(ePos+2 <= len);

			if (pTmp[ePos] == PACKET_END1 && pTmp[ePos+1] == PACKET_END2)
			{
				if( m_CryptionFlag )	{	// ¾ÏÈ£È­
					pBuff = new BYTE[length+1]; 
					jct.JvDecryptionFast( length, (unsigned char*)pTmp+sPos+2, pBuff );
					/*
					if(pBuff[0] != 0xfc)	{ // ¾ÐÃà Ç¬ µ¥ÀÌÅÍ ¿À·ù ÀÏ°æ¿ì ¹öÆÛ¿¡¼­ »èÁ¦ ÇØ¹ö¸°´Ù
						printf("CIOCPSocket2::PutOutCore - Decryption Error... sockid(%d)\n", m_Socket);
						delete[] pBuff;
						Close();
						goto cancelRoutine;
					}
					
					recv_packet = (WORD)GetShort( (char*)pBuff, index );
					//printf("^^^ IOCPSocket2,, PullOutCore ,,, recv_val = %d ^^^\n", recv_packet);
					if( m_Rec_val >= recv_packet )	{	// ¹«½Ã,,
						if( recv_packet == 0 )	{
							m_Rec_val = recv_packet;
						}
						else	{
							printf("CIOCPSocket2::PutOutCore - recv_packet Error... sockid(%d), len=%d, recv_packet=%d \n", m_Socket, length, recv_packet);
							delete[] pBuff;
							m_pBuffer->HeadIncrease(6+length); //6: header 2+ end 2+ length 2 + cryption 4
							goto cancelRoutine;
						}
					}
					else	m_Rec_val = recv_packet;

					length = length-4;
					if( length <= 0 )	{
						printf("CIOCPSocket2::PutOutCore - length Error... sockid(%d), len=%d\n", m_Socket, length);
						delete[] pBuff;
						Close();
						goto cancelRoutine;
					}
					data = new char[length+1];
					CopyMemory((void *)data, (const void *)(pBuff+4), length);
					data[length] = 0;
					foundCore = TRUE;
					int head = m_pBuffer->GetHeadPos(), tail = m_pBuffer->GetTailPos();
					delete[] pBuff;
				}
				else	{					// 
					data = new char[length+1];
					CopyMemory((void *)data, (const void *)(pTmp+sPos+2), length);
					data[length] = 0;
					foundCore = TRUE;
					int head = m_pBuffer->GetHeadPos(), tail = m_pBuffer->GetTailPos();
					//printf("data : %s, len : %d\n", data, length);
				}
//				printf("data : %s, len : %d\n", data, length);
//				printf("head : %d, tail : %d\n", head, tail );
				break;
			}
			else 
			{
				m_pBuffer->HeadIncrease(3);
				break;
			}
		}
	}

	if( m_CryptionFlag )	{
		if (foundCore) m_pBuffer->HeadIncrease(10+length); //6: header 2+ end 2+ length 2 + cryption 4
	}
	else	{
		if (foundCore) m_pBuffer->HeadIncrease(6+length); //6: header 2+ end 2+ length 2
	}

	delete[] pTmp;

	return foundCore;

cancelRoutine:	
	delete[] pTmp;
	return foundCore;
}
*/
BOOL CIOCPSocket2::PullOutCore(char*& data, int& length)
{
	BYTE* pTmp = NULL;
	BYTE* pBuff = NULL;
	int len = 0;
	BOOL foundCore = FALSE;
	MYSHORT slen;
	DWORD recv_packet = 0;
	int index = 1;

	// 1. Buffer geçerlilik kontrolü
	len = m_pBuffer->GetValidCount();
	if (len <= 0 || len >= MAX_PACKET_SIZE)
		return FALSE;

	pTmp = new BYTE[len];
	if (pTmp == NULL) return FALSE;

	m_pBuffer->GetData((char*)pTmp, len);
	foundCore = FALSE;

	int sPos = 0, ePos = 0;

	for (int i = 0; i < len && !foundCore; i++)
	{
		// Minimum paket boyutu kontrolü (Start1, Start2, Length1, Length2, End1, End2)
		if (i + 5 >= len)
			break;

		// Paket baþlangýç imzasýný ara (0xAA, 0x55)
		if (pTmp[i] == PACKET_START1 && pTmp[i + 1] == PACKET_START2)
		{
			sPos = i + 2;

			slen.b[0] = pTmp[sPos];
			slen.b[1] = pTmp[sPos + 1];

			length = slen.w; // Paketin net uzunluðu

			// Uzunluk doðrulamasý
			if (length < 0 || length >= MAX_PACKET_SIZE || (sPos + length + 2) > len)
				goto cancelRoutine;

			ePos = sPos + length + 2;

			// Paket bitiþ imzasýný kontrol et (0x55, 0xAA)
			if (pTmp[ePos] == PACKET_END1 && pTmp[ePos + 1] == PACKET_END2)
			{
				if (m_CryptionFlag) // Þifreleme aktifse
				{
					pBuff = new BYTE[length + 1];
					memset(pBuff, 0x00, length + 1);

					// JCT Decryption: Þifreyi çöz
					if (jct.JvDecryptionWithCRC32(length, (unsigned char*)pTmp + sPos + 2, pBuff) < 0)
					{
						printf("CIOCPSocket2::PullOutCore - Decryption Error... sockid(%d)\n", m_Socket);
						if (pBuff) delete[] pBuff;
						Close(); // Þifreleme bozulduysa soketi kapat
						goto cancelRoutine;
					}

					// Paketin içindeki sýra numarasýný (serial) oku
					index = 0; // Index sýfýrlanmalý
					recv_packet = GetDWORD((char*)pBuff, index);

					// Sunucu sayacýný güncelle (Bazý sourcelarda m_Rec_val++ kullanýlýr)
					m_Rec_val = recv_packet;

					// Paket boyutu düzeltme (4 byte Serial + 4 byte CRC çýkarýlýr)
					length = length - 8;

					if (length <= 0 || length >= MAX_PACKET_SIZE) {
						if (pBuff) delete[] pBuff;
						goto cancelRoutine;
					}

					data = new char[length + 1];
					// Serial'dan sonraki asýl veriyi kopyala (index 4'ten baþlar)
					CopyMemory((void*)data, (const void*)(pBuff + 4), length);
					data[length] = 0;
					foundCore = TRUE;

					if (pBuff) delete[] pBuff;
				}
				else // Þifreleme kapalýysa (Ham paket)
				{
					data = new char[length + 1];
					CopyMemory((void*)data, (const void*)(pTmp + sPos + 2), length);
					data[length] = 0;
					foundCore = TRUE;
				}
				break;
			}
			else
			{
				// Yanlýþ bitiþ imzasý, buffer'ý kaydýr
				m_pBuffer->HeadIncrease(1);
				goto cancelRoutine;
			}
		}
	}

	// Paket baþarýyla çekildiyse buffer kafasýný ilerlet
	if (foundCore)
	{
		// 1298 Protokolü: Header(2) + Length(2) + Data(length) + End(2)
		m_pBuffer->HeadIncrease(length + (m_CryptionFlag ? 14 : 6));
	}

cancelRoutine:
	if (pTmp) delete[] pTmp;
	return foundCore;
}

BOOL CIOCPSocket2::AsyncSelect( long lEvent )
{
	int retEventResult, err;

	retEventResult = WSAEventSelect( m_Socket, m_hSockEvent, lEvent );
	err = WSAGetLastError();

	return ( !retEventResult );
}

BOOL CIOCPSocket2::SetSockOpt( int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel )
{
	int retValue;
	retValue = setsockopt( m_Socket, nLevel, nOptionName, (char *)lpOptionValue, nOptionLen );

	return ( !retValue );
}

BOOL CIOCPSocket2::ShutDown( int nHow )
{
	int retValue;
	retValue = shutdown( m_Socket, nHow );

	return ( !retValue );
}

void CIOCPSocket2::Close()
{
	if ( m_pIOCPort == NULL ) return;

	HANDLE	hComport = NULL;
	OVERLAPPED		*pOvl;
	pOvl = &m_RecvOverlapped;
	pOvl->Offset = OVL_CLOSE;

	if( m_Type == TYPE_ACCEPT )
		hComport = m_pIOCPort->m_hServerIOCPort;
	else
		hComport = m_pIOCPort->m_hClientIOCPort;

	int retValue = PostQueuedCompletionStatus( hComport, (DWORD)0, (DWORD)m_Sid, pOvl );

	if ( !retValue ) {
		int errValue;
		errValue = GetLastError();
		printf("PostQueuedCompletionStatus Error : %d\n", errValue);
	}
}

void CIOCPSocket2::CloseProcess()
{
	m_State = STATE_DISCONNECTED;

	if( m_Socket != INVALID_SOCKET )
		closesocket( m_Socket );
}

void CIOCPSocket2::InitSocket( CIOCPort* pIOCPort )
{
	m_pIOCPort = pIOCPort;
	m_RecvOverlapped.hEvent = NULL;
	m_SendOverlapped.hEvent = NULL;
	m_pBuffer->SetEmpty();
	m_nSocketErr = 0;
	m_nPending = 0;
	m_nWouldblock = 0;

	Initialize();
}

BOOL CIOCPSocket2::Accept( SOCKET listensocket, struct sockaddr* addr, int* len )
{
	m_Socket = accept( listensocket, addr, len);
	if( m_Socket == INVALID_SOCKET) {
		int err = WSAGetLastError();
		printf("Socket Accepting Fail - %d\n", err);
		char logstr[1024]; memset( logstr, NULL, 1024 );
		sprintf( logstr, "Socket Accepting Fail - %d\r\n", err);
		LogFileWrite( logstr );
		return FALSE;
	}

//	int flag = 1;
//	setsockopt(m_Socket, SOL_SOCKET, SO_DONTLINGER, (char *)&flag, sizeof(flag));

//	int lensize, socklen=0;

//	getsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socklen, &lensize);
//	printf("getsockopt : %d\n", socklen);

//	struct linger lingerOpt;

//	lingerOpt.l_onoff = 1;
//	lingerOpt.l_linger = 0;

//	setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char *)&lingerOpt, sizeof(lingerOpt));

	return TRUE;
}

void CIOCPSocket2::Parsing(int length, char *pData)
{

}

void CIOCPSocket2::Initialize()
{
	m_wPacketSerial = 0;
	m_pRegionBuffer->iLength = 0;
	memset( m_pRegionBuffer->pDataBuff, 0x00, REGION_BUFF_SIZE );
	m_CryptionFlag = 0;
}

void CIOCPSocket2::SendCompressingPacket(const char *pData, int len)
{
	if( len <= 0 || len >= 49152)	{
		printf("### SendCompressingPacket Error : len = %d ### \n", len);
		return;
	}

	int send_index = 0, count = 0;
	char send_buff[49152];		memset(send_buff, 0x00, 49152);
	do {
		if( m_pCompressMng->m_nBufferStatus == W ) {
			bb();
			count++;
			continue;
		}
		m_pCompressMng->m_nBufferStatus = W;
		m_pCompressMng->m_dwThreadID =  ::GetCurrentThreadId();
		bb();
		if( m_pCompressMng->m_dwThreadID != ::GetCurrentThreadId() ) {	// Dual Lock System...
			count++;
			continue;
		}
		m_pCompressMng->PreCompressWork(pData, len);
		m_pCompressMng->Compress();

		SetByte(send_buff, WIZ_COMPRESS_PACKET, send_index );
		SetShort(send_buff, (short)m_pCompressMng->m_nOutputBufferCurPos, send_index );
		SetShort(send_buff, (short)m_pCompressMng->m_nOrgDataLength, send_index );
		SetDWORD(send_buff, m_pCompressMng->m_dwCrc, send_index);
		SetString( send_buff, m_pCompressMng->m_pOutputBuffer, m_pCompressMng->m_nOutputBufferCurPos, send_index);

		m_pCompressMng->Initialize();	// buffer clear
		m_pCompressMng->m_nBufferStatus = E;
		break;
	} while( count < 50 );
	if( count > 49 ) {
		printf("Compressing Fail Packet\n");
		Send( (char*)pData, len );
	}
	else
		Send( send_buff, send_index );
}

void CIOCPSocket2::RegionPacketAdd(char *pBuf, int len)
{
	int count = 0;
	do {
		if( m_pRegionBuffer->bFlag == W ) {
			bb();
			count++;
			continue;
		}
		m_pRegionBuffer->bFlag = W;
		m_pRegionBuffer->dwThreadID = ::GetCurrentThreadId();
		bb();
		if( m_pRegionBuffer->dwThreadID != ::GetCurrentThreadId() ) {	// Dual Lock System...
			count++;
			continue;
		}
		SetShort( m_pRegionBuffer->pDataBuff, len, m_pRegionBuffer->iLength );
		SetString( m_pRegionBuffer->pDataBuff, pBuf, len, m_pRegionBuffer->iLength );
		m_pRegionBuffer->bFlag = WR;
		break;
	} while( count < 30 );
	if( count > 29 ) {
//		printf("Region packet Add Drop\n");
		Send( pBuf, len );
	}
}

void CIOCPSocket2::RegioinPacketClear(char* GetBuf, int& len)
{
	int count = 0;
	do {
		if( m_pRegionBuffer->bFlag == W ) {
			bb();
			count++;
			continue;
		}
		m_pRegionBuffer->bFlag = W;
		m_pRegionBuffer->dwThreadID = ::GetCurrentThreadId();
		bb();
		if( m_pRegionBuffer->dwThreadID != ::GetCurrentThreadId() ) {	// Dual Lock System...
			count++;
			continue;
		}
		
		int index = 0;
		SetByte( GetBuf, WIZ_CONTINOUS_PACKET, index );
		SetShort( GetBuf, m_pRegionBuffer->iLength, index );
		SetString( GetBuf, m_pRegionBuffer->pDataBuff, m_pRegionBuffer->iLength, index );
		len = index;

		memset( m_pRegionBuffer->pDataBuff, 0x00, REGION_BUFF_SIZE );
		m_pRegionBuffer->iLength = 0;
		m_pRegionBuffer->bFlag = E;
		break;
	} while( count < 30 );
	if( count > 29 )
		printf("Region packet Clear Drop\n");
}

// Cryption
void CIOCPSocket2::SendCryptionKey()
{
	char send_buff[128]; memset(send_buff, 0x00, 128);
	int send_index = 0;
	//SetByte(send_buff, WIZ_CRYPTION, send_index);
	SetInt64(send_buff, m_Public_key, send_index);

	// Þifreleme motorunun (JCT) baþlangýç sayaçlarýný SIFIRLA
	m_Sen_val = 0;
	m_Rec_val = 0;

	Send(send_buff, send_index);
	m_CryptionFlag = 1; // Artýk þifreleme aktif
}


///~