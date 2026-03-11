#include <stdio.h>
#include "event_cb.h"

void switch_led_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * sw = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(sw, LV_STATE_CHECKED))
        {
            printf("LED ON\n");
        }
        else
        {
            printf("LED OFF\n");
        }
    }
}

void servo_button_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED)
    {
        printf("Servo button pressed\n");
    }
    else if(code == LV_EVENT_RELEASED)
    {
        printf("Servo button released\n");
    }
    else if(code == LV_EVENT_CLICKED)
    {
        printf("Servo button clicked\n");
    }
}