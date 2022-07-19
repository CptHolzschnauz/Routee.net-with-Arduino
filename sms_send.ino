#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <UrlEncode.h>
WiFiSSLClient routee;
int status = WL_IDLE_STATUS;
char WIFI_LOGIN[20] = "WIFI SSID";    // SSID  WiFi 
char WIFI_PASSWORD[20] = "password"; // Password WIFI

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Routee Gateway");
  status = WiFi.begin(WIFI_LOGIN, WIFI_PASSWORD);
  delay(10000);
  if (status != WL_CONNECTED)
  {
    Serial.println("Couldn't get a wifi connection");
  }
  // if you are connected, print out info about the connection:
  else
  {
    Serial.println("WIFI connected!");
  }
}

void loop() {
  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send sms
  readSerial(remoteNum);
  Serial.println(remoteNum);
  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);
  
  String post_text = "{ \"body\": \"" + String(txtMsg) + "\",\"to\" :\"" + String(remoteNum) + "\",\"from\": \"BSG\"}";
  Serial.print("post_text: ");
  Serial.println(post_text);

  routee_sms_send(post_text);

}

void routee_sms_send(String RUT_SMS)
{

 const char *SMS_SERVER = "connect.routee.net";
  const unsigned short SMS_SERVER_PORT = 443;

  HttpClient https(routee, SMS_SERVER, SMS_SERVER_PORT);
  https.beginRequest();
  https.post("/sms");
  https.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  https.sendHeader(HTTP_HEADER_CONTENT_LENGTH, RUT_SMS.length());
  https.sendHeader("authorization", "Bearer cbxxxxxx-xxxx-xxxx-xxxx-9fcc3c6f27d0");
  https.endRequest();
  https.print(RUT_SMS);
  int statusCode_ssl = https.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode_ssl);
  String response_ssl = https.responseBody();
  Serial.println(response_ssl);

}

int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}
