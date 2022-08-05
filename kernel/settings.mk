# Libraries
LIBS=static commons cunit readline pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

#Argumentos pruebas
BASE=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
PLANIFICACION=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/PLANI_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
MEMORIA=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/MEMORIA_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
TLB=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/TLB_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL=/home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_KERNEL.cfg /home/utnso/shared/git/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log"
