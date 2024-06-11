#ifndef EVENTLOG_H
#define EVENTLOG_H

#include "EventLog_global.h"


#include <QObject>
//#include <QMouseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QEvent>
#include <QTcpSocket>
#include <QSerialPort>
#include <QFileOpenEvent>
#include <QDebug>

#define NONE -1
#define TCP 0
#define SERIAL 1
#define CONSOLE 2
#define FILE 3

class QEventLogger : public QObject {
    Q_OBJECT

public:
    explicit QEventLogger(const QString & logFileBaseName, QWidget * mainWidget, const bool screenshotsEnabled, QObject * parent = 0);

protected:
    bool eventFilter(QObject * obj, QEvent * event);
    void appendToLog(const QString & inputType, const QString & eventType, const QString & targetWidget, const QString & details);

private:
    const bool screenshotsEnabled;
    QString screenshotDirName;
    QWidget * mainWidget;
    QFile * logFile;
    QTextStream * log;
    QTime * time;
    QHash<QString, QHash<QObject *, uint> > widgetPointerToID;
};

#endif // EVENTLOG_H
