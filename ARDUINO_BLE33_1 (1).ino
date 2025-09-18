#include <Wire.h>
#include <ArduinoBLE.h>

#define I2C_ADDRESS 0x08  // Misma dirección que en el maestro
const int relayPin = 12;  // Puedes cambiar este número al pin que estés usando

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  
  Wire.begin(I2C_ADDRESS); // Inicia I2C como esclavo
  Wire.onReceive(receiveEvent); // Registra el evento de recepción
  
  Serial.println("Arduino 33 BLE - Esclavo I2C control de relevador");
}

void loop() {
  // El procesamiento principal se hace en la función de callback
  delay(100);
}

void receiveEvent(int howMany) {
  if (howMany == sizeof(float)) {
    float temperatura;
    byte *tempBytes = (byte *)&temperatura;
    
    // Leer los 4 bytes del 
    for(int i = 0; i < sizeof(float); i++) {
      tempBytes[i] = Wire.read();
    }
    
    Serial.print("Temperatura recibida: ");
    Serial.print(temperatura);float
    Serial.println(" °C");
    
    // Control del relevador SSR
    if (temperatura <= 60.0) {
      digitalWrite(relayPin, HIGH); // Enciende el relevador
      Serial.println("Relevador ENCENDIDO (temperatura <= 60°C)");
    } else {
      digitalWrite(relayPin, LOW); // Apaga el relevador
      Serial.println("Relevador APAGADO (temperatura > 60°C)");
    }
  } else {
    // Descarta datos si no son un float completo
    while(Wire.available()) {
      Wire.read();
    }
    Serial.println("Error: Datos recibidos no coinciden con tamaño de float");
  }
}
