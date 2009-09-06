#ifndef DIRRECIEVERTHREAD_H
#define DIRRECIEVERTHREAD_H

#include <QThread>
#include <QDomNodeList>
#include "peermanager.h"
#include "filerecieverthread.h"
#include "pointers.h"

class DirRecieverThread : public QThread
{
    Q_OBJECT

private:
    Pointers *m_ptr;
    PeerManager *manager;
    QDomNodeList fileList;
    QDomNodeList dirList;
    QString peerName;
    QString dirName;
    FileRecieverThread *fileReciever;
    bool cancel;
    QString m_ID;
public:
    DirRecieverThread(Pointers *ptr, QString ID, QString dir, QDomNodeList files, QDomNodeList dirs,
                      QString peername, QObject *parent);
    ~DirRecieverThread();
    void run();
    void stopTransfer();
signals:
    void done(QString ID);
    void progress(QString ID, int filesDone);
};

#endif // DIRRECIEVERTHREAD_H
