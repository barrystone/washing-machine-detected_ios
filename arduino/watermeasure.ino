/*
YF‐ S201 Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
Adaptation Courtesy: www.hobbytronics.co.uk
*/
#include <WiFiNINA.h>

int i = 0;
int isWashing = 0;
int status=WL_IDLE_STATUS;
char ssid[]="QR";
char pass[]="11111111";
char server[]="api.thingspeak.com";
WiFiClient client;
String upload;
int received=0;


volatile int flow_frequency; // Measures flow sensor pulsesunsigned 
int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

unsigned long washStartTime;

void flow () // Interrupt function
{
   flow_frequency++;
}

void uploadData(int data){
  upload="GET /update?key=YDMPIK2BF9YB9R07&field1="+String(data);
  received=0;
  while(received==0){
    if (!client.connected()) {
      Serial.println();
      Serial.println("connection teardown");
      client.stop();
    
      while (!client.connect(server, 80)) {
        delay(15000);
      }
      Serial.println("connected to server ");
    }
    client.println(upload);
    client.println();
    Serial.print("uploaded: ");
    Serial.println(data);
    while (client.available()) {
      char c = client.read();
      i=(int) c;
      if (i>=48 && i<=57) received=1;
      Serial.write(c);
    }
    if(received==0) delay(5000);
  }
}

void setup()
{
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3), flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;

  while (status!=WL_CONNECTED && i<=10) {
    Serial.print("嘗試連線第 ");
    Serial.print(i);
    Serial.println("次...");
    status=WiFi.begin(ssid,pass);
    if (status!=WL_CONNECTED)
       Serial.println("     無法連線");
    else 
       Serial.println("     成功連線");
  
    i++;
    delay(5000);
  }
  uploadData(-1);
}

void loop ()
{
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if(currentTime >= (cloopTime + 1000))
  {
    cloopTime = currentTime; // Updates cloopTime
    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
    flow_frequency = 0; // Reset Counter
    Serial.print(l_hour, DEC); // Print litres/hour
    Serial.println(" L/hour");
  }
  
  if(currentTime > (washStartTime+20000) ){
    if(isWashing == 1){
      isWashing=0;
      uploadData(isWashing);
    }else if(l_hour>50){
      isWashing = 1;
      washStartTime = currentTime;
      uploadData(isWashing);
    }
  
  }
}
