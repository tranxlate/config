/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.14
// 명칭 : ConfigManager
// 기능 : 설정(Config)파일을 관리할 수 있다.
//////////////////////////////////////////////////////////////////////////////*/

#ifndef _CONFIGMANAGER_H_
#define _CONFIGMANAGER_H_

#define MAX_KEY_NUM 	512
#define MAX_BUF 	4096
//#define CONF_PERMISSION "r"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ConfigManager
{
private:
	char* mid(const char *szStr, int iStart, int iEnd);
	int indexof(const char *szStr, int iChr);
	int key_alloc();
	int key_free();
	int parse_key(char *szLine);
private:
	struct KEY_INFO
	{
		char *szKey;
		char *szValue;
		int iLen;
	};
	typedef struct KEY_INFO KEYINFO;
	KEYINFO **Keys;
private:
	char *m_szBuf;
	FILE *m_fpConf;
	char *m_szFileName;
	int m_iMaxKey;
public:
	ConfigManager();
	~ConfigManager();
	int SetConf(const char *szFileName);
	int CloseConf();
	int ChkConf(const char *szFileName);
	int InsertKey(const char *szKey, const char *szValue);
	int InsertKey(const char *szKey, long nValue);
	int InsertKey(const char *szKey, int iValue);
	int InsertLine();
	int InsertTitle(const char *szTitle);
	int MakeConf();
	int MakeConf(const char *szFileName);
	int WriteConf(const char *szFileName);
	int WriteConf();
	int ReadConf(const char *szFileName);
	int ReadConf();
	char* SearchKey(const char *szKey);
	int SearchNumKey(const char *szKey);
	long SearchLongKey(const char *szKey);
};

#endif