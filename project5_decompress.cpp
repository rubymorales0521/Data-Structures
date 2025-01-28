#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    vector<string> inputWords; // Stores original input
    map<string, int> wordFrequency; // Tracks words and their frequencies
    vector<string> sortedWords; // Holds words sorted by frequency
    string wordBuffer; // Temporary storage for input words
    int maxFrequency = 0; // Tracks the maximum frequency of words
    vector<int> indices; // Stores the indices for reconstructed text

    // Reading input words until EOF
    while (true) {
        cin >> wordBuffer; // Read a word
        if (cin.fail()) { // Stop if no input is available
            break;
        }
        inputWords.push_back(wordBuffer); // Add the word to the list
    }

    // Calculating word frequencies
    for (size_t k = 0; k < inputWords.size(); k++) {
        auto it = wordFrequency.find(inputWords[k]);
        if (it != wordFrequency.end()) {
            it->second++; // Increment frequency if word exists
            if (it->second > maxFrequency) {
                maxFrequency = it->second; // Update max frequency
            }
        } else {
            wordFrequency.insert(make_pair(inputWords[k], 1)); // Add word with frequency 1
        }
    }

    // Sorting words based on frequency
    for (int freq = maxFrequency; freq >= 1; --freq) {
        for (const auto& entry : wordFrequency) {
            if (entry.second == freq) {
                sortedWords.push_back(entry.first); // Add word to sorted list
            }
        }
    }

    // Mapping original words to sorted indices
    for (const auto& input : inputWords) {
        for (size_t idx = 0; idx < sortedWords.size(); idx++) {
            if (input == sortedWords[idx]) {
                indices.push_back(idx); // Store the index of the matched word
                break;
            }
        }
    }

    // Reconstructing text based on sorted indices
    vector<string> reconstructedText;
    for (const auto& idx : indices) {
        reconstructedText.push_back(sortedWords[idx]);
    }

    // Printing the reconstructed text
    for (size_t i = 0; i < reconstructedText.size(); i++) {
        cout << reconstructedText[i];
        if (i != reconstructedText.size() - 1) {
            cout << " "; // Add space between words
        }
    }
    cout << endl;

    return 0;
}

