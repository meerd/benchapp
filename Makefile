WORKING_DIRECTORY := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

all:
	(cd $(WORKING_DIRECTORY) && mkdir -p build/ && cd build/ && cmake .. && make)

run:
	$(WORKING_DIRECTORY)/build/benchapp	
