
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
const char dot = 'd';       //100; //We can give +/- 50 to begin with //denoted d
const char dash = 'D';      //500; //where dash == 3*dot (again +/-50) // denoted D
const char space = 's';     //1100; //where space == 7*dot (+/-50) // denoted s

//Initializes the condition for the state change loop
byte previous_value = LOW;


int p = 0; //phrase counter

//--------------------------------------------------------------------------------------------------------------------------------

class Sequence {
    
    public:

      char charSequence[6] = {0};
      //Array is 0 initialized
      int size = 0;
    
      Sequence() {}; //Default Constructor;

};

//--------------------------------------------------------------------------------------------------------------------------------

Sequence phrase[10];

//--------------------------------------------------------------------------------------------------------------------------------

class MorseTree {
    private:
      class Node {
        public:
          char data;  
          Node* left;
          Node* right;

          Node(char c) { //parameter based constructor
            this->data = c;
            this->left = nullptr;
            this->right = nullptr;
          }

      };
    
    Node* root;

    public:

      char decodeSequence(const Sequence& seq) {
        Node* current = root;

        for (int i = 0; i < seq.size; i++) {
          if (seq.charSequence[i] == dot)
            current = current->left;
          else if (seq.charSequence[i] == dash)
            current = current->right;

          if (current == nullptr)
            return '?';  // invalid sequence
        }

        return current->data;
      }

      MorseTree() {

        root = new Node('\0');

        // Level 1
        root->left  = new Node('E');
        root->right = new Node('T');

        // Level 2
        root->left->left  = new Node('I');
        root->left->right = new Node('A');
        root->right->left = new Node('N');
        root->right->right= new Node('M');

        // Level 3
        root->left->left->left   = new Node('S');
        root->left->left->right  = new Node('U');
        root->left->right->left  = new Node('R');
        root->left->right->right = new Node('W');
        root->right->left->left  = new Node('D');
        root->right->left->right = new Node('K');
        root->right->right->left = new Node('G');
        root->right->right->right= new Node('O');

        // Level 4
        root->left->left->left->left   = new Node('H');
        root->left->left->left->right  = new Node('V');
        root->left->left->right->left  = new Node('F');
        root->left->left->right->right = new Node('\0');
        root->left->right->left->left  = new Node('L');
        root->left->right->left->right = new Node('\0');
        root->left->right->right->left = new Node('P');
        root->left->right->right->right= new Node('J');

        root->right->left->left->left   = new Node('B');
        root->right->left->left->right  = new Node('X');
        root->right->left->right->left  = new Node('C');
        root->right->left->right->right = new Node('Y');
        root->right->right->left->left  = new Node('Z');
        root->right->right->left->right = new Node('Q');
        root->right->right->right->left = new Node('\0');
        root->right->right->right->right= new Node('\0');

        // Level 5 (numbers + some symbols)
        root->left->left->left->left->left   = new Node('5');
        root->left->left->left->left->right  = new Node('4');
        root->left->left->left->right->right = new Node('3');
        root->left->left->right->right->right= new Node('2');
        root->left->right->right->right->right= new Node('1');

        root->right->left->left->left->left  = new Node('6');
        root->right->right->left->left->left = new Node('7');
        root->right->right->right->left->left= new Node('8');
        root->right->right->right->right->left= new Node('9');
        root->right->right->right->right->right= new Node('0');

        // Level 6 (punctuation)
        root->left->left->right->right->left->left = new Node('?');
        root->left->right->left->right->left->right= new Node('.');
        root->right->left->right->left->right->left= new Node(';');
        root->right->left->right->left->right->right= new Node('!');
        root->right->right->left->left->right->right= new Node(',');
        root->right->left->left->left->left->right  = new Node('-');
        root->right->left->left->left->right        = new Node('=');
        root->right->left->left->right->left        = new Node('/');
        root->right->right->right->left->right      = new Node(':');
        root->left->right->left->right->left        = new Node('+');

      }



};

//--------------------------------------------------------------------------------------------------------------------------------

MorseTree alphabet;

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
        } 
        else {
            pause_start = millis();
            press_duration = millis() - start_time;

            if (50 <= press_duration && press_duration <= 150)
              currentSequence.charSequence[i] = dot;
  
            else if (450 <= press_duration && press_duration <= 550)
              currentSequence.charSequence[i] = dash;

            i++;

            digitalWrite(buzzer, LOW);

            //These are for debugging, we'll do a bit of front-end later.
            Serial.print("Duration: ");
            Serial.print(press_duration);
            Serial.print("ms");
            Serial.println();
        }
        delay(5); //minimizes false readings.
      }
      previous_value = value;

    } while (pause_duration < 6000);
    
    currentSequence.size = i;
    
    return currentSequence;
}

//--------------------------------------------------------------------------------------------------------------------------------

void Decode() {
  String decoded_message = "";

  for (int i = 0; i < p; i++)
    decoded_message += alphabet.decodeSequence(phrase[i]);

  Serial.println(decoded_message);
}

//--------------------------------------------------------------------------------------------------------------------------------


void setup() {
  String starter_message = "Morse Code Reader Output:";
  Serial.println(starter_message);
  
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.begin(9600);

  Serial.println();

}

//rework the loop to do the following.
//If there is a change in button_value, call the sequencer.
//If not print out a waiting message, and call the decoder
//Do not take in any inputs while the decoder is running.


void loop() {
  
  pause_duration = 0;
  byte value = digitalRead(button);

  if (value != previous_value) {
    if (p < 10) {
      phrase[p] = Sequencer();
      p++;  
    }
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

//Change of plans, after some reasearch, and conversation with a friend. I've learned that morse code was actually built to be decoded like a binary search tree
//Using the right direction for dashes, and the left direction for dots.
//Using this fact, I can ditch the memory heavy dictionary I currently use.
