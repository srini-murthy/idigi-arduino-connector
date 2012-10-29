/*
 * Copyright (c) 2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <iDigi.h>

extern "C" {
 #include <stdlib.h>
 #include <stdio.h> 
}

// ===========================================================================
// iDigiDiaDataStreamT.ino: send temperature data samples to iDigi using Dia
//                          sample format
//                         
// To use this sample you must have a simple temperature sensor such as the
// Microchip MCP9700 (or similar).  Temperature sensors such as the MCP9700
// are called thermistors.  They usually have three pins.  Two pins are
// connected between power and ground and a third acts the sensor output
// reading.  In this sample, the output is connected to an analog input
// pin of the Arduino.
// 
// This sample collects the analog input values, transforms them to a
// temperature and then periodically uploads these sample to iDigi.  The
// data may be viewed by logging into your iDigi account and navigating
// to Data Services->Data Streams.
// ===========================================================================

// -------------------------------------------
// ITEMS YOU SHOULD CONFIGURE FOR YOUR ARDUINO
// -------------------------------------------
#define ETHERNET_DHCP 1                          // Set to 1 if you want to use DHCP   
#define IDIGI_SERVER       "my.idigi.com"        // iDigi server hostname to use
#define IDIGI_DEVICE_NAME  "Arduino Mega"        // How your device will be labelled on iDigi
#define IDIGI_VENDOR_ID    0x03000009            // If you don't know what this is, leave it alone :)
byte mac[] =                                     // Set this to the MAC address of your Ethernet shield
    { 0x90, 0xA2, 0xDA, 0x05, 0x00, 0x57 };      // iDigi Device ID will be 00000000-00000000-90A2DAFF-FF050057

#if(ETHERNET_DHCP == 0)
IPAddress ip(192, 168, 1, 42);                   // If not using DHCP, set this IP address
IPAddress gw(192, 168, 1, 1);                    // Set to your router's address
IPAddress nameserver(8, 8, 8, 8);                // Set to your nameserver
IPAddress subnet(255, 255, 255, 0);              // Set your subnet mask
#endif /* ETHERNET_DHCP */

#define SAMPLE_PERIOD   15000                    // How often to sample sensor
#define UPLOAD_PERIOD   60000                    // How often to upload to iDigi
#define T_ANALOG_INPUT  A8                       // Which Analog input pin to use
#define T_CALIBRATION   -3.91                    // Sensor calibration offset
/// -------------------------------------------
///         END OF CONFIGURATION ITEMS
/// -------------------------------------------

bool idigi_connected = false;
iDigiDiaDataset dataset(8, "arduino");
unsigned long int counter = 0;
long int next_sample_time = 0;
long int next_upload_time = 0;

void setup() {
  String deviceId;
  
  // Setup the Arduino's to be an analog input:
  pinMode(T_ANALOG_INPUT, INPUT);
  
  Serial.begin(9600);
  Serial.println("Starting up...");
 
  Serial.println("Starting Ethernet..."); 
#if(ETHERNET_DHCP == 0)
  // Static IP Configuration
  Ethernet.begin(mac, ip, nameserver, gw, subnet);
  Serial.println("Starting iDigi...");
  iDigi.setup(mac, ip, IDIGI_VENDOR_ID, IDIGI_SERVER, IDIGI_DEVICE_NAME);
  Serial.println("iDigi started!");
#else
  // DHCP Configuration
  Ethernet.begin(mac);
  Serial.println("Starting iDigi...");
  iDigi.setup(mac, Ethernet.localIP(), IDIGI_VENDOR_ID, IDIGI_SERVER, IDIGI_DEVICE_NAME);
  Serial.println("iDigi started!");
#endif /* ETHERNET_DHCP */
  Serial.println("Ethernet started!");
  delay(500);

  iDigi.getDeviceIdString(&deviceId);
  Serial.print("iDigi Device ID: ");
  Serial.println(deviceId);
  
  next_sample_time = ((long) millis()) + SAMPLE_PERIOD;
  next_upload_time = ((long) millis()) + UPLOAD_PERIOD;
}

// A simple function to scale floating point values; compare with the
// Arduino map() function:
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Converts a float to an ASCII string of fixed-point precision:
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long w = (long)f;
  sprintf(a, "%lu", w);
  while (*a != '\0') a++;
  *a++ = '.';
  long decimal = abs((long)((f - w) * p[precision]));
  sprintf(a,"%lu",decimal);
  return ret;
}

void loop() {
  long int now = (long) millis();
  char value[32] = { 0 };
  
  if (idigi_connected ^ iDigi.isConnected())  // detect change in iDigi status
  {
    idigi_connected = iDigi.isConnected();
    Serial.print("iDigi");
    if (idigi_connected)
    {
      Serial.println(" connected.");
    } else {
      Serial.println(" disconnected.");
    }
  }
  
  if (idigi_connected && (now - next_sample_time >= 0) && dataset.spaceAvailable())
  {
    // time for a sensor reading, collect it and tranform it to a temperature:
    double temperature = fmap(analogRead(T_ANALOG_INPUT), 0.0, 1023.0, 0.0, 5.0);
    temperature = ((temperature - 0.5)/0.01) + T_CALIBRATION;
    
    ftoa(value, temperature, 2);
    Serial.print("Adding ");
    Serial.print(temperature);
    Serial.println(" to dataset.");
    dataset.add("temperature", value, "C");
    counter++;
    next_sample_time = ((long) millis()) + SAMPLE_PERIOD;
  }
  
  if (idigi_connected && (now - next_upload_time >= 0) && dataset.size() > 0)
  {
    // time to upload to iDigi!
    Serial.print("putDiaDataSet: about to upload ");
    Serial.print(dataset.size());
    Serial.println(" samples.");
    size_t bytesUploaded = iDigi.dataService.putDiaDataset(&dataset);
    
    if (bytesUploaded < 0)
    {
      Serial.print("putDiaDataSet: error returned from putDiaDataSet(");
      Serial.print(bytesUploaded, DEC);
      Serial.println(")");
      
      return;
    } else {
      Serial.print("putDiaDataSet: uploaded ");
      Serial.print(bytesUploaded, DEC);
      Serial.println(" bytes");
    }

    dataset.clear();
    next_upload_time = ((long) millis()) + UPLOAD_PERIOD;
  }

  iDigi.step();
}

