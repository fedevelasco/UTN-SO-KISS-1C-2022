# Libraries
LIBS=static commons cunit readline pthread
# static commons pthread m cunit readline

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static
# /home/utnso/Escritorio/TP Operativos repo/tp-2022-1c-Grupo-TP-SO/static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=1001 ../console/instructions.txt

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log"