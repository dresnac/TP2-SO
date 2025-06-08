#!/bin/bash

# Verificar modo de compilación
if [ -z "$1" ] || [ "$1" == "from_scratch" ]; then
    TARGET="all"
elif [ "$1" == "buddy" ]; then
    TARGET="buddy"
else
    echo "Uso: ./compile.sh [from_scratch|buddy]"
    echo " - from_scratch (por defecto): usa el manejador de memoria básico"
    echo " - buddy: usa el manejador de memoria tipo buddy"
    exit 1
fi

# Ejecutar el build
if [ "$TARGET" == "buddy" ]; then
    echo "Compilando en modo: buddy"
else
    echo "Compilando en modo: from_scratch"
fi

sudo docker start TP2_SO

sudo docker exec -it TP2_SO make clean -C /root/Toolchain
sudo docker exec -it TP2_SO make clean -C /root/

sudo docker exec -it TP2_SO make -C /root/Toolchain
sudo docker exec -it TP2_SO make -C /root/ $TARGET

sudo docker stop TP2_SO
sudo chmod 777 Image/x64BareBonesImage.qcow2
