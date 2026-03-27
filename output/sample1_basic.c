#include <stdio.h>
int main() {
    int fuel = 100;
    float temperature = 25.5;
    char status = 'A';
    double distance = 1500.75;
    printf("Mission initialized\n");
    printf("%d\n", fuel);
    printf("%d\n", temperature);
    fuel = 75;
    int result = 0;
    result = (fuel + 25);
    result = (result - 10);
    result = (result * 2);
    result = (result / 4);
    result = (result % 7);
    fuel++;
    fuel--;
    printf("Mission complete\n");
    return 0;
}
