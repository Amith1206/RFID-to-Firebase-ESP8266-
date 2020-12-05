#include <Hash.h> //using sha1 hash
#include <SPI.h> // SPI library- Serial Peripheral Interface Protocol
#include <MFRC522.h> // RFID library (https://github.com/miguelbalboa/rfid)
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://nodemcu-1206.firebaseio.com/"      
#define FIREBASE_AUTH "YtEadYbnayiO7jIrytZo1z7eWaBJ3EjuQ8uUcFEk"            
#define WIFI_SSID "Tenda_2B1C00"                                  
#define WIFI_PASSWORD "Rex@mra81"

#define SS_PIN D4 //Slave Select
#define RST_PIN D2 // Reset Pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // Set up mfrc522 on the Arduino
FirebaseData firebaseData;
FirebaseJson json;
void setup() {
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init();// Initialize Proximity Coupling Device (PCD)--to emit and capture EM waves
  Serial.begin(9600); // open serial connection
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase
 
}
 
void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
    if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
      Serial.print("RFID TAG ID:");
      String id;
      for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
          id= id + (mfrc522.uid.uidByte[i]); // 11 bit id
      }
      String resultid = sha1(id);//Encrypting id using SHA1 Algorithm
      Serial.println(" " + resultid); // Print out of encrypted id is complete.
      delay(2000);
      json.set("chipid",resultid);
      json.set("readerid","H101");
      if (Firebase.pushJSON(firebaseData, "/rfid/id", json)) {

  Serial.println(firebaseData.dataPath());

  Serial.println(firebaseData.pushName());

  Serial.println(firebaseData.dataPath() + "/"+ firebaseData.pushName());

} else {
  Serial.println(firebaseData.errorReason());
}
//      Firebase.setInt(firebaseData,"/Push/Int",1);
//      if (firebaseData.error()) 
//    {
// 
//      Serial.print("pushing /logs failed:");
//      Serial.println(Firebase.error()); 
//      return;
//  }
    }
  }
}
