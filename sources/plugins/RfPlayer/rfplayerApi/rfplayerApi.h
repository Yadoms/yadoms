//
// This file encapsulate the RFPlayer API
// Its aim is to make usb_frame_api.h compatible with Yadoms, without any modification
// of it (to facilitate evolution when new versions are avalaible).
//
// This file take part of Yadoms
// usb_frame_api.h is a ZiBlue furniture
//
// Usage : Yadoms must include this file, but never include directly usb_frame_api.h
//

// usb_frame_api.h as no pragme once (or equivalent #ifndef/define mechanism)
#pragma once

// IS_PACKED must be defined to compile with usb_frame_api.h
#define IS_PACKED
#include "usb_frame_api.h"
