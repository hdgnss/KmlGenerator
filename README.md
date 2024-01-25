# KmlGenerator

A tool to generate NMEA to KML file. 

## Usage
```
KmlGenerator [options]
GPS NMEA TO KML

Options:
  -h, --help                                                        Displays help on commandline options.
  --help-all                                                        Displays help including Qt specific options.
  -v, --version                                                     Displays version information.
  -i, --input <path>                                                Input GPS NMEA file
  -c, --color <red,green,blue,orange,yellow,indigo,magenta,white>   KML Color
  -o, --icon <default,arrow,arrow_speed>                            KML Icon
  -s, --stt                                                         Generate Kml for STT
  -f, --force                                                       Overwrite existing files
```

```
KmlGenerator -i './nmea/gps.nmea'
```

```
KmlGenerator -c green -i './nmea/gps.nmea'
```

```
KmlGenerator -c green -o arrow -i './nmea/gps.nmea'
```

```
KmlGenerator -c green -o arrow -s -i './nmea/gps.nmea'
```

```
KmlGenerator -c green -o arrow -f -i './nmea/gps.nmea'
```