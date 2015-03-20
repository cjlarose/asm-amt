EMCC=/usr/local/bin/emcc
CC=/usr/bin/g++
NODE=/usr/local/bin/node
INCLUDES=include/bit_map.hpp include/amt.hpp

./dist/amt.out.js: hello_world.cpp $(INCLUDES)
	mkdir -p ./dist
	$(EMCC) -std=c++0x -I./include $< -o $@

./dist/amt.out: hello_world.cpp $(INCLUDES)
	mkdir -p ./dist
	$(CC) -std=c++0x -I./include $< -o $@

.PHONY: test
test: ./dist/amt.out.js
	$(NODE) ./dist/amt.out.js

.PHONY: test-native
test-native: ./dist/amt.out
	./dist/amt.out

.PHONY: clean
clean:
	rm -rf ./dist
