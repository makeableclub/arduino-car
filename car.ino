#include <AFMotor.h>

#include <AFMotor.h>
#include <SoftwareSerial.h>

SoftwareSerial Genotronex(A5, A4);
char BluetoothData;
char content[10] = "";
char character;


AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR12_64KHZ);
AF_DCMotor motor4(4, MOTOR12_64KHZ);

#define trigPin1 9
#define echoPin1 10

bool d = false;  // debug flag
bool autoMode = false;

void setup() {
    Genotronex.begin(9600);
    if(d) Serial.begin(9600);

  // put your setup code here, to run once:
    motor1.setSpeed(200);
    motor2.setSpeed(200);
    motor3.setSpeed(200);
    motor4.setSpeed(200);
 

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

    autoMode = false;
}

void testOneByOne() {
  // put your main code here, to run repeatedly:
 
      motor1.run(FORWARD);
      delay(2000);
      motor1.run(RELEASE);

      motor2.run(FORWARD);
      delay(2000);
      motor2.run(RELEASE);
      
      motor3.run(FORWARD);
      delay(2000);
      motor3.run(RELEASE);
      
      motor4.run(FORWARD);
      delay(2000);
      motor4.run(RELEASE);
}

void loopBlue2() {
  int i = 0;
  char content[10] = "";
  if (Genotronex.available() > 0) 
  {       
    while(Genotronex.available() > 0 && i<10) {
      character = Genotronex.read();
      content[i] = character;
      i++;
    }
    content[i]='\0';
    if(d) Serial.println( content );
  }

  delay(100);
}

void loopBlue() {
  // put your main code here, to run repeatedly:
   if (Genotronex.available()) {    
      BluetoothData=Genotronex.read();

      if(d) Serial.println( BluetoothData);

      if(BluetoothData=='1'){   // if number 1 pressed ....
        motor1.run(FORWARD);
      }
      
      if (BluetoothData=='0'){// if number 0 pressed ....
        motor1.run(RELEASE);
      }
   }
   delay(100);
}

void loopTest(){    
    // testOneByOne();
    // testrun();
    if(UltrasonicDistance(trigPin1, echoPin1)<35){
      turnleft();
    }
    else{
      testrun();
    }

}

void loop(){
    // first detect any bluetooth input
    if(Genotronex.available() > 0) {
        BluetoothData = Genotronex.read();
        if(d) Serial.print( "bluetooth reading: " );
        if(d) Serial.println(BluetoothData);

        // clear the pipe
        while(Genotronex.available() > 0) {
          Genotronex.read();
        }
    }
    else {
      BluetoothData = ' ';
    }
    

    // switch to manual mode, click SELECT
    if( BluetoothData == '5' ) {
      autoMode = false;
      if(d) Serial.println("switched to manual");
    }

    // switch to the auto mode, click START
    if( BluetoothData == '6' ) {
      autoMode = true;
      if(d) Serial.println("switched to auto");
    }

    // click X to stop, and switch to manual mode
    if(BluetoothData == '9'){
        autoMode = false;
        testrelease();
        if(d) Serial.println("Stop it");                
    }

  
    // auto drive mode
    if(autoMode == true) {
        // detect obstacle
        if(UltrasonicDistance(trigPin1, echoPin1) < 35){
            if(d) Serial.println("obstacle is true");
            turnleft();
            if(d) Serial.println("turning to LEFT");      
        }
        else {
            if(d) Serial.println("obstacle is false");
            testrun();
            if(d) Serial.println("straight run");      
        }
    }
    // manual control mode
    else {
      
      if(BluetoothData == '1'){
          if(d) Serial.println("go forward");                
          testrun();
      }
      if(BluetoothData == '2'){
          if(d) Serial.println("go backward");                
          testbackward();
      }
      if(BluetoothData == '3'){
          if(d) Serial.println("go left");                
          turnleft();
      }
      if(BluetoothData == '4'){
          if(d) Serial.println("go right");                
          turnright();
      }
      /*
      if(BluetoothData == ' '){
          if(d) Serial.println("go nowhere");                
          testrelease();
      } 
      */     
    }

    delay(200);
}


void testrun(){
  Serial.println("Motors are Moving");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void testrelease(){
  Serial.println("Motors have Stopped");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void turnleft(){
  Serial.println("Motors are turning left");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnright(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void testbackward(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

long UltrasonicDistance(const int p1, const int p2)
{
  long duration, distance;
  digitalWrite(p1, LOW);  // Added this line, p1 is the TrigPin
  delayMicroseconds(2); // Added this line
  digitalWrite(p1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(p1, LOW);
  duration = pulseIn(p2, HIGH); //p2 is the echoPin
  distance = (duration / 2) * 0.034; // speed of sound is 0.034 cm/us

  Serial.print("distance: ");
  Serial.println( distance );
  return distance;
}
