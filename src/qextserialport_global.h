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
#ifdef _TTY_NOWARN_
#  define _TTY_NOWARN_PORT_
#endif

/*macros for warning and debug messages*/
#ifdef _TTY_NOWARN_PORT_
#  define TTY_PORTABILITY_WARNING(s)
#else
#  define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_PORT_*/

#ifdef _TTY_NOWARN_
#  define TTY_WARNING(s)
#else
#  define TTY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_*/

#endif // QEXTSERIALPORT_GLOBAL_H

