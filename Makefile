all:
	mkdir build; cd build; cmake ../CMakeLists.txt; make
clean:
	rm -rf build