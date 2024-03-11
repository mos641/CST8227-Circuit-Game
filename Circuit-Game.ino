/**
 ** Name: Mostapha A
 ** Expected Grade: 100
 ** Known Issues: N/A
**/

#include <Bounce2.h> // import Bounce2

void countUpdate(int num = -1);

Bounce pushButton = Bounce();

const int TIMING = 500;

int SER = 14; //data
int LATCH = 15; //RCLK
int CLOCK = 16; //SRCLK

int pshBttn = 19;

int count = 0;
int savedCount = 0;

int bttnCount = 0;
int roundCount = 1;

unsigned long lastTime = 0;

// decimal representations of flipped(common anode) binary numbers for the 7-Segment display, ending with the decimal and display off
int numbers[12] = {136, 235, 76, 73, 43, 25, 24, 203, 8, 9, 247, 255};

void setup() {
  Serial.begin(9600);
  Serial.print("Round #");
  Serial.print(roundCount);
  Serial.println("…");
  // set pin modes
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(pshBttn, INPUT);

  // attach pbs pin to bounce object
  pushButton.attach(pshBttn);
  pushButton.interval(5);

  // shuffle random function
  randomSeed(analogRead(22));
}

void loop() {
  // update bounce object
  pushButton.update();

  // if buton was pressed, use match game logic
  if (pushButton.fell() == true) {
    updateCheckMatch();
  }

  // change the count after the set timing interval
  if ((millis() - lastTime) >= TIMING) {
    countUpdate();
    lastTime = millis();
  }
}

void countUpdate(int num = -1) {
  if (num == -1) {
    // generate random number
    int prevRand = count;
    while (count == prevRand) {
      count = random(0, 10);
    }
  }
  else {
    count = num;
  }

  // set latch low until changes are made then return to high
  digitalWrite(LATCH, LOW);
  shiftOut(SER, CLOCK, MSBFIRST, numbers[count]);
  digitalWrite(LATCH, HIGH);
}

void updateCheckMatch() {
  bttnCount++;
  Serial.print("PBS press # ");
  Serial.print(bttnCount);
  Serial.print(" ==> ");
  Serial.println(count);
  delay(2000);
  // if this is the first press save count, otherwise compare and restart
  if (bttnCount == 1) {
    savedCount = count;
  } else {
    if (savedCount == count) {
      Serial.println("**MATCH**");
      Serial.println();
      // blink for two seconds
      for (int i = 0; i < 4; i++) {
        countUpdate(11);
        delay(250);
        countUpdate(savedCount);
        delay(250);
      }
    } else {
      Serial.println("NO Match… period.");
      Serial.println();
      // display the decimal point on the 7-segment display for 2-seconds
      for (int i = 0; i < 4; i++) {
        // set latch low until changes are made then return to high
        countUpdate(10);
        delay(250);
        countUpdate(11);
        delay(250);
      }
    }
    // restart, new round
    bttnCount = 0;
    roundCount++;
    Serial.print("Round #");
    Serial.print(roundCount);
    Serial.println("…");
  }
  lastTime = millis();
}
