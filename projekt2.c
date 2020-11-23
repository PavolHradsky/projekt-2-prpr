/*
* FIIT STU
* Proceduralne Programovanie
* Autor: Pavol Hradsky

* Program: Projekt 2
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 50

typedef struct zvieratka {
   char meno[MAX];
   char druh[MAX];
   int vyska;
   double vaha;
   int datum_narodenia;
   int datum_krmenia;
   char meno_osetrovatela[MAX];
   struct zvieratka *next;
} ZVIERATKA;

ZVIERATKA *novy_zaznam(char meno[], char druh[], int vyska, double vaha, int datum_narodenia, int datum_krmenia, char meno_osetrovatela[]){
    ZVIERATKA *result = malloc(sizeof(ZVIERATKA));
    strcpy(result->meno, meno);
    strcpy(result->druh, druh);
    result->vyska = vyska;
    result->vaha = vaha;
    result->datum_narodenia = datum_narodenia;
    result->datum_krmenia = datum_krmenia;
    strcpy(result->meno_osetrovatela, meno_osetrovatela);
    return result;
}

ZVIERATKA *vloz_prvy_prvok(ZVIERATKA **head, ZVIERATKA **tail, ZVIERATKA *zaznam){
    zaznam->next = *head;
    *head = zaznam;
    while(zaznam->next != NULL){
        zaznam = zaznam->next;
    }
    *tail = zaznam;
    return zaznam;
}

ZVIERATKA *vloz_na_zaciatok(ZVIERATKA **head, ZVIERATKA *zaznam){
    zaznam->next = *head;
    *head = zaznam;
    return zaznam;
}

ZVIERATKA *vloz_na_koniec(ZVIERATKA **tail, ZVIERATKA *zaznam){
    zaznam->next = (*tail)->next;
    (*tail)->next = zaznam;
    *tail = zaznam;
    return zaznam;
}

void vypis_zoznam(ZVIERATKA *head){
    ZVIERATKA *tmp = head;
    while(tmp != NULL){
        printf("%s\n%s\n%d\n%.2lf\n%d\n%d\n%s\n\n", tmp->meno, tmp->druh, tmp->vyska, tmp->vaha, tmp->datum_narodenia, tmp->datum_krmenia, tmp->meno_osetrovatela);
        tmp = tmp->next;
    }
}



int n(ZVIERATKA **head, ZVIERATKA **tail){
    ZVIERATKA *tmp;
    FILE *fr;

    if(*head != NULL){
        tmp = *head;
        while(*head != NULL){
            tmp = *head;
            *head = (*head)->next;
            free(tmp);
        }
    }

    if((fr = fopen("zvierata.txt", "r")) == NULL){
        printf("Neotvoreny subor\n");
        return -1;
    }
    char tmp_str[MAX];
    int pocet = 0;

    while(fgets(tmp_str, MAX, fr) != NULL){
        pocet++;
    }
    rewind(fr);

    char tmp_meno[MAX];
    char tmp_druh[MAX];
    int tmp_vyska = 0;
    double tmp_vaha;
    int tmp_datum_narodenia;
    int tmp_datum_krmenia;
    char tmp_meno_osetrovatela[MAX];

    int velkost = 0;
    for(int i = 0; i <= pocet; i++){
        fgets(tmp_str, MAX, fr);
        //printf("%s", tmp_str);
        //printf("%d ", i);
        switch (i%8)
        {
        case 0:
            if(i != 0){
                tmp = novy_zaznam(tmp_meno, tmp_druh, tmp_vyska, tmp_vaha, tmp_datum_narodenia, tmp_datum_krmenia, tmp_meno_osetrovatela);
                //printf("OK");
                if(i == 8){
                    vloz_prvy_prvok(head, tail, tmp);
                }else{
                    vloz_na_koniec(tail, tmp);
                }
                velkost++;
                //vloz_na_zaciatok(head, tmp);
            }
            break;
        case 1:
            tmp_str[strlen(tmp_str)-1] = 0;
            strcpy(tmp_meno, tmp_str);
            break;
        case 2:
            tmp_str[strlen(tmp_str)-1] = 0;
            strcpy(tmp_druh, tmp_str);
            break;
        case 3:
            tmp_vyska = atoi(tmp_str);
            break;
        case 4:
            tmp_vaha = atof(tmp_str);
            break;
        case 5:
            tmp_datum_narodenia = atoi(tmp_str);
            break;
        case 6:
            tmp_datum_krmenia = atoi(tmp_str);
            break;
        case 7:
            if(i != pocet){
                tmp_str[strlen(tmp_str)-1] = 0;
            }
            strcpy(tmp_meno_osetrovatela, tmp_str);
            break;
        default:
            break;
        }

    }
    printf("Nacitalo sa %d zaznamov\n", velkost);
    return velkost;
}

void v(int velkost, ZVIERATKA *head){
    ZVIERATKA *tmp = head;
    for(int i = 1; i <= velkost; i++){
        printf("%d.\n", i);
        printf("Meno: %s\nDruh: %s\nVyska: %d\nVaha: %.2lf\nDatum narodenia: %d\nDatum krmenia: %d\nMeno osetrovatela: %s\n", tmp->meno, tmp->druh, tmp->vyska, tmp->vaha, tmp->datum_narodenia, tmp->datum_krmenia, tmp->meno_osetrovatela);
        tmp = tmp->next;
    }
}

int p(ZVIERATKA *head, int velkost){//fine, but treba to aj zapisat (samotna funkcia?)

    ZVIERATKA *tmp_pred = head;
    int miesto;
    char meno[MAX];
    char druh[MAX];
    int vyska;
    double vaha;
    int datum_narodenia;
    int datum_krmenia;
    char meno_osetrovatela[MAX];

    scanf("%d", &miesto);
    scanf("%s", meno);
    scanf("%s", druh);
    scanf("%d", &vyska);
    scanf("%lf", &vaha);
    scanf("%d", &datum_narodenia);
    scanf("%d", &datum_krmenia);
    scanf("%s", meno_osetrovatela);

    ZVIERATKA *tmp = novy_zaznam(meno, druh, vyska, vaha, datum_narodenia, datum_krmenia, meno_osetrovatela);

    for(int i = 1; i < miesto-1; i++){
        tmp_pred = tmp_pred->next;
    }

    tmp->next = tmp_pred->next;
    tmp_pred->next = tmp;

    velkost++;
    return velkost;

}

int main(void){

    ZVIERATKA *head = NULL;
    ZVIERATKA *tail = NULL;

    //ZVIERATKA *tmp;

    int velkost = n(&head, &tail);
    velkost = p(head, velkost);
    v(velkost, head);


    return 0;
}