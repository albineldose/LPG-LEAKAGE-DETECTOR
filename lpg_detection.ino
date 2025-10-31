#include <SoftwareSerial.h>

SoftwareSerial sim800l(10, 11); // RX, TX for GSM module

int Input = A0;
int Motor = 2;
int TOT_PWR = 3;
int Buzzer = 4;

int val;
int MAX = 300; // Threshold for gas leakage
String phoneNumber = "+1234567890"; // Replace with your actual phone number

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600); // Start communication with GSM module

  pinMode(Input, INPUT);
  pinMode(Motor, OUTPUT);
  pinMode(TOT_PWR, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  Serial.println("GSM Module Initialized...");
}

void loop() {
  val = analogRead(A0);
  Serial.print("Sensor Value: ");
  Serial.println(val);
  delay(500);

  if (val >= MAX) {
    Serial.println("GAS LEAK DETECTED!");
    digitalWrite(Motor, HIGH);
    digitalWrite(TOT_PWR, LOW);
    alertUser(); // Send SMS and Call
    buzzAlarm();
  } else {
    digitalWrite(Motor, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(TOT_PWR, HIGH);
    Serial.println("NORMAL CONDITION");
  }
}

void alertUser() {
  sendSMS();
  makeCall();
}

void sendSMS() {
  Serial.println("Sending SMS...");
  sim800l.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  sim800l.print("AT+CMGS=\""); 
  sim800l.print(phoneNumber); // Send to this number
  sim800l.println("\"");
  delay(1000);
  sim800l.println("WARNING: GAS LEAK DETECTED! Take immediate action.");
  delay(1000);
  sim800l.write(26); // End of SMS (CTRL+Z)
  delay(5000);
  Serial.println("SMS Sent.");
}

void makeCall() {
  Serial.println("Making Call...");
  sim800l.print("ATD");
  sim800l.print(phoneNumber);
  sim800l.println(";");
  delay(15000); // Call duration (15 seconds)
  sim800l.println("ATH"); // Hang up
  Serial.println("Call Ended.");
}

void buzzAlarm() {
  for (int i = 0; i < 5; i++) { // Beep 5 times
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
  }
}
