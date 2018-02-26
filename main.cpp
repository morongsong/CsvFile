// hello.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>




int _tmain(int argc, TCHAR* argv[])
{
    //printf("hello world\n");
	{
		BOOL v;
		SystemParametersInfo(SPI_GETBEEP, 0, &v, 0);
	}

    ::testing::InitGoogleTest(&argc, argv);

    ::testing::GTEST_FLAG(filter) = "*.*";


    int final_result = RUN_ALL_TESTS();
    Sleep(100);

    getchar();
    return 0;
}

