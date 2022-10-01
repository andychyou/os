target = testlib
objects = bitmap.o debug.o hash.o hex_dump.o list.o testlib.o
headers = bitmap.h debug.h hash.h hex_dump.h limits.h list.h round.h

$(target): $(objects)
	gcc -o $(target) $(objects)
$(objects): $(headers)

.PHONY: clean
clean: 
	rm $(target) $(objects) 
