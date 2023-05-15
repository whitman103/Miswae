default: compress compressStubs blitzInterface blitzInterfaceStubs


PYBIND_FLAGS = -O2 -std=c++17 -Wall -shared -fPIC -undefined dynamic_lookup -I/Library/Frameworks/Python.framework/Versions/3.11/include/python3.11 -I/opt/homebrew/include -I/opt/homebrew/Frameworks/Python.framework/Headers
LIBFLAGS = -L/opt/homebrew/Frameworks/Python.framework/Versions/3.11/lib/python3.11/config-3.11-darwin -ldl -L/opt/homebrew/lib -framework CoreFoundation -lblitzwave

PYBIND_EXTENSION:=$(shell python3.11-config --extension-suffix)

blitzInterface.o: blitzInterface.cpp
	g++ -c -O3 blitzInterface.cpp -o blitzInterface.o $(PYBIND_FLAGS)

blitzInterface: blitzInterface.o
	$(CXX) $(PYBIND_FLAGS) $(LIBFLAGS) -o blitzInterface$(PYBIND_EXTENSION) blitzInterface.o -fPIC 

blitzModule:= blitzInterface

blitzInterfaceStubs: blitzInterface
	${shell python3 -c 'from pybind11_stubgen import ModuleStubsGenerator; module = ModuleStubsGenerator("${blitzModule}"); module.parse(); fp=open("${blitzModule}.pyi","w");fp.write("\n".join(module.to_lines()));fp.close()'}
	mv blitzInterface$(PYBIND_EXTENSION) ../test
	mv blitzInterface.pyi ../test

compressModule:= compress

compress: compress.cpp compressors.hpp utils.o
	$(CXX) $(PYBIND_FLAGS) $(LIBFLAGS) -o compress$(PYBIND_EXTENSION) compress.cpp utils.o -fPIC

compressStubs: compress
	${shell python3 -c 'from pybind11_stubgen import ModuleStubsGenerator; module = ModuleStubsGenerator("${compressModule}"); module.parse(); fp=open("${compressModule}.pyi","w");fp.write("\n".join(module.to_lines()));fp.close()'}
	mv compress$(PYBIND_EXTENSION) ../test
	mv compress.pyi ../test

utils.o: utils.cpp utils.hpp
	g++ -c -O3 -std=c++17 utils.cpp -o utils.o

clean:
	rm utils.o blitzInterface.cpython-311-darwin.so blitzInterface.pyi blitzInterface.o compress.cpython-311-darwin.so compress.pyi