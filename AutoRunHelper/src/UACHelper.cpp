#include <AutoRunHelper/UACHelper.h>

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

namespace P1 {
  namespace AutoRunHelper {
 
    UACHelper::UACHelper()
    {
    }

    UACHelper::~UACHelper()
    {
    }

    bool UACHelper::isUserAdminByRole()
      /*++ 
      Routine Description: This routine returns TRUE if the caller's
      process is a member of the Administrators local group. Caller is NOT
      expected to be impersonating anyone and is expected to be able to
      open its own process and process token. 
      Arguments: None. 
      Return Value: 
      TRUE - Caller has Administrators local group. 
      FALSE - Caller does not have Administrators local group. --
      */ 
    {
      BOOL b;
      SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
      PSID AdministratorsGroup; 
      b = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup); 
      if (b) 
      {
        if (!CheckTokenMembership( NULL, AdministratorsGroup, &b)) 
        {
          b = FALSE;
        }
        FreeSid(AdministratorsGroup); 
      }

      return(b);
    }

    bool UACHelper::isUacEnabled()
    {
      QSettings settings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        QSettings::NativeFormat);

      QVariant value = settings.value("EnableLUA");
      return value.isValid() && (value.toInt() == 1);
    }

    bool UACHelper::isUserAdmin()
    {
      if (!UACHelper::isUacEnabled())
        return UACHelper::isUserAdminByRole();

      TOKEN_ELEVATION_TYPE te;
      if (UACHelper::elevationType(&te) != S_OK)
        return false;

      return te != TokenElevationTypeDefault;
    }

    HRESULT UACHelper::elevationType(__out TOKEN_ELEVATION_TYPE *ptet)
    {
      HRESULT hResult = E_FAIL; // assume an error occured
      HANDLE hToken	= NULL;

      if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken ))
        return hResult;

      DWORD dwReturnLength = 0;

      if (::GetTokenInformation(
        hToken,
        TokenElevationType,
        ptet,
        sizeof( *ptet ),
        &dwReturnLength ))
      {
        hResult = S_OK;
      }

      ::CloseHandle( hToken );
      return hResult;
    }

    bool UACHelper::restartToElevateRights()
    {
      QString dir = QCoreApplication::applicationDirPath();
      QString exe = QCoreApplication::applicationFilePath();
      QStringList args = QCoreApplication::arguments();
      args.removeFirst();

      QString commandLineArgs("");
      if (args.size() > 0) {
        commandLineArgs = args.join("\" \"");
        commandLineArgs.prepend(L'"');
        commandLineArgs.append(L'"');
      }

      QString open("runas");

      SHELLEXECUTEINFOW shex;
      ZeroMemory(&shex, sizeof(shex));
      
      shex.cbSize			  = sizeof( SHELLEXECUTEINFO ); 
      shex.fMask			  = 0; 
      shex.hwnd			    = 0;
      shex.lpVerb			  = reinterpret_cast<const WCHAR*>(open.utf16()); 
      shex.lpFile			  = reinterpret_cast<const WCHAR*>(exe.utf16()); 
      shex.lpParameters	= reinterpret_cast<const WCHAR*>(commandLineArgs.utf16()); 
      shex.lpDirectory	= reinterpret_cast<const WCHAR*>(dir.utf16()); 
      shex.nShow			  = SW_NORMAL; 

      if (::ShellExecuteExW(&shex)) {
        QCoreApplication::exit();
        return true;
      }

      return false;
    }

    bool UACHelper::isUserElevatedAdmin( void )
    {
      if (!UACHelper::isUacEnabled())
        return UACHelper::isUserAdminByRole();

      TOKEN_ELEVATION_TYPE te;
      if (UACHelper::elevationType(&te) != S_OK)
        return false;

      return te == TokenElevationTypeFull;
    }

  }
}