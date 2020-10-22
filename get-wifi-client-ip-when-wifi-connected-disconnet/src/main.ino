#include <ESP8266WiFi.h>
// include plain C library
extern "C"
{
#include "user_interface.h"
}

/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID "jh-get-ip-esp"
#define APPSK "00000000"
#endif

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

/** Should I connect to WLAN asap? */
boolean connect;

boolean waitingDHCP = false;
char last_mac[18];

// Manage incoming device connection on ESP access point
void onNewStation(WiFiEventSoftAPModeStationConnected sta_info)
{
    Serial.println("New Station :");
    sprintf(last_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(sta_info.mac));
    Serial.printf("MAC address : %s\n", last_mac);
    Serial.printf("Id : %d\n", sta_info.aid);
    waitingDHCP = true;
}

String macToString(const unsigned char *mac)
{
    char buf[20];
    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected &evt)
{
    Serial.print("disconnect station: ");
    Serial.println(macToString(evt.mac)); //调用macToString函数将mac转换成字符串
}

void setup()
{

    static WiFiEventHandler e1;
    static WiFiEventHandler e2;

    Serial.begin(9600);
    Serial.println();
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(softAP_ssid, softAP_password);

    // Event subscription
    e1 = WiFi.onSoftAPModeStationConnected(onNewStation);
    e2 = WiFi.onSoftAPModeStationDisconnected(onStationDisconnected);
}

void loop()
{
    Serial.println("Loop");

    if (waitingDHCP)
    {
        String cb;
        if (deviceIP(last_mac, cb))
        {
            Serial.println("Ip address :");
            Serial.println(cb); //do something
        }
        else
        {
            Serial.println("Problem during ip address request :");
            Serial.println(cb); //do something else
        }
    }

    delay(2000);
}

boolean deviceIP(char *mac_device, String &cb)
{

    struct station_info *station_list = wifi_softap_get_station_info();

    while (station_list != NULL)
    {
        char station_mac[18] = {0};
        sprintf(station_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(station_list->bssid));
        String station_ip = IPAddress((&station_list->ip)->addr).toString();

        if (strcmp(mac_device, station_mac) == 0)
        {
            waitingDHCP = false;
            cb = station_ip;
            return true;
        }

        station_list = STAILQ_NEXT(station_list, next);
    }

    wifi_softap_free_station_info();
    cb = "DHCP not ready or bad MAC address";
    return false;
}