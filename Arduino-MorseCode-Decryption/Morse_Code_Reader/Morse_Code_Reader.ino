
//Encoder Button pin #
const byte encoderButton = 7;

//Decode Button pin #
const byte decoderButton = 12;

//Needed for duration calculation
unsigned int press_start;
unsigned int pause_start;

//These variables need to be global for my function designs.
unsigned int press_duration = 0;
unsigned int pause_duration = 0;

//Might be useful for cleaner code;
const char dot = 'd';       //100; //We can give +/- 50 to begin with //denoted d
const char dash = 'D';      //500; //where dash == 3*dot (again +/-50) // denoted D
const char space = 's';     //1100; //where space == 7*dot (+/-50) // denoted s

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

//Reads button signal, calculates press duratio. Converts accordingly into dots and dashes
//Also measures pause duration to return the function (and move on to decoding)
//Keep displaying for now the durations in the Serial


Sequence Sequencer() {
    Sequence currentSequence;
    int i = 0; //counter

    delay(3000); //one click to call the sequencer. Ready, set, encode!

    do {
      byte value = digitalRead(encoderButton);

      if (value == HIGH) {
        press_start = millis();
        pause_duration = 0;
        while (digitalRead(encoderButton) == HIGH)
        {}
      
        pause_start = millis();
        press_duration = millis() - press_start;
        if (i < 6) {
          if (50 <= press_duration && press_duration <= 150)
            currentSequence.charSequence[i] = dot;
  
          else if (450 <= press_duration && press_duration <= 550)
            currentSequence.charSequence[i] = dash;

        i++;
        currentSequence.size = i;
        }
        
        //These are for debugging, we'll do a bit of front-end later.
        Serial.print("Duration: ");
        Serial.print(press_duration);
        Serial.print("ms");
        Serial.println();
      }
      delay(5); //minimizes false readings.
    } while (pause_duration < 6000);
  
    return currentSequence;
} 

//--------------------------------------------------------------------------------------------------------------------------------

void Decoder() {
  String decoded_message = "";
  p = 0;

  for (int i = 0; i < p; i++)
    decoded_message += alphabet.decodeSequence(phrase[i]);

  
  pause_duration = 0;
  Serial.println(decoded_message);
}

//--------------------------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);

  Serial.println();

  String starter_message = "Morse Code Reader Output:";
  Serial.println(starter_message);
  
  pinMode(encoderButton, INPUT);
  pinMode(decoderButton, INPUT);

}

//rework the loop to do the following.
//If there is a change in button_value, call the sequencer.
//If not print out a waiting message.
//Do not take in any inputs while the decoder is running.


void loop() {

  byte encoderValue = digitalRead(encoderButton);
  byte decoderValue = digitalRead(decoderButton);

  if (encoderValue == HIGH && decoderValue == HIGH) {
    Serial.println("Error! Please perform one task at a time.");
  }
  else if (encoderValue == HIGH) {
    Sequencer();
  }
  else if (decoderValue == HIGH) {
    Decoder();
  }
}


//between each character there is one dit (1 time unit)
//1 dit is a dot, and 1 dah is a dash (3 time units)
//between words --> dah dit dah (7 time units)
