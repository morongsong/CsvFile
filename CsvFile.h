/** 
   *@file         CsvFile.h
   *@brief        CsvFile.h
   *@author       jimsmorong
   *@date         2017/07/06 16:13:02
 */
#ifndef __CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
#define __CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
#pragma once


/// CsvFile
class CsvFile
{
public:
    class CMapIDStr;
    typedef int IROW;
    typedef int ICOL;
public:
    CsvFile();
    ~CsvFile();
public:
    bool Load(const char* pszFile);
    bool Save(const char* pszFile = NULL);
public:
    bool SetString(IROW nRow, ICOL nCol, const char* pszVal);
    bool SetInt(IROW nRow, ICOL nCol, int nVal);
    bool SetDouble(IROW nRow, ICOL nCol, double dVal);
public:
    const char* GetString(IROW nRow, ICOL nCol, const char* pszDefVal="");
    int GetInt(IROW nRow, ICOL nCol,int nDef = 0);
    double GetDouble(IROW nRow, ICOL nCol,double dDev = 0);
public:
    bool HaveData(IROW nRow, ICOL nCol);
    int GetMaxRow() { return m_nMaxRow; }
    int GetMaxCol() { return m_nMaxCol; }
private:
    char m_szFile[MAX_PATH];
    CMapIDStr* m_pData;
    int m_nMaxRow;
    int m_nMaxCol;
};

#endif //__CsvFile_H_08C5C33E_39F3_4ED8_B3F4_0FFD010D110A__
