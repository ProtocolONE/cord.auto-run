#include <AutoRunHelper/UACHelper.h>

#include <gtest/gtest.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#ifdef _DEBUG
#pragma comment(lib, "AutoRunHelperX86d.lib")
#else
#pragma comment(lib, "AutoRunHelperX86.lib")
#endif 

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    qDebug() << "started";
    qDebug() << "UAC: " << P1::AutoRunHelper::UACHelper::isUacEnabled();
    qDebug() << "Admin Group: " << P1::AutoRunHelper::UACHelper::isUserAdminByRole();
    qDebug() << "Admin: " << P1::AutoRunHelper::UACHelper::isUserAdmin();
    qDebug() << "Eleveated Admin: " << P1::AutoRunHelper::UACHelper::isUserElevatedAdmin();

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
