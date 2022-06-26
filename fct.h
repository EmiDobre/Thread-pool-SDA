//DOBRE EMILIA ILIANA 315CB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct cel
{
    struct cel *urm;
    void* info;  
}TCel, *TLG;

typedef int Thread;
typedef struct {
    unsigned short int id;
    Thread thread;
    unsigned char prior;
    unsigned int time;
}Task;


//stiva:
typedef struct stiva
{
    size_t dime;          //dim element
    TLG vf;              //adresa celula varf, celula care este TLG
}TStiva;

#define DIME(a) (((TStiva*)(a))->dime)
#define VF(a) 	(((TStiva*)(a))->vf)

typedef int (*TF)(void* el);
typedef int (*AFI)(void* el, FILE* f, char c);
void* InitS(size_t d);
int Push (void* s, void* el);
int Pop(void* s, void* el);
int Rastoarna(void* dest, void* sursa);
void Suprapune(void* dest, void* sursa);

void AfisareSt(void*s, TF afi_el);
int afi_thread (void* el);
void CreateStiva ( int C, void* stiva);
int Cauta_Idle(void* stiva, int id);

//coada:
typedef struct {
    size_t dime;
    TLG ic, sf;
}TCoada, *ACoada;


#define DIMEC(a) (((ACoada)(a))->dime)
#define IC(a) 	(((ACoada)(a))->ic)
#define SF(a) 	(((ACoada)(a))->sf)

void* InitC(size_t d);
int IntroducC(void* c, void* el);
int ExtragC(void* coada, void* el);
int ConcatC(void* dest, void* sursa);
int NrElemCoada( void* coada );
void AfisareCoada(void* coada, AFI afi_el, FILE* fout, char tip);

int AfiseazaTask(void* el, FILE* fout, char tip);
void Add_Coada_Waiting(int nr_tasks, int time, int prior, void* coada, int* id_crt, int** prv, void* copie );
void* CreateWaiting( void* aux, int id_crt, int nr_task);
Task CautaTask(void* coada, int id );
Task CautaTaskThread(void* coada, int thread );
int InsOrd(void* waiting, Task new);
void Ins_ORDWaiting(int nr_tasks, int time, int prior, void* coada, int* id_crt);
void OrdoneazaDupaTimp( void* coada, void* aux );
void run(void* waiting, void* running, void* finished, int** id_disp, void* stiva, 
        int C, int Q, int exe, int* nr_disp, void* finished_ord, int run_check);
int Disponibil( void* coada, void* stiva, int** id_disp);
void OrdoneazaFinish(void* finish, void* finish_ord);
void MinimCozi(void** cozi, int** ordine, int nr, int poz);
void Timp_Total(int* total_timp, void* waiting );


//main
int afla_Q(int Q, char* input);
int afla_C (void* stiva, int C, char* input);
void get_task(void* waiting, void* running, void* finish_ord, FILE* fout, char* input);
void print(void* waiting, void* running, void* finish_ord, char* input, FILE* fout);
