#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice = 0;
    int computer = 0;
    int seed = 42;
    int playing = 1;
    int temp = 0;
    int result = 0;
    int wins = 0;
    int losses = 0;
    int ties = 0;
    int rounds = 0;
    printf("%s\n", "================================");
    printf("%s\n", "   ROCK  PAPER  SCISSORS  GAME");
    printf("%s\n", "================================");
    printf("%s\n", "Enter a lucky number (1-99):");
    scanf("%d", &seed);
    while ((playing == 1)) {
        printf("%s\n", "--------------------------------");
        printf("%s\n", "  1 = Rock");
        printf("%s\n", "  2 = Paper");
        printf("%s\n", "  3 = Scissors");
        printf("%s\n", "  0 = Quit");
        printf("%s\n", "Your choice:");
        scanf("%d", &choice);
        if ((choice == 0)) {
            playing = 0;
        } else {
            if (((choice > 0) && (choice <= 3))) {
                rounds = (rounds + 1);
                temp = (seed + rounds);
                temp = (temp * 37);
                temp = (temp + 29);
                seed = (temp % 197);
                if ((seed < 0)) {
                    seed = (0 - seed);
                }
                computer = (seed % 3);
                computer = (computer + 1);
                switch (choice) {
                    case 1:
                        printf("%s\n", "You chose:     ROCK");
                        break;
                    case 2:
                        printf("%s\n", "You chose:     PAPER");
                        break;
                    case 3:
                        printf("%s\n", "You chose:     SCISSORS");
                        break;
                }
                switch (computer) {
                    case 1:
                        printf("%s\n", "Computer chose: ROCK");
                        break;
                    case 2:
                        printf("%s\n", "Computer chose: PAPER");
                        break;
                    case 3:
                        printf("%s\n", "Computer chose: SCISSORS");
                        break;
                }
                result = (choice - computer);
                result = (result + 3);
                result = (result % 3);
                if ((result == 0)) {
                    printf("%s\n", ">> TIE GAME! <<");
                    ties = (ties + 1);
                } else {
                    if ((result == 1)) {
                        printf("%s\n", ">> YOU WIN! <<");
                        wins = (wins + 1);
                    } else {
                        printf("%s\n", ">> COMPUTER WINS! <<");
                        losses = (losses + 1);
                    }
                }
            } else {
                printf("%s\n", "Invalid! Pick 1, 2, 3, or 0.");
            }
        }
    }
    printf("%s\n", "================================");
    printf("%s\n", "        FINAL SCORE");
    printf("%s\n", "================================");
    printf("%s\n", "Rounds played:");
    printf("%d\n", rounds);
    printf("%s\n", "Wins:");
    printf("%d\n", wins);
    printf("%s\n", "Losses:");
    printf("%d\n", losses);
    printf("%s\n", "Ties:");
    printf("%d\n", ties);
    printf("%s\n", "================================");
    printf("%s\n", "Thanks for playing!");
    return 0;
}
