 /*
 * Copyright (C) 2024 HDGNSS
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
#ifndef NMEABASE_H
#define NMEABASE_H

#include "gnsstype.h"

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class NmeaBase
{
public:
    NmeaBase(bool debug = false);
    ~NmeaBase();
    bool setBase(QString base);
    void initBase(QString nmeafile);
    void closeBase();
    QString getVersion(void);

    void getKmlPoints(QVector<KmlPoint> *points, bool stt);
    void getUtcList(QVector<QString> *utcs);
    void getSatellite(QString utc, QStringList parameters, QString system, QVector<QHash<QString, QString>> *satellites);
    void getPlotPoints(QStringList parameters, QString table, QString condition, GnssPlotData *data);
    void getSatPoints(QString system, GnssPlotData *data);
    void getGllVersion(GnssGllVersion *version);
    void getStartUtc(QString table, qint64 *utc);

private:
    QSqlDatabase mDatabase;
    QSqlQuery mSqlQuery;
    bool mDebugEnabled;
    const QString mTAG = "NMEA";
};

#endif // NMEABASE_H