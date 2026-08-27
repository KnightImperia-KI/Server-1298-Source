#if !defined(AFX_STDAFX_H__N3BASE__INCLUDED_)
#define AFX_STDAFX_H__N3BASE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Windows Sürümleri Tanýmlarý (VS 2022 Uyumu Ýçin)
#ifndef WINVER
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#endif

// Standart MFC ve Windows Kütüphaneleri
#include <afxwin.h>         // MFC standart bileþenleri
#include <afxext.h>         // MFC uzantýlarý
#include <afxdtctl.h>       // MFC Internet Explorer 4 Common Controls desteði
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC Windows Common Controls desteði
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <list>

// Proje özel baþlýklarý
#include "My_3DStruct.h"

/////////////////////////////////////////////////////////////////////////////
// Bellek Yönetimi ve Hata Ayýklama Tanýmlarý

#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
    // Debug modunda özel bellek izleme
#define DEBUG_NEW new(THIS_FILE, __LINE__)
#else
    // Release modunda standart tanýmlar
#ifndef DEBUG_NEW
#define DEBUG_NEW new
#endif

#define AfxCheckMemory() TRUE
#define AfxIsMemoryBlock(p, nBytes) TRUE
#define AfxEnableMemoryTracking(bTrack) FALSE
#define AfxOutputDebugString(lpsz) ::OutputDebugString(lpsz)
#define AfxDiagnosticInit() TRUE
#endif

/////////////////////////////////////////////////////////////////////////////
// printf Makrosu Çakýþma Giderici
#ifdef printf
#undef printf
#endif

#if defined(_DEBUG)
#define printf printf
#else
    // Release modunda printf fonksiyonunu printf'e veya boþ iþleme yönlendirir
#define printf 1 ? (void)0 : printf
#endif

/////////////////////////////////////////////////////////////////////////////
// Gereksiz Uyarýlarý Kapatma
#pragma warning(disable:4786) // Karakter sýnýrý uyarýsý (STL için)
#pragma warning(disable:4996) // Eski fonksiyon kullanýmý uyarýsý (strcpy vb.)
#pragma warning(disable:4005) // Makro yeniden tanýmlama uyarýsý

#endif // !defined(AFX_STDAFX_H__N3BASE__INCLUDED_)
