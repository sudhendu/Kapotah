/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>

class QVBoxLayout;

namespace Kapotah
{
    class Transfer;
}

using namespace Kapotah;

class TransferDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TransferDialog (QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~TransferDialog();

    private slots:
        void addTransfer (Transfer *transfer);

    private:
        QVBoxLayout *m_layout;
};

#endif // TRANSFERDIALOG_H