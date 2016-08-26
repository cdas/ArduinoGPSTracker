#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial gpsShieldSerial(2,3);
String comdata = "";

void setup() {
  
  // Open serial connection from PC to Arduino
  Serial.begin(9600);
  
  // Open serial connection from PC?Arduino to GPSShield
  gpsShieldSerial.begin(9600);
  
  Serial.print("GPS Tracker by Daseking");
  Serial.print("TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  delay(500);
}

void loop() {
  delay(2000);
  gpsShieldSerial.println("AT");
  
  delay(2000);
  gpsShieldSerial.println("AT+CGNSPWR=1");
  
  delay(2000);
  gpsShieldSerial.println("AT+CGNSTST=1");
  
  delay(2000);
  gpsShieldSerial.listen();
  
  while(1) {
    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;
    
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (gpsShieldSerial.available() > 0) 
      {
        char c = gpsShieldSerial.read();
        // Serial.write(c); // Uncomment this to get raw gps data to PC's serial monitor
        if (gps.encode(c))
          newData = true;
      }
    }
    
    if (newData)
    {
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);
      Serial.print("LAT=");
      Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      Serial.print(" LON=");
      Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      Serial.print(" SAT=");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print(" PREC=");
      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    }
    
    gps.stats(&chars, &sentences, &failed);  
    Serial.print(" CHARS=");
    Serial.print(chars);
    Serial.print(" SENTENCES=");
    Serial.print(sentences);
    Serial.print(" CSUM ERR=");
    Serial.println(failed);
    if (chars == 0)
      Serial.println("** No characters received from GPS: check wiring **");

//    while (Serial.available())
//      gpsShieldSerial.write(Serial.read());
  }  
  
}
