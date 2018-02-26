#include "stdafx.h"
#include "CsvFile.h"

TEST(CsvFile,abc)
{
    {
        CsvFile oCsvFile;
        oCsvFile.SetInt(1,1,1);
        oCsvFile.SetInt(1,2,3);
        oCsvFile.SetDouble(2,2,3.3);
        oCsvFile.SetString(3,3,"3.ff3");
        oCsvFile.SetString(4, 4, ",,,\n,,");
        oCsvFile.Save("hello.csv");
    }

    {
        CsvFile oCsvFile;
        oCsvFile.Load("hello.csv");
        EXPECT_EQ(1,oCsvFile.GetInt(1,1));
        EXPECT_EQ(3,oCsvFile.GetInt(1,2));  
        EXPECT_DOUBLE_EQ(3.3,oCsvFile.GetDouble(2,2));
        EXPECT_STREQ("3.ff3", oCsvFile.GetString(3,3));
        EXPECT_STREQ(",,,\n,,", oCsvFile.GetString(4, 4));
    }

    return;
}

