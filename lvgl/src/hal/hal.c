#include "hal.h"


lv_display_t * drm_hal_init(const char * drm_card, const char * input_device)
{
    printf("Initializing DRM HAL\n");
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_linux_drm_create();
    if(!disp) return NULL;
    if(drm_card && drm_card[0])
    {
        printf("Using DRM card: %s\n", drm_card);
        if(lv_linux_drm_set_file(disp, drm_card, 448) != LV_RESULT_OK) {
            return NULL;
        }
    }


#if LV_USE_EVDEV
    if(input_device && input_device[0]) {
        printf("Using evdev input: %s\n", input_device);
        lv_indev_t * pointer = lv_evdev_create(LV_INDEV_TYPE_POINTER, input_device);
        if(pointer) {
            lv_indev_set_display(pointer, disp);
            lv_indev_set_group(pointer, lv_group_get_default());
        }
    }
#endif

    lv_display_set_default(disp);
    printf("DRM Resolution: %d x %d\n", lv_display_get_horizontal_resolution(disp), 
                                    lv_display_get_vertical_resolution(disp));
    return disp;
}
