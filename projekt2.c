/*
* FIIT STU
* Proceduralne Programovanie
* Autor: Pavol Hradsky

* Program: Projekt 2
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100 //maximalna velkost retazca

//funkcie na pracu so spajanymi zoznamami
typedef struct zvieratka { //vytvorenie struktury
   char meno[MAX];
   char druh[MAX];
   int vyska;
   double vaha;
   int datum_narodenia;
   int datum_krmenia;
   char meno_osetrovatela[MAX];
   struct zvieratka *next;
} ZVIERATKA;

//vytvori novy zaznam, priradi udaje z argumentu do struktury a vrati ju
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

//vlozi prvy prvok zoznamu, priradi mu head a tail
ZVIERATKA *vloz_prvy_prvok(ZVIERATKA **head, ZVIERATKA **tail, ZVIERATKA *zaznam){
    zaznam->next = *head;
    *head = zaznam;
    while(zaznam->next != NULL){
        zaznam = zaznam->next;
    }
    *tail = zaznam;
    return zaznam;
}

//vlozi zaznam na zaciatok zoznamu, aktualizuje head
ZVIERATKA *vloz_na_zaciatok(ZVIERATKA **head, ZVIERATKA *zaznam){
    zaznam->next = *head;
    *head = zaznam;
    return zaznam;
}

//vlozi zaznam na koniec zoznamu, aktualizuje tail (tato funkcia sa nepouziva)
ZVIERATKA *vloz_na_koniec(ZVIERATKA **tail, ZVIERATKA *zaznam){
    zaznam->next = (*tail)->next;
    (*tail)->next = zaznam;
    *tail = zaznam;
    return zaznam;
}

//vypise spajany zoznam (tato funkcia sa nepouziva)
void vypis_zoznam(ZVIERATKA *head){
    ZVIERATKA *tmp = head;
    while(tmp != NULL){
        printf("%s\n%s\n%d\n%.2lf\n%d\n%d\n%s\n\n", tmp->meno, tmp->druh, tmp->vyska, tmp->vaha, tmp->datum_narodenia, tmp->datum_krmenia, tmp->meno_osetrovatela);
        tmp = tmp->next;
    }
}


//aktualizuje subor, otvori subor a zapise do neho vsetky udaje zo spajaneho zoznamu, zavrie subor
void aktualizuj_subor(ZVIERATKA *head){
    ZVIERATKA *tmp = head;
    FILE *fw;
    fw = fopen("zvierata.txt", "w");
    while(tmp != NULL){
        fprintf(fw, "$$$\n");
        fprintf(fw, "%s\n", tmp->meno);
        fprintf(fw, "%s\n", tmp->druh);
        fprintf(fw, "%d\n", tmp->vyska);
        fprintf(fw, "%.5lf\n", tmp->vaha);
        fprintf(fw, "%d\n", tmp->datum_narodenia);
        fprintf(fw, "%d\n", tmp->datum_krmenia);
        fprintf(fw, "%s\n", tmp->meno_osetrovatela);
        tmp = tmp->next;
    }
    fclose(fw);
}

//funkcie

/*      ----FUNKCIA N----       
* Otvorenie suboru, porovnanie spravnosti, vytvorenie a zapisanie ho do spajaneho zoznamu
*/
int n(ZVIERATKA **head, ZVIERATKA **tail){//ready
    ZVIERATKA *tmp; //vytvorenie docastneho zaznamu
    FILE *fr;
    //otestovanie, ci uz bol spajany zoznam vytvoreny, ak ano tak sa uvolni
    if(*head != NULL){
        tmp = *head;
        while(*head != NULL){
            tmp = *head;
            *head = (*head)->next;
            free(tmp);
        }
    }
    //otvorenie suboru zvierata.txt, overenie ci sa otvoril spravne
    if((fr = fopen("zvierata.txt", "r")) == NULL){
        printf("Neotvoreny subor\n");
        return -1;
    }
    char tmp_str[MAX];//premenna, kam sa bude ukladat riedok subora
    int pocet = 0;
    //spocitanie riadkov v subore
    while(fgets(tmp_str, MAX, fr) != NULL){
        pocet++;
    }
    rewind(fr);

    //vytvorenie premennych na jednotlive data zo suboru
    char tmp_meno[MAX];
    char tmp_druh[MAX];
    int tmp_vyska = 0;
    double tmp_vaha;
    int tmp_datum_narodenia;
    int tmp_datum_krmenia;
    char tmp_meno_osetrovatela[MAX];

    int velkost = 0;
    for(int i = 0; i <= pocet; i++){
        fgets(tmp_str, MAX, fr); //po riadkoch cita subor a zapisuje riadky do premennej tmp_str
        //zistujeme delitelnost cislom 8 - pocet riadkov po ktorom sa opakuje zaznam
        switch (i%8)
        {
        case 0:
            if(i != 0){ //ak je vysledok 0 a nie je to prvy riadok, vytvori sa zaznam a vlozi sa na koniec spajaneho zoznamu (pripadne ako prvy prvok)
                tmp = novy_zaznam(tmp_meno, tmp_druh, tmp_vyska, tmp_vaha, tmp_datum_narodenia, tmp_datum_krmenia, tmp_meno_osetrovatela);
                if(i == 8){
                    vloz_prvy_prvok(head, tail, tmp);
                }else{
                    vloz_na_koniec(tail, tmp);
                }
                velkost++; //pocet zaznamov v spajanom zozname
            }
            break;
        case 1:
            if(strlen(tmp_str)>50){ //overi ci je meno kratsie ako 50 znakov, inak vypise chybu
                printf("Nespravne zadanz udaj: meno\n");
                return -1;
            }
            tmp_str[strlen(tmp_str)-1] = 0; //odstrani sa novy riadok na konci
            strcpy(tmp_meno, tmp_str); //skopiruje sa do premennej tmp_meno
            break;
        case 2:
            if(strlen(tmp_str)>30){ //overi ci je druh kratsie ako 30 znakov, inak vypise chybu
                printf("Nespravne zadany udaj: druh\n");
                return -1;
            }
            tmp_str[strlen(tmp_str)-1] = 0;//odstrani sa novy riadok na konci
            strcpy(tmp_druh, tmp_str);//skopiruje sa do premennej tmp_druh
            break;
        case 3:
            if(atoi(tmp_str)<0 || atoi(tmp_str)>30000){ //overi sa ci je vyska v danom intervale, inak vypise chybu
                printf("Nespravne zadany udaj: vyska\n");
                return -1;
            }
            tmp_vyska = atoi(tmp_str);//skopiruje sa do premennej tmp_vyska
            break;
        case 4:
            if(atof(tmp_str)<0 || atof(tmp_str)>3000000){//overi sa ci je vaha v danom intervale, inak vypise chybu
                printf("Nespravne zadany udaj: vaha\n");
                return -1;
            }
            tmp_vaha = atof(tmp_str);//skopiruje sa do premennej tmp_vaha
            break;
        case 5:

            tmp_datum_narodenia = atoi(tmp_str);//skopiruje sa do premennej tmp_datum_narodenia
            break;
        case 6:
            tmp_datum_krmenia = atoi(tmp_str);//skopiruje sa do premennej tmp_datum_krmenia
            break;
        case 7:
            if(i != pocet){
                tmp_str[strlen(tmp_str)-1] = 0;//ak to nie je posledny riadok v subore, odstrani sa novy riadok na konci
            }
            if(strlen(tmp_str)>50){//overi ci je meno kratsie ako 50 znakov, inak vypise chybu
                printf("Nespravne zadanz udaj: meno osetrovatela\n");
                return -1;
            }
            strcpy(tmp_meno_osetrovatela, tmp_str);//skopiruje sa do premennej tmp_meno_osetrovatela
            break;
        default:
            break;
        }

    }
    printf("Nacitalo sa %d zaznamov\n", velkost); //vypise pocet nacitanych zaznamov, zavrie subor a vrati velkost
    fclose(fr);
    return velkost;
}

/*      ----FUNKCIA V----       
* vypisanie spajaneho zoznamu
*/
int v(int velkost, ZVIERATKA *head){//ready
    //overi ci uz bol vytvoreny spajany zoznam (ak nie, velkost == -1) inak vypise chybu
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //postupne prechadza spajany zoznam a vypisuje udaje
    ZVIERATKA *tmp = head;
    for(int i = 1; i <= velkost; i++){
        printf("%d.\n", i);
        printf("Meno: %s\nDruh: %s\nVyska: %d\nVaha: %.2lf\nDatum narodenia: %d\nDatum krmenia: %d\nMeno osetrovatela: %s\n", tmp->meno, tmp->druh, tmp->vyska, tmp->vaha, tmp->datum_narodenia, tmp->datum_krmenia, tmp->meno_osetrovatela);
        tmp = tmp->next; //posunie sa na dalsi zaznam
    }
    return 0;
}

/*      ----FUNKCIA P----       
* vlozenie noveho zaznamu na n-te miesto spajaneho zoznamu, aktualizovanie subora
*/
int p(ZVIERATKA *head, ZVIERATKA **tail, int velkost){//ready
    //overi ci bol spajany zaznam vytvoreny
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //nacitanie premennych + miesta na ktore sa ma zaznam priradit, overi spravnost udajov (ako vo funkcii n)
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
    getchar();
    fgets(meno, MAX, stdin);
    meno[strlen(meno)-1] = 0;
    //getchar();
    fgets(druh, MAX, stdin);
    druh[strlen(druh)-1] = 0;
    scanf("%d", &vyska);
    scanf("%lf", &vaha);
    scanf("%d", &datum_narodenia);
    scanf("%d", &datum_krmenia);
    getchar();
    fgets(meno_osetrovatela, MAX, stdin);
    meno_osetrovatela[strlen(meno_osetrovatela)-1] = 0;

    if(strlen(meno)>50){
        printf("Nespravne zadany udaj: meno\n");
        return -1;
    }
    if(strlen(druh)>30){
        printf("Nespravne zadany udaj: druh\n");
        return -1;
    }
    if(vyska<0 || vyska>30000){
        printf("Nespravne zadany udaj: vyska\n");
        return -1;
    }
    if(vaha<0 || vaha>3000000){
        printf("Nespravne zadany udaj: vaha\n");
        return -1;
    }
    if(strlen(meno_osetrovatela)>50){
        printf("Nespravne zadany udaj: meno osetrovatela\n");
        return -1;
    }
    //vytvori novy zaznam z danych udajov
    ZVIERATKA *tmp = novy_zaznam(meno, druh, vyska, vaha, datum_narodenia, datum_krmenia, meno_osetrovatela);
    //ak je miesto mensie ako velkost subora, najde dane miesto a priradi zaznam do spajaneho zoznamu, inak priradi zaznam na koniec zpajaneho zoznamu a aktualizuje subor
    if (miesto <= velkost){
        for(int i = 1; i < miesto-1; i++){
            tmp_pred = tmp_pred->next;
        }

        tmp->next = tmp_pred->next;
        tmp_pred->next = tmp;
    }else{
        vloz_na_koniec(tail, tmp);
    }
    aktualizuj_subor(head);


    
    velkost++; //pripocita aj velkost zoznamu
    return velkost; //vrati novu velkost

}

/*      ----FUNKCIA Z----       
* vymazanie zaznamu podla mena
*/
int z(int velkost, ZVIERATKA *head){//ready
    //overi ci bol spajany zoznam uz vytvoreny
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //vytvorenie premennych tmp a tmp_pred aby sme mohli na koniec spojit zoznam bez daneho zaznamu
    ZVIERATKA *tmp = head;
    ZVIERATKA *tmp_pred = head;
    //nacitanie mena
    char meno[MAX];
    getchar();
    fgets(meno, MAX, stdin);
    meno[strlen(meno)-1] = 0;
    

    //najde meno v zozname
    while(strcmp(tmp->meno, meno) != 0 && tmp->next != NULL){
        tmp_pred = tmp;
        tmp = tmp->next;
    }
    if(strcmp(tmp->meno, meno) == 0){ //ak meno nasiel, uvolni dany zaznam a spoji zaznam pred nim so zaznamom za nim, zmensi velkost a aktualizuje zoznam
        tmp_pred->next = tmp->next;
        free(tmp);
        printf("Zviera s menom %s bolo vymazane.\n", meno);
        velkost--;
        aktualizuj_subor(head);
    }else{ //ak meno nenasiel, vypise hlasku
        printf("Zviera nebolo najdene\n");
    }
    return velkost; //vrati velkost

    
}

/*      ----FUNKCIA H----       
* zistenie ktore zvierata neboli nakrmene od daneho datumu
*/
int h(int velkost, ZVIERATKA *head){//ready
    //overi, ci bol uz zoznam vytvoreny
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //vytvorenie premennej tmp a datum a nacitanie datumu
    ZVIERATKA *tmp = head;
    int datum;
    scanf("%d", &datum);

    int i = 1; //pocitadlo
    int boli_nakrmene = 0; //bool (0 alebo 1)
    while(tmp->next != NULL){ //prejde cely zoznam a porovnava datum krmenia s danym datumom. ak je mensi, vypise dane zviera a zmeni hodnotu boli_nakrmene na 1
        if(tmp->datum_krmenia < datum){
            printf("%d.\n", i);
            printf("Meno: %s\nDruh: %s\nVyska: %d\nVaha: %.2lf\nDatum narodenia: %d\nDatum krmenia: %d\nMeno osetrovatela: %s\n", tmp->meno, tmp->druh, tmp->vyska, tmp->vaha, tmp->datum_narodenia, tmp->datum_krmenia, tmp->meno_osetrovatela);
            boli_nakrmene = 1;
            i++;
        }
        tmp = tmp->next;
    }
    if(boli_nakrmene == 0){ //ak boli_nakrmene nebola zmenena na 1 -> vsetky zvierata boli nakrmene, vypise hlasku
        printf("Vsetky zvierata boli k datumu %d nakrmene\n", datum);
    }
    return velkost; //vrati velkost
}

/*      ----FUNKCIA A----       
* aktualizovanie datumu nakrmenia zvierata
*/
int a(int velkost, ZVIERATKA **head){//ready
    //overi ci uz bol zoznam vytvoreny
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //nacitanie mena a noveho datumu nakrmenia
    ZVIERATKA *tmp = *head;
    char meno[MAX];
    int datum;
    getchar();
    fgets(meno, MAX, stdin);
    meno[strlen(meno)-1] = 0;
    scanf("%d", &datum);

    //hlada zviera s danym menom
    while(strcmp(tmp->meno, meno) != 0 && tmp->next != NULL){
        tmp = tmp->next;
    }
    //ak ho naslo, priradi mu novy datum, vypise hlasku a aktualizuje subor
    if(strcmp(tmp->meno, meno) == 0){
        tmp->datum_krmenia = datum;
        printf("Zviera s menom %s bolo naposledy nakrmene dna %d\n", tmp->meno, datum);
        aktualizuj_subor(*head);
    }else{ //ak ho nenaslo vypise hlasku
        printf("Zviera nebolo najdene\n");
    }
    return 0;
}

/*      ----FUNKCIA K----       
* uvolnenie spajaneho zoznamu
*/
int k(int velkost, ZVIERATKA **head){//ready
    //overi, ci uz bol zoznam vytvoreny
    if(velkost == -1){
        printf("Subor este nebol otvoreny\n");
        return -1;
    }
    //prejde cely spajany zoznam, priradi hlavicku dalsiemu prvku a dany prvok uvolni
    ZVIERATKA *tmp = *head;
    while(*head != NULL){
        tmp = *head;
        *head = (*head)->next;
        //printf("Zviera s menom %s bolo uvolnene\n", tmp->meno);
        free(tmp);
    }
    return 0;
}

/*      ----MAIN----       
* vytvorenie head a tail, volanie funkcii
*/
int main(void){
    //vytvorenie premennych na pamatanie zaciatku a konca spajaneho zoznamu
    ZVIERATKA *head = NULL;
    ZVIERATKA *tail = NULL;
    //premenna funkcia na nacitavanie znakov a velkost s defoultnou hodnotou -1
    char funkcia;
    int velkost = -1;

    while(1){ //nekonecny cyklus
        scanf("%c", &funkcia); //nacita znak a spusti danu funkciu
        if(funkcia == 'n'){
            velkost = n(&head, &tail);//spusti funkciu a priradi navratovu hodnotu funkcie do premennej velkost
            if(velkost == -1){
                printf("Chyba v udajoch\n"); //ak bola vratena -1, vypise chybu a ukonci program
                return velkost;
            }
        }
        if(funkcia == 'v'){
            v(velkost, head); //volanie funkcie v
        }
        if(funkcia == 'p'){
            velkost = p(head, &tail, velkost); //volanie funkcie p, aktualizovanie velkosti
        }
        if(funkcia == 'z'){
            velkost = z(velkost, head); //volanie funkcie z, aktualizovanie velkosti
        }
        if(funkcia == 'h'){
            velkost = h(velkost, head); //volanie funkcie h, aktualizovanie velkosti
        }
        if(funkcia == 'a'){
            a(velkost, &head); //volanie funkcie a
        }
        if(funkcia == 'k'){
            k(velkost, &head);  //volanie funkcie k, ukoncenie programu
            return 0;
        }
    }



    return 0;//end
}
