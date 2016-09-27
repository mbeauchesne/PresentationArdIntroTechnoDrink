#include <ArduinoJson.h>
#include <TimeLib.h>

// these constants won't change:
const int micSensor = A0;
const int lumSensor = A2;
const int tempSensor = A4;
const int maxthreshold = 550;  // threshold value to decide when the detected sound is a knock or not
const int minthreshold = 450;


const char host[] = "http://192.168.5.111"; 
const int port = 9200;

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


// these variables will change:
float micVolt = 0;     
float micReading = 0;
float tempVolt = 0;    
float tempReading = 0;
float lumVolt = 0;
float lumReading = 0;

void setup() {
  Serial.begin(115200);       // use the serial port
  //Serial.begin(9600); 
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }

    processSyncMessage();

    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(2,INPUT);

    randomSeed(analogRead(0));
}

time_t t;
unsigned long time;
String json;

int isPushButton = 0;
int isOn = 0;
long randNumber1;
long randNumber2;
long randNumber3;

int isSync = 0;
float lastValueVoltMic = 0;
float lastValueVoltMic2 = 0;

void loop() {
  
  t = now();
  
  // read the sensor and store it in the variable sensorReading:
  tempVolt = (analogRead(tempSensor)/1024.0)*5.0;
  micVolt = analogRead(micSensor);
  lumVolt = analogRead(lumSensor);

  micReading = ((((micVolt >= 512) ? micVolt - 512 : 512 - micVolt ) + lastValueVoltMic + lastValueVoltMic2)/3)/8;
  lastValueVoltMic2 = lastValueVoltMic;
  lastValueVoltMic = micReading;

  tempReading = (tempVolt - .5) *100;

  lumReading = (lumVolt)/50;
  
  isPushButton = digitalRead(2);
 

 
  String url = "/technodrink/typename";

  if (isSync == 0) {
    processSyncMessage();
  } else {
    digitalWrite(5, 255);
    if(isPushButton && isOn == 0) {
      isOn = 1;
      delay(1000);
    } else if (isPushButton && isOn == 1) {
      isOn = 0;
      delay(1000);
    }
    

    /*
    if(isOn) {
      
      randNumber1 = random(2)*255;
      randNumber2 = random(2)*255;
  
      digitalWrite(3, randNumber1);
      digitalWrite(4, randNumber2);
      
    } else {
      digitalWrite(3, 0);
      digitalWrite(4, 0);

    }
    */
    // if the sensor reading is greater than the threshold:
    if(true){
   // if (micReading >= maxthreshold || micReading <= minthreshold) {
        json="{";
        json +="\"led\":\"" ;
        json += isOn*10;
        json += "\",\"sound\":\"";
        json += micReading;
        json += "\",";
        json += "\"temperature\":\"";
        json += tempReading;
        json += "\",";
        json += "\"lumens\":\"";
        json += lumReading;
        json += "\",";
        json += "\"timestamp\":\"";
        json += t;
        json += "\",";
        json += "\"date\":\"";
        json += addZero(year(t));
        json += "-";
        json += addZero(month(t));
        json += "-"; 
        json += addZero(day(t));
        json += "T"; 
        json += addZero(hour(t));
        json += ":";
        json += addZero(minute(t));
        json += ":"; 
        json += addZero(second(t));
        json += "-0400";
        json += "\"";
        json +="}";

        
        Serial.println(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Content-Length: " + json.length() + "\r\n" +
                 "\r\n" + json);
    }
  }
  
  //Serial.println(sensorReading);
  if(isOn) {
    digitalWrite(4, 255);
    delay(490); // + normal delay = 2 req/sec
  } else {
    digitalWrite(4, 0);
  }
  delay(10);  // delay to avoid overloading the serial port buffer 100 req/sec
}

String addZero(int digits){

  String digitsWithZero = String(digits);

   if(digits < 10) {
      digitsWithZero = "0"+digitsWithZero;
   }

   return digitsWithZero;
}

void processSyncMessage() {
  digitalWrite(3, 255);
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    //Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
      isSync =1;
      digitalWrite(3, 0);
      digitalWrite(4, 255);
    }  
  }
  
}


