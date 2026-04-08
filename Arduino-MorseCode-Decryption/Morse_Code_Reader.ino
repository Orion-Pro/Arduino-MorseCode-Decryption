//Button input number
const byte button = 8;

//Needed for duration calculation
unsigned long start_time;

//Initializes the condition for the state change loop
byte previous_value = LOW;



public class Sequence<Dot, Dash> {

}

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

}

//I will make global parameters for dit and dah lengths (as I am not a pro morse code communicator and I will absolutely need to slow down the pace)
//I will also ensure reasonably forgiving ranges for the time it takes to register a sequence.

//between each character there is one dit (1 time unit)
//1 dit is a dot, and 1 dah is a dash (3 time units)
//between words --> dah dit dah (7 time units)

//As a simplifying assumption I will begin by creating a sequence class for a single sentence/sequence, I will later on try to generalize the program for continuous use.