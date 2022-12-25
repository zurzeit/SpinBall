CXX = g++
PYBIND = $(shell python3 -m pybind11 --includes)
PYTHON_INC = $(shell python3-config --includes)
EXT_SFX = $(shell python3-config --extension-suffix)
DEBUG = DEBUG
.PHONY: all
all: main_project.cpp
	$(CXX) -O3 -Wall -shared -std=c++17 -fPIC $(PYBIND) main_project.cpp -o _spinball$(EXT_SFX) $(PYTHON_INC)

.PHONY: clean
clean:
	rm -rf *.o *.exe *.so __pycache__ .pytest_cache
.PHONY: test
test:
	python3 -m pytest

viz:
	python3 viz_example.py
	
setup: requirements.txt
	pip install -r requirements.txt
	
