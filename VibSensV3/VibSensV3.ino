

/**********************************************
 * Mechincal Vibrations MPE292 Project MTE 200
 * 
 * Measuring Vibrations Frequency using Arduino
 * 
 * Code Last Edited by: Hany Elesawy 2022.05.22
 * 
 * --------- Team Members ---------
 * Hany Ali Ibrahim Elesawy
 * Roudina Hamada Elkahwagy
 * Nahed Mamdouh Abdullah
 * Mohamed Mahmoud Mohamed Zahran
 * Amr Marwan Saad Mahmoud
 * Mohamed Mahmoud Mohamed Sarary
 *********************************************/




/****************************************************************** General Stuff ******************************************************************/
#include <LiquidCrystal.h>

/*LCD Pins(-6- RS, E, D4, D5, D6, D7*/
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define ledRed     A0
#define ledYellow  A1
#define ledGreen   A2
#define ledWhite   A3
#define buzzer     A4


#define dVibSens 2                                                      /*vibration sensor digital pin*/

unsigned long startMillis=0;
unsigned long firstTime=0;                                              /*Time relapsed until first High detected be vibration sensor*/
unsigned long secondTime=0;                                             /*Time relapsed until second High detected be vibration sensor*/
unsigned long timeSinceLastVib=0;                                       /* the time (ms) since a vibration was detected*/
unsigned long resetFreqTime=2500;                                       /* the time (ms) at which to reset freq if there is no vibrations*/
long period=1000;
float freq;

int cycleState=1;                                                       /*cycleState determines whether to update firstTime or secondTime */



/****************************************************************** Setup ******************************************************************/
void setup(){

  lcd.begin(16, 2);
  
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledWhite, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  pinMode(dVibSens, INPUT); 


  digitalWrite(ledWhite, HIGH);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(buzzer, LOW);  
  Serial.begin(9600);
  startMillis = millis();
  
}
void loop(){


/****************************************************************** Main logic ******************************************************************/
  unsigned long measurement = vibration();
  delay(50);
  
  if (measurement > 0 )                                                            
    period = calcPeriod();

  if(period==0)   
    freq = 0;
  else
    freq = (float)(1000)/period;

  if( (millis()- timeSinceLastVib) > resetFreqTime ){
    period=0;
    freq=0;
  }

  if(round(freq) == 0)
  {
    digitalWrite(ledWhite, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(buzzer, LOW);  
  }
  else if(  freq > 0.1 && freq < 2){
    digitalWrite(ledWhite, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzer, LOW);
  }
  if( round(freq) > 2 && freq < 5){
    digitalWrite(ledWhite, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(buzzer, LOW);
  }
 if( round(freq) > 5 ){
    digitalWrite(ledWhite, LOW);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(buzzer, HIGH);
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vib. Frequency: ");
  lcd.setCursor(0, 1);
  lcd.print(freq);
  lcd.print(" Hz");

  
  Serial.print("The frequency is: ");
  Serial.print(freq);
  Serial.println(" Hz");


}

/****************************************************************** User Defined Functions ******************************************************************/

long vibration(){
  unsigned long measurement = pulseIn (dVibSens, HIGH);                              //wait for the pin to get HIGH and returns measurement
  return measurement;
}

long calcPeriod(){

  if(cycleState==1){
      firstTime=millis();
      timeSinceLastVib=millis();
      cycleState=2;
      
    }
    else if(cycleState==2){
      secondTime=millis();
      timeSinceLastVib=millis();
      cycleState=1;
    }
    long period = secondTime - firstTime;
    if(period<1)                                                                    /* in every third cycle the result is -ve so we take its absolute*/
    period=abs(period);
    
    return period;
  }
