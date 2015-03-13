EMCC=/usr/local/bin/emcc
NODE=/usr/local/bin/node
INCLUDES=include/bit_map.h

a.out.js: hello_world.c $(INCLUDES)
	$(EMCC) -I./include hello_world.c

.PHONY: test
test: a.out.js
	$(NODE) a.out.js
