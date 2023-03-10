#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
/*Put WiFi SSID & Password*/
const char* ssid = "WifiName";   // Enter SSID here
const char* password = "Password"; // Enter Password here

SoftwareSerial NodeMCU(D2,D3);


ESP8266WebServer server(80);

bool status = LOW;
 int i=1;
void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(D4, OUTPUT);
  NodeMCU.begin(4800);
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check NodeMCU is connected to Wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/lock", handle_lock);
  server.on("/unlocked", handle_unlocked);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}
void loop() {
  server.handleClient();
 
  if(status)
  {
    NodeMCU.print(i);
    }

}

void handle_OnConnect() {
  status = LOW;
  Serial.println("Unlocked");
  server.send(200, "text/html", updateWebpage(status)); 
}

void handle_lock() {
  status = HIGH;
  Serial.println("Locked");
  server.send(200, "text/html", updateWebpage(status)); 
}

void handle_unlocked() {
  status = LOW;
  Serial.println("Unlocked");
  server.send(200, "text/html", updateWebpage(status)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String updateWebpage(uint8_t status){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>IOT smart door lock</title>\n";
  ptr +="<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>IOT smart door lock</h3>\n";
  
   if(status){
    ptr +="<p>Locked</p><a class=\"button button-off\" href=\"/unlocked\">Locked</a>\n";
   }else{
    ptr +="<p>Unlocked</p><a class=\"button button-on\" href=\"/lock\">Unlock</a>\n";
   }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
