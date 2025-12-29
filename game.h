#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

class Game {
private:
    std::string secretWord;
    std::string hiddenWord;
    int lives;
    std::vector<char> guessedLetters;

    std::string loadRandomWord();
    bool isAlreadyGuessed(char c) const;
    void revealLetters(char c);

public:
    Game(int difficulty);
    
   
    int play(); 
};

#endif