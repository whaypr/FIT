CXX      = gcc
CXXFLAGS = -Wall -pedantic -g
LIBS     = -lcrypto -lm
OUT      = exec

SRC     := $(shell find . -name '*.c')

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
	