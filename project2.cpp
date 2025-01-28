#include <iostream>
#include <string>
using namespace std;

class Chip {

  char chipType; // Type of the chip (A: Addition, S: Subtraction, etc.)
  string id; // Unique identifier for the chip
  Chip* input1; // Pointer to the first input chip
  Chip* input2; // Pointer to the second input chip (can be NULL)
  Chip* output; // Ptr to the output chip (is NULL for output chips)
  double inputValue; //for the input chip

public:
  // Constructor
  Chip(char chipType, string id) {
      this->chipType = chipType;
      this->id = id;
      this->input1 = nullptr;
      this->input2 = nullptr;
      this->output = nullptr;
      this->inputValue = 0.0;
  }

  //getters and setters
  char getChipType() {
    return chipType;
  }
  string getId() {
    return id;
  }
  Chip* getInput1() {
    return input1;
  }
  Chip* getInput2() {
    return input2;
  }
  Chip* getOutput() {
    return output;
  }
  double getInputValue() {
    return inputValue;
  }

  void setInput1(Chip* inputChip) { // Sets the first input chip
      this->input1 = inputChip;
  }
  void setInput2(Chip* inputChip) { // Sets the second input chip (can be NULL)
     this->input2 = inputChip;
  }
  void setOutput(Chip* outputChip) { // Sets the output chip (can be NULL)
     output = outputChip;
  }
  void setInputValue(double inputValue) { //sets the input value
     this->inputValue = inputValue;
  }

  void compute() {; // compute the output value of chip
     
      if(input1 != nullptr) {
        input1->compute();
      }
      if (input2 != nullptr) {
        input2->compute();
      }

      //operation based on chip type
        //Addition chip A
      switch (chipType) {
        case 'A': // Addition chip
            inputValue = input1->inputValue + input2->inputValue;
            output->setInputValue(inputValue);
            break;
        
        case 'N':
            inputValue = -input1->inputValue;
            output->setInputValue(inputValue);
            break;

        case 'M':
            inputValue = input1->inputValue * input2->inputValue;
            output->setInputValue(inputValue);
            break;

        case 'D': // Division chip
            inputValue = input1->inputValue / input2->inputValue;
            output->setInputValue(inputValue);
            break;

        case 'S': //Subtraction chip
            inputValue = input1->inputValue - input2->inputValue;
            output->setInputValue(inputValue);
            break;

        case 'O': 
            break;
        }
  }

  //method to display chip info
  void display() { // Displays the chip's information
    switch(chipType) {
        case 'I':
            cout << chipType + id << ", Output = " << output->getChipType() + output->getId() << endl;
            break;

        case 'O':
            if(id == "50") {
                return;
            }
            break;

        case 'A':
            cout << chipType + id << ", Input 1 = " << input1->getChipType() + input1->getId() << ", Input 2 = " << input2->getChipType() + input2->getId() << ", Output = " << output->getChipType() + output->getId() << endl;
            break;

        case 'N':
            cout << chipType + id << ", Input 1 = " << input1->getChipType() + input1->getId() << ", Input 2 = None, Output = " << output->getChipType() + output->getId() << endl;
            break;

        case 'M':
            cout << chipType + id << ", Input 1 = " << input1->getChipType() + input1->getId() << ", Input 2 = " << input2->getChipType() + input2->getId() << ", Output = " <<  output->getChipType() + output->getId() << endl;
            break;

        case 'D':
            cout << chipType + id << ", Input 1 = " << input1->getChipType() + input1->getId() << ", Input 2 = " << input2->getChipType() + input2->getId() << ", Output = " <<  output->getChipType() + output->getId() << endl;
            break;

        case 'S':
            cout << chipType + id << ", Input 1 = " << input1->getChipType() + input1->getId() << ", Input 2 = " << input2->getChipType() + input2->getId() << ", Output = " <<  output->getChipType() + output->getId() << endl;
            break;
    }
  }
};

//main
int main() {
    int numChip;
    Chip** allChip;
    Chip* o50Chip = nullptr;  // Initialize to nullptr
    int numCommands;
    int index = -1; // Initialize index

    cin >> numChip;
    allChip = new Chip*[numChip];

    // Create an array Chip objects pointers
    for (int i = 0; i < numChip; i++) {
        string input;
        cin >> input;
        allChip[i] = new Chip(input[0], input.substr(1));
    }

    cin >> numCommands;
    for (int i = 0; i < numCommands; i++) {
        string cType, inp, out;
        double numVal;

        cin >> cType;

        if (cType == "I") {
            cin >> inp >> numVal;
            for (int j = 0; j < numChip; j++) {
                if (allChip[j]->getChipType() + allChip[j]->getId() == inp) {
                    allChip[j]->setInputValue(numVal);
                }
            }
        } else if (cType == "A") {
            cin >> inp >> out;
            for (int j = 0; j < numChip; j++) {
                if (allChip[j]->getChipType() + allChip[j]->getId() == inp) {
                    for (int k = 0; k < numChip; k++) {
                        if (allChip[k]->getChipType() + allChip[k]->getId() == out) {
                            if (allChip[k]->getInput1() == nullptr) {
                                allChip[j]->setOutput(allChip[k]);
                                allChip[k]->setInput1(allChip[j]);
                            } else {
                                allChip[j]->setOutput(allChip[k]);
                                allChip[k]->setInput2(allChip[j]);
                            }
                        }
                    }
                }
            }
        } else if (cType == "O") {
            cin >> inp;
            for (int j = 0; j < numChip; j++) {
                allChip[j]->compute();
                if (allChip[j]->getChipType() + allChip[j]->getId() == inp) {
                    allChip[j]->setOutput(allChip[j]);
                }
            }
        }
    }

    cout << "Computation Starts" << endl;
    for (int i = 0; i < numChip; i++) {
        allChip[i]->compute();
        if (allChip[i]->getChipType() == 'O' && allChip[i]->getId() == "50") {
            index = i;
        }
    }

    if (index != -1) {
        cout << "The output value from this circuit is " << allChip[index]->getInputValue() << endl;
    }

    cout << "***** Showing the connections that were established" << endl;
    for (int i = 0; i < numChip; i++) {
        if (allChip[i]->getChipType() == 'O' && allChip[i]->getId() == "50") {
            o50Chip = allChip[i];
        } else {
            allChip[i]->display();
        }
    }

    if (o50Chip != nullptr) {
        o50Chip->display();
        cout << o50Chip->getChipType() + o50Chip->getId() << ", Input 1 = " << o50Chip->getInput1()->getChipType() + o50Chip->getInput1()->getId() << endl;
    }

    return 0;
}


/*LLM/AI Tool Usage

Prompts and suggestions:

1. I asked to help me understand how to create a method for computing output value of a chip, this is when I learned about the compute method. I was able to perform different opperations based on the chip types.
the prompt I used was "How can I implment the compute method to be working well with different chip operations"
Rationale: to correctly compute the output value of the chip, I needed to implement various arithmetic operations depending on the chip type. The prompt provided a structured approach to implementing this logic.
Developing: From here I knew I had to create different arithmetic operations depending on the chip type, helping me create the structure of the code more. I started with additions, then added the other operations.

2. I had trouble working with displaying the output values so i prompted "how do I display the output values of each chip into a display method?"
Rationale: I wanted my display method to really show all the connections while making sure the operations where correct and the o50 chip was shown last.
Development: Imade my display to iterate over each chip and making sure o50 chip was displayed last.

Testing and Debugging

I tested my code, specificaly the compute method by trying the different inputs and comparing it to the given outputs. This showed me that some parts of the compute worked while the others did not so it helped me identify exxactly what part was wrong.

Another test I did was with the display method since i knew this was what was important to show correctly. first I noticed the o50 chip was displaying incorrectly in the wrong spot
to fix this I used a loop to sort through all chips and saving o50 to last. once this was implemnted and more errors were fixed my whole display mehtod was correct.
*/

