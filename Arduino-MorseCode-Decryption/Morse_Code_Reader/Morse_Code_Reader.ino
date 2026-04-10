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
const int dot = 100; //We can give +/- 50 to begin with
const int dash = 500; //where dash == 3*dot (again +/-50)
const int space = 1100; //where space == 7*dot (+/-50)

//Initializes the condition for the state change loop
byte previous_value = LOW;

//--------------------------------------------------------------------------------------------------------------------------------

int aSeq[] = {dot, dash};
int bSeq[] = {dash, dot, dot, dot};
int cSeq[] = {dash, dot, dash, dot};
int dSeq[] = {dash, dot, dot};
int eSeq[] = {dot};
int fSeq[] = {dot, dot, dash, dot};
int gSeq[] = {dash, dash, dot};
int hSeq[] = {dot, dot, dot, dot};
int iSeq[] = {dot, dot};
int jSeq[] = {dot, dash, dash, dash};
int kSeq[] = {dash, dot, dash};
int lSeq[] = {dot, dash, dot, dot};
int mSeq[] = {dash, dash};
int nSeq[] = {dash, dot};
int oSeq[] = {dash, dash, dash};
int pSeq[] = {dot, dash, dash, dot};
int qSeq[] = {dash, dash, dot, dash};
int rSeq[] = {dot, dash, dot};
int sSeq[] = {dot, dot, dot};
int tSeq[] = {dash};
int uSeq[] = {dot, dot, dash};
int vSeq[] = {dot, dot, dot, dash};
int wSeq[] = {dot, dash, dash};
int xSeq[] = {dash, dot, dot, dash};
int ySeq[] = {dash, dot, dash, dash};
int zSeq[] = {dash, dash, dot, dot};

int oneSeq[]  = {dot, dash, dash, dash, dash};
int twoSeq[]  = {dot, dot, dash, dash, dash};
int threeSeq[]= {dot, dot, dot, dash, dash};
int fourSeq[] = {dot, dot, dot, dot, dash};
int fiveSeq[] = {dot, dot, dot, dot, dot};
int sixSeq[]  = {dash, dot, dot, dot, dot};
int sevenSeq[]= {dash, dash, dot, dot, dot};
int eightSeq[]= {dash, dash, dash, dot, dot};
int nineSeq[] = {dash, dash, dash, dash, dot};
int zeroSeq[] = {dash, dash, dash, dash, dash};

int questionSeq[] = {dot, dot, dash, dash, dot, dot};     
int exclamSeq[]   = {dash, dot, dash, dot, dash, dash};   
int periodSeq[]   = {dot, dash, dot, dash, dot, dash};    
int commaSeq[]    = {dash, dash, dot, dot, dash, dash};   
int semicolonSeq[]= {dash, dot, dash, dot, dash, dot};    
int colonSeq[]    = {dash, dash, dash, dot, dot, dot};    
int plusSeq[]     = {dot, dash, dot, dash, dot};          
int minusSeq[]    = {dash, dot, dot, dot, dot, dash};     
int slashSeq[]    = {dash, dot, dot, dash, dot};          
int equalsSeq[]   = {dash, dot, dot, dot, dash};

//--------------------------------------------------------------------------------------------------------------------------------

class Sequence {
    public: //For now, I'll make it private later (maybe)
      int duration_sequence[20] = {0}; //20 is a place holder, I need to look at the morse alphabet to determine max number of inputs for a single char.
      //Array is 0 initialized
      int size = 0;
      
    public:
      Sequence() {}; //Default Constructor;

      Sequence(int arr[], int n) { //parameter based constructor
        size = n;
        for (int i = 0; i < n; i++) {
          duration_sequence[i] = arr[i];
        }
      };

      bool operator==(const Sequence& other) const {
        if (this->size != other.size)
          return false;
        
        for (int i = 0; i < other.size; i++) {
          if (abs(other.duration_sequence[i] - this->duration_sequence[i]) <= 50)
            continue;

          else
            return false;
        }
        return true;
      }

};

//--------------------------------------------------------------------------------------------------------------------------------

Sequence seqAlphabet[] = {
    Sequence(aSeq, 2),  Sequence(bSeq, 4),  Sequence(cSeq, 4),
    Sequence(dSeq, 3),  Sequence(eSeq, 1),  Sequence(fSeq, 4),
    Sequence(gSeq, 3),  Sequence(hSeq, 4),  Sequence(iSeq, 2),
    Sequence(jSeq, 4),  Sequence(kSeq, 3),  Sequence(lSeq, 4),
    Sequence(mSeq, 2),  Sequence(nSeq, 2),  Sequence(oSeq, 3),
    Sequence(pSeq, 4),  Sequence(qSeq, 4),  Sequence(rSeq, 3),
    Sequence(sSeq, 3),  Sequence(tSeq, 1),  Sequence(uSeq, 3),
    Sequence(vSeq, 4),  Sequence(wSeq, 3),  Sequence(xSeq, 4),
    Sequence(ySeq, 4),  Sequence(zSeq, 4),

    Sequence(oneSeq, 5),   Sequence(twoSeq, 5),   Sequence(threeSeq, 5),
    Sequence(fourSeq, 5),  Sequence(fiveSeq, 5),  Sequence(sixSeq, 5),
    Sequence(sevenSeq, 5), Sequence(eightSeq, 5), Sequence(nineSeq, 5),
    Sequence(zeroSeq, 5),

    Sequence(questionSeq, 6), Sequence(exclamSeq, 6), Sequence(periodSeq, 6),
    Sequence(commaSeq, 6),    Sequence(semicolonSeq, 6), Sequence(colonSeq, 6),
    Sequence(plusSeq, 5),     Sequence(minusSeq, 6),  Sequence(slashSeq, 5),
    Sequence(equalsSeq, 5)
};

char charAlphabet[] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6',
  '7','8','9','0','?','!','.',',',';',':','+','-','/','='
};

Sequence phrase[10]; 

//--------------------------------------------------------------------------------------------------------------------------------


// const UnorderedMap<Sequence, char> = {
//   {}
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
    
    return currentSequence;
}

void Decode() {
  String decoded_message = "";

  for (int i = 0; i < 10; i++) {
    Sequence sq = phrase[i];

    for (int j = 0; j < 46; j++) {
      if (sq == seqAlphabet[j])
        decoded_message += charAlphabet[j];
      else
        continue;
    }
  }

  Serial.println(decoded_message);

}

void setup() {
  String starter_message = "Morse Code Reader Output:";
  Serial.println(starter_message);
  
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.begin(9600);

  int size = sizeof(charAlphabet) / sizeof(charAlphabet[0]); //I will remove it later, I just want to know, and I can't be bother to count XD
  Serial.println();

}

//rework the loop to do the following.
//If there is a change in button_value, call the sequencer.
//If not print out a waiting message, and call the decoder
//Do not take in any inputs while the decoder is running.

int p = 0; //phrase counter

void loop() {
  
  byte value = digitalRead(button);

  if (value != previous_value) {
    phrase[p] = Sequencer();
    p++;
  }
  else if (pause_duration >= 6000)
    Decode();

}
  //Move these comments to the ReadMe?
  //Program now calculates button pressed duration correctly. We can use this to convert ticks into dots and dashes (using reasonable ranges).
  //From there we can create a dictionary, with all of the morse characters.
  //In the loop() store every n seconds all ticks, or store ticks into the same object until you reach an x ms long pause (pick one). Or do both and compare lol.


//I will make global parameters for dit and dah lengths (as I am not a pro morse code communicator and I will absolutely need to slow down the pace)
//I will also ensure reasonably forgiving ranges for the time it takes to register a sequence.

//between each character there is one dit (1 time unit)
//1 dit is a dot, and 1 dah is a dash (3 time units)
//between words --> dah dit dah (7 time units)

//As a simplifying assumption I will begin by creating a sequence class for a single sentence/sequence, I will later on try to generalize the program for continuous use.

//Well, program is too fat, I'm gonna have to trim it down, quite a bit.