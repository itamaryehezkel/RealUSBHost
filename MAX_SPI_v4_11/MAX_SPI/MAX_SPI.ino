#include "RealUSBHost/MAX3421E.h"

USBHost usb(BACKLOG_CONDET | BACKLOG_USB_RAW | BACKLOG_PP_ENUMERATION);  

void setup() {
  startSerial(F("========PROGRAM START=============|\n")); //8   13+1
  //Serial.println(getString(HID_RDH_S_PHYSICAL_MIN));
  usb.init();
  
             // Serial.println(getString(HID_RDH_S_USAGEPAGE));
}

void loop() {
  usb.task();
}

/*
NO_BACKLOG        BACKLOG_REVISION
BACKLOG_ARDUINO   BACKLOG_MAX
BACKLOG_MALLOC    BACKLOG_CONDET
BACKLOG_USB_RAW   BACKLOG_PP_ENUMERATION
*/