CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall -lfl -ly
EXEC=tcompil

$(EXEC): $(EXEC).tab.o lex.yy.o
	$(CC) -o $@ $^ $(LDFLAGS)

lex.yy.c: $(EXEC).lex
	  flex $(EXEC).lex

$(EXEC).tab.c: $(EXEC).y
	bison -d $(EXEC).y -v

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f lex.yy.*
	rm -f *.tab.*
	rm -f $(EXEC).output

table:
	bison -v $(EXEC).y