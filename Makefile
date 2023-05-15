default: compress compressStubs pyBlitz pyBlitzStubs


PYBIND_FLAGS = -O2 -std=c++17 -Wall -shared -fPIC -undefined dynamic_lookup -I/Library/Frameworks/Python.framework/Versions/3.11/include/python3.11 -I/opt/homebrew/include -I/opt/homebrew/Frameworks/Python.framework/Headers
LIBFLAGS = -L/opt/homebrew/Frameworks/Python.framework/Versions/3.11/lib/python3.11/config-3.11-darwin -ldl -L/opt/homebrew/lib -framework CoreFoundation -lblitzwave

blitzTry.o: blitzTry.cpp
	g++ -c -O3 blitzTry.cpp -o blitzTry.o $(PYBIND_FLAGS)

pyBlitz: blitzTry.o
	$(CXX) $(PYBIND_FLAGS) $(LIBFLAGS) -o blitzTry.cpython-311-darwin.so blitzTry.o -fPIC 

blitzModule:= blitzTry

pyBlitzStubs: pyBlitz
	${shell python3 -c 'from pybind11_stubgen import ModuleStubsGenerator; module = ModuleStubsGenerator("${blitzModule}"); module.parse(); fp=open("${blitzModule}.pyi","w");fp.write("\n".join(module.to_lines()));fp.close()'}

compressModule:= compress

compress: compress.cpp compressors.hpp utils.o
	$(CXX) $(PYBIND_FLAGS) $(LIBFLAGS) -o compress.cpython-311-darwin.so compress.cpp utils.o -fPIC

compressStubs: compress
	${shell python3 -c 'from pybind11_stubgen import ModuleStubsGenerator; module = ModuleStubsGenerator("${compressModule}"); module.parse(); fp=open("${compressModule}.pyi","w");fp.write("\n".join(module.to_lines()));fp.close()'}

utils.o: utils.cpp utils.hpp
	g++ -c -O3 -std=c++17 utils.cpp -o utils.o

clean:
	rm utils.o blitzTry.cpython-311-darwin.so blitzTry.o compress.cpython-311-darwin.so