#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include <signal.h>

#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>


//Ex. 3 parte prática exame ep. normal 2022/2023
//Implementação um bocado à "padeiro". Pfv. alterem/adaptem e se conseguirem melhor solução, pfv partilhem.

struct nota{
    char c;
    int num;
};




int running;
int acordes;
pid_t insts[3];

pthread_mutex_t m_running;

void sair(){
    printf("\n\n%d notas/acordes.\n\n", acordes);
    pthread_mutex_lock(&m_running);
    running=0;

    for(int i=0; i<3;i++){
        kill(insts[i],SIGINT);
    }
    pthread_mutex_unlock(&m_running);
    exit(0);
}

void *inst(void * i){
    int  ins =  (int *)i;
    
    pid_t p;
    int pf[2];
    char buff[3];
    char rec[5];
    char c;

    switch (ins)
    {
    case 0:
        c='p';
        break;

    case 1:
        c='g';
        break;

    case 2:
        c='b';
        break;
    
    default:
        printf("NENHUM INSTRUMENTO!!!");
        break;
    }


    if (pipe(pf) != 0)
    {
        fprintf(stderr, "Pipe Failed");
        return NULL;
    }
    
    p = fork();
    
    if(p<0){
        printf("FORK FALHOU!\n\n");
        return NULL;
    }

    if(p==0){
        close(pf[0]); //FECHA A PARTE DE LEITURA DO PIPE
        close(1); //FECHA O STDOUT
        dup(pf[1]); //PARA COLOCAR A PARTE DE ESCRITA NO MESMO
        dup2(pf[1],1);
        close(pf[1]); //E FECHA O DUPLICADO
        execl("./inst", "./inst", (char *)NULL); 
        printf("PROGRAMA NAO ENCONTRADO!\n"); //NAO ENCONTROU O PROGRAMA
        exit(-1);
    }

    
    if(p>0){ 
        insts[ins]=p;
        close(pf[1]);

        int ret;
        pthread_mutex_lock(&m_running);
        while(running==1){

            if(read(pf[0], buff, sizeof(buff))>0){ 
            pthread_mutex_unlock(&m_running);
                printf("%c%s ",c, buff);
                fflush(stdout);
                acordes++;
            }else{
               break;
            }
        }
    }
    
}


void *KB(){
    //Nada de muito interessante: literalmente lê cenas do teclado e vê o que se faz com elas

    char buff[1024];
    int l_timer;
    pthread_mutex_lock(&m_running);
    while(running==1){
        pthread_mutex_unlock(&m_running);
        printf("Texto: ");
        scanf("%[^\n]", buff);
        while (getchar() != '\n')
            continue;
        if(!strcmp(buff,"sair")){
            printf("Adeus\n");
            sair();
            break;
        }
        setbuf(stdout, NULL);
    }
    return;
}


int main(int argc, char *argv[]){
    int seg;
    if(argc!=2)
        seg=10;
    else
        seg=atoi(argv[1]);

    pthread_mutex_init(&m_running, NULL);


    running=1;
    acordes=0;
    pthread_t thrInst[3], thrKB; 
	int i;
    signal(SIGALRM, &sair);
    alarm(seg);
    pthread_create( &thrKB, NULL, KB, NULL);
    for(i=0; i<3;i++){
        pthread_create( &thrInst[i], NULL, inst, (void *) i);
        
    }
    //iniciar as threads	
    printf("THR CRIADAS!");
    wait(NULL);
    //e receciona-las quando acabarem
    pthread_join(&thrKB, NULL);
    printf("\nTHR KB MORREU\n");
     for(i=0; i<3;i++){
        pthread_join( &thrInst[i], NULL);
        
    }
    printf("XAU!!!!");

}