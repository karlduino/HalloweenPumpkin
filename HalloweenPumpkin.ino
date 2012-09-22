/**********************************************************************
 * HalloweenPumpkin
 *
 * A halloween pumpkin project, based on a LadyAda project:
 *   http://www.ladyada.net/make/pumpkin/pumpkin.html
 *
 * Karlduino, September, 2012
 * 
 * licensed under GPL 3
 **********************************************************************/

const int pirPin = 19;
const int turnOffTime = 1000;
long lastPIR=0, curTime;
int PIRreading;

int PIRstate = 0;


const int eyes=18;
const int mouth[3]= [17, 16, 15]; // center, middle, outer


void setup(void)
{
  pinMode(pirPin, INPUT);
  pinMode(eyes, OUTPUT);
  digitalWrite(eyes, LOW);
  for(int i=0; i<3; i++) {
    pinMode(mouth[i], OUTPUT);
    digitalWrite(eyes, LOW);
  }
}


void loop(void) {
  curTime = millis();
  PIRreading = digitalRead(pirPin);

  if(PIRreading==HIGH) {
    lastPIR = curTime;
    PIRstate = 1;
  }
  else if(PIRstate==1 && curTime > lastPIR + turnOffTime) {
    PIRstate = 0;
  }

  if(PIRstate==0) {
    digitalWrite(eyes, LOW);
    for(int i=0; i<3; i++) {
      digitalWrite(mouth[i], LOW);
    }
  }
  else {
    digitalWrite(eyes, HIGH);
    for(int i=0; i<3; i++) {
      digitalWrite(mouth[i], HIGH);
    }
  }
}
