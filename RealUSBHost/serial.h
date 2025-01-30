#ifndef RSERIAL_H

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}


void printUnder(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("_"));
}
void printComa(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F(","));
}
void printDash(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("-"));
}
void printTab(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("\t"));
}
void printSpace(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F(" "));
}

void printRoundOpen(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("("));
}


void printRoundClose(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F(")"));
}

void printPipe(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("|"));
}

void printMoreThan(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F(">"));
}

void printLessThan(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("<"));
}

void printColon(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F(":"));
}
void printDot(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("."));
}
void printPower(int amount){
  for(int i = 0; i < amount; i++)
    Serial.print(F("^"));
}

void startSerial(unsigned long baudRate, unsigned int startDelay, String print){
  if(baudRate)
    Serial.begin(baudRate);
  else
    Serial.begin(115200);
  while(!Serial);
  if(startDelay)
    delay(startDelay);  
  if(print.length() > 0)
    Serial.print(print);
  else
    Serial.println();
}

void startSerial(){
  startSerial(DEFAULT_BAUDRATE, DEFAULT_START_DELAY, DEFAULT_FIRST_PRINT);
}
void startSerial(String firstPrint){
  startSerial(DEFAULT_BAUDRATE, DEFAULT_START_DELAY, firstPrint);
}

void printHex(byte arr[], int size){
  for(int i = 0; i < size; i++){
    Serial.print(arr[i],HEX);
    if(i != size-1)
      printSpace(1);
  }
}

void printlnHex(byte arr[], int size){
  for(int i = 0; i < size; i++){
    Serial.print(arr[i],HEX);
    if(i != size-1)
      printSpace(1);
    else
      Serial.println(); 
  }
}
void printMainbmParser(int8_t value8){
  /*printRoundOpen(1);
  if( (value8 & 1) == 1)  //Constant
    Serial.print(F("Const"));
  else //Data
    Serial.print(F("Data"));
  printComa(1);
  if( (value8 & 2) == 2)  //Data type: Variable 
    Serial.print(F("Var"));
  else //Data type: Array
    Serial.print(F("Array"));
  printComa(1);
  if( (value8 & 4) == 4)  //Coordinates: Relative
    Serial.print(F("Rel"));
  else //Coordinates: Absolute
    Serial.print(F("Abs"));
  printRoundClose(1);
  */
  Serial.println();
}

void printString(int addr){
  int strLen = EEPROM.read(addr);
  if(addr == 0)
    strLen = 10;
  //Serial.println(strLen);
  char data[strLen + 1];
  for (int i = 0; i < strLen; i++){
    data[i] = EEPROM.read(addr + 1 + i);
    Serial.print(data[i]);
  }
}


void printUsage(uint16_t usagePage, uint16_t usage, boolean printUsage){
  switch(usagePage){
    case 0:
      Serial.println(F("Undefined"));
      break;
    case 1:
      if(printUsage)
        switch(usage){
          case 0:
            Serial.println(F("Undefined"));
            break;
          case 1:
            Serial.println(F("Pointer"));
            break;
          case 2:
            Serial.println(F("Mouse"));
            break;
          case 3:
            Serial.println(F("Reserved"));
            break;
          case 4:
            Serial.println(F("Joystick"));
            break;
          case 5:
            Serial.println(F("Gamepad"));
            break;
          case 6:
            Serial.println(F("Keyboard"));
            break;
          case 7:
            Serial.println(F("Keypad"));
            break;
          case 8:
            Serial.println(F("Multi-axis Controller"));
            break;
          case 9:
            Serial.println(F("Tablet PC System Controls"));
            break;
          case 0x0a:
            Serial.println(F("Water Cooling Device"));
            break;
          case 0x0b:
            Serial.println(F("Computer Chassis Device"));
            break;
          case 0x0c:
            Serial.println(F("Wireless Radio Controls"));
            break;
          case 0x0d:
            Serial.println(F("Portable Device Control"));
            break;
          case 0x0e:
            Serial.println(F("System Multi-Axis Controller"));
            break;
          case 0x0f:
            Serial.println(F("Spatial Controller"));
            break;
          case 0x10:
            Serial.println(F("Assistive Control"));
            break;
          case 0x11:
            Serial.println(F("Device Dock"));
            break;
          case 0x12:
            Serial.println(F("Dockable Device"));
            break;
          case 0x13:
            Serial.println(F("Call State Management Control"));
            break;
          case 0x30:
            Serial.println(F("X"));
            break;
          case 0x31:
            Serial.println(F("Y"));
            break;
          case 0x32:
            Serial.println(F("Z"));
            break;
          case 0x33:
            Serial.println(F("Rx"));
            break;
          case 0x34:
            Serial.println(F("Ry"));
            break;
          case 0x35:
            Serial.println(F("Rz"));
            break;
          case 0x36:
            Serial.println(F("Slider"));
            break;
          case 0x37:
            Serial.println(F("Dial"));
            break;
          case 0x38:
            Serial.println(F("Wheel"));
            break;
          case 0x39:
            Serial.println(F("Hat Switch"));
            break;
          case 0x3a:
            Serial.println(F("Counted Buffer"));
            break;
          case 0x3b:
            Serial.println(F("Byte Count"));
            break;
          case 0x3c:
            Serial.println(F("Motion Wakeup"));
            break;
          case 0x3d:
            Serial.println(F("Start"));
            break;
          case 0x3e:
            Serial.println(F("Select"));
            break;
          case 0x40:
            Serial.println(F("Vx"));
            break;
          case 0x41:
            Serial.println(F("Vy"));
            break;
          case 0x42:
            Serial.println(F("Vz"));
            break;
          case 0x43:
            Serial.println(F("Vbrx"));
            break;
          case 0x44:
            Serial.println(F("Vbry"));
            break;
          case 0x45:
            Serial.println(F("Vbrz"));
            break;
          case 0x46:
            Serial.println(F("Vno"));
            break;
          case 0x47:
            Serial.println(F("Feature Notification"));
            break;
          case 0x48:
            Serial.println(F("Resolution Multiplier"));
            break;
          case 0x49:
            Serial.println(F("Qx"));
            break;
          case 0x4a:
            Serial.println(F("Qy"));
            break;
          case 0x4b:
            Serial.println(F("Qz"));
            break;
          case 0x4c:
            Serial.println(F("Qw"));
            break;
          case 0x80:
            Serial.println(F("System Control"));
            break;
          case 0x81:
            Serial.println(F("System Power Down"));
            break;
          case 0x82:
            Serial.println(F("System Sleep"));
            break;
          case 0x83:
            Serial.println(F("System Wake Up"));
            break;
          case 0x84:
            Serial.println(F("System Context Menu"));
            break;
          case 0x85:
            Serial.println(F("System Main Menu"));
            break;
          case 0x86:
            Serial.println(F("System App Menu"));
            break;
          case 0x87:
            Serial.println(F("System Menu Help"));
            break;
          case 0x88:
            Serial.println(F("System Menu Exit"));
            break;
          case 0x89:
            Serial.println(F("System Menu Select"));
            break;
          case 0x8a:
            Serial.println(F("System Menu Right"));
            break;
          case 0x8b:
            Serial.println(F("System Menu Left"));
            break;
          case 0x8c:
            Serial.println(F("System Menu Up"));
            break;
          case 0x8d:
            Serial.println(F("System Menu Down"));
            break;
          case 0x8e:
            Serial.println(F("System Cold Restart"));
            break;
          case 0x8f:
            Serial.println(F("System Warm Restart"));
            break;
          case 0x90:
            Serial.println(F("D-pad Up"));
            break;
          case 0x91:
            Serial.println(F("D-pad Down"));
            break;
          case 0x92:
            Serial.println(F("D-pad Right"));
            break;
          case 0x93:
            Serial.println(F("D-pad Left"));
            break;
          case 0x94:
            Serial.println(F("Index Trigger"));
            break;
          case 0x95:
            Serial.println(F("Palm Trigger"));
            break;
          case 0x96:
            Serial.println(F("Thumbstick"));
            break;
          case 0x97:
            Serial.println(F("System Function Shift"));
            break;
          case 0x98:
            Serial.println(F("System Function Shift Lock"));
            break;
          case 0x99:
            Serial.println(F("System Function Shift Lock Indicator"));
            break;
          case 0x9a:
            Serial.println(F("System Dismiss Notification"));
            break;
          case 0x9b:
            Serial.println(F("System Do Not Disturb"));
            break;
          case 0xa0:
            Serial.println(F("System Dock"));
            break;
          case 0xa1:
            Serial.println(F("System Undock"));
            break;
          case 0xa2:
            Serial.println(F("System Setup"));
            break;
          case 0xa3:
            Serial.println(F("System Break"));
            break;
          case 0xa4:
            Serial.println(F("System Debugger Break"));
            break;
          case 0xa5:
            Serial.println(F("Application Break"));
            break;
          case 0xa6:
            Serial.println(F("Application Debugger Break"));
            break;
          case 0xa7:
            Serial.println(F("System Speaker Mute"));
            break;
          case 0xa8:
            Serial.println(F("System Hibernate"));
            break;
          case 0xa9:
            Serial.println(F("System Microphone Mute"));
            break;
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Generic Desktop"));
      break;
    case 2:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Simulation Controls"));
      break;
    case 3:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("VR Controls"));
      break;
    case 4:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Sport Controls"));
      break;
    case 5:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Game Controls"));
      break;
    case 6:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Generic Device Controls"));
      break;
    case 7:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Keyboard/Keypad"));
      break;
    case 8:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("LED"));
      break;
    case 9:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Button"));
      break;
    case 10:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Ordinal"));
      break;
    case 0x0b:
      if(printUsage)
        switch(usage){
          case 0x0000: 
            Serial.println(F("Undefined"));
            break;
          case 0x0001: 
            Serial.println(F("Phone"));
            break;
          case 0x0002: 
            Serial.println(F("Answering Machine"));
            break;
          case 0x0003: 
            Serial.println(F("Message Controls"));
            break;
          case 0x0004: 
            Serial.println(F("Handset"));
            break;
          case 0x0005: 
            Serial.println(F("Headset"));
            break;
          case 0x0006: 
            Serial.println(F("Telephony Key Pad"));
            break;
          case 0x0007: 
            Serial.println(F("Programmable Button"));
            break;
          case 0x0020: 
            Serial.println(F("Hook Switch"));
            break;
          case 0x0021: 
            Serial.println(F("Flash"));
            break;
          case 0x0022: 
            Serial.println(F("Feature"));
            break;
          case 0x0023: 
            Serial.println(F("Hold"));
            break;
          case 0x0024: 
            Serial.println(F("Redial"));
            break;
          case 0x0025: 
            Serial.println(F("Transfer"));
            break;
          case 0x0026: 
            Serial.println(F("Drop"));
            break;
          case 0x0027: 
            Serial.println(F("Park"));
            break;
          case 0x0028: 
            Serial.println(F("Forward Calls"));
            break;
          case 0x0029: 
            Serial.println(F("Alternate Function"));
            break;
          case 0x002A: 
            Serial.println(F("Line"));
            break;
          case 0x002B: 
            Serial.println(F("Speaker Phone"));
            break;
          case 0x002C: 
            Serial.println(F("Conference"));
            break;
          case 0x002D: 
            Serial.println(F("Ring Enable"));
            break;
          case 0x002E: 
            Serial.println(F("Ring Select"));
            break;
          case 0x002F: 
            Serial.println(F("Phone Mute"));
            break;
          case 0x0030: 
            Serial.println(F("Caller ID"));
            break;
          case 0x0031: 
            Serial.println(F("Send"));
            break;
          case 0x0050: 
            Serial.println(F("Speed Dial"));
            break;
          case 0x0051: 
            Serial.println(F("Store Number"));
            break;
          case 0x0052: 
            Serial.println(F("Recall Number"));
            break;
          case 0x0053: 
            Serial.println(F("Phone Directory"));
            break;
          case 0x0070: 
            Serial.println(F("Voice Mail"));
            break;
          case 0x0071: 
            Serial.println(F("Screen Calls"));
            break;
          case 0x0072: 
            Serial.println(F("Do Not Disturb"));
            break;
          case 0x0073: 
            Serial.println(F("Message"));
            break;
          case 0x0074: 
            Serial.println(F("Answer On/Off"));
            break;
          case 0x0090: 
            Serial.println(F("Inside Dial Tone"));
            break;
          case 0x0091: 
            Serial.println(F("Outside Dial Tone"));
            break;
          case 0x0092: 
            Serial.println(F("Inside Ring Tone"));
            break;
          case 0x0093: 
            Serial.println(F("Outside Ring Tone"));
            break;
          case 0x0094: 
            Serial.println(F("Priority Ring Tone"));
            break;
          case 0x0095: 
            Serial.println(F("Inside Ringback"));
            break;
          case 0x0096: 
            Serial.println(F("Priority Ringback"));
            break;
          case 0x0097: 
            Serial.println(F("Line Busy Tone"));
            break;
          case 0x0098: 
            Serial.println(F("Reorder Tone"));
            break;
          case 0x0099: 
            Serial.println(F("Call Waiting Tone"));
            break;
          case 0x009A: 
            Serial.println(F("Confirmation Tone 1"));
            break;
          case 0x009B: 
            Serial.println(F("Confirmation Tone 2"));
            break;
          case 0x009C: 
            Serial.println(F("Tones Off"));
            break;
          case 0x009D: 
            Serial.println(F("Outside Ringback"));
            break;
          case 0x009E: 
            Serial.println(F("Ringer"));
            break;
          case 0x00B0: 
            Serial.println(F("Phone Key 0"));
            break;
          case 0x00B1: 
            Serial.println(F("Phone Key 1"));
            break;
          case 0x00B2: 
            Serial.println(F("Phone Key 2"));
            break;
          case 0x00B3: 
            Serial.println(F("Phone Key 3"));
            break;
          case 0x00B4: 
            Serial.println(F("Phone Key 4"));
            break;
          case 0x00B5: 
            Serial.println(F("Phone Key 5"));
            break;
          case 0x00B6: 
            Serial.println(F("Phone Key 6"));
            break;
          case 0x00B7: 
            Serial.println(F("Phone Key 7"));
            break;
          case 0x00B8: 
            Serial.println(F("Phone Key 8"));
            break;
          case 0x00B9: 
            Serial.println(F("Phone Key 9"));
            break;
          case 0x00BA: 
            Serial.println(F("Phone Key Star"));
            break;
          case 0x00BB: 
            Serial.println(F("Phone Key Pound"));
            break;
          case 0x00BC: 
            Serial.println(F("Phone Key A"));
            break;
          case 0x00BD: 
            Serial.println(F("Phone Key B"));
            break;
          case 0x00BE: 
            Serial.println(F("Phone Key C"));
            break;
          case 0x00BF: 
            Serial.println(F("Phone Key D"));
            break;
          case 0x00C0: 
            Serial.println(F("Phone Call History Key"));
            break;
          case 0x00C1: 
            Serial.println(F("Phone Caller ID Key"));
            break;
          case 0x00C2: 
            Serial.println(F("Phone Settings Key"));
            break;
          case 0x00F0: 
            Serial.println(F("Host Control"));
            break;
          case 0x00F1: 
            Serial.println(F("Host Available"));
            break;
          case 0x00F2: 
            Serial.println(F("Host Call Active"));
            break;
          case 0x00F3: 
            Serial.println(F("Activate Handset Audio"));
            break;
          case 0x00F4: 
            Serial.println(F("Ring Type"));
            break;
          case 0x00F5: 
            Serial.println(F("Re-dialable Phone Number"));
            break;
          case 0x00F8: 
            Serial.println(F("Stop Ring Tone"));
            break;
          case 0x00F9: 
            Serial.println(F("PSTN Ring Tone"));
            break;
          case 0x00FA: 
            Serial.println(F("Host Ring Tone"));
            break;
          case 0x00FB: 
            Serial.println(F("Alert Sound Error"));
            break;
          case 0x00FC: 
            Serial.println(F("Alert Sound Confirm"));
            break;
          case 0x00FD: 
            Serial.println(F("Alert Sound Notification"));
            break;
          case 0x00FE: 
            Serial.println(F("Silent Ring"));
            break;
          case 0x0108: 
            Serial.println(F("Email Message Waiting"));
            break;
          case 0x0109: 
            Serial.println(F("Voicemail Message Waiting"));
            break;
          case 0x010A: 
            Serial.println(F("Host Hold"));
            break;
          case 0x0110: 
            Serial.println(F("Incoming Call History Count"));
            break;
          case 0x0111: 
            Serial.println(F("Outgoing Call History Count"));
            break;
          case 0x0112: 
            Serial.println(F("Incoming Call History"));
            break;
          case 0x0113: 
            Serial.println(F("Outgoing Call History"));
            break;
          case 0x0114: 
            Serial.println(F("Phone Locale"));
            break;
          case 0x0140: 
            Serial.println(F("Phone Time Second"));
            break;
          case 0x0141: 
            Serial.println(F("Phone Time Minute"));
            break;
          case 0x0142: 
            Serial.println(F("Phone Time Hour"));
            break;
          case 0x0143: 
            Serial.println(F("Phone Date Day"));
            break;
          case 0x0144: 
            Serial.println(F("Phone Date Month"));
            break;
          case 0x0145: 
            Serial.println(F("Phone Date Year"));
            break;
          case 0x0146: 
            Serial.println(F("Handset Nickname"));
            break;
          case 0x0147: 
            Serial.println(F("Address Book ID"));
            break;
          case 0x014A: 
            Serial.println(F("Call Duration"));
            break;
          case 0x014B: 
            Serial.println(F("Dual Mode Phone"));
            break;
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Telephony Device"));
      break;
    case 0x0c:
      if(printUsage)
        switch(usage){
          case 0x0000: 
            Serial.println(F("Undefined"));
            break;
          case 0x0001: 
            Serial.println(F("Consumer Control"));
            break;
          case 0x0002: 
            Serial.println(F("Numeric Key Pad"));
            break;
          case 0x0003: 
            Serial.println(F("Programmable Buttons"));
            break;
          case 0x0004: 
            Serial.println(F("Microphone"));
            break;
          case 0x0005: 
            Serial.println(F("Headphone"));
            break;
          case 0x0006: 
            Serial.println(F("Graphic Equalizer"));
            break;
          case 0x0020: 
            Serial.println(F("+10"));
            break;
          case 0x0021: 
            Serial.println(F("+100"));
            break;
          case 0x0022: 
            Serial.println(F("AM/PM"));
            break;
          case 0x0030: 
            Serial.println(F("Power"));
            break;
          case 0x0031: 
            Serial.println(F("Reset"));
            break;
          case 0x0032: 
            Serial.println(F("Sleep"));
            break;
          case 0x0033: 
            Serial.println(F("Sleep After"));
            break;
          case 0x0034: 
            Serial.println(F("Sleep Mode"));
            break;
          case 0x0035: 
            Serial.println(F("Illumination"));
            break;
          case 0x0036: 
            Serial.println(F("Function Buttons"));
            break;
          case 0x0040: 
            Serial.println(F("Menu"));
            break;
          case 0x0041: 
            Serial.println(F("Menu Pick"));
            break;
          case 0x0042: 
            Serial.println(F("Menu Up"));
            break;
          case 0x0043: 
            Serial.println(F("Menu Down"));
            break;
          case 0x0044: 
            Serial.println(F("Menu Left"));
            break;
          case 0x0045: 
            Serial.println(F("Menu Right"));
            break;
          case 0x0046: 
            Serial.println(F("Menu Escape"));
            break;
          case 0x0047: 
            Serial.println(F("Menu Value Increase"));
            break;
          case 0x0048: 
            Serial.println(F("Menu Value Decrease"));
            break;
          case 0x0060: 
            Serial.println(F("Data On Screen"));
            break;
          case 0x0061: 
            Serial.println(F("Closed Caption"));
            break;
          case 0x0062: 
            Serial.println(F("Closed Caption Select"));
            break;
          case 0x0063: 
            Serial.println(F("VCR/TV"));
            break;
          case 0x0064: 
            Serial.println(F("Broadcast Mode"));
            break;
          case 0x0065: 
            Serial.println(F("Snapshot"));
            break;
          case 0x0066: 
            Serial.println(F("Still"));
            break;
          case 0x0067: 
            Serial.println(F("Picture-in-Picture Toggle"));
            break;
          case 0x0068: 
            Serial.println(F("Picture-in-Picture Swap"));
            break;
          case 0x0069: 
            Serial.println(F("Red Menu Button"));
            break;
          case 0x006A: 
            Serial.println(F("Green Menu Button"));
            break;
          case 0x006B: 
            Serial.println(F("Blue Menu Button"));
            break;
          case 0x006C: 
            Serial.println(F("Yellow Menu Button"));
            break;
          case 0x006D: 
            Serial.println(F("Aspect"));
            break;
          case 0x006E: 
            Serial.println(F("3D Mode Select"));
            break;
          case 0x006F: 
            Serial.println(F("Display Brightness Increment"));
            break;
          case 0x0070: 
            Serial.println(F("Display Brightness Decrement"));
            break;
          case 0x0071: 
            Serial.println(F("Display Brightness"));
            break;
          case 0x0072: 
            Serial.println(F("Display Backlight Toggle"));
            break;
          case 0x0073: 
            Serial.println(F("Display Set Brightness to Minimum"));
            break;
          case 0x0074: 
            Serial.println(F("Display Set Brightness to Maximum"));
            break;
          case 0x0075: 
            Serial.println(F("Display Set Auto Brightness"));
            break;
          case 0x0076: 
            Serial.println(F("Camera Access Enabled"));
            break;
          case 0x0077: 
            Serial.println(F("Camera Access Disabled"));
            break;
          case 0x0078: 
            Serial.println(F("Camera Access Toggle"));
            break;
          case 0x0079: 
            Serial.println(F("Keyboard Brightness Increment"));
            break;
          case 0x007A: 
            Serial.println(F("Keyboard Brightness Decrement"));
            break;
          case 0x007B: 
            Serial.println(F("Keyboard Backlight Set Level"));
            break;
          case 0x007C: 
            Serial.println(F("Keyboard Backlight"));
            break;
          case 0x007D: 
            Serial.println(F("Keyboard Backlight Set Minimum"));
            break;
          case 0x007E: 
            Serial.println(F("Keyboard Backlight Set Maximum"));
            break;
          case 0x007F: 
            Serial.println(F("Keyboard Backlight Auto"));
            break;
          case 0x0080: 
            Serial.println(F("Selection"));
            break;
          case 0x0081: 
            Serial.println(F("Assign Selection"));
            break;
          case 0x0082: 
            Serial.println(F("Mode Step"));
            break;
          case 0x0083: 
            Serial.println(F("Recall Last"));
            break;
          case 0x0084: 
            Serial.println(F("Enter Channel"));
            break;
          case 0x0085: 
            Serial.println(F("Order Movie"));
            break;
          case 0x0086: 
            Serial.println(F("Channel"));
            break;
          case 0x0087: 
            Serial.println(F("Media Selection"));
            break;
          case 0x0088: 
            Serial.println(F("Media Select Computer"));
            break;
          case 0x0089: 
            Serial.println(F("Media Select TV"));
            break;
          case 0x008A: 
            Serial.println(F("Media Select WWW"));
            break;
          case 0x008B: 
            Serial.println(F("Media Select DVD"));
            break;
          case 0x008C: 
            Serial.println(F("Media Select Telephone"));
            break;
          case 0x008D: 
            Serial.println(F("Media Select Program Guide"));
            break;
          case 0x008E: 
            Serial.println(F("Media Select Video Phone"));
            break;
          case 0x008F: 
            Serial.println(F("Media Select Games"));
            break;
          case 0x0090: 
            Serial.println(F("Media Select Messages"));
            break;
          case 0x0091: 
            Serial.println(F("Media Select CD"));
            break;
          case 0x0092: 
            Serial.println(F("Media Select VCR"));
            break;
          case 0x0093: 
            Serial.println(F("Media Select Tuner"));
            break;
          case 0x0094: 
            Serial.println(F("Quit"));
            break;
          case 0x0095: 
            Serial.println(F("Help"));
            break;
          case 0x0096: 
            Serial.println(F("Media Select Tape"));
            break;
          case 0x0097: 
            Serial.println(F("Media Select Cable"));
            break;
          case 0x0098: 
            Serial.println(F("Media Select Satellite"));
            break;
          case 0x0099: 
            Serial.println(F("Media Select Security"));
            break;
          case 0x009A: 
            Serial.println(F("Media Select Home"));
            break;
          case 0x009B: 
            Serial.println(F("Media Select Call"));
            break;
          case 0x009C: 
            Serial.println(F("Channel Increment"));
            break;
          case 0x009D: 
            Serial.println(F("Channel Decrement"));
            break;
          case 0x009E: 
            Serial.println(F("Media Select SAP"));
            break;
          case 0x00A0: 
            Serial.println(F("VCR Plus"));
            break;
          case 0x00A1: 
            Serial.println(F("Once"));
            break;
          case 0x00A2: 
            Serial.println(F("Daily"));
            break;
          case 0x00A3: 
            Serial.println(F("Weekly"));
            break;
          case 0x00A4: 
            Serial.println(F("Monthly"));
            break;
          case 0x00B0: 
            Serial.println(F("Play"));
            break;
          case 0x00B1: 
            Serial.println(F("Pause"));
            break;
          case 0x00B2: 
            Serial.println(F("Record"));
            break;
          case 0x00B3: 
            Serial.println(F("Fast Forward"));
            break;
          case 0x00B4: 
            Serial.println(F("Rewind"));
            break;
          case 0x00B5: 
            Serial.println(F("Scan Next Track"));
            break;
          case 0x00B6: 
            Serial.println(F("Scan Previous Track"));
            break;
          case 0x00B7: 
            Serial.println(F("Stop"));
            break;
          case 0x00B8: 
            Serial.println(F("Eject"));
            break;
          case 0x00B9: 
            Serial.println(F("Random Play"));
            break;
          case 0x00BA: 
            Serial.println(F("Select Disc"));
            break;
          case 0x00BB: 
            Serial.println(F("Enter Disc"));
            break;
          case 0x00BC: 
            Serial.println(F("Repeat"));
            break;
          case 0x00BD: 
            Serial.println(F("Tracking"));
            break;
          case 0x00BE: 
            Serial.println(F("Track Normal"));
            break;
          case 0x00BF: 
            Serial.println(F("Slow Tracking"));
            break;
          case 0x00C0: 
            Serial.println(F("Frame Forward"));
            break;
          case 0x00C1: 
            Serial.println(F("Frame Back"));
            break;
          case 0x00C2: 
            Serial.println(F("Mark"));
            break;
          case 0x00C3: 
            Serial.println(F("Clear Mark"));
            break;
          case 0x00C4: 
            Serial.println(F("Repeat From Mark"));
            break;
          case 0x00C5: 
            Serial.println(F("Return To Mark"));
            break;
          case 0x00C6: 
            Serial.println(F("Search Mark Forward"));
            break;
          case 0x00C7: 
            Serial.println(F("Search Mark Backwards"));
            break;
          case 0x00C8: 
            Serial.println(F("Counter Reset"));
            break;
          case 0x00C9: 
            Serial.println(F("Show Counter"));
            break;
          case 0x00CA: 
            Serial.println(F("Tracking Increment"));
            break;
          case 0x00CB: 
            Serial.println(F("Tracking Decrement"));
            break;
          case 0x00CC: 
            Serial.println(F("Stop/Eject"));
            break;
          case 0x00CD: 
            Serial.println(F("Play/Pause"));
            break;
          case 0x00CE: 
            Serial.println(F("Play/Skip"));
            break;
          case 0x00CF: 
            Serial.println(F("Voice Command"));
            break;
          case 0x00D0: 
            Serial.println(F("Invoke Capture Interface"));
            break;
          case 0x00D1: 
            Serial.println(F("Start or Stop Game Recording"));
            break;
          case 0x00D2: 
            Serial.println(F("Historical Game Capture"));
            break;
          case 0x00D3: 
            Serial.println(F("Capture Game Screenshot"));
            break;
          case 0x00D4: 
            Serial.println(F("Show or Hide Recording Indicator"));
            break;
          case 0x00D5: 
            Serial.println(F("Start or Stop Microphone Capture"));
            break;
          case 0x00D6: 
            Serial.println(F("Start or Stop Camera Capture"));
            break;
          case 0x00D7: 
            Serial.println(F("Start or Stop Game Broadcast"));
            break;
          case 0x00D8: 
            Serial.println(F("Start or Stop Voice Dictation Session"));
            break;
          case 0x00D9: 
            Serial.println(F("Invoke/Dismiss Emoji Picker"));
            break;
          case 0x00E0: 
            Serial.println(F("Volume"));
            break;
          case 0x00E1: 
            Serial.println(F("Balance"));
            break;
          case 0x00E2: 
            Serial.println(F("Mute"));
            break;
          case 0x00E3: 
            Serial.println(F("Bass"));
            break;
          case 0x00E4: 
            Serial.println(F("Treble"));
            break;
          case 0x00E5: 
            Serial.println(F("Bass Boost"));
            break;
          case 0x00E6: 
            Serial.println(F("Surround Mode"));
            break;
          case 0x00E7: 
            Serial.println(F("Loudness"));
            break;
          case 0x00E8: 
            Serial.println(F("MPX"));
            break;
          case 0x00E9: 
            Serial.println(F("Volume Increment"));
            break;
          case 0x00EA: 
            Serial.println(F("Volume Decrement"));
            break;
          case 0x00F0: 
            Serial.println(F("Speed Select"));
            break;
          case 0x00F1: 
            Serial.println(F("Playback Speed"));
            break;
          case 0x00F2: 
            Serial.println(F("Standard Play"));
            break;
          case 0x00F3: 
            Serial.println(F("Long Play"));
            break;
          case 0x00F4: 
            Serial.println(F("Extended Play"));
            break;
          case 0x00F5: 
            Serial.println(F("Slow"));
            break;
          case 0x0100: 
            Serial.println(F("Fan Enable"));
            break;
          case 0x0101: 
            Serial.println(F("Fan Speed"));
            break;
          case 0x0102: 
            Serial.println(F("Light Enable"));
            break;

          case 0x0103: 
            Serial.println(F("Light Illumination Level"));
            break;
          case 0x0104: 
            Serial.println(F("Climate Control Enable"));
            break;
          case 0x0105: 
            Serial.println(F("Room Temperature"));
            break;
          case 0x0106: 
            Serial.println(F("Security Enable"));
            break;
          case 0x0107: 
            Serial.println(F("Fire Alarm"));
            break;
          case 0x0108: 
            Serial.println(F("Police Alarm"));
            break;
          case 0x0109: 
            Serial.println(F("Proximity"));
            break;
          case 0x010A: 
            Serial.println(F("Motion"));
            break;
          case 0x010B: 
            Serial.println(F("Duress Alarm"));
            break;
          case 0x010C: 
            Serial.println(F("Holdup Alarm"));
            break;
          case 0x010D: 
            Serial.println(F("Medical Alarm"));
            break;

          case 0x0150: 
            Serial.println(F("Balance Right"));
            break;
          case 0x0151: 
            Serial.println(F("Balance Left"));
            break;
          case 0x0152: 
            Serial.println(F("Bass Increment"));
            break;
          case 0x0153: 
            Serial.println(F("Bass Decrement"));
            break;
          case 0x0154: 
            Serial.println(F("Treble Increment"));
            break;
          case 0x0155: 
            Serial.println(F("Treble Decrement"));
            break;
          case 0x0160: 
            Serial.println(F("Speaker System"));
            break;
          case 0x0161: 
            Serial.println(F("Channel Left"));
            break;
          case 0x0162: 
            Serial.println(F("Channel Right"));
            break;
          case 0x0163: 
            Serial.println(F("Channel Center"));
            break;
          case 0x0164: 
            Serial.println(F("Channel Front"));
            break;
          case 0x0165: 
            Serial.println(F("Channel Center Front"));
            break;
          case 0x0166: 
            Serial.println(F("Channel Side"));
            break;
          case 0x0167: 
            Serial.println(F("Channel Surround"));
            break;
          case 0x0168: 
            Serial.println(F("Channel Low Frequency Enhancement"));
            break;
          case 0x0169: 
            Serial.println(F("Channel Top"));
            break;
          case 0x016A: 
            Serial.println(F("Channel Unknown"));
            break;
          case 0x0170: 
            Serial.println(F("Sub-channel"));
            break;
          case 0x0171: 
            Serial.println(F("Sub-channel Increment"));
            break;
          case 0x0172: 
            Serial.println(F("Sub-channel Decrement"));
            break;
          case 0x0173: 
            Serial.println(F("Alternate Audio Increment"));
            break;
          case 0x0174: 
            Serial.println(F("Alternate Audio Decrement"));
            break;
          case 0x0180: 
            Serial.println(F("Application Launch Buttons"));
            break;
            /*
          case 0x0181: 
            Serial.println(F("AL Launch Button Configuration Tool"));
            break;
          case 0x0182: 
            Serial.println(F("AL Programmable Button Configuration"));
            break;
          case 0x0183: 
            Serial.println(F("AL Consumer Control Configuration"));
            break;
          case 0x0184: 
            Serial.println(F("AL Word Processor"));
            break;
          case 0x0185: 
            Serial.println(F("AL Text Editor"));
            break;
          case 0x0186: 
            Serial.println(F("AL Spreadsheet"));
            break;
          case 0x0187: 
            Serial.println(F("AL Graphics Editor"));
            break;
          case 0x0188: 
            Serial.println(F("AL Presentation App"));
            break;
          case 0x0189: 
            Serial.println(F("AL Database App"));
            break;
          case 0x018A: 
            Serial.println(F("AL Email Reader"));
            break;
          case 0x018B: 
            Serial.println(F("AL Newsreader"));
            break;
          case 0x018C: 
            Serial.println(F("AL Voicemail"));
            break;
          case 0x018D: 
            Serial.println(F("AL Contacts/Address Book"));
            break;
          case 0x018E: 
            Serial.println(F("AL Calendar/Schedule"));
            break;
          case 0x018F: 
            Serial.println(F("AL Task/Project Manager"));
            break;
          case 0x0190: 
            Serial.println(F("AL Log/Journal/Timecard"));
            break;
          case 0x0191: 
            Serial.println(F("AL Checkbook/Finance"));
            break;
          case 0x0192: 
            Serial.println(F("AL Calculator"));
            break;
          case 0x0193: 
            Serial.println(F("AL A/V Capture/Playback"));
            break;
          case 0x0194: 
            Serial.println(F("AL Local Machine Browser"));
            break;
          case 0x0195: 
            Serial.println(F("AL LAN/WAN Browser"));
            break;
          case 0x0196: 
            Serial.println(F("AL Internet Browser"));
            break;
          case 0x0197: 
            Serial.println(F("AL Remote Networking/ISP Connect"));
            break;
          case 0x0198: 
            Serial.println(F("AL Network Conference"));
            break;
          case 0x0199: 
            Serial.println(F("AL Network Chat"));
            break;
          case 0x019A: 
            Serial.println(F("AL Telephony/Dialer"));
            break;
          case 0x019B: 
            Serial.println(F("AL Logon"));
            break;
          case 0x019C: 
            Serial.println(F("AL Logoff"));
            break;
          case 0x019D: 
            Serial.println(F("AL Logon/Logoff"));
            break;
          case 0x019E: 
            Serial.println(F("AL Terminal Lock/Screensaver"));
            break;
          case 0x019F: 
            Serial.println(F("AL Control Panel"));
            break;
          case 0x01A0: 
            Serial.println(F("AL Command Line Processor/Run"));
            break;
          case 0x01A1: 
            Serial.println(F("AL Process/Task Manager"));
            break;
          case 0x01A2: 
            Serial.println(F("AL Select Task/Application"));
            break;
          case 0x01A3: 
            Serial.println(F("AL Next Task/Application"));
            break;
          case 0x01A4: 
            Serial.println(F("AL Previous Task/Application"));
            break;
          case 0x01A5: 
            Serial.println(F("AL Preemptive Halt Task/Application"));
            break;
          case 0x01A6: 
            Serial.println(F("AL Integrated Help Center"));
            break;
          case 0x01A7: 
            Serial.println(F("AL Documents"));
            break;
          case 0x01A8: 
            Serial.println(F("AL Thesaurus"));
            break;
          case 0x01A9: 
            Serial.println(F("AL Dictionary"));
            break;
          case 0x01AA: 
            Serial.println(F("AL Desktop"));
            break;
          case 0x01AB: 
            Serial.println(F("AL Spell Check"));
            break;
          case 0x01AC: 
            Serial.println(F("AL Grammar Check"));
            break;
          case 0x01AD: 
            Serial.println(F("AL Wireless Status"));
            break;
          case 0x01AE: 
            Serial.println(F("AL Keyboard Layout"));
            break;
          case 0x01AF: 
            Serial.println(F("AL Virus Protection"));
            break;
          case 0x1B0: 
            Serial.println(F("AL Encryption"));
            break;
          case 0x1B1: 
            Serial.println(F("AL Screen Saver"));
            break;
          case 0x1B2: 
            Serial.println(F("AL Alarms"));
            break;
          case 0x1B3: 
            Serial.println(F("AL Clock"));
            break;
          case 0x1B4: 
            Serial.println(F("AL File Browser"));
            break;
          case 0x1B5: 
            Serial.println(F("AL Power Status"));
            break;
          case 0x1B6: 
            Serial.println(F("AL Image Browser"));
            break;
          case 0x1B7: 
            Serial.println(F("AL Audio Browser"));
            break;
          case 0x1B8: 
            Serial.println(F("AL Movie Browser"));
            break;
          case 0x1B9: 
            Serial.println(F("AL Digital Rights Manager"));
            break;
          case 0x1BA: 
            Serial.println(F("AL Digital Wallet"));
            break;
          case 0x1BC: 
            Serial.println(F("AL Instant Messaging"));
            break;
          case 0x1BD: 
            Serial.println(F("AL OEM Features/ Tips/Tutorial Browser"));
            break;
          case 0x1BE: 
            Serial.println(F("AL OEM Help"));
            break;
          case 0x1BF: 
            Serial.println(F("AL Online Community"));
            break;
          case 0x1C0: 
            Serial.println(F("AL Entertainment Content Browser"));
            break;
          case 0x1C1: 
            Serial.println(F("AL Online Shopping Browser"));
            break;
          case 0x1C2: 
            Serial.println(F("AL SmartCard Information/Help"));
            break;
          case 0x1C3: 
            Serial.println(F("AL Market Monitor/Finance Browser"));
            break;
          case 0x1C4: 
            Serial.println(F("AL Customized Corporate News Browser"));
            break;
          case 0x1C5: 
            Serial.println(F("AL Online Activity Browser"));
            break;
          case 0x1C6: 
            Serial.println(F("AL Research/Search Browser"));
            break;
          case 0x1C7: 
            Serial.println(F("AL Audio Player"));
            break;
          case 0x1C8: 
            Serial.println(F("AL Message Status"));
            break;
          case 0x1C9: 
            Serial.println(F("AL Contact Sync"));
            break;
          case 0x1CA: 
            Serial.println(F("AL Navigation"));
            break;
          case 0x1CB: 
            Serial.println(F("AL Context-aware Desktop Assistant"));
            break;
            //*/  
          case 0x200: 
            Serial.println(F("Generic GUI Application Controls"));
            break;
            /*
          case 0x201: 
            Serial.println(F("AC New"));
            break;
          case 0x202: 
            Serial.println(F("AC Open"));
            break;
          case 0x203: 
            Serial.println(F("AC Close"));
            break;
          case 0x204: 
            Serial.println(F("AC Exit"));
            break;
          case 0x205: 
            Serial.println(F("AC Maximize"));
            break;
          case 0x206: 
            Serial.println(F("AC Minimize"));
            break;
          case 0x207: 
            Serial.println(F("AC Save"));
            break;
          case 0x208: 
            Serial.println(F("AC Print"));
            break;
          case 0x209: 
            Serial.println(F("AC Properties"));
            break;
          case 0x21A: 
            Serial.println(F("AC Undo"));
            break;
          case 0x21B: 
            Serial.println(F("AC Copy"));
            break;
          case 0x21C: 
            Serial.println(F("AC Cut"));
            break;
          case 0x21D: 
            Serial.println(F("AC Paste"));
            break;
          case 0x21E: 
            Serial.println(F("AC Select All"));
            break;
          case 0x21F: 
            Serial.println(F("AC Find"));
            break;
          case 0x220: 
            Serial.println(F("AC Find and Replace"));
            break;
          case 0x221: 
            Serial.println(F("AC Search"));
            break;
          case 0x222: 
            Serial.println(F("AC Go To"));
            break;
          case 0x223: 
            Serial.println(F("AC Home"));
            break;
          case 0x224: 
            Serial.println(F("AC Back"));
            break;
          case 0x225: 
            Serial.println(F("AC Forward"));
            break;
          case 0x226: 
            Serial.println(F("AC Stop"));
            break;
          case 0x227: 
            Serial.println(F("AC Refresh"));
            break;
          case 0x228: 
            Serial.println(F("AC Previous Link"));
            break;
          case 0x229: 
            Serial.println(F("AC Next Link"));
            break;
          case 0x22A: 
            Serial.println(F("AC Bookmarks"));
            break;
          case 0x22B: 
            Serial.println(F("AC History"));
            break;
          case 0x22C: 
            Serial.println(F("AC Subscriptions"));
            break;
          case 0x22D: 
            Serial.println(F("AC Zoom In"));
            break;
          case 0x22E: 
            Serial.println(F("AC Zoom Out"));
            break;
          case 0x22F: 
            Serial.println(F("AC Zoom"));
            break;
          case 0x230: 
            Serial.println(F("AC Full Screen View"));
            break;
          case 0x231: 
            Serial.println(F("AC Normal View"));
            break;
          case 0x232: 
            Serial.println(F("AC View Toggle"));
            break;
          case 0x233: 
            Serial.println(F("AC Scroll Up"));
            break;
          case 0x234: 
            Serial.println(F("AC Scroll Down"));
            break;
          case 0x235: 
            Serial.println(F("AC Scroll"));
            break;
          case 0x236: 
            Serial.println(F("AC Pan Left"));
            break;
          case 0x237: 
            Serial.println(F("AC Pan Right"));
            break;
          case 0x238: 
            Serial.println(F("AC Pan"));
            break;
          case 0x239: 
            Serial.println(F("AC New Window"));
            break;
          case 0x23A: 
            Serial.println(F("AC Tile Horizontally"));
            break;
          case 0x23B: 
            Serial.println(F("AC Tile Vertically"));
            break;
          case 0x23C: 
            Serial.println(F("AC Format"));
            break;
          case 0x23D: 
            Serial.println(F("AC Edit"));
            break;
          case 0x23E: 
            Serial.println(F("AC Bold"));
            break;
          case 0x23F: 
            Serial.println(F("AC Italics"));
            break;
          case 0x240: 
            Serial.println(F("AC Underline"));
            break;
          case 0x241: 
            Serial.println(F("AC Strikethrough"));
            break;
          case 0x242: 
            Serial.println(F("AC Subscript"));
            break;
          case 0x243: 
            Serial.println(F("AC Superscript"));
            break;
          case 0x244: 
            Serial.println(F("AC All Caps"));
            break;
          case 0x245: 
            Serial.println(F("AC Rotate"));
            break;
          case 0x246: 
            Serial.println(F("AC Resize"));
            break;
          case 0x247: 
            Serial.println(F("AC Flip Horizontal"));
            break;
          case 0x248: 
            Serial.println(F("AC Flip Vertical"));
            break;
          case 0x249: 
            Serial.println(F("AC Mirror Horizontal"));
            break;
          case 0x24A: 
            Serial.println(F("AC Mirror Vertical"));
            break;
          case 0x24B: 
            Serial.println(F("AC Font Select"));
            break;
          case 0x24C: 
            Serial.println(F("AC Font Color"));
            break;
          case 0x24D: 
            Serial.println(F("AC Font Size"));
            break;
          case 0x24E: 
            Serial.println(F("AC Justify Left"));
            break;
          case 0x24F: 
            Serial.println(F("AC Justify Center H"));
            break;
          case 0x250: 
            Serial.println(F("AC Justify Right"));
            break;
          case 0x251: 
            Serial.println(F("AC Justify Block H"));
            break;
          case 0x252: 
            Serial.println(F("AC Justify Top"));
            break;
          case 0x253: 
            Serial.println(F("AC Justify Center V"));
            break;
          case 0x254: 
            Serial.println(F("AC Justify Bottom"));
            break;
          case 0x255: 
            Serial.println(F("AC Justify Block V"));
            break;
          case 0x256: 
            Serial.println(F("AC Indent Decrease"));
            break;
          case 0x257: 
            Serial.println(F("AC Indent Increase"));
            break;
          case 0x258: 
            Serial.println(F("AC Numbered List"));
            break;
          case 0x259: 
            Serial.println(F("AC Restart Numbering"));
            break;
          case 0x25A: 
            Serial.println(F("AC Bulleted List"));
            break;
          case 0x25B: 
            Serial.println(F("AC Promote"));
            break;
          case 0x25C: 
            Serial.println(F("AC Demote"));
            break;
          case 0x25D: 
            Serial.println(F("AC Yes"));
            break;
          case 0x25E: 
            Serial.println(F("AC No"));
            break;
          case 0x25F: 
            Serial.println(F("AC Cancel"));
            break;
          case 0x260: 
            Serial.println(F("AC Catalog"));
            break;
          case 0x261: 
            Serial.println(F("AC Buy/Checkout"));
            break;
          case 0x262: 
            Serial.println(F("AC Add to Cart"));
            break;
          case 0x263: 
            Serial.println(F("AC Expand"));
            break;
          case 0x264: 
            Serial.println(F("AC Expand All"));
            break;
          case 0x265: 
            Serial.println(F("AC Collapse"));
            break;
          case 0x266: 
            Serial.println(F("AC Collapse All"));
            break;
          case 0x267: 
            Serial.println(F("AC Print Preview"));
            break;
          case 0x268: 
            Serial.println(F("AC Paste Special"));
            break;
          case 0x269: 
            Serial.println(F("AC Insert Mode"));
            break;
          case 0x26A: 
            Serial.println(F("AC Delete"));
            break;
          case 0x26B: 
            Serial.println(F("AC Lock"));
            break;
          case 0x26C: 
            Serial.println(F("AC Unlock"));
            break;
          case 0x26D: 
            Serial.println(F("AC Protect"));
            break;
          case 0x26E: 
            Serial.println(F("AC Unprotect"));
            break;
          case 0x26F: 
            Serial.println(F("AC Attach Comment"));
            break;
          case 0x270: 
            Serial.println(F("AC Delete Comment"));
            break;
          case 0x271: 
            Serial.println(F("AC View Comment"));
            break;
          case 0x272: 
            Serial.println(F("AC Select Word"));
            break;
          case 0x273: 
            Serial.println(F("AC Select Sentence"));
            break;
          case 0x274: 
            Serial.println(F("AC Select Paragraph"));
            break;
          case 0x275: 
            Serial.println(F("AC Select Column"));
            break;
          case 0x276: 
            Serial.println(F("AC Select Row"));
            break;
          case 0x277: 
            Serial.println(F("AC Select Table"));
            break;
          case 0x278: 
            Serial.println(F("AC Select Object"));
            break;
          case 0x279: 
            Serial.println(F("AC Redo/Repeat"));
            break;
          case 0x27A: 
            Serial.println(F("AC Sort"));
            break;
          case 0x27B: 
            Serial.println(F("AC Sort Ascending"));
            break;
          case 0x27C: 
            Serial.println(F("AC Sort Descending"));
            break;
          case 0x27D: 
            Serial.println(F("AC Filter"));
            break;
          case 0x27E: 
            Serial.println(F("AC Set Clock"));
            break;
          case 0x27F: 
            Serial.println(F("AC View Clock"));
            break;
          case 0x280: 
            Serial.println(F("AC Select Time Zone"));
            break;
          case 0x281: 
            Serial.println(F("AC Edit Time Zones"));
            break;
          case 0x282: 
            Serial.println(F("AC Set Alarm"));
            break;
          case 0x283: 
            Serial.println(F("AC Clear Alarm"));
            break;
          case 0x284: 
            Serial.println(F("AC Snooze Alarm"));
            break;
          case 0x285: 
            Serial.println(F("AC Reset Alarm"));
            break;
          case 0x286: 
            Serial.println(F("AC Synchronize"));
            break;
          case 0x287: 
            Serial.println(F("AC Send/Receive"));
            break;
          case 0x288: 
            Serial.println(F("AC Send To"));
            break;
          case 0x289: 
            Serial.println(F("AC Reply"));
            break;
          case 0x28A: 
            Serial.println(F("AC Reply All"));
            break;
          case 0x28B: 
            Serial.println(F("AC Forward Msg"));
            break;
          case 0x28C: 
            Serial.println(F("AC Send"));
            break;
          case 0x28D: 
            Serial.println(F("AC Attach File"));
            break;
          case 0x28E: 
            Serial.println(F("AC Upload"));
            break;
          case 0x28F: 
            Serial.println(F("AC Download (Save Target As)"));
            break;
          case 0x290: 
            Serial.println(F("AC Set Borders"));
            break;
          case 0x291: 
            Serial.println(F("AC Insert Row"));
            break;
          case 0x292: 
            Serial.println(F("AC Insert Column"));
            break;
          case 0x293: 
            Serial.println(F("AC Insert File"));
            break;
          case 0x294: 
            Serial.println(F("AC Insert Picture"));
            break;
          case 0x295: 
            Serial.println(F("AC Insert Object"));
            break;
          case 0x296: 
            Serial.println(F("AC Insert Symbol"));
            break;
          case 0x297: 
            Serial.println(F("AC Save and Close"));
            break;
          case 0x298: 
            Serial.println(F("AC Rename"));
            break;
          case 0x299: 
            Serial.println(F("AC Merge"));
            break;
          case 0x29A: 
            Serial.println(F("AC Split"));
            break;
          case 0x29B: 
            Serial.println(F("AC Disribute Horizontally"));
            break;
          case 0x29C: 
            Serial.println(F("AC Distribute Vertically"));
            break;
          case 0x29D: 
            Serial.println(F("AC Next Keyboard Layout Select"));
            break;
          case 0x29E: 
            Serial.println(F("AC Navigation Guidance"));
            break;
          case 0x29F: 
            Serial.println(F("AC Desktop Show All Windows"));
            break;
          case 0x2A0: 
            Serial.println(F("AC Soft Key Left"));
            break;
          case 0x2A1: 
            Serial.println(F("AC Soft Key Right"));
            break;
          case 0x2A2: 
            Serial.println(F("AC Desktop Show All Applications"));
            break;
          case 0x2B0: 
            Serial.println(F("AC Idle Keep Alive"));
            break;
          case 0x2C0: 
            Serial.println(F("Extended Keyboard Attributes Collection"));
            break;
          case 0x2C1: 
            Serial.println(F("Keyboard Form Factor"));
            break;
          case 0x2C2: 
            Serial.println(F("Keyboard Key Type"));
            break;
          case 0x2C3: 
            Serial.println(F("Keyboard Physical Layout"));
            break;
          case 0x2C4: 
            Serial.println(F("Vendor-Specific Keyboard Physical Layout"));
            break;
          case 0x2C5: 
            Serial.println(F("Keyboard IETF Language Tag Index"));
            break;
          case 0x2C6: 
            Serial.println(F("Implemented Keyboard Input Assist Controls"));
            break;
          case 0x2C7: 
            Serial.println(F("Keyboard Input Assist Previous"));
            break;
          case 0x2C8: 
            Serial.println(F("Keyboard Input Assist Next"));
            break;
          case 0x2C9: 
            Serial.println(F("Keyboard Input Assist Previous Group"));
            break;
          case 0x2CA: 
            Serial.println(F("Keyboard Input Assist Next Group"));
            break;
          case 0x2CB: 
            Serial.println(F("Keyboard Input Assist Accept"));
            break;
          case 0x2CC: 
            Serial.println(F("Keyboard Input Assist Cancel"));
            break;
          case 0x2D0: 
            Serial.println(F("Privacy Screen Toggle"));
            break;
          case 0x2D1: 
            Serial.println(F("Privacy Screen Level Decrement"));
            break;
          case 0x2D2: 
            Serial.println(F("Privacy Screen Level Increment"));
            break;
          case 0x2D3: 
            Serial.println(F("Privacy Screen Level Minimum"));
            break;
          case 0x2D4: 
            Serial.println(F("Privacy Screen Level Maximum"));
            break;
          case 0x500: 
            Serial.println(F("Contact Edited"));
            break;
          case 0x501: 
            Serial.println(F("Contact Added"));
            break;
          case 0x502: 
            Serial.println(F("Contact Record Active"));
            break;
          case 0x503: 
            Serial.println(F("Contact Index"));
            break;
          case 0x504: 
            Serial.println(F("Contact Nickname"));
            break;
          case 0x505: 
            Serial.println(F("Contact First Name"));
            break;
          case 0x506: 
            Serial.println(F("Contact Last Name"));
            break;
          case 0x507: 
            Serial.println(F("Contact Full Name"));
            break;
          case 0x508: 
            Serial.println(F("Contact Phone Number Personal"));
            break;
          case 0x509: 
            Serial.println(F("Contact Phone Number Business"));
            break;
          case 0x50A: 
            Serial.println(F("Contact Phone Number Mobile"));
            break;
          case 0x50B: 
            Serial.println(F("Contact Phone Number Pager"));
            break;
          case 0x50C: 
            Serial.println(F("Contact Phone Number Fax"));
            break;
          case 0x50D: 
            Serial.println(F("Contact Phone Number Other"));
            break;
          case 0x50E: 
            Serial.println(F("Contact Email Personal"));
            break;
          case 0x50F: 
            Serial.println(F("Contact Email Business"));
            break;
          case 0x510: 
            Serial.println(F("Contact Email Other"));
            break;
          case 0x511: 
            Serial.println(F("Contact Email Main"));
            break;
          case 0x512: 
            Serial.println(F("Contact Speed Dial Number"));
            break;
          case 0x513: 
            Serial.println(F("Contact Status Flag"));
            break;
          case 0x514: 
            Serial.println(F("Contact Misc."));
            break;
            //*/

          
          default:
            Serial.print(usage, HEX);
            Serial.println(F("(Reserved)"));
            break;
        }
      else
        Serial.println(F("Consumer"));
      break;
    case 0x0d:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Digitizers"));
      break;
    case 0x0e:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Haptics"));
      break;
    case 0x0f:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Physical Input Device"));
      break;
    case 0x10:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Unicode"));
      break;
    case 0x11:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("SoC"));
      break;
    case 0x12:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Eye and Head Trackers"));
      break;
    case 0x14:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Auxiliary Display"));
      break;
    case 0x20:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Sensors"));
      break;
    case 0x40:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Medical Instrument"));
      break;
    case 0x41:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Braille Display"));
      break;
    case 0x59:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Lighting And Illumination"));
      break;
    case 0x80:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Monitor"));
      break;
    case 0x81:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Monitor Enumerated"));
      break;
    case 0x82:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("VESA Virtual Controls"));
      break;
    case 0x84:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Power"));
      break;
    case 0x85:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Battery System"));
      break;
    case 0x8c:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Barcode Scanner"));
      break;
    case 0x8d:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Scales"));
      break;
    case 0x8e:
    if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Magnetic Stripe Reader"));
      break;
    case 0x90:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Camera Control"));
      break;
    case 0x91:
      if(printUsage)
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      else
        Serial.println(F("Arcade"));
      break;
    case 0x92:
      if(printUsage){
        switch(usage){
          default:
            Serial.println(usage, HEX);
            break;
        }
      }else
        Serial.println(F("Gaming Device"));
      break;
    default:
      if(printUsage){
        Serial.print(usage, HEX);
        Serial.println(F("(vendor)"));
      }else{
        Serial.print(usagePage, HEX);
        Serial.println(F("(vendor)"));
      }
      break;
  }
}


String getString(int addr){
  int strLen = EEPROM.read(addr);
  char data[strLen + 1];
  for (int i = 0; i < strLen; i++){
    data[i] = EEPROM.read(addr + 1 + i);
    Serial.print(data[i]);
  }
    
  data[strLen] = '\0'; 
  return String(data);
  /*
byte values[] = {4, 8, 20, 24, 36, 40, 52, 56, 68, 72, 84, 88, 100, 116, 120, 128, 132, 136, 144, 148, 152, 160, 164, 168, 176, 180, 192};
String str[28] = {"USAGE PAGE", "USAGE", "LOGICAL MINIMUM", "USAGE MINIMUM", "LOGICAL MAXIMUM", "USAGE MAXIMUM", "PHYSICAL MINIMUM", "DESIGNATOR INDEX", "PHYSICAL MAXIMUM", "DESIGNATOR MINIMUM", "UNIT EXPONENT", "DESIGNATOR MAXIMUM", "UNIT", "REPORT SIZE", "STRING INDEX", "INPUT", "REPORT ID", "STRING MINIMUM", "OUTPUT", "REPORT COUNT", "STRING MAXIMUM", "COLLECTION", "PUSH", "DELIMITER", "FEATURE", "POP", "END COLLECTION",""};
int addresses[27] = {0, 11, 17, 33, 47, 63, 77, 94, 111, 128, 147, 161, 180, 185, 197, 210, 216, 226, 241, 248, 261, 276, 287, 292, 302, 310, 314};

::::::::::SETUP::::::::::::::::::::::::::::::::::::::::::::::::::::::
String str[28] = {"USAGE PAGE", "USAGE", "LOGICAL MINIMUM", "USAGE MINIMUM", "LOGICAL MAXIMUM", "USAGE MAXIMUM", "PHYSICAL MINIMUM", "DESIGNATOR INDEX", "PHYSICAL MAXIMUM", "DESIGNATOR MINIMUM", "UNIT EXPONENT", "DESIGNATOR MAXIMUM", "UNIT", "REPORT SIZE", "STRING INDEX", "INPUT", "REPORT ID", "STRING MINIMUM", "OUTPUT", "REPORT COUNT", "STRING MAXIMUM", "COLLECTION", "PUSH", "DELIMITER", "FEATURE", "POP", "END COLLECTION",""};
int size = -1;
  do{
    size++;
  }while(str[size].length() > 0);
    
  Serial.println(String("Size: ")+size);
  int address = 0;
  int addresses[size] = {0};

  for(int i = 0; i < size; i++){
    Serial.print(str[i]);
    Serial.print(" ");
    Serial.println(address);
    addresses[i] = address;
    address = getNextStringAddr(address, str[i]);
  }

  address = 0;
  for(int i = 0; i < size; i++){
    Serial.print("#define ");
    Serial.print(str[i]);
    Serial.print(" ");
    Serial.println(address);
    addresses[i] = address;
    address = getNextStringAddr(address, str[i]);
  }


  Serial.println("ADDRESSES: ");
  Serial.print("int addresses[");
  Serial.print(size);
  Serial.print("] = {");
  address = 0;
  for(int i = 0; i < size; i++){
    Serial.print(address);
    if(i != size-1)
      Serial.print(", ");
    addresses[i] = address;
    address = getNextStringAddr(address, str[i]);
  }
  Serial.println("};");

Serial.println("READ: ");
  int addr = 0;
  for(int i = 0; i < size; i++){
    String newStr;
    Serial.print(addr);
    addr = readStringFromEEPROM(addr, &newStr);
    Serial.print("  ");
    Serial.print(newStr);
    Serial.print("\t");

    Serial.print(addresses[i]);
    Serial.print("  ");
    Serial.println(getString(addresses[i]));
  }

  while(1); //::::::::::::::::WRITE GUARD!!!::::::::::::::::::::

  Serial.println("WRITE: ");
  addr = 0;
  for(int i = 0; i < size; i++){
    if(addr > 1024){
      Serial.println("addr over 1024 wrror!");
      while(true);
      break;
    }
    Serial.print(addr);
    addr = writeStringToEEPROM(addr, str[i]);
    Serial.print(" ");
    Serial.println(str[i]);
  }
  
  Serial.println("READ: ");
  addr = 0;
  for(int i = 0; i < size; i++){
    String newStr;
    Serial.print(addr);
    addr = readStringFromEEPROM(addr, &newStr);
    Serial.print(" ");
    Serial.println(newStr);
  }
//::::::::::END OF SETUP:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  //*/
}

/*
void printReportHeader(byte header){
  switch(header & B00001100){

    case HID_RDH_MAIN:
      switch(header & B11111100){
        case HID_RDH_INPUT:
          Serial.print("");
          break;
        case HID_RDH_OUTPUT:
          Serial.print("");
          break;
        case HID_RDH_COLLECTION:
          Serial.print("");
          break;
        case HID_RDH_FEATURE:
          Serial.print("");
          break;
        case HID_RDH_END_COLLECTION:
          Serial.print("");
          break;
      }
      break;

    case HID_RDH_GLOBAL:
      switch(header & B11111100){
        case HID_RDH_USAGEPAGE:
          Serial.print("");
          break;
        case HID_RDH_LOGICAL_MIN:
          Serial.print("");
          break;
        case HID_RDH_LOGICAL_MAX:
          Serial.print("");
          break;
        case HID_RDH_PHYSICAL_MIN:
          Serial.print("");
          break;
        case HID_RDH_PHYSICAL_MAX:
          Serial.print("");
          break;
        case HID_RDH_UNIT_EXPONENT:
          Serial.print("");
          break;
        case HID_RDH_UNIT:
          Serial.print("");
          break;
        case HID_RDH_REPORT_SIZE:
          Serial.print("");
          break;
        case HID_RDH_REPORT_ID:
          Serial.print("");
          break;
        case HID_RDH_REPORT_COUNT:
          Serial.print("");
          break;
        case HID_RDH_PUSH:
          Serial.print("");
          break;
        case HID_RDH_POP:
          Serial.print("");
          break;
      }
      break;
  

    case HID_RDH_LOCAL:
      switch(header & B11111100){
        case HID_RDH_USAGE:
          Serial.print("");
          break;
        case HID_RDH_USAGE_MIN:
          Serial.print("");
          break;
        case HID_RDH_USAGE_MAX:
          Serial.print("");
          break;
        case HID_RDH_DESIGNATOR_INDEX:
          Serial.print("");
          break;
        case HID_RDH_DESIGNATOR_MIN:
          Serial.print("");
          break;
        case HID_RDH_DESIGNATOR_MAX:
          Serial.print("");
          break;
        case HID_RDH_STRING_INDEX:
          Serial.print("");
          break;
        case HID_RDH_STRING_MIN:
          Serial.print("");
          break;
        case HID_RDH_STRING_MAX:
          Serial.print("");
          break;
        case HID_RDH_DELIMITER:
          Serial.print("");
          break;
    }
    break;
  }
}
//*/

#define RSERIAL_H
#endif