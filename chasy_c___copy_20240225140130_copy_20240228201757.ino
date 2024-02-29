#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
AsyncWebServer server(80);
int selected = 0;
int entered = 0;
#define CLK 13
#define DT 12
#define SW 14
#include <GyverEncoder.h>
Encoder enc(CLK, DT, SW);
int dd = 10;
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
const char *ssid = "Matvei";
const char *password = "44444444";
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char *PARAM_INPUT_1 = "input1";
const char *PARAM_INPUT_2 = "input2";
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_INPUT_4 = "input4";
const char *PARAM_INPUT_5 = "input5";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
  </form><br>
  <form action="/get">
    input4: <input type="text" name="input4">
    <input type="submit" value="Submit">
  </form>
  </form><br>
  <form action="/get">
    input5: <input type="text" name="input5">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void displaymenu(void) {
  enc.tick();
    if (enc.isHolded() and dd == 10) {
    display.clearDisplay();
    dd = 0;
    entered = -1;
    delay(50);
  }
  if (enc.isLeft() and dd == 0) {
    selected = selected + 1;
    delay(50);
  };

  if (enc.isRight() and dd == 0) {
    selected = selected - 1;
    delay(50);
  };

  if (enc.isClick() and dd == 0) {
    entered = selected;
    dd = 10;
    delay(50);
  };

  const char *options[6] = {
    " Время ",
    " Menu option 2 ",
    " Menu option 3 ",
    " Menu option 4 ",
    " Menu option 5 ",
    " IP адреса "

  };
  if (entered == -1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(utf8rus("Центральное Меню"));
    display.println("");
    for (int i = 0; i < 7; i++) {
      if (i == selected) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println(utf8rus(options[i]));
      } else if (i != selected) {
        display.setTextColor(SSD1306_WHITE);
        display.println(utf8rus(options[i]));
      }
    }
  } else if (entered == 0) {
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    String formattedTime = timeClient.getFormattedTime();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    int currentSecond = timeClient.getSeconds();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    display.clearDisplay();
    display.setTextSize(2);
    if (currentHour > 9) {
      display.setCursor(17, 25);
      display.println(currentHour);
    } else {
      display.setCursor(17, 25);  //12 pixels
      display.println("0");
      display.setCursor(29, 25);
      display.println(currentHour);
    }
    display.setCursor(42, 25);
    display.println(":");
    if (currentMinute > 9) {
      display.setCursor(52, 25);
      display.println(currentMinute);
    } else {
      display.setCursor(52, 25);  //12 pixels
      display.println("0");
      display.setCursor(64, 25);
      display.println(currentMinute);
    }
    display.setCursor(76, 25);
    display.println(":");
    if (currentSecond > 9) {
      display.setCursor(86, 25);
      display.println(currentSecond);
    } else {
      display.setCursor(86, 25);  //12 pixels
      display.println("0");
      display.setCursor(98, 25);
      display.println(currentSecond);
    }

    display.display();
  } else if (entered == 1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("Menu option 2");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println("PAST SIMPLEV-ed ili 2-form nepr gl otric i vopr did");
  } else if (entered == 2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("Menu option 3");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println("PRESENT PERFECT have/has -ed 3-form nepr gl he,she,it has ost have");
  } else if (entered == 3) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("Menu option 4");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println(utf8rus("turn into: превратиться, превратить turn off: выключить turn on: включить turn in ложиться"));
  } else if (entered == 4) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("Menu option 5");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("More cool stuff :)");
  } else if (entered == 5) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("IP v4, v6");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println(WiFi.localIP());
  } else if (entered == 6) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Custom menu test"));
    display.println("Menu option 7");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("More cool stuff :)");
  }
  display.display();
}

void setup() {
  Serial.begin(115200);

  enc.setType(TYPE2);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  delay(2000);
  display.cp437(true);
  display.clearDisplay();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 5);
  display.println(utf8rus("Подключение к сети"));
  display.setCursor(45, 20);
  display.println("Wi-fi");
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  timeClient.begin();
  timeClient.setTimeOffset(18000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
          dd=10;
      entered = -10;
      display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(inputMessage);
  }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    } else if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage = request->getParam(PARAM_INPUT_5)->value();
      inputParam = PARAM_INPUT_5;
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}
int frame = 0;
void loop() {
  displaymenu();
}
/* Recode russian fonts from UTF-8 to Windows-1251 */
String utf8rus(String source) {
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length();
  i = 0;
  while (i < k) {
    n = source[i];
    i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0:
          {
            n = source[i];
            i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
            break;
          }
        case 0xD1:
          {
            n = source[i];
            i++;
            if (n == 0x91) {
              n = 0xB8;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
            break;
          }
      }
    }
    m[0] = n;
    target = target + String(m);
  }
  return target;
}