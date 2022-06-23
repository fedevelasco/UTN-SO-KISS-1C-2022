build:
	make  -C console
	make  -C server

debug:
	make debug -C console
	make debug -C server

clean:
	make clean -C console
	make clean -C server
