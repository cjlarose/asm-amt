EMCC=/usr/local/bin/emcc
CC=/usr/bin/g++
CCFLAGS=-std=c++0x -I./include -Wall -Wextra -Werror -pedantic
NODE=/usr/local/bin/node
INCLUDES=include/bit_map.hpp include/amt.hpp
IDL_BINDER=/usr/local/bin/python /usr/local/Cellar/emscripten/1.29.6/libexec/tools/webidl_binder.py
BUILD_DIR=./build

./dist/amt.out.js: glue_wrapper.cpp $(INCLUDES) $(BUILD_DIR)/glue.js
	mkdir -p ./dist
	$(EMCC) -std=c++0x -I./include -I$(BUILD_DIR) --post-js $(BUILD_DIR)/glue.js -o $@ $<

$(BUILD_DIR)/glue.cpp $(BUILD_DIR)/glue.js: amt.idl
	mkdir -p $(BUILD_DIR)
	$(IDL_BINDER) $< $(BUILD_DIR)/glue

test_trie.out: ./test/trie.cpp $(INCLUDES)
	$(CC) $(CCFLAGS) -g -I./test/include -o $@ $<

test_bitmap.out: ./test/bit_map.cpp $(INCLUDES)
	$(CC) $(CCFLAGS) -g -I./test/include -o $@ $<

.PHONY: test-js
test-js: ./dist/amt.out.js
	./node_modules/.bin/mocha test-js/*.js

.PHONY: test-native
test-native: test_trie.out test_bitmap.out
	./test_trie.out
	./test_bitmap.out

.PHONY: test
test: test-native test-js

.PHONY: clean
clean:
	rm -rf ./dist $(BUILD_DIR) *.out
