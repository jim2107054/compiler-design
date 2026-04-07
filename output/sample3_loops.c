#include <stdio.h>
#include <stdlib.h>

int main() {
    int counter = 0;
    int fuel = 100;
    printf("%s\n", "Countdown sequence:");
    for (int i = 10; (i > 0); i = (i - 1)) {
        printf("%d\n", i);
    }
    printf("%s\n", "Liftoff!");
    printf("%s\n", "Fuel consumption:");
    while ((fuel > 50)) {
        printf("%d\n", fuel);
        fuel = (fuel - 10);
    }
    printf("%s\n", "Low fuel warning");
    printf("%s\n", "Counter sequence:");
    counter = 0;
    do {
        printf("%d\n", counter);
        counter = (counter + 1);
    } while ((counter < 5));
    printf("%s\n", "Loop with break:");
    for (int j = 0; (j < 10); j = (j + 1)) {
        if ((j == 5)) {
            printf("%s\n", "Breaking at 5");
            break;
        }
        printf("%d\n", j);
    }
    printf("%s\n", "Loop with skip (odd numbers only):");
    for (int k = 0; (k < 10); k = (k + 1)) {
        if (((k % 2) == 0)) {
            continue;
        }
        printf("%d\n", k);
    }
    return 0;
}
