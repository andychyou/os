target = testlib
objects = bitmap.o debug.o hash.o hex_dump.o list.o testlib.o

$(target): $(objects)
	gcc -o $(target) $(objects)


.PHONY: clean
clean: 
	rm $(target) $(objects) 
