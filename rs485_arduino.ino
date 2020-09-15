#include <SoftwareSerial.h>
const int SSERIAL_RX_PIN = 10;  //Soft Serial Receive pin
const int SSERIAL_TX_PIN = 11;  //Soft Serial Transmit pin
const int SSERIAL_CTRL_PIN= 3;   //RS485 Direction control
const int LED_PIN = 13;
const int RS485_TRANSMIT = HIGH;
const int RS485_RECEIVE = LOW;

SoftwareSerial RS485Serial(SSERIAL_RX_PIN, SSERIAL_TX_PIN); // RX, TX

int byteReceived;

void setup()
{
  Serial.begin(9600);           // Start the built-in serial port
  Serial.println("Master Device");
  Serial.println("Type in upper window, press ENTER");
  
  pinMode(LED_PIN, OUTPUT);     // Configure any output pins
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
    RS485Serial.write(byteReceived);                 // Send byte to Remote Arduino
    delay(1);                                        // Wait before going back to Receive mode
    digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);   // Put RS485 back into Receive mode    
  }
  
  if (RS485Serial.available())            //Data from the Slave is available
   {
    digitalWrite(LED_PIN, HIGH);          // Show activity on LED
    byteReceived = RS485Serial.read();    // Read received byte
    Serial.write(byteReceived);           // Show on Serial Monitor
    delay(10);
    digitalWrite(LED_PIN, LOW);           // Turn LED back off
   }  
}
