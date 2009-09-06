/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "fileserver.h"
#include "fileserverthread.h"

FileServer::FileServer() : QTcpServer()
{
}

bool FileServer::idExists(QString ID)
{
    return fileList.contains(ID);
}

QString FileServer::getFileName(QString ID)
{
    return fileList[ID];
}


void FileServer::addFile(QString ID, QString FileName)
{
    fileList[ID] = FileName;
}

void FileServer::incomingConnection(int descriptor)
{
    FileServerThread *thread = new FileServerThread(descriptor, this);
    connect(thread, SIGNAL(startedTransfer(QString,QString)), this, SIGNAL(startedTransfer(QString,QString)));
    connect(thread, SIGNAL(finishedTransfer(QString,QString)), this, SIGNAL(finishedTransfer(QString,QString)));
    thread->start();
}
