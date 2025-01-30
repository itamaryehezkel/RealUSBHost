#include <EEPROM.h>

String getString(int addr){
  int strLen = EEPROM.read(addr);
  char data[strLen + 1];
  for (int i = 0; i < strLen; i++){
    data[i] = EEPROM.read(addr + 1 + i);
    Serial.print(data[i]);
  }
    
  data[strLen] = '\0'; 
  return String(data);
}


int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}

int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; 
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}



void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(500);
  // put your setup code here, to run once:
  /*

byte values[] = {4, 8, 20, 24, 36, 40, 52, 56, 68, 72, 84, 88, 100, 116, 120, 128, 132, 136, 144, 148, 152, 160, 164, 168, 176, 180, 192};
String str[] = {"USAGE PAGE", "USAGE", "LOGICAL MINIMUM", "USAGE MINIMUM", "LOGICAL MAXIMUM", "USAGE MAXIMUM", "PHYSICAL MINIMUM", "DESIGNATOR INDEX", "PHYSICAL MAXIMUM", "DESIGNATOR MINIMUM", "UNIT EXPONENT", "DESIGNATOR MAXIMUM", "UNIT", "REPORT SIZE", "STRING INDEX", "INPUT", "REPORT ID", "STRING MINIMUM", "OUTPUT", "REPORT COUNT", "STRING MAXIMUM", "COLLECTION", "PUSH", "DELIMITER", "FEATURE", "POP", "END COLLECTION",""};
int addresses[] = {0, 11, 17, 33, 47, 63, 77, 94, 111, 128, 147, 161, 180, 185, 197, 210, 216, 226, 241, 248, 261, 276, 287, 292, 302, 310, 314};



  */


byte values[] = {4, 8, 20, 24, 36, 40, 52, 56, 68, 72, 84, 88, 100, 116, 120, 128, 132, 136, 144, 148, 152, 160, 164, 168, 176, 180, 192};
String str[69] = {
  F("USAGE PAGE"), F("USAGE"), F("LOGICAL MINIMUM"), F("USAGE MINIMUM"), F("LOGICAL MAXIMUM"), F("USAGE MAXIMUM"), F("PHYSICAL MINIMUM"),
  F("DESIGNATOR INDEX"), F("PHYSICAL MAXIMUM"), F("DESIGNATOR MINIMUM"), F("UNIT EXPONENT"), F("DESIGNATOR MAXIMUM"), F("UNIT"), F("REPORT SIZE"),
  F("STRING INDEX"), F("INPUT"), F("REPORT ID"), F("STRING MINIMUM"), F("OUTPUT"), F("REPORT COUNT"), F("STRING MAXIMUM"), F("COLLECTION"), F("PUSH"),
  F("DELIMITER"), F("FEATURE"), F("POP"), F("END COLLECTION"),F("DEVICE"),F("bLength"),F("bDescriptorType"),F("bcdUSB"),F("bDeviceClass"),
  F("bDeviceSubClass"),F("bDeviceProtocol"),F("bMaxPacketSize0"),F("idVendor"),F("idProduct"),F("bcdDevice"),F("iManufacturer"),
  F("iProduct"),F("iSerialNumber"),F("bNumConfigurations"),F("CONFIGURATION"),F("wTotalLength"),F("bNumInterfaces"),F("bConfigurationValue"),
  F("iConfiguration"),F("bmAttributes"),F("bMaxPower"),F("HID"),F("bcdHID"),F("bCountryCode"),F("bNumDescriptors"),F("bReportDescriptorType"),
  F("wDescriptorLength"),F("ENDPOINT"),F("bEndpointAddress"), F("bmAttributes"), F("wMaxPacketSize"), F("bInterval"),F("INTERFACE"),
  F("bInterfaceNumber"), F("bAlternateSetting"), F("bNumEndpoints"), F("bInterfaceClass"), F("bInterfaceSubClass"), F("bInterfaceProtocol"), F("iInterface"),
  
  F("hrSUCCESS(0)"),F("hrBUSY(1)"),F("hrBADREQ(2)"),F("hrUNDEF(3)"),F("hrNAK(4)"),F("hrSTALL(5)"),F("hrTOGERR(6)"),F("hrWRONGPID(7)"),F("hrBADBC(8)"),
  F("hrPIDERR(9)"),F("hrPKTERR(10)"),F("hrCRCERR(11)"),F("hrKERR(12)"),F("hrJERR(13)"),F("hrTIMEOUT(14)"), F("hrBABBLE(15)"), F("Unkown error code"),
  F("Malloc"),
  F("")
  };
//int addresses[] = {0, 11, 17, 33, 47, 63, 77, 94, 111, 128, 147, 161, 180, 185, 197, 210, 216, 226, 241, 248, 261, 276, 287, 292, 302, 310, 314};

//::::::::::SETUP::::::::::::::::::::::::::::::::::::::::::::::::::::::
//String str[28] = {"USAGE PAGE", "USAGE", "LOGICAL MINIMUM", "USAGE MINIMUM", "LOGICAL MAXIMUM", "USAGE MAXIMUM", "PHYSICAL MINIMUM", "DESIGNATOR INDEX", "PHYSICAL MAXIMUM", "DESIGNATOR MINIMUM", "UNIT EXPONENT", "DESIGNATOR MAXIMUM", "UNIT", "REPORT SIZE", "STRING INDEX", "INPUT", "REPORT ID", "STRING MINIMUM", "OUTPUT", "REPORT COUNT", "STRING MAXIMUM", "COLLECTION", "PUSH", "DELIMITER", "FEATURE", "POP", "END COLLECTION",""};
int size = -1;
Serial.println(F("-----------START-----------"));
  do{
    size++;
   // Serial.println(str[size]);
    //Serial.println(!str[size].equals(""));
    //while(!str[size].equals(""));

  }while(!str[size].equals(""));
    //!str[size].equals("")
  Serial.print(F("Size: "));
  Serial.println(size);
  

  int address = 0;
  int addresses[size] = {0};
/*
  for(int i = 0; i < size; i++){
    Serial.print(str[i]);
    Serial.print(" ");
    Serial.println(address);
    addresses[i] = address;
    
    
    address = address+1+ str[i].length();
  }
  while(1);
  //*/

  address = 0;
  for(int i = 0; i < size; i++){
    Serial.print(F("#define "));
    Serial.print(F("HID_SA_"));
    String st = String(str[i]);
    st.replace(" ","_");
    Serial.print(st);
    Serial.print(F(" "));
    Serial.println(address);
    addresses[i] = address;
    address = address+1+ str[i].length();
  }


  Serial.println("================");
  Serial.print("int addresses[");
  Serial.print(size);
  Serial.print("] = {");
  address = 0;
  for(int i = 0; i < size; i++){
    Serial.print(address);
    if(i != size-1)
      Serial.print(", ");
    addresses[i] = address;
    address = address+1+ str[i].length();
  }
  Serial.println("};");

while(1);

Serial.println("READ: ");
  int addr = 0;
  if(false)
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
}

void loop() {
  // put your main code here, to run repeatedly:

}
