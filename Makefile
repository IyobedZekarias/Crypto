#CPPFLAGS = -g -std=gnu++17 -Wall -Wextra -Wold-style-cast -Werror -Wshadow -Wconversion -mrdseed -mrdrnd -maes -msha

LibFolder = bin
LibName = crypto_iz
ROOT_DIR:=$(abspath $(lastword $(MAKEFILE_LIST)))
dir:=$(subst Makefile,$(LibFolder),$(ROOT_DIR))
space := $(subst ,, )
rmsp:=$(subst $(space),\$(space),$(dir))

#CPPFLAGS = -g -std=gnu++17 -Wold-style-cast -Werror -Wconversion -Wshadow -mrdseed -mrdrnd -maes -msha -fno-builtin
CPPFLAGS = -g -std=gnu++17 -mrdseed -mrdrnd -maes -msha -fpermissive
Lib = -L$(rmsp)


objs: xor.o rand.o aes.o FileIO.o sha.o rsa.o NNI.o dh.o

lib: bin/crypto_xor.o bin/crypto_rand.o bin/crypto_aes.o \
bin/FileIO.o bin/crypto_sha.o bin/crypto_rsa.o bin/NNI.o bin/crypto_dh.o
	g++ -shared $^ -o bin/lib$(LibName).so  $(Lib) -Wl,-rpath=$(rmsp) -Wl,--whole-archive -lfftw3l -Wl,--no-whole-archive
# $(Lib) -Wl,-rpath=$(rmsp) -Wl,--whole-archive -lfftw3f -Wl,--no-whole-archive

xor.o: src/crypto_xor.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@

rand.o: src/crypto_rand.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@

aes.o: src/crypto_aes.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@

FileIO.o: src/FileIO.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/$@

rsa.o: src/crypto_rsa.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@

sha.o: src/crypto_sha.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@

NNI.o: src/NNI.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/$@

dh.o: src/crypto_dh.cpp
	g++ -c -fPIC $(CPPFLAGS) $^ -o bin/crypto_$@



PATH_TO_FILE = bin/lib$(LibName).so
xor: progs/xor.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s xor.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif
	

rand: progs/rand.cpp 
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s rand.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

file: progs/file.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s FileIO.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

aes:
	@echo "usage: you need to make either aesecb or aescbc"

aesecb: progs/aesecb.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s aes.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

aescbc: progs/aescbc.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s aes.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

sha: progs/sha.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s sha.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

rsa: progs/rsa.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s NNI.o
	@make -s rsa.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

nni: progs/nni.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s NNI.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

dh: progs/dh.cpp
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	@make -s dh.o
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName)
endif

tests: progs/tests.cpp 
ifneq ("$(wildcard $(PATH_TO_FILE))","")
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName) 
else
	@make -s objs
	@make -s lib
	g++ $(Lib) -Wl,-rpath=$(rmsp) $(CPPFLAGS) -o $@ $^ -l$(LibName) 
endif

test: tests 
	valgrind --vgdb=no -q --track-origins=yes ./tests
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
	./sha plaintext hashtext demo
else
ifeq ($(p),shat)
	@make sha
	./sha plaintext hashtext demo t
else
ifeq ($(p),rsa)
	@make rsa
	./rsa plaintext ciphertext
else
	@echo "usage: make p=<options> demo\n      options: xor, rand, aesecb, aescbc, sha"
endif
endif
endif
endif
endif
endif
endif

clean:
	rm -f bin/*.o bin/*.so rand tests xor aescbc aesecb sha rsa dh nni
