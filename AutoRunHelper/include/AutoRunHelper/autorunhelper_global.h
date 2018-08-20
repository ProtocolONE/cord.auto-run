#pragma once

#include <QtCore/qglobal.h>

#ifdef AUTORUNHELPER_STATIC_LIB
  # define AUTORUNHELPER_EXPORT 
#else

#ifdef AUTORUNHELPER_LIB
# define AUTORUNHELPER_EXPORT Q_DECL_EXPORT
#else
# define AUTORUNHELPER_EXPORT Q_DECL_IMPORT
#endif

#endif