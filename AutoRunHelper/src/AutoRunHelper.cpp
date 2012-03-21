/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include "AutoRunHelper.h"

namespace GGS {
  namespace AutoRunHelper {
    AutoRunHelper::AutoRunHelper(void)
    {
    }

    AutoRunHelper::~AutoRunHelper(void)
    {
    }

    bool AutoRunHelper::removeFromAutoRun()
    {
      if (!UACHelper::isUserElevatedAdmin())
        return false;

      if (this->isUsingTaskScheduler()) 
        return this->removeTaskFromScheduler();

      return this->removeTaskFromRegistry();
    }

    bool AutoRunHelper::addToAutoRun()
    {
      if (!UACHelper::isUserElevatedAdmin())
        return false;

      if (this->isUsingTaskScheduler()) 
        return this->addTaskToScheduler();

      return this->addTaskToRegistry();
    }

    bool AutoRunHelper::removeTaskFromRegistry()
    {
      QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat);
      settings.remove(this->_taskName);
      return true;
    }
    
    bool AutoRunHelper::addTaskToRegistry()
    {
      QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat);
      QString params = QString("\"%1\" %2").arg(this->_taskExe, this->_arguments);
      settings.setValue(this->_taskName, params);
      return true;
    }
    
    bool AutoRunHelper::addTaskToScheduler()
    {
      if (CoInitialize(NULL) == S_FALSE)
      {
        qWarning()  << __LINE__ << __FILE__ << "cant CoInitialize trying without";
        return this->internalAddTaskToScheduler();
      }
      
      bool result = this->internalAddTaskToScheduler();
      CoUninitialize();
      return result;
    }

    bool AutoRunHelper::removeTaskFromScheduler()
    {
      if (CoInitialize(NULL) == S_FALSE)
      {
        qWarning()  << __LINE__ << __FILE__ << "cant CoInitialize trying without";
        return this->internalRemoveTaskFromScheduler();
      }

      bool result = this->internalRemoveTaskFromScheduler();
      CoUninitialize();
      return result;
    }

    bool AutoRunHelper::internalRemoveTaskFromScheduler()
    {
      CComPtr<ITaskService> service;
      CHECK_HRESULT(service.CoCreateInstance(__uuidof(TaskScheduler)));
      CHECK_HRESULT(service->Connect(
        CComVariant(),   // local computer
        CComVariant(),   // current user
        CComVariant(),   // current domain
        CComVariant()));

      CComPtr<ITaskFolder> folder;
      CHECK_HRESULT(service->GetFolder(CComBSTR(L"\\"), &folder));
      CHECK_HRESULT(folder->DeleteTask(CComBSTR( reinterpret_cast<const WCHAR*>(this->_taskName.utf16()) ), 0));
      return true;
    }

    bool AutoRunHelper::internalAddTaskToScheduler()
    {
      CComPtr<ITaskService> service;
      CHECK_HRESULT(service.CoCreateInstance(__uuidof(TaskScheduler)));
      CHECK_HRESULT(service->Connect(
        CComVariant(),   // local computer
        CComVariant(),   // current user
        CComVariant(),   // current domain
        CComVariant()));

      CComPtr<ITaskDefinition> definition;
      CHECK_HRESULT(service->NewTask(0, &definition));

      CComPtr<ITaskFolder> folder;
      CHECK_HRESULT(service->GetFolder(CComBSTR(L"\\"), &folder));

      QString xmlDefinition = this->xmlDefinition();
      CComBSTR inxml(reinterpret_cast<const WCHAR*>(xmlDefinition.utf16()));
      CHECK_HRESULT(definition->put_XmlText(inxml));

      CComPtr<IRegisteredTask> registeredTask;
      CHECK_HRESULT(folder->RegisterTaskDefinition(
        CComBSTR( reinterpret_cast<const WCHAR*>(this->_taskName.utf16()) ),
        definition,
        TASK_CREATE_OR_UPDATE,
        CComVariant(), // user name
        CComVariant(), // password
        TASK_LOGON_INTERACTIVE_TOKEN,
        CComVariant(), // sddl
        &registeredTask));
        
      return true;
    }

    QString AutoRunHelper::xmlDefinition()
    {
      QString cleanPath = QDir::cleanPath(this->_taskExe);
      int lastIndex = cleanPath.lastIndexOf('/');
      QString targetDirectory("");
      if(lastIndex != -1) {
        targetDirectory = cleanPath.mid(0, lastIndex + 1);
      }
      
      QDateTime dt = QDateTime::currentDateTimeUtc();
      dt.setTimeSpec(Qt::OffsetFromUTC);
      QString currentDateString = dt.toString(Qt::ISODate);

      QString definition = QString("<?xml version=\"1.0\" encoding=\"UTF-16\"?> " \
        "<Task version=\"1.2\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">" \
        "<RegistrationInfo>" \
        "<Author>Global Gamers Solution</Author>" \
        "<Description>%1</Description>" \
        "</RegistrationInfo>" \
        "<Triggers>" \
        "<LogonTrigger>" \
        "<StartBoundary>%5</StartBoundary>" \
        "<Enabled>true</Enabled>" \
        "<Delay>PT1M</Delay>" \
        "</LogonTrigger>" \
        "</Triggers>" \
        "<Principals>" \
        "<Principal id=\"Author\">" \
        "<GroupId>S-1-5-32-544</GroupId>" \
        "<RunLevel>HighestAvailable</RunLevel>" \
        "</Principal>" \
        "</Principals>" \
        "<Settings>" \
        "<MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>" \
        "<DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>" \
        "<StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>" \
        "<AllowHardTerminate>false</AllowHardTerminate>" \
        "<StartWhenAvailable>true</StartWhenAvailable>" \
        "<RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>" \
        "<IdleSettings>" \
        "<Duration>PT20M</Duration>" \
        "<WaitTimeout>PT10M</WaitTimeout>" \
        "<StopOnIdleEnd>false</StopOnIdleEnd>" \
        "<RestartOnIdle>false</RestartOnIdle>" \
        "</IdleSettings>" \
        "<AllowStartOnDemand>false</AllowStartOnDemand>" \
        "<Enabled>true</Enabled>" \
        "<Hidden>false</Hidden>" \
        "<RunOnlyIfIdle>false</RunOnlyIfIdle>" \
        "<WakeToRun>false</WakeToRun>" \
        "<ExecutionTimeLimit>PT0S</ExecutionTimeLimit>" \
        "<Priority>5</Priority>" \
        "<RestartOnFailure>" \
        "<Interval>PT5M</Interval>" \
        "<Count>1</Count>" \
        "</RestartOnFailure>" \
        "</Settings>" \
        "<Actions Context=\"Author\">" \
        "<Exec>" \
        "<Command>%2</Command>" \
        "<Arguments>%3</Arguments>" \
        "<WorkingDirectory>%4</WorkingDirectory>" \
        "</Exec>" \
        "</Actions>" \
        "</Task>").arg(this->_description, this->_taskExe, this->_arguments, targetDirectory, currentDateString);

      return definition;
    }

    bool AutoRunHelper::isUsingTaskScheduler()
    {
      QSysInfo::WinVersion version = QSysInfo::windowsVersion();
      return version == QSysInfo::WV_VISTA || version == QSysInfo::WV_WINDOWS7;
    }

  }
}