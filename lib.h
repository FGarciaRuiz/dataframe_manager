/*DEFINICIÓN DE LA LIBRERÍA*/
#ifndef LIB_H
#define LIB_H

/*LIBRERÍAS INCLUIDAS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*COLORES Y SALTOS DEFINIDOS*/
#define ROJO    "\x1b[31m"
#define VERDE   "\x1b[32m"
#define BLANCO  "\x1B[37m"
#define RESET   "\x1b[0m"
#define T       "\t\t"
#define S       "\n"
#define DS      "\n\n"
#define EAM     "ERROR AL ASIGNAR MEMORIA"
#define EAF     "ERROR: NO ES VALIDO EL FICHERO SOLICITADO"
#define EPA     "ERROR: POCOS ARGUMENTOS PARA ESTE COMANDO"
#define EDA     "ERROR: DEMASIADOS ARGUMENTOS PARA ESTE COMANDO"
#define EDFA    "ERROR: NO HAY UN DATAFRAME ACTIVO"
#define ECNE    "ERROR: LA COLUMNA INDICADA NO EXISTE"
#define NaN     "#N/A"

/*ESTRUCTURAS A UTILIZAR*/

// Tipo enumerado para representar los diferentes tipos de datos en las columnas
typedef enum {
    TEXTO,
    NUMERICO,
    FECHA
} TipoDato;

// Estructura para representar una columna del dataframe
typedef struct {
    char nombre[30];       // Nombre de la columna
    TipoDato tipo;         // Tipo de datos de la columna (TEXTO, NUMERICO, FECHA)
    void *datos;           // Puntero genérico para almacenar los datos de la columna
    unsigned char *esNulo; // Array paralelo, indica valores nulos (1/0: nulo/noNulo)
    int numFilas;          // Número de filas en la columna
} Columna;

// Estructura para representar el dataframe como un conjunto de columnas
typedef struct {
    char nombre[51];       // Nombre del dataframe
    Columna *columnas;     // Array de columnas (con tipos de datos distintos)
    int numColumnas;       // Número de columnas en el dataframe
    int numFilas;          // Número de filas (igual para todas las columnas)
    int *indice;           // Array para ordenar las filas
} Dataframe;

// Alias para tipos FECHA: 'Fecha' alias de 'struct tm' (<time.h>)
typedef struct tm Fecha;

// Estructura para representar un nodo de la lista
typedef struct NodoLista{
    Dataframe *df;                // Puntero a un dataframe
    int iDFs;                     // Indice del dataframe
    struct NodoLista *siguiente;  // Puntero al siguiente nodo de la lista
} Nodo;

// Estructura para representar la lista de Dataframe’s
typedef struct {
    int numDFs;       // Número de dataframes almacenados en la lista
    Nodo *primero;    // Puntero al primer Nodo de la lista
} Lista;

//Estructura para extraer información de fichero CSV leído
typedef struct {
    char* datos;      // String con el contenido total del fichero
    int numFilas;     // Cantidad de filas del fichero
    int numColumnas;  // Cantidad de columnas del fichero
}ficheroCSV;

/*FUNCIONES EN EL MAIN*/

Nodo* insertar_nodo_final(Lista* lista, Dataframe* df);
//  PROTOTIPO: Nodo* insertar_nodo_final(Lista*,Dataframe*)
//  FUNCIÓN: Inserta un nodo al final de una lista enlazada con el dataframe indicado.
//  RETORNO: Devuelve un puntero al nodo insertado.

Dataframe* importar_csv(const char* NombreArchivo);
//  PROTOTIPO: Dataframe* importar_csv(const char*)
//  FUNCIÓN: Carga el dataframe a partir del nombre del archivo CSV.
//  RETORNO:  Devuelve un puntero al dataframe cargado.

int buscar_df_nombres(Lista* lista,const char* nombre_dataframe);
//  PROTOTIPO: int buscar_df_nombres(Lista*,const char*)
//  FUNCIÓN: Busca un nombre de dataframe que coincida con el dispuesto por parámetros.
//  RETORNO: Devuelve el indice de dataframe, si el nombre no se encuentra, devuelve la cantidad de dataframes en la lista.

void mostrar_lista(Lista* lista);
//  PROTOTIPO: void mostrar_lista(Lista*)
//  FUNCIÓN: Muestra una los dataframes que hayan cargados en la lista con sus nombres, nºfilas y nºcolumnas.
//  RETORNO: No devuelve.

void prefix_columna(Dataframe* df,const char* nombre_columna,int nchars,const char* nueva_columna);
//  PROTOTIPO: void prefix_columna(Dataframe*,const char*,int,const char*)
//  FUNCIÓN: Crea una nueva columna(nombre que no haya sido tomado ya) donde se copiaran los 'n' caracteres de la columna de TEXTO indicada.
//  RETORNO: No devuelve.

void metadatos(Nodo* nodo);
//  PROTOTIPO: void metadatos(Nodo*)
//  FUNCIÓN: Muestra por pantalla información sobre el dataframe activo (nº Columnas y filas,nombre,tipo y nºnulos en cada columna).
//  RETORNO: No devuelve.

void mostrar_dataframe(Dataframe* df,int nFilas);
//  PROTOTIPO: void mostrar_dataframe(Dataframe*,int)
//  FUNCIÓN: Muestra por pantalla las nFilas del dataframe con todas las ordenaciones que se hayan realizado.
//  RETORNO: No devuelve.

void mostrar_df_ultimos(Dataframe* df,int nFilas);
//  PROTOTIPO: void mostrar_df_ultimos(Dataframe*,int)
//  FUNCIÓN: Muestra por pantalla las nFilas del dataframe con todas las ordenaciones que se hayan realizado, en orden inverso.
//  RETORNO: No devuelve.

void quarter_columna(Dataframe* df,const char* nombre_columna,const char* nueva_columna);
//  PROTOTIPO: void quarter_columna(Dataframe*,const char*,const char*)
//  FUNCIÓN: Crea una nueva columna donde indicará a que trimestre del año pertenece las fechas de la columna a analizar.
//  RETORNO: No devuelve.

void eliminar_filas_nulas(Dataframe* df,const char* nombre_columna);
//  PROTOTIPO: void eliminar_filas_nulas(Dataframe*,const char*)
//  FUNCIÓN: Elimina todas las filas con valores nulos en una columna gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.

void ordenar_asc(Dataframe* df,const char* nombre_columna);
//  PROTOTIPO: void ordenar_asc(Dataframe*,const char*)
//  FUNCIÓN: Ordena la columna solicitada ascendentemente.
//  RETORNO: No devuelve.

void ordenar_des(Dataframe* df,const char* nombre_columna);
//  PROTOTIPO: void ordenar_des(Dataframe*,const char*)
//  FUNCIÓN: Ordena la columna solicitada descendentemente.
//  RETORNO: No devuelve.

void filtros(Dataframe* df, const char* nombre_columna, const char* comp, const char* refch);
//  PROTOTIPO: void filtros(Dataframe*,const char*,const char*,const char*)
//  FUNCIÓN: Identifica que tipo de dato es el de la columna a filtrar y se llama a la respectiva función.
//  RETORNO: No devuelve.

void exportar_csv(Dataframe* df,const char* NombreArchivo);
//  PROTOTIPO: void exportar_csv(Dataframe*,const char*)
//  FUNCIÓN: Genera un fichero csv a partir de un dataframe.
//  RETORNO: No devuelve.

/*FUNCIONES DE MANEJO CON STRINGS*/

void trim(char *cad);
//  PROTOTIPO: void trim(char*)
//  FUNCIÓN: Eliminar los espacios ubicados al incio y al final de un string.
//  RETORNO: No devuelve.

int validar_numero_natural(const char *str);
//  PROTOTIPO: int validar_numero_natural(const char*)
//  FUNCIÓN: Determinar si un string representa un número natural.
//  RETORNO: Devuelve '1' si el string es un número natural y '0' si no.

int validar_numero_entero(const char *str);
//  PROTOTIPO: int validar_numero_entera(const char*)
//  FUNCIÓN: Determinar si un string representa un número entero.
//  RETORNO: Devuelve '1' si el string es un número entero y '0' si no.

int validar_numero(const char *cad);
//  PROTOTIPO: int validar_numero(const char*)
//  FUNCIÓN: Determinar si un string representa un número real.
//  RETORNO: Devuelve '1' si el string es un número real y '0' si no.

int validar_fecha(const char *comando);
//  PROTOTIPO: int validar_fecha(const char*)
//  FUNCIÓN: Determinar si un string representa una fecha con el formato (AAAA-MM-DD).
//  RETORNO: Devuelve '1' si el string es una fecha y '0' si no.

/*FUNCIONES DE BÚSQUEDA*/

Nodo* recorrer_lista(Lista* lista, int n);              
//  PROTOTIPO: Nodo* recorrer_lista(Lista*,int)
//  FUNCIÓN: Recorre la lista hasta el nodo indicado.
//  RETORNO: Devuelve el puntero al nodo de la posición que se indique en n siempre que existan, de lo contrario devuelve NULL.

int columna_index(Dataframe* df,const char* nombre_columna);
//  PROTOTIPO: int columna_index(Dataframe*,const char*)
//  FUNCIÓN: Ubica la columna de un dataframe a partir de su nombre.
//  RETORNO: Devuelve el índice de la columna. En caso que esta no se encuentre, devuelve el número total de columnas.

int fila_nula(Columna* columna,int nColumna);
//  PROTOTIPO: int fila_nula(Columna*,int)
//  FUNCIÓN: Ubica la primera fila nula de una columna.
//  RETORNO: Devuelve el índice de la fila. En caso que esta no se encuentre, devuelve el número total de filas.

void filtro_txt(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch);
//  PROTOTIPO: void filtro_txt(Dataframe*,const char*,const char*,const char*)
//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo TEXTO.
//  RETORNO: No devuelve.

void filtro_num(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch);
//  PROTOTIPO: void filtro_num(Dataframe*,const char*,const char*,const char*)
//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo NUMÉRICO.
//  RETORNO: No devuelve.

void filtro_fch(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch);
//  PROTOTIPO: void filtro_fch(Dataframe*,const char*,const char*,const char*)
//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo FECHA.
//  RETORNO: No devuelve.

/*FUNCIONES DE CARGA DEL DATAFRAME*/

ficheroCSV* copiar_csv(const char* NombreArchivo);      
//  PROTOTIPO: fichero* copiar_csv(const char*)
//  FUNCIÓN: Copia todo el contenido de un archivo csv en un string y cuenta nºfilas y nºcolumnas.
//  RETORNO: Devuelve una estructura tipo ficheroCSV(String con el contenido,nº filas y nº columnas).

Dataframe* cargar_dataframe(ficheroCSV* fichero);
//  PROTOTIPO: Dataframe* cargar_dataframe(ficheroCSV*)
//  FUNCIÓN: Carga toda la estructura del ficheroCSV en un Dataframe.
//  RETORNO: Devuelve el dataframe cargado.

/*FUNCIONES DE ELIMINACIÓN DINÁMICA*/

void eliminar_columna(Dataframe* df,const char* nombre_columna);
//  PROTOTIPO: void eliminar_columna(Dataframe*,const char*)
//  FUNCIÓN: Elimina una columna gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.

void eliminar_fila(Dataframe* df, int nFila);
//  PROTOTIPO: void eliminar_filas(Dataframe*,int)
//  FUNCIÓN: Elimina una fila gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.

void liberar_dataframe(Dataframe* df);
//  PROTOTIPO: void liberar_dataframe(Dataframe*)
//  FUNCIÓN: Libera toda la memoria reservada para el dataframe.
//  RETORNO: No devuelve.

void liberar_lista(Lista* lista);
//  PROTOTIPO: void liberar_lista(Lista*)
//  FUNCIÓN: Libera toda la memoria reservada para la lista de dataframes, incluyéndolos.
//  RETORNO: No devuelve.

#endif