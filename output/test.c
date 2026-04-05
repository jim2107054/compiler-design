#include <stdio.h>
#include <stdlib.h>

int main() {
    int fuel = 54;
    float temperature = 25.500000;
    char status = 'A';
    double distance = 150.750000;
    printf("%s\n", "Mission initialized");
    printf("%d\n", fuel);
    printf("%f\n", temperature);
    fuel = 75;
    int result = 0;
    result = (fuel + 25);
    result = (result - 10);
    result = (result * 2);
    result = (result / 4);
    result = (result % 7);
    fuel = (fuel + 1);
    fuel = (fuel - 1);
    printf("%d\n", fuel);
    printf("%d\n", result);
    printf("%s\n", "Mission complete");
    return 0;
}
