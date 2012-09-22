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
const int turnOffTime = 0;
long lastPIR=0, curTime;
int PIRreading;

int PIRstate = 0;


const int eyes=18;
const int mouth[3]= {17, 16, 15}; // center, middle, outer


void setup(void)
{
  pinMode(pirPin, INPUT);
  faceSetup();
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
    faceOff();
  }
  else {
    faceOn();
  }
}


void faceSetup() {
  pinMode(eyes, OUTPUT);
  for(int i=0; i<3; i++)
    pinMode(mouth[i], OUTPUT);
  faceOff();
}

void faceOff() {
  digitalWrite(eyes, LOW);
  for(int i=2; i>=0; i--) {
    delay(750);
    digitalWrite(mouth[i], LOW);
  }
}

void faceOn() {
  for(int i=0; i<3; i++) {
    digitalWrite(mouth[i], HIGH);
    delay(750);
  }
  digitalWrite(eyes, HIGH);
}
