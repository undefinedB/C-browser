#include <ncurses.h>

#include <menu.h>

#include <string.h>

#include <stdlib.h>

#include "siteuri.h"

//compareA si compareS, pentru cautarile avansate si simple
int cmpS(site v1, site v2) {
  int x = strcmp(v1.continut, v2.continut);
  if (x != 0) return (x < 0);
  else {
    return (v1.accesari > v2.accesari);
  }
}
int cmpA(site v1, site v2) {
  return (v1.accesari > v2.accesari);
}

//functie care printeaza legenda

void printlegenda(WINDOW * legenda, char * string) {
  werase(legenda);
  wprintw(legenda, string);
  refresh();
  wrefresh(legenda);
  return;
}


//functie care printeaza bara de cautare cu numele browserului
//si cu un string in ea
//browserul se numeste "BROWSERUL LUI BIJU" si are 18 caractere
//bara de cautare are 24 caractere

void printbara(WINDOW * bara, int y, int x, char * string) {
  mvprintw(y / 3, (x - strlen("BROWSERUL LUI BIJU")) / 2, "BROWSERUL LUI BIJU");
  refresh();
  werase(bara);
  box(bara, 0, 0);
  int lungime = strlen(string);
  if (lungime > 20) {
    mvwprintw(bara, 1, 1, string + lungime - 20);
  } else mvwprintw(bara, 1, 1, string);
  wrefresh(bara);
  return;
}

//functie care afiseaza o pagina

void printpagina(site si)

{
  // voi extrage culorile, daca ele exista
  //intr-un cod html, dupa indicatiile date, strstr la "color:" are trebui sa dea pointer exact
  //la tag-ul p descris
    culoare cul = black;
    culoare culbg = white;
    //iau linia cu <p, pe ea ar trebui sa fie tag-ul
    char *pointer = strstr(si.continut, "<p");
    pointer = strtok(pointer, "\n");
    //culorile
    if(strstr(pointer, "color:red")!=NULL) {
      cul = red;
      pointer = strstr(pointer, "color:red") +1;
    }
    if(strstr(pointer, "color:black")!=NULL) {
      cul = black;
      pointer = strstr(pointer, "color:black")+1;
    }
    if(strstr(pointer, "color:white")!=NULL) {
      cul = white;
      pointer = strstr(pointer, "color:white")+1;
    }
    if(strstr(pointer, "color:yellow")!=NULL) {
      cul = yellow;
      pointer = strstr(pointer, "color:yellow")+1;
    }
    if(strstr(pointer, "color:blue")!=NULL) {
      cul = blue;
      pointer = strstr(pointer, "color:blue")+1;
    }
    if(strstr(pointer, "color:green")!=NULL) {
      cul = green;
      pointer = strstr(pointer, "color:green")+1;
    }
    if(strstr(pointer, "background-color:red")!=NULL) {
      culbg = red;
    }
    if(strstr(pointer, "background-color:black")!=NULL) {
      culbg = black;
    }
    if(strstr(pointer, "background-color:white")!=NULL) {
      culbg = white;
    }
    if(strstr(pointer, "background-color:yellow")!=NULL) {
      culbg = yellow;
    }
    if(strstr(pointer, "background-color:blue")!=NULL) {
      culbg = blue;
    }
    if(strstr(pointer, "background-color:green")!=NULL) {
      culbg = green;
    }
    culbg = white;
    attron(A_BOLD);
    printw(si.titlu);
    attroff(A_BOLD);
    //culorile
    start_color();
    init_pair(1, cul, culbg);
    wbkgd(stdscr, COLOR_PAIR(1));
    mvprintw(1,1,si.paragraf);
    return;
}



//functie care printeaza rezultatele

void rezultate(WINDOW *legenda, int c, char * setcuvinte, int y, int x) {
  //mai intai incarc in memorie si sortez, asemenea task-urilor anterioare
  if (c == 'A') {
    int k = strlen(setcuvinte);
    //conform algoritmului cu stdin, va trebui ca setcuvinte sa se termine in \n, deci fac un dump
    //nu este neaparat sa fie \n, doar ceva ce poate fi scos ulterior
    char *dump = calloc(k+2, 1);
    sprintf(dump, "%s%c", setcuvinte, 1);
    vsite * v = malloc(sizeof(vsite));
    incarcaR(v, dump);
    sortare(v, & cmpA);
    //creez un window
    WINDOW * results = newwin(y - 1, x, 0, 0);
    //o bara pentru textul cautat
    WINDOW * baratext = newwin(3, x - 2, 1, 1);
    box(baratext, 0, 0);
    mvwprintw(baratext, 1, 1, setcuvinte);
    box(results, 0, 0);
    //voi crea un meniu cu o fereastra asociata
    WINDOW * menuwin = newwin(y - 6, x - 2, 4, 1);
    ITEM ** selectii;
    //ITEM *selectiecurenta;
    MENU * meniu;
    //aloc selectiile
    selectii = calloc(v -> indice + 1, sizeof(ITEM * ));
    for (int i = 0; i < v -> indice; i++)
      selectii[i] = new_item((v -> v)[i].URL, (v -> v)[i].titlu);
    selectii[v -> indice] = (ITEM * ) NULL;
    meniu = new_menu(selectii);

    keypad(menuwin, TRUE);

    set_menu_win(meniu, menuwin);
    set_menu_sub(meniu, derwin(menuwin, y - 8, x - 4, 1, 1));

    box(menuwin, 0, 0);
    refresh();
    post_menu(meniu);
    wrefresh(menuwin);
    //wrefresh(results);
    wrefresh(baratext);
    while ((c = wgetch(menuwin)) != 'B') {
      switch (c) {
      case KEY_DOWN:
        menu_driver(meniu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(meniu, REQ_UP_ITEM);
        break;
      //pe enter voi afisa site-ul
      case 10:
        {
            erase();
            printpagina((v->v)[item_index(current_item(meniu))]);
            printlegenda(legenda, "B - back");
            refresh();
            while((c=wgetch(menuwin))!='B');
            wbkgd(stdscr, COLOR_PAIR(0));
            erase();
            refresh();
            printlegenda(legenda, "Enter - select B - back");
            box(menuwin, 0, 0);
            box(baratext, 0, 0);
            mvwprintw(baratext, 1, 1, setcuvinte);
            wrefresh(baratext);
            post_menu(meniu);
            wrefresh(menuwin);
        }
        break;
      }
    }
    //eliberez memoria si dau clear
    erase();
    refresh();
    for (int i = 0; i < (v -> indice); i++)
      free_item(selectii[i]);
    free(selectii);
    free_menu(meniu);
    descarca(v);
    free(dump);
  }
  //al doilea caz
  if (c == 'S') {
    int k = strlen(setcuvinte);
    //conform algoritmului cu stdin, va trebui ca setcuvinte sa se termine in \n, deci fac un dump
    //nu este neaparat sa fie \n, doar ceva ce poate fi scos ulterior
    char *dump = calloc(k+2, 1);
    sprintf(dump, "%s%c", setcuvinte, 1);
    vsite * v = malloc(sizeof(vsite));
    incarcaR(v, dump);
    sortare(v, & cmpS);
    //creez un window
    WINDOW * results = newwin(y - 1, x, 0, 0);
    //o bara pentru textul cautat
    WINDOW * baratext = newwin(3, x - 2, 1, 1);
    box(baratext, 0, 0);
    mvwprintw(baratext, 1, 1, setcuvinte);
    box(results, 0, 0);
    //voi crea un meniu cu o fereastra asociata
    WINDOW * menuwin = newwin(y - 6, x - 2, 4, 1);
    ITEM ** selectii;
    //ITEM *selectiecurenta;
    MENU * meniu;
    //aloc selectiile
    selectii = calloc(v -> indice + 1, sizeof(ITEM * ));
    for (int i = 0; i < v -> indice; i++)
      selectii[i] = new_item((v -> v)[i].URL, (v -> v)[i].titlu);
    selectii[v -> indice] = (ITEM * ) NULL;
    meniu = new_menu(selectii);

    keypad(menuwin, TRUE);

    set_menu_win(meniu, menuwin);
    set_menu_sub(meniu, derwin(menuwin, y - 8, x - 4, 1, 1));

    box(menuwin, 0, 0);
    refresh();
    post_menu(meniu);
    wrefresh(menuwin);
    //wrefresh(results);
    wrefresh(baratext);
    while ((c = wgetch(menuwin)) != 'B') {
      switch (c) {
      case KEY_DOWN:
        menu_driver(meniu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(meniu, REQ_UP_ITEM);
        break;
      //pe enter voi afisa site-ul
      case 10:
        {
            erase();
            printpagina((v->v)[item_index(current_item(meniu))]);
            printlegenda(legenda, "B - back");
            refresh();
            while((c=wgetch(menuwin))!='B');
            wbkgd(stdscr, COLOR_PAIR(0));
            erase();
            refresh();
            printlegenda(legenda, "Enter - select B - back");
            box(menuwin, 0, 0);
            box(baratext, 0, 0);
            mvwprintw(baratext, 1, 1, setcuvinte);
            wrefresh(baratext);
            post_menu(meniu);
            wrefresh(menuwin);
        }
        break;
      }
    }
    //eliberez memoria si dau clear
    erase();
    refresh();
    for (int i = 0; i < (v -> indice); i++)
      free_item(selectii[i]);
    free(selectii);
    free_menu(meniu);
    descarca(v);
    free(dump);
  }



  return;
}




//functie de ecran principal

void ecranPRINCIPAL() {
  clear();
  int x, y;
  getmaxyx(stdscr, y, x);
  int lungimeX = (x - 18 - 6) / 2;
  int lungimeY = y / 3 + 1;
  int height = 3;
  int width = 18 + 6;
  WINDOW * baracautare = newwin(height, width, lungimeY, lungimeX); //bara de cautare
  WINDOW * legenda = newwin(1, x - 1, y - 1, 0);
  wattron(legenda, A_REVERSE);
  printlegenda(legenda, "C - search Q - quit");
  printbara(baracautare, y, x, "");
  //declar c, care nu este 0
  int c = 0;
  //daca nu citesc Q si daca caracterul anterior nu este Q, continui

  while (c != 'Q' && (c = getch()) != 'Q') {
    if (c == 'C') {
      //updatez legenda
      printlegenda(legenda, "Enter - search Q - quit");

      //mut cursorul pe bara de cautare
      curs_set(1);
      wmove(baracautare, 1, 1);
      wrefresh(baracautare);

      //trebuie sa retin cautarea intr-un string, acesta este alocat
      //dinamic, iar realocarea se face astlfel incat capacitatea se
      //dubleaza de fiecare daca cand nu mai are loc
      char * string = calloc(1, 1);
      int cap = 1;

      //daca citesc Q e quit
      while ((c = getch()) != 'Q') {
        //realocare
        int l = strlen(string);
        if (cap - 1 < l) {
          cap = cap * 2;
          string = realloc(string, cap);
        }


        switch (c) {

          //backspace sterge
        case 127: {
          string[l - 1] = '\0';
        }
        break;

        //enter blocheaza si intra in modurile de cautare
        case 10: {
          curs_set(0);
          printlegenda(legenda, "A - advanced S - simple Q - quit");
          while ((c = getch())) {
            if (c == 'A' || c == 'S') {
              clear();
              refresh();
              //updatez legenda
              printlegenda(legenda, "Enter - select B - back");
              rezultate(legenda, c, string, y, x);
              printlegenda(legenda, "Enter - search Q - quit");
              curs_set(1);
              //label-ul
              printbara(baracautare, y, x, string);
              break;
            }
            if (c == 'Q') return;
          }
        }
        break;

        //cazul default
        default: {
          char * clona = strdup(string);
          sprintf(string, "%s%c", clona, c);
          free(clona);
        }
        break;
        }
        printbara(baracautare, y, x, string);
      }


      
    }
  }

  return;
}

//main-ul

int main() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  ecranPRINCIPAL();
  endwin();
  return 0;
}