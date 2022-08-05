# Libraries
LIBS=static commons pthread m cunit readline

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-Grupo-TP-SO/ram_swap/ram_swap.config /home/utnso/tp-2022-1c-Grupo-TP-SO/ram_swap/ips.config

#Argumentos para pruebas
BASE=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
PLANIFICACION=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/PLANI_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
MEMORIA=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/MEMORIA_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
TLB=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/TLB_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_MEMORIA.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log" -v
