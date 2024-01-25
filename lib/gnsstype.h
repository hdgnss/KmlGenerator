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

#ifndef GNSSTYPE_H
#define GNSSTYPE_H

#include <QHash>
#include <QString>
#include <QVector>

typedef struct
{
    QString  prn;
    QString  elevation;
    QString  azimuth;
    QString  snr_l1;
    QString  snr_l2;
    QString  snr_l5;
    QString  system;
    QString  signal;
    QString  used_l1;
    QString  used_l2;
    QString  used_l5;
    QString  info;
    QString  pr_code_l1;
    QString  rr_code_l1;
    QString  pr_code_l2;
    QString  rr_code_l2;
    QString  pr_code_l5;
    QString  rr_code_l5;
} GnssSatellite;

typedef struct
{
    QString utc;
    QString status;
    QString latitude;
    QString longitude;
    QString speed;
    QString bearing;
    QString magnetic;
    QString magnetic_dir;
    QString mode;
    QString quality;
    QString sats;
    QString dop;
    QString altitude;
    QString altitude_units;
    QString geo_sep;
    QString geo_sep_units;
    QString age;
    QString station;
    QString pdop;
    QString hdop;
    QString vdop;
    QString ttff;
    QString fix;
} GnssPosition;

typedef struct
{
    GnssPosition position;
    QVector<QHash<QString, QString>> satellites;
} KmlPoint;

typedef struct
{
    QVector<qint64> dataX;
    QVector<QString> dataY;
    bool isY2Axis;
    QString name;
} GnssPlotSeries;

typedef struct
{
    QString plotName;
    QString labelX;
    QString labelY;
    QString labelY2;
    QString type;
    QHash<QString,GnssPlotSeries> plotData;
} GnssPlotData;

typedef struct
{
    QString chip;
    QString gll;
    QString branch;
    QString minor;
    QString build;
} GnssGllVersion;

#endif //GNSSTYPE_H