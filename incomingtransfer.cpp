/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2010 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "incomingtransfer.h"

#include "incomingtransferthread.h"
#include <qdir.h>

using namespace Kapotah;

IncomingTransfer::IncomingTransfer (QList< TransferFile > files,
                                    QHostAddress peer, QObject* parent) : Transfer (files, peer, parent)
{
    //Calculate total number of files and total size
    foreach (TransferFile file, files) {
        ++m_numFiles;
        m_totalSize += file.size;
    }
}

IncomingTransfer::~IncomingTransfer()
{

}

void IncomingTransfer::start()
{
    if (m_destinationDir.isEmpty()) {
        emit needDestinationDir();
    } else {
        m_filesIterator = m_files.begin();
        startNextFile();
    }
}

Transfer::TransferType IncomingTransfer::type()
{
    return Incoming;
}

void IncomingTransfer::startNextFile()
{
    QDir destinationDir(m_destinationDir);
    m_thread = new IncomingTransferThread (m_peerAddress, m_filesIterator->id,
                                           destinationDir.absoluteFilePath(m_filesIterator->path),
                                           m_filesIterator->size, this);
    connect (m_thread, SIGNAL (done (QString)), SLOT (currentFileFinished()));
    connect (m_thread, SIGNAL (progress (quint64, quint64)), SLOT (reportProgress (quint64, quint64)));
    m_thread->start();
}

void IncomingTransfer::currentFileFinished()
{
    m_thread->wait();
    ++m_filesDone;
    m_sizeDone += m_filesIterator->size;
    ++m_filesIterator;

    if (m_filesIterator == m_files.end()) {
        emit done();
    } else {
        startNextFile();
    }
}

void IncomingTransfer::setDestinationDir (QString path)
{
    m_destinationDir = path;
}

void IncomingTransfer::reportProgress (quint64 done, quint64 size)
{
    emit progress (m_sizeDone + done, m_totalSize);
}

#include "incomingtransfer.moc"