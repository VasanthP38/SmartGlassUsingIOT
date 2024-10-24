#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char *ssid = "Root";
const char *password = "rootroot";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

String arr_days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String date_time;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    display.print("...");
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  int textWidth = 12; //text size
  int textHeight = 16; //text size
  int startX = (SCREEN_WIDTH - (textWidth * 6)); //starting X position
  int startY = (SCREEN_HEIGHT - textHeight) / 2; //starting Y position

  display.setCursor(2, startY);
  display.println("ACTIVATED");
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH / 2; x++) {
      uint16_t leftPixel = display.getPixel(x, y);
      uint16_t rightPixel = display.getPixel(SCREEN_WIDTH - 1 - x, y);
      display.drawPixel(x, y, rightPixel);
      display.drawPixel(SCREEN_WIDTH - 1 - x, y, leftPixel);
    }
  }
  display.display();
  delay(3000);
  timeClient.begin();
}

void loop()
{
  timeClient.update();

  display.clearDisplay();

  //Serial.println(timeClient.getFormattedTime());

  display.setTextSize(2);
  display.setCursor(0, 0);
  int textHeight = 16;
  int startY = (SCREEN_HEIGHT - textHeight) / 2;
  display.setCursor(0, startY);
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();

  if (hh > 12)
  {
    hh = hh - 12;
    display.print(hh);
    display.print(":");
    display.print(mm);
    display.print(":");
    display.print(ss);
    display.println("PM");
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.println("PM");

  }
  else
  {
    display.print(hh);
    display.print(":");
    display.print(mm);
    display.print(":");
    display.print(ss);
    display.println("AM");
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.println("AM");
  }

  //Mirror the display
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH / 2; x++) {
      uint16_t leftPixel = display.getPixel(x, y);
      uint16_t rightPixel = display.getPixel(SCREEN_WIDTH - 1 - x, y);
      display.drawPixel(x, y, rightPixel);
      display.drawPixel(SCREEN_WIDTH - 1 - x, y, leftPixel);
    }
  }

  display.display();

  delay(100);
}
