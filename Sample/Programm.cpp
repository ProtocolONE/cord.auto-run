/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include "Programm.h"


Programm::Programm(void)
{
}


Programm::~Programm(void)
{
}

void Programm::start()
{
  qDebug() << "started";
  qDebug() << "UAC: " << GGS::AutoRunHelper::UACHelper::isUacEnabled();
  qDebug() << "Admin Group: " << GGS::AutoRunHelper::UACHelper::isUserAdminByRole();
  qDebug() << "Admin: " << GGS::AutoRunHelper::UACHelper::isUserAdmin();

  if(!GGS::AutoRunHelper::UACHelper::isUserAdminByRole()) {
    if (!GGS::AutoRunHelper::UACHelper::restartToElevateRights()) {
      qDebug() << "Restart failed. May be user didn't accept UAC.";
    }
  }

  QStringList args = QCoreApplication::arguments();
  GGS::AutoRunHelper::AutoRunHelper autorunHelper;
  autorunHelper.setTaskName("qGNA");
  if (args.contains(QString("/remove"))) {
    qDebug() << "remove from autorun result: " << autorunHelper.removeFromAutoRun();
  } else {
    autorunHelper.setPathToExe(QCoreApplication::applicationFilePath());
    autorunHelper.setCommandLineArguments(QString::fromLocal8Bit("/minimized /update /мегапараметр"));
    autorunHelper.setTaskDescription(QString::fromLocal8Bit("Тестовая задача для проверки автозапуска. Test programm for testing autorun manager."));
    qDebug() << "add to autorun result: " << autorunHelper.addToAutoRun();
  }

}
