all: main

hex_gen.h: hex.py
	python3 $^ >$@

sbox_gen.h: sbox.py
	python3 $^ >$@

main: main.c hex.h hex_gen.h sbox_gen.h aes.h
	gcc -std=c99 main.c -o main

clean:
	rm main

distclean: clean
	rm hex_gen.h
