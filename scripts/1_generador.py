import csv
import random

def generador_procesos(cantidad=25, archivo_salida='Procesos.csv'):	
	with open(archivo_salida, mode = 'w', newline = '') as file:
		writer = csv.writer(file)
		writer.writerow(["PID", "Burst_Time", "Priority", "Memory_MB"])
		
		for pid in range(1, cantidad+1):
			#Simulamos rafagas y cantidades realistas
			burst = random.randint(1, 15)
			prio = random.randint(1, 5)
			
			#Asignamos bloques de memoria en potencias de 2 (tipico en SO) o tamanos arbitrarios
			mem_options = [32, 64, 128, 256, 512]
			mem = random.choice(mem_options)
			
			writer.writerow([pid, burst, prio, mem])

	print(f"[{archivo_salida}] Generados {cantidad} procesos correctamente.")

if __name__ == "__main__":
	generador_procesos(25)
