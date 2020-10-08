# esp8266-12F-captive-portal

this is a test repo for testing `captive portal` on iOS & Android system.

[base code](https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced)

i'm building, uploading these code via [platform.io](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start) on vscode to my esp8266-12F board via a micro usb cable.

## Android

My Android 11 phone connected to wifi AP SSID `ESP-ap`, click it will redirect to captive portal in a special Android built-in browser. It cannot access `192.168.4.1` on other apps like Chrome.

## iOS and MacOS

It won't show pop up window to captive portal. Instead, we can access it via `http://192.168.4.1`.

## Modifications

Only added below part to the base code in `src/handleHttp.ino`:

```cpp
 if (server.hostHeader() != "connectivitycheck.gstatic.com")
    {
      return false;
    }
```
