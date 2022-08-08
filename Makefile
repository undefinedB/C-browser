build:
	gcc -o task1 task1.c siteuri.c -I. -Wall
	gcc -o task2 task2.c siteuri.c -I. -Wall
	gcc -o task3 task3.c siteuri.c -I. -Wall
	gcc -o task4 task4.c siteuri.c -I. -Wall
	gcc -o browser browser.c siteuri.c -I. -Wall -lncurses -lmenu
	gcc -o bijoo browsermodular.c siteuri.c -I. -Wall -lncurses -lmenu
