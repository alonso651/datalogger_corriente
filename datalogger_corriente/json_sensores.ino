String json_sensores(){

    // Sensibilidad del sensor en V/A
    //float SENSIBILITY = 0.185;   // Modelo 5A
    //float SENSIBILITY = 0.100; // Modelo 20A
    float SENSIBILITY = 0.066; // Modelo 30A
    
    int SAMPLESNUMBER = 10;

    //Lectura de los sensores
    float current = getCorriente(SAMPLESNUMBER);
    float currentRMS = 0.707 * current;
    float power = 230.0 * currentRMS;
    IPAddress ip = WiFi.localIP();

    //Transformación de tipos - float -> Char
    char corriente[8];
    dtostrf(current, 6, 2, corriente); 
    char corrienteRMS[8];
    dtostrf(currentRMS, 6, 2, corrienteRMS); 
    char potencia[8];
    dtostrf(power, 6, 2, potencia); 
    String ipLocal="";
    for (int i=0; i<4; i++) {
      ipLocal += i  ? "." + String(ip[i]) : String(ip[i]);
    } 

    //Construcción del Objeto JSON
    StaticJsonBuffer<250> doc;
    JsonObject& root = doc.createObject();
    root["nodo"] = String(ESP.getChipId());
    //root["ssid"] = String(wifiManager.getSSID());
    //root["pass"] = String(wifiManager.getPassword());
    root["localIP"] = ipLocal;
    root["intensidad"] = corriente;
    root["intensidadRMS"] = corrienteRMS;
    root["potencia"] = potencia;
    String output;
    root.printTo(output);
    return output;
}
  
float getCorriente(int samplesNumber)
{
   float voltage;
   float corrienteSum = 0;
   for (int i = 0; i < samplesNumber; i++)
   {
      voltage = analogRead(A0) * 5.0 / 1023.0;
      corrienteSum += (voltage - 2.5) / SENSIBILITY;
   }
   return(corrienteSum / samplesNumber);
}