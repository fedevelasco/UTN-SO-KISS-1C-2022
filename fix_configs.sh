#!/bin/bash

for i in "/home/utnso/tp-2022-1c-Grupo-TP-SO/pruebas/"
do
  echo -e "\n\nFixing config ${i}\n\n"
  dos2unix $i $i
done