#include <gtest/gtest.h>

#include <AutoRunHelper/AutoRunHelper.h>

#include <taskschd.h>
#include <atlbase.h>

#include <QtCore/QString>
#include <QtCore/QSysInfo>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtXml/QDomDocument>

void RemoveOldTask( QString &taskName ) 
{
  HRESULT hr;
  CComPtr<ITaskService> service;
  hr = service.CoCreateInstance(__uuidof(TaskScheduler));
  ASSERT_EQ(S_OK, hr);
  hr = service->Connect(
    CComVariant(),   // local computer
    CComVariant(),   // current user
    CComVariant(),   // current domain
    CComVariant());
  ASSERT_EQ(S_OK, hr);
  CComPtr<ITaskFolder> folder;
  hr = service->GetFolder(CComBSTR(L"\\"), &folder);
  ASSERT_EQ(S_OK, hr);

  hr = folder->DeleteTask(CComBSTR( reinterpret_cast<const WCHAR*>(taskName.utf16()) ), 0);

  CComPtr<IRegisteredTask> task1;
  hr = folder->GetTask(CComBSTR( reinterpret_cast<const WCHAR*>(taskName.utf16()) ), &task1);
  ASSERT_NE(S_OK, hr);
}

void CheckForCreation(const QString &taskName, 
                      const QString &exe, 
                      const QString &args, 
                      const QString &description,
                      const QString &author) 
{
  HRESULT hr;
  CComPtr<ITaskService> service;
  hr = service.CoCreateInstance(__uuidof(TaskScheduler));
  ASSERT_EQ(S_OK, hr);
  hr = service->Connect(
    CComVariant(),   // local computer
    CComVariant(),   // current user
    CComVariant(),   // current domain
    CComVariant());
  ASSERT_EQ(S_OK, hr);
  CComPtr<ITaskFolder> folder;
  hr = service->GetFolder(CComBSTR(L"\\"), &folder);
  ASSERT_EQ(S_OK, hr);

  CComPtr<IRegisteredTask> task1;
  hr = folder->GetTask(CComBSTR( reinterpret_cast<const WCHAR*>(taskName.utf16()) ), &task1);
  ASSERT_EQ(S_OK, hr);

  CComPtr<ITaskDefinition> definition;
  hr = task1->get_Definition(&definition);
  ASSERT_EQ(S_OK, hr);

  CComBSTR xmlString;
  hr = definition->get_XmlText(&xmlString);
  ASSERT_EQ(S_OK, hr);

  QDomDocument doc;
  
  QString qxml = QString((QChar*)xmlString.m_str, ::SysStringLen(xmlString.m_str));
  ASSERT_TRUE( doc.setContent(qxml) );

  ASSERT_EQ(description, doc.documentElement()
    .firstChildElement("RegistrationInfo")
    .firstChildElement("Description").text());

  QDomElement exec = doc.documentElement()
                    .firstChildElement("Actions")
                    .firstChildElement("Exec");

  ASSERT_EQ(exe, exec.firstChildElement("Command").text());
  ASSERT_EQ(args, exec.firstChildElement("Arguments").text());

  QString cleanPath = QDir::cleanPath(exe);
  int lastIndex = cleanPath.lastIndexOf('/');
  QString targetDirectory("");
  if(lastIndex != -1) {
    targetDirectory = cleanPath.mid(0, lastIndex + 1);
  }

  ASSERT_EQ(targetDirectory, exec.firstChildElement("WorkingDirectory").text());

  QString authorFromXml = doc.documentElement()
    .firstChildElement("RegistrationInfo")
    .firstChildElement("Author").text();

  ASSERT_EQ(author, authorFromXml);
}

void CheckForRemove(const QString &taskName)
{
  HRESULT hr;
  CComPtr<ITaskService> service;
  hr = service.CoCreateInstance(__uuidof(TaskScheduler));
  ASSERT_EQ(S_OK, hr);
  hr = service->Connect(
    CComVariant(),   // local computer
    CComVariant(),   // current user
    CComVariant(),   // current domain
    CComVariant());
  ASSERT_EQ(S_OK, hr);
  CComPtr<ITaskFolder> folder;
  hr = service->GetFolder(CComBSTR(L"\\"), &folder);
  ASSERT_EQ(S_OK, hr);

  CComPtr<IRegisteredTask> task1;
  hr = folder->GetTask(CComBSTR( reinterpret_cast<const WCHAR*>(taskName.utf16()) ), &task1);
  ASSERT_NE(S_OK, hr);
}


void InternalTaskSchedulerTest() 
{
  QString taskAuthor("SuperDuper");
  QString taskName("TestAutoRunHelperTest");
  QString exe = QString::fromLocal8Bit("C:\\какой-то\\Тестовый файл.exe");
  QString args = QString::fromLocal8Bit("/параметр \"тестовый параметр\" /test");
  QString description = QString::fromLocal8Bit("Какое-то описани. Some description.");
  CoInitialize(NULL);
  RemoveOldTask(taskName);
  CoUninitialize();

  P1::AutoRunHelper::AutoRunHelper helper;
  helper.setTaskAuthor(taskAuthor);
  helper.setTaskName(taskName);
  helper.setPathToExe(exe);
  helper.setCommandLineArguments(args);
  helper.setTaskDescription(description);

  helper.addToAutoRun();

  CoInitialize(NULL);
  CheckForCreation(taskName, exe, args, description, taskAuthor);
  CoUninitialize();
  
  helper.removeFromAutoRun();

  CoInitialize(NULL);
  CheckForRemove(taskName);
  CoUninitialize();
}

void TaskSchedulerTest() 
{
  InternalTaskSchedulerTest();
}

void RegistryTest()
{
  QString taskName("TestAutoRunHelperTest");
  QString exe = QString::fromLocal8Bit("C:\\какой-то\\Тестовый файл.exe");
  QString args = QString::fromLocal8Bit("/параметр \"тестовый параметр\" /test");
  QString description = QString::fromLocal8Bit("Какое-то описани. Some description.");
  
  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
    QSettings::NativeFormat);
  settings.remove(taskName);

  ASSERT_FALSE(settings.contains(taskName));

  P1::AutoRunHelper::AutoRunHelper helper;

  helper.setTaskName(taskName);
  helper.setPathToExe(exe);
  helper.setCommandLineArguments(args);
  helper.setTaskDescription(description);

  helper.addToAutoRun();

  ASSERT_TRUE(settings.contains(taskName));
  QString expectionValue = QString("\"%1\" %2").arg(exe, args);
  QVariant actual = settings.value(taskName);
  ASSERT_TRUE(actual.isValid());
  QString actualPath = actual.toString();
  ASSERT_EQ(0, actualPath.compare(expectionValue));

  helper.removeFromAutoRun();
  ASSERT_FALSE(settings.contains(taskName));
}

// INFO https://jira.gamenet.ru:8443/browse/QGNA-1112
// Если строка в автозапуске заканчивается пробелом и без аргументов, то процесс не стартует.
void RegistryTestEmptyArguments()
{
  QString taskName("TestAutoRunHelperTestEmpty");
  QString exe = QString::fromLocal8Bit("C:\\какой-то\\Тестовый файл.exe");
  QString args;
  QString description = QString::fromLocal8Bit("Какое-то описани. Some description.");

  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
    QSettings::NativeFormat);
  settings.remove(taskName);

  ASSERT_FALSE(settings.contains(taskName));

  P1::AutoRunHelper::AutoRunHelper helper;

  helper.setTaskName(taskName);
  helper.setPathToExe(exe);
  helper.setCommandLineArguments(args);
  helper.setTaskDescription(description);

  helper.addToAutoRun();

  ASSERT_TRUE(settings.contains(taskName));
  QString expectionValue = QString("\"%1\"").arg(exe);
  QVariant actual = settings.value(taskName);
  ASSERT_TRUE(actual.isValid());
  QString actualPath = actual.toString();
  ASSERT_EQ(0, actualPath.compare(expectionValue));

  helper.removeFromAutoRun();
  ASSERT_FALSE(settings.contains(taskName));
}

TEST(AutoRunHelperTest, addRemove)
{
  TaskSchedulerTest();
  return;

  QSysInfo::WinVersion version = QSysInfo::windowsVersion();
  if (version == QSysInfo::WV_VISTA || version == QSysInfo::WV_WINDOWS7 ) {
    // тест на шедулер
    TaskSchedulerTest();
  } else {
    // тест на реестр
    RegistryTest();
    RegistryTestEmptyArguments();
  }
  
}