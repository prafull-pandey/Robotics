
// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10

const uint64_t myRadioPipe = 0xE8E8F0F0E1LL;


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

struct DataToSend{
    byte ch1;
    byte ch2;
    byte ch3;
    byte ch4;
    byte ch5;
    byte ch6;
    byte ch7;
};

DataToSend sentData;

void setup() {

    Serial.begin(9600);
    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate( RF24_250KBPS );
    radio.openWritingPipe(myRadioPipe);

    //reset data
    sentData.ch1=127;
    sentData.ch2=127;
    sentData.ch3=127;
    sentData.ch4=127;
    sentData.ch5=0;
    sentData.ch6=0;
    sentData.ch7=0;

}

//====================

void loop() {
    sentData.ch1=map(analogRead(A0),0,1024,0,255);
    sentData.ch2=map(analogRead(A1),0,1024,0,255);
    sentData.ch3=map(analogRead(A2),0,1024,0,255);
    sentData.ch4=map(analogRead(A3),0,1024,0,255);
    sentData.ch5=digitalRead(2);
    sentData.ch6=digitalRead(3);
    sentData.ch7=map(analogRead(A4),0,1024,0,255);

    radio.write(&sentData, sizeof(DataToSend));
    Serial.print(sentData.ch1);
    Serial.print(" ");
    Serial.print(sentData.ch2);
    Serial.print(" ");
    Serial.print(sentData.ch3);
    Serial.print(" ");
    Serial.println(sentData.ch4);
}

//====================
