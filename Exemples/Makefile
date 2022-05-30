EXEC=code

%.o: %.asm
	nasm -f elf64 -o $@ $<

$(EXEC): $(EXEC).o utils.o
	gcc -o $@ $^ -nostartfiles

clean:
	rm *.o