
 //accel


 //KONSTANTA ZA STUPNJEVE! (stepper1.currentPosition()/10*0.68) (0.65 eksperimentalno)


 #include <AccelStepper.h>
#define HALFSTEP 8

#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  7     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1


#include <dht.h>



#include <LiquidCrystal.h>
#include <SoftwareSerial.h> // use the software uart

//za lcd
SoftwareSerial bluetooth(2, 4); // RX, TX
const int rs = 0, en = 1, d4 = A5, d5 = A4, d6 = A3, d7 = A2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
dht DHT;
#define DHT11_PIN 12
//od blututa
unsigned long previousMillis = 0;        // will store last time
const long interval = 500;           // interval at which to delay
static uint32_t tmp; // increment this
const int button1Pin = 9;  // Gumb
const int button2Pin = 8;  // Gumb
const float konstanta_1 = 0.65;

int stepsPerRevolution = 2048; // amount of steps per revolution
//Stepper myStepper(stepsPerRevolution, stepIN1Pin, stepIN3Pin, stepIN2Pin, stepIN4Pin); //za stepper library
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
 
   stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(100.0);
//  stepper1.setSpeed(200);
  lcd.begin(16, 2);

    lcd.setCursor(0, 1);
  lcd.print("Temperatura:");
  lcd.setCursor(0, 0);
    lcd.print("      vlaga:");
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  pinMode(13, OUTPUT); 
//  Serial.begin(9600);  
  bluetooth.begin(9600); // start the bluetooth uart at 9600 which is its default
  delay(200); // wait for voltage to stabilize
  bluetooth.print("Grijanje"); 
                                     
  delay(3000); // wait for settings to take affect. 
}

void loop() {

int button1State, button2State;
 
if (stepper1.distanceToGo()<2 && stepper1.distanceToGo()>-2){
  

 button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  
  }
  else {
    button1State = LOW;
  button2State = LOW;
    }
  

if (stepper1.currentPosition()>-5){
//  
if (((button1State == LOW) && !(button2State == LOW))) { // if we're pushing button 1 OR button 2


if (stepper1.distanceToGo()<=0){ //bug fixes... works but it could be better...
stepper1.moveTo(stepper1.currentPosition()-2);
}
else{
  stepper1.moveTo(stepper1.currentPosition()+2);
  }
  stepper1.enableOutputs();
stepper1.moveTo(stepper1.currentPosition()+1);
stepper1.stop();
lcd.setCursor(0, 0);
lcd.print("     ");
//stepper1.move(200);
//stepper1.run();
stepper1.setSpeed(-20000);
stepper1.runSpeed();
stepper1.moveTo(stepper1.currentPosition()-1);



}
}

if (stepper1.currentPosition()/10*konstanta_1<100){

 if (((button2State == LOW) && !(button1State == LOW))) { 

if (stepper1.distanceToGo()<=0){
stepper1.moveTo(stepper1.currentPosition()-2);
}
else{
  stepper1.moveTo(stepper1.currentPosition()+2);
  }
  stepper1.enableOutputs();
lcd.setCursor(0, 0);
lcd.print("     ");

stepper1.setSpeed(20000);
stepper1.runSpeed();
stepper1.moveTo(stepper1.currentPosition()+1);

  }
  
}

  
 

if (bluetooth.available()){



  switch (bluetooth.read()) {
  
    case '1':
     if(!stepper1.isRunning()){
     bluetooth.print("Grijanje UGASENO!!!!!!!!!!!!!!!");
       digitalWrite(13,LOW); 
       stepper1.enableOutputs();

        stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(200);
        stepper1.moveTo(0);
        lcd.setCursor(2, 0);
       lcd.print(" ");
     }

     else{
       bluetooth.print("Cekaj (WORKING)!!!!!!!!!!!!!!!");
      }
    break;

    case '2':

      
     if(!stepper1.isRunning()){
    bluetooth.print("Grijanje UPALJENO!!!!!!!!!!!!!!!");
    stepper1.enableOutputs();


           stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(200);
        stepper1.moveTo(960);
        lcd.setCursor(2, 0);
       lcd.print(" ");
 }

   else{
       bluetooth.print("Cekaj (WORKING)!!!!!!!!!!!!!!!");
      }
       
        break;

   
     case '3':

     bluetooth.print(int( stepper1.currentPosition()/10*konstanta_1));
     bluetooth.print("% ");
     bluetooth.print( DHT.temperature);
    bluetooth.print("C  ");
    if(stepper1.isRunning()){
       bluetooth.print("Working!!");
      }
      else {
        bluetooth.print("Idle!!!");
        }
    }


   
}
  
  
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    tmp++;
    if (tmp % 4 == 0){
       int chk = DHT.read11(DHT11_PIN);
        lcd.setCursor(12, 1);
       lcd.print(DHT.temperature);
       lcd.setCursor(14,1);
       lcd.print((char)223);
       lcd.setCursor(15, 1);
       lcd.print("C");
       lcd.setCursor(12,0);
       lcd.print(DHT.humidity, 0);
       lcd.setCursor(15,0);
        lcd.print("%");
        
        

    }
  }

lcd.setCursor(0, 0);
        lcd.print( int( stepper1.currentPosition()/10*konstanta_1));

       

if (stepper1.isRunning()){
     digitalWrite(13,HIGH); 
  lcd.setCursor(3,0);
lcd.print("!");
}
else{
     digitalWrite(13,LOW); 
lcd.setCursor(3,0);
lcd.print(" ");

stepper1.disableOutputs();

}

if (int( stepper1.currentPosition()/10*konstanta_1)<0 || int( stepper1.currentPosition()/10*konstanta_1)>=101){

  
 stepper1.stop();

  }
        stepper1.run();

        


}
