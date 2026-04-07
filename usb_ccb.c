#include "usb_ccb.h"
#include <furi.h>
#include <furi_hal.h>
#include <notification/notification_messages.h>

#define TAG "UsbCcbApp"

uint32_t usb_ccb_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

UsbCcb* usb_ccb_app_alloc() {
    UsbCcb* app = malloc(sizeof(UsbCcb));

    // Gui
    app->gui = furi_record_open(RECORD_GUI);

    // Notifications
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // View dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Start view
    app->usb_ccb_start = usb_ccb_start_alloc();
    view_set_previous_callback(usb_ccb_start_get_view(app->usb_ccb_start), usb_ccb_exit);
    view_dispatcher_add_view(app->view_dispatcher, UsbCcbViewStart, usb_ccb_start_get_view(app->usb_ccb_start));

    view_dispatcher_switch_to_view(app->view_dispatcher, UsbCcbViewStart);

    return app;
}

void usb_ccb_app_free(UsbCcb* app) {
    furi_assert(app);

    // Reset notification
    notification_internal_message(app->notifications, &sequence_reset_blue);

    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, UsbCcbViewStart);
    usb_ccb_start_free(app->usb_ccb_start);
    view_dispatcher_free(app->view_dispatcher);
    // Close records
    furi_record_close(RECORD_GUI);
    app->gui = NULL;
    furi_record_close(RECORD_NOTIFICATION);
    app->notifications = NULL;

    // Free rest
    free(app);
}

int32_t usb_ccb_app(void* p) {
    UNUSED(p);
    // Switch profile to Hid
    UsbCcb* app = usb_ccb_app_alloc();

    FuriHalUsbInterface* usb_mode_prev = furi_hal_usb_get_config();
    furi_hal_usb_unlock();
    furi_check(furi_hal_usb_set_config(&usb_hid, NULL) == true);

    view_dispatcher_run(app->view_dispatcher);

    // Change back profile
    furi_hal_usb_set_config(usb_mode_prev, NULL);
    usb_ccb_app_free(app);

    return 0;
}