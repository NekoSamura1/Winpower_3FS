#include <Arduino.h>

#include "Winpower_3FS.h"

#define TIMER_RECIEVE 0

#define BUTTON_MENU 0xd
#define BUTTON_MINUS 0x2d
#define BUTTON_PLUS 0x2b

struct Data3FS data3FSLocal;

Winpower_3FS winpower_3FS(&Serial2);

// put function declarations here:
void handleMenu(void *);
void handleButtons(void *);

void setup()
{
  // put your main code here, to run once:
  Serial.begin(921600);
  Serial.write('\n');

  winpower_3FS.sendCommand(1, 0, 4, 2);
  winpower_3FS.updateData(true);
  // memcpy(&data3FSLocal, &data3FS, sizeof(Data3FS) + 1);
  data3FSLocal = winpower_3FS.getData();

  xTaskCreate(
      handleMenu,
      "menu",
      10000,
      NULL,
      1,
      NULL);

  xTaskCreate(
      handleButtons,
      "buttons",
      10000,
      NULL,
      1,
      NULL);
}

void loop()
{
  static uint32_t counter = 0;

  static unsigned long timerRecieve = 0;
  static unsigned long timerRecieveBenchmark = 0;

  if (millis() - timerRecieve > TIMER_RECIEVE)
  {
    timerRecieve = millis();

    // timerRecieveBenchmark = micros();
    winpower_3FS.updateData(true);
  }
}

// put function definitions here:
int_fast8_t cursor = 0;
bool inMenu = false;
bool changed = true;

void handleButtons(void *)
{
  char input = 0;
  while (1)
  {

    if (Serial.available() > 0)
    {
      input = Serial.read();
      Serial.flush(false);
    }
    else
    {
      input = 0;
    }
    if (inMenu)
    {
      if (input == BUTTON_MENU) // enter
      {
        winpower_3FS.sendCommand(data3FSLocal.gear, data3FSLocal.mode, data3FSLocal.abc, data3FSLocal.sensivity);
        inMenu = false;
      }
      switch (cursor)
      {
      case 0:
        switch (input)
        {
        case BUTTON_MINUS: // -
          data3FSLocal.mode--;
          break;
        case BUTTON_PLUS: // +
          data3FSLocal.mode++;
          break;
        default:
          break;
        }
        if (data3FSLocal.mode > 3)
        {
          data3FSLocal.mode = 3;
        }
        break;

      case 1:
        switch (input)
        {
        case BUTTON_MINUS: // -
          data3FSLocal.gear--;
          break;
        case BUTTON_PLUS: // +
          data3FSLocal.gear++;
          break;
        default:
          break;
        }
        if (data3FSLocal.gear > 6)
        {
          data3FSLocal.gear = 6;
        }
        if (data3FSLocal.gear < 1)
        {
          data3FSLocal.gear = 1;
        }
        break;

      case 2:
        switch (input)
        {
        case BUTTON_MINUS: // -
          data3FSLocal.abc--;
          break;
        case BUTTON_PLUS: // +
          data3FSLocal.abc++;
          break;
        default:
          break;
        }
        if (data3FSLocal.abc > 40)
        {
          data3FSLocal.abc = 40;
        }
        if (data3FSLocal.abc < 4)
        {
          data3FSLocal.abc = 4;
        }
        break;

      case 3:
        switch (input)
        {
        case BUTTON_MINUS: // -
          data3FSLocal.sensivity--;
          break;
        case BUTTON_PLUS: // +
          data3FSLocal.sensivity++;
          break;
        default:
          break;
        }
        if (data3FSLocal.sensivity > 12)
        {
          data3FSLocal.sensivity = 12;
        }
        if (data3FSLocal.sensivity < 2)
        {
          data3FSLocal.sensivity = 2;
        }
        break;

      default:
        break;
      }
    }
    else
    {
      switch (input)
      {
      case BUTTON_MENU: // enter
        inMenu = true;
        break;
      case BUTTON_MINUS: // -
        cursor--;
        break;
      case BUTTON_PLUS: // +
        cursor++;
        break;
      default:
        break;
      }
      if (cursor > 3)
      {
        cursor = 0;
      }
      if (cursor < 0)
      {
        cursor = 3;
      }
    }

    if (input == BUTTON_PLUS or input == BUTTON_MINUS or input == BUTTON_MENU)
    {
      changed = true;
    }

    vTaskDelay(20);
  }
}

void handleMenu(void *arg)
{
  Serial.write('\f');
  while (1)
  {
    if (data3FSLocal.oxygenConcentration != winpower_3FS.getOxygenConcentration() or
        data3FSLocal.flowRate != winpower_3FS.getFlowRate() or
        data3FSLocal.respiratorySign != winpower_3FS.getRespiratorySign() or
        data3FSLocal.respiratoryRate != winpower_3FS.getRespiratoryRate() or
        data3FSLocal.temperature != winpower_3FS.getTemperature())
    {
      changed = true;
    }

    if (changed)
    {
      changed = false;

      Serial.print("\033[F\033[F\033[F\033[F\033[F\033[F\033[F\033[F\033[F\033[F\033[F##############################\r\n");
      if (cursor == 0 and !inMenu)
      {
        Serial.print("-> ");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print("mode:\t");
      Serial.print(data3FSLocal.mode);
      if (cursor == 0 and inMenu)
      {
        Serial.print(" <-");
      }
      else
      {
        Serial.write("   ");
      }
      switch (data3FSLocal.mode)
      {
      case 1:
        Serial.print("\tpulse \r\n");
        break;
      case 2:
        Serial.print("\tdirect\r\n");
        break;
      case 3:
        Serial.print("\tfixed \r\n");
        break;
      default:
        Serial.print("\toff   \r\n");
        break;
      }

      if (cursor == 1 and !inMenu)
      {
        Serial.print("-> ");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print("gear:\t");
      Serial.print(data3FSLocal.gear);
      if (cursor == 1 and inMenu)
      {
        Serial.print(" <-");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print("\t1..6\r\n");

      if (cursor == 2 and !inMenu)
      {
        Serial.print("-> ");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print("ABC1:\t");
      Serial.print(data3FSLocal.abc);
      if (cursor == 2 and inMenu)
      {
        Serial.print(" <-");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print(" \t4..40\r\n");

      if (cursor == 3 and !inMenu)
      {
        Serial.print("-> ");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print("sens:\t");
      Serial.print(data3FSLocal.sensivity);
      if (cursor == 3 and inMenu)
      {
        Serial.print(" <-");
      }
      else
      {
        Serial.write("   ");
      }
      Serial.print(" \t2..12\r\n");

      Serial.print("------------------------------\r\n");

      Serial.print("   O2:\t\t");
      Serial.print((float)winpower_3FS.getOxygenConcentration() / 10, 1);

      Serial.print("\t%\r\n   flow:\t");
      Serial.print((float)winpower_3FS.getFlowRate() / 10, 1);

      Serial.print(" \tl/m\r\n   breathing:\t");
      
      Serial.print(winpower_3FS.getRespiratorySign());
      if (winpower_3FS.getRespiratorySign())
      {
        Serial.print("\t(TOT)");
      }
      else
      {
        Serial.print("\t(O_O)");
      }

      // Serial.print(data3FS.respiratorySign);

      Serial.print("\r\n   freq:\t");
      Serial.print(winpower_3FS.getRespiratoryRate());

      Serial.print(" \t*60hz\r\n   temp:\t");
      Serial.print(winpower_3FS.getTemperature());

      Serial.print("\tC\r\n##############################\b");

      data3FSLocal.oxygenConcentration = winpower_3FS.getOxygenConcentration();
      data3FSLocal.flowRate = winpower_3FS.getFlowRate();
      data3FSLocal.respiratorySign = winpower_3FS.getRespiratorySign();
      data3FSLocal.respiratoryRate = winpower_3FS.getRespiratoryRate();
      data3FSLocal.temperature = winpower_3FS.getTemperature();
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}