#ifndef STRUCTS_H



typedef struct Result {
  boolean rcvTOG;
  boolean sndTOG;
  byte rcode;
} Result;


typedef struct Device{  //0x01 device descriptor
      uint16_t bcdUSB;
      byte bDeviceClass;
      byte bDeviceSubClass;
      byte bDeviceProtocol;
      byte bMaxPacketSize0;
      uint16_t idVendor;
      uint16_t idProduct;
      uint16_t bcdDevice;
      byte iManufacturer;
      byte iProduct;
      byte iSerialNumber;
      byte bNumConfigurations;
    } Device;


typedef struct Configuration{ //0x02 Configuration descriptor
      uint16_t wTotalLength;
      byte bNumInterfaces;
      byte bConfigurationValue;
      byte iConfiguration;
      byte bmAttributes;
      byte bMaxPower;
    } Configuration;

typedef struct Interface{  //0x04 Configuration descriptor
      uint8_t bInterfaceNumber;
      uint8_t bAlternateSetting;
      uint8_t bNumEndpoints;
      uint8_t bInterfaceClass;
      uint8_t bInterfaceSubClass;
      uint8_t bInterfaceProtocol;
      uint8_t iInterface;
    } Interface;

        
typedef struct Endpoint{  //0x05 Configuration descriptor of length 7
      uint8_t bEndpointAddress;
      uint8_t bmAttributes;
      uint16_t wMaxPacketSize;
      uint8_t bInterval;
    } Endpoint;

      
typedef struct HID{//0x21 Configuration descriptor
      uint16_t bcdHID;
      uint8_t bCountryCode;
      uint8_t bNumDescriptors;
      uint8_t bReportDescriptorType;
      uint16_t wDescriptorLength;
    } HID;

//DESCX

typedef struct Descriptor{
  uint8_t bLength;
  uint8_t bDescriptorType;
  union{
    Device device;
    Configuration config;
    Interface interface;
    Endpoint endpoint;
    HID hid;
  };
}Descriptor;


typedef struct InterfaceCollection{
  Interface interface;
  Endpoint endpoint;
  HID hid;
}InterfaceCollection;
/*
typedef struct EndpointSettings{
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t bInterval;
}EndpointSettings;
//*/



#define STRUCTS_H
#endif