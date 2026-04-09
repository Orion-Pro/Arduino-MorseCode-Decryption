#include "UnorderedMap.h"
#include "Hashtable.h"


//Button pin #
const byte button = 8;
//Buzzer pin #
const byte buzzer = 5;

//Needed for duration calculation
unsigned long start_time;
unsigned long pause_start;

//These variables need to be global for my function designs.
int press_duration = 0;
int pause_duration = 0;

//Might be useful for cleaner code;
const int dot;
const int dash; //where dash == 3*dot


//Initializes the condition for the state change loop
byte previous_value = LOW;

//--------------------------------------------------------------------------------------------------------------------------------

// const UnorderedMap<Sequence, char> = {

// };

//Dictionary needs to translate a sequence of inputs into chars
//These inputs consists of ints (durations)
//One character will be represented by a single sequence
//So we can store in an array of a reasonable max_size all of the durations.
//Do we need an intermediate conversion step to just store dots and dashes?

//--------------------------------------------------------------------------------------------------------------------------------

//I also need to store my sequences into an array, to decode the message in the final step.
//This project will work sequentially.
  //Receive encoded input
  //Store encoded input
  //Decode input
  //Output

//i.e. code will not be decrypted simultaneously with the input reading.

//--------------------------------------------------------------------------------------------------------------------------------

class Sequence {
    public: //For now, I'll make it private later (maybe)
      int duration_sequence[20] = {0}; //20 is a place holder, I need to look at the morse alphabet to determine max number of inputs for a single char.
      //Array is 0 initialized
      
    public:
      Sequence() {}
};

Sequence Sequencer() {
    Sequence currentSequence;
    int startDuration = 2000; //Placeholder, makes the function start;  //I need to calculate the pause durations as well.
    
    int i = 0; //counter

    do {
      byte value = digitalRead(button);

      if (value != previous_value) {
        if (value == HIGH) {
          pause_duration = millis() - pause_start;
          start_time = millis();
          digitalWrite(buzzer, HIGH);
        } else {
            pause_start = millis();
            press_duration = millis() - start_time;

            currentSequence.duration_sequence[i] = press_duration;
            i++;
            digitalWrite(buzzer, LOW);

            Serial.print("Duration: ");
            Serial.print(press_duration);
            Serial.print("ms");
            Serial.println();
        }
        delay(5); //minimizes false readings.
      }
      previous_value = value;

    } while (pause_duration < 6000);
}

void setup() {
  String starter_message = "Morse Code Reader Output:";
  Serial.println(starter_message);
  
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.begin(9600);
}

//rework the loop to do the following.
//If there is a change in button_value, call the sequencer.
//If not print out a waiting message, and call the decoder
//Do not take in any inputs while the decoder is running.

void loop() {
  
  byte value = digitalRead(button);

  if (value != previous_value) {
    if (value == HIGH) {
    pause_duration = millis() - pause_start;
    start_time = millis();
    digitalWrite(buzzer, HIGH);
    } else {
    pause_start = millis();
    press_duration = millis() - start_time;
    digitalWrite(buzzer, LOW);
    Serial.print("Duration: ");
    Serial.print(press_duration);
    Serial.print("ms");
    Serial.println();
    }
    delay(5); //minimizes false readings.
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