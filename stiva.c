//DOBRE EMILIA ILIANA 315CB

#include "fct.h"

void* InitS(size_t d) {
    TStiva* s;
    s = (TStiva*)calloc(1, sizeof(TStiva));
    if(!s) return NULL;
    s->dime = d;
    s->vf = NULL;
    return (void*)s;
}

int Push (void* s, void* el) {
    //aloc celula TLG

    TLG aux = (TLG)calloc(1, sizeof(TCel));
    if ( !aux ) return 0;
    aux->info = calloc(1, DIME(s));
    if ( !aux->info ) return 0;

    //copiez elem

    memcpy(aux->info, el, DIME(s));
    aux->urm = VF(s);
    VF(s) = aux;

    return 1;
}

void AfisareSt(void*s, TF afi_el) {
    TLG lista = NULL;
    printf("----Stiva:\n");
    for ( lista = VF(s); lista !=  NULL; lista = lista->urm ) {
        afi_el(lista->info);
    }
    printf("\n");
}

int Pop(void* s, void* el) {    //pune in el varful stivei/ extrag
    if ( VF(s) == NULL ) return 0;
    memcpy(el, VF(s)->info, DIME(s));
    TLG aux = VF(s);
    VF(s) = aux->urm;
    free(aux->info);
    free(aux);
    return 1;
}

//copiaza el stivei sursa l astiva dest
int Rastoarna(void* dest, void* sursa) {
    //daca sursa e vida

    if ( VF(sursa) == NULL ) 
        return 0;

    //daca dest e vida copiez varful

        VF(dest) = VF(sursa);
        VF(sursa) = NULL;
    return 1;
}

//copie stiva de inturi pt sursa
void Suprapune(void* aux, void* sursa) {
    int el;
    Rastoarna(aux, sursa);
    while ( VF(aux) != NULL ) {
        Pop(aux, &el);
        Push(sursa, &el);
    }
}

int afi_thread (void* el) {
    if ( el == NULL ) return 0;
    printf("el: %d\n", *(int*)el);
    return 1;
}

void CreateStiva ( int C, void* stiva) {
    int N = 2 * C;
    for ( int i = N-1; i >= 0; i-- ) {
        Push(stiva, &i);
    }
}

int Cauta_Idle(void* stiva, int id) {
    void* aux = InitS(sizeof(int));
    int el, find = 0;
    while ( VF(stiva) != NULL ) {
        Pop(stiva, &el);
        Push(aux, &el);
        if ( el == id ) 
            find = 1;
    }
    Suprapune(aux, stiva);
    if ( find == 1 )
        return 1;
    else 
        return 0;
}