
#include <Arduino.h>
#include <Keyboard.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#define MINIMUM_FIRMWARE_VERSION_LED "0.6.6"
#define SLEEP_TIME 40 // In milliseconds

// Note: 40ms for SLEEP_TIME is a 'safe' value. On Xbox this can be reduced to 20ms for
// faster callout delivery but PS4 Pro will not tolerate a 20ms delay. Not sure about PS5
// don't have one to test with. Maybe make this an app setting in the future

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

        if (ch == '\n') {
            Keyboard.press(KEY_RETURN);
            delay(SLEEP_TIME);
            Keyboard.release(KEY_RETURN);

            // empty string between newlines is the
            // signal to change chat channels. Delay
            // a bit during this process so user has
            // time to see the impact of the change.
            if (data.length() == 0) {
                data = "";
                Keyboard.press(KEY_TAB);
                delay(SLEEP_TIME);
                Keyboard.release(KEY_TAB);
                delay(1000);
                Keyboard.press(KEY_ESC);
                delay(SLEEP_TIME);
                Keyboard.release(KEY_ESC);
            } else {
                // If not the change-channel signal, send
                // the received data as a callout on the
                // current channel.                
                for (auto x: data) {
                    Keyboard.write(x);
                }

                delay(SLEEP_TIME);
                Keyboard.press(KEY_RETURN);
                delay(SLEEP_TIME);
                Keyboard.release(KEY_RETURN);

                data = "";
            }
        } else {
            data += ch;
        }
    }
}
