#ifndef QEXTSERIALPORT_GLOBAL_H
#define QEXTSERIALPORT_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(Q_OS_WIN)
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


/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef QESP_NO_WARN
#  define QESP_NO_PORTABILITY_WARN
#endif

/*macros for warning and debug messages*/
#ifdef QESP_NO_PORTABILITY_WARN
#  define QESP_PORTABILITY_WARNING  while(false)qWarning
#else
#  define QESP_PORTABILITY_WARNING qWarning
#endif /*QESP_NOWARN_PORT*/

#ifdef QESP_NO_WARN
#  define QESP_WARNING while(false)qWarning
#else
#  define QESP_WARNING qWarning
#endif /*QESP_NOWARN*/

#endif // QEXTSERIALPORT_GLOBAL_H

