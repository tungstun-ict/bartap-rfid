#pragma once

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         0          // Configurable, see typical pin layout above
#define SS_PIN          2         // Configurable, see typical pin layout above

namespace barTap{
  class nfc{
    private:
      //MFRC522
      MFRC522 mfrc522 = MFRC522(SS_PIN, RST_PIN);

    public:
      nfc();

      void getInfo();
      int readClassic();
      int readUL();

      int read(){
          int result0 = readClassic();
          if (result0 >= 0){
              return result0;
          }//else
          result0 = readUL();
          if (result0 >= 0){
              return result0;
          }//else
          return -1;
      };
  };
}