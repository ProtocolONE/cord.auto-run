/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (�) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#ifndef _GGS_AUTORUNHELPER_UACHELPER_H_
#define _GGS_AUTORUNHELPER_UACHELPER_H_

#include "autorunhelper_global.h"
#include <Windows.h>
#include <Shellapi.h>

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include <QtCore/QDebug>

namespace GGS {
  namespace AutoRunHelper {

    /// <summary>����� ��������� ������ �� ����������� ������� ���� ������������ � ����� ��� �������� ���� ����������. </summary>
    /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
    class AUTORUNHELPER_EXPORT UACHelper
    {
    public:
      UACHelper(void);
      ~UACHelper(void);

      /// <summary>�� ����� � ������� ��������� ������� UAC � ������� �� ��.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      static bool uacEnabled(void);


      /// <summary>Query if this object is user admin.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if user admin, false if not.</returns>
      static bool isUserAdmin(void);
      

      /// <summary>Query if this object is user elevated admin.</summary>
      /// <remarks>Ilya.Tkachenko, 08.03.2012.</remarks>
      /// <returns>true if user elevated admin, false if not.</returns>
      static bool isUserElevatedAdmin(void);


      /// <summary>��������� ������� �� ������������ � ������ �������.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if user admin by role, false if not.</returns>
      static bool isUserAdminByRole(void);


      /// <summary>������������� ������� ���������� ��� �������� ����.</summary>
      /// <remarks>Ilya.Tkachenko, 07.03.2012.</remarks>
      /// <returns>true if it succeeds, false if it fails.</returns>
      static bool restartToElevateRights();

    private:
      static HRESULT elevationType(__out TOKEN_ELEVATION_TYPE * ptet);
    };

  }
}
#endif // _GGS_AUTORUNHELPER_UACHELPER_H_