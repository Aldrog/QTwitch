#ifndef QTWITCH_GLOBAL_H
#define QTWITCH_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QMetaType>
#include <memory>

#if defined(QTWITCH_LIBRARY)
#  define QTWITCHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTWITCHSHARED_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
Q_DECLARE_SMART_POINTER_METATYPE(std::unique_ptr)

#endif // QTWITCH_GLOBAL_H
