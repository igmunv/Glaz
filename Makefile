all:
	gcc -o glaz src/main.c
	$(MAKE) run;

run:
	./glaz
