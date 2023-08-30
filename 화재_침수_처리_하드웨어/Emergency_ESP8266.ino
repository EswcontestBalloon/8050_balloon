/* 
  보드매니저 : esp8266 by ESP8266 Community Version 2.7.4
  라이브러리 : MySQL Connector Arduino by Dr.Charles Bell 1.2.0
*/

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <pins_arduino.h>


// Wifi 접속 정보
const char* ssid = "LGWiFi_48B6";
const char* password = "vndtjsxla";

//sql 연결셋팅부
IPAddress server_addr(113,131,245,30); // MySQL 데이터베이스 IP 주소
char user[] = "balloon"; // MySQL 데이터베이스 ID
char passwd[] = "balloon"; // MySQL 데이터베이스 PASSWORD

WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

char SELECT_SQL[] = "SELECT datas FROM balloon.monitor WHERE id = 1"; // SQL 쿼리문

void setup() {
  Serial.begin(115200); // 전송속도

  pinMode(14, OUTPUT);  // 경광등
  pinMode(12, OUTPUT);  // 스프링쿨러
  pinMode(13, OUTPUT);  // 배수펌프

  Wifi_connect(); // 와이파이 연결

  Serial.print("Connecting to SQL...  ");

  if (conn.connect(server_addr, 3306, user, passwd)) {
    Serial.println("OK.");
  }
  else {
    Serial.println("FAILED.");
  }
}


void loop() {
  row_values *row = NULL;
  char res[9];

  if (WiFi.status() != WL_CONNECTED) { // 와이파이 연결이 이상할시에 재접속 하는 코드
    Serial.println("try reconnect");
    Wifi_connect();
  }

  delay(100);

  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(SELECT_SQL); // 실제 excute되는 시점
  cursor->get_columns();

  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      strcpy(res, row->values[0]);
    }
    Serial.println(res);
  } while (row != NULL);

  if (!strcmp(res, "Fire")) {
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    Serial.println("Fire Alam");
    delay(10000);
  }
  else if (!strcmp(res, "Flooding")) {
    digitalWrite(14, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    Serial.println("Flooding Alam");
    delay(10000);
  }
  else {
    digitalWrite(14, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }

  delay(3000);
}


void Wifi_connect() {
  Serial.println("---------------------------------------");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // 와이파이 이름과 비밀번호를 통해 WIFI연결을 시작 // WL_CONNECTED라는 값을 돌려준다

  while (WiFi.status() != WL_CONNECTED) {
    // 네트워크의 연결 상태, 8개의 리턴값
    // STATUS와 WL_CONNECTED 값이 같은지를 통해 제대로 연결이 되있는지를 확인할 수 있다
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wifi connected!");
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
}