 /*
 * Copyright (C) 2023 HDGNSS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "config.h"
#include "kmlgenerator.h"

#include <QDebug>
#include <QDate>
#include <QTimeZone>


KmlGenerator::KmlGenerator(){
    mKmlFile = new QFile();
    mKmlStream = new QXmlStreamWriter();
}

KmlGenerator::~KmlGenerator(){
    delete mKmlFile;
    delete mKmlStream;
}


int KmlGenerator::onGenerateKml(QString filename, QVector<KmlPoint> points, QString color, QString icon){
    mKmlFile->setFileName(filename);

    if (!mKmlFile->open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Output file can not open\n";
        return -1;
    }

    mKmlStream->setDevice(mKmlFile);

    mKmlStream->setAutoFormatting(true);
     
    mKmlStream->writeStartDocument();
    mKmlStream->writeStartElement("kml"); //kml
    mKmlStream->writeAttribute("xmlns", "http://earth.google.com/kml/2.0");

    mKmlStream->writeStartElement("Document"); // Document
    mKmlStream->writeTextElement("name", filename);

    onPoints(points, icon);
    onCourse(points, color);

    mKmlStream->writeStartElement("Style"); //Style
    mKmlStream->writeAttribute("id", "khStyle526_default");
    mKmlStream->writeStartElement("IconStyle"); //IconStyle
    mKmlStream->writeAttribute("id", "khIconStyle528_default");
    mKmlStream->writeTextElement("color", mColorNormalHash[color]);
    mKmlStream->writeEndElement(); // End IconStyle
    mKmlStream->writeStartElement("LabelStyle"); //LabelStyle
    mKmlStream->writeAttribute("id", "khLabelStyle530_default");
    mKmlStream->writeTextElement("color", "ffffff");
    mKmlStream->writeEndElement(); // End LabelStyle
    mKmlStream->writeStartElement("LineStyle"); //LineStyle
    mKmlStream->writeAttribute("id", "khLineStyle531_default");
    mKmlStream->writeEndElement(); // End LineStyle
    mKmlStream->writeStartElement("PolyStyle"); //PolyStyle
    mKmlStream->writeAttribute("id", "khPolyStyle532_default");
    mKmlStream->writeEndElement(); // End PolyStyle
    mKmlStream->writeEndElement(); // End Style

    mKmlStream->writeStartElement("Style"); //Style
    mKmlStream->writeAttribute("id", "khStyle526_default_dark");
    mKmlStream->writeStartElement("IconStyle"); //IconStyle
    mKmlStream->writeAttribute("id", "khIconStyle528_default");
    mKmlStream->writeTextElement("color", mColorDarkHash[color]);
    mKmlStream->writeEndElement(); // End IconStyle
    mKmlStream->writeStartElement("LabelStyle"); //LabelStyle
    mKmlStream->writeAttribute("id", "khLabelStyle530_default");
    mKmlStream->writeTextElement("color", "ffffff");
    mKmlStream->writeEndElement(); // End LabelStyle
    mKmlStream->writeStartElement("LineStyle"); //LineStyle
    mKmlStream->writeAttribute("id", "khLineStyle531_default");
    mKmlStream->writeEndElement(); // End LineStyle
    mKmlStream->writeStartElement("PolyStyle"); //PolyStyle
    mKmlStream->writeAttribute("id", "khPolyStyle532_default");
    mKmlStream->writeEndElement(); // End PolyStyle
    mKmlStream->writeEndElement(); // End Style


    mKmlStream->writeEndElement(); // End Document

    mKmlStream->writeEndElement(); // End kml
    mKmlStream->writeEndDocument();

    mKmlFile->close();
    return 0; 
}


void KmlGenerator::onPoints(QVector<KmlPoint> points, QString icon){
    const QString format = "%1,%2,%3";

    mKmlStream->writeStartElement("Folder"); // FolderPoints
    mKmlStream->writeTextElement("name", "Points");

    QDateTime datetime;
    datetime.setTimeZone(QTimeZone::utc());

    foreach (KmlPoint point, points) {
        bool isdf = false;
        qint64 dutc = point.position.utc.toLongLong() - mLastUtc;

        dutc = (dutc<3600000)?dutc:3600000;
        mLastUtc = point.position.utc.toLongLong();
        datetime.setMSecsSinceEpoch(point.position.utc.toLongLong());
        mKmlStream->writeStartElement("Placemark"); // Placemark
        mKmlStream->writeTextElement("name", datetime.toString("yyyy/MM/dd HH:mm:ss.zzz")); 

        mKmlStream->writeStartElement("description");
        mKmlStream->writeCDATA(onGenerateHtml(point, &isdf, dutc));
        mKmlStream->writeEndElement(); // END description

        if(isdf){
            mKmlStream->writeTextElement("styleUrl", "#khStyle526_default_dark");
        }
        else{
            mKmlStream->writeTextElement("styleUrl", "#khStyle526_default");
        }

        float scale = 0.3 + dutc/5000.0;
        
        scale = (scale<1.5)?scale:1.5;

        mKmlStream->writeStartElement("Style");
        mKmlStream->writeStartElement("IconStyle");
        mKmlStream->writeTextElement("scale", QString::number(scale,'f', 1));
        mKmlStream->writeTextElement("heading", point.position.bearing);
        mKmlStream->writeStartElement("Icon");

        double speed = point.position.speed.toDouble();
        if(speed < 0.539){
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/static.png");
        }
        else if(speed < (0.539*10)){
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/walking.png");
        }
        else if(speed < (0.539*35)){
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/biking.png");
        }
        else if(speed < (0.539*150)){
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/auto.png");
        }
        else if(speed < (0.539*900)){
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/plane.png");
        }
        else{
            mKmlStream->writeTextElement("href", "https://tools.hdgnss.com/kml/"+icon+"/rocket.png");
        }
        
        mKmlStream->writeEndElement(); //Icon
        mKmlStream->writeEndElement(); //IconStyle
        mKmlStream->writeEndElement(); //Style

        mKmlStream->writeStartElement("MultiGeometry");
        mKmlStream->writeStartElement("Point");
        mKmlStream->writeTextElement("coordinates", QString(format).arg(point.position.longitude, point.position.latitude, point.position.altitude));
        mKmlStream->writeEndElement(); //Point
        mKmlStream->writeEndElement(); //MultiGeometry

        mKmlStream->writeEndElement(); // END Placemark
    }
    mKmlStream->writeEndElement(); // End FolderPoints
}

QString KmlGenerator::onGenerateHtml(KmlPoint point, bool *isdf, long dutc){
    const QHash<QString,QString> constellation = {{"0","U"}, {"1","G"},{"2","R"},{"3","E"},{"4","C"},{"5","J"},{"6","I"},{"7","U"},{"8","U"},{"9","U"}};
    QString html;
    
    QXmlStreamWriter htmlStream(&html);

    int view=0, used=0, l1=0, l2=0, l5=0;
    for(int j=0; j<point.satellites.size(); j++){
        if(!point.satellites[j]["L1SNR"].isEmpty()){
            view++;
            l1=1;
        }
        if(!point.satellites[j]["L2SNR"].isEmpty()){
            view++;
            l2=1;
        }
        if(!point.satellites[j]["L5SNR"].isEmpty()){
            view++;
            l5=1;
        }
        if(point.satellites[j]["L1Used"] == "1"){
            used++;
        }
        if(point.satellites[j]["L2Used"] == "1"){
            used++;
        }
        if(point.satellites[j]["L5Used"] == "1"){
            used++;
        }
    }
    *isdf = ((l1+l2+l5)>1);

    htmlStream.writeStartElement("table");
    htmlStream.writeStartElement("tr");
    htmlStream.writeTextElement("td", "Sats:");
    htmlStream.writeTextElement("td", QString::number(point.satellites.size()));
    htmlStream.writeTextElement("td", "View(CH):");
    htmlStream.writeTextElement("td", QString::number(view));
    htmlStream.writeTextElement("td", "Use(CH):");
    htmlStream.writeTextElement("td", QString::number(used));
    htmlStream.writeTextElement("td", "EA:");
    htmlStream.writeTextElement("td", point.position.dop);
    htmlStream.writeTextElement("td", QString("ΔT(ms):"));
    htmlStream.writeTextElement("td", QString::number(dutc));
    htmlStream.writeTextElement("td", "Speed(m/s):");
    htmlStream.writeTextElement("td", QString::number(point.position.speed.toDouble()*0.514444, 'f', 2));
    htmlStream.writeEndElement(); // END tr
    htmlStream.writeEndElement(); // END table


    if(point.satellites.size() > 0){
        htmlStream.writeStartElement("table");   
        
        htmlStream.writeStartElement("tr");
        htmlStream.writeStartElement("th"); //th
        htmlStream.writeAttribute("style", "text-align:center;");
        htmlStream.writeCharacters("");
        htmlStream.writeEndElement(); // END th
        for(int j=0; j<point.satellites.size(); j++){
            htmlStream.writeStartElement("th"); //th
            htmlStream.writeAttribute("style", "text-align:center;");
            htmlStream.writeCharacters(constellation[point.satellites[j]["System"]]);
            htmlStream.writeEndElement(); // END th
        }
        htmlStream.writeEndElement(); // END tr
   

        const QList<QString> infos = {"PRN", "Elevation", "Azimuth"};
        const QList<QString> infot = {"PRN", "EL", "AZ"};
        for(int i=0; i<infos.size(); i++){
            htmlStream.writeStartElement("tr");
            htmlStream.writeStartElement("td"); //td
            htmlStream.writeAttribute("style", "text-align:center;");
            htmlStream.writeCharacters(infot[i]);
            htmlStream.writeEndElement(); // END td
            for(int j=0; j<point.satellites.size(); j++){
                htmlStream.writeStartElement("td"); //td
                htmlStream.writeAttribute("style", "text-align:center;");
                htmlStream.writeCharacters(point.satellites[j][infos[i]]);
                htmlStream.writeEndElement(); // END td
            }
            htmlStream.writeEndElement(); // END tr
        }

        const QList<QString> snrs = {"L1SNR", "L2SNR", "L5SNR"};
        const QList<QString> snrt = {"L1", "L2", "L5"};
        const QList<QString> used = {"L1Used", "L2Used", "L5Used"};
        for(int i=0; i<snrs.size(); i++){
            htmlStream.writeStartElement("tr");
            htmlStream.writeStartElement("td"); //td
            htmlStream.writeAttribute("style", "text-align:center;");
            htmlStream.writeCharacters(snrt[i]);
            htmlStream.writeEndElement(); // END td
            for(int j=0; j<point.satellites.size(); j++){
                if(point.satellites[j][used[i]]=="1"){
                    htmlStream.writeStartElement("td"); //td
                    htmlStream.writeAttribute("style", "color:DodgerBlue;text-align:center;");
                    htmlStream.writeCharacters(point.satellites[j][snrs[i]]);
                    htmlStream.writeEndElement(); // END td
                }
                else{
                    htmlStream.writeStartElement("td"); //td
                    htmlStream.writeAttribute("style", "text-align:center;");
                    htmlStream.writeCharacters(point.satellites[j][snrs[i]]);
                    htmlStream.writeEndElement(); // END td
                }
            }
            htmlStream.writeEndElement(); // END tr
        }
        htmlStream.writeEndElement(); // END table
    }

    htmlStream.writeStartElement("p"); //p
    htmlStream.writeAttribute("align", "right");
    htmlStream.writeStartElement("font"); //font
    htmlStream.writeAttribute("color", "gray");
    htmlStream.writeCharacters(QString("%1:%2, Copyright © 2024 HDGNSS").arg(PROJECT_NAME, PROJECT_VER));
    htmlStream.writeEndElement(); // END font
    htmlStream.writeEndElement(); // END p
    return html;
}

void KmlGenerator::onCourse(QVector<KmlPoint> points, QString color)
{
    const QString format = " %1,%2,%3";
    QString course = "";
    foreach (KmlPoint point, points) {
        course += QString(format).arg(point.position.longitude, point.position.latitude, point.position.altitude);
    }
    mKmlStream->writeStartElement("Folder"); // FolderCourse
    mKmlStream->writeTextElement("name", "Course");
    mKmlStream->writeStartElement("Placemark"); // Placemark
    mKmlStream->writeStartElement("Style");
    mKmlStream->writeStartElement("LineStyle");
    mKmlStream->writeTextElement("color", mColorNormalHash[color]);
    mKmlStream->writeEndElement(); //LineStyle
    mKmlStream->writeEndElement(); //Style

    mKmlStream->writeStartElement("LineString");
    
    mKmlStream->writeTextElement("coordinates", course);
    mKmlStream->writeEndElement(); //LineString

    mKmlStream->writeEndElement(); // END Placemark
    mKmlStream->writeEndElement(); // End FolderCourse
}
