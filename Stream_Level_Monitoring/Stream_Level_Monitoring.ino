
  //STREAM LEVEL MONITORING AND ALERT SYSTEM
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  #include <SoftwareSerial.h> 
  SoftwareSerial gsm(2, 3); // RX, TX pins for the GSM module
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  const int FLOAT_SWITCH_PIN = 2; // The digital input pin for the float switch sensor
  const int LED_RED= 13; // The digital output pin for the LED
  const int LED_ORANGE= 12; // The digital output pin for the LED
  const int LED_YELLOW= 11; // The digital output pin for the LED
  const int LED_GREEN= 10; // The digital output pin for the LED
  const int NUM_RECIPIENTS = 2 ;// The number of recipients    
  const String RECIPIENT_NUMBERS[NUM_RECIPIENTS] = {"+639286262566","+639294806171"}; //if youw want add more numbers, be sure to add the NUM_RECIPIENTS(number of recipients above. 
  int buzz=9;  //declare variable to store the alarm
  int trig=4;   //declare variable to store the trigger pin
  int echo=5;  //declarea variable to store the echo pin
  int depth=5; //depth initiliaze t0 20 meters
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  void setup() 
  {
    //pinModes for inputs and outputs
    pinMode(FLOAT_SWITCH_PIN, INPUT);// float switch input
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(buzz, OUTPUT);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    Serial.begin(9600);// initializing the serial port 
    gsm.begin(9600);//initializing the gsm
    delay(1000);
  }
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  void loop() {
   
   int floatSwitch = digitalRead(FLOAT_SWITCH_PIN);
    digitalWrite(trig, LOW);
    delayMicroseconds(10);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long t = pulseIn(echo, HIGH); 
    float cm = t / 29 / 2;
    float m = cm / 100.0; // convert cm to meters
    float waterLevel= depth-m;
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
   if (waterLevel > 3  && floatSwitch == HIGH) {   //if water level is 3 meters above
     digitalWrite(LED_GREEN, LOW); // Turn off the LED\]B
    Serial.print("red-");
    Serial.println(waterLevel);
    digitalWrite(LED_RED, HIGH); // Turn on the Red LED
     tone(buzz,1000);
     delay(10000);//10 seconds of alarm
     digitalWrite(LED_RED, LOW); // Turn on the Red LED
//    gsmCall(); //call the user to notify 
    sendSMS("Red Level Alert: Please evacuate immediately");   //send sms to the user 
     delay(1000);
   
    } 
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
    else if(waterLevel <=3 && waterLevel >=1.1)
    {
       digitalWrite(LED_GREEN, LOW); // Turn off the LED\]B
       Serial.print("orange-");
       Serial.println(waterLevel);
       digitalWrite(LED_ORANGE, HIGH);
       tone(buzz,1000);
       delay(5000);
       digitalWrite(LED_ORANGE, LOW);
   
       sendSMS("Orange Level Alert: Warning! Please evacuate immediately");   //send sms to the user 
       delay(1000);
     }
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
    else if(waterLevel <=1 && waterLevel >=0.5)
      {
     digitalWrite(LED_GREEN, LOW); // Turn off the LED\]B
     Serial.print("yellow-");
     Serial.println(waterLevel);
     digitalWrite(LED_YELLOW, HIGH);
     tone(buzz,1000);
     delay(3000);
     digitalWrite(LED_YELLOW, LOW);
    
     sendSMS("Yellow Level Alert: Warning! Please prepare  to evacuate to a higher place!");   //send sms to the user 
       delay(1000);
     }
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  else {
     Serial.print("green-");
     Serial.println(waterLevel);
     digitalWrite(LED_GREEN, HIGH); // Turn off the LED
     noTone(buzz);
  }
     delay(1000);
  }
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  void sendSMS(String message) {   //MESSAGE AS THE PARAMETER
    gsm.println("AT+CMGF=1"); // Set the SMS mode to text mode
    delay(1000);
    for (int i = 0; i < NUM_RECIPIENTS; i++) {      
      String cmd = "AT+CMGS=\"" + RECIPIENT_NUMBERS[i] + "\"";
      gsm.println(cmd); // Send the recipient phone number
      delay(1000);
      gsm.println(message); // The SMS message to send
      delay(1000);
      gsm.write((byte) 26); // Send the Ctrl+Z character to indicate the end of the
    }
  }
  /*---------------------------------------------------------------------------------------------------------------------------------------------*/
  /*void gsmCall()  //function for calling
  {
    gsm.println("ATD+ +639294806171;");
  }*/
