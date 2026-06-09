CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude -g

SRC_DIRS = src src/algorithms src/memory src/scheduler src/utils

SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

TESTS = $(wildcard tests/*.c)

all: clean build

build:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o bin/main

run: build
	./bin/main

test: clean
	mkdir -p bin
	$(CC) $(CFLAGS) $(TESTS) $(filter-out src/main.c, $(SRC)) -o bin/tests
	./bin/tests

# NUEVO FLUJO AUTOMATIZADO
simulate: build
	@echo "\n=== 0. PREPARANDO ESTRUCTURA DE CARPETAS ==="
	mkdir -p data/inputs data/outputs data/logs reports/png
	@echo "\n=== 1. GENERANDO CARGA EN DATA/INPUTS ==="
	python3 scripts/1_generador.py
	@echo "\n=== 2. EJECUTANDO KERNEL DEL SIMULADOR ==="
	./bin/main
	@echo "\n=== 3. GENERANDO GRÁFICAS EN REPORTS/PNG ==="
	python3 scripts/2_graficador.py
	@echo "\n=== 4. ANALIZANDO TENDENCIAS DE CPU ==="
	python3 scripts/3_analisis.py
	@echo "\n=== SIMULACIÓN EN CADENA CONCLUIDA ==="

clean:
	rm -rf bin/*
