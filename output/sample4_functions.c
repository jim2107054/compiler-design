#include <stdio.h>
#include <stdlib.h>

int double_value(int num) {
    return (num * 2);
}

int factorial(int n) {
    if ((n <= 1)) {
        return 1;
    }
    int n_minus_1 = (n - 1);
    int fact_recursive = factorial(n_minus_1);
    int result = (n * fact_recursive);
    return result;
}

void display_message() {
    printf("%s\n", "=== Space Mission Status ===");
}

int main() {
    display_message();
    int val1 = 10;
    int doubled = double_value(val1);
    printf("%s\n", "Double of 10:");
    printf("%d\n", doubled);
    int fact = factorial(5);
    printf("%s\n", "Factorial of 5:");
    printf("%d\n", fact);
    int scores[5] = {85, 90, 78, 92, 88};
    printf("%s\n", "Mission scores:");
    int total = 0;
    for (int i = 0; (i < 5); i = (i + 1)) {
        printf("%d\n", scores[i]);
        total = (total + scores[i]);
    }
    printf("%s\n", "Total score:");
    printf("%d\n", total);
    float temps[3] = {25.500000, 28.299999, 22.100000};
    printf("%s\n", "Temperature readings:");
    for (int j = 0; (j < 3); j = (j + 1)) {
        printf("%f\n", temps[j]);
    }
    return 0;
}
