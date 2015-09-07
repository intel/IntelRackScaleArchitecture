mrgenfile: link
	g++ encrypter.cpp -I./ -fPIC -shared -o libencrypter.so -lcrypt
	gcc main.c -Wall -o encrypt -L./ -lencrypter -Wl,-rpath=./

.PHONY: link clean
link:
	ln -s ../../src/lib/libencrypter/encrypter.cpp encrypter.cpp
	(mkdir libencrypter;cd libencrypter; ln -s ../../../src/include/libencrypter/encrypter.hpp encrypter.hpp; cd ..)

clean:
	rm -rf encrypter.cpp libencrypter libencrypter.so encrypt
