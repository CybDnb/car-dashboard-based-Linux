/**
 * @file lv_drivers.h
 *
 */

#ifndef LV_DRIVERS_H
#define LV_DRIVERS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_SDL
    #include "sdl/lv_sdl_window.h"
    #include "sdl/lv_sdl_mouse.h"
    #include "sdl/lv_sdl_mousewheel.h"
    #include "sdl/lv_sdl_keyboard.h"
#endif

#if LV_USE_X11
    #include "x11/lv_x11.h"
#endif

#if LV_USE_LINUX_DRM
    #include "display/drm/lv_linux_drm.h"
#endif
#if LV_USE_LINUX_FBDEV
    #include "display/fb/lv_linux_fbdev.h"
#endif

#if LV_USE_TFT_ESPI
    #include "display/tft_espi/lv_tft_espi.h"
#endif
#if LV_USE_LOVYAN_GFX
    #include "display/lovyan_gfx/lv_lovyan_gfx.h"
#endif

#if LV_USE_GENERIC_MIPI
    #include "display/lcd/lv_lcd_generic_mipi.h"
#endif
#if LV_USE_ILI9341
    #include "display/ili9341/lv_ili9341.h"
#endif
#if LV_USE_ST7735
    #include "display/st7735/lv_st7735.h"
#endif
#if LV_USE_ST7789
    #include "display/st7789/lv_st7789.h"
#endif
#if LV_USE_ST7796
    #include "display/st7796/lv_st7796.h"
#endif
#if LV_USE_NV3007
    #include "display/nv3007/lv_nv3007.h"
#endif

#if LV_USE_RENESAS_GLCDC
    #include "display/renesas_glcdc/lv_renesas_glcdc.h"
#endif
#if LV_USE_ST_LTDC
    #include "display/st_ltdc/lv_st_ltdc.h"
#endif
#if LV_USE_FT81X
    #include "display/ft81x/lv_ft81x.h"
#endif

#if LV_USE_DRAW_EVE
    #include "draw/eve/lv_draw_eve_display.h"
    #include "draw/eve/lv_draw_eve_display_defines.h"
#endif

#if LV_USE_NUTTX
    #include "nuttx/lv_nuttx_entry.h"
    #if LV_USE_NUTTX_LCD
        #include "nuttx/lv_nuttx_lcd.h"
    #endif
    #if LV_USE_NUTTX_TOUCHSCREEN
        #include "nuttx/lv_nuttx_touchscreen.h"
    #endif
    #if LV_USE_NUTTX_MOUSE
        #include "nuttx/lv_nuttx_fbdev.h"
    #endif
    #if LV_USE_NUTTX_LIBUV
        #include "nuttx/lv_nuttx_libuv.h"
    #endif
#endif

#if LV_USE_EVDEV
    #include "evdev/lv_evdev.h"
#endif
#if LV_USE_LIBINPUT
    #include "libinput/lv_libinput.h"
#endif

#if LV_USE_WINDOWS
    #include "windows/lv_windows_input.h"
    #include "windows/lv_windows_display.h"
#endif

#if LV_USE_OPENGLES
    #include "opengles/lv_opengles_window.h"
    #include "opengles/lv_opengles_texture.h"
    #include "opengles/lv_opengles_driver.h"
    #include "opengles/lv_opengles_glfw.h"
    #include "opengles/lv_opengles_egl.h"
#endif

#if LV_USE_QNX
    #include "qnx/lv_qnx.h"
#endif

#if LV_USE_WAYLAND
    #include "wayland/lv_wayland.h"
#endif

#if LV_USE_UEFI
    #include "uefi/lv_uefi_context.h"
    #include "uefi/lv_uefi_indev.h"
    #include "uefi/lv_uefi_display.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_DRIVERS_H*/
