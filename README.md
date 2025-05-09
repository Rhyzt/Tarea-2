# Spotifind

## Descripción

Esta aplicacion permite filtrar canciones respecto al genero de estas, el nombre del artista o la velocidad (tempo), de datasets de canciones previamente cargados. La aplicacion permite cargar datasets de canciones, mostrar canciones con un genero especifico, mostrar canciones en las cuales este un artista en especifico y buscar canciones dependiendo de la velocidad ingresada (lenta, moderada, rapida). Creada con el fin de facilitar la gestion en centros de servicio tecnico.

## Cómo compilar y ejecutar
Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal (`main.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando:
        
        ```bash
        gcc *.c -o Spotifind
        ```
        
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```
        ./Spotifind
        ```
        

## Funcionalidades

### Funcionando correctamente:
Asumiendo que el usuario nunca comete errores y entrega datos pertinentes a la app (ej: entrega un numero del 1 al 6 en un menu de 6 opciones, no entrega palabras cuando se piden numeros, etc). Ademas, solo se podran filtrar correctamente si las palabras a buscar estan conformadas por caracteres pertenecientes a la tabla ASCII [Vease](https://www.ascii-code.com)

- Cargar canciones de un dataset, que tenga 20 columnas correspondientes a: id,track,artists,album_name,track_name,popularity,duration_ms,explicit,danceability,energy,key,loudness,mode,speechiness,acousticness,instrumentalness,liveness,valence,tempo,time_signature,track_genre (la primera linea se saltara ya que se asume que posee los nombres de las columnas)
- Buscar y mostrar canciones segun el genero dado (se distinguen mayusculas de minusculas, type-sensitive).
- Buscar y mostrar canciones segun el artista dado (se distinguen mayusculas de minusculas, type-sensitive).
- Buscar y mostrar canciones segun la velocidad dada (lentas , moderadas o rapidas).

### A mejorar:

- Para los generos, hacer que no sea type-sensitive.
- Hay algunos strings que son estaticos, seria mas flexibles poder usarlos con memoria dinamica.
=======


## Ejemplo de uso

**Opcion 1: Cargar la lista de canciones**

Se carga un nuevo dataset con canciones.

```
Opción seleccionada: 1) Cargar Canciones
Ingrese la direccion del archivo CSV: /data/archivo.csv
```

La aplicacion almacenara en memoria toda la informacion relevante (id, artists, album_name, track_name, tempo y track_genre) para luego que esta se pueda filtrar con las opciones disponibles.

**Opcion 2: Filtrar canciones por genero**

```
Opción seleccionada: 2) Buscar por Genero
Ingrese el genero deseado: acoustic
```

La aplicacion buscara y mostrara todas las canciones con su informacion, las cuales coincidan con el genero dado.

**Opcion 3: Filtrar canciones por artista**

```
Opción seleccionada: 3) Buscar por Artistas
Ingrese el artista deseado: Audien
```

La aplicacion buscara y mostrara todas las canciones con su informacion, las cuales contengan al artista dado.

**Opcion 4: Filtrar canciones por tempo**

El usuario eligira una velocidad de las sgtes:
**Lentas**: Tempo < 80
**Moderadas**: Tempo >= 80 y Tempo < 120
**Rapidas**: Tempo >= 120

```
Opción seleccionada: 4) Buscar por Tempo (BPM)
Ingrese la velocidad deseada (Lentas, moderadas o rapidas): mODERadas
```
La aplicacion buscara y mostrara todas las canciones con su informacion, las cuales vayan a la velocidad dada.


**Opcion 6: Salir del programa**

```
Opción seleccionada: 6) Salir
```
El programa limpia todo lo que tenga almacenado en ese momento y luego se cierra.

=======
