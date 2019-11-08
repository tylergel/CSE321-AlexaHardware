#include <Servo.h>
#include <dht.h>
#include <Ethernet.h>
#include <HttpClient.h>
Servo myservo;
dht DHT;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 unsigned long byteCount = 0;
char server[] = "www.webshockinnovations.com";    
  
EthernetClient client;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(A1,INPUT);
  myservo.attach(9);
  myservo.write(90);

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
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
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  
  delay(5000);
 
}
 
void loop() {


  int water_level = getWaterLevel();
  Serial.print("Water Level");
  Serial.println(water_level);
  
  int voltage_level = getVoltageLevel();  
  Serial.print("Voltage Level");    
  Serial.println(voltage_level); 
 
  int temp = getTemperature();
  Serial.print("Temperature");
  Serial.println(temp, 1);
  
  moveMotor();
  delay(5000);

   insertStuff(temp, water_level, voltage_level);
}

int getVoltageLevel() {
  return analogRead(A1);
}
int getWaterLevel() {
  return analogRead(0);
}
void moveMotor() {
  
  myservo.write(30);// move servos to center position -> 120°
  delay(500);

  myservo.write(120);// move servos to center position -> 120°
  delay(500);
}
  
int getTemperature() {
   int chk = DHT.read11(7);
   return DHT.temperature;
}

void insertStuff(int temp, int level,int quality)   //CONNECTING to API
{

  String temp1 = String(temp);
  String quality1=String(quality);
  String level1=String(level);
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /cse321/api.php?temperature="+temp1+"&level="+level1+"&quality="+quality1+" HTTP/1.1");
    client.println("Host: www.webshockinnovations.com");
    client.println();
    

    // note the time that the connection was made:
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
 int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    if (1) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }
}
