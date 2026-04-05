#include <stdio.h>
#include <stdlib.h>

int main() {
    int fuel = 100;
    int counter = 0;
    float temperature = 25.500000;
    int scores[3] = {85, 90, 78};
    printf("%s\n", "Countdown:");
    for (int i = 10; (i > 0); i = (i - 1)) {
        printf("%d\n", i);
    }
    while (((fuel > 0) && (fuel <= 100))) {
        if ((fuel == 50)) {
            printf("%s\n", "Half fuel remaining");
        }
        fuel = (fuel - 10);
        if ((fuel < 20)) {
            break;
        }
    }
    do {
        printf("%d\n", counter);
        counter = (counter + 1);
    } while ((counter < 5));
    int status = 2;
    switch (status) {
        case 1:
            printf("%s\n", "Status: Active");
            break;
        case 2:
            printf("%s\n", "Status: Standby");
            break;
        default:
            printf("%s\n", "Status: Unknown");
            break;
    }
    int total = 0;
    for (int j = 0; (j < 3); j = (j + 1)) {
        total = (total + scores[j]);
    }
    printf("%d\n", total);
    if (((temperature > 20.000000) && (temperature < 30.000000))) {
        printf("%s\n", "Temperature is comfortable");
    } else {
        printf("%s\n", "Temperature is extreme");
    }
    return 0;
}
