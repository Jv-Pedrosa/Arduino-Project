#include <SoftwareSerial.h>

SoftwareSerial SIM900A(10, 11);
#define POWER_PIN 7
#define SIGNAL_PIN A5
const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int led4 = 5;
const int led5 = 6;
const int buzzer = 9;  // Buzzer to Arduino pin 9

const int RELAY_PIN = 12; // Relay ITO
int value = 0;             // Variable to store the sensor value

bool messageSent = false;  // Variable to track whether the SMS has been sent
unsigned long lastMessageTime = 0;  // Variable to store the time when the last message was sent

void setup()
{
  // GSM
  SIM900A.begin(9600);
  Serial.begin(9600);
  Serial.println("SIM900A Ready");

  // LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  // Buzzer
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Relay
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); // Turn the sensor OFF
}

void loop()
{
  digitalWrite(led5, HIGH);
  if (SIM900A.available() > 0)
    Serial.write(SIM900A.read());

  digitalWrite(POWER_PIN, HIGH);  // Turn the sensor ON
  delay(10);                      // Wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // Read the analog value from the sensor
  digitalWrite(POWER_PIN, LOW);   // Turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);

  if (value >= 130)
  {
    if (!messageSent)
    {
      sample1();
      Serial.println("ALARMING LEVEL 3!");
      SendMessage("EMERGENCY ALERT3! Evacuate to higher ground immediately.");
      messageSent = true;  // Set the flag to true to indicate that the message has been sent
      lastMessageTime = millis();  // Record the time when the message was sent
    }
    alarmTone1();
    leds3();
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (value <= 120 && value >= 61)
  {
    if (!messageSent)
    {
      sample2();
      Serial.println("ALARMING LEVEL 2!");
      SendMessage("EMERGENCY ALERT2! Pack your belongings and be careful.");
      messageSent = true;
      lastMessageTime = millis();
    }
    alarmTone2();
    leds2();
    digitalWrite(RELAY_PIN, HIGH);
  }
  else if (value <= 50 && value >= 10)
  {
    if (!messageSent)
    {
      sample3();
      Serial.println("ALARMING LEVEL 1! ");
      SendMessage("EMERGENCY ALERT1! Possible flood is coming; be alert and ready.");
      messageSent = true;
      lastMessageTime = millis();
    }
    alarmTone3();
    leds1();
    digitalWrite(RELAY_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
  }

  // Check if 10 seconds have passed since the last message was sent
  if (millis() - lastMessageTime >= 10000)
  {
    messageSent = false;  // Reset the flag to allow sending a new message
  }
}

void alarmTone1()
{
  tone(buzzer, 400); // Send 1KHz sound signal...
  delay(200);        // ...for 200 milliseconds
  noTone(buzzer);    // Stop sound...
  delay(200);        // ...for 200 milliseconds
}

void alarmTone2()
{
  tone(buzzer, 500); // Send 1KHz sound signal...
  delay(400);        // ...for 400 milliseconds
  noTone(buzzer);    // Stop sound...
  delay(400);        // ...for 400 milliseconds
}

void alarmTone3()
{
  tone(buzzer, 700); // Send 1KHz sound signal...
  delay(600);        // ...for 600 milliseconds
  noTone(buzzer);    // Stop sound...
  delay(600);        // ...for 600 milliseconds
}

void leds1()
{
  digitalWrite(led4, HIGH);
  delay(500);
  digitalWrite(led4, LOW);
}

void leds2()
{
  digitalWrite(led3, HIGH);
  delay(300);
  digitalWrite(led3, LOW);
}

void leds3()
{
  digitalWrite(led1, HIGH);
  delay(100);
  digitalWrite(led1, LOW);
}

void SendMessage(const char *message)
{
  Serial.println("Sending Message");
  SIM900A.println("AT+CMGF=1");          // Sets the GSM Module in Text Mode
  delay(100);
  SIM900A.println("AT+CMGS=\"+639217871467\"\r"); // Mobile phone number to send message
  delay(100);
  SIM900A.print(message); // Message content
  delay(100);
  SIM900A.println((char)26); // ASCII code of CTRL+Z
  delay(100);
  Serial.println("Finish");
}


void sample1()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+639453624978\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("EMERGENCY ALERT3! Evacuate to higher ground immediately.");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}

void sample2()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+639453624978\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("EMERGENCY ALERT2! Pack your belongings and be careful.");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}


void sample3()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+639453624978\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("EMERGENCY ALERT1! Possible flood is coming; be alert and ready.");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}


