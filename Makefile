WORKING_DIRECTORY := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CMAKE_STD_BUILD=mkdir -p build/ && cd build/ && cmake .. && make

.PHONY: all b_prepare b_config wjcryptlib menuconfig menuconfig_build run

all: b_config wjcryptlib
	(mkdir -p build/ && cd build/ && cmake -DBENCHAPP_CONFIG='$(BENCHAPP_CONFIG)' .. && make)

b_prepare:
	cd $(WORKING_DIRECTORY)
	([ ! -f .config ] && cp ./configs/benchapp_defconfig .config;) 2>/dev/null; true

b_config: b_prepare
include .config
$(foreach v, $(filter CONFIG_%,$(.VARIABLES)), $(eval BENCHAPP_CONFIG += -D$(v)=$($(v))))
#$(info $(BENCHAPP_CONFIG))

wjcryptlib:	
	(cd $(WORKING_DIRECTORY)/external/WjCryptLib && $(CMAKE_STD_BUILD))

menuconfig_build: 
	(cd $(WORKING_DIRECTORY)/tools/menuconfig && $(CMAKE_STD_BUILD))

menuconfig: b_prepare menuconfig_build
	(cd $(WORKING_DIRECTORY) && ./tools/menuconfig/build/mconf configs/Config)

run:
	$(WORKING_DIRECTORY)/build/benchapp

distclean:
	rm -rf $(WORKING_DIRECTORY)/build
	rm -rf $(WORKING_DIRECTORY)/external/WjCryptLib/build	
