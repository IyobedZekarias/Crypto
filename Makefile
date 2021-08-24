CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha

rand: rand.cpp crypto.cpp

tests: tests.cpp crypto.cpp

test: tests
	valgrind -q ./tests

clean:
	rm -f *.o rand tests
