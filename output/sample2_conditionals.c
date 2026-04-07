#include <stdio.h>
#include <stdlib.h>

int main() {
    int fuel = 150;
    int status = 3;
    if ((fuel > 75)) {
        printf("%s\n", "Fuel level: HIGH");
    } else {
        if ((fuel > 50)) {
            printf("%s\n", "Fuel level: MEDIUM");
        } else {
            if ((fuel > 25)) {
                printf("%s\n", "Fuel level: LOW");
            } else {
                printf("%s\n", "Fuel level: CRITICAL");
            }
        }
    }
    if ((fuel >= 50)) {
        printf("%s\n", "Fuel is sufficient");
    }
    if ((fuel <= 100)) {
        printf("%s\n", "Fuel within limits");
    }
    if ((fuel == 60)) {
        printf("%s\n", "Fuel is exactly 60");
    }
    if ((fuel != 0)) {
        printf("%s\n", "Fuel is not empty");
    }
    if (((fuel > 40) && (fuel < 80))) {
        printf("%s\n", "Fuel in optimal range");
    }
    if (((status == 1) || (status == 2))) {
        printf("%s\n", "Status is valid");
    }
    switch (status) {
        case 1:
            printf("%s\n", "Status: Active");
            break;
        case 2:
            printf("%s\n", "Status: Standby");
            break;
        case 3:
            printf("%s\n", "Status: Completed");
            break;
        default:
            printf("%s\n", "Status: Unknown");
            break;
    }
    return 0;
}
