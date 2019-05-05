WORKING_DIRECTORY := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CMAKE_STD_BUILD=mkdir -p build/ && cd build/ && cmake .. && make

.PHONY: all _config wjcryptlib menuconfig menuconfig_build run


all: _config wjcryptlib
	cd $(WORKING_DIRECTORY)
	([ ! -f .config ] && cp ./configs/benchapp_defconfig .config;) 2>/dev/null; true
	(mkdir -p build/ && cd build/ && cmake -DBENCHAPP_CONFIG='$(BENCHAPP_CONFIG)' .. && make)

_config:
include .config
$(foreach v, $(filter CONFIG_%,$(.VARIABLES)), $(eval BENCHAPP_CONFIG += -D$(v)=$($(v))))
#$(info $(BENCHAPP_CONFIG))

wjcryptlib:	
	(cd $(WORKING_DIRECTORY)/external/WjCryptLib && $(CMAKE_STD_BUILD))

menuconfig_build:
	(cd $(WORKING_DIRECTORY)/tools/menuconfig && $(CMAKE_STD_BUILD))

menuconfig: menuconfig_build
	(cd $(WORKING_DIRECTORY) && ./tools/menuconfig/build/mconf configs/Config)

run:
	$(WORKING_DIRECTORY)/build/benchapp	
