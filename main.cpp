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

#include <QtGui/QApplication>
#include "dialog.h"
#include "server.h"
#include "fileserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server server;
    if (!server.listen( QHostAddress::Any, 9876 ))
        qDebug() << "SERVER LISTEN FAILED";

    FileServer fserver;
    if (!fserver.listen( QHostAddress::Any, 9877 ))
        qDebug() << "FILE SERVER LISTEN FAILED";

    Dialog w(&server,&fserver);
    w.show();
    return a.exec();
}
