client.wasm: src/*.c
	@clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -Wno-macro-redefined -O3 -o $@ $^