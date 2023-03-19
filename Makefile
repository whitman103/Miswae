default: compress


compress: compressors.o compress.cpp io.o
	g++ -O3 -std=c++17 compress.cpp compressors.o io.o -o compress

io.o: io.cpp io.hpp
	g++ -c -O3 -std=c++17 io.cpp -o io.o

compressors.o: compressors.cpp compressors.hpp io.hpp io.o
	g++ -c -O3 -std=c++17 compressors.cpp io.o -o compressors.o

clean:
	rm compressors.o compress