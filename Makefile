### Makefile that delegates building either to cmake or legacy Makefile 
### Cmake is default, if you don't want to use CMake, you can do make legacy   
### ( or move this file and rename Makefile.legacy to Makefile,
###  or modify the all/clean/install targets below  ) 
###


.PHONY: all configure clean cleaner install cmake-build cmake-clean cmake-install

all: cmake-build 
clean: cmake-clean
install: cmake-install 


### TODO add doxygen into CMakelists 
doc: legacy-doc

cmake-build: build/Makefile 
	@+make -C  ./build

legacy-doc: 
	@make -f Makefile.legacy doc 

configure: build/Makefile 
	@ccmake . build 

cmake-install: 
	@make -C ./build install 

build/Makefile: 
	@echo "Setting up cmake build."
	@mkdir -p build 
#	rm -rf bin
	@cd build && cmake ../ 
#	ln -sf build/bin bin 

distclean: 
	@echo "Removing cmake directory" 
	@rm -rf build 

cmake-clean: build/Makefile 
	@make -C ./build clean 

