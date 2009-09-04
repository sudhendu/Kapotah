#include "filestatusdialog.h"
#include "server.h"
#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>

FileStatusDialog::FileStatusDialog(Pointers *ptr, QWidget *parent) :
    QDialog(parent), m_ptr(ptr) {
    m_ui.setupUi(this);
    filePauseMapper = new QSignalMapper(this);
    fileCancelMapper = new QSignalMapper(this);

    connect(fileCancelMapper, SIGNAL(mapped(QString)), this, SLOT(fileCancelClicked(QString)));
    connect(filePauseMapper, SIGNAL(mapped(QString)), this, SLOT(filePauseClicked(QString)));

    connect(ptr->server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
    connect(ptr->server, SIGNAL(dirRecieved(QDomNodeList,QDomNodeList,QString)), this, SLOT(dirRecieved(QDomNodeList,QDomNodeList,QString)));
}

void FileStatusDialog::dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username)
{
    show();
    //addTransfer(fileList, dirList, QString("Directory"), 0, QString("TODO"), username, false, true);   //TODO
}

void FileStatusDialog::fileRecieved(QString filename,qint64 size,QString ID,QString username)
{
    show();
    addFileTransfer(filename, size, ID, username, false);
}

void FileStatusDialog::addFileTransfer(QString filename, qint64 size, QString ID, QString senderName, bool isUpload)
{
    FileTransfer *transfer = new FileTransfer;

    //Create the new widgets
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause = new QToolButton();
    QToolButton *cancel = new QToolButton();
    QToolButton *type = new QToolButton();

    //Store info
    transfer->box = box;
    transfer->progress = progress;
    transfer->fileName = filename;
    transfer->inProgress = false;
    transfer->senderName = senderName;
    transfer->cancel = cancel;
    transfer->pause = pause;
    transfer->isUpload = isUpload;

    /*if (isDir) {
        transfer->fileList = fileList;
        transfer->dirList = dirList;
        qint64 totalSize=0;

        //Calculate total size of dir
        for (int i=0;i<fileList.count();i++) {
            totalSize += fileList.at(i).toElement().attribute("size").toLongLong();
        }
        transfer->fileSize = totalSize;
        transfer->bytesDone = 0;
    }*/
    transfer->fileSize = size;
    fileTransfers[ID] = transfer;

    //Set the widgets

    pause->setAutoRaise(true);
    cancel->setAutoRaise(true);
    type->setAutoRaise(true);

    pause->setArrowType(Qt::RightArrow);
    if (isUpload) {
        type->setArrowType(Qt::UpArrow);
        type->setToolTip("This Is A Upload");
    }
    else {
        type->setIcon(QIcon(":/images/kget.png"));
        type->setToolTip("This Is A Download");
    }

    //Layout the new widgets
    layout->addWidget(progress, 0, 0);
    layout->addWidget(pause, 0, 1);
    layout->addWidget(cancel, 0, 2);
    layout->addWidget(type, 0, 3);
    box->setLayout(layout);
    box->setTitle(filename + " with " + senderName);

    //Add the widgets to the ScrollArea

    //Remove the spacer
    m_ui.scrollAreaWidgetContents->layout()->takeAt(m_ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the group box
    m_ui.scrollAreaWidgetContents->layout()->addWidget(box);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout())->addStretch();

    //Connect signals
    connect(cancel, SIGNAL(clicked()), fileCancelMapper, SLOT(map()));
    connect(pause, SIGNAL(clicked()), filePauseMapper, SLOT(map()));

    //Set mapping
    fileCancelMapper->setMapping(cancel, ID);
    filePauseMapper->setMapping(pause, ID);
}

void FileStatusDialog::fileCancelClicked(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    transfer->reciever->stopTransfer();
    transfer->progress->setMaximum(0);
    transfer->progress->setValue(0);
    transfer->inProgress = false;
}

void FileStatusDialog::filePauseClicked(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    if (transfer->inProgress) {        //If already started download
        transfer->reciever->start();
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save",
                                                    QDir::home().absoluteFilePath(transfer->fileName)); //CHANGES
    if (filename=="")
        return;
    transfer->savePath = filename;
    transfer->reciever = new FileRecieverThread(m_ptr, ID, transfer->fileSize,
                                                     transfer->senderName, transfer->savePath, this);
    connect(transfer->reciever, SIGNAL(progress(QString,QString,QString,qint64,qint64)),
            this, SLOT(fileProgress(QString,QString,QString,qint64,qint64)));
    connect(transfer->reciever, SIGNAL(done(QString)), this, SLOT(fileDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfer->inProgress = true;
}

void FileStatusDialog::fileProgress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone)
{
    FileTransfer *transfer = fileTransfers[ID];
    if (!transfer->inProgress)
        return;

    transfer->progress->setMaximum(size);
    transfer->progress->setValue(bytesDone);
    transfer->box->setToolTip(QString::number(bytesDone) + " of " + QString::number(size));
}

void FileStatusDialog::fileDone(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    transfer->pause->hide();
    transfer->cancel->hide();
}

void FileStatusDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

