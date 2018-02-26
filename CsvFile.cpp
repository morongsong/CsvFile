/**
   *@file         CsvFile.cpp
   *@brief        CsvFile.cpp
   *@author       jimsmorong
   *@date         2017/07/06 16:13:08
*/

#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable: 4786)

#include <list>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include <Windows.h>
#include <map>
#include "CsvFile.h"



#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#define strcpy_s strcpy
#define sprintf_s sprintf
#endif

typedef std::string string;

#define CSVFILE_MAX_ROW 65535
#define CSVFILE_MAX_COL 65535

static void StrReplaceAdt(string& src,const char* pszOld, const char* pszNew)
{
    int old_str_len = strlen(pszOld);
    int new_str_len = strlen(pszNew);
    int pos = 0;
    while ((pos = int(src.find(pszOld, pos))) != src.npos)
    {
        src.replace(pos, old_str_len, pszNew);
        pos += new_str_len;
    }
    return;
}

static string DecodeSpec(LPCSTR pszText)
{
    string strResult = pszText;
    strResult = strResult.substr(1, strResult.length() - 2);
    StrReplaceAdt(strResult, "\"\"", "\"");
    return strResult;
}

static string EncodeSpec(LPCSTR pszText)
{
    string strResult = pszText;
    if (strResult.empty())
    {
        return "";
    }
    StrReplaceAdt(strResult, "\"", "\"\"");
    strResult.insert(0, "\"");
    strResult += "\"";
    return strResult;
}

class CsvFile::CMapIDStr : public std::map<unsigned int, std::string>
{

};

CsvFile::CsvFile()
{
    m_nMaxRow = 0;
    m_nMaxCol = 0;
	m_pData = new CMapIDStr;
	m_szFile[0] = 0;
}

CsvFile::~CsvFile()
{
	delete m_pData;
}

bool CsvFile::Load(const char* pszFile)
{
	if(!pszFile) {return false;}
    strcpy_s(m_szFile,pszFile);

#if defined(_MSC_VER) && (_MSC_VER > 1200)
    FILE* pfile = NULL;
    if (fopen_s(&pfile, pszFile, ("r")) != 0)
    {
        return false;
    }
#else
    FILE* pfile = fopen(pszFile, "r");
    if (pfile == 0)
    {
        return false;
    }
#endif

    fseek(pfile, 0, SEEK_END);
    int dwsize = ftell(pfile);
    rewind(pfile);

    char* filebuffer = (char*)malloc(dwsize*sizeof(char));
    fread(filebuffer, 1, dwsize, pfile);
	fclose(pfile);

    char* pCurBegin = filebuffer;
    char* pCur = pCurBegin;

    m_nMaxRow = 0;
    m_nMaxCol = 0;
    m_pData->clear();

    struct SBlockCell
    {
        int nRow;         ///< 行
        int nCol;         ///< 列
        int nCountDouLeft;///< 逗号个数
        bool bSqec;       ///< 是否是特别的
        char* pCurStart; 
    };
    SBlockCell oCurCell;
    oCurCell.nRow = 1;
    oCurCell.nCol = 1;
    oCurCell.nCountDouLeft = 0;
    oCurCell.pCurStart = pCur;
    oCurCell.bSqec = false;
    for (int i = 0; i < dwsize; i++)
    {
        char chCur = *pCur;

        if ((chCur == ',') && (oCurCell.nCountDouLeft == 0))
        {
            *pCur = 0;
            unsigned int nId = ((oCurCell.nRow << 16) | oCurCell.nCol);
            (*m_pData)[nId] = (oCurCell.bSqec ? DecodeSpec(oCurCell.pCurStart) : oCurCell.pCurStart);
            if (m_nMaxRow < oCurCell.nRow) { m_nMaxRow = oCurCell.nRow; }
            if (m_nMaxCol < oCurCell.nCol) { m_nMaxCol = oCurCell.nCol; }
            oCurCell.bSqec = false;
            oCurCell.pCurStart = pCur + 1;
            oCurCell.nCol++;
        }
        else if((chCur == '\n') && (oCurCell.nCountDouLeft == 0))
        {
            *pCur = 0;
            unsigned int nId = ((oCurCell.nRow << 16) | oCurCell.nCol);
            (*m_pData)[nId] = (oCurCell.bSqec ? DecodeSpec(oCurCell.pCurStart) : oCurCell.pCurStart);
            if (m_nMaxRow < oCurCell.nRow) { m_nMaxRow = oCurCell.nRow; }
            if (m_nMaxCol < oCurCell.nCol) { m_nMaxCol = oCurCell.nCol; }
            oCurCell.bSqec = false;
            oCurCell.pCurStart = pCur + 1;
            oCurCell.nCol = 1;
            oCurCell.nRow++;
        }
        else if ((chCur == '"') && (oCurCell.nCountDouLeft == 0))
        {
            oCurCell.bSqec = true;
            oCurCell.nCountDouLeft++;
        }
        else if ((chCur == '"') && (oCurCell.nCountDouLeft > 0))
        {
            oCurCell.nCountDouLeft--;
        }

        pCur++;
    }
    free(filebuffer);
    return true;
}

bool CsvFile::Save(const char* pszFile/* = nullptr*/)
{
    if (pszFile != NULL)
    {
        strcpy_s(m_szFile,pszFile);
    }
    
#if defined(_MSC_VER) && (_MSC_VER > 1200)
    FILE* pfile = nullptr;
    if (fopen_s(&pfile, m_szFile, "w") != 0)
    {
        return false;
    }
#else
	FILE* pfile = fopen(m_szFile, "w");
	if (pfile == 0)
    {
        return false;
    }
#endif 

    for (int iRow = 1; iRow <= m_nMaxRow; iRow++)
    {
        for (int iCol = 1; iCol <= m_nMaxCol; iCol++)
        {
            std::string key = GetString(iRow, iCol, "");
            if (key.size() == 0)
            {
                key = (iCol!= m_nMaxCol)?(','): ('\n');
            }
            else
            {
                if ((key.find('\n') != key.npos)
                    || (key.find(',') != key.npos)
                    || (key.find('\"') != key.npos)
                    )
                {
                    key = EncodeSpec(key.c_str());
                    key += (iCol != m_nMaxCol) ? (',') : ('\n');
                }
                else
                {
                    key += (iCol != m_nMaxCol) ? (',') : ('\n');
                }

            }
            fwrite(key.c_str(), 1, key.size(), pfile);
        }
    }
    fclose(pfile);

    return true;
}

bool CsvFile::SetString(IROW nRow, ICOL nCol, const char* pszVal)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    if (m_nMaxRow < nRow) { m_nMaxRow = nRow; }
    if (m_nMaxCol < nCol) { m_nMaxCol = nCol; }
    unsigned int nId = ((nRow << 16) | nCol);
    (*m_pData)[nId] = pszVal;
    return true;
}

bool CsvFile::SetInt(IROW nRow, ICOL nCol, int nVal)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    if (m_nMaxRow < nRow) { m_nMaxRow = nRow; }
    if (m_nMaxCol < nCol) { m_nMaxCol = nCol; }
    char szBuf[64];
    sprintf_s(szBuf,"%d", nVal);
    unsigned int nId = ((nRow << 16) | nCol);
    (*m_pData)[nId] = szBuf;
    return true;
}

bool CsvFile::SetDouble(IROW nRow, ICOL nCol, double dVal)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    if (m_nMaxRow < nRow) { m_nMaxRow = nRow; }
    if (m_nMaxCol < nCol) { m_nMaxCol = nCol; }
    char szBuf[64];
	sprintf_s(szBuf, "%f", dVal);
    unsigned int nId = ((nRow << 16) | nCol);
    (*m_pData)[nId] = szBuf;
    return true;
}

bool CsvFile::HaveData(IROW nRow, ICOL nCol)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    unsigned int nId = ((nRow << 16) | nCol);
    return m_pData->find(nId) != m_pData->end() ? true : false;
}

const char* CsvFile::GetString(IROW nRow, ICOL nCol, const char* pszDefVal/* = ""*/)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    unsigned int nId = ((nRow << 16) | nCol);
    CMapIDStr::iterator iter = m_pData->find(nId);
    if (iter != m_pData->end())
    {
        std::string& key = iter->second;
        return key.c_str();
    }
    return pszDefVal;
}

int CsvFile::GetInt(IROW nRow, ICOL nCol, int nDef/* = 0*/)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    unsigned int nId = ((nRow << 16) | nCol);
    CMapIDStr::iterator iter = m_pData->find(nId);
    if (iter != m_pData->end())
    {
        std::string& key = iter->second;
        return atoi(key.c_str());
    }
    return nDef;
}

double CsvFile::GetDouble(IROW nRow, ICOL nCol, double dDev/* = 0*/)
{
    assert((nRow > 0) && (nRow < CSVFILE_MAX_ROW));
    assert((nCol > 0) && (nCol < CSVFILE_MAX_COL));
    unsigned int nId = ((nRow << 16) | nCol);
    CMapIDStr::iterator iter = m_pData->find(nId);
    if (iter != m_pData->end())
    {
        std::string& key = iter->second;
        return atof(key.c_str());
    }
    return dDev;
}

#pragma warning(pop)