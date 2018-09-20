//*********************************************************************************
//* NTPtimeESP.h is located here: https://github.com/SensorsIot/NTPtimeESP
//* HomeseerSend.h is located here https://github.com/futuretechguy/HomeseerSend (only need if homeseer softwareis present in network)
//*********************************************************************************


#include "devicecontrol.h"
#include <HomeseerSend.h>
#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <NTPtimeESP.h>
#include <DHT.h>


#define DBG_OUTPUT_PORT Serial

#define DHTPIN 12     // sensor's Data pin
#define DHTTYPE DHT11 // sensor type

int SolPin = 4;
int LastFlowRate = 0;
int LastTotalLitres = 0;
int LastTemperature = 0;
int LastHumidity = 0;
int ledStatus = LOW;

byte sensorInterrupt = 5; // 0 = digital pin D1
float calibrationFactor = 4.5;
volatile byte pulseCount;

float CurrTemperature = 0;
float CurrHumidity = 0;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long elapseTime;
unsigned long totalLitres;

unsigned long dhtStartTime = 0;    //millis();
unsigned long ntpStartTime = 0;    //millis();
unsigned long dhtInterval = 10000; //1 minutes
unsigned long ntpInterval = 10000; //5 minutes

const byte interruptPin = 0;
const char *host = "MistingSystem";
String oServerMessage;
boolean device_one_state = false;

DHT dht(DHTPIN, DHTTYPE);
HSSend HSSend(true); //initilize HomeseerSend

NTPtime NTPch("us.pool.ntp.org");
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int wsClientNumber[5] = {-1, -1, -1, -1, -1};
int lastClientIndex = 0;
const int max_ws_client = 5;

String startH = "";
String startM = "";
String stopH = "";
String stopM = "";

//char* oPinState;

strDateTime dateTime;

volatile int state = LOW;

//if start hourt matches the actual hour return true
bool StartTimeSync()
{
    if (dateTime.hour == startH.toInt() && dateTime.minute == startM.toInt())
    {
        if (!startH.toInt() == 0 && !startM.toInt() == 0)
        { // don't turn on at 00:00
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//if start minute matches the actual minute return true
bool StopTimeSync()
{
    if (dateTime.hour == stopH.toInt() && dateTime.minute == stopM.toInt())
    {
        if (!stopH.toInt() == 0 && !stopM.toInt() == 0)
        { // don't turn off at 00:00
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void ProcessGetRequest(String message)
{
    int val;
    if (message.indexOf("/gpio/0") != -1)
    {
        val = 0;
        //digitalWrite(SolPin, val);
        setStatus(true);
    }
    else if (message.indexOf("/gpio/1") != -1)
    {
        setStatus(false);
        val = 1;
        //digitalWrite(SolPin, val);
    }

    String s = "The valve is: ";
    s += (val) ? "On" : "Off";
    //s += (val) ? "/On : /Off";
    server.send(200, "text/plain", s);
    //wsSendState;
}

void pulseCounter()
{
    // Increment the pulse counter
    pulseCount++;
}

void SendHumidity(float oCurrentHumidity)
{
    String url;
    if (oCurrentHumidity != LastHumidity)
    {

        url = HSSend.GetJsonDevCtrlURL("1217", String(oCurrentHumidity));
        HSSend.SendData(url);
        url = String();
        LastHumidity = oCurrentHumidity;
    }
}

void SendTemperature(float oCurrentTemperature)
{
    String url;
    if (oCurrentTemperature != LastTemperature)
    {

        url = HSSend.GetJsonDevCtrlURL("1218", String(oCurrentTemperature));
        HSSend.SendData(url);
        url = String();
        LastTemperature = oCurrentTemperature;
    }
}

void setup()
{
    // prepare GPIO
    pinMode(SolPin, OUTPUT);
    digitalWrite(SolPin, 0);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    elapseTime = 0;

    DBG_OUTPUT_PORT.begin(115200);
    DBG_OUTPUT_PORT.print("n");
    DBG_OUTPUT_PORT.setDebugOutput(false);

    EEPROM.begin(512);

    //=======[ WiFiManager Wifi init ]======================================
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 1, 51), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0)); //use local environment IP address
    wifiManager.autoConnect("AutoConnectAP");

    //========================[ OTA init]=====================================
    // UnComment for OTA update
    //ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("ESP_MistingSystem");

    //ArduinoOTA.setPassword((const char *)"Mist8266");
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });
    ArduinoOTA.begin(); /* setup the OTA server */

    DBG_OUTPUT_PORT.println("");
    DBG_OUTPUT_PORT.print("Connected! IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());

    //======================[ DNS init ]====================================

    MDNS.begin(host);
    DBG_OUTPUT_PORT.print("Open http://");
    DBG_OUTPUT_PORT.print(host);
    DBG_OUTPUT_PORT.println(".local/device.html to see the controls");

    //========================[ Web Server init]=============================
    server.on("/device.html", []() { //load RGB control page
        Serial.println("device.html");
        server.send(200, "text/html", PAGE_DeviceControlPage);
    });

    server.onNotFound([]() { //called when the url is not defined here

        if (isValidRequest(server.uri()))
        {
            ProcessGetRequest(server.uri()); //process gpio GET commands
        }
        else
        {
            server.send(404, "text/plain", "Command Not Found"); //use it to load content from SPIFFS
        }
    });

    server.on("/all", HTTP_GET, []() { //get heap status, analog input value and all GPIO statuses in one json call

        String json = GetESPJson();
        server.send(200, "text/json", json);
        json = String();
    });

    server.begin();

    //========================[Web Socket init]==============================

    webSocket.begin(); // start webSocket server
    webSocket.onEvent(webSocketEvent);

    MDNS.addService("http", "tcp", 80); // Add service to MDNS
    MDNS.addService("ws", "tcp", 81);
    DBG_OUTPUT_PORT.println("HTTP server started");

    //=======================================================================

    GetScheduleFromEEPROM(); //load time data from EEPROM

    HSSend.SetHost("192.168.1.46", 80);
}

void loop()
{
    ArduinoOTA.handle();

    //=============================[Water flow sensor]===============================================-
    if ((millis() - elapseTime) > 1000) // Only process counters once per second
    {
        detachInterrupt(digitalPinToInterrupt(sensorInterrupt));

        flowRate = ((1000.0 / (millis() - elapseTime)) * pulseCount) / calibrationFactor;
        elapseTime = millis();

        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;
        totalLitres = totalMilliLitres / 1000;

        DBG_OUTPUT_PORT.print("Flow rate: ");
        DBG_OUTPUT_PORT.print(int(flowRate)); // Print the integer part of the variable
        DBG_OUTPUT_PORT.println("L/min");
        SendFlowRate(int(flowRate));

        DBG_OUTPUT_PORT.print("Quantity: ");
        DBG_OUTPUT_PORT.print(totalLitres);
        DBG_OUTPUT_PORT.println("L");
        DBG_OUTPUT_PORT.println("");
        SendTotalVolume(totalLitres);

        pulseCount = 0;
        attachInterrupt(digitalPinToInterrupt(sensorInterrupt), pulseCounter, FALLING);
    }

    server.handleClient();
    webSocket.loop();
    if ((millis() - elapseTime) > 1000)
    {
        wsSendState();
    }

    unsigned long CurrentTime = millis();

    //==========================[Humidity and Temperature sensor]======================================
    if (CurrentTime - dhtStartTime > dhtInterval)
    {
        GetDHT22Values();
        SendTemperature(CurrTemperature);
        SendHumidity(CurrHumidity);
        dhtStartTime = CurrentTime;
    }

    //======================================[Time server]===============================================
    if (CurrentTime - ntpStartTime > ntpInterval)
    {

        dateTime = NTPch.getNTPtime(-5.0, 1);
        if (dateTime.valid)
        {
            NTPch.printDateTime(dateTime);
            //DBG_OUTPUT_PORT.println(ShowTimeComparison());
            if (StartTimeSync()) //if time match run schedule
            {
                setStatus(false);
                //wsSendState();
            }
            else if (StopTimeSync())
            {
                setStatus(true);
                //wsSendState();
            }
            ntpStartTime = CurrentTime;
        }
    }
}

void setStatus(boolean st)
{
    if (st)
    {
        // GetValveState()
        //device_one_state = true;
        DBG_OUTPUT_PORT.print("Change valve state:");
        DBG_OUTPUT_PORT.println("OFF");
        digitalWrite(SolPin, LOW);
        oServerMessage = "Valve set to: OFF";
    }
    else
    {
        //device_one_state = false;
        DBG_OUTPUT_PORT.print("Change valve state:");
        DBG_OUTPUT_PORT.println("On");
        digitalWrite(SolPin, HIGH);
        oServerMessage = "Valve set to: ON";
    }
}

String GetValveState()
{
    DBG_OUTPUT_PORT.print("PinState: ");
    DBG_OUTPUT_PORT.println(digitalRead(SolPin));

    if (digitalRead(SolPin) == 1)
    {
        return "On";
    }
    else
    {
        return "Off";
    }
}

void GetDHT22Values()
{
    float h = dht.readHumidity();
    float f = dht.readTemperature(true); // Fahrenheit
    if (isnan(h) || isnan(f))
    {
        DBG_OUTPUT_PORT.println("Failed to read from DHT sensor!");
    }
    else
    {

        DBG_OUTPUT_PORT.print("Humidity: ");
        DBG_OUTPUT_PORT.print(h);
        DBG_OUTPUT_PORT.print(" %\t");
        DBG_OUTPUT_PORT.println(" ");
        CurrHumidity = h;

        DBG_OUTPUT_PORT.print("Temperature: ");
        DBG_OUTPUT_PORT.print(f);
        DBG_OUTPUT_PORT.print(" *F\t");
        DBG_OUTPUT_PORT.println(" ");
        CurrTemperature = f;
    }
}

String GetMistStateJson(String oTemp, String oHumid, String oValve, String oFlow, String oVolume)
{

    String json;
    json += "{";
    json += "\"Temperature\"";
    json += ":";
    json += "\"" + oTemp + "\"";
    json += ",";
    json += "\"Humidity\"";
    json += ":";
    json += "\"" + oHumid + "\"";
    json += ",";
    json += "\"Valve\"";
    json += ":";
    json += "\"" + oValve + "\"";
    json += ",";
    json += "\"FlowRate\"";
    json += ":";
    json += "\"" + oFlow + "\"";
    json += ",";
    json += "\"FlowVolume\"";
    json += ":";
    json += "\"" + oVolume + "\"";
    json += ",";
    json += "\"" + String("Start") + "\"";
    json += ":";
    json += "\"" + startH + ":" + startM + "\"";
    json += ",";
    json += "\"" + String("Stop") + "\"";
    json += ":";
    json += "\"" + stopH + ":" + stopM + "\"";
    json += ",";
    json += "\"Message\"";
    json += ":";
    json += "\"" + oServerMessage + "\"";
    json += "}";
    return json;
}

void wsSendState()
{ //update state "on connect"

    String json = GetMistStateJson(String(CurrTemperature), String(CurrHumidity), GetValveState(), String(flowRate), String(totalLitres));
    int numcl = 0;
    for (numcl = 0; numcl < max_ws_client; numcl++)
    {
        if (wsClientNumber[numcl] != -1)
        {
            webSocket.sendTXT(wsClientNumber[numcl], json); //send to socket client
        }
    }
    json = String();
}

boolean isValidNumber(String str)
{
    boolean isNum = false;
    for (byte i = 0; i < str.length(); i++)
    {
        isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
        if (!isNum)
            return false;
    }
    return isNum;
}

void GetScheduleFromEEPROM()
{

    char val1 = EEPROM.read(15);
    char val2 = EEPROM.read(16);
    char val3 = EEPROM.read(17);
    char val4 = EEPROM.read(18);
    char val5 = EEPROM.read(19);
    char val6 = EEPROM.read(20);
    char val7 = EEPROM.read(21);
    char val8 = EEPROM.read(22);

    startH += val1;
    startH += val2;
    startM += val3;
    startM += val4;
    stopH += val5;
    stopH += val6;
    stopM += val7;
    stopM += val8;

    //Test if the stored value is numeric
    if (!isValidNumber(startH))
    {
        startH = "00";
    }
    if (!isValidNumber(startM))
    {
        startM = "00";
    }
    if (!isValidNumber(stopH))
    {
        stopH = "00";
    }
    if (!isValidNumber(stopM))
    {
        stopM = "00";
    }
}

String GetESPJson()
{
    String json;
    json += "{";
    json += "\"heap\"";
    json += ":";
    json += "\"" + String(ESP.getFreeHeap()) + "\"";
    json += ",";
    json += "\"analog\"";
    json += ":";
    json += "\"" + String(analogRead(A0)) + "\"";
    json += ",";
    json += "\"gpio\"";
    json += ":";
    json += "\"" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16))) + "\"";
    json += "}";
    return json;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        DBG_OUTPUT_PORT.printf("[%u] Disconnected!n", num);
        break;

    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        DBG_OUTPUT_PORT.printf("[%u] Connected from %d.%d.%d.%d url: %sn", num, ip[0], ip[1], ip[2], ip[3], payload);
        int index = (num % max_ws_client);
        if (index <= 0)
            index = 0;
        wsClientNumber[index] = num;
        DBG_OUTPUT_PORT.printf("Save client index %d :%un", index, num);
        //wsSendState();   // send message to client
        oServerMessage = "Status: Connected";
    }
    break;

    case WStype_TEXT:
        DBG_OUTPUT_PORT.printf("[%u] get Text: %sn", num, payload);
        String strPayload = (char *)payload;
        String oSize = "00";

        if (payload[0] == '1')
        {
            setStatus(true);
            //wsSendState();
        }
        else if (payload[0] == '0')
        {
            setStatus(false);
            //wsSendState();
        }
        else if (strPayload.indexOf(":") > -1 && strPayload.substring(0, strPayload.indexOf("@")) == "start")
        {                                                                                        //if valud start time is received
            startH = strPayload.substring(strPayload.indexOf("@") + 1, strPayload.indexOf(":")); // Extract hours
            startM = strPayload.substring(strPayload.indexOf(":") + 1);                          // Extract minutes
            //wsSendTimeJson("Start", startH, startM);  //bounce back to client                                         //bounce back start time to client
            oServerMessage = String("Start set to: " + Convert24To12(startH + ":" + startM));
            //save start time to memory
            EEPROM.write(15, startH[0]);
            EEPROM.write(16, startH[1]);
            EEPROM.write(17, startM[0]);
            EEPROM.write(18, startM[1]);
            EEPROM.commit();
            //wsSendState();
        }

        else if (strPayload.indexOf(":") > -1 && strPayload.substring(0, strPayload.indexOf("@")) == "stop")
        {                                                                                       //if valud stop time is received
            stopH = strPayload.substring(strPayload.indexOf("@") + 1, strPayload.indexOf(":")); // Extract hours
            stopM = strPayload.substring(strPayload.indexOf(":") + 1);                          // Extract minutes
            //wsSendTimeJson("Stop", stopH , stopM);                                                                //bounce back stop time to client
            oServerMessage = String("Stop set to: " + Convert24To12(stopH + ":" + stopM));
            //save start time to memory
            EEPROM.write(19, stopH[0]);
            EEPROM.write(20, stopH[1]);
            EEPROM.write(21, stopM[0]);
            EEPROM.write(22, stopM[1]);
            EEPROM.commit();
            //wsSendState();
        }

        else if (strPayload.indexOf(":") == -1 && strPayload.substring(0, 5) == "start")
        {                  //if start time cancel received
            startH = "00"; // set hours variable to 00
            startM = "00"; // set minutes variable to 00
            //wsSendTimeJson("Stop", startH , startM);                                                              //bounce back start cancal client
            oServerMessage = String("Start time cleared");
            EEPROM.write(15, startH[0]); //save 00 to start memory
            EEPROM.write(16, startH[1]);
            EEPROM.write(17, startM[0]);
            EEPROM.write(18, startM[1]);
            EEPROM.commit();
            //wsSendState();
        }

        else if (strPayload.indexOf(":") == -1 && strPayload.substring(0, 4) == "stop")
        {                 //if stop time cancel received
            stopH = "00"; // set stop hours variable to 00
            stopM = "00"; // set stop minutes variable to 00
            //wsSendTimeJson("Stop", stopH , stopM);                                                                  //bounce back stop cancal to client
            oServerMessage = String("Stop time cleared");
            EEPROM.write(19, stopH[0]); //save 00 to stop memory
            EEPROM.write(20, stopH[1]);
            EEPROM.write(21, stopM[0]);
            EEPROM.write(22, stopM[1]);
            EEPROM.commit();
            //wsSendState();
        }
        else
        {
            oServerMessage = String("Message received");
        }
        break;
    }
}

void SendFlowRate(int oFlowRate)
{
    String url;
    if (oFlowRate != LastFlowRate)
    {
        String StrFlow;
        StrFlow += oFlowRate;
        url = HSSend.GetJsonDevCtrlURL("1212", StrFlow);
        HSSend.SendData(url);
        url = String();
        StrFlow = String();
        LastFlowRate = oFlowRate;
    }
}

void SendTotalVolume(int oTotalLitres)
{
    String url;
    String StrTotalLitres;
    StrTotalLitres += oTotalLitres;
    if (oTotalLitres != LastTotalLitres)
    {
        url = HSSend.GetJsonDevCtrlURL("1213", StrTotalLitres);
        HSSend.SendData(url);
        url = String();
        StrTotalLitres = String();
        LastTotalLitres = oTotalLitres;
    }
}

bool isValidRequest(String message)
{
    String gpiocmd = message.substring(1, 5);
    Serial.println(gpiocmd);
    if (gpiocmd == "gpio")
    {
        return true;
    }
    else
    {
        return false;
    }
}

String ShowTimeComparison()
{

    String TimeComp = "Actual Time: = ";
    TimeComp += String(dateTime.hour);
    TimeComp += ":";
    TimeComp += String(dateTime.minute);
    TimeComp += "\n";
    TimeComp += "Saved Start Time = ";
    TimeComp += String(startH);
    TimeComp += ":";
    TimeComp += String(startM);
    TimeComp += "\n";
    TimeComp += "Saved Stop Time = ";
    TimeComp += String(stopH);
    TimeComp += ":";
    TimeComp += String(stopM);
    return TimeComp;
}

String Convert24To12(String oTime)
{
    String oM = oTime.substring(oTime.indexOf(":") + 1);
    String oH = oTime.substring(0, oTime.indexOf(":"));
    String oAmPm = "AM";

    if (oH.toInt() > 12 && oH.toInt() < 24)
    {
        oAmPm = "PM";
        //String oH = oTime.substring(0, oTime.indexOf(":"));
        switch (oH.toInt())
        {
        case 13:
            oH = "1";
            break;
        case 14:
            oH = "2";
            break;

        case 15:
            oH = "3";
            break;

        case 16:
            oH = "4";
            break;

        case 17:
            oH = "5";
            break;

        case 18:
            oH = "6";
            break;

        case 19:
            oH = "7";
            break;

        case 20:
            oH = "8";
            break;

        case 21:
            oH = "9";
            break;

        case 22:
            oH = "10";
            break;

        case 23:
            oH = "11";
            break;
        }
    }
    return oH + ":" + oM + " " + oAmPm;
}

bool IsStartTimeSet()
{

    if (String(startH + ":" + startM) = "00:00")
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool IsStopTimeSet()
{

    if (String(stopH + ":" + stopM) = "00:00")
    {
        return false;
    }
    else
    {
        return true;
    }
}
