#!/bin/bash
#SBATCH -J QR-CUDA
#SBATCH -e log.err.%j
#SBATCH -o log.out.%j
#SBATCH -n 1                  # Prozesse
#SBATCH -c 1                  # Kerne pro Prozess
#SBATCH --mem-per-cpu=1800    # Hauptspeicher in MByte pro Rechenkern
#SBATCH -t 00:05:00           # in Stunden und Minuten, oder '#SBATCH -t 10' - nur Minuten
#SBATCH -p kurs1      

# -------------------------------
# Anschliessend schreiben Sie Ihre eigenen Befehle, wie z.B.
module load gcc
module load cuda
./qr_template 100 200
./qr_template 200 200
./qr_template 300 400
./qr_template 400 300
./qr_template 600 600
