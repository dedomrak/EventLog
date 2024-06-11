#ifndef EVENTLOG_GLOBAL_H
#define EVENTLOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EVENTLOG_LIBRARY)
#  define EVENTLOG_EXPORT Q_DECL_EXPORT
#else
#  define EVENTLOG_EXPORT Q_DECL_IMPORT
#endif

#endif // EVENTLOG_GLOBAL_H
