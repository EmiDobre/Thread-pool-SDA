//DOBRE EMILIA ILIANA 315CB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fct.h"

void imparteCuv(char* sir, char* el, char c, int* poz ) {
	int i = 0;
	while ( *(sir+i+*poz) != c ) i++;
	memcpy(el, sir+*poz, i);
	*poz = *poz+i+1;	
}


int main(int argc, char** argv) {
    FILE* f = fopen(argv[1], "rt");
    FILE* fout = fopen(argv[2], "wt");

    char input[1000];
    int i = 0, poz = 0, Q = 0, C = 0, id_crt = 0;
    void* stiva = InitS(sizeof(Thread));
    void* aux_waiting = InitC(sizeof(Task));
    void* copie_aw = InitC(sizeof(Task));
    void* waiting = InitC(sizeof(Task));
    void* running = InitC(sizeof(Task));
    void* finished = InitC(sizeof(Task));
    void* finish_ord = InitC(sizeof(Task));

    int* id_disp = calloc(32767, sizeof(int)); //vect care retine id urille disp
    int nr_disp = 0; //nr de elem din vect de elem disp 
    int run_check = 0;
    int exe = 0;
    int total_time = 0;

    while ( fgets(input, 1000, f) != NULL ) {
        
        if ( i == 0)
            Q = afla_Q(Q, input);

        if ( i == 1) 
            C = afla_C(stiva, C, input);
            

        if ( input[0] == 'a' ) {
            poz = 0;
            char* tasks = calloc(7, sizeof(char));
            imparteCuv(input+10, tasks, ' ', &poz);
            int nr_tasks = atoi(tasks);

            char* time_exe = calloc(11, sizeof(char));
            imparteCuv(input+10, time_exe, ' ', &poz);
            int time = atoi(time_exe);

            char* prioritate = calloc(4, sizeof(char));
            imparteCuv(input+10, prioritate, '\n', &poz);
            int prior = atoi(prioritate);
            int contor;

            //daca coada finish nu are elem
            if ( run_check == 0) {
                for ( contor = 0; contor < nr_tasks; contor++ ) 
                    fprintf(fout,"Task created successfully : ID %d.\n", contor+id_crt+1);
                Ins_ORDWaiting(nr_tasks, time, prior, waiting, &id_crt);
                id_crt = nr_tasks + id_crt;
                
            }
            //cand dupa run se goleste tot waitingul=> cand am dat run si exe>q
            if ( run_check == 1 ) {
                if ( Q <= exe ) {
                    if ( IC(waiting) == NULL ) {
                        //printf("Coada waiting s- a golit adaug acum %d elem de la id 1\n", nr_tasks);
                        id_crt = 0; 
                    }

                    for ( contor = 0; contor < nr_tasks; contor++ ) 
                        fprintf(fout,"Task created successfully : ID %d.\n", contor+id_crt+1);
                    Ins_ORDWaiting(nr_tasks, time, prior, waiting, &id_crt);
                    id_crt = nr_tasks + id_crt;
                    //printf("De la %d o sa am noi id uri\n", id_crt);
                    
                }
            }
            free(tasks); free(time_exe); free(prioritate);
        }
        

        if ( strncmp(input, "get_task", 8) == 0)
            get_task(waiting, running, finish_ord, fout, input);
            


        if ( strncmp(input, "get_thread", 10) == 0 ) {
            char* id_thread = calloc(10, sizeof(char));
            poz = 0;
            imparteCuv(input+11, id_thread, '\n', &poz);
            int id = atoi(id_thread);
            if ( Cauta_Idle(stiva, id) == 1 ) {
                if ( IC(running) != NULL && SF(running) != NULL ) {
                    Task el = CautaTaskThread(running, id);
                    fprintf(fout, "Thread %d is running task %d (remaining_time = %d).",id, el.id, el.time);
                }
                if ( IC(running) == NULL )
                    fprintf(fout, "Thread %d is idle.\n", id);
            }
            free(id_thread);
        }

        if ( input[0] == 'p' )
            print(waiting, running, finish_ord, input, fout);


        if ( input[0] == 'r' ) {
            char* run_for = calloc(10, sizeof(char));
            poz = 0;
            imparteCuv(input+4, run_for, '\n', &poz);
            exe = atoi(run_for);
            fprintf(fout, "Running tasks for %d ms...\n", exe);
            free(run_for);

            ///aflu maxim de timp si il adaug inainte sa fac run efectiv;pt Q<exe
            if ( Q <= exe && NrElemCoada(waiting) <= 2*C ) 
                Timp_Total(&total_time, waiting);
            run_check = 1;

            run(waiting, running, finished, &id_disp, stiva, C, Q, exe, &nr_disp, finish_ord, run_check);
        }

        if ( strncmp(input, "finish", 5) == 0 ) {
            fprintf(fout,"Total time: %d", total_time);
        }

        i++;
    }
}

int afla_Q(int Q, char* input) {
    char nr[6];
    int poz = 0; int j = 0;
    while ( input[j] != '\n' ) {
        nr[j] = input[j];
        j++;
    }
    nr[j] = '\0';
    Q = atoi(nr);
    return Q;
}

int afla_C (void* stiva, int C, char* input ) {
    char nr[3];
    int j = 0;
    while ( input[j] != '\n' ) {
        nr[j] = input[j];
        j++;
    }
    nr[j] = '\0';
    C = atoi(nr);
    CreateStiva(C, stiva);
    return C;
}



void get_task(void* waiting, void* running, void* finish_ord, FILE* fout, char* input) {
    char* id_task =  calloc(6, sizeof(char));
    int poz = 0;
    imparteCuv(input+9, id_task, '\n', &poz);
    int id = atoi(id_task);   
    CautaTask(waiting, id);
    Task el = CautaTask(waiting, id);

    if ( el.id == 0 ) { //nu e in waiting
        el = CautaTask(running, id); 
        if ( el.id == 0 ) { //nu e nici in running
            el = CautaTask(finish_ord, id);
            if ( el.id == 0 ) 
                fprintf(fout, "Task %d not found.\n", id);
                else
                    fprintf(fout, "Task %d is finished (executed_time = %d).\n", el.id, el.time);
            } else {
                fprintf(fout, "Task %d is running (remaining_time = %d).\n", el.id, el.time);    
            }      
        
        } else {
            fprintf(fout, "Task %d is waiting (remaining_time = %d).\n", el.id, el.time);
        }
    free(id_task);
}

void print(void* waiting, void* running, void* finish_ord, char* input, FILE* fout) {
    if ( input[6] == 'w' ) {
        fprintf(fout,"====== Waiting queue =======\n");
        AfisareCoada(waiting, AfiseazaTask, fout, 'w');
    }

    if ( input[6] == 'r' ) {
        fprintf(fout,"====== Running in parallel =======\n");
        AfisareCoada(running, AfiseazaTask, fout, 'r');
    }

    if ( input[6] == 'f' ) {
        fprintf(fout,"====== Finished queue =======\n");
        AfisareCoada(finish_ord, AfiseazaTask, fout, 'f');
    }
}