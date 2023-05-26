#include "Machine.h"

#define R_ARM_FORCE_FEEDBACK_CONTROL_1_pos        7
#define R_ARM_FORCE_FEEDBACK_CONTROL_1_neg        6
#define R_ARM_FORCE_FEEDBACK_CONTROL_2_pos        5
#define R_ARM_FORCE_FEEDBACK_CONTROL_2_neg        4
#define L_ARM_FORCE_FEEDBACK_CONTROL_1_pos        A5
#define L_ARM_FORCE_FEEDBACK_CONTROL_1_neg        A4
#define L_ARM_FORCE_FEEDBACK_CONTROL_2_pos        A3
#define L_ARM_FORCE_FEEDBACK_CONTROL_2_neg        A2
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos       3
#define PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg       2
#define BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos       8
#define BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg       12
#define BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos       13
#define BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg       A1
#define CHOP_IMPACT_FEEDBACK_CONTROL_1_pos        A0
#define CHOP_IMPACT_FEEDBACK_CONTROL_1_neg        0

enum Actions {PUNCH, CHOP, LBLOCK, RBLOCK, ELECPOS};

struct Session 
{

    bool sessionStatus[5] = {0, 0, 0, 0, 0};

    Session() { }

    void activateRForceFeedback() {
        if (!sessionStatus[ELECPOS]) {
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
        }
        else if (sessionStatus[ELECPOS]) {
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
            digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
        }
    }

    void deactivateRForceFeedback() {
        digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
        digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
        digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
        digitalWrite(R_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
    }

    void activateLForceFeedback() {
        if (!sessionStatus[ELECPOS]) {
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, LOW);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, LOW);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
        }
        else if (sessionStatus[ELECPOS]) {
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, LOW);
            digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, LOW);
        }
    }

    void deactivateLForceFeedback() {
        digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_pos, HIGH);
        digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_1_neg, HIGH);
        digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_pos, HIGH);
        digitalWrite(L_ARM_FORCE_FEEDBACK_CONTROL_2_neg, HIGH);
    }




    //0 00, 1 01
    void togglePunchImpulseFeedback() {
    //  DEVICE_STATUS[4]++;
        if (sessionStatus[PUNCH]) {
            digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, LOW);
            digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, LOW);
            Serial.println("Punching");
        }
        else if (!sessionStatus[PUNCH]) {
            digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_pos, HIGH);
            digitalWrite(PUNCH_IMPACT_FEEDBACK_CONTROL_2_neg, HIGH);
        }
        sessionStatus[PUNCH] = !sessionStatus[PUNCH];
    }

    void toggleRBlockImpulseFeedback() {  
        if (sessionStatus[RBLOCK]) {
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos, LOW);
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg, LOW);
            Serial.println("Right arm blocking");
        }
        else if (!sessionStatus[RBLOCK]) {
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_pos, HIGH);
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_R_neg, HIGH);
        }
        sessionStatus[RBLOCK] = !sessionStatus[RBLOCK];
    }

    void toggleLBlockImpulseFeedback() {
        if (sessionStatus[LBLOCK]) {
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos, LOW);
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg, LOW);
            Serial.println("Left arm blocking");
        }
        else if (!sessionStatus[LBLOCK]) {
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_pos, HIGH);
            digitalWrite(BLOCK_IMPACT_FEEDBACK_CONTROL_L_neg, HIGH);
        }
        sessionStatus[LBLOCK] = !sessionStatus[LBLOCK];
    }

    void toggleChopImpulseFeedback() {
        if (sessionStatus[CHOP]) {
            digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_pos , LOW);
            digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_neg , LOW);
            Serial.println("Choping");
        }
        else if (!sessionStatus[CHOP]) {
            digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_pos , HIGH);
            digitalWrite(CHOP_IMPACT_FEEDBACK_CONTROL_1_neg , HIGH);
        }
        sessionStatus[CHOP] = !sessionStatus[CHOP];
    }

    

};
