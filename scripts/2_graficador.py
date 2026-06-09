import pandas as pd
import matplotlib.pyplot as plt

def graficar_resultados(archivo_entrada = "data/outputs/resultados.csv"):
    try:
        df = pd.read_csv(archivo_entrada)
        # 1. Limpiamos las cabeceras (nombres de las columnas)
        df.columns = df.columns.str.strip()
        # 2. Limpiamos los datos internos de la columna para quitar espacios fantasmas
        df['Estado_Final'] = df['Estado_Final'].str.strip()
    except FileNotFoundError:
        print(f"Error: No se encontro el archivo {archivo_entrada}. Corre el simulador en C primero.")
        return 

    # Separamos los procesos por su estado final
    completados = df[df['Estado_Final'] == 'COMPLETADO']
    bloqueados = df[df['Estado_Final'] == 'BLOQUEADO_MEMORIA']

    fig, ax = plt.subplots(figsize=(10, 6))

    # Graficamos procesos completados (Azul) y bloqueados por memoria (Rojo)
    ax.bar(completados['PID'].astype(str), completados['Memoria_Usada'], 
           color='steelblue', label='Completados')

    if not bloqueados.empty:
        ax.bar(bloqueados['PID'].astype(str), bloqueados['Memoria_Usada'],
               color='crimson', label='Bloqueados (FALTA RAM)')

    ax.set_xlabel('PID del Proceso')
    ax.set_ylabel('Memoria Requerida (MB)')
    ax.set_title('Uso de Memoria por Proceso y Estado de Finalizacion')
    ax.legend()
    ax.grid(axis='y', linestyle='--', alpha=0.7)

    plt.xticks(rotation=45)
    plt.tight_layout()

    # Guardamos la imagen en la carpeta de reportes
    ruta_salida = "reports/png/grafica_recursos.png"
    plt.savefig(ruta_salida, dpi=300)
    print(f"Grafica guardada como '{ruta_salida}'.")

if __name__ == "__main__":
    graficar_resultados()
