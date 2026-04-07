#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <notification/notification.h>

#include "views/usb_ccb_start.h"

typedef struct {
    Gui* gui;
    NotificationApp* notifications;
    ViewDispatcher* view_dispatcher;
    UsbCcbStart* usb_ccb_start;
} UsbCcb;

typedef enum {
    UsbCcbViewStart,
} UsbCcbView;
