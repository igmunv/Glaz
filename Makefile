all:
	gcc -o output/glaz src/main.c
	$(MAKE) run;

run:
	output/glaz
