/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include <AutoRunHelper/UACHelper>
#include <AutoRunHelper/AutoRunHelper>

#include "Programm.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#ifdef _DEBUG
#pragma comment(lib, "AutoRunHelperX86d.lib")
#else
#pragma comment(lib, "AutoRunHelperX86.lib")
#endif 

#include <BugTrap\BugTrap.h>

void initBugTrap()
{
  BT_SetAppName(_T("AutoRunHelperSample"));
  BT_SetAppVersion(_T("1.0.0.0"));
  BT_SetSupportEMail(_T("support@gamenet.ru"));
  BT_SetSupportURL(_T("https://support.gamenet.ru"));
  BT_SetFlags(BTF_DETAILEDMODE | BTF_ATTACHREPORT | BTF_SCREENCAPTURE);
  BT_SetSupportServer(_T("fs1.gamenet.ru"), 9999);
  BT_InstallSehFilter();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    initBugTrap();
    qDebug() << "----------------------------";
    qDebug() << QCoreApplication::arguments();
    qDebug() << "----------------------------";
    //GGS::AutoRunHelper::UACHelper helper;

    GGS::AutoRunHelper::AutoRunHelper helper;
    qDebug() << "qGNA removed? - " << helper.removeFromAutoRun("qGNA", "qCS.exe");

    return 0;
    Programm prog;
    QTimer::singleShot(0, &prog, SLOT(start()));
    return a.exec();
}
