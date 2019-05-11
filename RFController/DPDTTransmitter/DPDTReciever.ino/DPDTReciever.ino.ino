#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> 

const uint64_t pipeIn = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
RF24 radio(9, 10);  //CSN and CE pins

struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Received_data received_data;

int ch1_value = 0;
int ch2_value = 0;
int ch3_value = 0;
int ch4_value = 0;
int ch5_value = 0;
int ch6_value = 0;
int ch7_value = 0;

void reset_the_Data() 
{
  // 'safe' values to use when NO radio input is detected
  received_data.ch1 = 127;      //Throttle (channel 1) to 0
  received_data.ch2 = 127;
  received_data.ch3 = 127;
  received_data.ch4 = 127;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;
}

const int motorPin1  = 2;    // Pin 15 of L293D IC
const int motorPin2  = 3;    // Pin 10 of L293D IC
//Motor B
const int motorPin3  = 4;   // Pin  7 of L293D IC
const int motorPin4  = 5;   // Pin  2 of L293D IC

 
    
void setup() {
Serial.begin(9600);
    Serial.println("SimpleRx Starting");
  reset_the_Data();
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);

  //Once again, begin and radio configuration
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  //We start the radio comunication
  radio.startListening();

}
unsigned long lastRecvTime = 0;

//We create the function that will read the data each certain time
void receive_the_data()
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  lastRecvTime = millis(); //Here we receive the data
}
}
int verticalDirection=0;
int horizontalDirection=0;

int getSigma(int val, int limit){
  if(val>limit){
    return 1;
  }else if(val< 0-limit){
    return -1;
  }else{
    return 0;
  }
}

const bool forward[]={HIGH,LOW,HIGH,LOW};
const bool stopward[]={LOW,LOW,LOW,LOW};
const bool backward[]={LOW,HIGH,LOW,HIGH};
const bool forwardRight[]={HIGH,LOW,LOW,LOW};
const bool forwardLeft[]={LOW,LOW,HIGH,LOW};
const bool backwardRight[]={LOW,HIGH,LOW,LOW};
const bool backwardLeft[]={LOW,LOW,LOW,HIGH};
const bool rightRotate[]={HIGH,LOW,LOW,HIGH};
const bool leftRotate[]={LOW,HIGH,HIGH,LOW};


void setMotor(bool motDirect[]){
  
    digitalWrite(motorPin1, motDirect[0]);
    digitalWrite(motorPin2, motDirect[1]);
    digitalWrite(motorPin3, motDirect[2]);
    digitalWrite(motorPin4, motDirect[3]);  
}

void driveMotorDirections(){
  if((verticalDirection==0)and (horizontalDirection==0)){
    Serial.print("Stop");
    setMotor(stopward);
  }else if((verticalDirection==-1)and (horizontalDirection==0)){
    setMotor(forward);
    Serial.print("forward");
  }else if((verticalDirection==1)and (horizontalDirection==0)){
    setMotor(backward);
    Serial.print("backward");
  }else if((verticalDirection==-1)and (horizontalDirection==-1)){
    setMotor(forwardRight);
    Serial.print("forwardRight");
  }else if((verticalDirection==-1)and (horizontalDirection==1)){
    setMotor(forwardLeft);
    Serial.print("forwardLeft");
  }else if((verticalDirection==0)and (horizontalDirection==-1)){
    setMotor(rightRotate);
    Serial.print("rightRotate");
  }else if((verticalDirection==0)and (horizontalDirection==1)){
    setMotor(leftRotate);
    Serial.print("leftRotate");
  }else if((verticalDirection==1)and (horizontalDirection==-1)){
    setMotor(backwardRight);
    Serial.print("backwardRight");
  }else if((verticalDirection==1)and (horizontalDirection==1)){
    setMotor(backwardLeft);
    Serial.print("backwardLeft");
  }else {
    setMotor(stopward);
    Serial.print("Stop");
  }
}

void loop() {
  receive_the_data();
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    // signal lost?
    reset_the_Data();
    //Go up and change the initial values if you want depending on
    //your aplications. Put 0 for throttle in case of drones so it won't
    //fly away
  } 
  ch1_value = map(received_data.ch1,0,255,-100,100);
  ch2_value = map(received_data.ch2,0,255,-100,100);
  ch3_value = map(received_data.ch3,0,255,1000,2000);
  ch4_value = map(received_data.ch4,0,255,1000,2000);
  ch5_value = map(received_data.ch5,0,1,1000,2000);
  ch6_value = map(received_data.ch6,0,1,1000,2000);
  ch7_value = map(received_data.ch7,0,255,1000,2000);

  verticalDirection=getSigma(ch1_value,60);
  horizontalDirection=getSigma(ch2_value,60);
  driveMotorDirections();

  
    Serial.print(ch1_value);
    Serial.print(" ");
    Serial.print(ch2_value);
    Serial.print(" ");
    Serial.print(verticalDirection);
    Serial.print(" ");
    Serial.println(horizontalDirection);
  
}
