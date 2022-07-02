CXX      = gcc
CXXFLAGS = -Wall -pedantic -g
LIBS     = -lm -lcrypto
OUT      = exec

SRC     := $(shell find . -name '*.c')

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
	