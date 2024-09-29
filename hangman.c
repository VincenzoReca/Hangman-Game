#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printHangman(char* Mistakes, char* secretWord, const int* cMistakes, char* banWord);
void fillMistakes(char* Mistakes, const int* countMistakes);
char* setGame(char* guess);
void getAttempt(char* guess, char* banWord, int* countMistakes, char* Mistakes, char* WrongAnswers, char* AlreadyUsedChar);
int alreadyUsed(char* AlreadyUsedChar, int LetterAttempted);
int checkWin(char* banWord);
char* toLowerString(char* str);

int main() {
    char* mistakes = malloc(6 * sizeof(char));
    char* wordToGuess = malloc(50 * sizeof(char)); 
    char* bannedWord;  
    char* AlreadyUsedLetters = malloc(27 * sizeof(char));
    int counterMistakes = 0, replay = 0;
    char* wrongLetters = malloc(26 * sizeof(char));

    do {
        counterMistakes = 0;                // Reset mistakes
        for(size_t i = 0; i < 6; i++) {
            mistakes[i] = ' ';              //initialize the mistakes' array
            wrongLetters[i] = ' ';          //initialize the wrongletters' array
            AlreadyUsedLetters[i] = ' ';    //initialize the alreadyusedletters' array
        }
        
        bannedWord = setGame(wordToGuess);                                          // Chiedi di inserire la nuova parola

        while(counterMistakes < 6 && !checkWin(bannedWord)) {                       //while there are lives and you didn't already win
            printHangman(mistakes, wordToGuess, &counterMistakes, bannedWord); 
            getAttempt(wordToGuess, bannedWord, &counterMistakes, mistakes, wrongLetters, AlreadyUsedLetters);
        }

        if(checkWin(bannedWord)) {                                                  //check the winning condition
            printHangman(mistakes, wordToGuess, &counterMistakes, bannedWord); 
            printf("You Win!!! ");
        } else {
            printf("You lose!\t the answer was: %s\n", wordToGuess);
        }

        printf("New Game? (1 = Yes, 0 = No)\n");            //rematch
        scanf("%d", &replay);
        fflush(stdin);
    } while(replay);

    free(mistakes);                                         //free the malloced memory
    free(wordToGuess);
    free(bannedWord);
    free(AlreadyUsedLetters);
    free(wrongLetters);

    return 0;
}

void printHangman(char* Mistakes, char* secretWord, const int* cMistakes, char* banWord) {      //print the hangman
    system("cls");

    printf("\t%c%c%c%c%c%c%c\n\t%c     %c\n\t%c    %c%c%c\n", 201, 205, 205, 205, 205, 205, 187, 186, Mistakes[0], 186, Mistakes[1], Mistakes[2], Mistakes[3]);
    printf("\t%c    %c %c\n      %c%c%c%c%c\n\n", 186, Mistakes[4], Mistakes[5], 205, 205, 202, 205, 205);
    printf("Word to guess:\t%s\n", banWord);                            // print the spaces of the word
}

void fillMistakes(char* Mistakes, const int* countMistakes) {           //manage the mistakes and shows the body of the man

    switch (*countMistakes) {
        case 1:
            Mistakes[0] = 111; 
            break;
        case 2:
            Mistakes[1] = 47;  
            break;
        case 3:
            Mistakes[2] = 124; 
            break;
        case 4:
            Mistakes[3] = 92;
            break;
        case 5:
            Mistakes[4] = 47;  
            break;
        case 6:
            Mistakes[5] = 92; 
            break;
        default:
            break;
    }
}

char* setGame(char* guess) {                //set the word to guess and a banned word that will be displayed under the hangman
    int i;

    printf("Insert the word to guess: \n");                             //get the input amd ban the word to display
    fgets(guess, 50, stdin);              
    guess[strcspn(guess, "\n")] = '\0';   
    guess = toLowerString(guess);
    int len = strlen(guess);
    char* bannedWord = malloc((strlen(guess) + 1) * sizeof(char));  

    for (i = 0; i < len; i++) {
        bannedWord[i] = '_';
    }
    bannedWord[len] = '\0'; 

    return bannedWord;          
}

void getAttempt(char* guess, char* banWord, int* countMistakes, char* Mistakes, char* WrongAnswers, char* AlreadyUsedChar) {
    char letterAttempt;
    int i = 0, isFound = 0, alreadyUsedLetter = 0;
    static int j = 0;  

    do {
        printf("Insert a letter: \t\tWrong letters:\t%s\n", WrongAnswers);      //get the try to guess
        scanf(" %c", &letterAttempt);  
        letterAttempt = tolower(letterAttempt);

        alreadyUsedLetter = alreadyUsed(AlreadyUsedChar, letterAttempt);  
        if (alreadyUsedLetter) {                                                //if the letter was already used, reask the letter
            printf("You already used this letter, try again!\n\n");
            system("pause");
            system("cls");
            printHangman(Mistakes, guess, countMistakes, banWord);
        }
    } while (alreadyUsedLetter); 

    AlreadyUsedChar[j] = letterAttempt;                                         //save the already used char
    j++;

    while (guess[i] != '\0') {                                                  //if guessed, show the right letter in the right position
        if (guess[i] == letterAttempt) {
            banWord[i] = letterAttempt;  
            isFound = 1;
        }
        i++;
    }

    if (!isFound) {                                                             //if the letter is wrong save it and display and print a new part of the hangman
        if(*countMistakes < 5){  
            printf("The letter isn't present in the word!\n\n");
            system("pause");
        }
        WrongAnswers[*countMistakes] = letterAttempt;  
        WrongAnswers[*countMistakes + 1] = '\0'; 
        (*countMistakes)++;
        fillMistakes(Mistakes, countMistakes);  
    }
}

int alreadyUsed(char* AlreadyUsedChar, int LetterAttempted) {                   //check if the letter is already used
    int i = 0;

    while (AlreadyUsedChar[i] != '\0') {
        if (LetterAttempted == AlreadyUsedChar[i]) {
            return 1; 
        }
        i++;
    }
    return 0;  
}

int checkWin(char* banWord) {           //check the win condition
    int i = 0, countUnderscore = 0;

    while(banWord[i] != '\0') {
        if(banWord[i] == '_') {
            countUnderscore++;
        }
        i++;
    }
    if(countUnderscore == 0) {          //return 1 if there are no more underscore and so you have guessed the word entirely, else return 0
        return 1;           
    }else return 0;
}

char* toLowerString(char* str) {         //transform every input in only lower letters
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);  
        i++;
    }
    return str;
}
