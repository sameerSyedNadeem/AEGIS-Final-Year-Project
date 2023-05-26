#include "Session.h"

struct Calibration
{
    Machine device;
    Session calibration;

    Calibration(int on=9, int off=10, int mode=1, int swap=11){
        device = Machine(on, off, mode, swap);
        calibration = Session();
    }
    
    bool calibrateForceFeedback() {
        char input1;
        Serial.flush();
        calibration.activateRForceFeedback();
        while (true) {
            if (Serial.available()) {
            input1 = Serial.read();
            if (input1 == 'i') {
                Serial.println("increasing");
                device.increaseMagnitude();
            }
            else if (input1 == 'd') {
                device.changeMode();
            }
            else if (input1 == 't') {
                calibration.sessionStatus[ELECPOS] = !calibration.sessionStatus[ELECPOS];
                calibration.activateRForceFeedback();
            }
            else if (input1 == 'n') {
                Serial.println("Calibrated force"); calibration.deactivateRForceFeedback();
                // return ELECTRODE_POSITION_R;
            }
            }
        }
    }

/*    bool CALIBRATING_L_FORCE_FEEDBACK(int* DEVICE_STATUS, bool ELECTRODE_POSITION_L = 0) {
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
    }*/

    void calibratingImpactFeedback() {
        char input1;
        Serial.flush();
        calibration.togglePunchImpulseFeedback();
        while (true) {
            if (Serial.available()) {
            input1 = Serial.read();
            if (input1 == 'i') {
                Serial.println("increasing");
                device.increaseMagnitude();
            }
            else if (input1 == 'd') {
                device.changeMode();
            }
            else if (input1 == 'n') {
                Serial.println("Calibrating impact"); 
                calibration.togglePunchImpulseFeedback();
                return;
            }
            }
        }
    }
}
