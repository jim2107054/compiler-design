#include <stdio.h>
int main() {
    int counter = 0;
    int fuel = 100;
    printf("Countdown sequence:\n");
    for (int i = 10; (i > 0); i--) {
        printf("%d\n", i);
    }
    printf("Liftoff!\n");
    printf("Fuel consumption:\n");
    while ((fuel > 50)) {
        printf("%d\n", fuel);
        fuel = (fuel - 10);
    }
    printf("Low fuel warning\n");
    printf("Counter sequence:\n");
    counter = 0;
    do {
        printf("%d\n", counter);
        counter++;
    }
    while ((counter < 5));
    printf("Loop with break:\n");
    for (int j = 0; (j < 10); j++) {
        if ((j == 5)) {
            printf("Breaking at 5\n");
            break;
        }
        printf("%d\n", j);
    }
    printf("Loop with skip (odd numbers only):\n");
    for (int k = 0; (k < 10); k++) {
        if (((k % 2) == 0)) {
            continue;
        }
        printf("%d\n", k);
    }
    return 0;
}
