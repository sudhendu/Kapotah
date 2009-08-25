# -------------------------------------------------
# Project created by QtCreator 2009-07-22T11:50:41
# -------------------------------------------------
QT += network \
    webkit \
    xml
TARGET = Chataroma
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    peermanager.cpp \
    peerinfo.cpp \
    chatdialog.cpp \
    messagesender.cpp \
    server.cpp \
    filesenderthread.cpp \
    fileserver.cpp \
    fileserverthread.cpp \
    filerecieverthread.cpp
HEADERS += dialog.h \
    peermanager.h \
    peerinfo.h \
    chatdialog.h \
    messagesender.h \
    server.h \
    filesenderthread.h \
    fileserver.h \
    fileserverthread.h \
    filerecieverthread.h
FORMS += dialog.ui \
    chatdialog.ui