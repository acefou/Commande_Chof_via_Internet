//Pilotage Relais via requete http

//
// Acefou
// V2.0 13/05/2018
// avec OTA après validation reception commande relais depuis internet

//60:01:94:64:c5:be         

#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <Adafruit_NeoPixel.h>


#define PINLED         D2
#define NUMPIXELS      16
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PINLED, NEO_GRB + NEO_KHZ800);
int delayLed = 1000; 
const int LaLed = 0;
unsigned long previousMillis = 0;
unsigned long StartMillisRELAIS = 0;
long OnTimeON = 250;           // milliseconds of on-time
long OffTimeON = 750;          // milliseconds of off-time
long OnTimeOFF = 250;           // milliseconds of on-time
long OffTimeOFF = 5000;          // milliseconds of off-time
int ledState = LOW;  
              


const char* version_croquis = "acefou v20180513.2.0 cmd chauff OTA";
const char* ssid = "ssid";
const char* password = "password";

RemoteDebug Debug;
 
const int PinRelais = D1; // D1 GPIO5
const long DelaiRelaisON = 3600000; //3600000; // utilisé pour le delai
String strinterval= "60"; //60 minutes = 60 minutes x 60 sec  x 1000 milliseconde = 3 600 000 ; //utilisée pour affichage uniquement
ESP8266WebServer  server(80);
                 
String localWebPage = "";
String htmlStyle="";
String sHID;

int i=0;
char ID_monESP[ 16 ];

boolean  blnChauffageEnMarche = false;


void logMoiCa(String Msg, bool NL) {
  Serial.print(Msg);
  Debug.print(Msg);
  if (NL==true){
    Serial.println("");
    Debug.println("");
  }
} 

void handleNotFound() {
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
    
      for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
      }
      server.send ( 404, "text/plain", message );
      logMoiCa("reception d une requete inconnue",true);
}
 void handleRoot() {
      server.send(200, "text/html", localWebPage);
      logMoiCa("reception d une requete home",true);
}

void handleStatus() {
      if (blnChauffageEnMarche == false) {
        server.send(200, "text/plain", "200");
        logMoiCa("Status : le chauffage n est pas en marche = 200",true);
      } else {
        server.send(201, "text/plain", "201");
        logMoiCa("Status : le chauffage est en marche = 201",true);
      }
}

void handleremcmdchof() {
      //page de retour 200
      logMoiCa("reception d une requete handleremotecommandechauffage",true);
      //logMoiCa((String)cptloop,true);
      if (blnChauffageEnMarche == false) {
        server.send(200, "text/plain", "200");
        delay(1000);
        logMoiCa("Status : avant commande : le chauffage n est pas en marche = 200",true);
        executeEnvoiCommande();
      } 
      else {
        server.send(201, "text/plain", "201");
        logMoiCa("Status : avant commande : le chauffage est en marche = 201",true);
        executeArretCommande();
      }
}

void handlecommandechauffage() {
    
      logMoiCa("reception d une requete commandechauffage pendant ",false);
      logMoiCa(strinterval , false);
      logMoiCa(" minutes.", true );
      //page de retour 200
      server.send(200, "text/html", localWebPage);
      executeEnvoiCommande();
}

void handleBonjour() {
      String sstrstatus;
      if (blnChauffageEnMarche == false) {
        sstrstatus="arr&#234;t&#233;";
      } else {
        sstrstatus="en marche";
      }

     String s = "<!DOCTYPE HTML>\r\n<html><p>";
     s += version_croquis;
     s += "</p><p>ID_monESP:";
     s += sHID;
     s += "</p><p>IP locale : ";
     s += WiFi.localIP().toString();
     s += "</p><p></p><p>Le chauffage est ";
     s += sstrstatus;
     s += "</p><p>L' ESP est READY et OK.</p>";
     s += "<a href='../'>Retour</a>";
     s += "</html>\n";
     server.send(200, "text/html", s);
     logMoiCa("reception d une requete Bonjour",true);
}

void executeEnvoiCommande() {
    blnChauffageEnMarche = true;
    StartMillisRELAIS = millis();
    digitalWrite(LED_BUILTIN, LOW);     // Turn la LED on
    digitalWrite(PinRelais, HIGH);      // maj GPIO : commande relais ON
    
    logMoiCa("execution d une commande : executeEnvoiCommande",true);
}
              

void executeArretCommande() {
    blnChauffageEnMarche = false;
    StartMillisRELAIS =0;
    digitalWrite(LED_BUILTIN, HIGH);     // Turn la LED off
    digitalWrite(PinRelais, LOW);      // maj GPIO : commande relais OFF
    
    logMoiCa("execution d une commande : executeArretCommande",true);

}


void setup() {
  // Ouverture du port serie en 115200 baud pour envoyer des messages de debug à l'ide par exemple
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println(version_croquis);

  htmlStyle += "<head><style>h1 {font-size: 56px;text-align: center;text-transform: uppercase;color: #3366ff;} .button{";
  htmlStyle += "display: inline-block;padding: 15px 25px;font-size: 96px;cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;";
  htmlStyle += "background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 9px #999;width: 100%;}";
  htmlStyle += ".button:hover {background-color: #3e8e41}";
  htmlStyle += ".button:active {background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}</style></head>";

  localWebPage += htmlStyle;
  localWebPage += "<p><br></p><h1> Commande Chauffage </h1>";
  localWebPage += "<div style='text-align: center;'>";
  localWebPage += "<p><a href=\"commandechauffage\">";
  localWebPage += "<button class='button'>On</button></a></p></div>";
  localWebPage += "<p><br>propuls&eacute; par Acefou</p>";


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
  // prepare GPIO5 (sortie)
  pinMode(PinRelais, OUTPUT);
  digitalWrite(PinRelais, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connection à ");
  Serial.println(ssid);

  // On se connecte a reseau WiFi avec le SSID et le mot de passe precedemment configure
  WiFi.begin(ssid, password);

  // On sort de la boucle uniquement lorsque la connexion a ete etablie. Sinon, on reboot le module ESP
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {
    delay(500);
    Serial.print(".");
    if (i==21){
      Serial.println("Echec Connection WiFi ! Rebooting...");
      delay(5000);
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.println("WiFi connecté");

  // connexion OK, on demarre le server web 
  server.begin();
  Serial.println("Server démarré");

  // On indique sur le port serie l'adresse ip de l'ESP pour le trouver facilement
  Serial.println(WiFi.localIP());

  uint32_t n = ESP.getChipId();
  snprintf(ID_monESP, sizeof ID_monESP, "%08X\n", (unsigned long)n); 
  // == Serial.printf("Chip ID = %08X\n", ESP.getChipId());
  //Serial.println(ID_monESP);
  //Serial.println("");

// initialisation du debug telnet
  Debug.begin(ID_monESP);

 // Routage
  server.on("/", handleRoot);
  server.on("/bonjour", handleBonjour);
  server.on("/commandechauffage", handlecommandechauffage); //via reseau local
  server.on("/handleremcmdchof", handleremcmdchof);         //via internet
  server.on("/status", handleStatus);
  server.onNotFound ( handleNotFound );
  
  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();

  ArduinoOTA.onStart([]() {
    Debug.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Debug.println("End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Debug.println(".");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Debug.println("Error");
    if (error == OTA_AUTH_ERROR) Debug.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Debug.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Debug.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Debug.println("Receive Failed");
    else if (error == OTA_END_ERROR) Debug.println("End Failed");
  
  });

  //construit le nom du host à partir de l'ID unique de ESP
  int iID = ESP.getChipId();
  sHID = "ESP_ID_" ;
  sHID +=String(iID, HEX); 
  char charBuf[50];
  sHID.toCharArray(charBuf, 50) ;

  ArduinoOTA.setHostname(charBuf); 
  Serial.println(charBuf);
  ArduinoOTA.begin(); 

  pixels.begin(); // This initializes the NeoPixel library.
  //un peu brillant
  pixels.setBrightness(1);
 //bleu
  pixels.setPixelColor(LaLed, pixels.Color(0,0,255));
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(3000);
  //eteint
  pixels.setPixelColor(LaLed, pixels.Color(0,0,0));
  pixels.show(); // This sends the updated pixel color to the hardware.


//signature de la fin du setup avec la led built-in : 3 flash de 2 secondes
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);   
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);   
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);   
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("READY : GO to Loop");
}

void loop() {
      ArduinoOTA.handle();   
      Debug.handle();
      server.handleClient();
      unsigned long currentMillis = millis();
      unsigned long currentMillisRelais = currentMillis;

  if (blnChauffageEnMarche == true) {
  
                   

            // == commande relais en cours ( initialisé par l'appel web)
            // 1)on fait clignoter en rouge
            // 2) on verifie si on n'est pas arrivé au terme du tps de commande relais

            //logMoiCa(" chauffage en marche", true);                   

            // 1) clignotement
                      if((ledState == HIGH) && (currentMillis - previousMillis >= OnTimeON))
                      {
                        ledState = LOW;  // Turn it off
                        previousMillis = currentMillis;  // Remember the time
                        //eteint
                        pixels.setPixelColor(LaLed, pixels.Color(0,0,0));
                        pixels.show(); // This sends the updated pixel color to the hardware.     
                      }
                      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTimeON))
                      {
                        ledState = HIGH;  // turn it on
                        previousMillis = currentMillis;   // Remember the time
                        //rouge
                        pixels.setPixelColor(LaLed, pixels.Color(255,0,0));
                        pixels.show(); // This sends the updated pixel color to the hardware                
                      }
            // 2) verification si on doit arreter le relais
                      /*
                      Serial.print(currentMillisRelais);
                      Serial.print("  ");
                      Serial.println(StartMillisRELAIS);
                      */
                      if (currentMillisRelais >= (StartMillisRELAIS  + DelaiRelaisON) ) {
                          executeArretCommande(); //Arret relais
                      }else {
                       //logMoiCa("currentMillisRelais >= StartMillisRELAIS  + DelaiRelaisON   FALSE",true);
                      }
      }else{
            
            // en attente, le relais est arreté, on fait juste clignoter en bleu, tranquille
            //logMoiCa(" chauffage est en arret", true);
            if((ledState == HIGH) && (currentMillis - previousMillis >= OnTimeOFF))
            {
              ledState = LOW;  // Turn it off
              previousMillis = currentMillis;  // Remember the time
              //eteint
              pixels.setPixelColor(LaLed, pixels.Color(0,0,0));
              pixels.show(); // This sends the updated pixel color to the hardware.
          
            }
            else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTimeOFF))
            {
              ledState = HIGH;  // turn it on
              previousMillis = currentMillis;   // Remember the time
              //bleu
              pixels.setPixelColor(LaLed, pixels.Color(0,0,255));
              pixels.show(); // This sends the updated pixel color to the hardware.
          
            }        
      }
}

