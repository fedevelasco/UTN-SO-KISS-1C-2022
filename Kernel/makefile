# VARIABLES

CC = gcc            # Compilador
OUTFILE = Kernel.out

SRC = $(shell find ./src -type f -iname ".c" -exec basename {} ;) # guardo en SRC todos los archivos de src en una lista
OBJ = $(SRC:%.c=./obj/%.o)                    # OBJ variable que tiene todos los archivos .c y les hace sus archivos .o


# REGLAS

build: makefolder $(OBJ)                       # Siempre chequea makefolder
	$(CC) -o $(OUTFILE) logKernel.c $(OBJ)          # COMPILA TODO

LINK = -lpthread -lcommons -lm
CFLAGS = -c                                    # Es para el debugger con -c

debug: CFLAGS := $(CFLAGS) -g                  # SE le asigna el flag -g al debug
debug: build


./obj/%.o: ./src/%.c                           # Regla para compilar los .o
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)        # Por cada OBJ se fija que esten actualizados y ejecuta
                                               # $< = ./src/%.c        $@ = ./obj/%.o

makefolder:
	if [ ! -d ./obj ]; then mkdir obj; fi      # Evalua si existe la carpeta obj


# CLEAN

clean:                                          # Elimina archivos binarios y memory leaks
	rm -rf obj
	rm -rf.out