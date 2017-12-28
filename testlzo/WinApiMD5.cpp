#include "stdafx.h"
#include "WinApiMD5.h"

CWinApiMD5::CWinApiMD5()
{
	pMD5Init = NULL;
	pMD5Update = NULL;
	pMD5Final = NULL;

	HINSTANCE hDLL;
	if ((hDLL = LoadLibraryA("advapi32.dll")) > 0)
	{
		pMD5Init = (PMD5Init)GetProcAddress(hDLL, "MD5Init");
		pMD5Update = (PMD5Update)GetProcAddress(hDLL, "MD5Update");
		pMD5Final = (PMD5Final)GetProcAddress(hDLL, "MD5Final");
	}
}

CWinApiMD5::~CWinApiMD5()
{
}

TCHAR* CWinApiMD5::GetMD5(unsigned char* pdatabuf, int datasize)
{
	MD5_CTX ctx;
	pMD5Init(&ctx);
	pMD5Update(&ctx, pdatabuf, datasize);
	pMD5Final(&ctx);

	int pos;
	for (pos = 0; pos < 16; pos++)
		swprintf_s(m_MD5Str + (pos * 2), 34 - pos * 2, _T("%02x"), ctx.digest[pos]);
	return m_MD5Str;
}
