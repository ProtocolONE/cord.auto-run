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

#include "Programm.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#ifdef _DEBUG
#pragma comment(lib, "AutoRunHelperX86d.lib")
#else
#pragma comment(lib, "AutoRunHelperX86.lib")
#endif 

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "----------------------------";
    qDebug() << QCoreApplication::arguments();
    qDebug() << "----------------------------";
    //GGS::AutoRunHelper::UACHelper helper;
    Programm prog;
    QTimer::singleShot(0, &prog, SLOT(start()));
    return a.exec();
}
