#ifndef DEFINES_H




 //MAIN
#define HID_RDH_MAIN 0x00
//MAIN HEADER TYPE
#define HID_RDH_INPUT 0x80
#define HID_RDH_OUTPUT 0x90
#define HID_RDH_COLLECTION 0xa0
#define HID_RDH_FEATURE 0xb0
#define HID_RDH_END_COLLECTION 0xc0



//GLOBAL
#define HID_RDH_GLOBAL 0x04
//GLOBAL HEADER TYPE
#define HID_RDH_USAGEPAGE B00000100
#define HID_RDH_LOGICAL_MIN B00010100
#define HID_RDH_LOGICAL_MAX B00100100
#define HID_RDH_PHYSICAL_MIN B00110100
#define HID_RDH_PHYSICAL_MAX B01000100
#define HID_RDH_UNIT_EXPONENT B01010100
#define HID_RDH_UNIT B01100100
#define HID_RDH_REPORT_SIZE B01110100
#define HID_RDH_REPORT_ID B10000100
#define HID_RDH_REPORT_COUNT B10010100
#define HID_RDH_PUSH B10100100
#define HID_RDH_POP B10110100



//LOCAL
#define HID_RDH_LOCAL 0X08
//LOCAL HEADER TYPE
#define HID_RDH_USAGE B00001000
#define HID_RDH_USAGE_MIN B00011000
#define HID_RDH_USAGE_MAX B00101000
#define HID_RDH_DESIGNATOR_INDEX B00111000
#define HID_RDH_DESIGNATOR_MIN B01001000
#define HID_RDH_DESIGNATOR_MAX B01011000
#define HID_RDH_STRING_INDEX B01111000
#define HID_RDH_STRING_MIN B10001000
#define HID_RDH_STRING_MAX B10011000
#define HID_RDH_DELIMITER B10101000



#define HID_SA_UsagePage 0
#define HID_SA_Usage 11
#define HID_SA_LogicalMin 17
#define HID_SA_UsageMin 29
#define HID_SA_LogicalMax 39
#define HID_SA_UsageMax 51
#define HID_SA_PhysicalMin 61
#define HID_SA_DesignatorIndex 74
#define HID_SA_PhysicalMax 91
#define HID_SA_DesignatorMin 104
#define HID_SA_UnitExponent 119
#define HID_SA_DesignatorMax 133
#define HID_SA_Unit 148
#define HID_SA_ReportSize 153
#define HID_SA_StringIndex 165
#define HID_SA_INPUT 178
#define HID_SA_ReportID 184
#define HID_SA_StringMin 194
#define HID_SA_OUTPUT 205
#define HID_SA_ReportCount 212
#define HID_SA_StringMax 225
#define HID_SA_Collection 236
#define HID_SA_PUSH 247
#define HID_SA_Delimiter 252
#define HID_SA_Feature 262
#define HID_SA_POP 270
#define HID_SA_ENDCollection 274

#define HID_SA_bLength 289
#define HID_SA_bDescriptorType 297
#define HID_SA_bcdUSB 313
#define HID_SA_bDeviceClass 320
#define HID_SA_bDeviceSubClass 333
#define HID_SA_bDeviceProtocol 349
#define HID_SA_bMaxPacketSize0 365
#define HID_SA_idVendor 381
#define HID_SA_idProduct 390
#define HID_SA_bcdDevice 400
#define HID_SA_iManufacturer 410
#define HID_SA_iProduct 424
#define HID_SA_iSerialNumber 433
#define HID_SA_bNumConfigurations 447
#define HID_SA_wTotalLength 466
#define HID_SA_bNumInterfaces 479
#define HID_SA_bConfigurationValue 494
#define HID_SA_iConfiguration 514
#define HID_SA_bmAttributes 529
#define HID_SA_bMaxPower 542
#define HID_SA_bcdHID 552
#define HID_SA_bCountryCode 559
#define HID_SA_bNumDescriptors 572
#define HID_SA_bReportDescriptorType 588
#define HID_SA_wDescriptorLength 610
#define HID_SA_bEndpointAddress 628
#define HID_SA_bmAttributes 645
#define HID_SA_wMaxPacketSize 658
#define HID_SA_bInterval 673
#define HID_SA_bInterfaceNumber 683
#define HID_SA_bAlternateSetting 700
#define HID_SA_bNumEndpoints 718
#define HID_SA_bInterfaceClass 732
#define HID_SA_bInterfaceSubClass 748
#define HID_SA_bInterfaceProtocol 767
#define HID_SA_iInterface 786



#define HID_SA_hrSUCCESS 797
#define HID_SA_hrBUSY 807
#define HID_SA_hrBADREQ 814
#define HID_SA_hrUNDEF 823
#define HID_SA_hrNAK 831
#define HID_SA_hrSTALL 837
#define HID_SA_hrTOGERR 845
#define HID_SA_hrWRONGPID 854
#define HID_SA_hrBADBC 865
#define HID_SA_hrPIDERR 873
#define HID_SA_hrPKTERR 882
#define HID_SA_hrCRCERR 891
#define HID_SA_hrKERR 900
#define HID_SA_hrJERR 907
#define HID_SA_hrTIMEOUT 914
#define HID_SA_hrBABBLE 924
#define HID_SA_Unkownerrorcode 933
#define HID_SA_Malloc 951
#define HID_SA_DEVICE 958
#define HID_SA_CONFIGURATION 965
#define HID_SA_INTERFACE 979
#define HID_SA_ENDPOINT 989
#define HID_SA_HID 998
#define HID_SA_SETUP 1002
#define HID_SA_IN 1008
#define HID_SA_ACK 1011
#define HID_SA_OUT 1015
#define HID_SA_ERROR 1019









#define NO_BACKLOG 0
#define BACKLOG_ARDUINO 0x01
#define BACKLOG_MAX 0x02
#define BACKLOG_CONDET 0x04
#define BACKLOG_USB_RAW 0x08
#define BACKLOG_REVISION 0x10
#define BACKLOG_PP_ENUMERATION 0x20 
#define BACKLOG_MALLOC 0x40


#define GET_DESCRIPTOR 0x06
#define DEVICE_DESCRIPTOR 1
#define CONFIGURATION_DESCRIPTOR 2
#define HIDREPORT_DESCRIPTOR 0x22



#define SERIAL_RX_BUFFER_SIZE 512
#define SERIAL_TX_BUFFER_SIZE 512


#define MAX_SS 10
//#define MAX_GPX 7
//#define MAX_INT 8
//#define MAX_RESET 9
//=============================================
#define SE0     0x00
#define SE1     0x01
#define FSHOST  0x02
#define LSHOST  0x03

#define rRCVFIFO    0x08    //1<<3  
#define rSNDFIFO    0x10    //2<<3
#define rSUDFIFO    0x20    //4<<3
#define rRCVBC      0x30    //6<<3
#define rSNDBC      0x38    //7<<3

#define rUSBIRQ     0x68    //13<<3
/* USBIRQ Bits  */
#define bmVBUSIRQ   0x40    //b6
#define bmNOVBUSIRQ 0x20    //b5
#define bmOSCOKIRQ  0x01    //b0

#define rUSBIEN     0x70    //14<<3
/* USBIEN Bits  */
#define bmVBUSIE    0x40    //b6
#define bmNOVBUSIE  0x20    //b5
#define bmOSCOKIE   0x01    //b0

#define rUSBCTL     0x78    //15<<3
/* USBCTL Bits  */
#define bmCHIPRES   0x20    //b5
#define bmPWRDOWN   0x10    //b4

#define rCPUCTL     0x80    //16<<3
/* CPUCTL Bits  */
#define bmPUSLEWID1 0x80    //b7
#define bmPULSEWID0 0x40    //b6
#define bmIE        0x01    //b0

#define rPINCTL     0x88    //17<<3
/* PINCTL Bits  */
#define bmFDUPSPI   0x10    //b4
#define bmINTLEVEL  0x08    //b3
#define bmPOSINT    0x04    //b2
#define bmGPXB      0x02    //b1
#define bmGPXA      0x01    //b0
// GPX pin selections
#define GPX_OPERATE 0x00
#define GPX_VBDET   0x01
#define GPX_BUSACT  0x02
#define GPX_SOF     0x03

#define rREVISION   0x90    //18<<3

#define rIOPINS1    0xa0    //20<<3

/* IOPINS1 Bits */
#define bmGPOUT0    0x01
#define bmGPOUT1    0x02
#define bmGPOUT2    0x04
#define bmGPOUT3    0x08
#define bmGPIN0     0x10
#define bmGPIN1     0x20
#define bmGPIN2     0x40
#define bmGPIN3     0x80

#define rIOPINS2    0xa8    //21<<3
/* IOPINS2 Bits */
#define bmGPOUT4    0x01
#define bmGPOUT5    0x02
#define bmGPOUT6    0x04
#define bmGPOUT7    0x08
#define bmGPIN4     0x10
#define bmGPIN5     0x20
#define bmGPIN6     0x40
#define bmGPIN7     0x80

#define rGPINIRQ    0xb0    //22<<3
/* GPINIRQ Bits */
#define bmGPINIRQ0 0x01
#define bmGPINIRQ1 0x02
#define bmGPINIRQ2 0x04
#define bmGPINIRQ3 0x08
#define bmGPINIRQ4 0x10
#define bmGPINIRQ5 0x20
#define bmGPINIRQ6 0x40
#define bmGPINIRQ7 0x80

#define rGPINIEN    0xb8    //23<<3
/* GPINIEN Bits */
#define bmGPINIEN0 0x01
#define bmGPINIEN1 0x02
#define bmGPINIEN2 0x04
#define bmGPINIEN3 0x08
#define bmGPINIEN4 0x10
#define bmGPINIEN5 0x20
#define bmGPINIEN6 0x40
#define bmGPINIEN7 0x80

#define rGPINPOL    0xc0    //24<<3
/* GPINPOL Bits */
#define bmGPINPOL0 0x01
#define bmGPINPOL1 0x02
#define bmGPINPOL2 0x04
#define bmGPINPOL3 0x08
#define bmGPINPOL4 0x10
#define bmGPINPOL5 0x20
#define bmGPINPOL6 0x40
#define bmGPINPOL7 0x80

#define rHIRQ       0xc8    //25<<3
/* HIRQ Bits */
#define bmBUSEVENTIRQ   0x01   // indicates BUS Reset Done or BUS Resume
#define bmRWUIRQ        0x02
#define bmRCVDAVIRQ     0x04
#define bmSNDBAVIRQ     0x08
#define bmSUSDNIRQ      0x10
#define bmCONDETIRQ     0x20
#define bmFRAMEIRQ      0x40
#define bmHXFRDNIRQ     0x80

#define rHIEN           0xd0    //26<<3

/* HIEN Bits */
#define bmBUSEVENTIE    0x01
#define bmRWUIE         0x02
#define bmRCVDAVIE      0x04
#define bmSNDBAVIE      0x08
#define bmSUSDNIE       0x10
#define bmCONDETIE      0x20
#define bmFRAMEIE       0x40
#define bmHXFRDNIE      0x80

#define rMODE           0xd8    //27<<3

/* MODE Bits */
#define bmHOST          0x01
#define bmLOWSPEED      0x02
#define bmHUBPRE        0x04
#define bmSOFKAENAB     0x08
#define bmSEPIRQ        0x10
#define bmDELAYISO      0x20
#define bmDMPULLDN      0x40
#define bmDPPULLDN      0x80

#define rPERADDR    0xe0    //28<<3

#define rHCTL       0xe8    //29<<3
/* HCTL Bits */
#define bmBUSRST        0x01
#define bmFRMRST        0x02
#define bmSAMPLEBUS     0x04
#define bmSIGRSM        0x08
#define bmRCVTOG0       0x10
#define bmRCVTOG1       0x20
#define bmSNDTOG0       0x40
#define bmSNDTOG1       0x80

#define rHXFR       0xf0    //30<<3 
/* Host transfer token values for writing the HXFR register (R30)   */
/* OR this bit field with the endpoint number in bits 3:0               */
#define tokSETUP  0x10  // HS=0, ISO=0, OUTNIN=0, SETUP=1
#define tokIN     0x00  // HS=0, ISO=0, OUTNIN=0, SETUP=0
#define tokOUT    0x20  // HS=0, ISO=0, OUTNIN=1, SETUP=0
#define tokINHS   0x80  // HS=1, ISO=0, OUTNIN=0, SETUP=0
#define tokOUTHS  0xA0  // HS=1, ISO=0, OUTNIN=1, SETUP=0
#define tokISOIN  0x40  // HS=0, ISO=1, OUTNIN=0, SETUP=0
#define tokISOOUT 0x60  // HS=0, ISO=1, OUTNIN=1, SETUP=0

#define rHRSL       0xf8    //31<<3

/* HRSL Bits */
#define bmRCVTOGRD  0x10
#define bmSNDTOGRD  0x20
#define bmKSTATUS   0x40
#define bmJSTATUS   0x80
#define bmSE0       0x00    //SE0 - disconnect state
#define bmSE1       0xc0    //SE1 - illegal state


#define hrSUCCESS   0x00
#define hrBUSY      0x01
#define hrBADREQ    0x02
#define hrUNDEF     0x03
#define hrNAK       0x04
#define hrSTALL     0x05
#define hrTOGERR    0x06
#define hrWRONGPID  0x07
#define hrBADBC     0x08
#define hrPIDERR    0x09
#define hrPKTERR    0x0A
#define hrCRCERR    0x0B
#define hrKERR      0x0C
#define hrJERR      0x0D
#define hrTIMEOUT   0x0E
#define hrBABBLE    0x0F



#define MODE_FS_HOST    (bmDPPULLDN|bmDMPULLDN|bmHOST|bmSOFKAENAB)
#define MODE_LS_HOST    (bmDPPULLDN|bmDMPULLDN|bmHOST|bmLOWSPEED|bmSOFKAENAB)


#define DEFAULT_BAUDRATE 115200
#define DEFAULT_START_DELAY 1000
#define DEFAULT_FIRST_PRINT F("")

#define DEFINES_H
#endif

/*
printString(HID_SA_UsagePage)
printString(HID_SA_Usage)
printString(HID_SA_LogicalMin)
printString(HID_SA_UsageMin)
printString(HID_SA_LogicalMax)
printString(HID_SA_UsageMax)
printString(HID_SA_PhysicalMin)
printString(HID_SA_DesignatorIndex)
printString(HID_SA_PhysicalMax)
printString(HID_SA_DesignatorMin)
printString(HID_SA_UnitExponent)
printString(HID_SA_DesignatorMax)
printString(HID_SA_Unit)
printString(HID_SA_ReportSize)
printString(HID_SA_StringIndex)
printString(HID_SA_INPUT)
printString(HID_SA_ReportID)
printString(HID_SA_StringMin)
printString(HID_SA_OUTPUT)
printString(HID_SA_ReportCount)
printString(HID_SA_StringMax)
printString(HID_SA_PUSH)
printString(HID_SA_Delimiter)
printString(HID_SA_Feature)
printString(HID_SA_POP)
printString(HID_SA_Collection)
printString(HID_SA_ENDCollection)


printString(HID_SA_bLength)
printString(HID_SA_bDescriptorType)
printString(HID_SA_bcdUSB)
printString(HID_SA_bDeviceClass)
printString(HID_SA_bDeviceSubClass)
printString(HID_SA_bDeviceProtocol)
printString(HID_SA_bMaxPacketSize0)
printString(HID_SA_idVendor)
printString(HID_SA_idProduct)
printString(HID_SA_bcdDevice)
printString(HID_SA_iManufacturer)
printString(HID_SA_iProduct)
printString(HID_SA_iSerialNumber)
printString(HID_SA_bNumConfigurations)


printString(HID_SA_wTotalLength)
printString(HID_SA_bNumInterfaces)
printString(HID_SA_bConfigurationValue)
printString(HID_SA_iConfiguration)
printString(HID_SA_bmAttributes)
printString(HID_SA_bMaxPower)


printString(HID_SA_bcdHID)
printString(HID_SA_bCountryCode)
printString(HID_SA_bNumDescriptors)
printString(HID_SA_bReportDescriptorType)
printString(HID_SA_wDescriptorLength)
printString(HID_SA_bEndpointAddress)
printString(HID_SA_bmAttributes)
printString(HID_SA_wMaxPacketSize)
printString(HID_SA_bInterval)

printString(HID_SA_bInterfaceNumber)
printString(HID_SA_bAlternateSetting)
printString(HID_SA_bNumEndpoints)
printString(HID_SA_bInterfaceClass)
printString(HID_SA_bInterfaceSubClass)
printString(HID_SA_bInterfaceProtocol)
printString(HID_SA_iInterface)

printString(HID_SA_hrSUCCESS)
printString(HID_SA_hrBUSY)
printString(HID_SA_hrBADREQ)
printString(HID_SA_hrUNDEF)
printString(HID_SA_hrNAK)
printString(HID_SA_hrSTALL)
printString(HID_SA_hrTOGERR)
printString(HID_SA_hrWRONGPID)
printString(HID_SA_hrBADBC)
printString(HID_SA_hrPIDERR)
printString(HID_SA_hrPKTERR)
printString(HID_SA_hrCRCERR)
printString(HID_SA_hrKERR)
printString(HID_SA_hrJERR)
printString(HID_SA_hrTIMEOUT)
printString(HID_SA_hrBABBLE)
printString(HID_SA_Unkownerrorcode)


printString(HID_SA_Malloc)

printString(HID_SA_SETUP)
printString(HID_SA_IN)
printString(HID_SA_ACK)
printString(HID_SA_OUT)
printString(HID_SA_ERROR)



printString(HID_SA_CONFIGURATION)
printString(HID_SA_INTERFACE)
printString(HID_SA_ENDPOINT)
printString(HID_SA_HID)




printString(HID_SA_bConfigurationValue)
printString(HID_SA_iConfiguration)
printString(HID_SA_bmAttributes)
printString(HID_SA_bMaxPower)

printString(HID_SA_bcdHID)
printString(HID_SA_bCountryCode)
printString(HID_SA_bNumDescriptors)
printString(HID_SA_bReportDescriptorType)
printString(HID_SA_wDescriptorLength)

printString(HID_SA_bEndpointAddress)
printString(HID_SA_bmAttributes)
printString(HID_SA_wMaxPacketSize)
printString(HID_SA_bInterval)

printString(HID_SA_bInterfaceNumber)
printString(HID_SA_bAlternateSetting)
printString(HID_SA_bNumEndpoints)
printString(HID_SA_bInterfaceClass)
printString(HID_SA_bInterfaceSubClass)
printString(HID_SA_bInterfaceProtocol)
printString(HID_SA_iInterface)
//*/