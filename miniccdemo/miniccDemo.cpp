// miniccDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "miniccTest.h"
#include "DayKlineTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	///普通基础对比压缩测试.
	CMiniccTest::DoTest();

	///k线数据压缩测试.
	CDayKlineTest::DoTest();

	getchar();

	return 0;
}

