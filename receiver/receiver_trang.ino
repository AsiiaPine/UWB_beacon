/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp8266-nodemcu/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <math.h>


float BaseDist =  0.60 ; // 60 cm

uint8_t broadcastAddress[] = {0x5C, 0xCF, 0x7F, 0x8E, 0xD6, 0xE9};
String incomingMsg;
String AString ;
String success;

float A;
float B;

float dist;
float ang;
float cos_a;

typedef struct struct_message {
    String msg;
} struct_message;

struct_message incomingMessage;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  incomingMsg = incomingMessage.msg;

  printIncomingMessage();
}

void printIncomingMessage()
{
  // Display Readings in Serial Monitor
  // Serial.println("INCOMING MESSAGE");


  if (incomingMsg[0] == '1' )
    {
      int pos = incomingMsg.indexOf(':');
      int Mpos = incomingMsg.indexOf('m');
      if (pos > 0)               {
        AString = incomingMsg.substring(pos + 1, Mpos - 1);
      

      char carray[AString.length() + 1];
      AString.toCharArray(carray, sizeof(carray));
      A = atof(carray);
      // Serial.println(A);
    }}


  else if (incomingMsg[0] == '2'  )
    {
      int pos = incomingMsg.indexOf(':');
      int Mpos = incomingMsg.indexOf('m');
      if (pos > 0)        {     
        AString = incomingMsg.substring(pos + 1, Mpos - 1);
      

      char carray[AString.length() + 1];
      AString.toCharArray(carray, sizeof(carray));
      B = atof(carray);
      //  Serial.println(B);
    }}

    // cos_B = (L ** 2 + l_2 ** 2 - l_1 ** 2) / (2 * L * l_2)

    // print(f"cos A: {cos_B}")

    // # if np.abs(cos_A) > 1:
    // #     cos_A = np.sign(cos_A) * np.nextafter(1, 0)
    // # midpoint btw tags
    // a = L / 2
    // d = np.sqrt(a ** 2 + l_2 ** 2 - 2 * a * l_2 * cos_B)
    // cos_alpha = (d ** 2 + a ** 2 - l_2 ** 2) / (2 * d * a)
    // print(f"cos alpha: {cos_alpha}")
double cos_B = (pow(BaseDist, 2) + pow(B, 2) - pow(A, 2)) / (2 * BaseDist * B);

// Serial.print("cos A: ");
// Serial.println(cos_B);

// if (abs(cos_A) > 1) {
//   cos_A = sign(cos_A) * nextafter(1, 0);
// }
// midpoint btw tags
double a = BaseDist/ 2;
double d = sqrt(pow(a, 2) + pow(B, 2) - 2 * a * B * cos_B);
double cos_alpha = (pow(d, 2) + pow(a, 2) - pow(B, 2)) / (2 * d * a);

Serial.print("alpha: ");
Serial.println((acos(cos_alpha)/PI * 180)-90);
Serial.print("dist: ");
Serial.println(d);





  // Serial.println();
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}