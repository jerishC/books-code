# file Makefile
# brief this file is use to build ch05 example
# author Jianlong Chen <jianlong99@gmail.com>
# date 2013-06-02
# $Id$

PROG = menu1\
	   menu2\
	   menu3\
	   password\
	   menu4\
	   badterm\
	   sizeterm\
	   menu5\
	   kbhit

all: $(PROG)

menu1: menu1.c
menu2: menu2.c
menu3: menu3.c
password: password.c
menu4: menu4.c
badterm: badterm.c
	$(CC) $^ -o $@ -lcurses
sizeterm: sizeterm.c
	$(CC) $^ -o $@ -lcurses
menu5: menu5.c
	$(CC) $^ -o $@ -lcurses
kbhit: kbhit.c
	$(CC) $^ -o $@ -lcurses

install:
	$(Q) install -d $(DESTDIR)/bin/ && install $(PROG) $(DESTDIR)/bin

clean:
	$(Q) -rm $(PROG)

