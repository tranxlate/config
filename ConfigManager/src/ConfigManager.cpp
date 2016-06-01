/*****************************************************************
Copyright (c) 2016 Jung-Min, Shin (tranxlate3@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*****************************************************************/


#include "../include/ConfigManager.h"

/*//////////////////////////////////////////////////////////////////////////////
//
// 				Internal Functions
//
//////////////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.21
// 명칭 : Indef Of
// 기능 : char 배열 중 한 글자가 있는 위치를 되돌려 준다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::indexof(const char *szStr, int iChr)
{
	int i=0, idx=-1;
	for(i=0;i<strlen(szStr);i++)
	{
		if(szStr[i]==iChr)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.23
// 명칭 : Key Alloc
// 기능 : 키를 입력받기 위해 메모리를 할당한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::key_alloc()
{
	if(Keys!=0) key_free();
	m_iMaxKey = 0;
	Keys = 0;
	Keys = new KEYINFO* [MAX_KEY_NUM];
	if(Keys==0) return -1;
	return 0;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.23
// 명칭 : Key Free
// 기능 : 할당된 키 메모리를 해제한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::key_free()
{
	int i;
	
	if(Keys)
	{
		for(i=0;i<m_iMaxKey;i++)
		{
			delete Keys[i]->szKey;
			delete Keys[i]->szValue;
		}
		delete [] Keys;
		Keys = 0;
	}
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.21
// 명칭 : String Middle
// 기능 : char 배열의 중간범위를 자른다.
//////////////////////////////////////////////////////////////////////////////*/

char* ConfigManager::mid(const char *szStr, int iStart, int iEnd)
{
	int i=0, idx=0, len=0;

	memset(m_szBuf, 0, MAX_BUF);
	len = strlen(szStr);
	if(len<=iStart || len<=iEnd || iEnd-iStart >= 4095)
		return (char*) 0;
	for(i=iStart;i<=iEnd;i++)
	{
		m_szBuf[idx++] = szStr[i];
	}
	return &m_szBuf[0];
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Parse Key
// 기능 : 라인으로부터 Key와 Value를 분리하고 메모리에 입력한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::parse_key(char *szLine)
{
	int iSplitIdx=0;
	char *szKey=0;
	char *szValue=0;
	int iRet=0;
	
	szLine[strlen(szLine)-1] = 0;
	iSplitIdx = indexof(szLine,'=');
//printf("Debug:%s\n",szLine);
	if(iSplitIdx < 0) return InsertKey((char*)"",szLine);
	else
	{
		szKey = new char [(iSplitIdx-1)+1];
		szValue = new char [(strlen(szLine)-iSplitIdx+1)+1];
		m_szBuf = 0;
		m_szBuf = new char [MAX_BUF];
		if(szKey==0 || szValue==0 || m_szBuf==0)
			return -1;
		memset(szKey, 0, sizeof(szKey));
		memset(szValue, 0, sizeof(szValue));
		strcpy(szKey, mid(szLine,0,iSplitIdx-1));
		strcpy(szValue, mid(szLine,iSplitIdx+1,strlen(szLine)-1));
		iRet = InsertKey(szKey, szValue);
		delete [] szKey;
		delete [] szValue;
		delete [] m_szBuf;
		m_szBuf = 0;
		return iRet;
	}
	return 0;
}

/*//////////////////////////////////////////////////////////////////////////////
//
// 				External Functions
//
//////////////////////////////////////////////////////////////////////////////*/


/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.14
// 명칭 : ConfigManager 생성자
// 기능 : 변수 초기화
//////////////////////////////////////////////////////////////////////////////*/

ConfigManager::ConfigManager()
{
	m_szFileName = 0;
	Keys = 0;
	m_szBuf = 0;
	if(key_alloc()!=0)
	{
		printf("Memory Error.\n");
		exit(1);
	}
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.14
// 명칭 : ConfigManager 소멸자
// 기능 : 힙 영역 반납
//////////////////////////////////////////////////////////////////////////////*/

ConfigManager::~ConfigManager()
{
	CloseConf();
	key_free();
	if(m_szFileName!=0)
	{
		delete [] m_szFileName;
		m_szFileName = 0;
	}
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.14
// 명칭 : Setting Config File
// 기능 : 파일 경로를 포함한 설정파일 이름을 메모리에 복사한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::SetConf(const char *szFileName)
{
	if(m_szFileName!=0) delete [] m_szFileName;
	m_szFileName = new char [strlen(szFileName)];
	if(m_szFileName==0) return -1;
	strncpy(m_szFileName, szFileName, strlen(szFileName));
	return 0;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Close Config
// 기능 : Config 파일을 종료한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::CloseConf()
{
	if(m_fpConf)
	{
		fclose(m_fpConf);
		m_fpConf = 0;
	}
	return 0;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.14
// 명칭 : Config 체크
// 기능 : Config 파일을 체크하여 결과값을 돌려준다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::ChkConf(const char *szFileName)
{
	if(SetConf(szFileName)!=0) return -1;
	m_fpConf = fopen(m_szFileName,"r");
	if(m_fpConf==0) return 0;
	CloseConf();
	return 1;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Insert Key
// 기능 : Key / Value값을 메모리에 입력한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::InsertKey(const char *szKey, const char *szValue)
{
	if(Keys==0) return -1;
	Keys[m_iMaxKey] = new KEYINFO();
	Keys[m_iMaxKey]->szKey = 0;
	Keys[m_iMaxKey]->szValue = 0;
	Keys[m_iMaxKey]->szKey = new char [strlen(szKey)+1];
	Keys[m_iMaxKey]->szValue = new char [strlen(szValue)+1];
	if(Keys[m_iMaxKey]->szKey==0 || Keys[m_iMaxKey]->szValue==0) return -1;
	memset(Keys[m_iMaxKey]->szKey, 0, strlen(szKey)+1);
	memset(Keys[m_iMaxKey]->szValue, 0, strlen(szValue)+1);
	strncpy(Keys[m_iMaxKey]->szKey, szKey, strlen(szKey));
	strncpy(Keys[m_iMaxKey]->szValue, szValue, strlen(szValue));
	m_iMaxKey++;
	return 0;
}

int ConfigManager::InsertKey(const char *szKey, long nValue)
{
	char szValue[20];
	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue,"%ld",nValue);
	return InsertKey(szKey, szValue);
}

int ConfigManager::InsertKey(const char *szKey, int iValue)
{
	char szValue[20];
	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue,"%d",iValue);
	return InsertKey(szKey, szValue);
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.02.27
// 명칭 : Insert Line
// 기능 : 공백 한 줄을 메모리에 입력한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::InsertLine()
{
	return InsertKey("", "");
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.02.27
// 명칭 : Insert Title
// 기능 : 키/값의 제목을 메모리에 입력한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::InsertTitle(const char *szTitle)
{
	char *szNewTitle = 0;
	int iRet=0;
	
	szNewTitle = new char [strlen(szTitle)+3];
	if(szNewTitle==0) return -1;
	memset(szNewTitle, 0, sizeof(szNewTitle));
	sprintf(szNewTitle, "[%s]", szTitle);
	iRet = InsertKey("", szNewTitle);
	delete [] szNewTitle;
	return iRet;
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Make Config
// 기능 : Config 파일을 생성한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::MakeConf()
{
	if(m_szFileName==0) return -1;
	m_fpConf = fopen(m_szFileName,"w");
	if(m_fpConf==0) return -1;
	CloseConf();
	return 0;
}

int ConfigManager::MakeConf(const char *szFileName)
{
	if(SetConf(szFileName)!=0) return -1;
	return MakeConf();
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Write Config
// 기능 : Config 파일에 Key=Value 값을 쓴다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::WriteConf()
{
	int i, iByte, iLen;
	char *szBuf;
	
	if(m_szFileName==0) return -1;
	else m_fpConf = fopen(m_szFileName,"w"); 
	if(m_fpConf==0) return -1;
	for(i=0;i<m_iMaxKey;i++)
	{
		szBuf = 0;
		iLen = strlen(Keys[i]->szKey)+strlen(Keys[i]->szValue)+2;
		szBuf = new char[iLen];
		if(szBuf==0) return -1;
		memset(szBuf, 0, iLen);
		if(strlen(Keys[i]->szKey)>0) 
			sprintf(szBuf, "%s=%s\n",Keys[i]->szKey, Keys[i]->szValue);
		else
			sprintf(szBuf, "%s\n", Keys[i]->szValue);
		fprintf(m_fpConf, "%s", szBuf);
		delete [] szBuf;
		//if(iByte==0) continue;
	}
	CloseConf();
	return 0;
}


int ConfigManager::WriteConf(const char *szFileName)
{
	if(SetConf(szFileName)!=0) return -1;
	return WriteConf();
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.15
// 명칭 : Read Config
// 기능 : Config 파일을 읽어들이고 메모리에 저장한다.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::ReadConf()
{
	int iByte=0;
	char *szBuf=0;
	
	if(m_szFileName==0) return -1;
	else m_fpConf = fopen(m_szFileName,"r"); 
	if(m_fpConf==0) return -1;
	szBuf = new char [MAX_BUF+1];
	if(szBuf==0) return -1;
	if(key_alloc()!=0) return -1;
	
	for(;;)
	{
		memset(szBuf, 0, MAX_BUF+1);
		if(fgets(szBuf, MAX_BUF, m_fpConf)==NULL) break;
//printf("Debug:%s\n",szBuf);
		parse_key(szBuf);
		
	}
	CloseConf();
	return 0;
}

int ConfigManager::ReadConf(const char *szFileName)
{
	if(SetConf(szFileName)!=0) return -1;
	return ReadConf();
}

/*//////////////////////////////////////////////////////////////////////////////
// 개발자 : sjm
// 날짜 : 2015.01.23
// 명칭 : Search Key
// 기능 : Key를 이용하여 Value값을 검색한다.
//////////////////////////////////////////////////////////////////////////////*/

char* ConfigManager::SearchKey(const char *szKey)
{
	int i;
	char *szValue=0;
	
	for(i=0;i<m_iMaxKey;i++)
	{
		if(strlen(szKey)!=strlen(Keys[i]->szKey)) continue;
		if(strcmp(szKey, Keys[i]->szKey)==0)
		{
			szValue = Keys[i]->szValue;
			break;
		}
	}
	
	return szValue;
}

int ConfigManager::SearchNumKey(const char *szKey)
{
	char *szValue=0;
	
	szValue = SearchKey(szKey);
	if(szValue==0) return 0;
	return atoi(szValue);
}

long ConfigManager::SearchLongKey(const char *szKey)
{
	char *szValue=0;
	
	szValue = SearchKey(szKey);
	if(szValue==0) return 0;
	return atol(szValue);
}