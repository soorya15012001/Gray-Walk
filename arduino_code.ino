#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
int X,Y,Z;                                         
int OX,OY,OZ;                                      

int VRx = A0;
int VRy = A1;
int SW = 5;

int hall_vcc = 3;
int hall_gnd = 2;
int hall_pin = 4;

int b1 = 12;
int g_b1 = 13;

int b2 = 10;
int g_b2 = 11;

int b3 = 8;
int g_b3 = 9;





int joy_gnd = 6;
int joy_vcc = 7;


int sda = A4;
int scl = A5;



void setup() {  
  Serial.begin(9600);
  
  Wire.begin();                                               // Initialise I2C communication
  mpu6050.begin();                                            // Initialise Gyro communication
  Serial.println("STRTM");                                    // Identifier "STARTM" for start mouse
  mpu6050.calcGyroOffsets(true);                              // Setting Gyro offsets
  mpu6050.update();                                           // Command to calculate the sensor data before using the get command
  OX = mpu6050.getAngleX();                                   // Getting angle X Y Z
  OY = mpu6050.getAngleY();
  OZ = mpu6050.getAngleZ();
  
  if(OX < 0){                                                 // Inverting the sign of all the three offset values for zero error correction
    OX = OX *(-1);}
  else{
    OX = (OX-OX)-OX;}

  if(OY < 0){
    OY = (OY *(-1));}
  else{
    OY = ((OY-OY)-OY)+10;}

  if(OZ < 0){
    OZ = OZ *(-1);}
  else{
    OZ = (OZ-OZ)-OZ;}

  
  
  pinMode(hall_pin, INPUT);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);   

  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);

  
  pinMode(joy_gnd, OUTPUT);
  digitalWrite(joy_gnd,LOW);

  pinMode(g_b2,OUTPUT);
  digitalWrite(g_b2,LOW);

  pinMode(g_b1,OUTPUT);
  digitalWrite(g_b1,LOW);

  pinMode(g_b3,OUTPUT);
  digitalWrite(g_b3,LOW);

  pinMode(joy_vcc,OUTPUT);
  digitalWrite(joy_vcc,HIGH);

  pinMode(hall_vcc, OUTPUT);
  digitalWrite(hall_vcc,HIGH);

  pinMode(hall_gnd, OUTPUT);
  digitalWrite(hall_gnd,LOW);

  

}


int prev_state=0;   // previous state of switch
int prev_b1=0;
int prev_b2=0;
int prev_b3=0;
int prev_b4=0;


float startTime;
float endTime;
float duration = 0;
float old = 0;
byte timerRunning;
long c = 0;

void loop() { 

  mpu6050.update();



  int but1=0, but2=0, but3=0;
  
  int z=0;
  String xpos,ypos;
  int sensitivity=5;
  
  int x = analogRead(VRx);
  int y = analogRead(VRy);

//  Serial.print(x);
//  Serial.print(":");
//  Serial.println(y);
  
  if(x>504)                     // when moved up 
  ypos="up"; 
  if(x<498)                   // when moved down
  ypos="down";   
  if(y>520)                    // when moved right
  xpos="right";  
  if(y<514)                  // when moved left
  xpos="left"; 
  
  int curr_state=digitalRead(SW); 
  if(curr_state==1 && prev_state==0)   // when SW is pressed 
    z=1;
  else
    z=0;
  prev_state=curr_state;

  // Buttons

  if(digitalRead(b2)==1 && prev_b2==0)   // when SW is pressed 
    but2=1;
  else
    but2=0;  
  prev_b2=digitalRead(b2);

  if(digitalRead(b3)==1 && prev_b3==0)   // when SW is pressed 
    but3=1;
  else
    but3=0;  
  prev_b3=digitalRead(b3);

//  if(digitalRead(b4)==1 && prev_b4==0)   // when SW is pressed 
//    but4=1;
//  else
//    but4=0;  
//  prev_b4=digitalRead(b4);      



  if (timerRunning == 0 && digitalRead(hall_pin) == LOW){ // button pressed & timer not running already
    startTime = millis();
    timerRunning = timerRunning + 1;
  }
  if (timerRunning == 1 && digitalRead(hall_pin) == HIGH){ // timer running, button released
    timerRunning = timerRunning + 1;
  }
  if (timerRunning == 2 && digitalRead(hall_pin) == LOW){
    timerRunning = timerRunning + 1;
  }
  if (timerRunning == 3 && digitalRead(hall_pin) == HIGH){
    endTime = millis();
    timerRunning = 0;
    duration = (endTime - startTime)/1000;
    c = 0;
  }

  if(old == duration){
    c = c+1;
  }

  if(c == 115){
    duration = 0;
    c = 0;
  }

  if(prev_b1 == 0 && digitalRead(b1) == 1){
    
    OX = mpu6050.getAngleX();                                   // Getting angle X Y Z
    OY = mpu6050.getAngleY();
    OZ = mpu6050.getAngleZ();
    
    if(OX < 0){                                                 // Inverting the sign of all the three offset values for zero error correction
      OX = OX *(-1);}
    else{
      OX = (OX-OX)-OX;}
  
    if(OY < 0){
      OY = (OY *(-1));}
    else{
      OY = (OY-OY)-OY;}
  
    if(OZ < 0){
      OZ = OZ *(-1);}
    else{
      OZ = (OZ-OZ)-OZ;}
    
    Serial.print(OX);
    Serial.print(":");    
    Serial.print(OY);
    Serial.print(":");    
    Serial.println(OZ);
  }

  prev_b1=digitalRead(b1);

  X = OX + mpu6050.getAngleX();   
  Y = OY + mpu6050.getAngleY();
  Z = OZ + mpu6050.getAngleZ();
  
  Serial.print("data");
  Serial.print(":");
  Serial.print(duration);
  Serial.print(":");
  Serial.print(xpos);
  Serial.print(":");
  Serial.print(ypos);
  Serial.print(":");
  Serial.print(z);  
  Serial.print(":");
  Serial.print(digitalRead(b1));    
  Serial.print(":");
  Serial.print(but2);    
  Serial.print(":");
  Serial.print(but3);    
//  Serial.print(":");
//  Serial.print(but4);    
  Serial.print(":");
  Serial.print(X);    
  Serial.print(":");
  Serial.print(Y);    
  Serial.print(":");
  Serial.println(Z);          
  
  old =  duration;

  

  
}
