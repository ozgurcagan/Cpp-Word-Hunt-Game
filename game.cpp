#include "Game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm> 
#include <cctype>   

using namespace std;

Game::Game(int difficulty) : lives(difficulty) {
    secretWord = loadRandomWord();
    transform(secretWord.begin(), secretWord.end(), secretWord.begin(), ::tolower);
    hiddenWord = string(secretWord.length(), '_');
}

string Game::loadRandomWord() {
    vector<string> words;
    ifstream file("words.txt");

    if (!file.is_open()) {
        cerr << "HATA: words.txt bulunamadi.\n";
        exit(EXIT_FAILURE);
    }

    string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    if (words.empty()) {
        cerr << "HATA: words.txt bos.\n";
        exit(EXIT_FAILURE);
    }

    srand(static_cast<unsigned>(time(nullptr)));
    return words[rand() % words.size()];
}

bool Game::isAlreadyGuessed(char c) const {
    for (char g : guessedLetters)
        if (g == c) return true;
    return false;
}

void Game::revealLetters(char c) {
    for (size_t i = 0; i < secretWord.length(); i++) {
        if (secretWord[i] == c)
            hiddenWord[i] = c;
    }
}


int Game::play() {
    string input;
    char guess;
    int point = 100;

    cout << "\n----------------------------------------";
    cout << "\nGizli Kelime: " << secretWord.length() << " harfli";
    cout << "\n----------------------------------------\n";

    while (lives > 0 && hiddenWord != secretWord) {
        cout << "\nKelime: ";
        for (char c : hiddenWord) cout << c << " ";

        cout << "\nKalan Hak: " << lives;
        cout << "\nTahmin: ";
        cin >> input;

        if (input.empty()) continue;
        guess = tolower(input[0]);

        if (isAlreadyGuessed(guess)) {
            cout << ">> Bu harfi zaten girdin!\n";
            continue;
        }

        guessedLetters.push_back(guess);

        if (secretWord.find(guess) != string::npos) {
            revealLetters(guess);
            point += 20;
            cout << ">> Dogru!\n";
        } else {
            lives--;
            point -= 15;
            cout << ">> Yanlis!\n";
        }
        
        if (point < 0) point = 0;
        cout << "Puan: " << point << endl;
    }

    if (hiddenWord == secretWord) {
        cout << "\nKAZANDIN! Kelime: " << secretWord << endl;
    } else {
        cout << "\nKAYBETTIN! Kelime: " << secretWord << endl;
    }

    return point;
}