//https://miliohm.com/rc522-rfid-reader-with-nodemcu/ NodeMCU RFID-RC522 bağlantı ve kodları
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <TimeLib.h>

String izinliler[] =       //İTU kartlarının id'leri uzun oldugu icin string olarak tanımladık
{
  "962273927",    //mavi sey
  "10212118473",  //Kenan Arslan
  "41104034",     //Serdar Bayraktar
  "1431071250",   //Alperen Şatıroğlu
  "161297110",    //Seyithan Karpuz
  "21710011214",  //Teoman Beyhun
  "13712617814",  //Ali Nabizadeh
  "207481249",    //Baran Ekşi
  "1112245164",   //Göksel Örsçekiç
  "2331689614",   //Murathan Bakır
  "751118573"     //Ufuk Köksal Yücedağ
};
int sayac = 0;  //ardisik kac kere giris yapıildi bilgisi

constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

Servo myservo;
String tag;
String prvtag;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  myservo.attach(D0, 500, 2400);
  myservo.write(0);
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial())
  {
    Serial.println("Okunuyor..."+ String(sayac) + " saniye gecti!");
    for (byte i = 0; i < 4; i++)
    {
      tag += rfid.uid.uidByte[i];
    }

    Serial.print("Kart ID: ");
    Serial.println(tag);
    bool flag = false;                                    //izinliler icindeyse true olur, degilse false devam der
    
    if (sayac==0)                                         //daha once izinli bir kart okunmamissa buraya girer
    {
      for (int i = 0; i < sizeof izinliler; i++)     //int size'i 4 oldugu icin 4'e bolduk sizeof(izinliler)/16
      {
         if (tag == izinliler[i])
        {
        sayac += 1;
        flag = true;                                      //eger okunan id izinliyse flag = true
        prvtag = tag;                                   //okunan kart hafizaya alınır
        Serial.println("Kapi Acildi!");
        Serial.println("----------------------");
        myservo.write(0);
        delay(1000);
        break;
        }
      
        else
        {  
          if (!flag);             //izinliler icerisinde tag daha önce bulunmuşsa sayacı sifirlar
          {
          sayac = 0;
          }
          Serial.println("Kapi Kilitlendi!");
          Serial.println("----------------------");
          myservo.write(100);
        }
      }
    }
    else
    {
      if (tag == prvtag)              //kilitlemek icin okunan kartın aynı kart oldugunu kontrol eder
      {
        if (sayac >= 4)
        {
          Serial.println("Kapi Kilitlendi!");
          Serial.println("----------------------");
          myservo.write(100);
          sayac = 0;
          prvtag = "p";
          //delay(3000);    //kilitlendikten sonra 3 saniye boyunca okumayacak
          rfid.PICC_HaltA();  //kilitlendikten sonra kartı çekene kadar okumayacak
          }
        sayac += 1;
        delay(1000);       //sayac 5 olana kadar her saniye test edecek
        
        }
      else                 //baska kart okunursa sayac sifirlanir ve kod yeniden baslar
      {
        sayac = 0;
        
        }
        
    }
  }
  tag = "";
  //rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
