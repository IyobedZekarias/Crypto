CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha

rand: rand.cpp crypto.cpp

tests: tests.cpp crypto.cpp

test: tests
	valgrind --vgdb=no -q ./tests

demo: rand
	./rand 16 1 1 1 1 1

clean:
	rm -f *.o rand tests
