# Libraries
LIBS=static commons cunit readline pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/kernel/config/kernel.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/kernel/config/ip.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log"
