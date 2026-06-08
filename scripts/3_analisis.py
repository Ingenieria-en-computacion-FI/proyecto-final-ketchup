import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

def analizar_tendencias(archivo_entrada="resultados.csv"):
    try:
        df = pd.read_csv(archivo_entrada)
        df.columns = df.columns.str.strip()
        df['Estado_Final'] = df['Estado_Final'].str.strip() # Solución al ValueError
    except FileNotFoundError:
        print("Error: Resultados no encontrados.")
        return

    plt.figure(figsize=(8, 6))

    sns.scatterplot(
        data=df,
        x='Burst_Original',
        y='Memoria_Usada',
        hue='Estado_Final',
        palette={'COMPLETADO': 'green', 'BLOQUEADO_MEMORIA': 'red'},
        s=100,
        alpha=0.8
    )
    
    plt.title('Analisis de Tendencias: Tiempo de CPU vs Consumo de Memoria')
    plt.xlabel('Rafaga de CPU (Unidades de Tiempo)')
    plt.ylabel('Memoria Requerida (MB)')
    plt.grid(True, linestyle=':', alpha=0.6)

    plt.savefig("analisis_dispersion.png", dpi=300)
    print("Analisis estadistico guardado como 'analisis_dispersion.png'. ")

if __name__ == "__main__":
    analizar_tendencias()
