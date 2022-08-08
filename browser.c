#include <ncurses.h>

#include <menu.h>

#include <string.h>

#include <stdlib.h>

#include <string.h>

#include "siteuri.h"
 //compareA si compareS, pentru cautarile avansate si simple
int cmp(site v1, site v2) {
  int x = strcmp(v1.continut, v2.continut);
  if (x != 0) return (x < 0);
  else {
    return (v1.accesari > v2.accesari);
  }
}
int cmpA(site v1, site v2) {
  return (v1.accesari > v2.accesari);
}
//pagina de rezultate, dupa selectarea unui tip de cautare
void rezultate(int c, char * setcuvinte, int y, int x) {
  //mai intai incarc in memorie si sortez, asemenea task-urilor anterioare
  if (c == 'A') {
    vsite * v = malloc(sizeof(vsite));
    incarcaR(v, setcuvinte);
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
  }
  return;
}
//ecran principal cu bara de cautare si legenda jos
//legenda se schimba daca sunt apasate taste din legenda
void ecranPRINCIPAL() {
  clear();
  int x, y;
  getmaxyx(stdscr, y, x);
  int lungimeX = (x - strlen("BROWSERUL LUI BIJU") - 6) / 2;
  int lungimeY = y / 3 + 1;
  int height = 3;
  int width = strlen("BROWSERUL LUI BIJU") + 6;
  WINDOW * baracautare = newwin(height, width, lungimeY, lungimeX); //bara de cautare
  box(baracautare, 0, 0);
  mvprintw(y / 3, (x - strlen("BROWSERUL LUI BIJU")) / 2, "BROWSERUL LUI BIJU");
  //legenda pentru comenzi
  WINDOW * legenda = newwin(1, strlen("Enter - search A - advanced S - simple B - back Q - quit"), y - 1, 0);
  wattron(legenda, A_REVERSE);
  wprintw(legenda, "C - search Q - quit");
  refresh();
  wrefresh(baracautare);
  wrefresh(legenda);
  //c poate fi Q eventual, ceea ce ar strica programul
  int c = 0;
  //daca nu citesc Q si daca caracterul anterior nu este Q, continui
  while (c != 'Q' && (c = getch()) != 'Q') {

    if (c == 'C') {
      curs_set(1);
      //schimb legenda
      werase(legenda);
      wprintw(legenda, "Enter - search Q - quit");
      wrefresh(legenda);
      //ma mut in modul de cauatare
      wmove(baracautare, 1, 1);
      wrefresh(baracautare);
      //trebuie sa retin cautarea intr-un string, acesta este alocat
      //dinamic, iar realocarea se face astlfel incat capacitatea se
      //dubleaza de fiecare daca cand nu mai are loc
      char * string = calloc(1, 1);
      int cap = 1;
      //continui doar daca nu citesc Q
      while ((c = getch()) != 'Q') {
        int lungime = strlen(string);
        if (cap - 1 < lungime) {
          cap = cap * 2;
          string = realloc(string, cap);
        }
        //daca dau backspace, voi sterge;
        switch (c) {
          //blochez
        case 10: {
          curs_set(0);
          werase(legenda);
          wprintw(legenda, "A - advanced S - simple Q - quit");
          wrefresh(legenda);
          //cazul de afisare al rezultatelor
          while ((c = getch())) {
            if (c == 'A' || c == 'S') {
              clear();
              refresh();
              //updatez legenda
              werase(legenda);
              wprintw(legenda, "Enter - select B - back");
              wrefresh(legenda);
              rezultate(c, string, y, x);
              werase(legenda);
              wprintw(legenda, "Enter - search Q - quit");
              wrefresh(legenda);
              curs_set(1);
              //label-ul
             mvprintw(y / 3, (x - strlen("BROWSERUL LUI BIJU")) / 2, "BROWSERUL LUI BIJU");
              break;
            }
            if (c == 'Q') return;
          }
        }
        break;
        case 127: {
          string[lungime - 1] = '\0';
        }
        break;
        default: {
          char * clona = strdup(string);
          sprintf(string, "%s%c", clona, c);
          free(clona);
        }
        break;
        }
        //enter blocheaza si cere ce mod de cautare 
        //vrea utilizatorul sa foloseasca
        wclear(baracautare);
        box(baracautare, 0, 0);
        //daca stringul iese din bara de cautare, voi afisa
        //doar cate caractere imi permite bara de cautare
        wmove(baracautare, 1, 1);wrefresh(baracautare);
        //actualizez lungimea
        lungime = strlen(string);
        if (lungime > width - 4) {
          wprintw(baracautare, string + lungime - width + 4);
        } else wprintw(baracautare, string);
        wrefresh(baracautare);
      }
      free(string);
    }
  }

  return;
}

int main() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  ecranPRINCIPAL();
  endwin();
  return 0;
}