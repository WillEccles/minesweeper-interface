INCLUDEPATH=-I "../src" 
CXXFLAGS=-std=c++14 -c iminesweeper_Game.cpp
LIBFLAGS=-std=c++14

ifeq ($(OS),Windows_NT)
	#windows is the OS, do stuff here
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		#include path for mac os
		INCLUDEPATH+=-I "$(shell /usr/libexec/java_home)/include" -I "$(shell /usr/libexec/java_home)/include/darwin"
		LIBFLAGS+=-dynamiclib -o libminesweeper.jnilib iminesweeper_Game.cpp ../src/iminesweeper.cpp
	endif
	ifeq ($(UNAME_S),Linux)
		#linux, do stuff here
	endif
endif

all:
	$(CXX) $(INCLUDEPATH) $(CXXFLAGS)
	$(CXX) $(INCLUDEPATH) $(LIBFLAGS)

