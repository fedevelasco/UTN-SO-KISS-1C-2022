# Libraries
LIBS=static commons m cunit readline pthread
# static commons pthread m cunit readline

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static
# /home/utnso/Escritorio/TP Operativos repo/tp-2022-1c-Grupo-TP-SO/static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-Grupo-TP-SO/cpu/configuracion.config /home/utnso/tp-2022-1c-Grupo-TP-SO/cpu/ips.config

#Argumentos para pruebas
BASE=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
PLANIFICACION=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/PLANI_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
MEMORIA=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/MEMORIA_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
TLB=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/TLB_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_CPU.cfg /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log"