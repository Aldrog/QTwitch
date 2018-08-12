#ifndef QTWITCH_GLOBAL_H
#define QTWITCH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTWITCH_LIBRARY)
#  define QTWITCHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTWITCHSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTWITCH_GLOBAL_H
