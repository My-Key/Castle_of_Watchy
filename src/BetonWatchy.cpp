#include "BetonWatchy.h"

const int posBatteryX = 15;
const int posBatteryY = 15;

const int posBatteryFillX = posBatteryX + 2;
const int posBatteryFillY = posBatteryY + 14;

const int posTimeCenterX = 100;
const int posTimeY = 110;

const int posAMPMTimeX = 160;
const int posAMPMTimeY = 140;

const int posDateX = 25;
const int posDate1Y = 150;

const int posDate2Y = 175;

const int posStepsIconX = 137;
const int posStepsIconY = 15;

const int posStepsBGX = 51;
const int posStepsBGY = 25;

const int posStepsOffsetY = 22;

const float VOLTAGE_MIN = 3.2;
const float VOLTAGE_MAX = 4.1;
const float VOLTAGE_RANGE = 0.9;

BetonWatchy::BetonWatchy()
{
  // Serial.begin(115200);
}

void BetonWatchy::drawWatchFace()
{
  display.fillScreen(GxEPD_BLACK);
  //display.drawBitmap(0, 0, epd_bitmap_BG, 200, 200, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);

  drawTime();
  drawDate();
  //drawSteps();
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

  display.setCursor(100 - w / 2, posTimeY);
  display.print(timeString);

  display.drawFastHLine(100 - w / 2 - 15, posTimeY, w + 30, GxEPD_WHITE);
  display.drawFastHLine(100 - w / 2 - 5, posTimeY + 1, w + 10, GxEPD_WHITE);

  if (HOUR_12_24 != 12)
    return;

  display.setFont(&BLKCHCRY12pt7b);
  display.setCursor(100 + w / 2, posTimeY);
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

  display.setCursor(100 - w / 2, posDate1Y);
  display.println(dayOfWeek);

  String day = String(currentTime.Day) + String(Ordinal(currentTime.Day));
  String date = day + String("    ") + month;

  display.getTextBounds(date, 0, 0, &x1, &y1, &w, &h);

  int width = w;

  display.setCursor(100 - w / 2, posDate2Y);
  display.print(date);

  display.drawFastHLine(100 - w / 2 - 10, posDate2Y + 1, w + 20, GxEPD_WHITE);
  
  display.getTextBounds(day, 0, 0, &x1, &y1, &w, &h);
  
  display.setCursor(100 - width / 2 + w + 4, posDate2Y + 10);
  display.print("of");
}

void BetonWatchy::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    
    uint32_t stepCount = sensor.getCounter();

    //display.drawBitmap(posStepsIconX, posStepsIconY, epd_bitmap_Shell, 48, 50, GxEPD_BLACK);
    
    //display.drawBitmap(posStepsBGX, posStepsBGY, epd_bitmap_Steps_BG, 85, 30, GxEPD_BLACK);

    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(stepCount), 0, 0, &x1, &y1, &w, &h);

    display.setFont(&BLKCHCRY12pt7b);
    display.setCursor(posStepsIconX - 10 - w, posStepsBGY + posStepsOffsetY);
    display.println(stepCount);
}

void BetonWatchy::drawBattery()
{
  float VBAT = getBatteryVoltage();

  // 12 battery states
  int batState = int(((VBAT - VOLTAGE_MIN) / VOLTAGE_RANGE) * 186);
  if (batState > 186)
    batState = 186;
  if (batState < 0)
    batState = 0;

  display.fillRect(6, 6, batState, 32, GxEPD_WHITE);

  display.drawBitmap(6, 6, epd_bitmap_Battery_Mask, 186, 32, GxEPD_BLACK);
  
  display.drawBitmap(0, 0, epd_bitmap_Battery_BG, 200, 72, GxEPD_WHITE);
}
