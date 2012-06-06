/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/
#include "UACHelper.h"
#include "gtest/gtest.h"
#include <QtCore/QCoreApplication>

#include <QtCore/QDebug>
//#include "vld.h"
//#pragma comment(lib, "vld.lib")

#ifdef _DEBUG
#pragma comment(lib, "AutoRunHelperX86d.lib")
#else
#pragma comment(lib, "AutoRunHelperX86.lib")
#endif 

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    qDebug() << "started";
    qDebug() << "UAC: " << GGS::AutoRunHelper::UACHelper::isUacEnabled();
    qDebug() << "Admin Group: " << GGS::AutoRunHelper::UACHelper::isUserAdminByRole();
    qDebug() << "Admin: " << GGS::AutoRunHelper::UACHelper::isUserAdmin();
    qDebug() << "Eleveated Admin: " << GGS::AutoRunHelper::UACHelper::isUserElevatedAdmin();

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
