// Defining the primary buttons on Tens Unit
#define ON_INCREASE  9
#define OFF_DECREASE 1
#define MODE         10
#define CHANNEL_SWAP 11

// Defining Electrode Switches
#define ARM_FORCE_FEEDBACK_CONTROL_1_pos        7
#define ARM_FORCE_FEEDBACK_CONTROL_1_neg        6
#define ARM_FORCE_FEEDBACK_CONTROL_2_pos        5
#define ARM_FORCE_FEEDBACK_CONTROL_2_neg        4
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos     3
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg     2

// Defining a delay for machine
#define DELAY  100 // in ms

bool ELECTRODE_POSITION = 0;


int DEVICE_STATUS[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // On-Off status
// Channel 0 (A), 1 (B), 2 (Both)
// MAGNITUDE_A
// MAGNITUDE_B
// ForceFeedback Electrode Position  (0 for inactive)
// ImpactFeedback Electrode Position (0 for inactive)
// Mode Channel A
// Mode Channel B

bool CALIBRATING_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION = 0) {
  char input1;
  Serial.flush();
  ACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION);
  while (true) {
    if (Serial.available()) {
      input1 = Serial.read();
      if (input1 == 'i') {
        Serial.println("increasing");
        INCREASE_MAGNITUDE(DEVICE_STATUS);
      }
      else if (input1 == 'd') {
        CHANGE_MODE(DEVICE_STATUS);
      }
      else if (input1 == 't') {
        ELECTRODE_POSITION = !ELECTRODE_POSITION;
        ACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION);
      }
      else if (input1 == 'n') {
        Serial.println("Calibrated force"); DEACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS);
        return ELECTRODE_POSITION;
      }
    }
  }
}

void CALIBRATING_IMPACT_FEEDBACK(int* DEVICE_STATUS) {
  char input1;
  Serial.flush();
  TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS, 1);
  while (true) {
    if (Serial.available()) {
      input1 = Serial.read();
      if (input1 == 'i') {
        Serial.println("increasing");
        INCREASE_MAGNITUDE(DEVICE_STATUS);
      }
      else if (input1 == 'd') {
        CHANGE_MODE(DEVICE_STATUS);
      }
      else if (input1 == 'n') {
        Serial.println("Calibrating impact"); TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS, 0);
        return;
      }
    }
  }
}

void setup() {

  Serial.begin(115200);

  // put your setup code here, to run once:
  for (int DIGITAL_BUTTONS = 2; DIGITAL_BUTTONS < 10; DIGITAL_BUTTONS++) {
    pinMode(DIGITAL_BUTTONS, OUTPUT);
    if (DIGITAL_BUTTONS < 8) digitalWrite(DIGITAL_BUTTONS, LOW);
    //else digitalWrite(DIGITAL_BUTTONS, HIGH);
  }

  CALIBRATING_IMPACT_FEEDBACK(DEVICE_STATUS);
  delay(300);
  Serial.println("done");
  SWAP_CHANNEL(DEVICE_STATUS);
  CHANGE_MODE(DEVICE_STATUS);
  CHANGE_MODE(DEVICE_STATUS);
  ELECTRODE_POSITION = CALIBRATING_FORCE_FEEDBACK(DEVICE_STATUS);
  Serial.println("done");
  SWAP_CHANNEL(DEVICE_STATUS);
}

bool flag = 0;
int imCount = 0;
int foCount = 0;
void loop() {
  // put your main code here, to run repeatedly:
  char ltr = '0';
  if (Serial.available() > 0)
  {
    ltr = Serial.read();
    Serial.print(ltr);
    if (ltr == '1') { flag = 1; Serial.print("flag accepted");}
    else { flag = 0; }
  }
  if (flag){
    Serial.println("active");
    if (imCount == 1)
    {TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS, 1);}imCount++;
    ACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION);
  }

  else {
//    Serial.println("deactive");
    TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS, 0);
    if (foCount > 10){
    DEACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS);}
    imCount = 0;
    foCount++;
  }
  delay(7);
}

void SHOW_DEVICE_STATUS(int* DEVICE_STATUS) {
  for (int i = 0; i < 7; i++) {
    Serial.print(DEVICE_STATUS[i]);
    Serial.print(", ");
  }
  Serial.println(DEVICE_STATUS[7]);
}

void SWITCH_ON(int* DEVICE_STATUS) {
  if (!DEVICE_STATUS[0]) {
    digitalWrite(ON_INCREASE, HIGH);
    delay(DELAY);
    digitalWrite(ON_INCREASE, LOW);
    delay(DELAY);
    DEVICE_STATUS[0] = 1;
  }
  else return;
}

void SWAP_CHANNEL(int* DEVICE_STATUS) {
  digitalWrite(CHANNEL_SWAP, HIGH);
  delay(DELAY);
  digitalWrite(CHANNEL_SWAP, LOW);
  delay(DELAY);
  DEVICE_STATUS[1]++;
  DEVICE_STATUS[1] %= 3;
}

void CHANGE_MODE(int* DEVICE_STATUS) {
  digitalWrite(MODE, HIGH);
  delay(DELAY);
  digitalWrite(MODE, LOW);
  delay(DELAY);
  if (DEVICE_STATUS[1] == 0) {
    DEVICE_STATUS[6]++;
    DEVICE_STATUS[6] %= 8;
  }
  else if (DEVICE_STATUS[1] == 1) {
    DEVICE_STATUS[7]++;
    DEVICE_STATUS[7] %= 8;
  }
  else if (DEVICE_STATUS[1] == 2) {
    DEVICE_STATUS[6]++; DEVICE_STATUS[6] %= 8;
    DEVICE_STATUS[7]++; DEVICE_STATUS[7] %= 8;
  }
}

void INCREASE_MAGNITUDE(int* DEVICE_STATUS) {
  digitalWrite(ON_INCREASE, HIGH);
  delay(DELAY);
  digitalWrite(ON_INCREASE, LOW);
  delay(DELAY);
  if (DEVICE_STATUS[1] == 0 && DEVICE_STATUS[2] < 8) {
    DEVICE_STATUS[2]++;
  }
  else if (DEVICE_STATUS[1] == 1 && DEVICE_STATUS[3] < 8) {
    DEVICE_STATUS[3]++;
  }
  else if (DEVICE_STATUS[1] == 2) {
    if (DEVICE_STATUS[2] < 8) {
      DEVICE_STATUS[2]++;
    }
    if (DEVICE_STATUS[3] < 8) {
      DEVICE_STATUS[3]++;
    }
  }
}

// not complete
void DECREASE_MAGNITUDE(int* DEVICE_STATUS) {
  digitalWrite(OFF_DECREASE, HIGH);
  delay(DELAY);
  digitalWrite(OFF_DECREASE, LOW);
  delay(DELAY);
  if (DEVICE_STATUS[1] == 0 && DEVICE_STATUS[2] > 0) {
    DEVICE_STATUS[2]++;
  }
  else if (DEVICE_STATUS[1] == 1 && DEVICE_STATUS[3] > 0) {
    DEVICE_STATUS[3]++;
  }
  else if (DEVICE_STATUS[1] == 2) {
    if (DEVICE_STATUS[2] < 8) {
      DEVICE_STATUS[2]++;
    }
    if (DEVICE_STATUS[3] < 8) {
      DEVICE_STATUS[3]++;
    }
  }
}

// Dont use this
void SWITCH_OFF(int* DEVICE_STATUS) {
  if (DEVICE_STATUS[0]) {
    digitalWrite(OFF_DECREASE, HIGH);
    delay(DELAY);
    digitalWrite(OFF_DECREASE, LOW);
    delay(DELAY);
  }
  else return;
}

//// 0 00, 1 01, 2 10, 3 11
//void CHANGE_FORCE_FEEDBACK(int* DEVICE_STATUS){
//  DEVICE_STATUS[4]++;
//  DEVICE_STATUS[4] %= 2;
//}

void ACTIVATE_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION) {
  if (!ELECTRODE_POSITION) {
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
  }
  else if (ELECTRODE_POSITION) {
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
  }
}

void DEACTIVATE_FORCE_FEEDBACK(int* DEVICE_STATUS) {
  digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
  digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
  digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
  digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
}

//void TOGGLE_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE){
//  if (ELECTRODE) {
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
//    DEVICE_STATUS[4] == 2;
//    }
//  else {
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
//    digitalWrite(ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
//    DEVICE_STATUS[4] == 1;
//    }
//}

//0 00, 1 01
void TOGGLE_IMPULSE_FEEDBACK(int* DEVICE_STATUS, bool ACTIVATE) {
  //  DEVICE_STATUS[4]++;
  DEVICE_STATUS[4] = ACTIVATE;
  if (ACTIVATE) {
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, LOW);
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, LOW);
  }
  else if (!ACTIVATE) {
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, HIGH);
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, HIGH);
  }
}


//void SIMULATING_INTERACTION(int* DEVICE_STATUS, bool ELECTRODE_POSITION){
////  CALIBRATING_IMPACT_FEEDBACK(DEVICE_STATUS);
//  SWAP_CHANNEL(DEVICE_STATUS);
////  TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS);
//  ACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION);
//  delay(500);
////  TOGGLE_IMPULSE_FEEDBACK(DEVICE_STATUS);
//  DEACTIVATE_FORCE_FEEDBACK(DEVICE_STATUS);
//}


