executable = exe
f_file = asm

${executable}: main.o ${f_file}.o
	gcc main.o ${f_file}.o -o ${executable}
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o
	
${f_file}.o: ${f_file}.s
	gcc -g -Wall -c ${f_file}.s -o ${f_file}.o
	
run: ${executable}
	./${executable}

clean:
	rm *.o ${executable}
