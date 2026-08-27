// BigFile.cpp: implementation of the CBigFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BigFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CBigFile, CFile)

CBigFile::CBigFile()
{

}

CBigFile::~CBigFile()
{

}

// .cpp dosyanýzdaki yeni hali:
ULONGLONG CBigFile::Seek(LONGLONG dOff, UINT nFrom)
{
    ASSERT_VALID(this);
    // m_hFile'in geçerli olduðundan emin olun
    if (m_hFile == INVALID_HANDLE_VALUE) // Veya m_hFile == (HANDLE)hFileNull
    {
        CFileException::ThrowOsError((LONG)ERROR_INVALID_HANDLE);
    }

    ASSERT(nFrom == begin || nFrom == end || nFrom == current);

    LARGE_INTEGER liOff, liNew;
    liOff.QuadPart = dOff;

    // SetFilePointerEx, 64-bit (Large Integer) deðerleri doðrudan destekler
    if (!::SetFilePointerEx((HANDLE)m_hFile, liOff, &liNew, (DWORD)nFrom))
    {
        CFileException::ThrowOsError((LONG)::GetLastError());
    }

    return (ULONGLONG)liNew.QuadPart;
}

