#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <string>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// define pins of the rgb led(s)
int ledPinBlue = 2;
int ledPinGreen = 3;
int ledPinRed = 4;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct user_data{
    String firstName;
    uint_fast32_t id;
    String lastName;
    String mail;
    String username;
};

struct person_data{
    int id;
    user_data userData;

    person_data() : id(0), userData(user_data{
      "firstName",
      0,
      "lastname",
      "mail",
      "username"
      }
    ) {}

    person_data(int id, const user_data &userData) : id(id), userData(userData) {}
};

//Display drawers
void draw_name(const user_data& userData){
  String firstName = userData.firstName;
  
  if(firstName.length() > 10){
    firstName = firstName.substring(0,8);
    firstName += "..";
  }
  
  //convert uint64_t to int;
  unsigned int id = userData.id;
  
  //clear display ahead of new items
  display.clearDisplay();

  //make filled rectangle for white welkom background
  display.fillRect(0,0,128,20,SSD1306_WHITE);

  //first inverse welkom text on 1x scale
  display.setCursor(1,3);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println(F("Welkom"));

  //first name text on 2x scale
  display.setCursor(1,23);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println(firstName);

  //last name text on 1x scale
  display.setCursor(1,40);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(userData.lastName);

  //last name text on 1x scale
  display.setCursor(1,55);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(id, HEX);

  //display items on display
  display.display();
}

void draw_idle_screen(String barName, const String& sessionName){
  //check string length
  if(barName.length() > 10){
    barName = barName.substring(0,8);
    barName += "..";
  }
  
  //clear display ahead of new items
  display.clearDisplay();

  //make filled rectangle for white welkom background
  display.fillRect(0,0,128,20,SSD1306_WHITE);

  //first inverse welkom text on 1x scale
  display.setCursor(1,3);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println(barName);

  //last name text on 1x scale
  display.setCursor(1,23);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(sessionName);

  //last name text on 1x scale
  display.setCursor(1,46);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("Open de app en houd\nhet tegen het scherm!"));

  //display items on display
  display.display();
}

void draw_not_valid(){  
  //clear display ahead of new items
  display.clearDisplay();

  //make filled rectangle for white welkom background
  display.fillRect(0,0,128,20,SSD1306_WHITE);

  //first inverse welkom text on 1x scale
  display.setCursor(1,3);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println(F(" Not Valid"));
 
  //display items on display
  display.display();
}

//RFID storage for person_data
class RFID_Storage{
  private:
    person_data person_data_storage[255];
    int index = 0;

  public:
    bool add(const person_data& id){
        if(index <= 255) {
            person_data_storage[index] = id;
            index++;
            return true;
        }
        return false;
    }

    signed int locate_id(const uint_fast32_t & id){
        for(int i = 0; i < index; i++){
            if(person_data_storage[i].userData.id == id){
                return i;
            }
        }
        return -1;
    }

    person_data get_from_id(const uint_fast32_t& id){
        signed int location = locate_id(id);
        if(location >= 0) {
            return person_data_storage[location];
        }return person_data();
    }
};

class Server_temp{
    //currently not not using the api just predefined numbers.
  private:
    RFID_Storage& rfidStorage;

    person_data personData0{
            0,
            user_data{
                "Daan",
                1,
                "Kerbusch",
                "daankerbusch@gmail.com",
                "Strijderdaan"
            }
    };

    person_data personData1{
            0,
            user_data{
                    "Apple",
                    2,
                    "lastApple",
                    "Apple@gmail.com",
                    "AppleBest"
            }
    };

    person_data personData2{
            0,
            user_data{
                    "Pear",
                    3,
                    "lastPear",
                    "Pear@gmail.com",
                    "PearBest"
            }
    };

    person_data personData3{
            0,
            user_data{
                    "Orange",
                    4,
                    "lastOrange",
                    "Orange@gmail.com",
                    "OrangeBest"
            }
    };


  public:
    Server_temp(RFID_Storage &rfidStorage) : rfidStorage(rfidStorage) {
        rfidStorage.add(personData0);
        rfidStorage.add(personData1);
        rfidStorage.add(personData2);
        rfidStorage.add(personData3);
    }
};

class encryption32{
  private:
    uint_fast32_t key;

  public:
    encryption32(uint_fast32_t key) : key(key) {}

    uint_fast32_t encrypt(const uint_fast32_t& id){
        return id^key;
    }

    uint_fast32_t decrypt(const uint_fast32_t& id){
        return key^id;
    }
};

//Function for setting the color of the leds
void setRGB(const int& red, const int& green, const int& blue){
  if(red > 255 || red < 0 || green > 255 || green < 0 || blue > 255 | blue < 0){
    return;
  }
  analogWrite(ledPinRed, red);
  analogWrite(ledPinGreen, green);
  analogWrite(ledPinBlue, blue);
}

class RFID_Control{
private:
    Server_temp& server;
    RFID_Storage& rfidStorage;

    const uint_fast32_t encryption_key;

public:
    RFID_Control(
            Server_temp &server,
            RFID_Storage &rfidStorage,
            const uint_fast32_t encryptionKey) :
        server(server),
        rfidStorage(rfidStorage),
        encryption_key(encryptionKey)
    {}

    void run(uint_fast32_t input){
        //uint_fast32_t input = rfidReader.read(true);
        //input = encryption32(encryption_key).decrypt(input);
        if(rfidStorage.locate_id(input) >= 0){
            person_data personData = rfidStorage.get_from_id(input);

            draw_name(personData.userData);
            
            Serial.println(personData.userData.firstName);
            Serial.println(personData.userData.lastName);

            setRGB(0,255,0);

            delay(2000);
            setRGB(0,0,0);
            draw_idle_screen("Bartjes BAR", "Oudjaarsavond");
            
        }else{
            draw_not_valid();
            
            setRGB(255,0,0);
            Serial.println(F("unknown id"));
            delay(250);
            setRGB(0,0,0);
            delay(250);
            setRGB(255,0,0);
            delay(250);
            setRGB(0,0,0);
            delay(1250);

            draw_idle_screen("Bartjes BAR", "Oudjaarsavond");
        }
    }
};



void setup() {
  //Set pinmode of the rgb pins
  pinMode(ledPinBlue, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  
  Serial.begin(9600);

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  RFID_Reader reader;

  RFID_Storage storage;

  Server_temp server(storage);

  uint_fast32_t key = 0x14D2405C;

  RFID_Control rfidControl(server, storage, key);

  setRGB(0,0,0);

  draw_idle_screen("Bartjes BAR", "Oudjaarsavond");

  delay(2000);

  rfidControl.run();
 
}

void loop() {
  
}
