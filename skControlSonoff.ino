#include <ESP8266WiFi.h>

//#define RELE_1  0
//#define RELE_2  2

int rele1=1, rele2=1;
int LED = 13;
int RELE_1 = 12;


WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:

 
  
  pinMode(RELE_1, OUTPUT);
  pinMode(LED, OUTPUT);

  delay(500);

  digitalWrite(RELE_1,LOW);
  digitalWrite(LED, LOW);

  Serial.begin(115200);
  delay(500);

  Serial.println("\r\n");
  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  WiFi.mode(WIFI_STA);
  //WiFi.softAP("MOVISTAR_47E8","ndfBakCEvtHwj8jSSEMJ");
  // WiFi.softAP("Wireless-N","z123456z");
  //WiFi.softAP("Hello_IoT", "12345678");
  // WiFi.softAP("AI-THINKER_C0E300");
  WiFi.begin("Wireless-N", "z123456z");
  WiFi.config(IPAddress(192, 168, 1, 46), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");

    if (++timeout > 100)
    {
      Serial.println("Sin Conexion WIFI");
      while (1) {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
      }
    }

  }
  
  Serial.println();

  // Check connection
  if (WiFi.status() == WL_CONNECTED)
  {
    // ... print IP Address
    Serial.print("IP address STATION: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED, HIGH);
  }
  else
  {
    Serial.println("Can not connect to WiFi station. Go into AP mode.");

    // Go into software AP mode.
    WiFi.mode(WIFI_AP);

    delay(10);

    WiFi.softAP("AI-THINKER_C0E300");

    Serial.print("IP address Access Point: ");
    Serial.println(WiFi.softAPIP());
  }

  IPAddress http_server_ip = WiFi.localIP();

  server.begin();

  Serial.print("Nuestra server IP:");
  Serial.print(http_server_ip);
  Serial.print("\r\n");

}

void loop() {
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();

  if (!client) {
    return;
  }
  Serial.print("NUEVO CLIENTE\r\n");

  while (!client.connected()) {

    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.print(req);
  Serial.print("\r\n");
  client.flush();


  if (req.indexOf("rele1/off") != -1)
  {
    rele1 = 0;

    Serial.print("rele1 off\r\n");
  }
  else if (req.indexOf("rele1/on") != -1)
  {
    rele1 = 1;
    Serial.print("rele1 on\r\n");
  }
  
  else
  {

    Serial.println("invalid request");
    client.stop();
    return;
  }

  digitalWrite(RELE_1, rele1);
  digitalWrite(LED, ! rele1);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "Rele1 "; 
  s += (rele1)  ? "on\n" :"off\n" ;
  s += "</html>\n";


  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");


}

