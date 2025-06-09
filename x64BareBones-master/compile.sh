#!/bin/bash

# --- Parámetros ---
MODE=$1
PVS=$2

# --- Validación del modo de compilación ---
if [ -z "$MODE" ] || [ "$MODE" == "from_scratch" ]; then
    TARGET="all"
elif [ "$MODE" == "buddy" ]; then
    TARGET="buddy"
else
    echo "Uso: ./compile.sh [from_scratch|buddy] [--pvs]"
    echo " - from_scratch (por defecto): usa el manejador de memoria básico"
    echo " - buddy: usa el manejador de memoria tipo buddy"
    echo " - --pvs (opcional): realiza análisis estático con PVS-Studio"
    exit 1
fi

# --- Mostrar modo de compilación ---
if [ "$TARGET" == "buddy" ]; then
    echo "Compilando en modo: buddy"
else
    echo "Compilando en modo: from_scratch"
fi

# --- Iniciar contenedor ---
sudo docker start TP2_SO

# --- Limpieza y compilación normal ---
sudo docker exec -it TP2_SO make clean -C /root/Toolchain
sudo docker exec -it TP2_SO make clean -C /root/
sudo docker exec -it TP2_SO make -C /root/Toolchain

# --- Compilación normal o trazada ---
if [ "$PVS" == "--pvs" ]; then
    echo "Ejecutando compilación con trazado para PVS-Studio..."
    sudo docker exec -it TP2_SO bash -c "pvs-studio-analyzer trace -- make -C /root/ $TARGET"
    
    echo "Ejecutando análisis estático con PVS-Studio..."
    sudo docker exec -it TP2_SO mkdir -p /root/pvs-output
    sudo docker exec -it TP2_SO chmod -R 777 /root/pvs-output
    sudo docker exec -it TP2_SO bash -c "pvs-studio-analyzer analyze -o /root/pvs-output/report.log"
    sudo docker exec -it TP2_SO bash -c "plog-converter -a GA:1,2 -t html -o /root/report_html.html /root/pvs-output/report.log"

    echo "Análisis completado. Los resultados están en /root/pvs-output dentro del contenedor."
else
    sudo docker exec -it TP2_SO make -C /root/ $TARGET
fi

# --- Finalizar ---
sudo docker stop TP2_SO
sudo chmod 777 Image/x64BareBonesImage.qcow2