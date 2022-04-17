
#include <Arduino.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#define MINIMUM_FIRMWARE_VERSION_LED "0.6.6"
#define SLEEP_TIME 20

Adafruit_BluefruitLE_SPI ble(8, 7, 4);

void setup(void)
{
    Keyboard.begin();

    if (ble.begin(false))
    {
        ble.echo(false);

        while (!ble.isConnected())
        {
            delay(500);
        }

        if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION_LED))
        {
            ble.sendCommandCheckOK("AT+HWModeLED=MODE");
        }

        ble.setMode(BLUEFRUIT_MODE_DATA);
    }
}

void loop(void)
{
    String data = "";

    while (ble.available())
    {
        char ch = (char)ble.read();

        if (ch == '\n')
        {
            Keyboard.press(KEY_ESC);
            delay(SLEEP_TIME);
            Keyboard.release(KEY_ESC);
            delay(SLEEP_TIME);
            Keyboard.press(KEY_RETURN);
            delay(SLEEP_TIME);
            Keyboard.release(KEY_RETURN);
            delay(SLEEP_TIME);
            Keyboard.print(data);
            data = "";
            Keyboard.press(KEY_RETURN);
            delay(SLEEP_TIME);
            Keyboard.releaseAll();
        }
        else
        {
            data += ch;
        }
    }
}
