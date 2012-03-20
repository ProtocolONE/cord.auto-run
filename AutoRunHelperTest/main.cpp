/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include "gtest/gtest.h"
#include <QtCore/QCoreApplication>

//#include "vld.h"
//#pragma comment(lib, "vld.lib")

#ifdef NDEBUG
#pragma comment(lib, "lib/gtest.lib")
#else
#pragma comment(lib, "lib/gtestd.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "AutoRunHelperX86d.lib")
#else
#pragma comment(lib, "AutoRunHelperX86.lib")
#endif 

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
