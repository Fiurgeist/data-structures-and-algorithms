.PHONY: build-c-debug
build-c-debug:
	mkdir -p ./build/$(folder)
	gcc -std=c99 -Wno-discarded-qualifiers -g $$(cat ./$(folder)/compile) -o ./build/$(folder)/run

.PHONY: run-c
run: build-c-debug
	./build/$(folder)/run

.PHONY: help
help:
	@echo "Please use 'make <target>' where <target> is one of"
	@echo "  build-c-debug           build C code"
	@echo "  run-c                   run C code"
