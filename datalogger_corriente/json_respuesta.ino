void json_respuesta(String dato) {
  //parse JSON
  int longitud = dato.length();
  longitud += 1;
  char jsonChar[longitud];
  dato.toCharArray(jsonChar, sizeof(jsonChar));
  StaticJsonBuffer<250> jsonBuffer;
  
	JsonObject& root1 = jsonBuffer.parseObject(jsonChar);
	// Test if parsing succeeds.
  if (!root1.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  const char* comando = root1["comando"];
  const char* valorComando = root1["valor"];
  String respuesta;
  if (String(comando) == "loopTime") {
    loopTime = atoi(valorComando);
    respuesta = "loopTime : ";
    respuesta += String(loopTime);
  }else if (String(comando) == "chipId") {
    respuesta = "ESP ChipId : ";
    respuesta += String(ESP.getChipId());
  }else if (String(comando) == "start") {
    respuesta = "Inicio : OK";
    record = true;
  }else if (String(comando) == "stop") {
    respuesta = "Fin : OK";
    record = false;
  }
  else {
    respuesta = "Comando desconocido";
  }

  //Construcción del Objeto JSON
  StaticJsonBuffer<200> doc1;
  JsonObject& root2 = doc1.createObject();
  root2[comando] = respuesta;
  String output;
  root2.printTo(output);

  client.publish("corriente_up1", respuesta.c_str());
  Serial.print("Publish message: ");
  Serial.print("[corriente_up1] "); Serial.println(respuesta);
}
