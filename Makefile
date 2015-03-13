EMCC=/usr/local/bin/emcc
NODE=/usr/local/bin/node
INCLUDES=include/bit_map.h include/amt.h

./dist/amt.out.js: hello_world.c $(INCLUDES)
	mkdir -p ./dist
	$(EMCC) -I./include $< -o $@

.PHONY: test
test: ./dist/amt.out.js
	$(NODE) ./dist/amt.out.js

.PHONY: clean
clean:
	rm -rf ./dist
