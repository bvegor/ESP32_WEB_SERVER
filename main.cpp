#include <WiFi.h>

int batterie = 14;
const char* ssid     = "Greenway-Velo-001";
const char* password = "123456789";

WiFiServer server(80);
String header;


String outputPhareState = "off";

const int outputPhare = 2;

void setup() {

if (batterie < 15)
{


  Serial.begin(115200);
  pinMode(outputPhare, OUTPUT);
  digitalWrite(outputPhare, LOW);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);
  
  server.begin();
}
}

void loop(){
  WiFiClient client = server.available();  

  if (client) {                             
    Serial.println("New Client.");       
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {            
        char c = client.read();          
        Serial.write(c);                  
        header += c;
        if (c == '\n') {                   
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /26/on") >= 0) {
              outputPhareState = "on";
              digitalWrite(outputPhare, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              outputPhareState = "off";
              digitalWrite(outputPhare, LOW);
            } 
              
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1><em> Greenway<em></h1> <h2>Allumer le feu avant du velo</h2>");
            client.println("<p>Phares - Statut " + outputPhareState + "</p>");    
            if (outputPhareState=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;    
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}