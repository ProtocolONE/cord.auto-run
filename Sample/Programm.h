/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#ifndef _SAMPLE_TEST_PROGRAMM_H_
#define _SAMPLE_TEST_PROGRAMM_H_

#include "UACHelper.h"
#include "AutoRunHelper.h"

#include <QtCore/QObject>
#include <QtCore/QDebug>


class Programm : public QObject
{
  Q_OBJECT
public:
  Programm(void);
  ~Programm(void);

public slots:
  void start();
};

#endif // _SAMPLE_TEST_PROGRAMM_H_