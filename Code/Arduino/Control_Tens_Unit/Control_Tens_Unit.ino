// Defining the primary buttons on Tens Unit
#define ON_INCREASE  9
#define OFF_DECREASE 1
#define MODE         10
#define CHANNEL_SWAP 11

// Defining Electrode Switches
#define R_ARM_FORCE_FEEDBACK_CONTROL_1_pos        7
#define R_ARM_FORCE_FEEDBACK_CONTROL_1_neg        6
#define R_ARM_FORCE_FEEDBACK_CONTROL_2_pos        5
#define R_ARM_FORCE_FEEDBACK_CONTROL_2_neg        4
#define L_ARM_FORCE_FEEDBACK_CONTROL_1_pos        A5
#define L_ARM_FORCE_FEEDBACK_CONTROL_1_neg        A4
#define L_ARM_FORCE_FEEDBACK_CONTROL_2_pos        A3
#define L_ARM_FORCE_FEEDBACK_CONTROL_2_neg        A2
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos     3
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg     2
#define BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos     8
#define BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg     12
#define BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos     13
#define BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg     A1
#define CHOP_IMPACT_FEEDBACK_CONTROL_1_pos      A0
#define CHOP_IMPACT_FEEDBACK_CONTROL_1_neg      0


// Defining a delay for machine
#define DELAY  100 // in ms

bool ELECTRODE_POSITION_R = 0;
bool ELECTRODE_POSITION_L = 0;
bool L_Block = 1;
bool R_Block = 1;
bool Chop = 1;
bool Punch = 1;

int DEVICE_STATUS[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
// 0th On-Off status
// 1st Channel 0 (A), 1 (B), 2 (Both)
// 2nd MAGNITUDE_A
// 3rd MAGNITUDE_B
// 4th RForceFeedback Electrode Position  (0 for inactive)
// 5th PunchImpactFeedback Electrode Position (0 for inactive)
// 6th Mode Channel A
// 7th Mode Channel B
// 8th LForceFeedback Electrode Position  (0 for inactive)
// 9th Chop Electrode position (0 for inactive)
// 10th Left block
// 11th Right block

bool CALIBRATING_R_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION_R = 0) {
  char input1;
  Serial.flush();
  ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_R);
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
        ELECTRODE_POSITION_R = !ELECTRODE_POSITION_R;
        ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_R);
      }
      else if (input1 == 'n') {
        Serial.println("Calibrated force"); DEACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS);
        return ELECTRODE_POSITION_R;
      }
    }
  }
}

bool CALIBRATING_L_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION_L = 0) {
  char input1;
  Serial.flush();
  ACTIVATE_L_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_L);
  while (true) {
    if (Serial.available()) {
      input1 = Serial.read();
      if (input1 == 't') {
        ELECTRODE_POSITION_L = !ELECTRODE_POSITION_L;
        ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_L);
      }
      else if (input1 == 'n') {
        Serial.println("Calibrated Left spatially"); DEACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS);
        return ELECTRODE_POSITION_L;
      }
    }
  }
}

void CALIBRATING_IMPACT_FEEDBACK(int* DEVICE_STATUS) {
  char input1;
  Serial.flush();
  TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
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
        Serial.println("Calibrating impact"); TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
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
  ELECTRODE_POSITION_R = CALIBRATING_R_FORCE_FEEDBACK(DEVICE_STATUS);
  ELECTRODE_POSITION_L = CALIBRATING_L_FORCE_FEEDBACK(DEVICE_STATUS);
  Serial.println("done");
  SWAP_CHANNEL(DEVICE_STATUS);
}

void loop() {
  // put your main code here, to run repeatedly:
  char ltr0 = '0';
  char ltr1 = '0';
  if (Serial.available() > 1)
  {
    ltr0 = Serial.read();
    ltr1 = Serial.read();
    Serial.print(ltr0);
    if (ltr0 == 'q' || ltr1 == 'q') {                                  // Left arm block
      TOGGLE_L_BLOCK_IMPULSE_FEEDBACK(DEVICE_STATUS);
      L_Block = 0;
      ACTIVATE_L_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_L);
    }
    if (ltr0 == 'w' || ltr1 == 'w'){
      ACTIVATE_L_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_L);
      TOGGLE_CHOP_IMPULSE_FEEDBACK(DEVICE_STATUS);
      Chop = 0;
    }
    if (ltr0 == 'r' || ltr1 == 'r'){
      ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_R);
      TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
      Punch = 0;
    }
    if (ltr0 == 't' || ltr1 == 't') {                                  // Right arm block
      TOGGLE_R_BLOCK_IMPULSE_FEEDBACK(DEVICE_STATUS);
      R_Block = 0;
      ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_R);
    }
    if (ltr0 == 'e' || ltr1 == 'e'){
      SWITCH_OFF(DEVICE_STATUS);
    }
  }
  else if (ltr0 == '0' && ltr1 == '0') {
    Serial.print(ltr0);
    bool L_Block = 1;     //Clearing for future impacts
    bool R_Block = 1;
    bool Chop = 1;
    bool Punch = 1;
    TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
    TOGGLE_L_BLOCK_IMPULSE_FEEDBACK(DEVICE_STATUS);
    DEACTIVATE_L_FORCE_FEEDBACK(DEVICE_STATUS);
  } // Copyright Team AEGIS
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

// Now can be used
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

void ACTIVATE_R_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION_R) {
  if (!ELECTRODE_POSITION_R) {
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
  }
  else if (ELECTRODE_POSITION_R) {
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
  }
}

void DEACTIVATE_R_FORCE_FEEDBACK(int* DEVICE_STATUS) {
  digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
  digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
  digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
  digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
}

void ACTIVATE_L_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION_L) {
  if (!ELECTRODE_POSITION_L) {
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
  }
  else if (ELECTRODE_POSITION_L) {
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
    digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
  }
}

void DEACTIVATE_L_FORCE_FEEDBACK(int* DEVICE_STATUS) {
  digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
  digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
  digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
  digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
}




//0 00, 1 01
void TOGGLE_PUNCH_IMPULSE_FEEDBACK(int* DEVICE_STATUS) {
  //  DEVICE_STATUS[4]++;
  DEVICE_STATUS[4] = Punch;
  if (Punch) {
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, LOW);
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, LOW);
  }
  else if (!Punch) {
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, HIGH);
    digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, HIGH);
  }
}

void TOGGLE_R_BLOCK_IMPULSE_FEEDBACK(int* DEVICE_STATUS) {
  
  DEVICE_STATUS[11] = R_Block;
  if (R_Block) {
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos, LOW);
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg, LOW);
  }
  else if (!R_Block) {
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos, HIGH);
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg, HIGH);
  }
}

void TOGGLE_L_BLOCK_IMPULSE_FEEDBACK(int* DEVICE_STATUS) {
  
  DEVICE_STATUS[10] = L_Block;
  if (L_Block) {
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos, LOW);
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg, LOW);
  }
  else if (!L_Block) {
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos, HIGH);
    digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg, HIGH);
  }
}

void TOGGLE_CHOP_IMPULSE_FEEDBACK(int* DEVICE_STATUS) {
  
  DEVICE_STATUS[9] = Chop;
  if (Chop) {
    digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_pos , LOW);
    digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_neg , LOW);
  }
  else if (!Chop) {
    digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_pos , HIGH);
    digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_neg , HIGH);
  }
}

//void TOGGLE_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE){
//  if (ELECTRODE) {
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
//    DEVICE_STATUS[4] == 2;
//    }
//  else {
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
//    digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
//    DEVICE_STATUS[4] == 1;
//    }
//}


//void SIMULATING_INTERACTION(int* DEVICE_STATUS, bool ELECTRODE_POSITION_R){
////  CALIBRATING_IMPACT_FEEDBACK(DEVICE_STATUS);
//  SWAP_CHANNEL(DEVICE_STATUS);
////  TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
//  ACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS, ELECTRODE_POSITION_R);
//  delay(500);
////  TOGGLE_PUNCH_IMPULSE_FEEDBACK(DEVICE_STATUS);
//  DEACTIVATE_R_FORCE_FEEDBACK(DEVICE_STATUS);
//}


