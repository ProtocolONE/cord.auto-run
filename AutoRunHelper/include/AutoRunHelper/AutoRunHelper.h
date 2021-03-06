#pragma once

#include <AutoRunHelper/autorunhelper_global.h>

#include <QtCore/QString>

namespace P1 {
  namespace AutoRunHelper {

    /// <summary>Класс реализует методы добавления приложения в автозапуск и удаление его. </summary>
    /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
    class AUTORUNHELPER_EXPORT AutoRunHelper
    {
    public:
      AutoRunHelper();
      ~AutoRunHelper();

      /*!
        \fn void AutoRunHelper::setTaskName(const QString& name)
      
        \brief Задает автора задачи в автозапуске.
      */
      void setTaskAuthor(const QString& taskAuthor);

      /*!
        \fn void AutoRunHelper::setTaskName(const QString& name);
      
        \brief Задает имя задачи автозапуска.
      
        \param name The name.
      */
      void setTaskName(const QString& name);

      /// <summary>Задает описание задачи. Использется только для планировщика задач.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="description">The description.</param>
      void setTaskDescription(const QString& description);


      /// <summary>Задает полный путь до программы.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="exePath">Full pathname of the executable file.</param>
      void setPathToExe(const QString& exePath);


      /// <summary>Задает параметры коммандной строки.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <param name="arguments">The arguments.</param>
      void setCommandLineArguments(const QString& arguments);


      /// <summary>Убрать задачу из автозапуска. Задача удаляется по имени заданном с помощью setTaskName.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      bool removeFromAutoRun();


      /// <summary>Добавить задачу в автозапуск.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      bool addToAutoRun();



      /*!
        \fn bool AutoRunHelper::removeFromAutoRun(const QString& name, const QString& exeName);
        \brief Удаляет задачу с указанным именем, если имя испольняемого файла равно exeName 
        \author Ilya.Tkachenko
        \date 20.08.2012
        \param name    The name.
        \param exeName Name of the executable.
        \return true if it succeeds, false if it fails.
        ### summary Убрать задачу из автозапуска. Задача удаляется по имени заданном с помощью
          setTaskName.
      */
      bool removeFromAutoRun(const QString& name, const QString& exeName);

    private:
      bool isUsingTaskScheduler();

      bool removeTaskFromRegistry();
      bool addTaskToRegistry();

      bool internalRemoveTaskFromScheduler();
      bool internalAddTaskToScheduler();

      bool removeTaskFromScheduler();
      bool addTaskToScheduler();


      bool removeTaskFromRegistryByNameAndExeName(const QString& name, const QString& exeName);
      bool removeTaskFromSchedulerByNameAndExeName(const QString& name, const QString& exeName);
      bool internalRemoveTaskFromSchedulerByNameAndExeName(const QString& name, const QString& exeName);

      QString xmlDefinition();

      QString _taskAuthor;
      QString _taskName;
      QString _description;
      QString _taskExe;
      QString _arguments;
    };

  }
}
