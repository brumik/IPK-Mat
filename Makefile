C++ = g++
CPPFLAGS = -Wall -Wextra -pedantic -std=c++11

DEPHEADS = Connection.h

DEPS = ipk-client.cpp Connection.cpp

EXES = ipk-client
ZIPFILES = *.cpp *.h *.md
ZIPNAME =xberky02.tgz

all: $(EXES)

ipk-client: $(DEPHEADS) $(DEPS)
	$(C++) $(CPPFLAGS) -o $@ $^ -lcrypto

.PHONY: clean zip

clean:
	rm -f $(EXES) $(ZIPNAME)

zip:
	tar -zcvf $(ZIPNAME) $(ZIPFILES) Makefile
