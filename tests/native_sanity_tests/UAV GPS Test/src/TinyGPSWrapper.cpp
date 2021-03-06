#include <Arduino.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "TinyGPSWrapper.h"

#define _GPS_NO_STATS

// #define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// #define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// #define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// #define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// #define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
// #define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"

TinyGPS gps;
SoftwareSerial ss(8, 7); //Hardcode RX TX pins

extern "C" void GPSInit(){
    ss.begin(9600);
//     ss.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
//     ss.println(PMTK_SET_NMEA_UPDATE_1HZ);
//     ss.println("$PMTK386,0.4*39"); //Set 0.4m/s nav ignore threshold
}

extern "C" int GPSRead(float* values){
    do{
        char c = ss.read();
        if(gps.encode(c)){
//             //Valid NMEA sentence found
//             values[0] = (float) gps.location.lat();
//             values[1] = (float) gps.location.lng();
//             values[2] = (float) gps.speed.mps();
//             values[3] = (float) gps.course.deg();
//             values[4] = (float) gps.altitude.meters();
//             values[5] = (float) gps.satellites.value();
//             values[6] = (float) gps.location.age();
//             return gps.location.isValid() ? 1 : 0;
            long lat, lon;
            unsigned long fix_age, time, date, speed, course;
            // retrieves +/- lat/long in 100000ths of a degree
            gps.get_position(&lat, &lon, &fix_age);
            // time in hhmmsscc, date in ddmmyy
            gps.get_datetime(&date, &time, &fix_age);
            // returns speed in 100ths of a knot
            speed = gps.speed();
            // course in 100ths of a degree
            course = gps.course();
            values[0] = lat/100000.0;
            values[1] = lon/100000.0;
            values[2] = speed/100.0;
            values[3] = course/100.0;
        }
    } while(ss.available());
    return 0;
}
