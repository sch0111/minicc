// miniccDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>

#include "miniccTest.h"
#include "DayKlineTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	///��ͨ�����Ա�ѹ������.
	CMiniccTest::DoTest();

	///k������ѹ������.
	CDayKlineTest::DoTest();

	getchar();

	return 0;
}

