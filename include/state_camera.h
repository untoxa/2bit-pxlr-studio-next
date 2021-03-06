#ifndef __STATE_CAMERA_H_INCLUDE__
#define __STATE_CAMERA_H_INCLUDE__

#include <gbdk/platform.h>
#include "gbcamera.h"
#include "globals.h"
#include "systemdetect.h"

BANKREF_EXTERN(state_camera)

typedef enum {
    camera_mode_manual,
    camera_mode_assisted,
    camera_mode_auto,
    camera_mode_iterate,
    N_CAMERA_MODES
} camera_mode_e;

typedef enum {
    trigger_mode_abutton,
    trigger_mode_timer,
    trigger_mode_interval,
    N_TRIGGER_MODES
} trigger_mode_e;

typedef enum {
    after_action_save,
    after_action_print,
    after_action_printsave,
    after_action_transfer,
    after_action_transfersave,
    after_action_picnrec,
    after_action_picnrec_video,
    N_AFTER_ACTIONS
} after_action_e;

typedef enum {
    idNone = 0,
    idExposure,
    idGain,
    idVOut,
    idContrast,
    idDither,
    idDitherLight,
    idInvOutput,
    idZeroPoint,
    idVoltageRef,
    idEdgeRatio,
    idEdgeExclusive,
    idEdgeOperation
} camera_menu_e;

typedef enum {
    changeNone = 0,
    changeDecrease,
    changeIncrease
} change_direction_e;

inline uint8_t inc_dec_int8(int8_t * value, int8_t delta, int8_t min, int8_t max, change_direction_e dir) {
    int8_t v = *value;
    switch (dir) {
        case changeDecrease:
            return (v != (*value = ((*value - delta) < min) ? min : (*value - delta)));
        case changeIncrease:
            return (v != (*value = ((*value + delta) > max) ? max : (*value + delta)));
        default:
            return FALSE;
    }
}
inline uint8_t inc_dec_int16(int16_t * value, int16_t delta, int16_t min, int16_t max, change_direction_e dir) {
    int16_t v = *value;
    switch (dir) {
        case changeDecrease:
            *value = ((*value - delta) < min) ? min : (*value - delta);
            break;
        case changeIncrease:
            *value = ((*value + delta) > max) ? max : (*value + delta);
            break;
    }
    return (v != *value);
}

typedef struct table_value_t {
    uint8_t value;
    const uint8_t * caption;
} table_value_t;

typedef struct camera_state_options_t {
    camera_mode_e camera_mode;
    trigger_mode_e trigger_mode;
    after_action_e after_action;
    uint8_t gallery_picture_idx;
    uint8_t print_frame_idx;
    uint8_t print_fast;
} camera_state_options_t;

#define OPTION(OPT) camera_state.OPT
extern camera_state_options_t camera_state;

typedef struct camera_mode_settings_t {
    int8_t current_exposure;
    int8_t current_gain;
    int8_t current_zero_point;
    int8_t current_edge_mode;
    int8_t current_voltage_ref;
    int16_t voltage_out;
    uint8_t dithering;
    uint8_t ditheringHighLight;
    uint8_t current_contrast;
    uint8_t invertOutput;
    uint8_t edge_exclusive;
    uint8_t edge_operation;
} camera_mode_settings_t;

#define MODE_SETTING(SET,STAT) current_settings[OPTION(STAT)].SET
#define SETTING(SET) MODE_SETTING(SET,camera_mode)
extern camera_mode_settings_t current_settings[N_CAMERA_MODES];

extern uint8_t recording_video;

extern uint8_t old_capture_reg;

inline uint8_t is_capturing() {
    SWITCH_RAM(CAMERA_BANK_REGISTERS);
    return (CAM_REG_CAPTURE & CAM00F_CAPTURING);
}
inline uint8_t image_captured() {
    SWITCH_RAM(CAMERA_BANK_REGISTERS);
    uint8_t v = CAM_REG_CAPTURE;
    uint8_t r = (((v ^ old_capture_reg) & CAM00F_CAPTURING) && !(v & CAM00F_CAPTURING));
    old_capture_reg = v;
    return r;
}
inline void image_capture() {
    SWITCH_RAM(CAMERA_BANK_REGISTERS);
    old_capture_reg = CAM_REG_CAPTURE = (CAM00F_POSITIVE | CAM00F_CAPTURING);
}


#endif
