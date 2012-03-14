#include "gtest/gtest.h"

#include "AutoRunHelper.h"

#include <taskschd.h>
#include <atlbase.h>

#include <QtCore/QString>
#include <QtCore/QSysInfo>
#include <QtCore/QSettings>
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

void CheckForCreation(const QString &taskName, const QString& exe, const QString& args, const QString& description ) 
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
  
  QString qxml = QString::fromUtf16((reinterpret_cast<const WCHAR*>(xmlString.m_str)));
  ASSERT_TRUE( doc.setContent(qxml) );

  ASSERT_EQ(0, doc.documentElement()
    .firstChildElement("RegistrationInfo")
    .firstChildElement("Description").text().compare(description));

  QDomElement exec = doc.documentElement()
                    .firstChildElement("Actions")
                    .firstChildElement("Exec");
  ASSERT_EQ(0, exec.firstChildElement("Command").text().compare(exe));
  ASSERT_EQ(0, exec.firstChildElement("Arguments").text().compare(args));

  QString cleanPath = QDir::cleanPath(exe);
  int lastIndex = cleanPath.lastIndexOf('/');
  QString targetDirectory("");
  if(lastIndex != -1) {
    targetDirectory = cleanPath.mid(0, lastIndex + 1);
  }

  ASSERT_EQ(0, exec.firstChildElement("WorkingDirectory").text().compare(targetDirectory));
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
  QString taskName("TestAutoRunHelperTest");
  QString exe = QString::fromLocal8Bit("C:\\какой-то\\Тестовый файл.exe");
  QString args = QString::fromLocal8Bit("/параметр \"тестовый параметр\" /test");
  QString description = QString::fromLocal8Bit("Какое-то описани. Some description.");
  CoInitialize(NULL);
  RemoveOldTask(taskName);
  CoUninitialize();

  GGS::AutoRunHelper::AutoRunHelper helper;

  helper.setTaskName(taskName);
  helper.setPathToExe(exe);
  helper.setCommandLineArguments(args);
  helper.setTaskDescription(description);

  helper.addToAutoRun();

  CoInitialize(NULL);
  CheckForCreation(taskName, exe, args, description);
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

  GGS::AutoRunHelper::AutoRunHelper helper;

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
  }
  
}