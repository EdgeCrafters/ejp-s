#include <stdio.h>

int main() {
    int arr[5] = {0};
    int i=0;

    for (int i=0; i<5; i++) {
        scanf("%d", &arr[i]);
    }

    for (int k=0; k<4; k++) {
        for (int j=0; j<4-k; j++) {
            if (arr[j] > arr[j+1]) {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }

    for (int k=0; k<5; k++) {
        printf("%d ", arr[k]);
    }
}