#include <stdio.h>
int add_cargo(int x, int y) {
    int result = (x + y);
    return result;
}

int factorial(int n) {
    if ((n <= 1)) {
        return 1;
    }
    int result = (n * factorial((n - 1)));
    return result;
}

void display_message(void) {
    printf("=== Space Mission Status ===\n");
}

int main() {
    display_message();
    int sum = add_cargo(10, 20);
    printf("Sum of 10 and 20:\n");
    printf("%d\n", sum);
    int fact = factorial(5);
    printf("Factorial of 5:\n");
    printf("%d\n", fact);
    int scores[5] = {85, 90, 78, 92, 88};
    printf("Mission scores:\n");
    int total = 0;
    for (int i = 0; (i < 5); i++) {
        printf("%d\n", scores[i]);
        total = (total + scores[i]);
    }
    printf("Total score:\n");
    printf("%d\n", total);
    float temps[3] = {25.5, 28.3, 22.1};
    printf("Temperature readings:\n");
    for (int j = 0; (j < 3); j++) {
        printf("%d\n", temps[j]);
    }
    return 0;
}
