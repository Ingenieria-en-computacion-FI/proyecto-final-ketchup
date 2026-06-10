import csv 
import random
import argparse
import sys

def generar_procesos(num_procesos, max_memoria, archivo_salida):
	procesos = []
	
	for pid in range(1, num_procesos + 1):
		#Inyectar "Edge cases "
		# 15 % de probabilidad de generar un proceso muy pesado (Memory hog)
		# Esto forzara al simulador a lidiar con la fragmentacion y la coalescencia
		if random.random() < 0.15:
			burst_time = random.randint(20, 50)
			#Solicita entre el 40% y el 80% de la memoria maxima
			memory_required = random.randint(int(max_memoria * 0.4), int(max_memoria * 0.8))
			priority = random.randint(1, 2)

		else:
			#Procesos ligeros y rapidos que llenaran huecos pequenos
			burst_time = random.randint(1, 10)
			memory_required = random.randint(10, int(max_memoria * 0.15))
			priority = random.randint(3, 5)

		#El documento expresa que un proceso requiere:
		# pid, burst_time, priority y memory_required
		# (remaining time) y state se inicializan internamente en C
		procesos.append([pid, burst_time, priority, memory_required])

	
	try:
		with open(archivo_salida, mode = 'w', newline = '') as file:
			writer = csv.writer(file)
			writer.writerows(procesos)
		print(f"[+] Exito: {num_procesos} procesos generados y guardados en '{archivo_salida}'.")

	except IOError as e:
		print(f"[-] Error al escribir el archivo {e}", file=sys.stderr)

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Generador de cargas de trabajo para simulador de OS')

	#Definicion de argumentos por linea de comandos
	parser.add_argument('-n', '--num', type=int, default=100, 
			   help='Numero de procesos a generar (default: 100)')
	parser.add_argument('-m', '--mem', type=int, default=1024, 
			   help='Memoria maxima del sistema simulado para calcular proporciones (default: 1024)')
	parser.add_argument('-o', '--out', type=str, default='procesos.csv', 
			help='Archivo de salida (default: procesos.csv)')

	args = parser.parse_args()
	generar_procesos(args.num, args.mem, args.out)

