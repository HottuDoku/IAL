
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    // Nastavíme first, last a active na NULL, protože zatím nikam neukazují
    list->firstElement  = NULL;
    list->lastElement   = NULL;
    list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    // Potřeba, protože do malloc nelze dávat pointer a trocha čarování a logistiky...
    DLLElementPtr cell = list->firstElement;

    // Cyklus dokud neexistuje první prvek... dokud není seznam prázdný
    while (list->firstElement != NULL){
        // 'firstElement' se přepíše na následující
        list->firstElement = list->firstElement->nextElement;
        // Uvolnění
        free(cell);
        // Logistika... cell je 'firstElement' ^^^, takže následující, fuj
        cell = list->firstElement;
    }

    // Nastavím first, last a active na NULL, protože nemají kam ukazovat
    list->firstElement  = NULL;
    list->activeElement = NULL;
    list->lastElement   = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *c:292)
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    //  Alokuje pamět o velikosti 'struktura DLLElement' do proměnné cell
    DLLElementPtr cell = malloc(sizeof(struct DLLElement));

    // Pokud malloc vráti NULL pointer, funkce zavolá error, jinak...
    if (cell != NULL){
        /**           ______ __________ ______
        * CELL ==>  | Prev |   DATA   | Next | 
        * Ukazatel na předchozí prvek zatím nastavíme ne NULL
        */
        cell->previousElement = NULL;
        // Rovnou do buňky přiřadíme data
        cell->data = data;
        // Další prvek nastavíme na předchozí 'první' prvek
        cell->nextElement = list->firstElement;
    }else{
        DLL_Error();
        return;
    }
    
    // Pokud předchozí 'první' je NULL == seznam je prázdný
    // tak poslední prvek je právě přidávaný
    if (list->firstElement == NULL){
        list->lastElement = cell;

    /** Pokud předchozí 'první' prvek není null == seznam prázdný není
     *  tak 'previous' posledního prvku bude ukazovat na právě přidávaný'
     *             Právě přidávaný                  Existující
     *         ______ __________ ______          ________ __________ ______
    CELL ==>  | Prev |   DATA   | Next | <=  <= |<= Prev |   DATA   | Next |*/ 
    }else{
        list->firstElement->previousElement = cell;
    }
    // Nově přidaná buňka bude vždy 'první prvek'
    list->firstElement = cell;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    // Alokace a error v případě chyby, stejné jako v DLL_InsertFirst, jinak...
    DLLElementPtr cell = malloc(sizeof(struct DLLElement));
    if (cell != NULL){
        // Přesně naopak, protože přidáváme prvek z druhé strany
        cell->previousElement = list->lastElement;
        cell->data = data;
        cell->nextElement = NULL;
    }else{
        DLL_Error();
        return;
    }
    // Opět přesně naopak, pokud je seznam prázdný
    // tak ukazatel na první je právě přidávaný prvek
    if (list->lastElement == NULL){
        list->firstElement = cell;

    // Pokud seznam prázdný není, tak next posledně přidaného prvnu bude ukazovat
    // na přávě přidávaný
    }else{ 
        list->lastElement->nextElement = cell;
    }
    // Nově přidaná buňka bude vždy 'poslední prvek'
    list->lastElement = cell;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    //          Aktivní = První
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    //          Aktivní = Poslední
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    // Pokud neexistuje první prvek, seznam je prázdný, jinak...
    if (list->firstElement != NULL){
        // Uloží do *dataPtr data z prvního prvku
        *dataPtr = list->firstElement->data;
    }else{
        DLL_Error();
        return;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    // Opak DLL_GetFirst, jinak...
    if (list->lastElement != NULL){
        // Uloží do *dataPtr data z posledního prvku
        *dataPtr = list->lastElement->data;    
    }else{
        DLL_Error();
        return;
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    // Uložíme si první prvek
    DLLElementPtr first_cell = list->firstElement;

    // Pokud je seznam prázdný, funkce se ukončí
    if (list->firstElement == NULL || list->lastElement == NULL){
        return;
    }

    // Pokud je první prvek aktivní, aktivita se ztratí
    if (list->firstElement == list->activeElement){
        list->activeElement = NULL;
    }

    // V seznamu je pouze jeden prvek => musíme nastavit last na NULL
    if (list->firstElement == list->lastElement){
        list->lastElement = NULL;

    }else{
        /** V seznamu je více prvků  
        *   Musíme nastavit 'předchozí' prvek následujícího prvku na NULL
        *        Tento byl first               Tento se stane first
        *  ______ __________ ______    x  x  ______ __________ ______
        * | Prev |   DATA   | Next |  <= <= | Prev |   DATA   | Next |*/ 
        list->firstElement->nextElement->previousElement = NULL;
    }

    // Teď nastavíme jako 'první' prvek následující a nakonec uvolníme místo z mazaného prvku
    list->firstElement = first_cell->nextElement;
    free(first_cell);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    // Uložíme si poslední prvek
    DLLElementPtr last_cell = list->lastElement;

    // Pokud je seznam prázdný funkce se ukončí
    if (list->lastElement == NULL || list->firstElement == NULL){
        return;
    }

    // Pokud je první prvek aktivní, aktivita se ztratí
    if (list->lastElement == list->activeElement){
        list->activeElement = NULL;
    }

    // Opak DLL_DeleteFirst
    // Musíme nastavit 'následující' prvek předchozího prvku na NULL 
    // nebo předposlední prvek nastavit na 'last'
    if (list->lastElement == list->firstElement){
        list->firstElement = NULL;

    }else{
        list->lastElement->previousElement->nextElement = NULL;
    }

    // Nastavíme jako 'poslední' prvek přecházející a uvolníme místo mazaného prvku
    list->lastElement = last_cell->previousElement;
    free(last_cell);
}

/**s
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    // Pokud seznam není aktivní, nebo je aktivní poslední prvek, funkce se ukončí
    if (list->activeElement == NULL || list->activeElement == list->lastElement){
        return;
    }
    /**                      __________________
    *                       /                  \
    *                      |                    |
    *                     Act   1    Z     2    Y
    *                  | cell |~x~| cell |~x~| cell |
    */
    // Do cell nastavíme adresu na Y, abychom mohli prvek Z vymazat (vynecháme ho ze seznamu)
    DLLElementPtr cell = list->activeElement->nextElement->nextElement;

    // Pokud prvek Y neexistuje, tak se jako 'poslední' nastaví právě aktivní prvek
    if (cell == NULL){
        list->lastElement = list->activeElement;

    // Pokud prvek Y existuje, tak se jako 'předchozí' prvek nastaví právě aktivní (odpojení x2)
    }else{
        cell->previousElement = list->activeElement;
    }
    // Vymažu požadovaný prvek
    free(list->activeElement->nextElement);
    // Připojím aktivní prvek k prvku Y (odpojení x1)
    list->activeElement->nextElement = cell;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    // Pokud není seznam aktivní, nebo je aktivní první prvek, funkce se ukončí
    if (list->activeElement == NULL || list->activeElement == list->firstElement){
        return;
    }
    /**                      __________________
    *                       /                  \
    *                      |                    |
    *                      Y    1    Z     2   Act
    *                  | cell |~x~| cell |~x~| cell |
    */
    // Do cell nastavíme adresu na Y, abychom mohli prvek Z vymazat (vynecháme ho ze seznamu)
    DLLElementPtr cell = list->activeElement->previousElement->previousElement;

    // Pokud prvek Y neexistuje, tak se jako 'první' nastaví právě aktivní prvek
    if (cell == NULL){
        list->firstElement = list->activeElement;

    // Pokud prvek Y existuje, tak se jako 'předchozí' prvek nastaví právě aktivní (odpojení x1)
    }else{
        cell->nextElement = list->activeElement;
    }
    // Vymažu požadovaný prvek
    free(list->activeElement->previousElement);
    // Připojím aktivní prvek k prvku Y (odpojení x2)
    list->activeElement->previousElement = cell;
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    // Pokud seznam není aktivní, funkce se ukončí
    if (list->activeElement == NULL){
        return;
    }

    // Alokuje pamět pro prvek
    DLLElementPtr cell = malloc(sizeof(struct DLLElement));

    // Pokud malloc vrátí NULL, funkce se ukončí, pokud ne, jinak...
    if (cell != NULL){
        // Jako 'předchozí' prvek nastaví právě aktivní, protože vkládáme za aktivní
        cell->previousElement = list->activeElement;
        // Přiřadíme data
        cell->data = data;
        // Navážeme na další prvek... prvek po aktivním
        cell->nextElement = list->activeElement->nextElement;    
    }else{
        DLL_Error();
        return;
    }

    // Navážeme aktivní prvek na právě vytvářený
    list->activeElement->nextElement = cell;

    // Pokud je aktivní prvek poslední => vkládáme nakonec seznamu
    // takže musíme nastavit právě alokovaný prvek jako 'last'
    if (list->lastElement == list->activeElement){
        list->lastElement = cell;

    }else{//             ___________________________________________________
        /**             /________________________                           \
         *             /                         \                           \
         *             ACT                     vkládaný                       X
         *  ______ __________ ______   ______ __________ ______   ______ __________ ______ 
         * | Prev |   DATA   | Next | | Prev |   DATA   | Next | | Prev |   DATA   | Next |
         * Teď prvek X 'previous' ukazuje na active, takže musíme změnit jeho 'prev' na 'vkládaný'
         */
        list->activeElement->nextElement->previousElement = cell;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    // Pokud seznam není aktivní, funkce se ukončí
    if (list->activeElement == NULL){
        return;
    }

    // Alokuje pamět pro prvek
    DLLElementPtr cell = malloc(sizeof(struct DLLElement));

    // Chyba, malloc vrátil NULL pointer, jinak...
    if (cell != NULL){
        // Jako 'následující' prvek nastaví právě aktivní, protože vkládáme před aktivní
        cell->nextElement = list->activeElement;
        // Přiřadíme data
        cell->data = data;
        // Navážeme na předchozí... prvek před aktivním
        cell->previousElement = list->activeElement->previousElement;
    }else{
        DLL_Error();
        return;
    }

    // Navážeme aktivní prvek na právě vytvářený
    list->activeElement->previousElement = cell;
    
    // Pokud je aktivní prvek první => vkládáme nazačátek seznamu
    // takže musíme nastavit právě alokovaný prvek jako 'first'
    if (list->firstElement == list->activeElement){
        list->firstElement = cell;

    }else{// Opak DLL_InsertAfter... too long...
        list->activeElement->previousElement->nextElement = cell;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    // Pokud neexistuje aktivní prvek, zavolá DLL_Error a ukončí funkci, jinak...
    if (list->activeElement != NULL){
        // Do *dataPtr uloží data z aktivního prvku seznamu
        *dataPtr = list->activeElement->data;    
    }else{
        DLL_Error();
        return;
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    // Pokud neexistuje aktivní prvek, ukončí funkci, jinak...
    if (list->activeElement != NULL){
        // Uloží do dat aktivního prvku hodnotu z int 'data'
        list->activeElement->data = data;   
    }else{
        return;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    /* Než jsem si pořádně přečetl... "Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním."
    else if (list->activeElement == list->lastElement){
        return;
    }
    */
    // Pokud neexistuje aktivní prvek, ukončí funkci, jinak...
    if (list->activeElement != NULL){
        // Nastaví jako aktivní prvek prvek, který následuje za ním...
        list->activeElement = list->activeElement->nextElement;
    }else{
        return;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    // Pokud neexistuje aktivní prvek, ukončí funkci, jinak...
    if (list->activeElement != NULL){
        // Nastaví jako aktivní prvek prvek, který mu předchází...
        list->activeElement = list->activeElement->previousElement;    
    }else{
        return;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    /**
    if (list->activeElement != NULL){
        return list->activeElement;
    }
    */
    // Vrátí hodnotu 1 je-li 'activeElement' různý od NULL, jinak 0
    return list->activeElement != NULL;
}

/* Konec c206.c */
