#include <Arduino.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <u8g2_fonts.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include "GB2312.h"

// extern const uint8_t chinese_city_gb2312[239032] U8G2_FONT_SECTION("chinese_city_gb2312");
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts; // 创建U8G2对象

/**
 * static const uint8_t MOSI  = 11; // SDA
 * static const uint8_t SCK   = 12; // SCL
 */
//******墨水屏初始化****** 128x296
GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ 15, /*DC=*/37, /*RST=*/36, /*BUSY=*/35)); // 第二批屏幕
// GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=5*/ 7, /*DC=*/4, /*RST=*/5, /*BUSY=*/6));  //第一批屏幕

char* findNextCharStart(const char* p) {
    if (p == NULL || *p == '\0') {
        return NULL;
    }

    do {
        p++;
    } while ((*p & 0xC0) == 0x80 && *p != '\0');

    return (*p == '\0') ? NULL : (char*)p;
}

void printUTF8Char(const char* start, const char* end) {
    while (start < end) {
        u8g2Fonts.print(*start); // 使用 u8g2 打印字符
        start++;
    }
}

void printWrappedText(int16_t x, int16_t y, const String& str, int16_t lineWidth, int16_t charSpacing) {
    int16_t cursorX = x;
    int16_t cursorY = y;
    char charArray[str.length() + 1];
    str.toCharArray(charArray, str.length() + 1);

    char* currentChar = charArray;
    char* nextChar;

    while (currentChar && *currentChar != '\0') {
        nextChar = findNextCharStart(currentChar);
        if (nextChar == NULL) {
            nextChar = currentChar + strlen(currentChar);
        }

        // Calculate the width of the current character
        char tempCharArray[nextChar - currentChar + 1];
        strncpy(tempCharArray, currentChar, nextChar - currentChar);
        tempCharArray[nextChar - currentChar] = '\0';

        uint16_t w = u8g2Fonts.getUTF8Width(tempCharArray);
        uint16_t h = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();

        if (cursorX + w > lineWidth) {
            cursorX = x;
            cursorY += h + 4; // Adjust line spacing
        }

        u8g2Fonts.setCursor(cursorX, cursorY);
        display.setPartialWindow(cursorX, cursorY - h/2-2, w, h + 2); // Ensure the full character is within the window
        display.firstPage();
        do {
            u8g2Fonts.setCursor(cursorX, cursorY);
            printUTF8Char(currentChar, nextChar);
        } while (display.nextPage());
        delay(10);
        
        cursorX += w + charSpacing; // Add character spacing
        currentChar = nextChar;
    }

}

void setup()
{
    Serial.begin(115200);
    display.init();
    u8g2Fonts.begin(display); // 将u8g2连接到display
    display.setRotation(3);   // 设置屏幕方向为竖屏
    display.firstPage();
    display.display(1);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); // 设置前景色
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE); // 设置背景色
    display.fillScreen(GxEPD_WHITE);
    u8g2Fonts.setFont(chinese_city_gb2312);
    u8g2Fonts.setCursor(0, 12);
    u8g2Fonts.print("古");
    u8g2Fonts.setCursor(14, 12);
    u8g2Fonts.print("诗");
    u8g2Fonts.setCursor(28, 12);
    u8g2Fonts.print("页");
    u8g2Fonts.setCursor(260, 12);
    printWrappedText(0, 12, "古诗页玩儿订sdf服赛凡森联网sd欧赛囧扽ad赛凡", 296,1);
    display.nextPage();
    String str = "你好啊";
    String str2 = "haa";
    Serial.println(str.length());
    Serial.println(u8g2Fonts.getUTF8Width(str.c_str()));
    Serial.println(str2);
    Serial.println(str2.length());
    Serial.println(str);
    Serial.println(str.length());
    Serial.println(u8g2Fonts.getUTF8Width(str.c_str()));
    Serial.println(str2);
    Serial.println(str2.length());
}

void loop()
{
}