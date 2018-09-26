/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (�) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#ifndef _GGS_AUTORUNHELPER_AUTORUNHELPER_H_
#define _GGS_AUTORUNHELPER_AUTORUNHELPER_H_

#include "autorunhelper_global.h"
#include "UACHelper.h"

#include <taskschd.h>
#include <atlbase.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QSysInfo>

#define CHECK_HRESULT(f) if (f != S_OK) return false

namespace GGS {
  namespace AutoRunHelper {

    /// <summary>����� ��������� ������ ���������� ���������� � ���������� � �������� ���. </summary>
    /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
    class AUTORUNHELPER_EXPORT AutoRunHelper
    {
    public:
      AutoRunHelper(void);
      ~AutoRunHelper(void);


      /// <summary>������ ��� ������ �����������.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="name">The name.</param>
      void setTaskName(const QString& name) { this->_taskName = name; }


      /// <summary>������ �������� ������. ����������� ������ ��� ������������ �����.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="description">The description.</param>
      void setTaskDescription(const QString& description) { this->_description = description; }


      /// <summary>������ ������ ���� �� ���������.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="exePath">Full pathname of the executable file.</param>
      void setPathToExe(const QString& exePath) { this->_taskExe = exePath; }


      /// <summary>������ ��������� ���������� ������.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="arguments">The arguments.</param>
      void setCommandLineArguments(const QString& arguments) { this->_arguments = arguments; }


      /// <summary>������ ������ �� �����������. ������ ��������� �� ����� �������� � ������� setTaskName.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      bool removeFromAutoRun();


      /// <summary>�������� ������ � ����������.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      bool addToAutoRun();

    private:
      bool isUsingTaskScheduler();

      bool removeTaskFromRegistry();
      bool addTaskToRegistry();

      bool internalRemoveTaskFromScheduler();
      bool internalAddTaskToScheduler();

      bool removeTaskFromScheduler();
      bool addTaskToScheduler();

      QString xmlDefinition();

      QString _taskName;
      QString _description;
      QString _taskExe;
      QString _arguments;
    };

  }
}

#endif // _GGS_AUTORUNHELPER_AUTORUNHELPER_H_