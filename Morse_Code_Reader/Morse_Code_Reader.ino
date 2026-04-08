const byte button = 8;
unsigned long start_time;
byte previous_value = LOW;

void setup() {
  String starter_message = "Morse Code Reader Output:";
  Serial.println(starter_message);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  byte value = digitalRead(button);
  int duration = 0;
  
  if (value != previous_value) {
    if (value == HIGH) {
    start_time = millis();
    } else {
    duration = millis() - start_time;
    Serial.print("Duration: ");
    Serial.print(duration);
    Serial.print("ms");
    Serial.println();
    }
  }
  previous_value = value;

  //Program now calculates button pressed duration correctly. We can use this to convert ticks into dots and dashes (using reasonable ranges).
  //From there we can create a dictionary, with all of the morse characters.
  //In the loop() store every n seconds all ticks, or store ticks into the same object until you reach an x ms long pause (pick one). Or do both and compare lol.
  // 

}
