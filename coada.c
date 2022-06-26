//DOBRE EMILIA ILIANA 315CB

#include "fct.h"
void* InitC(size_t d) {
    TCoada* coada;
    coada = (TCoada*)malloc(sizeof(TCoada));
    if( !coada ) return NULL;
    coada->dime = d;
    coada->ic =  NULL;
    coada->sf = NULL;
    return (void*)coada;
}

int IntroducC(void* c, void* el) {
    TLG aux = (TLG)calloc(1, sizeof(TCel));
    if ( !aux ) return 0;
    aux->info = calloc(1, DIME(c));
    memcpy(aux->info, el, DIMEC(c));

    //daca coada este vida:
    if ( SF(c) ==  NULL && IC(c) == NULL ) {
        SF(c) = aux;
        IC(c) = aux;
        return 1;
    } 
    SF(c)->urm = aux;
    SF(c) = aux;
    return 1; 
}

int ExtragC(void* coada, void* el) {
    //daca coada este vida
    if ( IC(coada) == NULL && SF(coada) == NULL ) 
        return 0;
    
    //daca nu e vida
    TLG aux = IC(coada);
    IC(coada) = aux->urm;
    memcpy(el, aux->info, DIMEC(coada)); //extrag din coada inceputul
    free(aux->info);
    free(aux);
    return 1;
}

int ConcatC(void* dest, void*sursa) {
    if ( IC(dest) == NULL || SF(dest) == NULL ) {//dest e vida
        IC(dest) = IC(sursa);
        SF(dest) = SF(sursa);
        IC(sursa) = NULL;
        SF(sursa) = NULL;  
        return 1;
    }
    if ( IC(sursa) == NULL &&  SF(sursa) == NULL )  //sursa e vida
        return 0;
    SF(dest)->urm = IC(sursa);
    SF(dest) = SF(sursa);
    IC(sursa) = NULL;
    SF(sursa) = NULL; 
    return 1;
}

int NrElemCoada( void* coada ) {
    int nr = 0;
    Task el;
    void* aux = InitC(sizeof(Task));
    while ( IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        IntroducC(aux, &el);
        nr++;
    }
    ConcatC(coada, aux);
    return nr;
}


void AfisareCoada(void* coada, AFI afi_el, FILE* fout, char tip) {
    void* aux = InitC(sizeof(Task));
    fprintf(fout,"[");
    if ( IC(coada) == NULL ) {
        fprintf(fout,"]\n");
        return;
    }
    Task el;
    int nr_elem = NrElemCoada(coada);
    int iteratii = 0;
    while ( IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        IntroducC(aux, &el);
        afi_el(&el, fout, tip);
        if ( iteratii < nr_elem - 1 )
            fprintf(fout, ",\n");
        iteratii++;
    }
    fprintf(fout,"]\n");
    ConcatC(coada, aux);
}


//fct program:

int AfiseazaTask(void* el, FILE* fout, char tip) {
    Task* task = (Task*)el;
    fprintf(fout,"(%d: ", task->id);
    fprintf(fout,"priority = %d, ", task->prior);
    if ( tip == 'w' )
        fprintf(fout,"remaining_time = %d)", task->time);
    if ( tip == 'r' ) {
        fprintf(fout,"remaining_time = %d, ", task->time);
        fprintf(fout,"running_thread = %d)", task->thread);
    }
    if ( tip == 'f' )
        fprintf(fout,"executed_time = %d)", task->time);
}


//cauta in coada data elementul dupa id=> 0/1 gasit

Task CautaTask(void* coada, int id ) {
    void* aux =  InitC(sizeof(Task));
    Task el, el_ret = {0, 0, 0, 0};
    int primul_gasit = 0;
    while(  IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        IntroducC(aux, &el);
        if ( el.id == id && primul_gasit == 0 ) {
            el_ret.id = el.id;
            el_ret.prior = el.prior;
            el_ret.thread = el.thread;
            el_ret.time = el.time;
            primul_gasit = 1;
        }
    }
    ConcatC(coada, aux);
    free(aux);
    return el_ret;
}

Task CautaTaskThread(void* coada, int thread ) {
    void* aux =  InitC(sizeof(Task));
    Task el, el_ret = {0, 0, 0, 0};
    int primul_gasit = 0;
    while(  IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        IntroducC(aux, &el);
        if ( el.thread == thread && primul_gasit == 0 ) {
            el_ret.id = el.id;
            el_ret.prior = el.prior;
            el_ret.thread = el.thread;
            el_ret.time = el.time;
            primul_gasit = 1;
        }
    }
    ConcatC(coada, aux);
    free(aux);
    return el_ret;
}


//COADA WAITING ORDONATA
void Ins_ORDWaiting(int nr_tasks, int time, int prior, void* waiting, int* id_crt) {
    int i = 1;
    Task el;
    void* aux = InitC(sizeof(Task));
    for ( i = 1; i <= nr_tasks; i++ ) {
        el.thread = -1;
        el.id = (*id_crt)+i;
        el.time = time;
        el.prior = prior;
        IntroducC(aux, &el);
    }

    //scot elem din coada pana gasesc un elem cu prior mai mare
    Task elem;
    void* copie = InitC(sizeof(Task));
    void* inceput = InitC(sizeof(Task));
    void* final = InitC(sizeof(Task));
    if ( SF(waiting) == NULL && IC(waiting) == NULL ) {
        //concatenez direct aux in waiting
        ConcatC(waiting, aux);
    } else {

        while ( IC(waiting) != NULL && SF(waiting) != NULL ) {
                ExtragC(waiting, &elem);
                if ( elem.prior < el.prior ) {
                    IntroducC(final, &elem);
                    break;
                }
                IntroducC(inceput, &elem);
        }
        while ( IC(waiting) != NULL && SF(waiting) != NULL ) {
            ExtragC(waiting, &elem);
            IntroducC(final, &elem);
        }

        //inceput di fde nul=>inserez la mijloc sau la sf/
        //vf daca aux are prior egala cu ultimul elem din coada de inceput=> le compar dupa timp
        
        if ( IC(inceput) != NULL && SF(inceput) != NULL ) {

            if ( ((Task*)SF(inceput)->info)->prior == ((Task*)SF(aux)->info)->prior ) {
                //ord dupa timp
                OrdoneazaDupaTimp(inceput, aux);
            }           
                ConcatC(waiting, inceput);
                ConcatC(waiting, aux);
                if ( IC(final) != NULL && SF(final) != NULL ) {
                    ConcatC(waiting,final);
                }
        }

        if ( IC(inceput) == NULL ) {
            ConcatC(waiting, aux);
            ConcatC(waiting,final);
        }
    }
}



void OrdoneazaDupaTimp( void* coada, void* aux ) {
    void* inceput = InitC(sizeof(Task));
    void* idem_aux = InitC(sizeof(Task));
    Task el;

    
    while( IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        if ( el.prior > ((Task*)SF(aux)->info)->prior ) {
            IntroducC(inceput, &el);
        } else {
            IntroducC(idem_aux, &el);
        }
    }

    //am inceputul cu elem strict mai mari la prior ca aux
    while( IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
      IntroducC(idem_aux, &el);
    }
    //am coada cu elem cu prior identica ca si aux
    if ( el.time > ((Task*)SF(aux)->info)->time ) {
        ConcatC(coada,inceput);
        ConcatC(coada, aux);
        ConcatC(coada, idem_aux);    
    } else {
        ConcatC(coada,inceput);
        ConcatC(coada, idem_aux);
        ConcatC(coada, aux);
    }
}

//FUNCTIA DE RUN:

void run( void* waiting, void* running, void* finished, int** id_disp, void* stiva, int C, int Q, int exe, int* nr_disp, void* finish_ord, int run_check) {
    int nr_elem = 0;
    //pun threadurile in campurile cozii wtg
    void* aux_w = InitC(sizeof(Task));
    Task el; int thread, iteratii, i = 0;
    nr_elem = NrElemCoada(waiting);

    //initializez campul de threaduri pt cate elem se pot baga in runnng
    //vf daca stiva are mai multe elm decat coada la mom actual

    if ( nr_elem <= 2*C ) 
        iteratii = nr_elem; 
    else 
        iteratii = 2*C;

    while ( i < iteratii) {
        ExtragC(waiting, &el);
        Pop(stiva, &thread);
        el.thread = thread;
        IntroducC(running, &el);
        i++;
    }
    //compar Q cu timpul de executat si obs cate elem din running se duc in finished
    if ( Q <= exe && nr_elem <= 2*C ) {
        ConcatC(finished, running);
        (*nr_disp) = Disponibil(finished, stiva, id_disp);
        
    } 
    //altfel scad din running/retin elem cu id uri disp prin vectorul de dispo
    // si elimin----de facut...

    if ( IC(finished) != NULL && SF(finished) != NULL ) 
        OrdoneazaFinish(finished, finish_ord);
}


//fct care baga threadurile inapoi in stiva
//face id urile disponibile

int Disponibil( void* coada, void* stiva, int** id_disp) {
    void* aux = InitC(sizeof(Task));
    Task el; int i = 0;
    while ( IC(coada) != NULL && SF(coada) != NULL ) {
        ExtragC(coada, &el);
        IntroducC(aux, &el);
        Push(stiva, &(el.thread));
        *( *id_disp + i) = el.id;
        i++;
    }
    ConcatC(coada, aux);
    return i;
}

void OrdoneazaFinish(void* finish, void* finish_ord) {
    void* aux = InitC(sizeof(Task));
    int min = 0;
    Task el, el_ant; int nr = 1;

    //aflu nr de cozi cu timpi dif
    ExtragC(finish, &el_ant);    
    IntroducC(aux, &el_ant);
    while ( IC(finish) != NULL && SF(finish) != NULL ) {
        ExtragC(finish, &el);
        if ( el_ant.time != el.time )
            nr++;
        IntroducC(aux, &el);
        el_ant = el;
    }
    ConcatC(finish, aux);
    
    //creez cozile cu timpi identici
    void** cozi = calloc(nr, sizeof(TCoada));
    int i = 0;
    for (; i < nr; i++ )
        cozi[i] = InitC(sizeof(Task));

    i = 0;
    ExtragC(finish, &el_ant);    
    IntroducC(aux, &el_ant);
    IntroducC(cozi[i], &el_ant);
    while ( IC(finish) != NULL && SF(finish) != NULL ) {
        ExtragC(finish, &el);
        if ( el_ant.time == el.time ) {
            IntroducC(cozi[i], &el);
        } else {
            i++;
            IntroducC(cozi[i], &el);
        }
        IntroducC(aux, &el);
        el_ant = el;
    }
    ConcatC(finish, aux);
    
    //concatenez in finish ordonat cele n cozi de i dupa timp
    int* ordine = calloc(nr, sizeof(int));
    int poz = 0;

    for ( poz = 0; poz < nr - 1; poz++ ) {
        MinimCozi(cozi, &ordine, nr, poz);
        ConcatC(aux, cozi[ordine[poz]]);
    }

    //concatenez ultima coada ramasa- cea mai mare resul fiind nule acum
    for ( poz = 0; poz < nr - 1; poz++ ) {
        if ( IC(cozi[poz]) != NULL && SF(cozi[poz]) != NULL ) {
            ConcatC(aux, cozi[poz]);
        }
    }
    if ( nr == 1 ) {
        ConcatC(finish_ord, finish);
    } else {
        ConcatC(finish_ord, aux);
        ConcatC(aux, finish);
    }

}

void MinimCozi(void** cozi, int** ordine, int nr, int poz) {
    int min = 0, i = 0;
    min = ((Task*)IC(cozi[i])->info)->time;
    for ( i = 0; i < nr; i++ ) {
        if ( IC(cozi[i]) != NULL && SF(cozi[i]) != NULL) {
            if ( ((Task*)IC(cozi[i])->info)->time < min ) {
                min = ((Task*)IC(cozi[i])->info)->time;
                *(*ordine+poz) = i;
            }
        }
    }
}

void Timp_Total(int* total_timp, void* waiting ) {
    if ( IC(waiting) == NULL && SF(waiting) == NULL ) 
        return;

    if ( IC(waiting) != NULL ) {
        void* aux = InitC(sizeof(Task));
        Task el, max;
        ExtragC(waiting, &max);
        IntroducC(aux, &max);
        while ( IC(waiting) != NULL && SF(waiting) != NULL ) {
            ExtragC(waiting, &el);
            IntroducC(aux, &el);
            if ( el.time > max.time ) 
                max = el;
        }
        ConcatC(waiting, aux);
        (*total_timp) = (*total_timp) + max.time;
    }
}
