# Arduino-MorseCode-Decryption

## Objective:
This project aims to take in morse coded user input pressed on a physical button, and print out the translated output.
This project will begin with a simple hardware setup. A breadboard, few resistors, a button, and an Arduino Uno. I've done some really short projects on Arduino but I would like to further familiarize myself with the language (C++ variant) and the libraries. Hence why I chose this project. While not excessively complex, it has plenty of room to grow in complexity, and improve.

---

I reworked the current prototype circuit to include a buzzer, wired in series with the button input, and the LED indicator. This eliminates a step I previously had of reading the button input and manually turning on the buzzer. This setup allows for the circuit to do so more organically. I intend to include a full circuit schematic towards the end of the project, but it will change as I add more I/O devices.

---

Through some research and conversation with a friend, I've learned that morse code was actually designed to work as a binary decision tree, where left moves are an added dot, and right moves are an added dash. The tree is made in a way where the most common letters are found higher up in the tree. Using this I wrote a MorseTree class in my program, and hardcoded into its constructor the entire tree. This will allow for O(logn) lookups for decryption, which is plenty fast enough. The primary constraint for this project, because of the arduino UNO's architecture, was the available memory for the program itself.
Previous versions of this project outline this issue, as the program would compile, but not upload to the board for testing.

This reworked data-structure is therefore both more time and space efficient.

---

Through some testing I recorded pressed durations for what I thought would be dots and dashes, and used the actual ratios between each values to create if statements to record each character properly.
100ms - dot : 500ms - dash : 1100ms -space (all +- 50ms)
This respects dash being 3 * dot, and space being 7 * dot.
After more testing I might update these boundaries.

---

Again, due to the UNO's architecture this program is designed to work sequentially, with no simultaneous processes occuring. (It also does not help that I have not yet learned multi-threading and simultaneous programming).

---

