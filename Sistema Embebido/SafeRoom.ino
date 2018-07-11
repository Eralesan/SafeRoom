
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial para BlueTooth 
#include <Servo.h>


Servo myservo;
int posicion = 0; 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


//SENSOR (BLUETOOTH) HC-06
const int BlueTooth2 = 3;
const int BlueTooth1 = 4;
SoftwareSerial ModBluetooth(BlueTooth1, BlueTooth2); // RX | TX 

//SENSOR (TEMPERATURA-HUMEDAD) DHT22
const int DHT22Pin = 5;
#define DHTPIN DHT22Pin //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22 //Se selecciona el DHT22(hay otros DHT)
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor


const int ServoPin = 6;
const int CoolerPin =11; 
const int BuzzerPin = 13 ; 

//Variables referidas a logica de proyecto:
 float ST_LOW_LIMIT = 90.0;
 float ST_HIGH_LIMIT = 100.0;
 float MONOXIDE_LOW_LIMIT = 400.0;
 float MONOXIDE_HIGH_LIMIT = 500.0;


 int wet=0;
 int monoxide=0;
 float temperature=0.0;
 int ST=0;
 float st=0.0;
 int manualSignal=0;
 boolean alarma=false;
 
 boolean estadoVentilacion=false;
 boolean estadoVentana=false;
 boolean estadoSafeRoom=false;
 boolean estadoLed=false;
 boolean estadoAlarma= false;

int MatrizSensacionTermica [][20] = {
{ 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95},
{ 16, 16, 17, 17, 17, 18, 18, 19, 19, 19, 19, 19, 20, 20, 20, 21, 21, 21, 21, 21},
{ 18, 18, 18, 19, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 22},
{ 19, 19, 19, 20, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23},
{ 20, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25},
{ 21, 21, 22, 22, 22, 22, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26},
{ 22, 23, 23, 23, 24, 24, 24, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28},
{ 24, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 29, 29, 29},
{ 25, 25, 25, 25, 26, 26, 26, 27, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 31},
{ 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 31, 32, 32, 33, 34, 34},
{ 26, 26, 27, 27, 27, 28, 29, 29, 29, 29, 30, 30, 31, 33, 33, 34, 35, 35, 37, 38},
{ 27, 27, 28, 28, 28, 28, 29, 29, 30, 30, 31, 32, 33, 34, 35, 36, 37, 39, 40, 41},
{ 28, 28, 29, 29, 29, 29, 30, 31, 31, 31, 33, 34, 35, 36, 37, 39, 40, 41, 45, 45},
{ 29, 29, 29, 29, 30, 31, 31, 33, 33, 34, 35, 35, 37, 39, 40, 42, 44, 45, 51, 51},
{ 29, 29, 30, 30, 31, 33, 33, 34, 34, 35, 36, 38, 39, 42, 43, 45, 49, 49, 53, 54},
{ 30, 30, 31, 31, 32, 34, 34, 35, 36, 37, 38, 41, 42, 44, 47, 48, 50, 52, 55, 00}
};

/*LEDS*/
int rojo=9;           //pin pwm 6
  int verde=10;          //pin pwm 10    
  //int azul=10;          //pin pwm 11
/*LEDS*/

/*  METODOS MONOXIDO */
 int getMonoxide() 
 {

    int monoxide;
    //check here about analog or digital and measures (percent or another).
    monoxide= analogRead(A0);
    
    return monoxide;
 }

void PrenderLed() {

  for (int i= 0; i< 2; i++) {

     analogWrite(rojo,255); //para controlar 5 voltios debo escribir 255
      analogWrite(verde,255);
      delay(1000);
      analogWrite(rojo,255); //para controlar 5 voltios debo escribir 255
      analogWrite(verde,0);
      delay(1000);
      analogWrite(rojo,0); //para controlar 5 voltios debo escribir 255
      analogWrite(verde,255);
      delay(1000);
  }
  estadoLed= false;
}

void apagarLed() {

  //analogWrite(rojo,0); //para controlar 5 voltios debo escribir 255
  //analogWrite(verde,255);
}

/* METODOS Servo */
void AbrirServo()
{
  myservo.write(0);
}

void CerrarServo()
{
  myservo.write(90);
}
/*  METODOS COOLER */
void EncenderPotenciaMediaCooler()
{
  analogWrite (CoolerPin,  255) ;

}
void EncenderPotenciaMaximaCooler()
{
   analogWrite (CoolerPin,  255) ;//n es el ciclo de trabajo
}



void ApagarCooler()
{
    analogWrite (CoolerPin,  0) ;//n es el ciclo de trabajo
}

/* FIN METODOS COOLER */

/*  METODOS TEMPERATURA */
 float getTemperature() 
 {

    float celciusDegree;
   
    return dht.readTemperature();
 }
/*  METODOS HUMEDAD */
 float getWet() 
 {
  
   float humedad = dht.readHumidity();
    return humedad;
    
 }


/* Display */
void setupDisplay()
{
  lcd.begin(16,2);
  lcd.print("ST:");
  
  //lcd.setCursor(11,0);
  //lcd.print((char)223);
  //lcd.print("C");
  lcd.setCursor(8,0);
  lcd.print("H:");

  lcd.setCursor(0,1);
  lcd.print("CO:");
}

void setup()  
{ 
     
    dht.begin(); //Se inicia el sensor
    ModBluetooth.begin(9600); 
    Serial.begin(9600);  
    ModBluetooth.println("MODULO CONECTADO");  
    ModBluetooth.print("#");
    myservo.attach(ServoPin,650,2400);
  pinMode(CoolerPin,  OUTPUT) ; 
  pinMode(BuzzerPin, OUTPUT);
  myservo.attach(ServoPin);
  setupDisplay();

  /*LEDS PWM*/
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
 // pinMode(azul, OUTPUT); 
  /*LEDS*/
    
}

// METODOS ALARMA--------------------

void activarAlarma() {
  digitalWrite(BuzzerPin,HIGH);
  alarma=true;
}


void desactivarAlarma() {
  digitalWrite(BuzzerPin,LOW);
  alarma=false;
}





void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{ 
      
    int x;    
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    float temperature= getTemperature();
    int monoxide= getMonoxide();
   
    for (x=0;x<loopTime;x++){    
      
      digitalWrite(speakerPin,HIGH);
     
      delayMicroseconds(delayAmount);
      digitalWrite(speakerPin,LOW);
      delayMicroseconds(delayAmount);
    }  
    
}  

// SENSACION TERMICA
int ObtenerSensacionTermica(float temperatura,int humedad){
  
  if((int)temperatura > 20 && (int)temperatura < 34){
    if(humedad > 0 && humedad < 100){
      int t = (int)temperatura;
      int n = humedad / 5;
      int h = n * 5;
      return (int)MatrizSensacionTermica[t,h];
    }
  }else{
    return (int)temperatura;
  }
    
    
}

float asignarValor (char vector[], int n) {

  float numero= atof(vector);
  Serial.print(numero);
  return numero;
}


void loop()  
{ 

      //voy a controlar cuantos voltajes mandarle a cada color
  wet = getWet(); //Se lee la humedad
  temperature = getTemperature(); //Se lee la temperatura
  monoxide= getMonoxide();
  ST = ObtenerSensacionTermica(temperature,wet);
    if (ModBluetooth.available())  
    {
        char VarChar; 
         
        VarChar = ModBluetooth.read(); 
        Serial.print(VarChar);
        
        if(VarChar == '1') 
        { 
          //digitalWrite(13, HIGH);
          ModBluetooth.print(temperature);
          ModBluetooth.print(",");
          ModBluetooth.print(wet);
          ModBluetooth.print(",");
          ModBluetooth.print(ST);
          ModBluetooth.print(",");
          ModBluetooth.print(monoxide);
          ModBluetooth.print(",");
          ModBluetooth.print((alarma==true)?"S":"N");
          ModBluetooth.print("#");
        }
        if(VarChar == '&') 
        {
          int  i = 0,j=0;
          String cadena= "";
          char Mat[4][6]= {{' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' '}};
          while(VarChar != '#'){
            VarChar  = ModBluetooth.read();
            Serial.print(VarChar);
           //cadena.concat(VarChar);
            if (VarChar == ',') {
              j++;
              i=0;
            } else {
              Mat[j][i]= VarChar;
              i++;
            }
          }
          ST_LOW_LIMIT = asignarValor(Mat[0],6);
          
          ST_HIGH_LIMIT = asignarValor(Mat[1],6);
         
          MONOXIDE_LOW_LIMIT = asignarValor(Mat[2],6);
          
          MONOXIDE_HIGH_LIMIT = asignarValor(Mat[3],6);
         
        }
        //ENCENDER / APARGAR VENTILACION
        if(VarChar == 'b') 
        { 
            if( estadoVentana == false ){
               AbrirServo();
               estadoVentana=true;
            }else{
              CerrarServo();
              estadoVentana=false;
            }
        }
        if(VarChar == 'c') 
        { 
            if( estadoVentilacion == false ){
              EncenderPotenciaMaximaCooler();
              estadoVentilacion=true;
              
            }else{
              EncenderPotenciaMediaCooler();
              estadoVentilacion=false;
            }
              
        }
        if(VarChar == 'd') 
        { 
            if( estadoLed == false ){
              PrenderLed();
              estadoLed=true;
              
            }else{
              apagarLed();
              estadoLed=false;
            }
              
        }
        if(VarChar == 'e') 
        { 
            if( estadoAlarma == false ){
              activarAlarma();
              estadoAlarma=true;
              
            }else{
              desactivarAlarma();
              estadoAlarma=false;
            }    
        }
      }
        

if((ST < ST_LOW_LIMIT) && (monoxide < MONOXIDE_LOW_LIMIT)) //Agrego parentesis entre las 2 condiciones
{
  //apagar vetilador,buzzer,led,girar servo a posición de cerrar
  if(estadoVentilacion == false){
    ApagarCooler();
  }

    //apagar vetilador,buzzer,led,girar servo a posición de cerrar
  if(estadoVentana == false){
    CerrarServo();
  }

  if(estadoAlarma == false) {

   desactivarAlarma(); 
  }

  if (estadoLed == false) {

    apagarLed();
  }
  lcd.setCursor(7,1);
  lcd.print("OK     ");
    //voy a controlar cuantos voltajes mandarle a cada color

  
  analogWrite(rojo,0); //para controlar 5 voltios debo escribir 255
  analogWrite(verde,255);

}else if((ST > ST_HIGH_LIMIT) || (monoxide >MONOXIDE_HIGH_LIMIT)){
    //prender ventilador(maxima potencia),buzzer,,led, girar servo a posición abrir.

    EncenderPotenciaMaximaCooler();
    activarAlarma();

    AbrirServo();
   lcd.setCursor(7,1);
   lcd.print("PELIGRO");
    //voy a controlar cuantos voltajes mandarle a cada color
  analogWrite(rojo,255); //para controlar 5 voltios debo escribir 255
  analogWrite(verde,0);
  //analogWrite(azul,0);
 }else {
      //prender ventilador(media),girar servo a posición abrir.
      EncenderPotenciaMediaCooler();
      AbrirServo();
      desactivarAlarma();

      lcd.setCursor(7,1);
      lcd.print("CUIDADO");
      //voy a controlar cuantos voltajes mandarle a cada color
  analogWrite(rojo,255); //para controlar 5 voltios debo escribir 255
  analogWrite(verde,255);
  //analogWrite(azul,255);
  }
  lcd.setCursor(3,0);
  lcd.print(ST);

  lcd.setCursor(3,1);
  lcd.print(monoxide);

  lcd.setCursor(10,0);
  lcd.print(wet);

 
  delay(1000);

}
