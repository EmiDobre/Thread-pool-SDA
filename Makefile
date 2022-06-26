build: 
	gcc -m32 -std=gnu99 -o tema2 stiva.c coada.c main.c
run:
	./tema2
clean:
	rm -rf tema2