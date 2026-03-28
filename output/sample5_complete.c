#include <stdio.h>
int check_fuel_status(int fuel_level) {
    if ((fuel_level > 75)) {
        return 3;
    }
    else if ((fuel_level > 50)) {
        return 2;
    }
    else if ((fuel_level > 25)) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
    printf("=================================\n");
    printf("  SPACE MISSION CONTROL CENTER\n");
    printf("=================================\n");
    int fuel = 100;
    int oxygen = 100;
    int counter = 0;
    float temperature = 22.5;
    double distance = 0.0;
    char mission_code = 'A';
    int status = 1;
    int sensor_data[5] = {98, 95, 97, 94, 96};
    printf("Initiating countdown...\n");
    for (int i = 10; (i > 0); i--) {
        printf("%d\n", i);
    }
    printf("LIFTOFF!\n");
    printf("--- Phase 1: Ascent ---\n");
    while (((fuel > 60) && (distance < 1000.0))) {
        fuel = (fuel - 5);
        distance = (distance + 100.0);
        if ((fuel == 75)) {
            printf("Fuel at 75%\n");
        }
        printf("%d\n", fuel);
    }
    printf("--- Phase 2: Orbit ---\n");
    printf("Entering stable orbit\n");
    int total_reading = 0;
    for (int j = 0; (j < 5); j++) {
        total_reading = (total_reading + sensor_data[j]);
        if ((sensor_data[j] < 95)) {
            printf("Warning: Low sensor reading\n");
        }
    }
    int avg_reading = (total_reading / 5);
    printf("Average sensor reading:\n");
    printf("%d\n", avg_reading);
    int fuel_status = check_fuel_status(fuel);
    switch (fuel_status) {
        case 3:
            printf("Fuel Status: EXCELLENT\n");
            break;
        case 2:
            printf("Fuel Status: GOOD\n");
            break;
        case 1:
            printf("Fuel Status: LOW\n");
            break;
        case 0:
            printf("Fuel Status: CRITICAL\n");
            break;
        default:
            printf("Fuel Status: UNKNOWN\n");
            break;
    }
    printf("--- Phase 3: Return ---\n");
    counter = 0;
    do {
        printf("Descent stage:\n");
        printf("%d\n", counter);
        counter++;
        fuel = (fuel - 3);
        if ((fuel < 20)) {
            printf("Emergency: Low fuel!\n");
            break;
        }
    }
    while ((counter < 5));
    printf("--- Final Status ---\n");
    if (((fuel > 10) && (oxygen > 50))) {
        printf("Systems nominal for landing\n");
    }
    else if (((fuel > 5) || (oxygen > 30))) {
        printf("Marginal conditions - proceed with caution\n");
    }
    else {
        printf("Critical conditions - emergency protocols\n");
    }
    if ((!((temperature > 30.0)))) {
        printf("Temperature within safe limits\n");
    }
    printf("=================================\n");
    printf("     MISSION SUMMARY\n");
    printf("=================================\n");
    printf("Final fuel level:\n");
    printf("%d\n", fuel);
    printf("Distance traveled:\n");
    printf("%d\n", distance);
    printf("Mission code:\n");
    printf("%d\n", mission_code);
    printf("=================================\n");
    printf("  MISSION COMPLETED SUCCESSFULLY\n");
    printf("=================================\n");
    return 0;
}
