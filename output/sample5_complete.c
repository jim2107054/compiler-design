#include <stdio.h>
#include <stdlib.h>

int check_fuel_status(int fuel_level) {
    if ((fuel_level > 75)) {
        return 3;
    } else {
        if ((fuel_level > 50)) {
            return 2;
        } else {
            if ((fuel_level > 25)) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

int main() {
    printf("%s\n", "=================================");
    printf("%s\n", "  SPACE MISSION CONTROL CENTER");
    printf("%s\n", "=================================");
    int fuel = 100;
    int oxygen = 100;
    int counter = 0;
    float temperature = 22.500000;
    double distance = 0.000000;
    char mission_code = 'A';
    int status = 1;
    int sensor_data[5] = {98, 95, 97, 94, 96};
    printf("%s\n", "Initiating countdown...");
    for (int i = 10; (i > 0); i = (i - 1)) {
        printf("%d\n", i);
    }
    printf("%s\n", "LIFTOFF!");
    printf("%s\n", "--- Phase 1: Ascent ---");
    while (((fuel > 60) && (distance < 1000.000000))) {
        fuel = (fuel - 5);
        distance = (distance + 100.000000);
        if ((fuel == 75)) {
            printf("%s\n", "Fuel at 75%");
        }
        printf("%d\n", fuel);
    }
    printf("%s\n", "--- Phase 2: Orbit ---");
    printf("%s\n", "Entering stable orbit");
    int total_reading = 0;
    for (int j = 0; (j < 5); j = (j + 1)) {
        total_reading = (total_reading + sensor_data[j]);
        if ((sensor_data[j] < 95)) {
            printf("%s\n", "Warning: Low sensor reading");
        }
    }
    int avg_reading = (total_reading / 5);
    printf("%s\n", "Average sensor reading:");
    printf("%d\n", avg_reading);
    int fuel_status = check_fuel_status(fuel);
    switch (fuel_status) {
        case 3:
            printf("%s\n", "Fuel Status: EXCELLENT");
            break;
        case 2:
            printf("%s\n", "Fuel Status: GOOD");
            break;
        case 1:
            printf("%s\n", "Fuel Status: LOW");
            break;
        case 0:
            printf("%s\n", "Fuel Status: CRITICAL");
            break;
        default:
            printf("%s\n", "Fuel Status: UNKNOWN");
            break;
    }
    printf("%s\n", "--- Phase 3: Return ---");
    counter = 0;
    do {
        printf("%s\n", "Descent stage:");
        printf("%d\n", counter);
        counter = (counter + 1);
        fuel = (fuel - 3);
        if ((fuel < 20)) {
            printf("%s\n", "Emergency: Low fuel!");
            break;
        }
    } while ((counter < 5));
    printf("%s\n", "--- Final Status ---");
    if (((fuel > 10) && (oxygen > 50))) {
        printf("%s\n", "Systems nominal for landing");
    } else {
        if (((fuel > 5) || (oxygen > 30))) {
            printf("%s\n", "Marginal conditions - proceed with caution");
        } else {
            printf("%s\n", "Critical conditions - emergency protocols");
        }
    }
    if (!((temperature > 30.000000))) {
        printf("%s\n", "Temperature within safe limits");
    }
    printf("%s\n", "=================================");
    printf("%s\n", "     MISSION SUMMARY");
    printf("%s\n", "=================================");
    printf("%s\n", "Final fuel level:");
    printf("%d\n", fuel);
    printf("%s\n", "Distance traveled:");
    printf("%lf\n", distance);
    printf("%s\n", "Mission code:");
    printf("%c\n", mission_code);
    printf("%s\n", "=================================");
    printf("%s\n", "  MISSION COMPLETED SUCCESSFULLY");
    printf("%s\n", "=================================");
    return 0;
}
