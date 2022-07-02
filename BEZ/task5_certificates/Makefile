CXX      = g++
CXXFLAGS = -Wall -pedantic -g
LIBS     = -lssl -lcrypto

OUT  = exec
FILEDIR = files


$(OUT): main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(OUT) $(LIBS)

run: ${OUT}
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f "$(FILEDIR)"/*
