#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int getRandomIndex(int min, int max)
{
    return rand() % (max - min + 1) + min;;
}

void passwordGenerator(int length, char *buffer){
    for(int i = 0; i<length; i++){
        buffer[i] = (char)getRandomIndex(33,126);

    }
    buffer[length] = '\0';
}

int main()
{
    srand(time(NULL));
    int length = 12;
    char password[length+1];
    passwordGenerator(length,password);
    printf("Password: %s\n", password);
    return 0;
}