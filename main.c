#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mapa.h"
#include "list.h"
#define LENTAS 0
#define MODERADAS 1
#define RAPIDAS 2

typedef struct {
  char id[100];
  char title[200];
  char *genre;
  char **artists;
  float bpm;
} Song;

// Declaracion de funciones
void inicializarListas(List **array); // Inicializar las listas de tempo
void mostrarMenuPrincipal(); // Muestra las opciones que puede escoger el usuario
void menuOpciones(Map *mapaGeneros, Map *mapaArtistas, List **BPMArray); //Procesa la opcion ingresada por el usuario
void cargarCanciones(Map *cancionesPorGenero, Map *cancionesPorArtistas, List **BPMArray, char *direccion); 
// Carga canciones desde una direccion dada y las asigna a sus respectivos mapas y listas
void buscarPorGenero(Map *mapaGeneros, char *genre); // Busca la lista del genero especificado en el mapa
void buscarPorArtista(Map *mapaArtistas, char *artista); // Busca la lista del artista especificado en el mapa
void buscarPorBPM(List **BPMArray, char *velocidad); // Busca la lista del tempo especificado en la lista
void printearCancion(Song *cancion);  // Imprime los datos de la cancion dada
void limpiarPrograma(Map *mapaGeneros, Map *mapaArtistas, List **BPMArray); // Limpia todos los datos usados por el programa en la finalizacion de este
void cleanList(List * list); // Limpia una lista

int main() {
    //freopen("entrada.txt", "r", stdin); //Usado para debugear

    Map *cancionesPorGenero = createMap(1000); // Se crea un mapa para almacenar caciones usando como key el genero (con capacidad para 1000 generos)

    Map *cancionesPorArtista = createMap(30000); // Se crea un mapa para almacenar caciones usando como key el artista (si son multiples, se agregan a todos los artistas)
    // Capacidad para guardar 30000 artistas
    List **BPMArray = malloc(sizeof(List *) * 3); // Se crea un array de punteros a List para almacenar canciones dependiendo de su tempo (con 3 tipos, lentas, moderadas y rapidasd)
    if (BPMArray == NULL) { // Verifica si la asignación de memoria falló
        perror("Error al asignar memoria para BPMArray");
        exit(EXIT_FAILURE);
    }   
    inicializarListas(BPMArray);

    while(1) {
        mostrarMenuPrincipal();
        menuOpciones(cancionesPorGenero, cancionesPorArtista, BPMArray);
    }
    return 0;
}

// Menú principal
void mostrarMenuPrincipal() {
  puts("========================================");
  puts("     Base de Datos de Canciones");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por Genero");
  puts("3) Buscar por Artistas");
  puts("4) Buscar por Tempo (BPM)");
  puts("5) Salir");
  puts("Seleccione una opcion: ");
}

void esperarAccion() {
    puts("Presione enter para continuar");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    puts(""); 
}

void verificarOpcion(int *num) {
    char str[3];
    while (1) {
        fgets(str, 3, stdin);
        if (str[strlen(str) - 1] != '\n') { //Se revisa si el usuario escribio mas de 2 caracteres
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Limpiar stdin para leer correctamente el proximo input
        }
        else {
            if (isdigit(str[0]) && str[1] == '\n') { //En caso de que el numero ingresado no sea valido
                *num = str[0] - '0';
                if (*num > 0 && *num < 6) break;
            }
        }
        puts("Ingresa una opcion Valida");
        esperarAccion();
    }
}

void menuOpciones(Map *mapaGeneros, Map *mapaArtistas, List **BPMArray) {
    int num;
    verificarOpcion(&num); //Verifica que la opcion ingresada sea valida
    char temp[100];
    char direccion[200];
    switch (num) {
    case 1: // Cargar canciones
        printf("Ingrese la direccion del archivo CSV: ");
        fgets(direccion, sizeof(direccion), stdin);
        direccion[strcspn(direccion, "\n")] = '\0';
        cargarCanciones(mapaGeneros, mapaArtistas, BPMArray, direccion);
        puts("Carga de canciones exitosa.");
        break;
    case 2: // Por Genero
        printf("Ingrese el genero deseado: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        buscarPorGenero(mapaGeneros, temp);
        esperarAccion();
        break;
    case 3: // Por Artista
        printf("Ingrese el artista deseado: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        buscarPorArtista(mapaArtistas, temp);
        esperarAccion();
        break;
    case 4: // Por Tempo
        printf("Ingrese la velocidad deseada (Lentas, moderadas o rapidas): ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        for (int i = 0 ; temp[i] != '\0' ; i++)
            temp[i] = toupper(temp[i]);
        buscarPorBPM(BPMArray, temp);
        esperarAccion();
        break;
    case 5: // Fin del Programa
        // Borrar Elementos de la listas de bpms (borrara todos los datos de las canciones)
        // Borrar el elemento lista de los bpms
        // Borrar cada lista en ambos mapas
        // Borrar los mapas
        limpiarPrograma(mapaGeneros, mapaArtistas, BPMArray);
        puts("Saliendo del programa...");
        exit(EXIT_SUCCESS);
        break;
    }
}

void inicializarListas(List **array) {
    for (int i = 0 ; i < 3 ; i++)
        array[i] = createList();
    
}

char **leerLineaCSV(FILE *archivo, const char *separador) {
    char buffer[2048];
    int columnas = 21;
    int dentroComillas = 0;

    int contSeparador = 0;

    char **linea = malloc(sizeof(char *) * (columnas + 1)); // Se reserva memoria para las columnas de la linea
    if (linea == NULL) {
        perror("Error al asignar memoria para la linea");
        exit(EXIT_FAILURE);
    }
    if (fgets(buffer, sizeof(buffer), archivo) == NULL) { // Obtiene la linea del archivo
        return NULL; // Si encuentra el EOF
    }
    int columnaActual = 0;
    char *campo = malloc(strlen(buffer) + 1); // Campo temporal
    if (campo == NULL) {
        perror("Error al asignar memoria para el campo");
        exit(EXIT_FAILURE);
    }
    int posCampo = 0;
    for (int i = 0; buffer[i] != '\0'; i++) { // Recorre cada caracter de la linea actual
        if (buffer[i] == '"') {
            dentroComillas = !dentroComillas; // Cambia a dentro y fuera de comillas
        } else if (buffer[i] == *separador && !dentroComillas) { // Revisa si se encontro el separador y ademas esta fuera de comillas (lo que significa que termino el campo)
            campo[posCampo] = '\0'; // Agrega el caracter nulo
            linea[columnaActual] = strdup(campo); // Copia el campo obtenido en el array de arrays
            if (linea[columnaActual] == NULL) {
                perror("Error al asignar memoria para el campo");
                exit(EXIT_FAILURE);
            }
            columnaActual++; // Pasa a la siguiente columna
            posCampo = 0; // Reinicia la pos del campo para poder leer correctamente el siguiente
        } else {
            campo[posCampo++] = buffer[i]; // Agrega el caracter leido al campo
        }
    }
    campo[posCampo] = '\0'; // Utilizado en el ultimo campo leido, ya que este no sale con coma
    
    linea[columnaActual] = malloc(strlen(campo) + 1);
    if (linea[columnaActual] == NULL) {
        perror("Error al asignar memoria para el campo");
        exit(EXIT_FAILURE);
    }
    strcpy(linea[columnaActual], campo);
    columnaActual++;
    linea[columnaActual] = NULL;
    free(campo); // Libera la memoria del campo
    campo = NULL;   
    return linea;
}

void limpiarLineaArchivo(char **linea, int col) { // Para liberar memoria almacenada por el csv
    for (int i = 0; i < col; i++) { // Se libera cada campo de la linea.
        if (linea[i] != NULL) {  // Solo libera r si no es NULL
            free(linea[i]);
            linea[i] = NULL;
        }
    }
    free(linea);
    linea = NULL;
}

char **separarString(char *cadena, const char *separador) {
    char *copia = strdup(cadena);
    if (copia == NULL) {
        perror("Error al asignar memoria para la copia del string");
        exit(EXIT_FAILURE);
    }
    if (copia == NULL || separador == NULL) return NULL;
    int capacidad = 10; // Capacidad inicial (10 artistas max inicialmente)
    int size = 0;

    char **result = malloc((capacidad + 1) * sizeof(char *)); // Se crea el array para almacenar strings de artistas
    if (result == NULL) { // Si falla la asignacion de memoria
        perror("Error al asignar memoria para los resultados");
        exit(EXIT_FAILURE); 
    }
    char *artista = strtok(copia, separador); // Va recorriendo la cadena, tomando un fragmento hasta encontrar el separador o '\0'
    while (artista != NULL) {
        if (size >= capacidad) { // Si se supera la capacidad inicial (hay mas de 10 artistas)
            capacidad *= 2;
            char **temp = realloc(result, (capacidad + 1) * sizeof(char *)); 
            if (temp == NULL) { // Si falla la asignacion de memoria
                perror("Error al aumentar la memoria para los resultados");
                exit(EXIT_FAILURE);
            }
            result = temp;
        }
        result[size++] = strdup(artista);  // Se agrega el artista al array
        if (result[size - 1] == NULL) { // Si la asignacion de memoria falla
            perror("Error al asignar memoria para el artista");
            exit(EXIT_FAILURE);
        }
        artista = strtok(NULL, separador); // Se avanza al siguiente campo (sgte artista)
    }
    result[size] = NULL; // Marcar el final del array con un NULL
    free(copia);
    copia = NULL;
    return result; // Retorna array de strings (artistas)
}

int contarColumnas(char **linea) {
    int columnas = 0;
    while (linea[columnas] != NULL) columnas++;
    return columnas;
}

void cargarCanciones(Map *cancionesPorGenero, Map *cancionesPorArtistas, List **BPMArray, char *direccion) { // Lee todo el archivo CSV y guarda las canciones en memoria, usando dos mapas hash que guardan listas (uno con key en genero y otro con key en artistas)
    FILE *archivo = fopen(direccion, "r"); // Intenta abrir el archivo CSV que contiene datos de canciones usando la direccion dada
    if (archivo == NULL) {
        perror("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
        exit(EXIT_FAILURE);
    }
    char **lineaArchivo;
    int cantFilas = 1;
    int cantColumnas;
    Pair *par = NULL;
    List *lista = NULL;
    char *buffer = malloc(2048);
    fgets(buffer, 2048, archivo); // Skipea la primera linea del documento (ya que no almacena una cancion)
    free(buffer);
    buffer = NULL;
    while((lineaArchivo = leerLineaCSV(archivo, ",")) != NULL) { // Lee cada línea del archivo CSV hasta el final
        if ((cantColumnas = contarColumnas(lineaArchivo)) != 21) { //Verificar si alguna linea no cuenta con informacion clave
            printf("Fila %d está malformada. Saltando...\n", cantFilas);
            limpiarLineaArchivo(lineaArchivo, cantColumnas); // Limpia la memoria de la linea
            cantFilas++;
            continue;
        }
        Song *cancion = (Song *)malloc(sizeof(Song)); // Crea un struct Song nuevo, asignandole memoria
        if (cancion == NULL) { // Verificar asignacion de memoria
            perror("Error al asignar memoria para la canción");
            exit(EXIT_FAILURE);
        }
        strcpy(cancion -> id, lineaArchivo[0]); // Asigna ID
        strncpy(cancion -> title, lineaArchivo[4], 200); // Asigna título
        if (strcmp(cancion -> id, "106638") == 0) {
            puts("");
        }
        cancion -> title[199] = '\0'; // Asigna el caracter nulo al final por seguridad
        cancion -> artists = separarString(lineaArchivo[2], ";"); // Separa el string de todos los artistas por sus ; y los inserta como un array en artists
        cancion -> genre = strdup(lineaArchivo[20]); // Asigna el genero
        cancion -> genre[strcspn(cancion -> genre, "\n")] = '\0'; // Borra el \n
        cancion -> bpm = strtof(lineaArchivo[18], NULL); // Asigna el tempo de la cancion y lo guarda como float2

        // Genero
        par = searchMap(cancionesPorGenero, cancion -> genre); // Se obtiene el pair
        if (par == NULL) { // Si es NULL, se crea una lista y se le asigna la key correspondiente al mismo artista
            lista = createList();
            insertMap(cancionesPorGenero, cancion -> genre, lista);
            pushBack(lista, cancion);
        }
        else {
            lista = (List *) par -> value;
            pushBack(lista, cancion); // Se agrega la cancion actual a la lista del artista
        }

        // Artista
        for (int j = 0; cancion -> artists[j] != NULL; j++) { // Se recorre el array de artistas
            par = searchMap(cancionesPorArtistas, cancion -> artists[j]); // Se obtiene el pair
            if (par == NULL) { // Si es NULL, se crea una lista y se le asigna la key correspondiente al mismo artista
                lista = createList();
                insertMap(cancionesPorArtistas, cancion -> artists[j], lista);
                pushBack(lista, cancion);
            }
            else {
                lista = (List *) par -> value;
                pushBack(lista, cancion); // Se agrega la cancion actual a la lista del artista
            }
        }

        // Tempo
        if (cancion -> bpm < 80) { // Asigna a la lista correspondiende dependiendo del tempo
            pushBack(BPMArray[LENTAS], cancion);
        } else if (cancion -> bpm > 120) {
            pushBack(BPMArray[RAPIDAS], cancion);
        } else {
            pushBack(BPMArray[MODERADAS], cancion);
        }
        limpiarLineaArchivo(lineaArchivo, cantColumnas); // Limpia la memoria de la linea
        cantFilas++;
    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

void juntarArtistas(char **artists, char *output, int outputSize) {
    output[0] = '\0'; // Inicia el string como vacio
    for (int i = 0 ; artists[i] != NULL ; i++) {
        char *copy = strdup(artists[i]);
        strncat(output, copy, outputSize - strlen(output) - 1); // Concatena al artista i en output
        if (artists[i + 1] != NULL) { // Revisa si el siguiente artista es el final o no
            strncat(output, ", ", outputSize - strlen(output) - 1);
        }
        free(copy);
    }
}

void printearCancion(Song *cancion) {
    char artistasUnidos[200]; // Para almacenar todos los artistas en una sola variable
    juntarArtistas(cancion -> artists, artistasUnidos, sizeof(artistasUnidos)); // Junta a todos los artistas, separados por comas

    printf("\n%s\n", cancion -> title);
    printf("----------------------------------------\n");
    printf("    ID:      %s\n", cancion -> id);
    printf(" Artista/s:  %s\n", artistasUnidos);
    printf("Tempo (BPM): %.2f\n", cancion -> bpm);
    printf("  Genero:    %s\n", cancion -> genre);
    printf("----------------------------------------\n");
}

void buscarPorGenero(Map *mapaGeneros, char *genre) {
    Pair *par = searchMap(mapaGeneros, genre); // Busca la lista de canciones con el genero ingresado en el mapa de generos y lo almacena
    if (par == NULL) { // Se revisa si se pudo encontrar el par con la key dada.
        printf("No existen canciones con el genero \"%s\" \n", genre); 
        return;
    }
    List *lista = (List *)par -> value;
    printf("Canciones con el genero \"%s\":", genre);
    Song *cancion = NULL;
    int i = 0;
    for(cancion = firstList(lista) ; cancion != NULL ; cancion = nextList(lista)) { // Recorre la lista hasta que el elemento actual sea nulo (que se alcance el final de la lista)
        printearCancion(cancion);
        i++;
    }
    printf("Se encontraron %d canciones con el genero \"%s\" \n", i, genre);
}

void buscarPorArtista(Map *mapaArtistas, char *artista) {
    Pair *par = searchMap(mapaArtistas, artista); // Busca la lista de canciones con el artista ingresado en el mapa de artistas y lo almacena
    if (par == NULL) { // Se revisa si se pudo encontrar el par con la key dada.
        printf("No existen canciones con el artista \"%s\" \n", artista);
        return;
    }
    List *lista = par -> value; // Se usara solo el value (la lista), asi que se obtiene
    
    printf("Canciones con el artista \"%s\":", artista);
    Song *cancion = NULL;
    int i = 0;
    for(cancion = firstList(lista) ; cancion != NULL ; cancion = nextList(lista)) { // Recorre la lista hasta que el elemento actual sea nulo (que se alcance el final de la lista)
        printearCancion(cancion);
        i++;
    }
    printf("Se encontraron %d canciones con el artista \"%s\" \n", i, artista);
}

void buscarPorBPM(List **BPMArray, char *velocidad) {
    List *lista = NULL;
    if (strcmp(velocidad, "LENTAS") == 0) {
        lista = BPMArray[LENTAS];
    } else if (strcmp(velocidad, "MODERADAS") == 0) {
        lista = BPMArray[MODERADAS];
    } else if (strcmp(velocidad, "RAPIDAS") == 0) {
        lista = BPMArray[RAPIDAS];
    } else {
        puts("No se ingreso una velocidad valida.");
        return;
    } 
    if (firstList(lista) == NULL) {
        printf("No existen canciones a velocidad \"%s\" \n", velocidad);
        return;
    }


    Song *cancion = NULL;
    printf("Canciones con velocidad \"%s\":", velocidad);
    int i = 0;
    for(cancion = firstList(lista) ; cancion != NULL ; cancion = nextList(lista)) { // Recorre la lista hasta que el elemento actual sea nulo (que se alcance el final de la lista)
        printearCancion(cancion);
        i++;
    }
    printf("Se encontraron %d canciones a velocidad \"%s\" \n", i, velocidad);
}

void limpiarMapaGeneros(Map *mapa) {
    Pair *i = firstMap(mapa);
    while (i != NULL) { // Itera por todo el mapa hasta llegar al final
        Pair *siguiente = nextMap(mapa); // Guarda el siguiente elemento del mapa
        sizeDown(mapa); // Reduce el tamaño del mapa
        free(i -> key); //Libera la key
        i -> key = NULL;
        free(i -> value); // Libera el value (el List)
        i -> value = NULL;
        free(i); // Libera el Par
        i = NULL;
        i = siguiente; // Avanza al sgte.
    }
}

void limpiarMapaArtistas(Map *mapa) {
    Pair *i = firstMap(mapa); // Busca el elemento del mapa
    while (i != NULL) { // Itera por todo el mapa hasta llegar al final
        Pair *siguiente = nextMap(mapa); // Guarda el siguiente elemento del mapa
        sizeDown(mapa); // Reduce el tamaño del mapa
        free(i -> key); //Libera la key
        i -> key = NULL;
        free(i -> value); // Libera el value (el List)
        i -> value = NULL;
        free(i); // Libera el Par
        i = NULL;
        i = siguiente; // Avanza al sgte.
    }
}

void cleanSong(Song *song) {
    if (song == NULL) return;

    if (song -> genre != NULL) { // Liberar la memoria del genre
        free(song -> genre); 
        song -> genre = NULL;
    }
    
    if (song -> artists != NULL) { // Libera la memoria del array de artistas
        for (int i = 0; song->artists[i] != NULL; i++) { // Itera por cada artista y va liberando la memoria correspondiente por string
            free(song -> artists[i]);
        }
        free(song -> artists);
        song -> artists = NULL;
    }
    free(song); // Se libera la memoria de Song
}

void cleanList(List * list) { 
    Song *cancion = NULL;
    while ((cancion = popFront(list)) != NULL) {
        cleanSong(cancion);
        cancion = NULL;
    } 
}

void limpiarPrograma(Map *mapaGeneros, Map *mapaArtistas, List **BPMArray) {
    // Limpieza del array
    for (int i = 0 ; i < 3 ; i++) { // Limpia cada lista almacenada en el array, incluyendo el song y todos sus elementos
        cleanList(BPMArray[i]); // Limpieza de la lista + datos
        free(BPMArray[i]); // Libera la lista
        BPMArray[i] = NULL;
    }
    free(BPMArray); // Se libera el array
    BPMArray = NULL;
 
    // Limpieza de mapas
    limpiarMapaGeneros(mapaGeneros);
    limpiarMapaArtistas(mapaArtistas);
    free(mapaGeneros);
    free(mapaArtistas);
    return;
}
