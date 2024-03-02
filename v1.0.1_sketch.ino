#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
//instructions des bibliothèques


const char *ssid = "IDOOM Fibre";
const char *password = "############";
//Configure l'ESP32 pour se connecter au réseau WiFi spécifié en utilisant les identifiants fournis.


WebServer server(80); //Initialise un serveur web sur le port 80
DHT dht(26, DHT22); //Définit un objet DHT pour les lectures du capteur sur la broche 26 en utilisant le type de capteur DHT22


void handleRoot() { //La fonction handleRoot() génère une page HTML dynamique avec les lectures de température et d'humidité avec les fonctions readDHTTemperature() et readDHTHumidity().
  char msg[1500];
  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    h3 { font-size: 2.0rem; }\
    p { font-size: 4.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>DATA-LOGGER ESP32 DHT - Reda</h2>\
      <h3>Projet USTHB</h3>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidité</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}


void setup(void) {


  Serial.begin(115200);
  dht.begin();
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");


  // Attendre la connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();
  delay(2);//permet au CPU de passer à d'autres tâches
}


float readDHTTemperature() {
  // Lire la température en Celsius (la valeur par défaut)
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Échec de lecture du capteur DHT!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}


float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Échec de lecture du capteur DHT.");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}
