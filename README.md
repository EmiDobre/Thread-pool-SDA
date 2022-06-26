DOBRE EMILIA ILIANA 315CB

MAIN:
    -am creat stiva de threaduri care urmeaza sa ia din coada waiting tasks prin functia afla_C
    -la introducerea comenzii add task se creeaza coada de waiting ordonata corespunzator cu id-urile care cresc si sunt diferite, astfel modific la fiecare apel id ul curent
    -in cazul in care s-a dat deja comanda run si timpul introdus este mai mare decat Q cuanta de timp si numarul de threaduri > nr_tasks din waiting, atunci cand se va apela functia de run, toata coada running se goleste si trece in finished astfel stim ca exista taskuri cu id disponibil => se verifica daca coada waiting este goala pt a se restarta id-ul curent



GET_TASK:
    -apeleaza functia de cautare intr-o coada data tinand cont daca s-a gasit deja elementul cu acel task sau nu in waiting/running si ultima data finished
    -functia de cautare cauta dupa id si returneaza elementul gasit prima data cu acel id(caz care se intampla doar pentru coada finished unde pot exista mai multe procese care s-au terminat si au acelasi id acum)



GET_THREAD:
    -functia de Cauta_Idle verifica daca exista elementul in stiva
    -daca exista se verifica daca ruleaza sau nu un task printr-o functie care Cauta dupa thread in coada running daca nu e goala



COADA WAITING ORDONATA - INSERARE ORDONATA:
    -creez coada aux formata din nr_tasks elemente de prioritati identice, aceasta fiind coada de inserat
    -aceasta coada aux se va concatena dupa inceputul si inaintea finalului cozii waiting (cozi pe care le creez prin scoaterea elementelor din coada pana cand gasesc un element cu prioritate mai mare)
    -se verifica apoi daca dupa crearea cozii, un elem din aux(fiind toate identice) are prioritatea identica cu ultimul element de la coada de inceput
    -acest lucru functioneaza pentru ca mereu se insereaza ordonat si in prima parte a cozii sunt elem cu prioritatile cele mai mari
    -daca elem are acc prior cu ultimul din coada de inceput este clar ca trebuie decis daca se introduce dupa inceput sau devine noul inceput
    
    =>FUNCTIA de ordonare dupa timp:
        -schimba inceputul cozii
        -in coada inceput(coada) am elem cu prior mai mare decat aux, apoi nist elem cu acc prior sau doar elem cu acc prior
        -iau din coada inceput:un nou inceput pana in elem cu prior egala cu aux
        -apoi am o coada "idem_aux" cu acele elem cu acc prior
        -compar si verifc care din cozile "idem_aux" si aux se pune primul
        -functia modifica inceputul cu auxul primit, adica coada
        -coada stiu ca nu e nula si are elem cu acc prior ca aux

    -astfel se decide ordinea de conactenare cu finalul si daca se ordoneaza sau nu dupa timp


RUN:
    -am tratat cazul in care Q <= timp introdus ca parametru si nr_elem_stiva*2 are mai multe elemente decat coada waiting pentru ca astfel am certitudinea ca toate taskurile din running se duc direct in finished, toate taskurile terminandu-si executia in paralel datorita numarului mare de threaduri(2*nr_elem_stiva = 2*C ) 
    -se initializeaza campul de threaduri pt capacitatea 2*C de elem care se introduc in coada running 
    -pentru cazul abordat, toata coada finished este running care se goleste, astfel am disponibile threaduri si id-uri

    =>FUNCTIA de Disponibilitate baga threadurile inapoi in stiva si retine id-urile disponibile (aceasta functie ar fi avut importanta pe cazul netratat din pacate :( )
    
    -in utlima faza se ordoneaza coada finsihed crescator dupa timpii de executie

    =>FUNCTIA de OrdonareFinish:

        -aflu numarul de timpi diferiti pentru a sparge coada de finish in mai multe cozi cu timpi identici de executie
        -concatenez in finish ordonat cele n cozi dupa timp
        -vectorul ajutator ordine retine ordinea cozilor in finish_ord => ord[poz] = numarul cozii de pe pozitia poz in finished concatenat ordonat
        -pentru n iteratii (cati timpi diferitie/cozi am) apelez:

            =>FUNCTIA de MinimCozi:

            -modifica vectorul de ordine si compara ce ramane din cozile neordonate
            -se afla astfel fiecare coada pe ce ordine trebuie sa fie si se sare peste cozile care sunt nule intrucat dupa fiecare apel se concateneaza coada gasita ca minma in functia de minim

        -in final, coada ramasa ultima, care nu are cu cine sa fie comparata este doar introdusa la final


FINISH:
    -timpul total de executie este calculat dupa cazul particular discutat si la functia RUN
    -in momentul in care se da run se apeleaza inainte de modificare functiei waiting, functia Timp_Total
    -timpul total este suma timpului maxim din fiecare coada waiting existenta nenula la introducerea comenzii run, intrucat daca se introduce run max se termina toate taskurile din prima (lucru indeplinit de conditia in care calculez timpul: numarul de threaduri > waiting tasks si Q>exe)


Punctajul obtinut local: 110/135.
