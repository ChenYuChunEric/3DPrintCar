/*
 * 360度Servo
 * 以MG90S.write(90)為基準，伺服馬達靜止狀態
 * 0與180為轉速最快
 * 0~90逆時針旋轉，90~180順時針旋轉
 * 其餘與一般控制相同
 */

#include <ESP8266WiFi.h>
#include <Servo.h>

//建立Servo
Servo Right;
Servo Left;
Servo Hand;

//360Servo停止值
int MG_stop = 90;

//90Servo數值
int Handpos = 5;

// 設定無線基地台SSID跟密碼
const char* ssid     = "";     //改成您的SSID
const char* password = "";     //改成您的密碼

// 設定 web server port number 80
WiFiServer server(80);

// 儲存 HTTP request 的變數
String header;

void setup() {
  Serial.begin(115200);
  
  // Servo賦予Pin
  Right.attach(D1);
  Left.attach(D2);
  Hand.attach(D3);

  // Servo初始化設定
  Right.write(MG_stop);
  Left.write(MG_stop);
  Hand.write(Handpos);

  // 使用SSID 跟 password 連線基地台
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 使用COM Port 列出取得的IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // 等待 clients 連線

  if (client) {                             // 假使新的用戶端連線
    Serial.println("New Client.");          // 從序列 Port印出訊息內容
    String currentLine = "";                // 清空這行的內容
    while (client.connected()) {            // 當 client繼續連線持續執行迴圈
      if (client.available()) {             // 假使從 client 有讀到字元
        char c = client.read();             // 讀取這個字元
        Serial.write(c);                    // 印出這個字元在串列視窗
        header += c;
        if (c == '\n') {                    // 假使是換行符號

          // 假使目前的一行是空白且有一個新行，就結束 client HTTP 的要求
          if (currentLine.length() == 0) {
            // HTTP 表頭開始時，會有回應碼 response code (如： HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // 將 GPIOs 開或關
            if (header.indexOf("GET /1/head") >= 0){
              Serial.println("head");
              Right.write(0);
              Left.write(180);
            }
            else if (header.indexOf("GET /1/back") >= 0){
              Serial.println("back");
              Right.write(180);
              Left.write(0);
            }
            else if (header.indexOf("GET /2/right") >= 0){
              Serial.println("right");
              Right.write(120);
              Left.write(120);
            }
            else if (header.indexOf("GET /2/left") >= 0){
              Serial.println("left");
              Right.write(60);
              Left.write(60);
            }
            else if(header.indexOf("GET /3/stop") >= 0){
              Serial.println("stop");
              Right.write(MG_stop);
              Left.write(MG_stop);
            }
            if(header.indexOf("GET /4/up") >= 0){
              Serial.println("up");
              if(Handpos <= 175){
                Handpos+=5;
              }
              if(Handpos >= 175){
                Handpos = 175;
              }
              Hand.write(Handpos);
              Serial.println(Handpos);
            }
            else if(header.indexOf("GET /4/down") >= 0){
              Serial.println("down");
              if(Handpos >= 5){
                Handpos-=5;
              }
              if(Handpos <= 5){
                Handpos = 5;
              }
              Hand.write(Handpos);
              Serial.println(Handpos);
            }
            // 顯示 HTML 網頁
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // 設定按鈕的CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto;text-align: center;}");
            // 按鈕1
            client.println(".button1 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕2
            client.println(".button2 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕3
            client.println(".button3 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕4
            client.println(".button4 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕5
            client.println(".button5 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕6
            client.println(".button6 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // 按鈕7
            client.println(".button7 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");

            // 網頁表頭
            client.println("<body><h1>ESP8266 Servo Web Server</html>");
            // 按鈕1 UP
            client.println("<p><a href=\"/1/head\"><button class=\"button1\">HEAD</button></a></p>");
            // 按鈕2.3.4 LEFT STOP RIGHT
            client.println("<p><a href=\"/2/left\"><button class=\"button2\">LEFT</button></a> <a href=\"/3/stop\"><button class=\"button3\">STOP</button></a> <a href=\"/2/right\"><button class=\"button4\">RIGHT</button/a></p>");
            // 按鈕5 DOWN
            client.println("<p><a href=\"/1/back\"><button class=\"button5\">BACK</button/><a></p>");
            // 按鈕6.7 UP DOWN
            client.println("<p><a href=\"/4/up\"><button class=\"button6\">UP</button></a> <a href=\"/4/down\"><button class=\"button7\">DOWN</button></a></p>");
            // 網頁結尾
            client.println("</body></html>");

            // 使用空白行結束 HTTP回應
            client.println();

            break;
          } else {   // 假使有新的一行, 清除目前這一行
            currentLine = "";
          }
        } else if (c != '\r') {  // 讀取到的不是換行符號
          currentLine += c;      // 增加一個字元在本行最後
        }
      }
    }
    // 清除表頭變數
    header = "";
    // 關閉連線 connection
    //client.stop();
    //Serial.println("Client disconnected.");
    Serial.println("");
  }
}
