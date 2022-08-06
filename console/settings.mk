# Libraries
LIBS=static commons cunit readline pthread

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=../static

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_1 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

# Argunmentos para las pruebas
BASE1=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_1 64 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
BASE2=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/BASE_2 128 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
PLANIFICACION1=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/PLANI_1 64 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
PLANIFICACION2=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/PLANI_2 128 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION1=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_1 64 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION2=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_2 128 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
SUSPENSION3=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/SUSPE_3 192 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
MEMORIA=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/MEMORIA_1 4096 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
TLB1=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/TLB_1 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
TLB2=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/TLB_2 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL1=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_1 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL2=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_2 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL3=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_3 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL4=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_4 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg
INTEGRAL5=/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/INTEGRAL_5 2048 /home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/IP.cfg

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log" --show-leak-kinds=all
HELGRIND_FLAGS=--log-file="helgrind.log"
