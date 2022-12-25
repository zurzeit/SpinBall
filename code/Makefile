CXX = g++
PYBIND = $(shell python3 -m pybind11 --includes)
PYTHON_INC = $(shell python3-config --includes)
EXT_SFX = $(shell python3-config --extension-suffix)
DEBUG = DEBUG
.PHONY: all
all: main_project.cpp matrix_vector.cpp
	# g++ main_project.cpp -o main_project.exe -std=c++11
	$(CXX) -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND) main_project.cpp -o _spinball$(EXT_SFX) $(PYTHON_INC)
# matrix_vector.o: matrix_vector.cpp
# 	g++ -c matrix_vector.cpp -o matrix_vector.o -std=c++17

.PHONY: main
main: main_project.cpp matrix_vector.cpp
	g++ main_project.cpp -o main_project.exe -std=c++17 "-D$(DEBUG)"
	./main_project.exe
.PHONY: clean
clean:
	rm -rf *.o *.exe *.so
.PHONY: test
test:
	python3 test_spinball.py
test2:
	python3 test2.py
states: states_test.cpp
	g++ states_test.cpp -o states_test.exe  -std=c++17 "-DDEBUG"
	./states_test.exe
	# $(CXX) -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND) test3.cpp -o _states$(EXT_SFX) $(PYTHON_INC)
.PHONY: processing
processing:
	python3 drag_lift_processing.py

viz:
	python3 viz_tool.py
