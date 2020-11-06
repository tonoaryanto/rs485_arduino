#include <SoftwareSerial.h>
const int SSERIAL_RX_PIN = 12;  //Soft Serial Receive pin
const int SSERIAL_TX_PIN = 13;  //Soft Serial Transmit pin
const int SSERIAL_CTRL_PIN= 3;   //RS485 Direction control
const int RS485_TRANSMIT = HIGH;
const int RS485_RECEIVE = LOW;

SoftwareSerial RS485Serial(SSERIAL_RX_PIN, SSERIAL_TX_PIN); // RX, TX

int byteReceived;

void setup()
{
  Serial.begin(9600);           // Start the built-in serial port  
  pinMode(SSERIAL_CTRL_PIN, OUTPUT);    
  
  digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);  // Put RS485 in receive mode  
  
  RS485Serial.begin(9600);   // Start the RS485 soft serial port 
}

void loop() 
{
  if (Serial.available())         // A char(byte) has been entered in the Serial Monitor
  {
    byteReceived = Serial.read();                   // Read the byte
    digitalWrite(SSERIAL_CTRL_PIN, RS485_TRANSMIT);  // Put RS485 in Transmit mode   

//    byte message[] = {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x38, 0x30, 0x30, 0x20, 0x32, 0x34, 0x38, 0x0d };
//    RS485Serial.write(message, sizeof(message));    

    RS485Serial.write(byteReceived);                 // Send byte to Remote Arduino
    //    delay(1);                                        // Wait before going back to Receive mode
    digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);   // Put RS485 back into Receive mode    
  }
  
  if (RS485Serial.available())            //Data from the Slave is available
   {
    byteReceived = RS485Serial.read();    // Read received byte
    Serial.write(byteReceived);           // Show on Serial Monitor
    //    delay(10);
   }  
}
