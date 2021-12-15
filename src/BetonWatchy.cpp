#include "BetonWatchy.h"

const int posBatteryX = 0;
const int posBatteryY = 2;

const int posBatteryFillX = posBatteryX + 5;
const int posBatteryFillY = posBatteryY + 6;

const int posTimeCenterX = 100;
const int posTimeY = 105;

const int posDateXCenter = 100;
const int posDate1Y = 150;

const int posDate2Y = 175;

const float VOLTAGE_MIN = 3.4;
const float VOLTAGE_MAX = 4.2;
const float VOLTAGE_RANGE = VOLTAGE_MAX - VOLTAGE_MIN;

BetonWatchy::BetonWatchy()
{
  // Serial.begin(115200);
}

void BetonWatchy::drawWatchFace()
{
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);

  drawTime();
  drawDate();
  drawBattery();
}

void BetonWatchy::drawTime()
{
  display.setFont(&BLKCHCRY40pt7b);

  bool am = currentTime.Hour < 12;
  int hour = currentTime.Hour;

  if (HOUR_12_24 == 12)
    hour = ((hour+11)%12)+1;
    
  int16_t  x1, y1;
  uint16_t w, h;

  String timeString = "";
  
  if (hour < 10)
    timeString += "0";
  
  timeString += String(hour);

  timeString += ":";

  if (currentTime.Minute < 10)
    timeString += "0";

  timeString += String(currentTime.Minute);

  display.getTextBounds(timeString, 0, 0, &x1, &y1, &w, &h);

  display.setCursor(posTimeCenterX - w / 2, posTimeY);
  display.print(timeString);

  display.drawFastHLine(posTimeCenterX - w / 2 - 15, posTimeY, w + 30, GxEPD_WHITE);
  display.drawFastHLine(posTimeCenterX - w / 2 - 5, posTimeY + 1, w + 10, GxEPD_WHITE);

  if (HOUR_12_24 != 12)
    return;

  display.setFont(&BLKCHCRY12pt7b);
  display.setCursor(posTimeCenterX + w / 2, posTimeY);
  display.print(am ? "AM" : "PM");
}

const char* BetonWatchy::Ordinal(uint8_t num)
{
  switch(num % 100)
  {
      case 11:
      case 12:
      case 13:
        return "th";
  }

  switch(num % 10)
  {
      case 1:
        return "st";
      case 2:
        return "nd";
      case 3:
        return "rd";
      default:
        return "th";
  }
}

void BetonWatchy::drawDate()
{
  display.setFont(&BLKCHCRY12pt7b);

  String dayOfWeek = dayStr(currentTime.Wday);
  String month = monthStr(currentTime.Month);
  
  int16_t  x1, y1;
  uint16_t w, h;
  
  display.getTextBounds(dayOfWeek, 0, 0, &x1, &y1, &w, &h);

  display.setCursor(posDateXCenter - w / 2, posDate1Y);
  display.println(dayOfWeek);

  String day = String(currentTime.Day) + String(Ordinal(currentTime.Day));
  String date = day + String("    ") + month;

  display.getTextBounds(date, 0, 0, &x1, &y1, &w, &h);

  int width = w;

  display.setCursor(posDateXCenter - w / 2, posDate2Y);
  display.print(date);

  display.drawFastHLine(posDateXCenter - w / 2 - 10, posDate2Y + 1, w + 20, GxEPD_WHITE);
  
  display.getTextBounds(day, 0, 0, &x1, &y1, &w, &h);
  
  display.setCursor(posDateXCenter - width / 2 + w + 4, posDate2Y + 10);
  display.print("of");
}

void BetonWatchy::drawBattery()
{
  float VBAT = getBatteryVoltage();

  // 12 battery states
  int batState = int(((VBAT - VOLTAGE_MIN) / VOLTAGE_RANGE) * 191);
  if (batState > 191)
    batState = 191;
  if (batState < 0)
    batState = 0;

  display.fillRect(posBatteryFillX, posBatteryFillY, batState, 25, GxEPD_WHITE);

  display.drawBitmap(posBatteryFillX, posBatteryFillY, epd_bitmap_Battery_Mask_2, 191, 25, GxEPD_BLACK);
  
  display.drawBitmap(posBatteryX, posBatteryY, epd_bitmap_Battery_BG_2, 200, 45, GxEPD_WHITE);
}
