///////////////////////////////////////////////SD////////////////////////////////////////////////////
#include <SPI.h>
#include <SD.h>
#define SD_CS_PIN 53     // Chip Select para módulo SD
#define PulsadorSD 49
#define ledPin 47
bool recording = false;
///////////////////////////////////////////////TCS34725///////////////////////////////////////////////
#include "Adafruit_TCS34725.h"
// Inicializar el sensor con valores por defecto
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
//////////////////////////////////////////////////LCD/////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>
// Configuración de la pantalla LCD 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4); // Dirección I2C, columnas, filas
#include <Wire.h>
/////////////////////////////////////////////MLX90614 TEMPERATURA/////////////////////////////////////
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
///////////////////////////////////////////////////BLE33//////////////////////////////////////////////
#define I2C_ADDRESS 0x08  // Dirección I2C del esclavo (Arduino 33BLE)
//////////////////////////////////////////////////BOMBAS//////////////////////////////////////////////
//////////////////////////////////////////////////BOMBA1//////////////////////////////////////////////
const int AUMENTARB1 = 33;    //Pin pulsador aumentar bomba 1
const int DISMINUIRB1 = 31;    // Pin pulsador dismunuir bomba 1
const int DETENERB1 = 29;    // Pin pulsador parar bomba 1
//////////////////////////////////////////////////BOMBA2//////////////////////////////////////////////
const int AUMENTARB2 = 39;    //Pin pulsador aumentar bomba 2
const int DISMINUIRB2 = 37;    // Pin pulsador dismunuir bomba 2
const int DETENERB2 = 35;    // Pin pulsador parar bomba 2
//////////////////////////////////////////////////BOMBA3//////////////////////////////////////////////
const int AUMENTARB3 = 45;    //Pin pulsador aumentar bomba 3
const int DISMINUIRB3 = 43;    // Pin pulsador dismunuir bomba 3
const int DETENERB3 = 41;    // Pin pulsador parar bomba 3
////////////////////////////////////////////ACTIVACION BOMBAS//////////////////////////////////////////
const int MAESTRO = 27; //Pin pulsador para activar o desactivar el sistema
bool ACTIVACION=false;    // Estado del sistema
/////////////////////////////////////////////////PULSADORES///////////////////////////////////////////
unsigned long ultimaPulsacion11 = 0;  // Tiempo última pulsación botón 1
unsigned long ultimaPulsacion12 = 0;  // Tiempo última pulsación botón 2
unsigned long ultimaPulsacion13 = 0;  // Tiempo última pulsación botón 3

unsigned long ultimaPulsacion21 = 0;  // Tiempo última pulsación botón 4
unsigned long ultimaPulsacion22 = 0;  // Tiempo última pulsación botón 5
unsigned long ultimaPulsacion23 = 0;  // Tiempo última pulsación botón 6

unsigned long ultimaPulsacion31 = 0;  // Tiempo última pulsación botón 7
unsigned long ultimaPulsacion32 = 0;  // Tiempo última pulsación botón 8
unsigned long ultimaPulsacion33 = 0;  // Tiempo última pulsación botón 9

unsigned long ultimaPulsacionM = 0;  // Tiempo última pulsación botón 10

//////////////////////////////////////////////////TB6612FNG///////////////////////////////////////////
const int PWM1 = 5;    // PWM Bomba 1
const int PWM2 = 9;    // PWM Bomba 2
const int PWM3 = 13;    // PWM Bomba 3
const int AIN1 = 3;   // Control de dirección 1 B1
const int AIN2 = 4;   // Control de dirección 2 B1
const int AIN3 = 7;   // Control de dirección 1 B2
const int AIN4 = 8;   // Control de dirección 2 B2
const int AIN5 = 11;   // Control de dirección 1 B3
const int AIN6 = 12;   // Control de dirección 2 B3
const int STBY1= 2;   // Standby D1
const int STBY2 = 6;   // Standby D2
const int STBY3 = 10;   // Standby D3

int velocidad1=0;
int velocidad2=0;
int velocidad3=0;

int flujo1=0;
int flujo2=0;
int flujo3=0;

void setup() {
//////////// Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INICIANDO SISTEMA");
  delay(2000);
  lcd.clear();
//////////// Inicializar sensor de color
  if (!tcs.begin()) {
    lcd.clear();
    lcd.print("Error sensor color");
    delay(2000);
    lcd.clear();
  }
  // Encender LED integrado del sensor
  tcs.setInterrupt(false); // false = LED ON, true = LED OFF
  delay(500); // Esperar estabilización
  
////////////Inicializar sensor de temperatura
  if (!mlx.begin()) {
    lcd.clear();
    lcd.print("Error sensor temp");
    delay(2000);
    lcd.clear();
  }
////////////////BLE33/////////////////
Wire.begin(); // Inicia I2C como maestro

  ///////////////////////// Configuración de pulsadores//////////////////////////////
  pinMode(AUMENTARB1, INPUT_PULLUP);
  pinMode(DISMINUIRB1, INPUT_PULLUP);
  pinMode(DETENERB1, INPUT_PULLUP);

  pinMode(AUMENTARB2, INPUT_PULLUP);
  pinMode(DISMINUIRB2, INPUT_PULLUP);
  pinMode(DETENERB2, INPUT_PULLUP);

  pinMode(AUMENTARB3, INPUT_PULLUP);
  pinMode(DISMINUIRB3, INPUT_PULLUP);
  pinMode(DETENERB3, INPUT_PULLUP);

  pinMode(MAESTRO, INPUT_PULLUP);

  ///////////////////////////Configuracuion de Bombas///////////////////////////////
  pinMode(PWM1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY1, OUTPUT);

  pinMode(PWM2, OUTPUT);
  pinMode(AIN3, OUTPUT);
  pinMode(AIN4, OUTPUT);
  pinMode(STBY2, OUTPUT);

  pinMode(PWM3, OUTPUT);
  pinMode(AIN5, OUTPUT);
  pinMode(AIN6, OUTPUT);
  pinMode(STBY3, OUTPUT);
  
  // Activar el módulo (salir de standby)
  digitalWrite(STBY1, HIGH);
  digitalWrite(STBY2, HIGH);
  digitalWrite(STBY3, HIGH);
  
  // Establecer dirección del motor
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN3, HIGH);
  digitalWrite(AIN4, LOW);
  digitalWrite(AIN5, HIGH);
  digitalWrite(AIN6, LOW);


}
void loop() {
  
//////////////////////MLX90614////////////////////////
float tempObjeto = mlx.readObjectTempC();
delay(200);
////////////////////////////BLE33///////////////////////////
  // Enviar temperatura al esclavo
  Wire.beginTransmission(I2C_ADDRESS);
  byte *tempBytes = (byte *)&tempObjeto;
  for(int i = 0; i < sizeof(float); i++) {
    Wire.write(tempBytes[i]);
  }
  Wire.endTransmission();
/////////////////////////////////////////TCS34725///////////////////////////////////////////////////
  uint16_t R, G, B, C;
// Leer valores brutos del sensor
tcs.getRawData(&R,&G,&B,&C);
int r=R+320;
int g=G-5;
int b=B+200;
int RR=map(r,0,480,0,255);
int GG=map(g,0,260,0,255);
int B2=map(b,0,450,0,255);
if(RR>=160 && GG<=50&& B2>=110){
  lcd.setCursor(17,0);
  lcd.print("NA");
}else{
  lcd.setCursor(17,0);
  lcd.print("  ");
}
//////////////////////////////////////////////////BOMBA1//////////////////////////////////////////////
  /////////AUMENTAR////////
  if (digitalRead(AUMENTARB1) == LOW && (millis() - ultimaPulsacion11) > 200) {
    velocidad1 = min(velocidad1 + 2, 255);
    ultimaPulsacion11 = millis();           // Registra momento de pulsación
  }

  /////////DISMUNUIR///////
  if (digitalRead(DISMINUIRB1) == LOW && (millis() - ultimaPulsacion12) > 200) {
    velocidad1 = max(velocidad1 - 2, 0);
    lcd.setCursor(3,0);
    lcd.print("   ");
    ultimaPulsacion12 = millis();           // Registra momento de pulsación
  }
    /////////PARAR////////
  if (digitalRead(DETENERB1) == LOW && (millis() - ultimaPulsacion13) > 200) {
    lcd.setCursor(3,0);
    lcd.print("   ");
    velocidad1 = 0;
    flujo1=0;
    ultimaPulsacion13 = millis();           // Registra momento de pulsación
  }
//////////////////////////////////////////////////BOMBA2//////////////////////////////////////////////
  /////////AUMENTAR////////
  if (digitalRead(AUMENTARB2) == LOW && (millis() - ultimaPulsacion21) > 200) {
    velocidad2 = min(velocidad2 + 2, 255);
    ultimaPulsacion21 = millis();           // Registra momento de pulsación
  }

  /////////DISMUNUIR///////
  if (digitalRead(DISMINUIRB2) == LOW && (millis() - ultimaPulsacion22) > 200) {
    velocidad2 = max(velocidad2 - 2, 0);
    lcd.setCursor(3,1);
    lcd.print("   ");
    ultimaPulsacion22 = millis();           // Registra momento de pulsación
  }
    /////////PARAR////////
  if (digitalRead(DETENERB2) == LOW && (millis() - ultimaPulsacion23) > 200) {
    lcd.setCursor(3,1);
    lcd.print("   ");
    velocidad2 = 0;
    flujo2=0;
    ultimaPulsacion23 = millis();           // Registra momento de pulsación
  }
//////////////////////////////////////////////////BOMBA3//////////////////////////////////////////////
  /////////AUMENTAR////////
  if (digitalRead(AUMENTARB3) == LOW && (millis() - ultimaPulsacion31) > 200) {
    velocidad3 = min(velocidad3 + 2, 255);
    ultimaPulsacion31 = millis();           // Registra momento de pulsación
  }

  /////////DISMUNUIR///////
  if (digitalRead(DISMINUIRB3) == LOW && (millis() - ultimaPulsacion32) > 200) {
    velocidad3 = max(velocidad3 - 2, 0);
    lcd.setCursor(3,2);
    lcd.print("   ");
    ultimaPulsacion32 = millis();           // Registra momento de pulsación
  }
    /////////PARAR////////
  if (digitalRead(DETENERB3) == LOW && (millis() - ultimaPulsacion33) > 200) {
    lcd.setCursor(3,2);
    lcd.print("   ");
    velocidad3 = 0;
    flujo3=0;
    ultimaPulsacion33 = millis();           // Registra momento de pulsación
  }
//////////////////////////////////////////////////FLUJOS//////////////////////////////////////////////////
//velocidad1=(-8.0/33.0)*flujo1*flujo1+(138.0/11.0)*flujo1(-1276.0/33.0);
//velocidad2=(1.0/52.0)*flujo2*flujo2+(167.0/52.0)*flujo2+(935.0/26.0);
//velocidad3=(-2.0/21.0)*flujo3*flujo3+(46.0/7.0)*flujo3+(16.0/21.0)-1.0;
//////////////////////////////////////////////////ACTIVACION//////////////////////////////////////////////
  /////////ACTIVACION////////
  if (digitalRead(MAESTRO) == LOW && (millis() - ultimaPulsacionM) > 200) {
    ACTIVACION=!ACTIVACION;
    ultimaPulsacionM = millis();           // Registra momento de pulsación
  }
    // Aplicar velocidad a las bombas si el sistema está activo
    if (ACTIVACION) {
      lcd.setCursor(1,3);
      lcd.print("ACTIVADO");
      analogWrite(PWM1, velocidad1);
      analogWrite(PWM2, velocidad2);
      analogWrite(PWM3, velocidad3);
    } else {
      lcd.setCursor(1,3);
      lcd.print("        ");
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 0);
      analogWrite(PWM3, 0);
    }
//////////////////////////////////////////////////////LCD/////////////////////////////////////////////////////////////
//////////////////////TCS34725////////////////////////
lcd.setCursor(11,0);
lcd.print("R");
lcd.setCursor(12,0);
lcd.print("   ");
lcd.setCursor(12,0);
lcd.print(RR);

lcd.setCursor(11,1);
lcd.print("G");
lcd.setCursor(12,1);
lcd.print("   ");
lcd.setCursor(12,1);
lcd.print(GG);

lcd.setCursor(11,2);
lcd.print("B");
lcd.setCursor(12,2);
lcd.print("   ");
lcd.setCursor(12,2);
lcd.print(B2);
//////////////////////MLX90614////////////////////////
lcd.setCursor(10,3);
lcd.print("T:");
lcd.setCursor(12,3);
lcd.print(tempObjeto);
lcd.setCursor(18,3);
lcd.print("oC");
///////////////////////BOMBA 1////////////////////////
lcd.setCursor(0,0);
lcd.print("B1:");
lcd.setCursor(3,0);
lcd.print(velocidad1);
lcd.setCursor(6,0);
lcd.print("mL/m");
///////////////////////BOMBA 2///////////////////////
lcd.setCursor(0,1);
lcd.print("B2:");
lcd.setCursor(3,1);
lcd.print(velocidad2);
lcd.setCursor(6,1);
lcd.print("mL/m");
///////////////////////BOMBA 3///////////////////////
lcd.setCursor(0,2);
lcd.print("B3:");
lcd.setCursor(3,2);
lcd.print(velocidad3);
lcd.setCursor(6,2);
lcd.print("mL/m");
}
