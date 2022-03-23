/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0;
#define TRUE  1;

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // Pokud pokud uzel neexistuje, funkce se ukončí s FALSE
    if (tree == NULL){
        return FALSE;
    }
    // Pokud je uzel se správným klíčem nalezen, zapíše si hodnotu uzlu a vrátí TRUE
    if ((tree->key) == key){
        (*value) = (tree->value);
        return TRUE;
    }
    // Pokud je hledaný klíč větší, než klíč kde se právě nacházíme, voláme pravý strom
    if ((tree->key) > key){
        return bst_search(tree->right, key, value);
    // Pokud je menší, voláme levý strom...
    }else{
        return bst_search(tree->left, key, value);
    }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Pokud uzel neexistuje, můžeme vkládat
    if ((*tree) == NULL){
        // Alokování paměti pro právě vkládanou buňku
        bst_node_t *insert = malloc(sizeof(struct bst_node));
        // Kontrola mallocu
        if (insert == NULL){
            return;
        }

        // Inicializace nové buňky
        (insert->key) = key;
        (insert->value) = value;
        (insert->left) = NULL;
        (insert->right) = NULL;
        (*tree) = insert;
    }
    // Pokud uzel existuje, aktualizujeme hodnotu
    if ((*tree)->key == key){
        ((*tree)->value) = value;
    // Pokud je klíč vkládaný klíč větší, ne v aktuálním uzlu, posuneme se na levou větev, jinak na pravou
    }else if (((*tree)->key) > key){
        bst_insert(&(*tree)->left, key, value);
    }else if (((*tree)->key) < key){
        bst_insert(&(*tree)->right, key, value);
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // Pokud je následující pravý uzel NULL, nacházíme se na buňce nejvíc v pravo
    if ((*tree)->right == NULL){
        // Zapsání pro posun
        (target->key) = ((*tree)->key);
        (target->value) = ((*tree)->value);

        // Uvolnění buňky
        bst_node_t *cell = (*tree);
        (*tree) = ((*tree)->left);
        free(cell);

    // Pokud se nenacházíme nejvíc v pravo, posuneme se víc doprava
    }else{
        bst_replace_by_rightmost(target,&((*tree)->right));
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Pokud uzel neexistuje, funkce se ukončí
    if ((*tree) == NULL){
        return;
    }
    // Posuneme se k požadovanému uzlu, buď doleva, nebo doprava
    if (((*tree)->key) > key){
        bst_delete(&((*tree)->left), key);
        return;
    }else if (((*tree)->key) < key){
        bst_delete(&((*tree)->right), key);
        return;
    // Pokud najdeme požadovaný uzel
    }else if (((*tree)->key) == key){
        bst_node_t *del = (*tree);
        // Pokud jsme v úplně levém prvku, připojíme ho napravo a uvolnímme
        if (((*tree)->left) == NULL){
            (*tree) = (del->right);
            free(del);
            return;
        // To stejné, akorát na pravé straně
        }else if (((*tree)->right) == NULL){
            (*tree) = (del->left);
            free(del);
            return;
        }else{
            // Pokud uzel není prázdný, zavoláme funkci replace_by__rightmost
            if ((*tree) == NULL){
                return;
            }else{
                bst_replace_by_rightmost((*tree),&((*tree)->left));
            }
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    // Pokud uzel neexistuje, nic se neděje
    if ((*tree) == NULL){
        return;
    // Pokud existuje, rekurzivně smažeme levý a pravý strom
    }else{
        bst_dispose(&((*tree)->left));
        bst_dispose(&((*tree)->right));

        /// Uvolnění paměti
        free((*tree));
        // Nastavení do původního stavu
        *tree = NULL;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

}
