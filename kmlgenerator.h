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

#ifndef KMLGENERATOR_H
#define KMLGENERATOR_H

#include "gnsstype.h"

#include <QHash>
#include <QFile>
#include <QString>
#include <QXmlStreamWriter>


class KmlGenerator
{
public:
    KmlGenerator();
    ~KmlGenerator();
    int onGenerateKml(QString filename, QVector<KmlPoint> points, QString color, QString icon);

private:
    QString onGenerateHtml(KmlPoint point, bool *df, long dutc);
    void onPoints(QVector<KmlPoint> points, QString icon);
    void onCourse(QVector<KmlPoint> points, QString color);
    
    qint64 mLastUtc = 0;
    QXmlStreamWriter *mKmlStream;
    QFile *mKmlFile;

    const QHash<QString, QString> mColorNormalHash = {{"red","ff1216d6"},{"green","ff058d0e"},{"blue","ffbe9a41"},{"orange","ff0584fb"},{"yellow","ff02cbfa"},{"indigo","ffd6625c"},{"magenta","ff7600d9"},{"white","ffffffff"}};
    const QHash<QString, QString> mColorDarkHash = {{"red","ff0a0d80"},{"green","ff035408"},{"blue","ff725c27"},{"orange","ff034f96"},{"yellow","ff017996"},{"indigo","ff803a37"},{"magenta","ff460082"},{"white","ffe5e5e5"}};

};
#endif // KMLGENERATOR_H