#include <SoftwareSerial.h>
#include <DS3231.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192,168,1,200);  // numeric IP for Google (no DNS)

IPAddress ip(192, 168, 1, 90);
IPAddress myDns(192, 168, 1, 1);

EthernetClient client;

const int SSERIAL_RX_PIN = 12;  //Soft Serial Receive pin
const int SSERIAL_TX_PIN = 13;  //Soft Serial Transmit pin
const int SSERIAL_CTRL_PIN= 3;  //RS485 Direction control
const int LED_PIN = 13;
const int RS485_TRANSMIT = HIGH;
const int RS485_RECEIVE = LOW;
int LEDmerah1 = 5, LEDmerah2 = 6, LEDmerah3 = 4; //Set Pin LED
SoftwareSerial RS485Serial(SSERIAL_RX_PIN, SSERIAL_TX_PIN); // RX, TX
DS3231  rtc(SDA, SCL); // inisialisasi penggunaan i2c

int byteReceived;
String loopdt = "";
int i = 0;
int J = 0;
int star = 0;
int akhir = 0;
byte message[22][18] = {
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x34, 0x30, 0x39, 0x36, 0x20, 0x35, 0x31, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x32, 0x31, 0x38, 0x20, 0x35, 0x30, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x31, 0x39, 0x37, 0x20, 0x35, 0x36, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x31, 0x39, 0x38, 0x20, 0x35, 0x37, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x31, 0x39, 0x39, 0x20, 0x35, 0x38, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x32, 0x30, 0x30, 0x20, 0x34, 0x31, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x37, 0x32, 0x30, 0x33, 0x20, 0x34, 0x34, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x31, 0x34, 0x32, 0x20, 0x34, 0x32, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x31, 0x39, 0x30, 0x20, 0x34, 0x35, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x38, 0x30, 0x30, 0x20, 0x32, 0x34, 0x38, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x32, 0x35, 0x39, 0x20, 0x35, 0x31, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x36, 0x34, 0x37, 0x36, 0x30, 0x20, 0x31, 0x30, 0x33, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x36, 0x34, 0x37, 0x36, 0x33, 0x20, 0x31, 0x30, 0x36, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x35, 0x39, 0x36, 0x37, 0x20, 0x31, 0x31, 0x30, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x35, 0x39, 0x36, 0x39, 0x20, 0x31, 0x31, 0x32, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x31, 0x35, 0x34, 0x20, 0x34, 0x35, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x31, 0x37, 0x30, 0x20, 0x34, 0x33, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x33, 0x37, 0x30, 0x38, 0x20, 0x35, 0x30, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x31, 0x33, 0x30, 0x32, 0x20, 0x33, 0x38, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x31, 0x33, 0x30, 0x31, 0x20, 0x33, 0x37, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x36, 0x32, 0x30, 0x30, 0x31, 0x20, 0x38, 0x39, 0x0d },
    {0xff, 0xff, 0x25, 0x54, 0x39, 0x30, 0x31, 0x52, 0x36, 0x32, 0x30, 0x30, 0x32, 0x20, 0x39, 0x30, 0x0d }
};

Time t;
char changet;
String command;
int timeAuto = 1;
String wdata;

void setup(){
  Serial.begin(9600);
  RS485Serial.begin(9600);

  rtc.begin();
  t = rtc.getTime();
  changet = t.sec;
 pinMode(LEDmerah1, OUTPUT); //Set LEDmerah sebagai output
 pinMode(LEDmerah2, OUTPUT); //Set LEDhijau sebagai output
 pinMode(LEDmerah3, OUTPUT); //Set LEDhijau sebagai output
//Setup Ethernet
              digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
              delay(500);

            if (Ethernet.begin(mac) == 0) {
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 mati
              Serial.println("Failed to configure Ethernet using DHCP");
              // Check for Ethernet hardware present
              if (Ethernet.hardwareStatus() == EthernetNoHardware) {
                Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
                while (true) {
                  delay(1); // do nothing, no point running without Ethernet hardware
                }
              }
              if (Ethernet.linkStatus() == LinkOFF) {
                Serial.println("Ethernet cable is not connected.");
              }
              // try to congifure using IP address instead of DHCP:
              Ethernet.begin(mac, ip, myDns);
              Serial.print("My IP address: ");
              Serial.println(Ethernet.localIP());
            } else {
              Serial.print("  DHCP assigned IP ");
              Serial.println(Ethernet.localIP());
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah1, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 mati
              delay(500);
            }
            delay(2000);
            digitalWrite(LEDmerah1, LOW); //LEDmerah1 mati
            setnettime();
            digitalWrite(LEDmerah2, LOW); //LEDmerah1 mati
//EndSetup Ethernet
}

void loop(){
  if (Serial.available()){
    char c = Serial.read();
    
    if(c == '\n'){
      parseCommand(command);
    }else{
      command += c;
    }
  }

  timePlay();
  getDataCom();
}

void parseCommand(String com){
  String part1;
  String part2;
  String part3;
  String part4;

  part1 = com.substring(0, com.indexOf("_"));

  if(part1.equalsIgnoreCase("#st")){
    part2 = com.substring((com.indexOf("_") + 1),(com.indexOf("_") + 3));
    part3 = com.substring((com.indexOf("_") + 3));

    int jam = part2.toInt();
    int menit = part3.toInt();
    int detik = 1;
    rtc.setTime(jam, menit, detik);
  }else if(part1.equalsIgnoreCase("#com")){
    part2 = com.substring(com.indexOf("_") + 1);
    
    int setdcom = part2.toInt();
    if(setdcom == 1){
      star = setdcom;
      i = 0;
      loopdt = "";
      Serial.println("Kirim perintah");
    }else if(setdcom == 0){
      star = setdcom;
    }else{
      Serial.println("Nilai salah");    
    }
  }else if(part1.equalsIgnoreCase("#gt")){
    part2 = com.substring(com.indexOf("_") + 1);

    int setdtime = part2.toInt();
    if(setdtime != 0 && setdtime != 1){
      Serial.println("Nilai salah");
    }else{
      timeAuto = setdtime;
    }
  }else{
    Serial.println("Perintah tidak dikenal");
  }
  command = "";
}

void timePlay(){
  if(timeAuto == 1){
    t = rtc.getTime();
    if(changet != t.sec){
      if(t.sec == 0){
        star = 1;
        i = 0;
        loopdt = "";
        pinMode(LED_PIN, OUTPUT);
        pinMode(SSERIAL_CTRL_PIN, OUTPUT);      
        digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);
        Serial.println("Kirim Perintah dimulai");
        Serial.print("Waktu = ");
        Serial.print(t.hour, DEC);
        Serial.print(" : ");
        Serial.print(t.min, DEC);
        Serial.print(" : ");
        Serial.println(t.sec, DEC);
        digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
      }
      changet = t.sec;
    }
  }
}

void getDataCom(){
    if (star == 1){
      star = 0;
      digitalWrite(SSERIAL_CTRL_PIN, RS485_TRANSMIT);  
      RS485Serial.write(message[i], sizeof(message[i]));    
      i = i + 1;
      digitalWrite(SSERIAL_CTRL_PIN, RS485_RECEIVE);   
      digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
    }
  
    if (RS485Serial.available()){
      digitalWrite(LEDmerah3, HIGH); //LEDmerah1 mati
      digitalWrite(LED_PIN, HIGH);
      String datahex = String(RS485Serial.read(),HEX);
      if(datahex != "ff" && datahex != "96" && datahex != "98" && datahex != "84" ){
        loopdt = loopdt + datahex;
        }
      if(datahex == "84"){
        akhir = 1;
        }
      digitalWrite(LED_PIN, LOW);  
    }else{
      if(akhir == 1){
        if(loopdt != ""){
          akhir = 0;
          if( i < 22){
            star = 1;
            loopdt = loopdt + "x";
            delay(50);
          }else{
            Serial.print("Data Controller : ");
            Serial.println(loopdt);
            Serial.println("Kirim perintah Selesai");
            delay(500);
            Serial.println("Data dikirim ke WebServer");
            
//Run Ethernet
            // if there's a successful connection:
            if (client.connect(server, 80)) {
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 Nyala
              Serial.print("Connect to ");
              Serial.println(client.remoteIP());
              // Make a HTTP request:
              client.print("GET /apidboansell/rcdata/dhouse?69646661726d=1&6964686f757365=1");
              client.print("&a64617461=");
              client.print(loopdt);
              client.println(" HTTP/1.1");
              client.println("Host: 192.168.1.200");
              client.println("Connection: close");
              client.println();
            } else {
              // if you couldn't make a connection:
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah1, LOW); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah1, HIGH); //LEDmerah1 mati
              delay(500);
              digitalWrite(LEDmerah1, LOW); //LEDmerah1 mati
              Serial.println("connection failed");
            }
//End Run Ethernet
          
            delay(1000);
            while(client.available()){
            // read an incoming byte from the server and print them to serial monitor:
              char c = client.read();
              wdata = wdata + String(c);
            }

            // if the server's disconnected, stop the client:
            if (!client.connected()) {
              String wpart = wdata.substring((wdata.indexOf("'")));
              Serial.println(wpart);
              if(wpart == "'OK'"){
                digitalWrite(LEDmerah2, HIGH); //LEDmerah2 Nyala
                Serial.println("Data Terkirim");
              }else{
                Serial.println("Data Tidak Terkirim");
                digitalWrite(LEDmerah2, LOW); //LEDmerah2 Mati
              }
              wdata = "";
              Serial.println("Memutuskan koneksi ke Webserver");
              client.stop();
              delay(1000);
              digitalWrite(LEDmerah1, LOW); //LEDmerah2 Mati
              digitalWrite(LEDmerah2, LOW); //LEDmerah2 Mati
              digitalWrite(LEDmerah3, LOW); //LEDmerah1 mati
            }
            Serial.println(".");
          }
        }      
      }
    }
}

void setnettime(){
            char server2[] = "apidbo.anselljaya.com";
            if (client.connect(server2, 80)) {
              digitalWrite(LEDmerah2, HIGH); //LEDmerah2 Nyala
              Serial.print("Menghubungkan ke ");
              Serial.println(client.remoteIP());
              // Make a HTTP request:
              client.println("GET /rcdata/nettime HTTP/1.1");
              client.println("Host: apidbo.anselljaya.com");
              client.println("Connection: close");
              client.println();
            } else {
              // if you couldn't make a connection:
              digitalWrite(LEDmerah2, LOW); //LEDmerah1 mati
              Serial.println("Memutuskan hubungan");
            }
            //End Run Ethernet
          
            delay(1000);
            while(client.available()){
            // read an incoming byte from the server and print them to serial monitor:
              char c = client.read();
              wdata = wdata + String(c);
            }

            // if the server's disconnected, stop the client:
            if (!client.connected()) {
              String wpart = wdata.substring((wdata.indexOf("'")));
              String part2 = wpart.substring(1,3);
              String part3 = wpart.substring(3,5);
              int jam = part2.toInt();
              int menit = part3.toInt();
              int detik = 1;
              if(wdata != ""){
                rtc.setTime(jam, menit, detik);
              }
              wdata = "";
              Serial.println("Memutuskan koneksi ke Webserver");
              client.stop();
              Time ts = rtc.getTime();
              Serial.print("Waktu = ");
              Serial.print(ts.hour, DEC);
              Serial.print(" : ");
              Serial.print(ts.min, DEC);
              Serial.print(" : ");
              Serial.println(ts.sec, DEC);
              digitalWrite(LEDmerah2, LOW); //LEDmerah2 Nyala
              delay(500);
              digitalWrite(LEDmerah2, HIGH); //LEDmerah2 Nyala
              delay(500);
              digitalWrite(LEDmerah2, LOW); //LEDmerah2 Nyala
              delay(500);
              digitalWrite(LEDmerah2, HIGH); //LEDmerah2 Nyala
              delay(500);
            }
}
