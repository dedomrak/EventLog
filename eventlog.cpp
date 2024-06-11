#include "eventlog.h"

QEventLogger::QEventLogger(const QString & logFileBaseName,
                           QWidget * mainWidget,
                           const bool screenshotsEnabled,
                           QObject * parent) : QObject(parent), mainWidget(mainWidget), screenshotsEnabled(screenshotsEnabled) {
    // Build log file name.
    QDateTime now = QDateTime::currentDateTime();
    QString fullLogFileName = logFileBaseName + " " + now.toString(Qt::ISODate).replace(":", "-") + ".csv";

    // Open log file.
    this->logFile = new QFile(fullLogFileName);
    if (!this->logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        exit(1);
    this->log = new QTextStream(this->logFile);

    // Write header to log file.
    *log << "; Date and time are: " << now.toString(Qt::ISODate) << '\n';
   // *log << "; Resolution: " << mainWidget->size().width() << 'x' << mainWidget->size().height() << '\n';
    *log << "time,input type,event type,target widget class,details\n";
    log->flush();

    // Create the dir in which screenshots will be stored, if requested.
    if(screenshotsEnabled) {
        screenshotDirName = "./screenshots " + now.toString(Qt::ISODate).replace(":", "-");
        qDebug() << QDir().mkdir(screenshotDirName);
    }

    // Start timer.
    this->time = new QTime();
    this->time->start();
}

bool QEventLogger::eventFilter(QObject * obj, QEvent * event) {
  //  static QMouseEvent * mouseEvent;
    static QKeyEvent * keyEvent;
    static QString eventType, details;
    static int inputType,  id;
    static QString inputTypeAsString, className, targetWidget;
    inputType = NONE;

    inputTypeAsString = "";

    switch (event->type()) {
    case QEvent::FileOpen:
        inputType = FILE;
        eventType = "FileOpen";
        break;
    case QEvent::KeyPress:
        inputType = CONSOLE;
        eventType = "KeyPress";
    case QEvent::KeyRelease:
        inputType = CONSOLE;
        eventType = "KeyRelease";
        break;
    case 1100:   //User event for serial port connections
        inputType = SERIAL;
        eventType = "SerialConnections";
        break;
    case QEvent::SockAct:
        inputType = TCP;
        eventType = "TcpConnection";
        break;
    default:
        break;
    }

    if (inputType == CONSOLE) {
            keyEvent = static_cast<QKeyEvent *>(event);

            // Build the details string.
            details = "";
            details += QString::number(keyEvent->key());
            details += ';' + keyEvent->text();
            inputTypeAsString = "Keyboard";
        }

      else if (inputType == TCP) {
        details = "";
        inputTypeAsString = "TCP Connection";

           // qDebug() << focusEvent << obj->metaObject()->className();
       }

    else if (inputType == SERIAL) {
      details = "";
      inputTypeAsString = "SERIAL Connection";

         // qDebug() << focusEvent << obj->metaObject()->className();
     }

    if (!inputTypeAsString.isEmpty()) {
        className = obj->metaObject()->className();
        if (!this->widgetPointerToID.contains(className) || !this->widgetPointerToID[className].contains(obj)) {
            this->widgetPointerToID[className][obj] = this->widgetPointerToID[className].size();
        }
        id = this->widgetPointerToID[className][obj];
        targetWidget = className + " " + QString::number(id);
        this->appendToLog(inputTypeAsString, eventType, targetWidget, details);
    }

    return false;
}

void QEventLogger::appendToLog(const QString & inputType, const QString & eventType, const QString & targetWidget, const QString & details) {
    const int elapsedTime = this->time->elapsed();
    *(this->log) << elapsedTime << ',' << inputType<< ',' << eventType << ',' << targetWidget << ',' << details << '\n';
    //qDebug() << elapsedTime << inputType << eventType << targetWidget << details;
    this->log->flush();
}
