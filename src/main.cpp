#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// Lexicon with words and their sentiment scores
unordered_map<string, int> lexicon = {
    {"happy", 2}, {"excellent", 3}, {"sad", -2}, {"bad", -3},
    {"great", 3}, {"terrible", -3}, {"good", 2}, {"poor", -2},
    {"love", 3}, {"hate", -3}, {"enjoy", 2}, {"dislike", -2},
    {"satisfied", 2}, {"dissatisfied", -2}, {"delay", -2},
    {"amazing", 3}, {"horrible", -3}, {"wonderful", 3}, {"mediocre", -1},
    {"fantastic", 3}, {"acceptable", 1}, {"appreciate", 2}, {"overjoyed", 3},
    {"slow", -2} // Additional lexicon entries
};

// Function to tokenize and preprocess text
vector<string> tokenize(const string& text) {
    stringstream ss(text);
    string word;
    vector<string> words;

    while (ss >> word) {
        // Convert word to lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        // Remove punctuation (simple approach)
        word.erase(remove_if(word.begin(), word.end(), [](char c) { return ispunct(static_cast<unsigned char>(c)); }), word.end());
        words.push_back(word);
    }
    return words;
}

// Function to calculate sentiment score based on lexicon and context
int calculateSentimentWithContext(const vector<string>& words) {
    int score = 0;
    bool negation = false;
    vector<int> scores;  // Store scores for segments

    for (size_t i = 0; i < words.size(); ++i) {
        string word = words[i];

        // Handle negation ("not" inverts the next sentiment word)
        if (word == "not") {
            negation = true;
            continue;
        }

        // Handle intensifiers (e.g., "very" doubles the score of the next word)
        bool intensify = (i > 0 && words[i - 1] == "very");

        // Handle adversative conjunctions (e.g., "but" resets the score)
        if (word == "but") {
            // Save the score before "but" to the scores vector
            scores.push_back(score);
            score = 0;  // Reset score after "but"
            continue;
        }

        // Check if word is in lexicon
        if (lexicon.find(word) != lexicon.end()) {
            int wordScore = lexicon[word];

            // Apply negation if present
            if (negation) {
                wordScore = -wordScore;
                negation = false; // Reset negation after applying
            }

            // Apply intensification if present
            if (intensify) {
                wordScore *= 2;
            }

            // Add word's score to total score
            score += wordScore;
        }
    }

    // Save the final score after processing the whole input
    scores.push_back(score);

    // Combine all segment scores
    int totalScore = 0;
    for (int s : scores) {
        totalScore += s;
    }

    return totalScore;
}

// Function to classify sentiment based on score
string classifySentiment(int score) {
    if (score > 0) return "Positive";
    else if (score < 0) return "Negative";
    else return "Neutral";
}

// Main function
int main() {
    // Take input from the user
    string post;
    cout << "Enter a social media post: ";
    getline(cin, post);

    // Tokenize and preprocess the post
    vector<string> words = tokenize(post);

    // Calculate sentiment score with context-based rules
    int score = calculateSentimentWithContext(words);

    // Classify the final sentiment
    string sentiment = classifySentiment(score);

    // Display the results
    cout << "Post: " << post << endl;
    cout << "Sentiment Score: " << score << endl;
    cout << "Sentiment Classification: " << sentiment << endl;

    return 0;
}
