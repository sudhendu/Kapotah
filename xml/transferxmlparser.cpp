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

#include "transferxmlparser.h"
#include <QXmlStreamReader>

QString TransferXMLParser::composeXML (AbstractXMLData* data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("transfer");

    foreach (Kapotah::TransferFile file, static_cast<TransferXMLData*> (data)->files) {
        writer.writeStartElement ("file");
        writer.writeAttribute ("id", file.id);
        writer.writeAttribute ("path", file.path);
        writer.writeAttribute ("size", QString::number (file.size));
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXMLData* TransferXMLParser::parseXML (const QString& xml)
{
    QXmlStreamReader reader (xml);
    reader.readNext();

    TransferXMLData *data = new TransferXMLData;
    data->type = AbstractXMLData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "transfer") {
                data->type = AbstractXMLData::Transfer;
                data->files.clear();
                reader.readNext();

                while (!reader.atEnd()) {
                    if (reader.isStartElement()) {
                        if (reader.name() == "file") {
                            Kapotah::TransferFile file = { reader.attributes().value ("id").toString(),
                                                           reader.attributes().value ("path").toString(),
                                                           reader.attributes().value ("size").toString().toInt()
                                                         };
                            data->files.append (file);
                        }
                    } else {
                        if (reader.name() == "transfer") {
                            return data;
                        }
                    }
                    reader.readNext();
                }
                break;
            }
        } else {
            reader.readNext();
        }
    }

    return data;
}
