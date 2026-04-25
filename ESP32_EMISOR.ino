#include <esp_now.h>
#include <WiFi.h>


#define RXD2 16
#define TXD2 17

uint8_t broadcastAddress[] = {0x34, 0x5F, 0x45, 0xAB, 0xE6, 0x78};

typedef struct struct_message {
  char dato;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  WiFi.mode(WIFI_STA); 

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

}

void loop() {
  if (Serial2.available()) {
    char dato = Serial2.read();

    myData.dato = dato;

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  }
}
