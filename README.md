# MCsvFile

Read And Write Csv File

This Can Procecss if Value of String Have Char : , " \n 

# How To Use
## Create And Save
    {
        CsvFile oCsvFile;
        oCsvFile.SetInt(1,1,1);
        oCsvFile.SetInt(1,2,3);
        oCsvFile.SetDouble(2,2,3.3);
        oCsvFile.SetString(3,3,"3.ff3");
        oCsvFile.SetString(4, 4, ",,,\n,,");
        oCsvFile.SetString(5, 6, ",");
        oCsvFile.SetString(5, 7, ",,");
        oCsvFile.SetString(5, 8, ",,,");
        oCsvFile.Save("hello.csv");
    }

## Load
    {
        CsvFile oCsvFile;
        oCsvFile.Load("hello.csv");
        EXPECT_EQ(1,oCsvFile.GetInt(1,1));
        EXPECT_EQ(3,oCsvFile.GetInt(1,2));  
        EXPECT_DOUBLE_EQ(3.3,oCsvFile.GetDouble(2,2));
        EXPECT_STREQ("3.ff3", oCsvFile.GetString(3,3));
        EXPECT_STREQ(",,,\n,,", oCsvFile.GetString(4, 4));
        EXPECT_STREQ(",", oCsvFile.GetString(5, 6));
        EXPECT_STREQ(",,", oCsvFile.GetString(5, 7));
        EXPECT_STREQ(",,,", oCsvFile.GetString(5, 8));
    }