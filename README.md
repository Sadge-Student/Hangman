# Hangman

> Write a Hangman game using C++​.  Use one file only.  Name it hangman.cpp

Hints:​

+ Use a vector to store three hangman words.  The words can be hardcoded.
+ Your game should shuffle the words in the vector before selecting one for use in the game.
+ The game should display the player's progress after each guess. Assume that the hangman word is 'BONUS'. If the player has guessed the following letters A, N, S, B your game should report that the Hangman word (so far) is B-N-S.  It should also show the number of incorrect guesses permitted and number of incorrect guesses remaining.
+ The game should count the number of guesses made by the player.
+ Win condition: the player guesses the letters in the hangman word
+ Lose condition: the number of incorrect guesses exceeds the number of letters in the hangman word
+ Use #include <cctype> to convert characters to upper case​
+ Use guess = toupper(guess); //cctype uppercase function
+ Comment your code liberally.  Ensure that your code is indented where appropriate.
+ Upload your .cpp file here (file must have a .cpp extension) Do not upload zipped files.  Do not upload a Visual Studio project.
