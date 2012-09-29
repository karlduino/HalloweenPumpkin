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

#include <WaveHC.h>
#include <WaveUtil.h>

const int pirPin = 19;
const int turnOffTime = 0;
long lastPIR=0, curTime;
int PIRreading;

int PIRstate = 0;
int lastPIRstate = 1;

const int eyes=6;
const int mouth[3]= {7, 8, 9}; // center, middle, outer
const int volumeInput = 1;

SdReader card;  // card
FatVolume vol;  // partition on card
FatReader root; // root directory on partition
FatReader file; // file on card
WaveHC wave;    // wave audio object

const int numFiles = 7;
char files[numFiles][13] = {"cackle.wav",   "catscrem.wav", "ghoullaf.wav",
                            "gostmoan.wav", "scream.wav",   "scream2.wav",
                            "screech.wav" };

const int numWelcomeFiles = 2;
char welcomeFiles[numWelcomeFiles][13] = {"hpyhalwn.wav", "welcome.wav" };


void setup(void)
{
  pinMode(pirPin, INPUT);
  pinMode(volumeInput, INPUT);
  faceSetup();

  // initialize card
  card.init();
  vol.init(card);
  root.openRoot(vol);

  randomSeed(analogRead(0));
}


void loop(void) {
  curTime = millis();
  lastPIRstate = PIRstate;
  PIRreading = digitalRead(pirPin);

  if(PIRreading==HIGH) {
    lastPIR = curTime;
    PIRstate = 1;
  }
  else if(PIRstate==1 && curTime > lastPIR + turnOffTime) {
    PIRstate = 0;
  }

  if(PIRstate==0) {
    faceOff(0);
  }
  else {
    eyesOn();
    if(lastPIRstate == 0) {
      playfile(welcomeFiles[random(numWelcomeFiles)]);
    }
    else {
      playfile(files[random(numFiles)]);
    }

    while(wave.isplaying) {// playing occurs in interrupts, so we print dots in realtime
      animateMouth(getVolume(volumeInput));
    }
    mouthOff(0);
    delay(5000 + random(10000));
  }
}


void faceSetup() {
  pinMode(eyes, OUTPUT);
  for(int i=0; i<3; i++)
    pinMode(mouth[i], OUTPUT);
  faceOff(0);
}

void faceOff(int delayAmount) {
  digitalWrite(eyes, LOW);
  mouthOff(delayAmount);
}

void mouthOff(int delayAmount) {
  for(int i=2; i>=0; i--) {
    delay(delayAmount);
    digitalWrite(mouth[i], LOW);
  }
}

void eyesOn() {
  digitalWrite(eyes, HIGH);
}

void eyesOff() {
  digitalWrite(eyes, HIGH);
}

void animateMouth(int volume) {
  const int thresholds[3] = {10, 45, 100};

  for(int i=0; i<3; i++) {
    if(volume > thresholds[i])
      digitalWrite(mouth[i], HIGH);
    else
      digitalWrite(mouth[i], LOW);
  }
}


// get measure of volume from wave shield
int getVolume(int volumePin)
{
  int volume, v, i;
  volume = 0;
  for(i=0; i<8; i++) {
    v = analogRead(volumePin) - 512;
    if(v < 0) v *= -1;
    if(v > volume) volume = v;
    delay(5);
  }
  return(volume-200);
}


// open and start playing a file
void playfile(char *name) {
  if (wave.isplaying) // already playing something
    wave.stop(); // stop it

  if(!file.open(root, name))
    return;

  if(!wave.create(file))
    return;

  wave.play();
}
