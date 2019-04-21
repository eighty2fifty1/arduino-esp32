/*
 * https://twitter.com/wakwak_koba/
 * 
 * Multi Clients: OK
 * Automaric Re-Connect after disconnected: NC
 */

#include "BLEDevice.h"

static void notifyCallback(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)  {
  Serial.printf("notifyCallback: %s %s handle: %02x value:", pRemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress().toString().c_str(), pRemoteCharacteristic->getUUID().toString().c_str(), pRemoteCharacteristic->getHandle());
  for(int i=0; i<length; i++)
    Serial.printf(" %02x", pData[i]);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  BLEDevice::init("");
  static BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);

  Serial.println("wait 10 mins..");
  auto pScanResults = pBLEScan->start(10);
  Serial.println();
  
  for(int i=0; i < pScanResults.getCount(); i++)  {
    auto advertisedDevice = pScanResults.getDevice(i);
    if (advertisedDevice.haveServiceUUID()) {      
      Serial.print(advertisedDevice.getAddress().toString().c_str());
      Serial.print(" ");
      Serial.print(advertisedDevice.getAddressType());
      Serial.print(" ");
      Serial.println(advertisedDevice.getName().c_str());
      auto pClient = BLEDevice::createClient();
      if(pClient->connect(&advertisedDevice))
      {
        auto* pRemoteServiceMap = pClient->getServices();
        for (auto itr : *pRemoteServiceMap)  {
          Serial.print(" ");
          Serial.println(itr.second->toString().c_str());

          auto *pCharacteristicMap = itr.second->getCharacteristicsByHandle();
          for (auto itr : *pCharacteristicMap)  {
            Serial.print("  ");
            Serial.print(itr.second->toString().c_str());
            
            if(itr.second->canNotify()) {
              itr.second->registerForNotify(notifyCallback);
              Serial.print(" notify registered");
            }
            Serial.println();
            auto* pDescriptorMap = itr.second->getDescriptors();
            if(pDescriptorMap)
              for (auto itr : *pDescriptorMap)  {
                Serial.print("   ");
                Serial.println(itr.second->toString().c_str());
              }
          }
        }
      }
    }
  }
}

void loop() {
}
