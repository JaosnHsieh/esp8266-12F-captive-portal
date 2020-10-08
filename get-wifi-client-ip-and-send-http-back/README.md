# get-wifi-client-ip-and-send-http-back

this is a test repo for testing `captive portal` on iOS & Android system.

[base code 1](https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced)

[base code 2](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/examples/BasicHttpClient/BasicHttpClient.ino#L43-L73)

## purpose

send a http request back when there is a wifi client ( phone or laptop ) connected to esp8266 ap.

mainly modifications part in `get-wifi-client-ip-and-send-http-back/src/handleHttp.ino`

```cpp

boolean captivePortal()
{
  String remoteIp = server.client().remoteIP().toString();
  Serial.println(remoteIp);

  WiFiClient client;

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "http://" + remoteIp + ":3001/api/mqtt"))
  { // HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else
  {
    Serial.printf("[HTTP} Unable to connect\n");
  }


```

##

i'm building, uploading these code via [platform.io](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start) on vscode to my esp8266-12F board via a micro usb cable.
