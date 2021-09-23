CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha
#CPPFLAGS = -g -std=gnu++17 -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha

all: xor rand tests aes

xor: xor.cpp crypto_xor.cpp crypto_rand.cpp FileIO.cpp

rand: rand.cpp crypto.cpp

tests: tests.cpp crypto_xor.cpp crypto_rand.cpp FileIO.cpp crypto_aes.cpp 

test: tests
	valgrind --vgdb=no -q ./tests

file: file.cpp crypto_rand.cpp FileIO.cpp

aes: aes.cpp crypto_aes.cpp FileIO.cpp crypto_rand.cpp

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
ifeq ($(p),aes)
	@make aes
	./aes key plaintext ciphertext demo
else
	@echo "usage: make p=<option> demo\n      options: xor, rand, aes"
endif
endif
endif

clean:
	rm -f *.o rand tests xor file aes
