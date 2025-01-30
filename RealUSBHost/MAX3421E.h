#ifndef MAX3421_H

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include "defines.h"
#include "serial.h"
#include "structs.h"


void printStatus(uint8_t rcode){
  switch((rcode & 0x0F)){
    
    case 0x00:
      printString(HID_SA_hrSUCCESS);
      break;
    case 0x01:
      printString(HID_SA_hrBUSY);
      break;
    case 0x02: 
      printString(HID_SA_hrBADREQ);
      break;
    case 0x03: 
      printString(HID_SA_hrUNDEF);
      break;
    case 0x04:
      printString(HID_SA_hrNAK);
      break;
    case 0x05:
      printString(HID_SA_hrSTALL);
      break;
    case 0x06:
      printString(HID_SA_hrTOGERR);
      break;
    case 0x07:
      printString(HID_SA_hrWRONGPID);
      break;
    case 0x08:
      printString(HID_SA_hrBADBC);
      break;
    case 0x09:
      printString(HID_SA_hrPIDERR);
      break;
    case 0x0A:
      printString(HID_SA_hrPKTERR);
      break;
    case 0x0B:
      printString(HID_SA_hrCRCERR);
      break;
    case 0x0C:
      printString(HID_SA_hrKERR);
      break;
    case 0x0D:
      printString(HID_SA_hrJERR);
      break;
    case 0x0E:
      printString(HID_SA_hrTIMEOUT);
      break;
    case 0x0F:
      printString(HID_SA_hrBABBLE);
      break;
    default:
      printString(HID_SA_Unkownerrorcode);
      break;
  }
  
}
//void(* resetFunc) (void) = 0;

class USBHost{
  public:
    USBHost(byte backlog);
    USBHost();
    uint8_t init(){
      initArduino();
      initMAX();
    }
    void printRevision(){
      double rev = regRd(rREVISION);
      rev = rev/10;
      Serial.print(F("Revision"));
      printColon(1);
      printSpace(1);
      Serial.print(rev);
      Serial.println();
    }
    void task(){
      byte flags = regRd(rHIRQ);

      if(flags & bmCONDETIRQ){
        regWr(rHIRQ, bmCONDETIRQ);
        emptyDescriptors();
        //free(this->interfaces);
        this->totalMalloced = 0;
        detectPeripheral();
      }

    }

    /*
for(int i = 0; i < 8; i++){
        switch((flags & (1 << i))){
          case bmBUSEVENTIRQ:
            break;
          case bmRWUIRQ:
            break;
          case bmRCVDAVIRQ:
            break;
          case bmSNDBAVIRQ:
            break;
          case bmSUSDNIRQ:
            break;
          case bmCONDETIRQ:
            regWr(rHIRQ, bmCONDETIRQ);
            detectPeripheral();
            break;
          case bmFRAMEIRQ:
            break;
          case bmHXFRDNIRQ:
            break;
        }
      }
    //*/
  private:
  
    boolean _firstRun;
    byte _status;
    byte _vbusState;
    byte _backlog;
    Descriptor Device;
    Descriptor Config;
    InterfaceCollection *interfaces;
    byte **rawBytes;
    byte *reports;
    uint16_t totalMalloced;

    boolean logEnabled(byte bit){
      return ((_backlog & bit) == bit);
    }

    void initArduino(){
      pinMode( MAX_SS, OUTPUT );
      digitalWrite( MAX_SS, HIGH);
      delayMicroseconds(100);
      if(logEnabled(BACKLOG_ARDUINO)){
        Serial.print(F("ARDUINO"));
        printColon(1);
        printSpace(1);
        Serial.print(F("setting up SPI"));
      }
      SPI.begin();
      if(logEnabled(BACKLOG_ARDUINO))
        printDot(1);
      SPI.setBitOrder(MSBFIRST);  //LSBFIRST or MSBFIRST
      if(logEnabled(BACKLOG_ARDUINO))
        printDot(1);
      SPI.setClockDivider(SPI_CLOCK_DIV2);
      if(logEnabled(BACKLOG_ARDUINO))
        printDot(1);
      SPI.setDataMode(SPI_MODE0);
      if(logEnabled(BACKLOG_ARDUINO))
        printDot(1);
      digitalWrite(MAX_SS,HIGH);
      asm volatile("nop");
      if(logEnabled(BACKLOG_ARDUINO))
        Serial.println(F("Done, (MSB,DIV2,MODE0)"));
    }
  void regWr( byte reg, byte val){
      digitalWrite(MAX_SS,LOW);
      asm volatile("nop");
      SPDR = ( reg | 0x02 );
      while(!( SPSR & ( 1 << SPIF )));
      _status = SPDR;
      SPDR = val;
      while(!( SPSR & ( 1 << SPIF )));
      digitalWrite(MAX_SS,HIGH);
      asm volatile("nop");
      return;
    }
    byte regRd( byte reg ){
      byte tmp;
      digitalWrite(MAX_SS,LOW);
      asm volatile("nop");
      SPDR = reg;
      while(!( SPSR & ( 1 << SPIF )));
      _status = SPDR;
      SPDR = 0; //send empty byte
      while(!( SPSR & ( 1 << SPIF )));
      digitalWrite(MAX_SS,HIGH); 
      asm volatile("nop");
      return( SPDR );
    }
    void gpioWr( byte val ){
      this->regWr( rIOPINS1, val );
      val = val >>4;
      this->regWr( rIOPINS2, val );
      return;     
    }
    byte gpioRd( void ){
      byte tmpbyte = 0;
      tmpbyte = this->regRd( rIOPINS2 );            //pins 4-7
      tmpbyte &= 0xf0;                        //clean lower nibble
      tmpbyte |= ( this->regRd( rIOPINS1 ) >>4 ) ;  //shift low bits and OR with upper from previous operation. Upper nibble zeroes during shift, at least with this compiler
      return( tmpbyte );
    }
    char * bytesWr( byte reg, byte nbytes, char * data ){
      digitalWrite(MAX_SS,LOW);
      asm volatile("nop");
      SPDR = ( reg | 0x02 );
      while( nbytes-- ) {
        while(!( SPSR & ( 1 << SPIF )));  //check if previous byte was sent
        _status = SPDR;
        SPDR = ( *data );               // send next data byte
        data++;                         // advance data pointer
      }
      while(!( SPSR & ( 1 << SPIF )));
      digitalWrite(MAX_SS,HIGH);
      asm volatile("nop");
      return( data );
    }
    char * bytesRd ( byte reg, byte nbytes, char  * data ){
      digitalWrite(MAX_SS,LOW);
      asm volatile("nop");
      SPDR = reg;      
      while(!( SPSR & ( 1 << SPIF )));    //wait
      _status = SPDR;
      while( nbytes ) {
        SPDR = 0; //send empty byte
        nbytes--;
        while(!( SPSR & ( 1 << SPIF )));
        *data = SPDR;
        data++;
      }
      digitalWrite(MAX_SS,HIGH);
      asm volatile("nop");
      return( data );   
    }
    void bytesRd2 ( byte reg, byte nbytes, byte data[] ){
      digitalWrite(MAX_SS,LOW);
      asm volatile("nop");
      SPDR = reg;      
      while(!( SPSR & ( 1 << SPIF )));    //wait
      _status = SPDR;
      for(int i = 0; i < nbytes; i++ ) {
        SPDR = 0; //send empty byte
        //nbytes--;
        while(!( SPSR & ( 1 << SPIF )));
        data[i] = SPDR;
        //data++;
      }
      digitalWrite(MAX_SS,HIGH);
      asm volatile("nop");
      //return( data );   
    }
    boolean reset(){
      byte tmp = 0;
      regWr( rUSBCTL, bmCHIPRES );                        //Chip reset. This stops the oscillator
      regWr( rUSBCTL, 0x00 );                             //Remove the reset
      while(!(regRd( rUSBIRQ ) & bmOSCOKIRQ )) {          //wait until the PLL is stable
          tmp++;                                          //timeout after 256 attempts
          if( tmp == 0 ) {
              return( false );
          }
        delayMicroseconds(100);
      }
      return( true );
    }
  void initMAX(){
     regWr( rPINCTL,( bmFDUPSPI + bmINTLEVEL + bmGPXB ));    //Full-duplex SPI, level interrupt, GPX
    if( reset() == false ) {                                //stop/start the oscillator
      printString(HID_SA_ERROR);
      printColon(1);
      printSpace(1);
      Serial.println(F("OSCOKIRQ failed to assert"));
      return 0;
    }else if(logEnabled(BACKLOG_MAX))
      Serial.println(F("Clock Started"));
    setInterrupts(0);
    if(logEnabled(BACKLOG_REVISION))
      printRevision();
    detectPeripheral();
  }
  boolean detectPeripheral(){
    if(!busprobe()){ 
      _firstRun = false;
      return false;
    }
    if(_firstRun){
      if( (_firstRun && (logEnabled(BACKLOG_MAX) || logEnabled(BACKLOG_CONDET))) )
        Serial.println(String(F("Detected "))+String(this->_vbusState == FSHOST ? F("Fast Speed") : F("Low Speed"))+ String(F(" device"))); 
    }else{
      if(logEnabled(BACKLOG_CONDET))
        Serial.println(String(F("Detected "))+String(this->_vbusState == FSHOST ? F("Fast Speed") : F("Low Speed"))+ String(F(" device"))); 
    }
    busReset();
    delay(100);
    regWr(rHCTL, bmRCVTOG1);
    if(regRd(rHIRQ) & bmFRAMEIRQ)
      regWr(rHIRQ, bmFRAMEIRQ);
    _firstRun = false;
    if(logEnabled(BACKLOG_MAX) || logEnabled(BACKLOG_USB_RAW)){
      Serial.print(F("MAX3421E"));
      printColon(1);
      printSpace(1);
      Serial.println(F("Enumerating Peripheral.."));
    }
    enumerate();
    return true;
  }
  void setInterrupts(uint8_t intFlags){
    if(intFlags){
      regWr(rHIEN,intFlags); 
      regWr( rCPUCTL, 0x01 ); 
    }
  }
  
  boolean busprobe( void ){
    regWr( rMODE, bmDPPULLDN|bmDMPULLDN|bmHOST );
    this->regWr( rHCTL,bmSAMPLEBUS );                                             // sample USB bus
    while(!(this->regRd( rHCTL ) & bmSAMPLEBUS ));                                //wait for sample operation to finish
    byte bus_sample;
    bus_sample = this->regRd( rHRSL );            //Get J,K status
    bus_sample &= ( bmJSTATUS|bmKSTATUS );      //zero the rest of the byte
    switch( bus_sample ) {                          //start full-speed or low-speed host 
      case( bmJSTATUS ):
        if(( this->regRd( rMODE ) & bmLOWSPEED ) == 0 ) {
          //regWr( rMODE, MODE_FS_HOST );       //start full-speed host
          this->_vbusState = FSHOST;
        }else {
          //regWr( rMODE, MODE_LS_HOST);        //start low-speed host
          this->_vbusState = LSHOST;
        }
        break;
      case( bmKSTATUS ):
        if(( this->regRd( rMODE ) & bmLOWSPEED ) == 0 ) {
          //regWr( rMODE, MODE_LS_HOST );       //start low-speed host
          this->_vbusState = LSHOST;
        }else {
          // regWr( rMODE, MODE_FS_HOST );       //start full-speed host
          this->_vbusState = FSHOST;
        }
        break;
      case( bmSE1 ):              //illegal state
        this->_vbusState = SE1;
        break;
      case( bmSE0 ):              //disconnected state
        this->_vbusState = SE0;
        break;
    }//end switch( bus_sample )
    switch(this->_vbusState){
      case LSHOST:
        this->regWr( rMODE, bmDPPULLDN | bmDMPULLDN | bmHOST | bmLOWSPEED | bmSEPIRQ | bmSOFKAENAB );
        return 1;
      case FSHOST:
        this->regWr( rMODE, bmDPPULLDN | bmDMPULLDN | bmHOST | bmSEPIRQ | bmSOFKAENAB );
        return 1;
      case SE0: //disconnected
        if(_firstRun)
          Serial.println(F("ERROR: No device detected."));
        else if(logEnabled(BACKLOG_CONDET)){
          Serial.println(F("Device disconnected."));
        }
        return 0;
      case SE1: //illegal state
        Serial.println(F("ERROR: Initialization failed! Illegal state detected."));
        return 0;
      default:
        Serial.println(String("Error: Unrecognised vBusState(")+_vbusState+String(F(")")));
        return 0;
    }
  }
  
  void busReset(){
    regWr(rHCTL, bmBUSRST); // Issue a bus reset
    while(regRd(rHCTL) & bmBUSRST); // Wait for bus reset to finish
    while(!(regRd(rHIRQ) & bmFRAMEIRQ)); // Wait for first frame
    regWr(rHIRQ, bmBUSEVENTIRQ | bmCONDETIRQ | bmFRAMEIRQ); //clear flags
    regWr(rHCTL,bmFRMRST);  // Clear frame counter
  }
  Result getResult(){
    byte tmp = regRd(rHRSL);
    Result result;
    result.sndTOG = tmp & bmSNDTOGRD;
    result.rcvTOG = tmp & bmRCVTOGRD;
    result.rcode = tmp & 0x0F;
    return result; 
  }
  Result dispatchSetup(byte arr[], int addr){
    regWr(rPERADDR, addr); //set the address register to 0, the default value for new devices
    bytesWr(rSUDFIFO, 8, arr);  //loading SUDFIFO buffer with 8 bytes of setup packet
    regWr(rHXFR, tokSETUP);  // launch the token transfer to control ep 0
    while( !((regRd(rHIRQ) & bmHXFRDNIRQ)==bmHXFRDNIRQ) ); //wait until transfer is complete
    regWr(rHIRQ,bmHXFRDNIRQ);
    Result result;
    return getResult();
  }
  void controlRead(uint8_t bmRequestType, uint8_t bDescriptorType, uint8_t index, uint16_t length, uint8_t addr, byte arr[]){
    
    if(logEnabled(BACKLOG_USB_RAW)){
      printUnder(6);  //______
      Serial.print(F("SOF GET "));
      switch(bDescriptorType){
        case DEVICE_DESCRIPTOR:
          //Serial.print(F("DEVICE"));
          printString(HID_SA_DEVICE);
          break;
        case CONFIGURATION_DESCRIPTOR:
          //Serial.print(F("CONFIG"));
          printString(HID_SA_CONFIGURATION);
          break;
        case HIDREPORT_DESCRIPTOR:
          
          printString(HID_SA_HID);
          printSpace(1);
          Serial.print(F("REPORT"));
          break;
        default:
          Serial.print(F("Unknown"));
          break;
      }
      printUnder(16+9); //String("________________"));
      Serial.println();
    }
     //80 6 0 1 0 0 12 0  //Descriptor DEV
      byte getDesc[] = {bmRequestType, GET_DESCRIPTOR, 0, bDescriptorType, index, 0, length, ((length & (0xFF << 8)) >> 8)};
      Result setupResult = dispatchSetup(getDesc,addr);
      
      if(logEnabled(BACKLOG_USB_RAW)){
       
        


        //Serial.print(F("|"));
        printPipe(1);
        printString(HID_SA_SETUP);
        //Serial.print(F("\t"));
        printTab(1);
       // Serial.print(rcodeToString(setupResult.rcode));
       printStatus(setupResult.rcode);
       printTab(1);
       printRoundOpen(1);
       Serial.print(8);
       printRoundClose(1);
       printLessThan(1);
       printDash(2);
       printSpace(1);

       // Serial.print(F("\t(8)<-- "));
        printlnHex(getDesc, 8);
        //Serial.print(F("|"));
        printPipe(1);
        printDash(48);
        //
        Serial.println();
      }
      uint16_t currentLength = length;
      uint16_t currentIndex = 0;

                    Serial.println(F("GOTCHA BITCH?%%%%%%%%%%%????????/"));
      switch(setupResult.rcode){
        case hrSUCCESS:
          do{
                   // Serial.println(F("GOTCHA BITCH????dfhdfgdfgh??????/"));
            regWr(rHXFR, tokIN);
            while(!(regRd(rHIRQ) & bmHXFRDNIRQ));
            regWr(rHIRQ, bmHXFRDNIRQ);
            Result inResult = getResult();
            switch(inResult.rcode){
              case hrSUCCESS:
                int size = regRd(rRCVBC);
                if(size){
                  byte response[size] = {0};
                  bytesRd(rRCVFIFO,size,response);
                  if((regRd(rHIRQ) & bmRCVDAVIRQ) == bmRCVDAVIRQ)
                    regWr(rHIRQ,bmRCVDAVIRQ);
                  if((regRd(rHIRQ) & bmRCVDAVIRQ) == bmRCVDAVIRQ)
                    Serial.println(F("GOTCHA BITCH!!!!!!!!!!!!!!!!"));
                  for(int i = 0; i < size; i++){
                    arr[currentIndex] = response[i];
                    currentIndex++;
                  }
                  currentLength -= size;
                  
                  Result ackResult = getResult();
                  if(logEnabled(BACKLOG_USB_RAW)){
                    Serial.println(F("GOTCHA BITCH!!!!!!!!!!!!!!!!"));
                    //Serial.print(F("|"));
                    printPipe(1);
                    printString(HID_SA_IN);
                    printTab(1);
                    //Serial.print(F("\t"));
                    //Serial.print(rcodeToString(setupResult.rcode));
                    printStatus(setupResult.rcode);
                    //Serial.print(F("\t("));
                    printTab(1);
                    printRoundOpen(1);
                    Serial.print(size);
                    printRoundClose(1);
                    printLessThan(1);
                    printDash(2);
                    printSpace(1);
                    //Serial.print(F(")<-- "));
                    for(int i = 0; i < size; i++){
                      Serial.print(response[i],HEX);
                      if(i != size-1)
                        printSpace(1);
                        //Serial.print(F(" "));
                      else
                        Serial.println(); 
                    }
                    //printlnHex(response,size);
                  }
                }
                break; //END OF IN success
                default:
                    printStatus(inResult.rcode);
                Serial.println();
                break;
            }
          }while(currentLength > 0);
          regWr(rHCTL, bmRCVTOG1);
          regWr(rSNDBC, 0);
          regWr(rHXFR, tokOUTHS);
          while(!(regRd(rHIRQ) & bmHXFRDNIRQ));
          regWr(rHIRQ, bmHXFRDNIRQ);
          Result ackResult = getResult();
          if(logEnabled(BACKLOG_USB_RAW)){
            //Serial.print(F("|"));
            printPipe(1);
            printString(HID_SA_ACK);
            //Serial.print(F("\t"));
            printTab(1);
            //Serial.println(rcodeToString(ackResult.rcode));
            
            printStatus(ackResult.rcode);
            Serial.println();
            
            //Serial.flush();
          }
          break; //END OF SETUP success   
        default:
          //printStatus(inResult.rcode);
          Serial.println();
          break;
      }
    if(logEnabled(BACKLOG_USB_RAW)){
      Serial.flush();
      //for(int i = 0; i < 23; i++)
      //Serial.print("^");
      printPower(20);
      Serial.println();
      //Serial.println(F("^^^^^^^^^^^^^^^^^^^^"));
    }
  }
  
  void controlRead(uint8_t bmRequestType, uint8_t bDescriptorType, uint8_t index, uint16_t length, uint8_t addr, byte * arr,int y){
    
    if(logEnabled(BACKLOG_USB_RAW)){
      printUnder(6);  //______
      Serial.print(F("SOF GET"));
      printSpace(1);
      switch(bDescriptorType){
        case DEVICE_DESCRIPTOR:
          //Serial.print(F("DEVICE"));
          
          printString(HID_SA_DEVICE);
          break;
        case CONFIGURATION_DESCRIPTOR:
          
          printString(HID_SA_CONFIGURATION);
          //Serial.print(F("CONFIG"));
          break;
        case HIDREPORT_DESCRIPTOR:
          
          printString(HID_SA_HID);
          printSpace(1);
          Serial.print(F("REPORT"));
          break;
        default:
          Serial.print(F("Unknown"));
          break;
      }
      printUnder(16+9); //String("________________"));
      Serial.println();
    }
     //80 6 0 1 0 0 12 0  //Descriptor DEV
      byte getDesc[] = {bmRequestType, GET_DESCRIPTOR, 0, bDescriptorType, index, 0, length, ((length & (0xFF << 8)) >> 8)};
      Result setupResult = dispatchSetup(getDesc,addr);
      
      if(logEnabled(BACKLOG_USB_RAW)){
       
        


        //Serial.print(F("|"));
        printPipe(1);
        printString(HID_SA_SETUP);
        printTab(1);
        //Serial.print(F("\t"));
        //Serial.print(rcodeToString(setupResult.rcode));
        printStatus(setupResult.rcode);
        printTab(1);
        printRoundOpen(1);
        Serial.print(8);
        printRoundClose(1);
        printDash(2);
        printSpace(1);
        //Serial.print(F("\t(8)<-- "));
        printlnHex(getDesc, 8);
        //Serial.print(F("|"));
        printPipe(1);
        printDash(48);
        //
        Serial.println();
      }
      uint16_t currentLength = length;
      uint16_t currentIndex = 0;

      switch(setupResult.rcode){
        case hrSUCCESS:
          do{
            regWr(rHXFR, tokIN);
            while(!(regRd(rHIRQ) & bmHXFRDNIRQ));
            regWr(rHIRQ, bmHXFRDNIRQ);
            Result inResult = getResult();
            switch(inResult.rcode){
              case hrSUCCESS:
                int size = regRd(rRCVBC);
                if(size){
                  byte response[size] = {0};
                  bytesRd2(rRCVFIFO,size,response);
                  //bytesRd(rRCVFIFO,size,response);
                 // Serial.println(response[0]);
                  //Serial.println(response[0]);
                  if((regRd(rHIRQ) & bmRCVDAVIRQ) == bmRCVDAVIRQ)
                    regWr(rHIRQ,bmRCVDAVIRQ);
                  if((regRd(rHIRQ) & bmRCVDAVIRQ) == bmRCVDAVIRQ)
                    Serial.println(F("GOTCHA BITCH!!!!!!!!!!!!!!!!"));
                    //arr[size-1] = 5;
                   // Serial.println(arr[size-1]);
                  for(int i = 0; i < size; i++){
                    arr[currentIndex] = response[i];
                    currentIndex++;
                  }
                  currentLength -= size;
                  
                  Result ackResult = getResult();
                    Serial.println(F("GOTCHA BITCH??????????????????/"));
                  if(logEnabled(BACKLOG_USB_RAW)){
                    Serial.println(F("GOTCHA BITCH!!!!!!!!!!!!!!!!"));
                    //Serial.print(F("|"));
                    printPipe(1);
                    printString(HID_SA_IN);
                    printTab(1);
                    //Serial.print(F("\t"));
                    
                    printStatus(ackResult.rcode);
                    //Serial.print(rcodeToString(setupResult.rcode));
                    //Serial.print(F("\t("));
                    printTab(1);
                    printRoundOpen(1);
                    Serial.print(size);
                    printRoundClose(1);
                    printLessThan(1);
                    printDash(2);
                    printSpace(1);
                    //Serial.print(F(")<-- "));
                    for(int i = 0; i < size; i++){
                      Serial.print(response[i],HEX);
                      if(i != size-1)
                        printSpace(1);
                      else
                        Serial.println(); 
                    }
                    //printlnHex(response,size);
                  }
                }
                break; //END OF IN success
            }
          }while(currentLength > 0);
          regWr(rHCTL, bmRCVTOG1);
          regWr(rSNDBC, 0);
          regWr(rHXFR, tokOUTHS);
          while(!(regRd(rHIRQ) & bmHXFRDNIRQ));
          regWr(rHIRQ, bmHXFRDNIRQ);
          Result ackResult = getResult();
          if(logEnabled(BACKLOG_USB_RAW)){
            //Serial.print(F("|"));
            printPipe(1);
            printString(HID_SA_ACK);
            //Serial.print(F("\t"));
            printTab(1);
            printStatus(ackResult.rcode);
            Serial.println();

            //Serial.println();
            
            //Serial.flush();
          }
          break; //END OF SETUP success   
        default:
          break;
      }
      
    if(logEnabled(BACKLOG_USB_RAW)){
      //Serial.flush();
      //for(int i = 0; i < 23; i++)
        //Serial.print("^");
      printPower(20);
      Serial.println();
      //Serial.println(F("^^^^^^^^^^^^^^^^^^^^"));
    }
  }
  void enumerate(){
    byte dev2[18];
    Serial.println(F("Getting DEVICE DESCRIPTOR..."));
    controlRead(0x80, DEVICE_DESCRIPTOR, 0, 18, 0, dev2);
    this->Device.bLength = dev2[0];
    this->Device.bDescriptorType = dev2[1];
    this->Device.device.bcdUSB = (dev2[3] << 8) | dev2[2];
    this->Device.device.bDeviceClass = dev2[4];
    this->Device.device.bDeviceSubClass = dev2[5];
    this->Device.device.bDeviceProtocol = dev2[6];
    this->Device.device.bMaxPacketSize0 = dev2[7];
    this->Device.device.idVendor = (dev2[9] << 8) | dev2[8];
    this->Device.device.idProduct = (dev2[11] << 8) | dev2[10];
    this->Device.device.bcdDevice = (dev2[13] << 8) | dev2[12];
    this->Device.device.iManufacturer = dev2[14];
    this->Device.device.iProduct = dev2[15];
    this->Device.device.iSerialNumber = dev2[16];
    this->Device.device.bNumConfigurations = dev2[17];

    byte conf1[9];
    Serial.println(F("Getting CONFIGURATION DESCRIPTOR..."));
    controlRead(0x80, CONFIGURATION_DESCRIPTOR, 0, 9, 0, conf1);
    this->Config.bLength = conf1[0];
    this->Config.bDescriptorType = conf1[1]; //0x02(config desc)
    this->Config.config.wTotalLength = (conf1[2]) | (conf1[3] << 8);
    this->Config.config.bNumInterfaces = conf1[4];  //2 in this case
    this->Config.config.bConfigurationValue = conf1[5];  //0x01
    this->Config.config.iConfiguration = conf1[6]; //0x00 no string desc for configuration
    this->Config.config.bmAttributes = conf1[7]; 
    this->Config.config.bMaxPower = conf1[8];
    
    byte response[this->Config.config.wTotalLength];
    uint16_t totalMem = sizeof(InterfaceCollection)*this->Config.config.bNumInterfaces;
    totalMalloced += totalMem;
    this->interfaces = (InterfaceCollection *)malloc(totalMem);
    if(logEnabled(BACKLOG_MALLOC)){
      printString(HID_SA_Malloc);
      //Serial.print(F(": "));
      printColon(1);
      printSpace(1);
      Serial.println(totalMem);
    }
    Serial.println(F("Getting CONFIGURATION DESCRIPTOR..."));
    controlRead(0x80, CONFIGURATION_DESCRIPTOR, 0, this->Config.config.wTotalLength, 0, response);
    int length = this->Config.config.bNumInterfaces;

    int offset = 0;
    if(response[1] == 0x02) //config we already got
      offset += response[0];
    int i = -1;
    while(offset < this->Config.config.wTotalLength && i < length){
      switch(response[offset+1]){
        case 0x04:  //interface
          i++;
          this->interfaces[i].interface.bInterfaceNumber = response[offset+2];
          this->interfaces[i].interface.bAlternateSetting = response[offset+3];
          this->interfaces[i].interface.bNumEndpoints = response[offset+4];
          this->interfaces[i].interface.bInterfaceClass = response[offset+5];
          this->interfaces[i].interface.bInterfaceSubClass = response[offset+6];
          this->interfaces[i].interface.bInterfaceProtocol = response[offset+7];
          this->interfaces[i].interface.iInterface = response[offset+8];
          offset += response[offset];
          break;
        case 0x05:
          this->interfaces[i].endpoint.bEndpointAddress = response[offset+2];
          this->interfaces[i].endpoint.bmAttributes = response[offset+3];
          this->interfaces[i].endpoint.wMaxPacketSize = response[offset+4] | (response[offset+5] << 8);
          this->interfaces[i].endpoint.bInterval = response[offset+6];
          offset += response[offset];
          break;
        case 0x21:           
          this->interfaces[i].hid.bcdHID = response[offset+2] | (response[offset+3] << 8);
          this->interfaces[i].hid.bCountryCode = response[offset+4];
          this->interfaces[i].hid.bNumDescriptors = response[offset+5];
          this->interfaces[i].hid.bReportDescriptorType = response[offset+6];
          this->interfaces[i].hid.wDescriptorLength = response[offset+7] | (response[offset+8] << 8);
          offset += response[offset];
          break;
        
        default:  //unhandled types in decimal: 36,37
          if(response[offset+1] != 36 && response[offset+1] != 37){
            Serial.print(F("Error: unhandled desc type= "));
            Serial.println(response[offset+1]);
          }
          //Serial.flush();
          offset += response[offset];
          break;
      }
    }//end of while || END OF FULL DESCRIPTOR PARSE
    /*
    
      int freeMem = freeMemory();
      Serial.println(freeMem);
      Serial.println(this->interfaces[0].hid.wDescriptorLength);
      Serial.println(this->interfaces[1].hid.wDescriptorLength);
    //*/
    //*
    uint16_t reportMem = 0;
    for(int i = 0; i < this->Config.config.bNumInterfaces; i++)
      reportMem += this->interfaces[i].hid.wDescriptorLength;
    uint16_t totalReportMem = sizeof(byte)*reportMem;
    //*/
    //this->reports = (byte*)malloc(totalReportMem);

     
    
    //-------------------------------------------------------------------------------------
    unsigned int rawBytesLength = sizeof(byte *)*this->Config.config.bNumInterfaces;

    if(logEnabled(BACKLOG_MALLOC)){
      printString(HID_SA_Malloc);
      //Serial.print(F(": "));
      printColon(1);
      printSpace(1);
      Serial.println(rawBytesLength);
      totalMalloced += rawBytesLength;
    } //END OF IF LOG ENABLEDBACKLOG_MALLOC


    
    //-------------------------------------------------------------------------------------
    /*
    Serial.print("rawBytesLength: ");
    Serial.println(rawBytesLength);
    Serial.print("fre: ");
    Serial.println(freeMemory());
   // Serial.println(this->interfaces[0].hid.wDescriptorLength);
    Serial.println(this->interfaces[1].hid.wDescriptorLength);
//*/
    this->rawBytes = (byte **)malloc(rawBytesLength);

for(int i = 0; i < this->Config.config.bNumInterfaces; i++){
  byte * reportArr = (byte *)malloc(this->interfaces[i].hid.wDescriptorLength);
  this->rawBytes[i] = reportArr;
  //rawBytes[i][0] = 5*(1-i);
}
//Serial.println(rawBytes[0][0]);

    if(rawBytesLength < freeMemory()){
      for(int i = 0; i < this->Config.config.bNumInterfaces; i++){


        //byte reportArr[this->interfaces[i].hid.wDescriptorLength];
        //reportArr = new unsigned byte[this->interfaces[i].hid.wDescriptorLength];
       // Serial.println(sizeof(byte) * this->interfaces[i].hid.wDescriptorLength);
        //byte * reportArr = (byte *)malloc(this->interfaces[i].hid.wDescriptorLength); //[this->interfaces[i].hid.wDescriptorLength] = {0};
        controlRead(0x81, this->interfaces[i].hid.bReportDescriptorType, this->interfaces[i].interface.bInterfaceNumber, this->interfaces[i].hid.wDescriptorLength, 0, rawBytes[i], 1);
//Serial.println(rawBytes[i][this->interfaces[i].hid.wDescriptorLength-1]);

 //Serial.println(freeMemory());
//printlnHex(rawBytes[i], this->interfaces[i].hid.wDescriptorLength);
        //Serial.println(this->interfaces[i].hid.wDescriptorLength);  //this->rawBytes[0][0]
        //byte * reportArr2 = (byte *)malloc(this->interfaces[i].hid.wDescriptorLength);
      //Serial.println(freeMemory());
        //for(int k = 0; k < this->interfaces[i].hid.wDescriptorLength; k++)
          //reportArr2[k] = reportArr[k];
        //reportArr2[0] = 5;
        //this->rawBytes[i] = reportArr;

        //Serial.println(freeMemory());
      }
      //Serial.println(freeMemory());
      //Serial.println("FREE MEM ^^");
      //Serial.println(this->rawBytes[0][0]);
      //Serial.println(sizeof(byte));
      //printlnHex(rawBytes[0], this->interfaces[0].hid.wDescriptorLength);

      if(logEnabled(BACKLOG_MALLOC)){
        for(int i = 0; i < this->Config.config.bNumInterfaces; i++){
          unsigned int ifaceLen = sizeof(byte) * this->interfaces[i].hid.wDescriptorLength;
          totalMalloced += ifaceLen;
          //Serial.print(F("Malloc: "));
          printString(HID_SA_Malloc);
          //Serial.print(F(": "));
          printColon(1);
          printSpace(1);
          Serial.println(ifaceLen);
        }
      } //END OF IF LOG ENABLEDBACKLOG_MALLOC
      
    }else{
      Serial.println(F("ERROR: Not enough memory!"));
      while(1);
    }

      

      if(false){
        for(int i = 0; i < this->Config.config.bNumInterfaces; i++) 
          printlnHex(rawBytes[i], this->interfaces[i].hid.wDescriptorLength);
       // Serial.println(":::::::::::::::::::::Starting HID REPORT:::::::::::::::::::::");
      }
      if(true){
        for(int i = 0; i < this->Config.config.bNumInterfaces; i++){
          printDash(10);
          printSpace(1);
          Serial.print(F("HID INTERFACE INDEX: "));
          Serial.println(i);
          int tabIndex = 0;
          int usageP = 0;
          for(int j = 0; j < this->interfaces[i].hid.wDescriptorLength; j++){

            uint8_t lenField = (rawBytes[i][j] & B00000011);

            uint8_t headerbLength = (lenField == 0x03) ? 4 : lenField;
            uint8_t headerbType   = rawBytes[i][j]   & B00001100;
            uint8_t headerbTag    = rawBytes[i][j]   & B11110000;
            
           // Serial.print("header: ");
            //j += headerbLength;
            j++;
            int value = 0;
            int8_t value8 = 0;
            int value16 = 0;
            switch(headerbLength){
              case 0:

                break;
              case 1:
                value8 = rawBytes[i][j]; 
                value = rawBytes[i][j];
                break;
              case 2:
                value = rawBytes[i][j] | (rawBytes[i][j+1] << 8);
                value16 = rawBytes[i][j] | (rawBytes[i][j+1] << 8);
               // Serial.println(rawBytes[i][j],HEX);
               // Serial.println(rawBytes[i][j+1],HEX);
                break;
              case 4:
                Serial.println("len=4");
                //value = rawBytes[i][j] | (rawBytes[i][j+1] << 8) | (rawBytes[i][j+2] << 16) | (rawBytes[i][j+3] << 24);
                break;
              default:
                Serial.println(F("FATAL ERROR: wrong size"));
                break;
            }
            j += headerbLength-1;
           // for(int m = 0; m < headerbLength; m)

            if(tabIndex){
              for(int k = 0; k < tabIndex; k++){
                printTab(2);
                if(k != tabIndex-1)
                  printPipe(1);
              }
            }
              
            switch(headerbType){
              case HID_RDH_MAIN:
                switch(headerbType | headerbTag){
                  case HID_RDH_INPUT:
                    printString(HID_SA_INPUT);
                    printColon(1);
                    printTab(2);
                    printMainbmParser(value8);
                    /*
_ws.col.info == "GET DESCRIPTOR Response HID Report"


Input (Const,Array,Abs)
    Header
    .... .... 1 = Data/constant: Constant     .... .... 0 = Data/constant: Data
    .... ...0 . = Data type: Array  .... ...1 . = Data type: Variable
    .... ..0. . = Coordinates: Absolute      Realtive

    .... .0.. . = Min/max wraparound: No Wrap
    .... 0... . = Physical relationship to data: Linear
    ...0 .... . = Preferred state: Preferred State
    ..0. .... . = Has null position: No Null position
    .0.. .... . = [Reserved]: False
    0... .... . = Bits or bytes: Buffered bytes (default, no second byte present)
      
                    */
                    //Serial.println(rawBytes[i][j]);
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_OUTPUT:
                    printString(HID_SA_OUTPUT);
                    printColon(1);
                    printTab(2);
                    printMainbmParser(value8);
                    //Serial.println(rawBytes[i][j]);
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_COLLECTION:
                    tabIndex++;
                    printString(HID_SA_Collection);
                    printColon(1);
                    printTab(1);
                    switch(value){
                      case 0x00:
                        Serial.println(F("Physical"));
                        break;
                      case 0x01:
                        Serial.println(F("Application"));
                        break;
                      default:
                      
                        Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                        break;
                    }
                    
                   // Serial.println();
                    break;
                  case HID_RDH_FEATURE:
                    printString(HID_SA_Feature);
                    printColon(1);
                    printTab(1);
                    printMainbmParser(value8);
                    //Serial.println(rawBytes[i][j]);
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_END_COLLECTION:
                    tabIndex--;
                    printString(HID_SA_ENDCollection);
                    //printColon(1);
                    //printSpace(1);
                    Serial.println();
                    if(tabIndex){
                      for(int k = 0; k < tabIndex; k++){
                        printTab(2);
                        if(k != tabIndex-1)
                          printPipe(1);
                      }
                    }
                    printUnder(30);
                    Serial.println();
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                }
                break;

              case HID_RDH_GLOBAL:
                switch(headerbType | headerbTag){
                  case HID_RDH_USAGEPAGE:
                    printString(HID_SA_UsagePage);
                    printColon(1);
                    //printSpace(1);
                    printTab(1);
                    usageP = value;
                    printUsage(value,0,false);
                    /*
                    switch(value){
                      case 0x01:
                        Serial.println(F("Generric Desktop Controls"));
                        break;
                      case 0x0c:
                        Serial.println(F("Consumer"));
                        break;
                      case 0x09:
                        Serial.println(F("Button"));
                        break;
                      case 0x07:
                        Serial.println(F("Keyboard/Keypad"));
                        break;
                      case 0x0a:
                        Serial.println(F("Ordinal"));
                        break;
                      case 0x0b:
                        Serial.println(F("Telephony"));
                        break;
                      default:
                        Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                        break;
                    }
                    //*/
                  //  Serial.println();
                    break;
                  case HID_RDH_LOGICAL_MIN:
                    printString(HID_SA_LogicalMin);
                    printColon(1);
                    printTab(1);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                  // Serial.println();
                    break;
                  case HID_RDH_LOGICAL_MAX:
                    printString(HID_SA_LogicalMax);
                    printColon(1);
                    printTab(1);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                   // Serial.println();
                    break;
                  case HID_RDH_PHYSICAL_MIN:
                    printString(HID_SA_PhysicalMin);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_PHYSICAL_MAX:
                    printString(HID_SA_PhysicalMax);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_UNIT_EXPONENT:
                    printString(HID_SA_UnitExponent);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println();
                    break;
                  case HID_RDH_UNIT:
                    printString(HID_SA_Unit);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_REPORT_SIZE:
                    printString(HID_SA_ReportSize);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? (uint16_t)value16 : ((headerbLength == 1) ? (uint8_t) value8 : value));
                    break;
                  case HID_RDH_REPORT_ID:
                    printString(HID_SA_ReportID);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value),HEX);
                    break;
                  case HID_RDH_REPORT_COUNT:
                    printString(HID_SA_ReportCount);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? (uint16_t) value16 : ((headerbLength == 1) ? (uint8_t)value8 : value));
                    break;
                  case HID_RDH_PUSH:
                    printString(HID_SA_PUSH);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_POP:
                    printString(HID_SA_POP);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                }
                break;

              case HID_RDH_LOCAL:
                switch(headerbType | headerbTag){
                  case HID_RDH_USAGE:
                    printString(HID_SA_Usage);
                    printColon(1);
                    //printTab(1);
                    printTab(2);
                    printUsage(usageP,value,true);
                    /*
                    switch(value){
                      case 0x00:
                        Serial.println(F("Reserved"));
                        break;
                      case 0x01:
                        Serial.println(F("Consumer Control"));
                        break;
                      case 0x80:
                        Serial.println(F("System Control"));
                        break;
                      case 0x06:
                        Serial.println(F("Keyboard"));
                        break;
                      case 0x02:
                        Serial.println(F("Mouse"));
                        break;
                      default:
                        switch(headerbLength){
                          case 0:
                            Serial.println();
                            break;
                          case 1:
                            Serial.println(value8);
                            break;
                          case 4:
                            Serial.println();
                            break;
                          default:
                            Serial.print(rawBytes[i][j-1],HEX);
                            printSpace(1);
                            Serial.println(rawBytes[i][j],HEX);
                            break;
                        }
                        //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                        //Serial.println(value16);
                        break;
                    } //*/
                    //Serial.println();
                    break;
                  case HID_RDH_USAGE_MIN:
                    printString(HID_SA_UsageMin);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                   // Serial.println(headerbLength);
                    switch(headerbLength){
                      case 1:
                        Serial.println(rawBytes[i][j], HEX);
                        break;
                      case 2:
                        Serial.println(value16, HEX);
                        break;
                    }
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value), HEX);
                    break;
                  case HID_RDH_USAGE_MAX:
                    printString(HID_SA_UsageMax);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    //Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value), HEX);
                     switch(headerbLength){
                      case 1:
                        Serial.println(rawBytes[i][j], HEX);
                        break;
                      case 2:
                        Serial.println(value16, HEX);
                        break;
                    }
                    break;
                  case HID_RDH_DESIGNATOR_INDEX:
                    printString(HID_SA_DesignatorIndex);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_DESIGNATOR_MIN:
                    printString(HID_SA_DesignatorMin);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_DESIGNATOR_MAX:
                    printString(HID_SA_DesignatorMax);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value));
                    break;
                  case HID_RDH_STRING_INDEX:
                    printString(HID_SA_StringIndex);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value), HEX);
                    break;
                  case HID_RDH_STRING_MIN:
                    printString(HID_SA_StringMin);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? (uint16_t)value16 : ((headerbLength == 1) ? (uint8_t)value8 : value));
                    break;
                  case HID_RDH_STRING_MAX:
                    printString(HID_SA_StringMax);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? (uint16_t)value16 : ((headerbLength == 1) ? (uint8_t)value8 : value));
                    break;
                  case HID_RDH_DELIMITER:
                    printString(HID_SA_Delimiter);
                    printColon(1);
                    printTab(1);
                    //Serial.println(rawBytes[i][j]);
                    Serial.println((headerbLength == 2) ? value16 : ((headerbLength == 1) ? value8 : value), HEX);
                    break;
                }
                break;
            }
            
            /*
        printColon(1);
        printSpace(1);
        Serial.println(rawBytes[i][j]); //*/
          }
          printDash(10);
          printSpace(1);
          Serial.print(F("END"));
          printSpace(1);
          printDash(10);
          Serial.println();
        }//end of for interfaces
      }
      if(false){ //this->Config.config.bNumInterfaces
      for(int i = 1; i < 2; i++)
        for(int j = 0; j < this->interfaces[i].hid.wDescriptorLength; j++){
          //Serial.println(getString(HID_RDH_S_PHYSICAL_MIN));
         // delay(100);
          uint16_t UsagePageLength = (uint16_t)(rawBytes[i][j] & B00000011); // 1 or 2
          uint16_t UsagePageValue = (UsagePageLength == 1) ?  rawBytes[i][j+1] : (rawBytes[i][j+1] | (rawBytes[i][j+2] << 8)) ;  // ?

          uint16_t UsageLength = (uint16_t)(rawBytes[i][j+1+UsagePageLength] & B00000011); // 1 or 2
          uint16_t UsageValue = (UsageLength == 1) ?  rawBytes[i][j+2+UsagePageLength] : (rawBytes[i][j+2+UsagePageLength] | (rawBytes[i][j+3+UsagePageLength] << 8)) ;  // ?

          if( (!(rawBytes[i][j] & B11110000) && ((rawBytes[i][j] & B00001100) == B00000100)) &&
              (rawBytes[i][j+1+UsagePageLength + 1 + UsageLength] == 0xa1) ){
          
            if((rawBytes[i][j] & 0x03) == 0x01){
              printString(HID_SA_UsagePage);
              printColon(1);
              printTab(1);
              Serial.println(UsagePageValue);
            }else{
              printString(HID_SA_UsagePage);
              printColon(1);
              printTab(1);
              Serial.println(UsagePageValue, HEX);
            }
            if((rawBytes[i][j] & 0x03) == 0x01){
              printString(HID_SA_Usage);
              printColon(1);
              printTab(2);
              Serial.print(UsageValue);
              Serial.println();
            }else{
              //Serial.print("Usage(vendor):\t\t");
             // Serial.print(hu);
              printString(HID_SA_Usage);
              //Serial.print(F(":\t\t"));
              printColon(1);
              printTab(2);
              Serial.println(UsageValue, HEX);
            }
            
            //Serial.println(F("Collection"))
            //printString(HID_SA_Collection);
            //Serial.println();

            j = j+2+UsagePageLength + UsageLength;
            int countCOLL = 0;
            int colIndex = 0;
            do{
              switch(rawBytes[i][j]){
                case 0xa1:
                  countCOLL++;
                  printString(HID_SA_Collection);
                  Serial.println();
                  colIndex++;
                  break;
                case 0xc0:
                  countCOLL--;
                  Serial.println();
                  printString(HID_SA_ENDCollection);
                  Serial.println();
                  colIndex++;
                  break;
                default:
                  int hLen = rawBytes[i][j] == 0x03 ? 4 : rawBytes[i][j];
                  switch(rawBytes[i][j] & 252){
                   // case HID_RDH_REPORT_ID:
                      //printString(HID_SA_ReportID);
                      //Serial.println();
                     // break;
                    default:
                      Serial.print(rawBytes[i][j]);
                  }
                  colIndex += hLen;
                  printSpace(1);
                  break;
              }
              /*
              if(rawBytes[i][j] == 0xa1){
                countCOLL++;
                printString(HID_SA_Collection);
                Serial.println();
                colIndex++;
              }else if(rawBytes[i][j] == 0xc0){
                countCOLL--;
                printString(HID_SA_ENDCollection);
                Serial.println();
                colIndex++;
              }else{
                
                //if(colIndex == 0){
                  //if(rawBytes[i][j] == 0x01){
                  //  Serial.println(F("Collection Type: Application"));
                  //}
                  //colIndex++;
                //}else{
                  int hLen = rawBytes[i][j] == 0x03 ? 4 : rawBytes[i][j];
                  switch(rawBytes[i][j] & 252){
                    case HID_RDH_REPORT_ID:
                      //printString(HID_SA_ReportID);
                      //Serial.println();
                      break;
                    default:
                      Serial.print(rawBytes[i][j]);
                  }
                  
                  colIndex += hLen;
                //}
              
                printSpace(1);
              }
              //*/
              //do something with collection
              if(countCOLL)
                j++;
            }while(countCOLL > 0) ;
            
          }else{
            Serial.println(this->interfaces[i].hid.wDescriptorLength);
            printHex(rawBytes[i], this->interfaces[i].hid.wDescriptorLength);
            Serial.println(F("FATAL ERROR!!!"));
            while(1);
          }
          
        }
        //printHex(rawBytes,this->interfaces[i].hid.wDescriptorLength);
      }
      //Serial.flush();
//*/

    if(logEnabled(BACKLOG_PP_ENUMERATION)){
     // Serial.println("test");
      printDevice();
      printConfig();
     // while(1);
      int length = this->Config.config.bNumInterfaces;
      for(int index = 0; index < length; index++){
        printUnder(6); //______
       // Serial.print("IFACE(");
       printString(HID_SA_INTERFACE);
        //Serial.print(F("(")); 
        printRoundOpen(1);
        Serial.print(index);
        printRoundClose(1);
        //Serial.print(F(")")); 
        printUnder(21); //_____________________");
        Serial.println();
        
        printString(HID_SA_bInterfaceNumber);
        printTab(1);
        Serial.println(this->interfaces[index].interface.bInterfaceNumber);
        
        printString(HID_SA_bAlternateSetting);
        printTab(1);
        Serial.println(this->interfaces[index].interface.bAlternateSetting);
        //if(this->interfaces[index].interface.bNumEndpoints != 1){
            //String("NumOfEPs\t")+
            
        printString(HID_SA_bNumEndpoints);
        printTab(2);
        Serial.println(this->interfaces[index].interface.bNumEndpoints);
        //}
          
    
        printString(HID_SA_bInterfaceClass);
        printTab(2);
        Serial.print(this->interfaces[index].interface.bInterfaceClass);
        //Serial.print(F("("));
        printRoundOpen(1);
        switch(this->interfaces[index].interface.bInterfaceClass){
          case 0x03:
            //Serial.print(F("HID"));
            printString(HID_SA_HID);
            break;
          case 0x01:
            Serial.print(F("Audio"));
            break;
          //case 0x02:
            //Serial.print("COM&CDC_CTRL");
            //break;
            /*
          case 0x05:
            Serial.print("Physical");
            break;
          case 0x06:
            Serial.print("Image");
            break;
          case 0x07:
            Serial.print("Printer");
            break;
          case 0x08:
            Serial.print("Mass Storage");
            break;
          case 0x09:
            Serial.print("HUB DEVICE");
            break;
          //case 0x0a:
            //Serial.print("CDC_Data");
            //break;
          case 0x0b:
            Serial.print("Smart Card");
            break;
          //case 0x0d:
            //Serial.print("Content Security");
            //break;
          case 0x0e:
            Serial.print("Video");
            break;
          case 0x10:
            Serial.print("Audio/Video Devices");
            break;
          /*
          case 0x0f:
            Serial.print("Personal Healthcare");
            break;
          case 0x11:
            Serial.print("Billboard Device Class");
            break;
          case 0x12:
            Serial.print("USB Type-C Bridge Class");
            break;
          case 0x13:
            Serial.print("USB Bulk Display Protocol Device Class");
            break;
          case 0x14:
            Serial.print("Interface	MCTP over USB Protocol Endpoint Device Class");
            break;
          case 0x3c:
            Serial.print("Interface	I3C Device Class");
            break;
          case 0xdc:
            Serial.print("Diagnostic Device");
            break;
          case 0xe0:
            Serial.print("Wireless Controller");
            break;
          case 0xef:
            Serial.print("Miscellaneous");
            break;
            //
          case 0xfe:
            Serial.print("Application Specific");
            break;
          case 0xff:
            Serial.print("Vendor Specific");
            break;*/
        }
        printRoundClose(1);
        Serial.println();
        //if(this->interfaces[index].interface.bInterfaceSubClass){
          
          printString(HID_SA_bInterfaceSubClass);
          printTab(1);
          Serial.print(this->interfaces[index].interface.bInterfaceSubClass);
          switch(this->interfaces[index].interface.bInterfaceSubClass){
            case 0x01:
              Serial.print(F("(Boot)"));
          }
          Serial.println();
        //}
        //if(this->interfaces[index].interface.bInterfaceProtocol){
          
          printString(HID_SA_bInterfaceProtocol);
          printTab(1);
          Serial.print(this->interfaces[index].interface.bInterfaceProtocol);
          switch(this->interfaces[index].interface.bInterfaceProtocol){
            case 0x01:
              Serial.print(F("(Keyboard)"));
              break;
            case 0x02:
              Serial.print(F("(Mouse)"));
              break;
          }
          Serial.println();
        //}
        
        printString(HID_SA_iInterface);
        printTab(2);
        Serial.println(this->interfaces[index].interface.iInterface);
        //END OF INTERFACE

        //START OF ENDPOINT
        printUnder(6); // ______       
        printString(HID_SA_ENDPOINT);
        //Serial.print(F("("));
        printRoundOpen(1);
        Serial.print(index);
        printRoundClose(1);
        //Serial.print(F(")"));
        printUnder(24); //________________________
        Serial.println();
        //Serial.print(String("EPAddr\t\t"));
        printString(HID_SA_bEndpointAddress);
        printTab(1);
        Serial.print(this->interfaces[index].endpoint.bEndpointAddress,HEX);
        Serial.println(String(F("(EP:"))+(this->interfaces[index].endpoint.bEndpointAddress & 0x0F)+String(F(")")));
        
        
        printString(HID_SA_bmAttributes);
        printTab(2);
        Serial.print(this->interfaces[index].endpoint.bmAttributes);
        if(this->interfaces[index].endpoint.bmAttributes == 0x03){
          Serial.print(F("(Interrupt)"));
        }
        Serial.println();
        
        
        printString(HID_SA_wMaxPacketSize);
        printTab(2);
        Serial.println(this->interfaces[index].endpoint.wMaxPacketSize);
        //if(this->interfaces[index].endpoint.bInterval > 1)
        
        printString(HID_SA_bInterval);
        printTab(2);
        Serial.println(this->interfaces[index].endpoint.bInterval);
        //END OF ENDPOINT
        
        //START OF HID  
        printUnder(6);//______
        
        printString(HID_SA_HID);
        //Serial.print(F("("));
        printRoundOpen(1);
        Serial.print(index);
        //Serial.print(F(")"));
        printRoundClose(1);
        printUnder(23); //(")_______________________"));
        Serial.println();
        
        printString(HID_SA_bcdHID);
        printTab(3);
        //Serial.print(String("bcdHID\t\t\t"));
        Serial.println(this->interfaces[index].hid.bcdHID,HEX);
        
        
        printString(HID_SA_bCountryCode);
        printTab(2);
        Serial.println(this->interfaces[index].hid.bCountryCode);
        //if(this->interfaces[index].hid.bNumDescriptors != 1)
        
        printString(HID_SA_bNumDescriptors);
        printTab(2);
        Serial.println(this->interfaces[index].hid.bNumDescriptors);
        
        //Serial.print(String("ReportType\t0x"));
        
        printString(HID_SA_bReportDescriptorType);
        printTab(1);
        Serial.print(this->interfaces[index].hid.bReportDescriptorType,HEX);
        switch(this->interfaces[index].hid.bReportDescriptorType){
          case 0x22:
            //Serial.print(F("("));
            printRoundOpen(1);
            printString(HID_SA_HID);
            printRoundClose(1);
            //Serial.print(F(")"));
            break;
        }
        Serial.println();
        
        printString(HID_SA_wDescriptorLength);
        printTab(1);
        Serial.println(this->interfaces[index].hid.wDescriptorLength);
        //END OF HID  */
      } //END OF FOR
    } //END OF IF LOG ENABLED BACKLOG_PP_ENUMERATION

    if(logEnabled(BACKLOG_MALLOC)){
      Serial.print(F("Total"));
      printSpace(1);
      printString(HID_SA_Malloc);
      printColon(1);
      printSpace(1);
      //Serial.print(F(": "));
      Serial.print(totalMalloced);
      printTab(1);
      Serial.print(F("Free Memory"));
      printColon(1);
      printSpace(1);
      Serial.println(freeMemory());
    } //END OF IF LOG ENABLEDBACKLOG_MALLOC

  } //END OF ENUMERATION

  void emptyDescriptors(){
    this->Device.bLength = 0;
    this->Device.bDescriptorType = 0;
    this->Device.device.bcdUSB = 0;
    this->Device.device.bDeviceClass = 0;
    this->Device.device.bDeviceSubClass = 0;
    this->Device.device.bDeviceProtocol = 0;
    this->Device.device.bMaxPacketSize0 = 0;
    this->Device.device.idVendor = 0;
    this->Device.device.idProduct = 0;
    this->Device.device.bcdDevice = 0;
    this->Device.device.iManufacturer = 0;
    this->Device.device.iProduct = 0;
    this->Device.device.iSerialNumber = 0;
    this->Device.device.bNumConfigurations = 0;

    this->Config.bLength = 0;
    this->Config.bDescriptorType = 0; //0x02(config desc)
    this->Config.config.wTotalLength = 0;
    this->Config.config.bNumInterfaces = 0;  //2 in this case
    this->Config.config.bConfigurationValue = 0;  //0x01
    this->Config.config.iConfiguration = 0; //0x00 no string desc for configuration
    this->Config.config.bmAttributes = 0; 
    this->Config.config.bMaxPower = 0;
  }
    void printDevice(){
    printUnder(6); //______
    

    //Serial.print("DEVICE");
    
printString(HID_SA_DEVICE);
    printUnder(23); //_______________________
    Serial.println();
    printString(HID_SA_bLength);
    printTab(3);
    Serial.println(this->Device.bLength);
    
    printString(HID_SA_bDescriptorType);
    printTab(2);
    Serial.println(this->Device.bDescriptorType);
    //Serial.print("bcdUSB\t\t");
    
    printString(HID_SA_bcdUSB);
    printTab(3);
    Serial.println(this->Device.device.bcdUSB,HEX);

    printString(HID_SA_bDeviceClass);
    printTab(2);
    Serial.println(this->Device.device.bDeviceClass);
    
    printString(HID_SA_bDeviceSubClass);
    printTab(2);
    Serial.println(this->Device.device.bDeviceSubClass);

    printString(HID_SA_bDeviceProtocol);    
    printTab(2);
    Serial.println(this->Device.device.bDeviceProtocol);
    
    //Serial.print("bMaxPacketSize0\t");
    
    

    printString(HID_SA_bMaxPacketSize0);
    printTab(2);
    Serial.println(this->Device.device.bMaxPacketSize0);
    //*
        
    printString(HID_SA_idVendor);
    printTab(2);
    //Serial.print("idVendor\t");
    Serial.println(this->Device.device.idVendor,HEX);
    
    //Serial.print("idProduct\t");
    
    printString(HID_SA_idProduct);
    printTab(2);
    Serial.println(this->Device.device.idProduct,HEX);
    

    //Serial.print(String("bcdDevice\t\t"));
    
    printString(HID_SA_bcdDevice);
    printTab(2);
    Serial.println(this->Device.device.bcdDevice,HEX);
    
    
    printString(HID_SA_iManufacturer);
    printTab(2);
    Serial.println(this->Device.device.iManufacturer);
    
    
    printString(HID_SA_iProduct);
    printTab(2);
    Serial.println(this->Device.device.iProduct);
    
    
    printString(HID_SA_iSerialNumber);
    printTab(2);
    Serial.println(this->Device.device.iSerialNumber);  
    
    
    printString(HID_SA_bNumConfigurations);
    printTab(1);
    Serial.println(this->Device.device.bNumConfigurations);

    //printString(HID_SA_wTotalLength);
    //printString(HID_SA_bNumInterfaces);
    //if(this->Device.device.bNumConfigurations != 1){

    //}
      
      //*/
  }
  
  void printConfig(){
    printUnder(6); // ______
    //Serial.print("CONFIGURATION");
    printString(HID_SA_CONFIGURATION);
    printUnder(16); //________________
    Serial.println();
    printString(HID_SA_bLength);
    printTab(3);
    Serial.println(this->Config.bLength);
    
    
    printString(HID_SA_bDescriptorType);
    printTab(2);
    Serial.println(this->Config.bDescriptorType);

    printString(HID_SA_wTotalLength);
    printTab(2);
//    Serial.print("wTotalLen\t");
    Serial.println(this->Config.config.wTotalLength);
//*
    printString(HID_SA_bNumInterfaces);
    printTab(2);
    Serial.println(this->Config.config.bNumInterfaces);

    //Serial.print("bConfigVal\t");  
    printString(HID_SA_bConfigurationValue);
    printTab(1);
    Serial.println(this->Config.config.bConfigurationValue);

    //Serial.print("iConfig\t\t"); 
    
    printString(HID_SA_iConfiguration);
    printTab(2);
    Serial.println(this->Config.config.iConfiguration);

    printString(HID_SA_bmAttributes);
    printTab(2);
    Serial.print(this->Config.config.bmAttributes);
  
    if((this->Config.config.bmAttributes & (1 << 6)) == (1 << 6)){
      printSpace(1);
      Serial.print(F("Self powered"));
    }else
      Serial.print(F(" Bus powered"));
    if((this->Config.config.bmAttributes & (1 << 5)) == (1 << 5)){
      Serial.print(F(", RWU"));
    }
    Serial.println();

    //Serial.print("bMaxPower\t");
    printString(HID_SA_bMaxPower);
    printTab(2);
    Serial.print(this->Config.config.bMaxPower*2);
    Serial.print(F("mA"));
    Serial.println();
    //*/

  }
};



USBHost::USBHost(byte backlog) : _status(0), _vbusState(SE0), _backlog(backlog), _firstRun(true){}
USBHost::USBHost() :          _status(0), _vbusState(SE0), _backlog(NO_BACKLOG), _firstRun(true){}

#define MAX3421_H
#endif