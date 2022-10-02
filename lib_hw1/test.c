#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int* arrinc(int* arr){
    for(int i = 0; i < 20; i++){
        arr[i] = 1;
    }
    return arr;
}

int main(){

    int arr[20];
    for(int i = 0; i < 20; i++){
        arr[i] = i;
    }

    return 0;
}