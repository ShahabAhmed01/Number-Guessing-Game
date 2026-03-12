// +-------------------------------------------------+
// |         NUMBER GUESSING GAME  v1.0             |
// |              written in C++                    |
// +-------------------------------------------------+

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

using namespace std;

// ── Colors ────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[38;5;203m"
#define BLUE    "\033[38;5;75m"
#define GREEN   "\033[38;5;84m"
#define YELLOW  "\033[38;5;220m"
#define GRAY    "\033[38;5;240m"
#define WHITE   "\033[38;5;255m"
#define CYAN    "\033[38;5;116m"

// ── Constants ─────────────────────────────────────
const int MIN        = 1;
const int MAX        = 100;
const int MAX_TRIES  = 7;

// =================================================
//  Enable ANSI colors on Windows
// =================================================
void enableColors() {
    #ifdef _WIN32
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD  mode    = 0;
        GetConsoleMode(console, &mode);
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleOutputCP(CP_UTF8);
    #endif
}

// =================================================
//  Draw attempt progress bar  e.g. [###....]
// =================================================
void drawProgressBar(int used, int total) {
    cout << "\n  " << GRAY << "[";

    for (int i = 0; i < total; i++) {
        bool filled  = (i < used);
        bool danger  = (used >= total - 2);

        if (filled)
            cout << (danger ? RED : YELLOW) << "#";
        else
            cout << GRAY << ".";
    }

    int left = total - used;
    cout << GRAY << "]" << RESET;
    cout << "  " << WHITE << BOLD << left << RESET
         << GRAY << " attempt" << (left != 1 ? "s" : "") << " left\n";
}

// =================================================
//  Return a hint string based on how far off guess is
// =================================================
string getHint(int guess, int secret) {
    int  diff      = abs(guess - secret);
    bool tooHigh   = (guess > secret);

    string direction = tooHigh ? "HIGH  /\\" : "LOW   \\/";
    string intensity;

    if      (diff > 40) intensity = "Way too  ";
    else if (diff > 20) intensity = "Too      ";
    else if (diff > 10) intensity = "A bit    ";
    else                intensity = "Just     ";

    return intensity + direction;
}

// =================================================
//  Print the game UI (title, bar, hint, history)
// =================================================
void renderUI(int attempt, int guesses[], int guessCount,
              const string& message, const string& messageColor)
{
    // Title box
    cout << "\n\n";
    cout << "  " << GRAY << "+----------------------------------+\n";
    cout << "  " << GRAY << "|" << BOLD << WHITE << "    **  NUMBER  ORACLE  **      " << GRAY << "|\n";
    cout << "  " << GRAY << "|" << DIM  << GRAY  << "    guess the number 1 - 100    " << GRAY << "|\n";
    cout << "  " << GRAY << "+----------------------------------+\n";

    // Attempts bar
    drawProgressBar(attempt, MAX_TRIES);

    // Hint message
    cout << "\n  " << messageColor << BOLD << "  " << message << RESET << "\n";

    // Previous guesses
    if (guessCount > 0) {
        cout << "\n  " << GRAY << DIM << "history | ";
        for (int i = 0; i < guessCount; i++)
            cout << WHITE << guesses[i] << GRAY << " ";
        cout << RESET << "\n";
    }

    // Input prompt
    cout << "\n  " << CYAN << "  > " << WHITE;
}

// =================================================
//  Print win or lose result screen
// =================================================
void renderResult(bool won, int secret, int attempts) {
    cout << "\n\n";
    cout << "  " << GRAY << "+----------------------------------+\n";

    if (won) {
        cout << "  " << GRAY << "|  " << GREEN << BOLD
             << "[+] CORRECT! You cracked it!    " << GRAY << "|\n";
        cout << "  " << GRAY << "|  " << WHITE
             << "    Number: " << BOLD << YELLOW << secret
             << RESET << WHITE << "  Guesses: " << BOLD << attempts
             << RESET << WHITE << "              " << GRAY << "|\n";
    } else {
        cout << "  " << GRAY << "|  " << RED << BOLD
             << "[X] OUT OF TRIES!               " << GRAY << "|\n";
        cout << "  " << GRAY << "|  " << WHITE
             << "    The number was " << BOLD << YELLOW << secret
             << RESET << WHITE << ".              " << GRAY << "|\n";
    }

    cout << "  " << GRAY << "+----------------------------------+\n\n";
}

// =================================================
//  Play one full round of the game
// =================================================
void playGame() {
    int  secret   = rand() % MAX + MIN;   // pick secret number
    int  attempt  = 0;
    int  guesses[MAX_TRIES];
    bool won      = false;

    string message      = "I have a number in mind. Can you find it?";
    string messageColor = CYAN;

    while (attempt < MAX_TRIES && !won) {

        renderUI(attempt, guesses, attempt, message, messageColor);

        int guess;
        cin >> guess;

        // Reject bad input
        if (cin.fail() || guess < MIN || guess > MAX) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            message      = "Enter a valid number between 1 and 100!";
            messageColor = YELLOW;
            continue;
        }

        guesses[attempt] = guess;
        attempt++;

        if (guess == secret) {
            won = true;
        } else {
            message      = getHint(guess, secret);
            messageColor = (guess > secret) ? RED : BLUE;
        }
    }

    renderResult(won, secret, attempt);
}

// =================================================
//  Entry point
// =================================================
int main() {
    enableColors();
    srand(static_cast<unsigned>(time(0)));   // seed random number generator

    char choice = 'y';

    while (choice == 'y' || choice == 'Y') {
        playGame();

        cout << "  " << DIM << GRAY << "Play again? (y / n)  " << CYAN << "> " << WHITE;
        cin  >> choice;
        cout << RESET;
    }

    cout << "\n  " << DIM << GRAY << "Thanks for playing. Goodbye.\n\n" << RESET;
    return 0;
}
