/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
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
#include <QtCore/QDebug>

#define CHECK_HRESULT(f) if (f != S_OK) { qWarning() << __LINE__ << __FILE__ << "HRESULT: " << f; return false; }

namespace GGS {
  namespace AutoRunHelper {

    /// <summary>Класс реализует методы добавления приложения в автозапуск и удаление его. </summary>
    /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
    class AUTORUNHELPER_EXPORT AutoRunHelper
    {
    public:
      AutoRunHelper(void);
      ~AutoRunHelper(void);


      /// <summary>Задает имя задачи автозапуска.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="name">The name.</param>
      void setTaskName(const QString& name) { this->_taskName = name; }


      /// <summary>Задает описание задачи. Использется только для планировщика задач.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="description">The description.</param>
      void setTaskDescription(const QString& description) { this->_description = description; }


      /// <summary>Задает полный путь до программы.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="exePath">Full pathname of the executable file.</param>
      void setPathToExe(const QString& exePath) { this->_taskExe = exePath; }


      /// <summary>Задает параметры коммандной строки.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="arguments">The arguments.</param>
      void setCommandLineArguments(const QString& arguments) { this->_arguments = arguments; }


      /// <summary>Убрать задачу из автозапуска. Задача удаляется по имени заданном с помощью setTaskName.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      bool removeFromAutoRun();


      /// <summary>Добавить задачу в автозапуск.</summary>
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