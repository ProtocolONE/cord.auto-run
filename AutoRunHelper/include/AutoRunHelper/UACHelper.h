#pragma once

#include <AutoRunHelper/autorunhelper_global.h>

#include <Windows.h>
#include <Shellapi.h>

namespace P1 {
  namespace AutoRunHelper {

    /// <summary>Класс реализует методы по определению текущих прав пользвоателя и метод для поднятия прав приложения. </summary>
    /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
    class AUTORUNHELPER_EXPORT UACHelper
    {
    public:
      UACHelper(void);
      ~UACHelper(void);

      /// <summary>По ключу в реестре проверяет наличие UAC и включен ли он.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      static bool isUacEnabled(void);


      /// <summary>Если UAC (Vista/Win7) выключени или это XP, то взворащает true, есил пользователь состоит
      /// 				 в группе админов. Если UAC включен, то возвращает true, если пользователь админ с поднятыми
      /// 				 правами (TokenElevationTypeFull) или с урезанными (TokenElevationTypeLimited).</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if user admin, false if not.</returns>
      static bool isUserAdmin(void);
      

      /// <summary>Если UAC (Vista/Win7) выключени или это XP, то взворащает true, есил пользователь состоит
      /// 				 в группе админов. Если UAC включен, то возвращает true, только если админ с поднятыми
      /// 				 правами (TokenElevationTypeFull).</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if user elevated admin, false if not.</returns>
      static bool isUserElevatedAdmin(void);


      /// <summary>Проверяет состоит ли пользователь в гурппе админов.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if user admin by role, false if not.</returns>
      static bool isUserAdminByRole(void);


      /// <summary>Перезапускает текущее приложения для поднятия прав. 
      /// 				 Текущее приложение закрывается через QCoreApplication::exit(). 
      /// 				 Необходимо учитывать, иначе, если остануться неубитые потоки, приложение не закроется.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      static bool restartToElevateRights();

    private:
      static HRESULT elevationType(__out TOKEN_ELEVATION_TYPE * ptet);
    };

  }
}
