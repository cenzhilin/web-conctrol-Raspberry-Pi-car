/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*      Arduino Software for the Control of the "ArduinoCar"          */
/*            (made for Arduino Yún)                    */
/*                                          */
/*      made by:    Wang, Weijian                      */
/*
/*                                          */
/*      last update:  Oktober 23, 2017                    */
/*                                          */
/*      features:                               */
/*        - WLAN Arduino Yun  (Name: 'root', PW: 'arduino')         */
/*          --> Standard-Adresse: http://192.168.240.1/         */
/*          --> Kommunikation:    192.168.240.1:255           */
/*        - Control ArduinoCar                        */
/*          --> Drive Forward:    'w' --> MOTOR_GO_FORWARD        */
/*          --> Turn Left:      'a' --> MOTOR_GO_LEFT         */
/*          --> Drive Backward:   's' --> MOTOR_GO_BACK         */
/*          --> Turn Right:     'd' --> MOTOR_GO_RIGHT          */
/*          --> Stop:       'x' --> MOTOR_GO_STOP         */
/*        - Control ArduinoCar Speed                     */
/*          --> Seekbar:   Maximal: 255     Minimal: 0                  */
/*        - No WiFi-Communication                       */
/*          --> MOTOR_GO_STOP                       */
/*        - AutomaticMode (TriggerMode)                   */
/*            --> MOTOR_GO_STOP                     */
/*            --> MOTOR_GO_BACK                     */
/*            --> MOTOR_GO_LEFT or MOTOR_GO_RIGHT             */
/*            --> MOTOR_GO_FORWARD                    */

/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

#include <Servo.h> 

Servo myservo_u;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
Servo myservo_d;  // create servo object to control a servo 

const int ENA = 5;    //L298 A -> Motor A aktivieren
const int ENB = 6;    //L298 B -> Motor B aktivieren
const int INPUT2 = 8;  //-> Motorschnittstellen 1-4
const int INPUT1 = 7;
const int INPUT3 = 12;
const int INPUT4 = 13;
int velocity = 210;    //Motorgeschwindigkeit


int TrigPinright = A0;
int EchoPinright = A1;
int TrigPinleft = A2;
int EchoPinleft = A3;
float distanceright;
float distanceleft;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


#define MOTOR_GO_BACK    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}
#define MOTOR_GO_FORWARD  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}
#define MOTOR_GO_LEFT   {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}
#define MOTOR_GO_RIGHT    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}
#define MOTOR_GO_STOP   {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,LOW);}

int value;
bool AutomaticMode = false;


void setup() {

    myservo_u.attach(9);  // attaches the servo on pin 9 to the servo object 
   myservo_d.attach(10);  // attaches the servo on pin 9 to the servo object 
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(INPUT3, OUTPUT);
  pinMode(INPUT4, OUTPUT);
  pinMode(TrigPinright, OUTPUT);
  pinMode(EchoPinright, INPUT);
  pinMode(TrigPinleft, OUTPUT);
  pinMode(EchoPinleft, INPUT);
}
void Sensor()
{
  digitalWrite(TrigPinright, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPinright, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPinright, LOW);
  distanceright = pulseIn(EchoPinright, HIGH) / 58.00;
  digitalWrite(TrigPinleft, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPinleft, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPinleft, LOW);
  distanceleft = pulseIn(EchoPinleft, HIGH) / 58.00;

//  Serial.print(distanceleft);
//  Serial.print("cm");
//  Serial.print(" ");
//  Serial.print(distanceright);
// Serial.print("cm");
//  Serial.println();


  delay(100);
}
void loop() {
    runProcess();
//    Serial.println("a/");
//  delay(50); // Poll every 50ms
}


void runProcess() {
//  String command;
  Sensor();
//  char cmd;
  analogWrite(ENA, velocity);
  analogWrite(ENB, velocity);

  serialEvent(); //call the function
  if (stringComplete) {
    
    // clear the string:
    stringComplete = false;
    
    if (inputString == "j") {
      if (!AutomaticMode) {
        MOTOR_GO_LEFT;
        //Serial.println("j");
       // Sensor();
      }
    }
    if (inputString == "l") {
      if (!AutomaticMode) {
        MOTOR_GO_RIGHT;
      //  Sensor();
      }
    }
    if (inputString == "i") {
      if (!AutomaticMode) {
        MOTOR_GO_FORWARD;
      //  Sensor();
      }
    }
    if (inputString == "k") {
      if (!AutomaticMode) {
        MOTOR_GO_BACK;
      //  Sensor();
      }
    }
    if (inputString == "p") {
      MOTOR_GO_STOP;
    }
    if (inputString == "1") {
      velocity = 70;
    }
    if (inputString == "2") { 
      velocity = 140;
    }
    if (inputString == "3") {
      velocity = 210;
    }
    if (inputString == "o") {
      AutomaticMode = true;
      MOTOR_GO_STOP;
    }
    if (inputString == "v") {
      AutomaticMode = false;
      MOTOR_GO_STOP;
    }
   if (inputString == "s") {
      myservo_u.write(90); 
   }
   if (inputString == "d") {
      myservo_d.write(0); 
   }
   if (inputString == "a") {
      myservo_d.write(90); 
   }
   if (inputString == "w") {
      myservo_u.write(0); 
   }
   inputString = "";
  }
  if (AutomaticMode == true)
  {
    Autonom();
  }
}




void Autonom()
{
  if (AutomaticMode == true) {

    if (distanceright <= 30 && distanceleft <= 30)
    {
      MOTOR_GO_STOP;

      delay(100);

      MOTOR_GO_BACK;
      delay(1000);
      MOTOR_GO_RIGHT;
      delay (800);
      MOTOR_GO_FORWARD;
    }
    if (distanceright <= 30 && distanceleft > 30)
    {
      MOTOR_GO_STOP;
      delay(100);
      MOTOR_GO_BACK;
      delay(1000);
      analogWrite(ENA, velocity);
      analogWrite(ENB, velocity);
      MOTOR_GO_LEFT;
      delay (400);
      MOTOR_GO_STOP;
      delay(100);
      MOTOR_GO_FORWARD;
    }
    else
    {
      if (distanceleft <= 30)
      {
        MOTOR_GO_STOP;
        delay(100);
        MOTOR_GO_BACK;
        delay(400);
        analogWrite(ENA, velocity);
        analogWrite(ENB, velocity);
        MOTOR_GO_RIGHT;
        delay (400);
        MOTOR_GO_STOP;
        delay(100);
        MOTOR_GO_FORWARD;
      }
      else
      {
        MOTOR_GO_FORWARD;
      }
    }
  }

}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
//    if (inChar == '\n') {
      stringComplete = true;
//    }
//    else
     inputString += inChar;
  }
}


