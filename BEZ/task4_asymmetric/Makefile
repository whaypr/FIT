CXX      = g++
CXXFLAGS = -Wall -pedantic -g
LIBS     = -lcrypto

OUT_SEAL = seal
OUT_OPEN = open
FILE     = testfile


all: seal open

seal: seal.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(OUT_SEAL) $(LIBS)

open: open.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(OUT_OPEN) $(LIBS)

run: seal open
	./testall.sh "$(OUT_SEAL)" "$(OUT_OPEN)" "$(FILE)"

clean:
	rm -f $(OUT_SEAL)
	rm -f $(OUT_OPEN)
	rm -f files/$(FILE)_sealed
	rm -f files/$(FILE)_sealed_opened