#include <ESP32Time.h>
#include <WiFi.h>

/*
 *  Varialbles de MUX y targetas 
 */
String mux_conexion;
String targeta_corte = "486854576510";  
String targeta_empleado1 = "484956656510"; 
String targeta_empleado2 = "485752496510";
String targeta_empleado3 = "484955486510";
/*
 *   VARIABLES COLABORADOR
 */
String Id_Colaborador = "";
String Fecha_Hora_Colaborador = "";

const char*ssid ="RED ACCESA";
const char*password ="037E32E7";

/*
 *    VARIABLES RELOJ
 */
ESP32Time rtc;
const char*ntpServer ="pool.ntp.org";
const long gmtOffset_sec = -6*3600;
const int daylightOffsett_sec=0;
String Reloj;

int rele_1 = 32;

void setup() {
  Serial.begin(9600);         // MONITOR SERIAL
  Serial1.begin(9600);        // MUX
  //Serial2.begin(9600);        // MONEDERO
  pinMode(rele_1, OUTPUT);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("CONECTADO");
  
  configTime(gmtOffset_sec, daylightOffsett_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo);
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
}

void loop() {
  if(Serial1.available() >0) {
    while (Serial1.available()) {
      char c = char(Serial1.read());
      mux_conexion = mux_conexion + String(Serial1.read()) ;
      //Serial.print(char(Serial1.read()));
    }
    
    if(mux_conexion == targeta_corte){
        Serial.println("En un momento le hacemos su corte");
    }
    else if(mux_conexion == targeta_empleado1 || mux_conexion == targeta_empleado2 || mux_conexion == targeta_empleado3){
        Serial.println("Entras al baño buen señor");
        Id_Colaborador = mux_conexion;
        Serial.println(Id_Colaborador);
        Reloj = rtc.getTime("%d/%m/%Y %H:%M:%S");        
        Fecha_Hora_Colaborador = Reloj;
        Serial.println(Fecha_Hora_Colaborador);
        
        digitalWrite(rele_1, HIGH);
        delay(1000);
        digitalWrite(rele_1, LOW);
    }
    else{
        Serial.println("Esta tarjeta no fue aceptada, consulte a su provedor\nTarjeta id = " + (mux_conexion));
    }
    
    mux_conexion = "";
  }

  
}
