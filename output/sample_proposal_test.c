#include <stdio.h>
int main() {
    int fuel = 100;
    int counter = 0;
    float temperature = 25.5;
    int scores[3] = {85, 90, 78};
    printf("Countdown:\n");
    for (int i = 10; (i > 0); i--) {
        printf("%d\n", i);
    }
    while (((fuel > 0) && (fuel <= 100))) {
        if ((fuel == 50)) {
            printf("Half fuel remaining\n");
        }
        fuel = (fuel - 10);
        if ((fuel < 20)) {
            break;
        }
    }
    do {
        printf("%d\n", counter);
        counter++;
    }
    while ((counter < 5));
    int status = 2;
    switch (status) {
        case 1:
            printf("Status: Active\n");
            break;
        case 2:
            printf("Status: Standby\n");
            break;
        default:
            printf("Status: Unknown\n");
            break;
    }
    int total = 0;
    for (int j = 0; (j < 3); j++) {
        total = (total + scores[j]);
    }
    printf("%d\n", total);
    if (((temperature > 20.0) && (temperature < 30.0))) {
        printf("Temperature is comfortable\n");
    }
    else {
        printf("Temperature is extreme\n");
    }
    return 0;
}
