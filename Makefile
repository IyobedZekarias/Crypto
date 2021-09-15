CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha

all: xor rand tests

xor: xor.cpp crypto_xor.cpp crypto.cpp FileIO.cpp

rand: rand.cpp crypto.cpp

tests: tests.cpp crypto_xor.cpp crypto.cpp

test: tests
	valgrind --vgdb=no -q ./tests

# demo: xor
# 	./xor 00FFAFF in.bin out.bin

demo:
ifeq ($(p),xor)
	./xor 00FFAFF in.bin out.bin
else
ifeq ($(p),rand)
	./rand 16 1 1 1 1 1
endif
endif

clean:
	rm -f *.o rand tests xor
