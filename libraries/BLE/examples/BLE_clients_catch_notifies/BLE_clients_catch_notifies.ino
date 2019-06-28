/*
 * https://twitter.com/wakwak_koba/
 */

#include "BLEDevice.h"
std::vector<BaseType_t> PinnedTasks;

static void notifyCallback(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)  {
  Serial.printf("notifyCallback: %s %s handle: %02x value:", pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress().toString().c_str(), pRemoteCharacteristic->getUUID().toString().c_str(), pRemoteCharacteristic->getHandle());
  for (int i=0; i<length; i++)
    Serial.printf(" %02x", pData[i]);
  Serial.println();
}

static void careBLEClient(BLEClient* pClient) {
  for(;;) {  
    if(pClient->isDisconnected() && pClient->connect()) {
      auto* pRemoteServiceMap = pClient->getServices();
      for (auto itr : *pRemoteServiceMap)  {
        auto *pCharacteristicMap = itr.second->getCharacteristicsByHandle();
        for (auto itr : *pCharacteristicMap)
          if(itr.second->canNotify())
            itr.second->registerForNotify(notifyCallback);
      }
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  BLEDevice::init("");
  auto* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);

  Serial.println("wait 10 secs..");
  auto pScanResults = pBLEScan->start(10);

  for (auto itr : pScanResults.getDevices()) {
    auto advertisedDevice = itr.second;
    if (advertisedDevice->haveServiceUUID())  {
      Serial.print("Found Device ");
      Serial.println(advertisedDevice->toString().c_str());
      PinnedTasks.push_back(xTaskCreatePinnedToCore([](void *p) {careBLEClient((BLEClient*)p);}
        , "careBLEClient", 4096, BLEDevice::createClient(advertisedDevice), 10, NULL, CONFIG_ARDUINO_RUNNING_CORE)
      );
    }
  }

  if(!PinnedTasks.size())
    ESP.restart();
}

void loop() {
}
