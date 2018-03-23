SRCDIR=src
TESTDIR=tests
SRCH=$(wildcard $(SRCDIR)/*.h)
SRC=$(wildcard $(SRCDIR)/*.cpp)
INCLUDEDIR=src

CXXFLAGS=-std=c++14 -I$(INCLUDEDIR)

CPPTESTFILES=$(wildcard $(TESTDIR)/cpp/*.cpp)

cpptest: $(SRCH) $(SRC) $(CPPTESTFILES)
	$(CXX) $(CXXFLAGS) $(CPPTESTFILES) $(SRC) -o cpptest

javatest: tests/java/Test.java
	cd tests/java/
	javac Test.java

all: cpptest javatest

clean:
	rm -f cpptest
