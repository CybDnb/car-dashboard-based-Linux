/**
 * @file main.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "hal/hal.h"

#include "ui.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  const char * drm_card = "/dev/dri/card0";
  const char * evdev_input = "/dev/input/event1";

  if(argc > 1 && argv[1] && argv[1][0]) {
    drm_card = argv[1];
  }
  if(argc > 2 && argv[2] && argv[2][0]) {
    evdev_input = argv[2];
  }

  /*Initialize LVGL*/
  lv_init();

  /* Initialize DRM display and optional evdev input */
  LV_LOG_WARN("lvgl: Initializing DRM HAL");
  if(!drm_hal_init(drm_card, evdev_input)) {
    return 1;
  }
  
  lv_obj_t * obj = lv_label_create(lv_screen_active());
  lv_label_set_text(obj, "LVGL DRM OK");
  lv_obj_center(obj);
  ui_init();
  // lv_obj_invalidate(lv_screen_active());
  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    // LV_LOG_WARN("RUN\n");
    if(sleep_time_ms == LV_NO_TIMER_READY){
	    sleep_time_ms =  LV_DEF_REFR_PERIOD;
    }
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}


#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
