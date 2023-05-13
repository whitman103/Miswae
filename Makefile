default: compress pyBlitz


CXXFLAGS = -O2 -std=c++17 -Wall -shared -fPIC -undefined dynamic_lookup -I/Library/Frameworks/Python.framework/Versions/3.11/include/python3.11 -I/opt/homebrew/include -I/opt/homebrew/Frameworks/Python.framework/Headers
LIBFLAGS = -L/opt/homebrew/Frameworks/Python.framework/Versions/3.11/lib/python3.11/config-3.11-darwin -ldl -L/opt/homebrew/lib -framework CoreFoundation -lblitzwave

blitzTry.o: blitzTry.cpp
	g++ -c -O3 blitzTry.cpp -o blitzTry.o $(CXXFLAGS)

pyBlitz: blitzTry.o
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -o blitzTry.cpython-311-darwin.so blitzTry.o -fPIC 

compress: compressors.o compress.cpp utils.o
	g++ -g -O0 -std=c++17 compress.cpp utils.o -o compress

utils.o: utils.cpp utils.hpp
	g++ -c -O3 -std=c++17 utils.cpp -o utils.o


compressors.o: compressors.cpp compressors.hpp io.hpp utils.o
	g++ -c -O3 -std=c++17 compressors.cpp utils.o -o compressors.o

clean:
	rm compressors.o compress utils.o blitzTry.cpython-311-darwin.so blitzTry.o