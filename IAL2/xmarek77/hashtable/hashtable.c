/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
	result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
	// Každy prvek v tabulce o velikosti (MAX_HT_SIZE) se nastaví na NULL
	for (int i = 0; i < MAX_HT_SIZE; ++i){
		((*table)[i]) = NULL;
	}
	
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
	ht_item_t *find = ((*table)[get_hash(key)]);

	// Hledáme dokud nejsme na konci tabulky a zároveň dokud jsme nenašli stejný klíč
	while (find != NULL && strcmp(find->key, key) != 0){
			find = find->next;
	}
	return find;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
	ht_item_t *find = ht_search(table, key);

	if (find != NULL){
		find->value = value; // Pokud prvek s daným klíčem existuje, aktualizuje se hodnota
	}else{
		ht_item_t *add_item = malloc(sizeof(ht_item_t)); // Alokujeme místo pro nový prvek
		if (add_item == NULL){ // Chyba při operaci malloc, funkce se ukončí
			return;
		}

		// Inicializace nového prvku
		add_item->key = key;
		add_item->value = value;
		add_item->next = ((*table)[get_hash(key)]); // Ukazatel na další prvek = první prvek

		// Protože přidáváme na začátek, musíme právě vkládaný prvek označit za 'první'
		((*table)[get_hash(key)]) = add_item;
	}
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
	ht_item_t *find = (ht_search(table, key));

	// Pokud prvek existuje, vrátí jeho hodnotu, pokud neexistuje, funkce se ukonči
	if (find != 0){
		return &(find->value);
	}else{
		return NULL;
	}
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
	ht_item_t *find = ((*table)[get_hash(key)]);
	ht_item_t *find_prev = ((*table)[get_hash(key)]);

	// Pokud prvek neexistuje, funkce se ukončí
	if (find == NULL){
		return;
	}

	// Cyklíme dokud nejsme na konci tabulky, nebo dokud nenarazíme na hledaný prvek
	while (find != NULL && strcmp(find->key, key) != 0){
		find_prev = find; // Předchozí prvek nastavíme na aktuální
		find = find->next; // A aktuální nastavíme na následující "posunujeme se"
	}

	// Pokud předchozí prven neexistuje, nacházíme se na prvním prvku tabulky
	if (find_prev != NULL){
		find_prev->next = find->next;
	}else{
		((*table)[get_hash(key)]) = find->next;
	}

	// Uvolnění zadaného prvku
	free(find);
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
	ht_item_t *find;
	ht_item_t *find_next;

	// Cyklíme od začátku tabulky do konce
	for (int i = 0; i < MAX_HT_SIZE; ++i){
		find_next = ((*table)[i]);
		// Cyklíme dokud 'následující' prvek není NULL => další prvek už neexistuje, konec tabulky
		while (find_next != NULL){
			// Posunování se v tabulce
			find = find_next->next;
			find_next = find;
		}
		// Uvolnění aktuálního prvku
		free(find_next);
		((*table)[i]) = NULL;
	}
}
