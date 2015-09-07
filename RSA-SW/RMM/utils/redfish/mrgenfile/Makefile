mrgenfile: link
	gcc -c -I ../../../src/include/ main.c -o main.o
	gcc -c -I ../../../src/include/ file.c -o file.o
	gcc -c -I ../../../src/include/ json.c -o json.o
	gcc -c -I ../../../src/include/ msg_reg.c -o msg_reg.o
	gcc -o $@ main.o msg_reg.o json.o file.o
	rm -f *.o file.c  json.c  msg_reg.c

.PHONY: link clean
link: 
	ln -s ../../../src/lib/libutils/file.c file.c
	ln -s ../../../src/lib/libjson/json.c
	ln -s ../../../src/lib/libredfish/msg_reg.c

clean:
	rm -f mrgenfile *.o file.c  json.c  msg_reg.c
