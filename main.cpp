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
#include "gnsstype.h"
#include "nmeabase.h"
#include "kmlgenerator.h"

#include <QDir>
#include <QList>
#include <QFileInfo>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>


int main(int argc, char *argv[])
{
    QCommandLineParser mCommandParser;

    QString mInput;
    QString mColor;
    QString mIcon;
    bool mSttEnable;
    bool mForce;
    bool mOverwrite = false;

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VER);

    mCommandParser.setApplicationDescription("GPS NMEA TO KML");
    mCommandParser.addHelpOption();
    mCommandParser.addVersionOption();

    QCommandLineOption mInputOption(QStringList() << "i" << "input",
                                    QCoreApplication::translate("main", "Input GPS NMEA file"),
                                    QCoreApplication::translate("main", "path"));
    mCommandParser.addOption(mInputOption);

    QCommandLineOption mColorOption(QStringList() << "c" << "color",
                                    QCoreApplication::translate("main", "KML Color"),
                                    QCoreApplication::translate("main", "red,green,blue,orange,yellow,indigo,magenta,white"),
                                    "red");
    mCommandParser.addOption(mColorOption);

    QCommandLineOption mIconOption(QStringList() << "o" << "icon",
                                    QCoreApplication::translate("main", "KML Icon"),
                                    QCoreApplication::translate("main", "default,arrow,arrow_speed"),
                                    "default");
    mCommandParser.addOption(mIconOption);

    QCommandLineOption mSttOption(QStringList() << "s" << "stt",
                                    QCoreApplication::translate("main", "Generate Kml for STT"));
    mCommandParser.addOption(mSttOption);

    QCommandLineOption mForceOption(QStringList() << "f" << "force",
                                    QCoreApplication::translate("main", "Overwrite existing files"));
    mCommandParser.addOption(mForceOption);

    mCommandParser.process(app);

    if (mCommandParser.isSet(mInputOption)){
        mInput = mCommandParser.value(mInputOption);
    }
    else{
        mCommandParser.showHelp(-1);
        return -1;
    }
    
    mColor = mCommandParser.value(mColorOption);
    mIcon  = mCommandParser.value(mIconOption);
    mSttEnable = mCommandParser.isSet(mSttOption);

    mForce = mCommandParser.isSet(mForceOption);

    KmlGenerator *mGenerate = new KmlGenerator();

    QFileInfo mLogFileInfo(mInput);
    QDir mLogDirectory(mLogFileInfo.absolutePath());
    QString mOutput = mLogDirectory.absoluteFilePath(mLogFileInfo.baseName() + ".kml");

    QString mDataFile = mLogDirectory.absoluteFilePath(mLogFileInfo.baseName() + ".nm");

    qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

    NmeaBase *nmeabase = new NmeaBase();
    if(!nmeabase->setBase(mDataFile)){
        qDebug() << "Data file can not open\n";
        return -2;
    }


    QFileInfo fileInfo(mDataFile);

    if (fileInfo.exists()) {
        qint64 fileSize = fileInfo.size();
        if(fileSize<135168 || mForce){
            nmeabase->initBase(mInput);
        }
    } 

    if (mLogFileInfo.isFile()) {
        QVector<KmlPoint> points;
        nmeabase->getKmlPoints(&points, mSttEnable);
        mGenerate->onGenerateKml(mOutput, points, mColor, mIcon);
    }
    
    nmeabase->closeBase();
    qDebug() << "dT:" << (QDateTime::currentDateTime().toMSecsSinceEpoch()-timestamp);

    return 0;
}

