//--- Kütüphaneleri çağırıyoruz ---//
#include <Wire.h> // I2C için şart
#include <LiquidCrystal_I2C.h> // LCD kütüphanesi
#include <ThreeWire.h> // DS1302 haberleşmesi için
#include <RtcDS1302.h> // DS1302 saat modülü için

//--- Pinleri Tanımlıyoruz ---//
// LCD'nin I2C adresi genelde 0x27 veya 0x3F olur. Ekranın arkasında yazar veya I2C scanner ile bulabilirsin.
// 16x2'lik bir ekran olduğunu varsayıyorum.
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Saat modülünün pinlerini bağlıyoruz, seninkilerle aynı
ThreeWire myWire(6, 5, 7); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () {
  Serial.begin(9600); // Ne olur ne olmaz, seri porttan da bakalım
  
  //--- LCD'yi başlatıyoruz ---//
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Sistem Hazir...");
  delay(1500);

  //--- RTC'yi (saati) başlatıyoruz ---//
  Rtc.Begin();

  // --- BU KISIM ÇOK ÖNEMLİ! --- //
  // Saati ilk defa ayarlamak için aşağıdaki satırların başındaki // işaretlerini kaldır,
  // kodu bir kere yükle. Saat ayarlandıktan sonra bu satırları tekrar // ile kapatıp
  // kodu BİR DAHA YÜKLE. Yoksa Arduino her yeniden başladığında saati sıfırlar.
  Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__)); // Bilgisayarın saatiyle ayarlar
  // Rtc.SetIsWriteProtected(true); // Ayarladıktan sonra yazma koruması aç
  
  if (!Rtc.IsDateTimeValid()) {
    // Saat ayarlı değilse veya pil bitikse ekrana yazı yazalım
    lcd.clear();
    lcd.print("RTC Ayarli Degil!");
    while(1); // burada takılı kalsın
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC yazmaya karsi korumali.");
  } else {
    Serial.println("RTC yazmaya karsi korumali DEGIL.");
  }
}

// Bu ufak fonksiyon, sayı 10'dan küçükse başına '0' koyar. (Örn: 7 -> 07)
// Görüntü daha düzgün olur.
// Hata vermemesi için loop() fonksiyonunun üstüne taşıdık.
void printIkiHane(int sayi) {
  if (sayi < 10) {
    lcd.print("0");
  }
  lcd.print(sayi);
}

void loop () {
    // RTC'den güncel zamanı çekiyoruz
    RtcDateTime now = Rtc.GetDateTime();

    // LCD'nin imlecini en başa alalım (0. sütun, 0. satır)
    lcd.setCursor(0, 0);

    // Tarihi yazdıralım
    printIkiHane(now.Day());
    lcd.print("/");
    printIkiHane(now.Month());
    lcd.print("/");
    lcd.print(now.Year());

    // İmleci alt satıra alalım (0. sütun, 1. satır)
    lcd.setCursor(0, 1);
    
    // Saati yazdıralım
    printIkiHane(now.Hour());
    lcd.print(":");
    printIkiHane(now.Minute());
    lcd.print(":");
    printIkiHane(now.Second());

    // Her saniye güncellesin yeter, abartmaya gerek yok
    delay(1000); 
}