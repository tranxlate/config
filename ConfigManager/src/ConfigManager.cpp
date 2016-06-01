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
// ������ : sjm
// ��¥ : 2015.01.21
// ��Ī : Indef Of
// ��� : char �迭 �� �� ���ڰ� �ִ� ��ġ�� �ǵ��� �ش�.
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
// ������ : sjm
// ��¥ : 2015.01.23
// ��Ī : Key Alloc
// ��� : Ű�� �Է¹ޱ� ���� �޸𸮸� �Ҵ��Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.23
// ��Ī : Key Free
// ��� : �Ҵ�� Ű �޸𸮸� �����Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.21
// ��Ī : String Middle
// ��� : char �迭�� �߰������� �ڸ���.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Parse Key
// ��� : �������κ��� Key�� Value�� �и��ϰ� �޸𸮿� �Է��Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.14
// ��Ī : ConfigManager ������
// ��� : ���� �ʱ�ȭ
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
// ������ : sjm
// ��¥ : 2015.01.14
// ��Ī : ConfigManager �Ҹ���
// ��� : �� ���� �ݳ�
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
// ������ : sjm
// ��¥ : 2015.01.14
// ��Ī : Setting Config File
// ��� : ���� ��θ� ������ �������� �̸��� �޸𸮿� �����Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Close Config
// ��� : Config ������ �����Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.14
// ��Ī : Config üũ
// ��� : Config ������ üũ�Ͽ� ������� �����ش�.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Insert Key
// ��� : Key / Value���� �޸𸮿� �Է��Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.02.27
// ��Ī : Insert Line
// ��� : ���� �� ���� �޸𸮿� �Է��Ѵ�.
//////////////////////////////////////////////////////////////////////////////*/

int ConfigManager::InsertLine()
{
	return InsertKey("", "");
}

/*//////////////////////////////////////////////////////////////////////////////
// ������ : sjm
// ��¥ : 2015.02.27
// ��Ī : Insert Title
// ��� : Ű/���� ������ �޸𸮿� �Է��Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Make Config
// ��� : Config ������ �����Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Write Config
// ��� : Config ���Ͽ� Key=Value ���� ����.
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
// ������ : sjm
// ��¥ : 2015.01.15
// ��Ī : Read Config
// ��� : Config ������ �о���̰� �޸𸮿� �����Ѵ�.
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
// ������ : sjm
// ��¥ : 2015.01.23
// ��Ī : Search Key
// ��� : Key�� �̿��Ͽ� Value���� �˻��Ѵ�.
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