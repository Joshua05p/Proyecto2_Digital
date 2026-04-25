#include <esp_now.h>
#include <WiFi.h>

#define RXD2 16
#define TXD2 17

typedef struct struct_message {
  char dato;
} struct_message;

struct_message incomingData;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  Serial2.write(incomingData.dato);
}

void setup() {

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();


  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  
}
