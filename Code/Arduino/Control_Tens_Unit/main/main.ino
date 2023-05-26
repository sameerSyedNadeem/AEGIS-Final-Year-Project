#include "Calibration.h"

// Defining the primary buttons on Tens Unit
#define ON_INCREASE  9
#define OFF_DECREASE 1
#define MODE         10
#define CHANNEL_SWAP 11


void setup() {
    Serial.begin(115200);
    for (int DIGITAL_BUTTONS = 2; DIGITAL_BUTTONS < 10; DIGITAL_BUTTONS++) {
    pinMode(DIGITAL_BUTTONS, OUTPUT);
    if (DIGITAL_BUTTONS < 8) digitalWrite(DIGITAL_BUTTONS, LOW);
    //else digitalWrite(DIGITAL_BUTTONS, HIGH);
    }
    

    delay(300);
    Serial.println("done");
    cal.device.swapChannel();
    cal.device.changeMode();
    cal.device.changeMode();
    cal.calibrateForceFeedback();
    Serial.println("done");
    cal.device.swapChannel();
}

void loop() {
    Calibration cal = Calibration(ON_INCREASE, OFF_DECREASE, MODE, CHANNEL_SWAP);
    cal.calibratingImpactFeedback();
  // put your main code here, to run repeatedly:
    char ltr0 = '0';
    char ltr1 = '0';
    if (Serial.available() > 1)
    {
    ltr0 = Serial.read();
    ltr1 = Serial.read();
    Serial.print(ltr0);
    if (ltr0 == 'q' || ltr1 == 'q') {                                  // Left arm block
        cal.calibration.toggleLBlockImpulseFeedback();
        cal.calibration.sessionStatus[LBLOCK] = 0;
        cal.calibration.activateLForceFeedback();
    }
    if (ltr0 == 'w' || ltr1 == 'w'){
        cal.calibration.activateLForceFeedback();
        cal.calibration.toggleChopImpulseFeedback();
        cal.calibration.sessionStatus[CHOP] = 0;
    }
    if (ltr0 == 'r' || ltr1 == 'r'){
        cal.calibration.activateRForceFeedback();
        cal.calibration.togglePunchImpulseFeedback();
        cal.calibration.sessionStatus[PUNCH] = 0;
    }
    if (ltr0 == 't' || ltr1 == 't') {  // Right arm block
        cal.calibration.toggleRBlockImpulseFeedback();
        cal.calibration.sessionStatus[RBLOCK] = 0;
        cal.calibration.activateRForceFeedback();
    }
    if (ltr0 == 'e' || ltr1 == 'e'){
        cal.device.switchOff();
    }
    }
    else if (ltr0 == '0' && ltr1 == '0') {
        Serial.print(ltr0);
        cal.calibration.sessionStatus[LBLOCK] = 1; //Clearing for future impacts
        cal.calibration.sessionStatus[RBLOCK] = 1;
        cal.calibration.sessionStatus[CHOP]   = 1;
        cal.calibration.sessionStatus[PUNCH]  = 1;
        cal.calibration.togglePunchImpulseFeedback();
        cal.calibration.toggleLBlockImpulseFeedback();
        cal.calibration.deactivateLForceFeedback();
    }
    delay(7);
}
