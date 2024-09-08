#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{

    srand(time(0));

    int number_to_guess = rand() % 100 + 1;
    int player_guess;
    int numberof_tries = 0;

    cout << "Welcome to the number Guessing game!\n";

    cout << "I have selected a number between 1 nad 100\n";

    do
    {
        cout << "Enter your guess:";
        cin >> player_guess;
        numberof_tries++;

        if (player_guess > number_to_guess)
        {
            cout << "Too high! Try again\n";
        }
        else if (player_guess < number_to_guess)
        {
            cout << "Too low! Try again\n";
        }
        else
        {
            cout << "Conratulations! You guessed it right!\n";
            cout<<"It took you :"<<numberof_tries<<" tries to guess!\n";
        }
    } while (player_guess != number_to_guess);

    return 0;
}