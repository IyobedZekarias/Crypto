#CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha
CPPFLAGS = -g -std=gnu++17 -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha -fno-builtin

all: xor rand tests aes

xor: xor.cpp crypto_xor.cpp crypto_rand.cpp FileIO.cpp

rand: rand.cpp crypto.cpp

tests: tests.cpp crypto_xor.cpp crypto_rand.cpp FileIO.cpp crypto_aes.cpp crypto_SHA.cpp

test: tests
	valgrind --vgdb=no -q --track-origins=yes ./tests

file: file.cpp crypto_rand.cpp FileIO.cpp

aesecb: aesecb.cpp crypto_aes.cpp FileIO.cpp crypto_rand.cpp

aescbc: aescbc.cpp crypto_aes.cpp FileIO.cpp crypto_rand.cpp

sha: sha.cpp crypto_SHA.cpp FileIO.cpp crypto_rand.cpp
# demo: aes
# 	./aes key plaintext ciphertext demo

#running instructions make p=<option> demo
demo:
ifeq ($(p),xor)    
	@make xor
	./xor 00FFAFF in.bin out.bin
else
ifeq ($(p),rand)
	@make rand
	./rand 16 1 1 1 1 1
else
ifeq ($(p),aesecb)
	@make aesecb
	./aesecb key plaintext hash demo
else
ifeq ($(p),aescbc)
	@make aescbc
	./aescbc key plaintext ciphertext demo
else
ifeq ($(p),sha)
	@make sha
	./sha plaintext ciphertext demo
else
	@echo "usage: make p=<option> demo\n      options: xor, rand, aesecb, aescbc"
endif
endif
endif
endif
endif

clean:
	rm -f *.o rand tests xor aescbc aesecb
