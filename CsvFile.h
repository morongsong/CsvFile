/** 
   *@file         CsvFile.h
   *@brief        CsvFile.h
   *@author       jimsmorong
   *@date         2017/07/06 16:13:02
 */
#ifndef __CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
#define __CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
#pragma once

#include <string>
#include <map>

/// CsvFile
class CsvFile
{
public:
    typedef int INUM;//123
    typedef int IABC;//ABC 
public:
    CsvFile();
    ~CsvFile();
public:
    bool Load(const char* pszFile);
    bool Save(const char* pszFile = NULL);
public:
    bool SetString(INUM nRow, IABC nCol, const char* pszVal);
    bool SetInt(INUM nRow, IABC nCol, int nVal);
    bool SetDouble(INUM nRow, IABC nCol, double dVal);
public:
    const char* GetString(INUM nRow, IABC nCol, const char* pszDefVal="");
    int GetInt(INUM nRow, IABC nCol,int nDef = 0);
    double GetDouble(INUM nRow, IABC nCol,double dDev = 0);
public:
    bool HaveData(INUM nRow, IABC nCol);
    int GetMaxRow() { return m_nMaxRow; }
    int GetMaxCol() { return m_nMaxCol; }
private:
    char m_szFile[1024];
	typedef std::map<unsigned int, std::string> CMapIDStr;
    CMapIDStr m_pData;
    int m_nMaxRow;
    int m_nMaxCol;
};

#endif //__CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
