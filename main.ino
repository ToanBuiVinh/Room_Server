/*
 * 1. @nhiet_do_va_do_am
 * 2. @dong_bo_thoi_gian
 * 3. @ir_dieu_khien
 * 4. @gui_email
 * 5. @LED_state // Trang thai led
 * 6. @Cua_ra_vao
 * 7. @Fan_control
 * 8. @test
 */
 //IP Address for OTA:            172.20.30.152
 //USER ID: Hyperlogy - Password: Hyper@123
 /*
  * Lib: 
  * 1. IRremoteESP8266
  * 2. ESP32_MailClient
  */

/* PIN OUT ~ ESP32 ~ IR ~ 2 x DHT11 ~
 * GPIO32:
 * IR_send: GPIO_12 ~ DATA of IR send
 * LED_state_air: GPIO 26 ~ Res 300 Ohm ~ Anot of LED
 * LED_state_fan: GPIO 25 ~ Res 300 Ohm ~ Anot of LED
 * LED_state_door: GPIO 22 ~ Res 300 Ohm ~ Anot of LED
 * DHT11(in): GPIO_4][ 
 * a's[a
 * [A]ÂAQ QCC ~ Data DHT11(in)
 * DHT11(out): GPIO_0 ~ RES 10k ~ VCC ~ Data DHT11(out)
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* hostname = "otaesp";
//@ir_dieu_khien
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <FirebaseESP32.h>
//@ir_dieu_khien

//@nhiet_do_va_do_am
#include "DHT.h"
//@nhiet_do_va_do_am

//@gui_email
#include "ESP32_MailClient.h"
//@gui_email

//@dong_bo_thoi_gian
//#include <WiFi.h>
#include "time.h"
//@dong_bo_thoi_gian

//@dong_bo_thoi_gian
const char* ssid = "HYPERLOGY-Guest-02";
const char* password = "hyperlogy2019";
//const char* ssid = "TP-Link_6014";
//const char* password = "t0lachua";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;
//@dong_bo_thoi_gian

//@Gui_Firebase
#define FIREBASE_HOST "https://esp32-device-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "vfRpihYU4RzT8Ge6wLAh3MPEFFmr6KgP5uNOm7Wf"
FirebaseData firebaseData;
String path = "/doan3";
//@Gui_Firebase

//@gui_email
//#define emailSenderAccount    "tungpch1906001@fpt.edu.vn"    
//#define emailSenderPassword   "Tung@1992"
#define emailSenderAccount    "cle75736@gmail.com"    
#define emailSenderPassword   "t0lachua"
//#define emailRecipient        "dinhchinh42@gmail.com"
//#define emailRecipient1       "hungnguyenrw@gmail.com"
#define emailRecipient        "dinhchinh42@gmail.com"
#define emailRecipient2       "tung.pham@hyperlogy.com"
#define emailRecipient3       "thang.le@hyperlogy.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "[Alert]Phòng Server"
//@gui_email

//@Web_Server_OTA
//WebServer server(80);
//
////Web Server Code
//      /* Style */
//      String style =
//      "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
//      "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
//      "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
//      "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
//      "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
//      ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";
//      
//      /* Login page */
//      String loginIndex = 
//      "<form name=loginForm>"
//      "<h1>ESP32 OTA Update Login </h1>"
//      "<input name=userid placeholder='User ID'> "
//      "<input name=pwd placeholder=Password type=Password> "
//      "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
//      "<script>"
//      "function check(form) {"
//      "if(form.userid.value=='Hyperlogy' && form.pwd.value=='Hyper@123')" //Autheticate user before OTA Update
//      "{window.open('/serverIndex')}"
//      "else"
//      "{alert('Error Password or Username')}"
//      "}"
//      "</script>" + style;
//       
//      /* Server Index Page */
//      String serverIndex = 
//      "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
//      "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
//      "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
//      "<label id='file-input' for='file'>   Choose file...</label>"
//      "<input type='submit' class=btn value='Update'>"
//      "<br><br>"
//      "<div id='prg'></div>"
//      "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
//      "<script>"
//      "function sub(obj){"
//      "var fileName = obj.value.split('\\\\');"
//      "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
//      "};"
//      "$('form').submit(function(e){"
//      "e.preventDefault();"
//      "var form = $('#upload_form')[0];"
//      "var data = new FormData(form);"
//      "$.ajax({"
//      "url: '/update',"
//      "type: 'POST',"
//      "data: data,"
//      "contentType: false,"
//      "processData:false,"
//      "xhr: function() {"
//      "var xhr = new window.XMLHttpRequest();"
//      "xhr.upload.addEventListener('progress', function(evt) {"
//      "if (evt.lengthComputable) {"
//      "var per = evt.loaded / evt.total;"
//      "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
//      "$('#bar').css('width',Math.round(per*100) + '%');"
//      "}"
//      "}, false);"
//      "return xhr;"
//      "},"
//      "success:function(d, s) {"
//      "console.log('success!') "
//      "},"
//      "error: function (a, b, c) {"
//      "}"
//      "});"
//      "});"
//      "</script>" + style;
//@Web_Server_OTA
      
//@nhiet_do_va_do_am
#define DHTPIN_in 0     // Digital pin connected to the DHT sensor ~ GPIO 4ESP32 for ESP32
#define DHTPIN_out 4     // Digital pin connected to the DHT22 sensor ~ GPIO 0 for 
#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE_2 DHT11 // DHT 11
//@nhiet_do_va_do_am

//@LED_state
#define LED_state_air 26 // Digital pin connected to GPIO_26
#define LED_state_fan 25 // Digital Pin connected to GPIO_25
#define LED_state_door 14  // Digital Pin connected to GPIO_23
#define Door_sensor 17 // Digital pin input connected to GPIO_22
#define Fan_control 27 // Digital pin output connected to GPIO_19
#define Cua_dieu_khien 13// Digital Pin output connected to GPIO_18
//@LED_state

//@gui_email
SMTPData smtpData; // The Email Sending data object contains config and data to send
//void sendCallback(SendStatus info); // Callback function to get the Email sending status
uint8_t email_noti = 0;
//@gui_email

//@ir_dieu_khien
const uint16_t kIrLed = 12; // GPIO 12 ESP32
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

/*@IR_data*/
//Power: On, Mode: 0 (Cool), Fan: 2 (Med), Temp: 25C, Zone Follow: Off, Sensor Temp: Off
uint16_t rawData[199] = {4330, 4400,  512, 1632,  512, 528,  542, 1630,  514, 1656,  486, 556,  514, 558,  514, 1656,  488, 584,  488, 558,  512, 1630,  512, 558,  512, 584,  488, 1656,  486, 1656,  488, 584,  486, 1654,  488, 556,  514, 1630,  514, 560,  512, 1630,  514, 1630,  512, 1630,  512, 1630,  514, 1630,  514, 1628,  540, 532,  514, 1628,  512, 558,  516, 582,  488, 556,  540, 556,  512, 558,  514, 1604,  512, 1630,  514, 556,  514, 556,  540, 530,  514, 584,  488, 582,  514, 530,  540, 532,  540, 530,  540, 1604,  538, 1630,  488, 1628,  542, 1602,  542, 1604,  540, 1630,  514, 5184,  4384, 4374,  538, 1606,  538, 558,  488, 1654,  514, 1630,  514, 530,  540, 532,  538, 1606,  538, 530,  540, 530,  540, 1630,  514, 556,  514, 532,  540, 1628,  514, 1628,  514, 506,  566, 1602,  540, 530,  542, 1602,  540, 532,  540, 1602,  540, 1628,  514, 1604,  538, 1630,  514, 1602,  542, 1604,  538, 530,  540, 1604,  538, 532,  540, 556,  514, 556,  514, 532,  540, 530,  542, 1604,  540, 1604,  540, 530,  542, 528,  542, 530,  540, 532,  540, 530,  540, 530,  540, 530,  540, 530,  542, 1602,  540, 1602,  540, 1602,  540, 1602,  540, 1602,  540, 1576,  568};
//Power: Off
uint16_t off_air[199] = {4378, 4402,  462, 1658,  510, 558,  512, 1658,  486, 1630,  512, 558,  512, 560,  510, 1632,  512, 584,  486, 534,  536, 1632,  510, 584,  486, 584,  462, 1682,  486, 1632,  486, 554,  542, 1632,  512, 532,  538, 1632,  510, 1632,  486, 1682,  462, 1682,  460, 586,  510, 1632,  486, 1658,  510, 1634,  510, 558,  512, 586,  462, 584,  486, 584,  486, 1656,  488, 584,  488, 584,  486, 1658,  510, 1632,  486, 1662,  482, 584,  488, 610,  462, 610,  484, 586,  462, 584,  486, 584,  510, 558,  488, 610,  462, 1656,  488, 1682,  462, 1656,  486, 1658,  486, 1656,  510, 5186,  4384, 4400,  512, 1658,  460, 584,  486, 1656,  512, 1658,  460, 584,  488, 584,  510, 1656,  462, 584,  486, 584,  488, 1682,  462, 582,  512, 532,  538, 1658,  460, 1658,  510, 560,  512, 1630,  488, 560,  536, 1630,  514, 1632,  512, 1632,  512, 1656,  462, 610,  462, 1682,  486, 1630,  514, 1630,  488, 584,  512, 556,  514, 530,  540, 584,  486, 1632,  514, 558,  512, 558,  512, 1656,  488, 1628,  512, 1656,  486, 560,  512, 582,  488, 556,  514, 558,  514, 532,  540, 556,  514, 560,  536, 532,  540, 1578,  538, 1630,  514, 1630,  512, 1628,  540, 1604,  514};
//@ir_dieu_khien

bool dieu_hoa = false; // Set bien toan cho dieu hoa, check bien nay moi khi dong bat trang thai. Chi dong tat 1 lan duy nhat.
int cnt = 0;

//@nhiet_do_va_do_am
DHT dht_in(DHTPIN_in, DHTTYPE);
DHT dht_out(DHTPIN_out, DHTTYPE_2);
//@nhiet_do_va_do_am

void setup() {
  //@Dang nhap Wifi, Firebase va nap firmware qua OTA
      Serial.println("Booting");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.waitForConnectResult() != WL_CONNECTED) 
        {
          Serial.println("Connection Failed! Rebooting...");
          delay(2000);
          ESP.restart();
         }
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      Firebase.reconnectWiFi(true);

    /*use mdns for host name resolution*/
//    if (!MDNS.begin(hostname)) 
//      {
//      Serial.println("Error setting up MDNS responder!");
//      while (1) 
//          {
//            delay(1000);
//          }
//       }
//      MDNS.begin(hostname); 
//       
//      Serial.println("mDNS responder started");
//      Serial.print("Access the webserver at http://");
//      Serial.print(hostname);
//      Serial.print(".local/");
// 
//  
//        /*return index page which is stored in serverIndex */
//        server.on("/", HTTP_GET, []() {
//          server.sendHeader("Connection", "close");
//          server.send(200, "text/html", loginIndex);
//        });
//        
//        server.on("/serverIndex", HTTP_GET, []() {
//          server.sendHeader("Connection", "close");
//          server.send(200, "text/html", serverIndex);
//        });
//        
//        /*handling uploading firmware file */
//        server.on("/update", HTTP_POST, []() {
//          server.sendHeader("Connection", "close");
//          server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
//          ESP.restart();
//        }, []() {
//          HTTPUpload& upload = server.upload();
//          
//          if (upload.status == UPLOAD_FILE_START) 
//            {
//              Serial.printf("Update: %s\n", upload.filename.c_str());
//              if (!Update.begin(UPDATE_SIZE_UNKNOWN)) 
//                { //start with max available size
//                 Update.printError(Serial);
//                }
//            } 
//          else if (upload.status == UPLOAD_FILE_WRITE) 
//            {
//         /* flashing firmware to ESP*/
//              if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
//                {
//                 Update.printError(Serial);
//                 }
//             } 
//          else if (upload.status == UPLOAD_FILE_END) {
//               if (Update.end(true)) 
//                  { //true to set the size to the current progress
//                  Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
//                   } 
//          else 
//            {
//              Update.printError(Serial);
//            }
//            
//          }
//        });
//        
//      server.begin();
  //@Dang nhap Wifi va nap firmware qua OTA

  irsend.begin(); //@ir_dieu_khien
  
  //@nhiet_do_va_do_am
  dht_in.begin(); // DHT11 (1) enable
  dht_out.begin(); // DHT11 (2) 
  //@nhiet_do_va_do_am
  
  //@Led_state
  pinMode(LED_state_air, OUTPUT);
  pinMode(LED_state_fan, OUTPUT);
  pinMode(LED_state_door, OUTPUT);
  pinMode(Fan_control, OUTPUT);
  pinMode(Cua_dieu_khien, OUTPUT);
  
  pinMode(Door_sensor, INPUT);
  //@Led_state
  
  Serial.begin(115200, SERIAL_8N1);

   // @dong_bo_thoi_gian to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  dong_bo_thoi_gian();

  //disconnect WiFi as it's no longer needed
//  WiFi.disconnect(true);
//  WiFi.mode(WIFI_OFF);
  //@dong_bo_thoi_gian

  
  //@gui_email
  Serial.println("Preparing to send email");
  Serial.println();
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("Phòng Server - Hyperlogy", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Đã bật điều hòa, temp > 35</h1><p>- Gửi từ Phòng Server Hyperlogy</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from ESP32 board", false);
  // Add recipients, you can add more than one recipient
//  smtpData.addRecipient(emailRecipient);
//  smtpData.addRecipient(emailRecipient1);
  smtpData.addRecipient(emailRecipient);
//  smtpData.addRecipient(emailRecipient2);
//  smtpData.addRecipient(emailRecipient3);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
  smtpData.setSendCallback(sendCallback);
  //@gui_email
}

void loop() {
  //server.handleClient(); //Nap FW qua OTA
  
  delay(1);
  float dht_in_t = dht_in.readTemperature(); // DHT in
  float dht_out_t = dht_out.readTemperature();//DHT (out)
  int th_up = 37;
  int th_dwn = 34;
  int state = 0;

  Serial.print(F("Temp in: "));
  Serial.println(dht_in_t);

  Serial.print(F("Temp out: "));
  Serial.println(dht_out_t);
  
  delay(3000);

  if(state == 0)
  {
    th_up = 37;
    th_dwn = 34;
  }
  else
  {
    th_up = 34;
    th_dwn = 31;
  }
    
  //if (dht_in_t > th_up || dht_out_t > th_dwn) // Nếu nhiệt độ trong > 35 độ || (hoặc) Nhiệt độ ngoài > 30 độ
  if (dht_in_t > 37 || dht_out_t > 34)
  {
    MailClient.sendMail(smtpData);
  }
  if (dht_in_t > 45 || dht_out_t > 45)
  {
    state = 1;
    cnt++;
    
    //state_air = 1; // Bật điều hòa
    if(dieu_hoa == false)
    {
      //bat_tat_dieu_hoa(1);
      dieu_hoa = true;
      Serial.println("Dieu hoa bat: ");
      Serial.println(dieu_hoa);  
    }

    if(cnt > 40)
    {
      cnt = 0;
      //state_fan = 0; // Tắt quạt
      //Quat_dieu_khien(0);
    }
    //state_door = 1; // Đóng cửa
    //Cua_trang_thai(1);
  }
  else
  {
    state = 0;
    //state_air = 0; // Tắt điều hòa
    Serial.println("Check: ");
    Serial.println(dieu_hoa);
    if(dieu_hoa == true)
    {
      for(int i = 0; i< 2; i++) // Tat 2 lan de chac chan
      {
        //bat_tat_dieu_hoa(0);  
      }
      dieu_hoa = false;
      Serial.println("Dieu hoa tat: ");
      Serial.println(dieu_hoa);  
    }
    //state_fan = 1; // Bật quạt
    //Quat_dieu_khien(1);
    //state_door = 0; // Mở cửa
    //Cua_trang_thai(0);
  }
  kiem_tra_thang_thai_cua(); // Kiem tra thang thai cua vao ra, bao hien thi led
  /*
  Serial.println("Dieu hoa: ");
  Serial.println(dieu_hoa);
  */
  Serial.println(dieu_hoa);
}zz

//@LED_state
void LED_state(bool state)
{
  if(state == 1){digitalWrite(LED_state_air, LOW);}
  else{digitalWrite(LED_state_air, HIGH);}
}

void Led_state_fan(bool state)
{
  if(state == 1){digitalWrite(LED_state_fan, LOW);}
  else{digitalWrite(LED_state_fan, HIGH);}
}
  
//@ir_dieu_khien
void bat_tat_dieu_hoa(bool state)
{
  //if(state == 1 && email_noti == 0)
  if(state == 1 )
  {
    irsend.sendRaw(rawData, 199, 38);
    digitalWrite(LED_state_air, HIGH);
    MailClient.sendMail(smtpData);
    Serial.println("Air on");
  }
  if(state == 0)
  {
    irsend.sendRaw(off_air, 199, 38);
    Serial.println("Air off");
    digitalWrite(LED_state_air, LOW);
    //email_noti = 1;
  }  
}

//@Cua_ra_vao
void trang_thai_cua_ra_vao()
{
    if(digitalRead(Door_sensor) == LOW)
  {
    Serial.println(F("Cua CLOSED"));
    //digitalWrite(LED_state_door, HIGH);
  }
  else
  {
    Serial.println(F("Cua OPEN"));
    //digitalWrite(LED_state_door, LOW);
  }
}

//@Cua_dieu_khien
void Cua_trang_thai(bool state)
{
  if(state == 1) // Dong Cua
  {
    digitalWrite(Cua_dieu_khien, HIGH);
    digitalWrite(LED_state_door, LOW);
    Serial.println(F("Cua CLOSED"));
  }
  else // Mo Cua
  {
    digitalWrite(Cua_dieu_khien, LOW);
    digitalWrite(LED_state_door, HIGH);
    Serial.println(F("Cua OPEN"));    
  }
}


//@Quat_dieu_khien
void Quat_dieu_khien(bool state)
{
  if(state == 1)
  {
    digitalWrite(Fan_control, HIGH);
    digitalWrite(LED_state_fan, LOW);
    Serial.println(F("Quat ON"));
  }
  else
  {
    
    digitalWrite(Fan_control, LOW);
    digitalWrite(LED_state_fan, HIGH);
    Serial.println(F("Quat OFF"));
    
  }
}

//@nhiet_do_va_do_am
void nhiet_do_va_do_am()
{
  //1 - DHT in
  //0 - DHT out
  //DHT (in)
  float dht_in_h = dht_in.readHumidity();
  float dht_in_t = dht_in.readTemperature();
  //DHT (out)
  float dht_out_h = dht_out.readHumidity();
  float dht_out_t = dht_out.readTemperature();
  //Check neu xay ra loi
  if (isnan(dht_in_h) || isnan(dht_in_t)) 
  {
    Serial.println(F("Failed to read from DHT(IN) sensor!"));
    return;
  }
   if (isnan(dht_out_h) || isnan(dht_out_t)) 
  {
    Serial.println(F("Failed to read from DHT(OUT) sensor!"));
    return;
  }
  /*
  if(side == 1)
  {
    //dht_in_h;
    Serial.print(F("DHT(in) Temp: "));
    Serial.print(dht_in_t);
    Serial.println(F("°C"));
  }
  if(side == 0)
  {
    //dht_out_h;
    Serial.print(F("DHT(out) Temp: "));
    Serial.print(dht_out_t);
    Serial.println(F("°C"));
  }
  */
  Firebase.setDouble(firebaseData, path + "nhietdo",dht_in_t);
  Firebase.setDouble(firebaseData, path + "doam",dht_in_h);
}

void kiem_tra_thang_thai_cua()
{
  if(digitalRead(Door_sensor) == LOW)
  {
    digitalWrite(LED_state_door, LOW);
    Serial.println("Cua da dong hoan toan");
  }
  else
  {
    digitalWrite(LED_state_door, HIGH);
    Serial.println("Cua dang mo");
  }
}

//@dong_bo_thoi_gian
void dong_bo_thoi_gian()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}
//@dong_bo_thoi_gian

//@gui_email
// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
//@gui_email
