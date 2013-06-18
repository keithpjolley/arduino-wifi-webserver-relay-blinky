/*
 * Socket App
 *
 * A simple socket application example using the WiShield 1.0
 */


// be sure to edit the begining of uip.c to match apps-conf.h  -- sigh


#define APP_WISERVER

#include <WiServer.h> 
//#include <WiShield.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {
  192,168,1,132};	// IP address of WiShield
unsigned char gateway_ip[] = {
  192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {
  255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {
  "YOURSSIDGOESHERE"};		// max 32 bytes

unsigned char security_type = 1;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {
  ""};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = {
  0xYO, 0xUR, 0xWA, 0xPP, 0xAS, 0xSW, 0xOR, 0xDG, 0xOE, 0xSH, 0xER, 0xE_, 0x__, // Key 0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
//---------------------------------------------------------------------------

int redState = 0;
int whiteState = 0;
int greenState = 0;

int redPin = 5;
int whitePin = 6;
int greenPin = 7; 

// This is our page serving function that generates web pages
boolean sendMyPage(char* URL) {
  if (1) {
    Serial.print("BEFORE: URL: ");
    Serial.println(URL);
    URL++;
    Serial.print("AFTER: URL: ");
    Serial.println(URL);
    Serial.print("As number: ");
    Serial.println(atoi(URL));
  }

  if (strcmp(URL, "/red") == 0)   redState   = !redState;
  if (strcmp(URL, "/white") == 0) whiteState = !whiteState;
  if (strcmp(URL, "/green") == 0) greenState = !greenState;
  digitalWrite(redPin, redState);
  digitalWrite(whitePin, whiteState);
  digitalWrite(greenPin, greenState);
  // Check if the requested URL matches "/"
  // if (strcmp(URL, "/") == 0) {
  // Use WiServer’s print and println functions to write out the page content
  WiServer.print("<html>");
  WiServer.print("Light Control<br><br>");
  printLightStatus("red", redState);
  printLightStatus("white", whiteState);
  printLightStatus("green", greenState);
  WiServer.print("The page you requested was: ");
  WiServer.print(URL);
  WiServer.print("<br>The arduino has been running for: ");
  WiServer.print(millis());
  WiServer.print(" milliseconds<br>");
  WiServer.print("</html>");
  // URL was recognized
  return true;
  //}
  // URL not found
  return false;
}

void printLightStatus( String lightName, int lightState) {
  WiServer.print(lightName);
  WiServer.print(" Light is ");
  if(lightState == 0) {
    WiServer.print(" <b>off</b> <a href=/");
    WiServer.print(lightName);
    WiServer.print(">Turn On</a><br>");
  } 
  else {
    WiServer.print(" <b>on</b> <a href=/");
    WiServer.print(lightName);
    WiServer.print(">Turn off</a><br>");
  }
} 

void setup() {
  Serial.begin(57600);
  // Initialize WiServer and have it use the sendMyPage function to serve pages
  Serial.println("initializing server");

  WiServer.init(sendMyPage);
  // Enable Serial output and ask WiServer to generate log messages (optional)
  Serial.println("server started");

  WiServer.enableVerboseMode(true);
  pinMode(redPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  digitalWrite(redPin, redState);
  digitalWrite(whitePin, whiteState);
  digitalWrite(greenPin, greenState);

}

void loop(){

  // Run WiServer
  WiServer.server_task();
  delay(10);
}


