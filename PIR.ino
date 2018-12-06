//Henry's Bench
// HC-SR501 Motion Detector
// Sample Sketch

#define pir1 2
#define pir2 3
#define ledpir1 4
#define ledpir2 5
#define outputled 6
#define switch_ 8
enum state {AVAILABLE, TRIGGERED, COOLDOWN};
const char* stateAsString[] = {"Available", "Triggered", "Cooldown"};

bool p1val; // Place to store read PIR Value
bool p2val; // Place to store read PIR Value
bool p1prev;
bool p2prev;

long p1downtime;
long p2downtime;

long p1triggerT;
long p2triggerT;

bool init_ = false;
bool switchOn = false;

int tableLine = 0;

state PIR1 = AVAILABLE;
state PIR2 = AVAILABLE;
state PIR1prev;
state PIR2prev;

void setup() {
  Serial.begin(9600);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(ledpir1, OUTPUT);
  pinMode(ledpir2, OUTPUT);
  pinMode(switch_, INPUT);
  pinMode(outputled, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledpir1, HIGH);
    digitalWrite(ledpir2, HIGH);
    delay(500);
  }
  digitalWrite(ledpir1, LOW);
  digitalWrite(ledpir2, LOW);
}

void loop() {
  switchOn = digitalRead(switch_);

  if (millis() > 60000 && !init_) {
    init_ = true;
    Serial.println("Warm up complete!");
  }

  p1val = digitalRead(pir1);
  p2val = digitalRead(pir2);

  if (!p1prev && p1val) { // If just triggered.
    PIR1 = TRIGGERED;
    p1triggerT = millis();
  }

  if (p1prev && !p1val) { // If just reset.
    p1downtime = millis();
    PIR1 = COOLDOWN;
  }

  if (millis() > p1downtime + 3000 && PIR1 == COOLDOWN) {
    PIR1 = AVAILABLE;
  }

  if (!p2prev && p2val) { // If just triggered.
    PIR2 = TRIGGERED;
    p2triggerT = millis();
  }

  if (p2prev && !p2val) { // If just reset.
    p2downtime = millis();
    PIR2 = COOLDOWN;
  }

  if (millis() > p2downtime + 3000 && PIR2 == COOLDOWN) {
    PIR2 = AVAILABLE;
  }

  digitalWrite(ledpir1, p1val);
  digitalWrite(ledpir2, p2val);

  if (PIR1 != PIR1prev || PIR2 != PIR2prev) {
    overview();
    checkTimings();
  }

  p1prev = p1val;
  p2prev = p2val;
  PIR1prev = PIR1;
  PIR2prev = PIR2;
}

void checkTimings() {
  long deltaT = p1triggerT - p2triggerT;
  if (abs(deltaT) < 2000) {
    if (p1triggerT < p2triggerT) { //PIR2 was later, walked into the room. Turn lights on.
      digitalWrite(outputled, HIGH);
    } else {
      digitalWrite(outputled, LOW);
    }
  }
}

void overview() {
  Serial.print("[");
  Serial.print(tableLine++);
  Serial.print("] \t");
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] \t\t");
  Serial.print(stateAsString[PIR1]);
  Serial.print("\t");
  Serial.print(stateAsString[PIR2]);
  Serial.println();
}
