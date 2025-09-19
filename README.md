Build with Qt5.14 for Windows 10 and Fedora 27 
Produce DLL which can be used in Windows 10
Add the QEventLog.h and QEventLog.cpp files to your project, and add
them to your .pro file.
Include QEventLog.h in MyApplication.h and add a QEventLogger member:
 QEventLog * eventLog;
In the MyApplication constructor, initialize the event logger and install it as an event filter
