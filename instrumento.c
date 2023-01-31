#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

void sair(){
    //printf("Bye!");
    exit(0);
}

int main(){
    int i, n;
    time_t t;
    char c;
    n = 5;

    /* Intializes random number generator */
    srand((unsigned) time(&t));
    signal(SIGINT, &sair);
    /* Print 5 random numbers from 0 to 49 */
    while(1) {
        int num;
        char cena[3];
        srand((unsigned) time(&t));
        c='A' + (random() % 7);
        srand((unsigned) time(&t));
        num =  1+ rand() % 9;
        sprintf(cena,"%c%d",c,num);
        printf("%s",cena);
        fflush(stdout);
        sleep(2);
    }

    return(0);

    
}