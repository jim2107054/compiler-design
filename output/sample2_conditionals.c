#include <stdio.h>
int main() {
    int fuel = 60;
    int status = 2;
    if ((fuel > 75)) {
        printf("Fuel level: HIGH\n");
    }
    else if ((fuel > 50)) {
        printf("Fuel level: MEDIUM\n");
    }
    else if ((fuel > 25)) {
        printf("Fuel level: LOW\n");
    }
    else {
        printf("Fuel level: CRITICAL\n");
    }
    if ((fuel >= 50)) {
        printf("Fuel is sufficient\n");
    }
    if ((fuel <= 100)) {
        printf("Fuel within limits\n");
    }
    if ((fuel == 60)) {
        printf("Fuel is exactly 60\n");
    }
    if ((fuel != 0)) {
        printf("Fuel is not empty\n");
    }
    if (((fuel > 40) && (fuel < 80))) {
        printf("Fuel in optimal range\n");
    }
    if (((status == 1) || (status == 2))) {
        printf("Status is valid\n");
    }
    switch (status) {
        case 1:
            printf("Status: Active\n");
            break;
        case 2:
            printf("Status: Standby\n");
            break;
        case 3:
            printf("Status: Completed\n");
            break;
        default:
            printf("Status: Unknown\n");
            break;
    }
    return 0;
}
