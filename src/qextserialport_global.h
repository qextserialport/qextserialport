#ifndef QEXTSERIALPORT_GLOBAL_H
#define QEXTSERIALPORT_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(Q_WS_WIN)
#  if !defined(QEXTSERIALPORT_EXPORT) && !defined(QEXTSERIALPORT_IMPORT)
#    define QEXTSERIALPORT_EXPORT
#  elif defined(QEXTSERIALPORT_IMPORT)
#    if defined(QEXTSERIALPORT_EXPORT)
#      undef QEXTSERIALPORT_EXPORT
#    endif
#    define QEXTSERIALPORT_EXPORT __declspec(dllimport)
#  elif defined(QEXTSERIALPORT_EXPORT)
#    undef QEXTSERIALPORT_EXPORT
#    define QEXTSERIALPORT_EXPORT __declspec(dllexport)
#  endif
#else
#  define QEXTSERIALPORT_EXPORT
#endif

#endif // QEXTSERIALPORT_GLOBAL_H

