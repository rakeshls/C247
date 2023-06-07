# include<WiFi.h>
# include<WiFiClient.h>
# inculde<WebServer.h>
# include<ESPmDNS.h>
const char* ssid = 'WR3005N3-757E';
const char* ssid = 'password';

String button = "<html><body id='bdy_1' style='height: 100px; width: 100px;'> <div id='bdy_2' style='height: 400px; width: 600px; margin-top: 50px; margin-left: 500px;'> <div style='margin-top: 100px; margin-left: 100px;'> <h2 style='color:white;'>ESP32 Webserver</h2> <p style='color:white;'>Click on the respected button to switch the Appliances:</p><div class='row'><table> <tr> <th><h3 style='color:white;'>Room Light</h3><input id= 'b1' type=button onClick=\"parent.location='/roomLight/on'\" value='On'> <input id= 'b2' type=button onClick=\"parent.location='/roomLight/off'\" value='Off'></th><th><h3 style='color:white;'>Room Bell</h3> <input id= 'b1' type=button onClick=\"parent.location='/roomBell/on'\" value='On'> <input id= 'b2' type=button onClick=\"parent.location='/roomBell/off'\" value='Off'></th></table></div></body></div></div><style>#bdy_1{background-image: url('https://i.postimg.cc/bvw7DS9S/Technological-background.jpg');background-repeat: no-repeat;background-size: 1920px 900px;font-family: 'helvetica neue';font-weight: 200;font-size: 20px;}#bdy_2{font-family: 'helvetica neue';font-weight: 200;font-size: 20px;}#b1{border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}#b2{border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}#b1{background-color: #4CAF50;}#b2{background-color: red;}</style></html>";

WebServer =server{80}

void handelRoot(){
  server.send(200,"text/html",button);
}

void handelNotFound(){
  String message = "file not found";
  message+="uri:";
  message+=server.uri()
  message+="\nMethod";
  message+=(server.method(==HTTP_GET)?"GET":"POST")
  message+="Arguments";
  message+=server.args();
  for(uint8_t i=0; i<server.args(); i++){
    message+=server.argName(i)+":"+server.arg(i)
  }
  server.send(404,"text/plain",message)
}

const int LED_1= 2;
const int Buzzer= 15;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_1,OUTPUT)
  pinMode(Buzzer,OUTPUT)
  Serial.begin(115200)
  WiFi.Mode(WiFi_sta)
  WiFi.begin(ssid,password)
  Serial.println("")
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) { //esp32.local/
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot); // esp32.local/

  server.on("/roomLight/on", []() {
    server.send(200, "text/html",button);
    digitalWrite(LED_1,HIGH);
    //delay(200);
    
  });

  server.on("/roomLight/off", []() {
    server.send(200, "text/html",button);
    digitalWrite(LED_1,LOW);
  });

  server.on("/roomBell/on", []() {
    server.send(200, "text/html",button);
    digitalWrite(Buzzer,HIGH); 
  });

  server.on("/roomBell/off", []() {
    server.send(200, "text/html",button);
    digitalWrite(Buzzer,LOW);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(LED_1,LOW);
  digitalWrite(Buzzer,LOW);
}

void loop(void) {
  server.handleClient();
}
}