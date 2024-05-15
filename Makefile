.PHONY: build-c-debug
build-c-debug:
	mkdir -p ./build/$(folder)
	gcc -std=c99 -Wno-discarded-qualifiers -g $$(cat ./$(folder)/compile) -o ./build/$(folder)/run

.PHONY: run-c
run-c: build-c-debug
	./build/$(folder)/run

.PHONY: test-zig
test-zig:
	zig build test

.PHONY: install-go
install-go:
	go get ./...
	go mod tidy

.PHONY: run-go
run-go:
	go run -race $(file)

.PHONY: help
help:
	@echo "Please use 'make <target>' where <target> is one of"
	@echo "  build-c-debug           build C code"
	@echo "  run-c [folder=SOURCES]  run C code"
	@echo "  test-zig                test Zig code"
	@echo "  install-go              install Go dependencies"
	@echo "  run-go [file=MAIN]      run Go code"
