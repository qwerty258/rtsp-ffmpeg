// Author:			Ï²Ö®ÀÇ¤Î¿ã×Ó
// Contact:			70565912@qq.com
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <assert.h>
#include <tchar.h>
#include <string>
using namespace std;

static wstring a2w_cp(const string& _str, const UINT& _cp)
{
	if ( !_str.size() )
		return L"";

	int nCharCount = MultiByteToWideChar(_cp, 0, _str.c_str(), (int)_str.size(), NULL, 0) + 1;
	WCHAR* pBuffer = new WCHAR[nCharCount];
	pBuffer[nCharCount-1] = L'\0';

	MultiByteToWideChar(_cp, 0,	_str.c_str(), (int)_str.size(), pBuffer, nCharCount);
	wstring _wstr = pBuffer;
	delete [] pBuffer;

	return _wstr;	
};

static string w2a_cp(const wstring& _wstr, const UINT& _cp)
{
	if ( !_wstr.size() )
		return "";

	int nCharCount = WideCharToMultiByte(_cp, 0, _wstr.c_str(), (int)_wstr.size(), NULL, 0,	NULL, NULL) + 1;
	CHAR* pBuffer = new CHAR[nCharCount];
	pBuffer[nCharCount-1] = '\0';

	WideCharToMultiByte(_cp, 0, _wstr.c_str(), (int)_wstr.size(), pBuffer, nCharCount, NULL, NULL);
	string _str = pBuffer;
	delete [] pBuffer;

	return _str;
};

#define a2w(_str) a2w_cp(_str, CP_ACP)
#define w2a(_wstr) w2a_cp(_wstr, CP_ACP)

#ifdef _UNICODE
typedef wstring string_t;
#	define w2t(_wstr) wstring(_wstr)
#	define w2t_cp(_wstr, _cp) wstring(_wstr)
#	define a2t(_str) a2w(_str)
#	define a2t_cp(_str, _cp) a2w_cp(_str, _cp)
#	define t2w(_tstr) wstring(_tstr)
#	define t2w_cp(_tstr, _cp) wstring(_tstr)
#	define t2a(_tstr) w2a(_tstr)
#	define t2a_cp(_tstr, _cp) w2a_cp(_tstr, _cp)
#else
typedef string string_t;
#	define w2t(_wstr) w2a(_wstr)
#	define w2t_cp(_wstr, _cp) w2a_cp(_wstr, _cp)
#	define a2t(_str) string(_str)
#	define a2t_cp(_str, _cp) string(_str)
#	define t2w(_tstr) a2w(_tstr)
#	define t2w_cp(_tstr, _cp) a2w_cp(_tstr, _cp)
#	define t2a(_tstr) string(_tstr)
#	define t2a_cp(_tstr, _cp) string(_tstr)
#endif

static size_t st_sprintf(string_t* pText, LPCTSTR pszFormat, ...)
{
	assert(pText);
	assert(pszFormat);

	va_list argList;
	va_start( argList, pszFormat );

	int iTextBufferLength;

#ifdef _vsctprintf
	iTextBufferLength = _vsctprintf( pszFormat, argList) + 1;
#else
	iTextBufferLength = MAX_PATH;
#endif

	int nWrit;
	TCHAR* sTemp;

	while(TRUE)
	{
		sTemp = new TCHAR[iTextBufferLength];
		assert(sTemp);
		
		nWrit = _vsntprintf(sTemp, iTextBufferLength, pszFormat, argList);
		if (nWrit < 0)
		{
			iTextBufferLength += MAX_PATH;
			delete []sTemp;
		}
		else
		{
			break;
		}
	}

	if (nWrit > 0)
		*pText = sTemp;
	else
		*pText = _T("");

	delete []sTemp;
	va_end( argList );

	return pText->length();
};

static size_t st_get_byte_length(LPCTSTR sText)
{
	assert(sText);
	return _tcslen(sText)*sizeof(TCHAR);
};
