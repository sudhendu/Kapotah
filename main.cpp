/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>
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

#include <QApplication>
#include <QDebug>
#include <udpmanager.h>
#include <announcer.h>
#include <messagedispatcher.h>
#include <peermanager.h>
#include <ui/peerdialog.h>
#include <fileserversingleton.h>
#include <peerenvironment.h>
#include <filesearcher.h>
#include <debug.h>

static const int s_messageServerPort = 45001;
static const int s_fileServerPort = 45002;

int main (int argc, char** argv)
{
    QApplication app (argc, argv);
    QApplication::setOrganizationName("BMSCE");
    QApplication::setOrganizationDomain("bmsce.in");
    QApplication::setApplicationName("Kapotah");

    if (QSystemTrayIcon::isSystemTrayAvailable())
        QApplication::setQuitOnLastWindowClosed(false);

    Kapotah::UdpManager::instance();   //Start the broadcast engine
    Kapotah::Announcer::instance()->setUserName(Kapotah::PeerEnvironment::getPeerName()); //Start the announcer
    Kapotah::PeerManager::instance();
    if (!Kapotah::MessageDispatcher::instance()->messageDispatcherServer()->listen(QHostAddress::Any,
        s_messageServerPort))
        qFatal("Can't listen to message port");
    if (!Kapotah::FileServerSingleton::instance()->fileServer()->listen(QHostAddress::Any,
        s_fileServerPort))
        qFatal("Can't listen to file port");
    PeerDialog peerDialog;
    peerDialog.show();
    return app.exec();
}
