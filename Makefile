EMCC=/usr/local/bin/emcc
CC=/usr/bin/g++
NODE=/usr/local/bin/node
INCLUDES=include/bit_map.hpp include/amt.hpp
IDL_BINDER=/usr/local/bin/python /usr/local/Cellar/emscripten/1.29.6/libexec/tools/webidl_binder.py
BUILD_DIR=./build

./dist/amt.out.js: glue_wrapper.cpp $(INCLUDES) $(BUILD_DIR)/glue.js
	mkdir -p ./dist
	$(EMCC) -std=c++0x -I./include -I$(BUILD_DIR) --post-js $(BUILD_DIR)/glue.js -o $@ $<

./dist/amt.out: hello_world.cpp $(INCLUDES)
	mkdir -p ./dist
	$(CC) -std=c++0x -I./include $< -o $@

$(BUILD_DIR)/glue.cpp $(BUILD_DIR)/glue.js: amt.idl
	mkdir -p $(BUILD_DIR)
	$(IDL_BINDER) $< $(BUILD_DIR)/glue

.PHONY: test
test: ./dist/amt.out.js
	$(NODE) ./dist/amt.out.js

.PHONY: test-native
test-native: ./dist/amt.out
	./dist/amt.out

.PHONY: clean
clean:
	rm -rf ./dist $(BUILD_DIR)
