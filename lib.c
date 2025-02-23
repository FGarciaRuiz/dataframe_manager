#include "lib.h"

/*FUNCIONES EN EL MAIN*/

//  FUNCIÓN: Inserta un nodo al final de una lista enlazada con el dataframe indicado.
//  RETORNO: Devuelve un puntero al nodo insertado.
Nodo* insertar_nodo_final(Lista* lista, Dataframe* df)
{
    if(!df)
        return NULL;
    Nodo* aux = lista->primero;      // Nodo auxiliar apuntando al principio de la lista
    Nodo* nuevo_nodo = (Nodo*) malloc(sizeof(Nodo)); if(!nuevo_nodo) printf(ROJO S EAM S); // Creo un nuevo nodo
    
    if(!lista->primero)              // Si la lista estaba vacía,
        lista->primero = nuevo_nodo; // el nuevo nodo va a ser el primero de la misma.
    else
    {
        while(aux->siguiente)        // Recorro lista hasta que el siguiente sea nulo.
            aux=aux->siguiente;
        aux->siguiente = nuevo_nodo; // Inserto el nuevo nodo al final
    }
    sprintf(df->nombre,"df%d",lista->numDFs); //Nombro al dataframe como dfn, donde n es el indice de Dataframe que le corresponde.
    nuevo_nodo->df = df;             // Inserto el dataframe en el nodo.
    nuevo_nodo->siguiente=NULL;      // Inicializo el puntero al siguiente nodo en NULL
    nuevo_nodo->iDFs=lista->numDFs;  // Asigno un indice de dataframe acorde a la cantidad de dataframes en lista
    lista->numDFs+=1;                // Incremento la cantidad de dataframes en lista.
    return nuevo_nodo;               // Devuelvo el nuevo nodo
}

//  FUNCIÓN: Carga el dataframe a partir del nombre del archivo CSV.
//  RETORNO: Devuelve un puntero al dataframe cargado.
Dataframe* importar_csv(const char* NombreArchivo)
{
    ficheroCSV* fichero=NULL;
    Dataframe* df=NULL;
    fichero=copiar_csv(NombreArchivo);  // Consigo el fichero procesado a una estructura del tipo ficheroCSV.
    if(fichero!=NULL)                   // Si se procesó con éxito empieza a cargar el dataframe, sino no.
    {
        df=cargar_dataframe(fichero);   // Carga el dataframe.
        if(!df)                         // Si no pudo cargar el dataframe, da error. 
        {
            printf(ROJO S"ERROR AL CARGAR DATAFRAME"S);
            return NULL;
        }
        return df;                      // Si se carga el dataframe con éxito, devuelvo un puntero a él.
    }
    return NULL;
}

//  FUNCIÓN: Busca un nombre de dataframe que coincida con el dispuesto por parámetros.
//  RETORNO: Devuelve el índice de dataframe, si el nombre no se encuentra, devuelve la cantidad de dataframes en la lista.
int buscar_df_nombres(Lista* lista,const char* nombre_dataframe)
{
    int index=0;
    Nodo* nodo=lista->primero;  // Nodo al inicio de la lista.
    while(index<lista->numDFs)
    {
        if(!strcmp(nodo->df->nombre,nombre_dataframe)) // Si el nombre del dataframe corresponde,
            return index;                              // devuelvo el índice.
        nodo=nodo->siguiente;   // Avanzo al siguiente nodo.
        index++;                // Incremento índice.
    }
    return index;               // Devuelvo índice (cantidad de dataframes en lista).
}

//  FUNCIÓN: Muestra una los dataframes que hayan cargados en la lista con sus nombres, nºfilas y nºcolumnas.
//  RETORNO: No devuelve.
void mostrar_lista(Lista* lista)
{
    int index=0;
    Nodo* nodo=lista->primero;  // Nodo al inicio de la lista.
    printf(VERDE S);
    while(index<lista->numDFs)  // Recorro la lista.
    {
        printf("%s: %d filas, %d columnas" S,nodo->df->nombre,nodo->df->numFilas,nodo->df->numColumnas); // Muestro por pantalla
        nodo=nodo->siguiente;   // Avanzo al siguiente nodo.
        index++;                // Incremento el índice.
    }
    printf(S RESET);
}

//  FUNCIÓN: Crea una nueva columna(nombre que no haya sido tomado ya) donde se copiaran los 'n' caracteres de la columna de TEXTO indicada.
//  RETORNO: No devuelve.
void prefix_columna(Dataframe* df,const char* nombre_columna,int nchars,const char* nueva_columna)
{
    int xColumna,nColumna,iFilas; // xColumna: índice de la columna a recortar, nColumna: índice de la nueva columna, iFilas: índice de filas.
    char **pch=NULL,**pa=NULL;    // pch: puntero auxiliar a nuevos strings, pa: puntero auxiliar a strings originales.
    Columna* columna=df->columnas;// columna: puntero a columnas del dataframe.            
    xColumna=columna_index(df,nombre_columna); // Asigno el índice correspondiente.
    if(columna_index(df,nueva_columna)!=df->numColumnas) 
    {   // Si el nombre de la nueva columna ya fue tomado, da error.
        printf(ROJO S "ERROR: EL NUEVO NOMBRE DE LA COLUMNA YA ESTA TOMADO" S);
        return;
    }
    if(xColumna==df->numColumnas)
    {   // Si la columna a recortar no fue encontrada, da error.
        printf(ROJO S ECNE S);
        return;
    }
    if(columna[xColumna].tipo!=TEXTO) 
    {   // Si la columna a recortar no es de tipo TEXTO, da error.
        printf(ROJO S "ERROR: ES NECESARIO QUE LA COLUMNA SEA DEL TIPO TEXTO" S);
        return;
    }
    df->numColumnas++; // Incremento la cantidad de columnas.
    // Reasigno la memoria de la columna a la nueva cantidad de columnas en el dataframe. 
    columna= (Columna*)realloc(columna,sizeof(Columna)*df->numColumnas); if(!columna) printf(ROJO S EAM S);
    nColumna=df->numColumnas-1;  // Asigno el índice correspondiente.
    columna[nColumna].tipo=TEXTO;// Determino el tipo de dato de la nueva columna (TEXTO).
    // Reservo memoria para las filas de la nueva columna.
    columna[nColumna].datos =(char**)malloc(sizeof(char*)*df->numFilas); if(!columna[nColumna].datos) printf(ROJO S EAM S);
    // Reservo memoria para el vector de nulos de la nueva columna.
    columna[nColumna].esNulo =(unsigned char*)malloc(sizeof(unsigned char)*df->numFilas); if(!columna[nColumna].esNulo) printf(ROJO S EAM S);
    columna[nColumna].numFilas=df->numFilas; // Igualo la cantidad de filas de la columna con el del dataframe.
    strcpy(columna[nColumna].nombre,nueva_columna); // Asigno el nombre indicado de la columna nueva.
    columna[nColumna].nombre[strlen(nueva_columna)]='\0'; // Inserto el caracter nulo al final.
    pa=(char**)columna[xColumna].datos; // Asigno el puntero de cadenas de la columna original.
    pch=(char**)columna[nColumna].datos;// Asigno el puntero de cadenas de la columna nueva.
    for(iFilas=0;iFilas<df->numFilas;iFilas++) // Recorro la fila entera.
    {   // Igualo los nulos en la nueva columna.
        columna[nColumna].esNulo[iFilas]=columna[xColumna].esNulo[iFilas];
        if(columna[nColumna].esNulo[iFilas]) // Evalúo si el valor es nulo o si no lo es.
        {   // Resevo memoria para el valor nulo.
            pch[iFilas] =(char*)malloc(sizeof(char)); if(!pch[iFilas]) printf(ROJO S EAM S);
            pch[iFilas][0]='\0'; // Valor nulo.
        }
        else
        {   // Reservo la cantidad de caracteres que vaya a recortar.
            pch[iFilas] =(char*)malloc(sizeof(char)*(nchars+1)); if(!pch[iFilas]) printf(ROJO S EAM S);
            strncpy(pch[iFilas],pa[iFilas],nchars); // Recorto el string en el nuevo string.
            pch[iFilas][nchars]='\0'; // Inserto el caracter nulo al final.
        }
    }
    df->columnas=columna; // Apunto nuevamente las columnas.
}

//  FUNCIÓN: Muestra por pantalla información sobre el dataframe activo (nº Columnas y filas,nombre,tipo y nºnulos en cada columna).
//  RETORNO: No devuelve.
void metadatos(Nodo* nodo)
{
    int iFilas,iColumnas,cant_nulos; // iFilas: índice de filas, iColumnas: índice de columnas, cant_nulos: cantidad de nulos
    Dataframe* df=nodo->df; // Dataframe del nodo.
    printf(VERDE S"METADATOS DEL DATAFRAME %s:"S,nodo->df->nombre); // Muestro el nombre del dataframe.
    printf(VERDE S"Cantidad de filas de datos: %i",df->numFilas);   // Muestro cantidad de filas.
    printf(VERDE S"Cantidad de columnas de datos: %i"S,df->numColumnas);// Muestro cantidad de columnas.
    printf(VERDE S"Metadatos por columnas: "S);
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++) // Recorro por columnas.
    {
        cant_nulos=0; // Inicializo la cantidad de nulos.
        printf(VERDE S"COLUMNA %i",iColumnas); // Muestro índice de columna.
        printf(VERDE S"Nombre: %s",df->columnas[iColumnas].nombre); // Muestro el nombre de la columna.
        if(df->columnas[iColumnas].tipo==FECHA) // Indico el tipo de dato de la columna.
            printf(VERDE S"Tipo: Fecha");
        else if(df->columnas[iColumnas].tipo==NUMERICO)
            printf(VERDE S"Tipo: Numerico");
        else
            printf(VERDE S"Tipo: Texto");
        for(iFilas=0;iFilas<df->numFilas;iFilas++) // Recorro todas las filas de la columna.
            if(df->columnas[iColumnas].esNulo[iFilas]) // Si es nulo, incremento la cantidad de nulos.
                cant_nulos++;
        printf(VERDE S"Cantidad de valores nulos: %i"S,cant_nulos); // Muestro la cantidad de nulos.
    }
    printf(S);
}

//  FUNCIÓN: Muestra por pantalla las nFilas del dataframe con todas las ordenaciones que se hayan realizado.
//  RETORNO: No devuelve.
void mostrar_dataframe(Dataframe* df,int nFilas)
{
    int iFilas,iColumnas,*i=df->indice; // iFilas: índice de filas, iColumnas: índice de columnas, i: puntero al índice.
    double* pnum=NULL; // pnum: puntero auxiliar a datos del tipo NUMERICO.
    Fecha* pdate=NULL; // pdate: puntero auxiliar a datos del tipo FECHA.
    char** pch=NULL;   // pch: puntero auxiliar a datos del tipo TEXTO.
    Columna* columna=df->columnas; // columna: puntero a columnas del dataframe.
    if(nFilas>df->numFilas) // Si las nFilas superan la cantidad de filas total,
        nFilas=df->numFilas;// se muestran el total de filas.
    if(nFilas==0)
    {
        printf(ROJO S "ERROR: EL PARAMETRO DE VIEW NO DEBE SER 0"S);
        return;
    }
    printf(VERDE S);
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++) // Muestra los nombres de las columnas.
        printf("%s"T,columna[iColumnas].nombre);
    for(iFilas=0;iFilas<nFilas;iFilas++) // Recorro por filas.
    {
        printf(DS);
        for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++) // Recorro por columnas.
        {
            if(columna[iColumnas].esNulo[i[iFilas]]) // Si es nulo muestra (#N/A).
                printf(NaN T);
            else
                switch (columna[iColumnas].tipo) // Muestro según el tipo.
                {
                case TEXTO:
                    pch=(char**)columna[iColumnas].datos;
                    printf("%s"T,pch[i[iFilas]]); // Muestro TEXTO.
                    break;
                
                case NUMERICO:
                    pnum=(double*)columna[iColumnas].datos;
                    printf("%f"T,pnum[i[iFilas]]);// Muestro NUMERICO.
                    break;
                
                case FECHA:
                    pdate=(Fecha*)columna[iColumnas].datos;
                    // Muestro FECHA.
                    printf("%i/%i/%i"T,pdate[i[iFilas]].tm_year+1900,pdate[i[iFilas]].tm_mon+1,pdate[i[iFilas]].tm_mday);
                    break;
                }
        }
    }
    printf(S RESET);
}

//  FUNCIÓN: Muestra por pantalla las nFilas del dataframe con todas las ordenaciones que se hayan realizado, en orden inverso.
//  RETORNO: No devuelve.
void mostrar_df_ultimos(Dataframe* df,int nFilas)
{
    int iFilas,iColumnas,*i=df->indice; // iFilas: índice de filas, iColumnas: índice de columnas, i: puntero al índice.
    double* pnum=NULL; // pnum: puntero auxiliar a datos del tipo NUMERICO.
    Fecha* pdate=NULL; // pdate: puntero auxiliar a datos del tipo FECHA.
    char** pch=NULL;   // pch: puntero auxiliar a datos del tipo TEXTO.
    Columna* columna=df->columnas; // columna: puntero a columnas del dataframe.
    if((nFilas*(-1))>df->numFilas) // Si las nFilas superan la cantidad de filas total,
        nFilas=df->numFilas;       // se muestran el total de filas.
    if(nFilas==0)
    {
        printf(ROJO S "ERROR: EL PARAMETRO DE VIEW NO DEBE SER -0"S);
        return;
    }
    printf(VERDE S);
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++) // Muestra los nombres de las columnas.
        printf("%s"T,columna[iColumnas].nombre);
    for(iFilas=(df->numFilas-1);iFilas>(df->numFilas+nFilas-1);iFilas--) // Recorro por filas en orden inverso.
    {
        printf(DS);
        for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++) // Recorro por columnas.
        {
            if(columna[iColumnas].esNulo[i[iFilas]]) // Si es nulo muestra (#N/A).
                printf(NaN T);
            else
                switch (columna[iColumnas].tipo) // Muestro según el tipo.
                {
                case TEXTO:
                    pch=(char**)columna[iColumnas].datos;
                    printf("%s"T,pch[i[iFilas]]); // Muestro TEXTO.
                    break;
                
                case NUMERICO:
                    pnum=(double*)columna[iColumnas].datos;
                    printf("%f"T,pnum[i[iFilas]]);// Muestro NUMERICO.
                    break;
                
                case FECHA:
                    pdate=(Fecha*)columna[iColumnas].datos;
                    // Muestro FECHA.
                    printf("%i/%i/%i"T,pdate[i[iFilas]].tm_mday,pdate[i[iFilas]].tm_mon+1,pdate[i[iFilas]].tm_year+1900);
                    break;
                }
        }
    }
    printf(S RESET);
}

//  FUNCIÓN: Crea una nueva columna donde indicará a que trimestre del año pertenece las fechas de la columna a analizar.
//  RETORNO: No devuelve.
void quarter_columna(Dataframe* df,const char* nombre_columna,const char* nueva_columna)
{
    int xColumna,nColumna,iFilas; // xColumna: índice de la columna a recortar, nColumna: índice de la nueva columna, iFilas: índice de filas.
    char** pch=NULL;              // pch: puntero auxiliar a strings de la columna nueva.   
    Fecha* pdate=NULL;            // pdate: puntero auxiliar a Fechas de la columna original.
    Columna* columna=df->columnas;// columna: puntero a columnas del dataframe.   
    xColumna=columna_index(df,nombre_columna); // Asigno el índice correspondiente.
    if(columna_index(df,nueva_columna)!=df->numColumnas) 
    {   // Si el nombre de la nueva columna ya fue tomado, da error
        printf(ROJO S "ERROR: EL NUEVO NOMBRE DE LA COLUMNA YA ESTA TOMADO" S);
        return;
    }
    if(xColumna==df->numColumnas)
    {   // Si la columna a analizar no fue encontrada, da error.
        printf(ROJO S ECNE S);
        return;
    }
    if(columna[xColumna].tipo!=FECHA)
    {   // Si la columna a analizar no es de tipo FECHA, da error.
        printf(ROJO S "ERROR: ES NECESARIO QUE LA COLUMNA SEA DEL TIPO FECHA" S);
        return;
    }
    df->numColumnas++; // Incremento la cantidad de columnas.
    // Reasigno la memoria de la columna a la nueva cantidad de columnas en el dataframe. 
    columna= (Columna*)realloc(columna,sizeof(Columna)*df->numColumnas); if(!columna) printf(ROJO S EAM S);
    nColumna=df->numColumnas-1;  // Asigno el índice correspondiente.
    columna[nColumna].tipo=TEXTO;// Determino el tipo de dato de la nueva columna (TEXTO).
    // Reservo memoria para las filas de la nueva columna.
    columna[nColumna].datos =(char**)malloc(sizeof(char*)*df->numFilas); if(!columna[nColumna].datos) printf(ROJO S EAM S);
    // Reservo memoria para el vector de nulos de la nueva columna.
    columna[nColumna].esNulo =(unsigned char*)malloc(sizeof(unsigned char)*df->numFilas); if(!columna[nColumna].esNulo) printf(ROJO S EAM S);
    columna[nColumna].numFilas=df->numFilas;// Igualo la cantidad de filas de la columna con el del dataframe.
    strcpy(columna[nColumna].nombre,nueva_columna); // Asigno el nombre indicado de la columna nueva.
    columna[nColumna].nombre[strlen(nueva_columna)]='\0'; // Inserto el caracter nulo al final.
    pdate=(Fecha*)columna[xColumna].datos;// Asigno el puntero de fechas de la columna original.
    pch=(char**)columna[nColumna].datos;  // Asigno el puntero de cadenas de la columna nueva.
    for(iFilas=0;iFilas<df->numFilas;iFilas++) // Recorro la fila entera.
    {   // Igualo los nulos en la nueva columna.
        columna[nColumna].esNulo[iFilas]=columna[xColumna].esNulo[iFilas];
        if(columna[nColumna].esNulo[iFilas]) // Evalúo si el valor es nulo o si no lo es.
        {   // Resevo memoria para el valor nulo.
            pch[iFilas] =(char*)malloc(sizeof(char)); if(!pch[iFilas]) printf(ROJO S EAM S);
            pch[iFilas][0]='\0'; // Valor nulo.
        }
        else
        {   // Reservo la cantidad de caracteres para indicar el trimestre.
            pch[iFilas] =(char*)malloc(sizeof(char)*(strlen("QN")+1)); if(!pch[iFilas]) printf(ROJO S EAM S);
            if(pdate[iFilas].tm_mon>=0 && pdate[iFilas].tm_mon<=2)      // Caso 1er trimestre.
                strcpy(pch[iFilas],"Q1");
            else if(pdate[iFilas].tm_mon>=3 && pdate[iFilas].tm_mon<=5) // Caso 2do trimestre.
                strcpy(pch[iFilas],"Q2");
            else if(pdate[iFilas].tm_mon>=6 && pdate[iFilas].tm_mon<=8) // Caso 3er trimestre.
                strcpy(pch[iFilas],"Q3");
            else if(pdate[iFilas].tm_mon>=9 && pdate[iFilas].tm_mon<=11)// Caso 4to trimestre.
                strcpy(pch[iFilas],"Q4");
            pch[iFilas][strlen("QN")]='\0'; // Inserto el caracter nulo al final.
        }
    }
    df->columnas=columna; // Apunto nuevamente las columnas.
}

//  FUNCIÓN: Elimina todas las filas con valores nulos en una columna gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.
void eliminar_filas_nulas(Dataframe* df,const char* nombre_columna)
{
    int nColumna=columna_index(df,nombre_columna); // nColumna: índice de la columna donde buscar nulos.
    Columna* columna=df->columnas;                 // columna: puntero auxiliar a las columnas.
    if(nColumna==df->numColumnas)   
    {   // Si la columna no se encuentra, da error.
        printf(ROJO S ECNE S);
        return;
    }
    while(fila_nula(columna,nColumna)<df->numFilas)     // Mientras que haya filas nulas,
        eliminar_fila(df,fila_nula(columna,nColumna));  // las elimino.
}

//  FUNCIÓN: Ordena la columna solicitada ascendentemente.
//  RETORNO: No devuelve.
void ordenar_asc(Dataframe* df,const char* nombre_columna)
{
    int iFilas,jFilas,nColumna,aux,*ind=df->indice;
    char** pch=NULL;    // pch: puntero auxiliar a textos.
    Fecha* pdate=NULL;  // pdate: puntero auxiliar a fechas.
    double* pnum=NULL;  // pnum: puntero auxiliar a números.
    Columna columna;    // columna: columna a ordenar.
    nColumna=columna_index(df,nombre_columna);
    if(nColumna==df->numColumnas)
    {   // Si la columna no se encuentra, da error.
        printf(ROJO S ECNE S);
        return;
    }
    columna=df->columnas[nColumna]; // Columna del dataframe a ordenar.
    switch (columna.tipo)           // Ordeno según el tipo.
    {
    case FECHA:
        pdate=(Fecha*)columna.datos;
        // Ordenación de índices burbuja ascendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if((pdate[ind[iFilas]].tm_year>pdate[ind[jFilas]].tm_year) || (pdate[ind[iFilas]].tm_mon>pdate[ind[jFilas]].tm_mon && pdate[ind[iFilas]].tm_year==pdate[ind[jFilas]].tm_year) || (pdate[ind[iFilas]].tm_mday>pdate[ind[jFilas]].tm_mday && pdate[ind[iFilas]].tm_mon==pdate[ind[jFilas]].tm_mon && pdate[ind[iFilas]].tm_year==pdate[ind[jFilas]].tm_year))
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;
    
    case TEXTO:
        pch=(char**)columna.datos;
        // Ordenación de índices burbuja ascendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if(strcmp(pch[ind[iFilas]], pch[ind[jFilas]]) > 0)
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;

    case NUMERICO:
        pnum=(double*)columna.datos;
        // Ordenación de índices burbuja ascendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if(pnum[ind[iFilas]]>pnum[ind[jFilas]])
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;    
    }
}

//  FUNCIÓN: Ordena la columna solicitada descendentemente.
//  RETORNO: No devuelve.
void ordenar_des(Dataframe* df,const char* nombre_columna)
{
    int iFilas,jFilas,nColumna,aux,*ind=df->indice;
    char** pch=NULL;    // pch: puntero auxiliar a textos.
    Fecha* pdate=NULL;  // pdate: puntero auxiliar a fechas.
    double* pnum=NULL;  // pnum: puntero auxiliar a números.
    Columna columna;    // columna: columna a ordenar.
    nColumna=columna_index(df,nombre_columna);
    if(nColumna==df->numColumnas)
    {   // Si la columna no se encuentra, da error.
        printf(ROJO S ECNE S);
        return;
    }
    columna=df->columnas[nColumna]; // Columna del dataframe a ordenar.
    switch (columna.tipo)           // Ordeno según el tipo.
    {
    case FECHA:
        pdate=(Fecha*)columna.datos;
        // Ordenación de índices burbuja descendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if((pdate[ind[iFilas]].tm_year<pdate[ind[jFilas]].tm_year) || (pdate[ind[iFilas]].tm_mon<pdate[ind[jFilas]].tm_mon && pdate[ind[iFilas]].tm_year==pdate[ind[jFilas]].tm_year) || (pdate[ind[iFilas]].tm_mday<pdate[ind[jFilas]].tm_mday && pdate[ind[iFilas]].tm_mon==pdate[ind[jFilas]].tm_mon && pdate[ind[iFilas]].tm_year==pdate[ind[jFilas]].tm_year))
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;
    
    case TEXTO:
        pch=(char**)columna.datos;
        // Ordenación de índices burbuja descendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if(strcmp(pch[ind[iFilas]], pch[ind[jFilas]]) < 0)
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;

    case NUMERICO:
        pnum=(double*)columna.datos;
        // Ordenación de índices burbuja descendente.
        for(iFilas=0;iFilas<columna.numFilas;iFilas++)
        {
            for(jFilas=iFilas+1;jFilas<columna.numFilas;jFilas++)
            {
                if(pnum[ind[iFilas]]<pnum[ind[jFilas]])
                {
                    aux = ind[jFilas];
                    ind[jFilas] = ind[iFilas];
                    ind[iFilas] = aux;
                }
            }
        }
        break;    
    }
}

//  FUNCIÓN: Identifica que tipo de dato es el de la columna a filtrar y se llama a la respectiva función.
//  RETORNO: No devuelve.
void filtros(Dataframe* df, const char* nombre_columna, const char* comp, const char* refch)
{
    if(columna_index(df,nombre_columna)==df->numColumnas)
    {   // Si la columna a filtrar no se encuentra, da error.
        printf(ROJO S ECNE S);
        return;
    }
    switch (df->columnas[columna_index(df,nombre_columna)].tipo)
    {   // Según el tipo, selecciono el filtrado.
    case TEXTO:
        filtro_txt(df,nombre_columna,comp,refch); // Filtrado para TEXTO.
        break;
    
    case NUMERICO:
        filtro_num(df,nombre_columna,comp,refch); // Filtrado para NUMERICO.
        break;
    
    case FECHA:
        filtro_fch(df,nombre_columna,comp,refch); // Filtrado para FECHA.
        break;
    }
}

//  FUNCIÓN: Genera un fichero csv a partir de un dataframe.
//  RETORNO: No devuelve.
void exportar_csv(Dataframe* df,const char* NombreArchivo)
{
    int iFilas,iColumnas,nchars=0,l=strlen(NombreArchivo);
    char aux[20],**pch=NULL; // aux: buffer para cargar los numeros y fechas.
    double* pnum=NULL;
    Fecha* pdate=NULL;
    Columna* columna=df->columnas;
    FILE* pf = fopen(NombreArchivo,"w"); if(!pf) printf(ROJO S EAF S); // Puntero al archivo.
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++)
    {   // Guardo los nombres de las columnas.
        fwrite(columna[iColumnas].nombre,sizeof(char),strlen(columna[iColumnas].nombre),pf);
        if(iColumnas<(df->numColumnas-1))
            fwrite(",",sizeof(char),1,pf); // Separo por comas entre columnas.
        else
            fwrite("\n",sizeof(char),1,pf);// Salto de última columna.
    }
    for(iFilas=0;iFilas<df->numFilas;iFilas++)
    {   // Recorro por filas.
        for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++)
        {   // Recorro por columnas.
            switch (columna[iColumnas].tipo) // Guardo valores según el tipo.
            {
            case TEXTO:
                pch = (char**)columna[iColumnas].datos;
                fwrite(pch[iFilas],sizeof(char),strlen(pch[iFilas]),pf); // Guardo TEXTO.
                break;
            
            case NUMERICO:
                pnum = (double*)columna[iColumnas].datos;
                sprintf(aux,"%f",pnum[iFilas]);
                fwrite(aux,sizeof(char),strlen(aux),pf); // Guardo NUMERICO.
                break;

            case FECHA:
                pdate = (Fecha*)columna[iColumnas].datos;
                sprintf(aux,"%i",pdate[iFilas].tm_year+1900);
                fwrite(aux,sizeof(char),strlen(aux),pf); // Guardo año de FECHA.
                sprintf(aux,"-%i",pdate[iFilas].tm_mon+1);
                fwrite(aux,sizeof(char),strlen(aux),pf); // Guardo mes de FECHA.
                sprintf(aux,"-%i",pdate[iFilas].tm_mday);
                fwrite(aux,sizeof(char),strlen(aux),pf); // Guardo día de FECHA.
                break;
            }
            strcpy(aux,""); // Vacío el buffer.
            if(iColumnas<(df->numColumnas-1))
                fwrite(",",sizeof(char),1,pf); // Separo por comas.
            else
                fwrite("\n",sizeof(char),1,pf);// Salto de última columna.
        }
    }
    fclose(pf); // Cierro el puntero.
}

/*FUNCIONES DE MANEJO CON STRINGS*/

//  FUNCIÓN: Eliminar los espacios ubicados al incio y al final de un string.
//  RETORNO: No devuelve.
void trim(char *cad)
{
    int j;
    while(cad[0]==' ')            // Mientras haya un espacio al inicio.
        for(j=0;j<strlen(cad);j++)
            cad[j]=cad[j+1];      // Elimino.
    while(cad[strlen(cad)-1]==' ')// Mientras haya un espacio al final.
        cad[strlen(cad)-1]='\0';  // Elimino.
}

//  FUNCIÓN: Determinar si un string representa un número natural.
//  RETORNO: Devuelve '1' si el string es un número natural y '0' si no.
int validar_numero_natural(const char *str)
{
    int i=0, nNumeros=0;
    char *cad=NULL;

    cad=(char*)malloc(sizeof(char)*strlen(str)); // Reservo para la cadena a analizar.
    strcpy(cad,str);
    trim(cad);

    if(cad==NULL || cad[0]=='\0')
    {   // Si la cadena es nula o está vacia, no es un número.
        free(cad);
        return 0;
    }
    
    while(cad[i]!='\0')
    {   // Recorro mientras el caracter sea distinto del caracter nulo.
        if(cad[i]<'0' || cad[i]>'9')
        {   // Si el caracter no está entre el 0 y 9, no es un número.
            free(cad);
            return 0;
        }
        else
            nNumeros++; // Aumento la cantidad de números contados.
        i++;            // Paso al siguiente caracter.
    }
    free(cad);
    if(nNumeros!=0)     // Siempre que haya al menos un número, puede ser un número.
        return 1;
    return 0;
}

//  FUNCIÓN: Determinar si un string representa un número entero.
//  RETORNO: Devuelve '1' si el string es un número entero y '0' si no.
int validar_numero_entero(const char *str)
{
    int i=0, nNumeros=0;
    char *cad=NULL;

    cad=(char*)malloc(sizeof(char)*strlen(str)); // Reservo para la cadena a analizar.
    strcpy(cad,str);
    trim(cad);

    if(cad==NULL || cad[0]=='\0')
    {   // Si la cadena es nula o está vacia, no es un número.
        free(cad);
        return 0;
    }
    if(cad[0]=='-' || cad[0]=='+')
        i=1; // Si el primer caracter es un signo, puede ser un número.
    while(cad[i]!='\0')
    {   // Recorro mientras el caracter sea distinto del caracter nulo.
        if(cad[i]<'0' || cad[i]>'9')
        {   // Si el caracter no está entre el 0 y 9, no es un número.
            free(cad);
            return 0;
        }
        else
            nNumeros++; // Aumento la cantidad de números contados.
        i++;            // Paso al siguiente caracter.
    }
    free(cad);
    if(nNumeros!=0)     // Siempre que haya al menos un número, puede ser un número.
        return 1;
    return 0;
}

//  FUNCIÓN: Determinar si un string representa un número real.
//  RETORNO: Devuelve '1' si el string es un número real y '0' si no.
int validar_numero(const char *str)
{
    int i=0, nPuntos=0, nNumeros=0;
    char *cad=NULL;

    cad=(char*)malloc(sizeof(char)*(strlen(str)+1)); // Reservo para la cadena a analizar.
    strcpy(cad,str);
    trim(cad);

    if(cad==NULL || cad[0]=='\0')
    {   // Si la cadena es nula o está vacia, no es un número.
        free(cad);
        return 0;
    }
    if(cad[0]=='-' || cad[0]=='+')
        i=1; // Si el primer caracter es un signo, puede ser un número.
    while(cad[i]!='\0')
    {   // Recorro mientras el caracter sea distinto del caracter nulo.
        if(cad[i]=='.')
        {
            nPuntos++; // Cuento los puntos en la cadena.
            if(nPuntos>1)
            {   // Si hay más de un punto, no es un número.
                free(cad);
                return 0;
            }
        }
        else if(cad[i]<'0' || cad[i]>'9')
        {   // Si el caracter no está entre el 0 y 9, no es un número.
            free(cad);
            return 0;
        }
        else
            nNumeros++; // Aumento la cantidad de números contados.
        i++;            // Paso al siguiente caracter.
    }
    free(cad);
    if(nNumeros!=0)     // Siempre que haya al menos un número, puede ser un número.
        return 1;
    return 0;
}

//  FUNCIÓN: Determinar si un string representa una fecha con el formato (AAAA-MM-DD) ó (AAAA/MM/DD).
//  RETORNO: Devuelve '1' si el string es una fecha y '0' si no.
int validar_fecha(const char *comando)
{
    int i, a, m, d; // a:año, m:mes, d:día.
    if(strlen(comando)!=10)
		return 0;   // Si el tamaño del string es mayor a 10 (AAAA/MM/DD), no es una fecha.
	else
	{  
		if((comando[4]!='/' && comando[4]!='-') || comando[4]!=comando[7])
	    	return 0;   // Si los separadores no son '/' y '-', da error.
		for(i=0 ; i<10 ; i++)
		{   // Recorro el string.
			if(i==4 || i==7)
				continue; // Me salto los separadores.
			if(comando[i]<'0' || comando[i]>'9')
				return 0; // Cada caracter que no sea separador, debe ser un número.
		}
		d=(comando[8]-'0')*10 + comando[9]-'0'; // Extraigo el día.
		m=(comando[5]-'0')*10 + comando[6]-'0'; // Extraigo el mes.
        // Extraigo el año.
		a=(comando[0]-'0')*1000 + (comando[1]-'0')*100 + (comando[2]-'0')*10 + (comando[3]-'0');
		if(a<1 || m<1 || m >12 || d<1 || d>31)
			return 0;   // Si el mes no está entre 1 y 12, y el día entre 1 y 31, no es una fecha.
		if( (m==4 || m==6 || m==9 || m==11) && d>30 )
			return 0;   // Si para esos meses, no tienen 30 días, no es una fecha.
		if(m==2)
		{   // Caso años bisiestos.
			if(a%400==0 || ((a%4)==0 && (a%100)!=0))
			{
				if(d>29)
					return 0;
			}
			else
			{
				if(d>28)
					return 0;
			}
		}
		return 1;   // Si después de todo no da 0, es una fecha válida.
	}
}

/*FUNCIONES DE BÚSQUEDA*/

//  FUNCIÓN: Recorre la lista hasta el nodo indicado.
//  RETORNO: Devuelve el puntero al nodo de la posición que se indique en n siempre que existan, de lo contrario devuelve NULL.
Nodo* recorrer_lista(Lista* lista, int n) 
{
    int index=0;
    Nodo* nodo=lista->primero; // Nodo al inicio de la lista.
    if(n>lista->numDFs) // Si el número es mayor a la cantidad de dataframes, da error. 
        return NULL;
    while(index<n)  // Termina de recorrer cuando se llega al número.
    {
        nodo=nodo->siguiente;
        index++;
    }
    return nodo;    // Devuelvo el nodo encontrado.
}

//  FUNCIÓN: Ubica la columna de un dataframe a partir de su nombre.
//  RETORNO: Devuelve el índice de la columna. En caso que esta no se encuentre, devuelve el número total de columnas.
int columna_index(Dataframe* df,const char* nombre_columna)
{
    int iColumnas;
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++)  // Recorro las columnas.
        if(!strcmp(nombre_columna,df->columnas[iColumnas].nombre)) // Si coincide el número,
            return iColumnas;                                      // Devuelvo el índice.
    return iColumnas; // Caso que no encuentre coincidencia, devuelvo nº total de columnas.
}

//  FUNCIÓN: Ubica la primera fila nula de una columna.
//  RETORNO: Devuelve el índice de la fila. En caso que esta no se encuentre, devuelve el número total de filas.
int fila_nula(Columna* columna,int nColumna)
{
    int iFilas;
    unsigned char* nulos=NULL;
    nulos=columna[nColumna].esNulo; // Puntero a vector de nulos.
    for(iFilas=0;iFilas<columna->numFilas;iFilas++) // Recorro por filas.
        if(nulos[iFilas])   // Si hay un nulo,
            return iFilas;  // devuelvo el índice.
    return columna->numFilas; // Devuelvo, en caso de no coincidencia, nº total de filas. 
}

//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo TEXTO.
//  RETORNO: No devuelve.
void filtro_txt(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch)
{
    int iFilas,nColumna=columna_index(df,nombre_columna),flag=1;
    char** pch=NULL;
    if(validar_fecha(refch) || validar_numero(refch))
    {   // Todo lo que no sea del tipo TEXTO, da error.
        printf(ROJO S "ERROR: EL VALOR DE COMPARACION DEBE TENER EL MISMO TIPO QUE LA COLUMNA A ANALIZAR"S);
        return;
    }
    if(strcmp(comp,"eq") && strcmp(comp,"neq") && strcmp(comp,"lt") && strcmp(comp,"gt"))
    {   // Todo criterio de comparación que no sea los indicados, da error.
        printf(ROJO S "ERROR: EL TIPO DE COMPARACION NO ES VALIDA" S);
        return;
    }
    pch=(char**)df->columnas[nColumna].datos;
    for(iFilas=0;iFilas<df->numFilas;iFilas++)
    {   // Recorro por filas.
        if(!strcmp(comp,"eq"))
        {   // Caso "iguales a" .
            if(strcmp(pch[iFilas],refch))
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"neq"))
        {   // Caso "distintos a" .    
            if(!strcmp(pch[iFilas],refch))
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"lt"))
        {   // Caso "menores a" .    
            if(strcmp(pch[iFilas],refch)<0)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"gt"))
        {   // Caso "mayores a" .    
            if(strcmp(pch[iFilas],refch)>0)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
    }
}

//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo NUMÉRICO.
//  RETORNO: No devuelve.
void filtro_num(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch)
{
    int iFilas,nColumna=columna_index(df,nombre_columna);
    double* pnum=NULL,ref=atof(refch);
    if(!validar_numero(refch))
    {   // Todo lo que no sea del tipo NUMERICO, da error.
        printf(ROJO S "ERROR: EL VALOR DE COMPARACION DEBE TENER EL MISMO TIPO QUE LA COLUMNA A ANALIZAR"S);
        return;
    }
    if(strcmp(comp,"eq") && strcmp(comp,"neq") && strcmp(comp,"lt") && strcmp(comp,"gt"))
    {   // Todo criterio de comparación que no sea los indicados, da error.
        printf(ROJO S "ERROR: EL TIPO DE COMPARACION NO ES VALIDA" S);
        return;
    }
    pnum=(double*)df->columnas[nColumna].datos;
    for(iFilas=0;iFilas<df->numFilas;iFilas++)
    {   // Recorro por filas.
        if(!strcmp(comp,"eq"))
        {   // Caso "iguales a" .
            if(pnum[iFilas]!=ref)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }  
        }
        else if(!strcmp(comp,"neq"))
        {   // Caso "distintos a" .   
            if(pnum[iFilas]==ref)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"lt"))
        {   // Caso "menores a" . 
            if(pnum[iFilas]>ref)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"gt"))
        {   // Caso "mayores a" .    
            if(pnum[iFilas]<ref)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
    }
}

//  FUNCIÓN: Filtra las filas con los criterios de valor y tipo de comparación en una columna de tipo FECHA.
//  RETORNO: No devuelve.
void filtro_fch(Dataframe* df,const char* nombre_columna,const char* comp,const char* refch)
{
    int iFilas,nColumna=columna_index(df,nombre_columna);
    Fecha* pdate=NULL, ref;
    if(!validar_fecha(refch))
    {   // Todo lo que no sea del tipo FECHA, da error.
        printf(ROJO S "ERROR: EL VALOR DE COMPARACION DEBE TENER EL MISMO TIPO QUE LA COLUMNA A ANALIZAR (AAAA/MM/DD)"S);
        return;
    }
    if(strcmp(comp,"eq") && strcmp(comp,"neq") && strcmp(comp,"lt") && strcmp(comp,"gt"))
    {   // Todo criterio de comparación que no sea los indicados, da error.
        printf(ROJO S "ERROR: EL TIPO DE COMPARACION NO ES VALIDA" S);
        return;
    }
    pdate=(Fecha*)df->columnas[nColumna].datos;
    // Separo la referencia.
    // Separo los días.
    ref.tm_mday= (refch[8]-'0')*10 + refch[9]-'0';
    // Separo los meses.
    ref.tm_mon= ((refch[5]-'0')*10 + refch[6]-'0')-1;
    // Separo los años.
    ref.tm_year= ((refch[0]-'0')*1000 + (refch[1]-'0')*100 + (refch[2]-'0')*10 + (refch[3]-'0'))-1900;
    for(iFilas=0;iFilas<df->numFilas;iFilas++)
    {   // Recorro por filas.
        if(!strcmp(comp,"eq"))
        {   // Caso "iguales a" .
            if(ref.tm_year!=pdate[iFilas].tm_year || ref.tm_mon!=pdate[iFilas].tm_mon || ref.tm_mday!=pdate[iFilas].tm_mday)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"neq"))
        {   // Caso "distintos a" .  
            if(ref.tm_year==pdate[iFilas].tm_year && ref.tm_mon==pdate[iFilas].tm_mon && ref.tm_mday==pdate[iFilas].tm_mday)
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"lt"))
        {   // Caso "menores a" .  
            if(pdate[iFilas].tm_year>ref.tm_year || (pdate[iFilas].tm_year==ref.tm_year && pdate[iFilas].tm_mon>ref.tm_mon) || (pdate[iFilas].tm_year==ref.tm_year && pdate[iFilas].tm_mon==ref.tm_mon && pdate[iFilas].tm_mday>ref.tm_mday))
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
        else if(!strcmp(comp,"gt"))
        {   // Caso "mayores a" .  
            if(pdate[iFilas].tm_year<ref.tm_year || (pdate[iFilas].tm_year==ref.tm_year && pdate[iFilas].tm_mon<ref.tm_mon) || (pdate[iFilas].tm_year==ref.tm_year && pdate[iFilas].tm_mon==ref.tm_mon && pdate[iFilas].tm_mday<ref.tm_mday))
            {
                eliminar_fila(df,iFilas);
                iFilas--;
            }
        }
    }
}

/*FUNCIONES DE CARGA DEL DATAFRAME*/

//  FUNCIÓN: Copia todo el contenido de un archivo csv en un string y cuenta nºfilas y nºcolumnas.
//  RETORNO: Devuelve una estructura tipo ficheroCSV(String con el contenido,nº filas y nº columnas).
ficheroCSV* copiar_csv(const char* NombreArchivo)
{
    int l=strlen(NombreArchivo),nchars=0; //l: largos del nombre y de todo el archivo, nchars: índice para recorrer el string
    char* buffer=NULL; //buffer: donde se carga el archivo
    ficheroCSV* fichero=(ficheroCSV*)malloc(sizeof(ficheroCSV)); if(!fichero) printf(ROJO S EAM S); //Reservo una estructura del tipo ficheroCSV
    fichero->numFilas=0;    //Inicializo a 0 la cantidad de filas
    fichero->numColumnas=0; //Inicializo a 0 la cantidad de columnas

    //Descarto archivos que no tengan formato csv
    if(NombreArchivo[l-1]!='v' || NombreArchivo[l-2]!='s' || NombreArchivo[l-3]!='c' || NombreArchivo[l-4]!='.')
    {
        printf(ROJO S EAF S);
        return NULL;
    }

    FILE* pf = fopen(NombreArchivo,"r"); if(!pf) {printf(ROJO S EAF S);  return NULL;} //Abro el archivo
    fseek (pf , 0 , SEEK_END); //Sitúo el puntero al final del archivo
    l = ftell (pf);            //Guardo la posicón del puntero al final del archivo (Última posición)
    rewind (pf);               //Retrocedo el puntero al principio del archivo
    buffer = (char*)malloc(sizeof(char)*(l+2)); if(!buffer) printf(ROJO S EAM S);   //Reservo memoria para el string del archivo
    while(!feof(pf))           //Leo hasta el END OF FILE (EOF)
    {
        buffer[nchars]=fgetc(pf);         //Leo caracter a caracter
        if((buffer[nchars]=='\n'))        //Incremento en uno las filas por cada salto de línea
        {
            if(fichero->numFilas==0)      //Tomo en cuenta la última columna que no tiene coma para terminar
                fichero->numColumnas++;
            fichero->numFilas++;          //Por cada salto tengo una fila
        }
        if(buffer[nchars]==',' && fichero->numFilas==0) //Incremento la cantidad de columnas por cada coma, pero sólo las cuento en la primera fila
            fichero->numColumnas++;
        nchars++;                         //Incremento en uno la variable de recorrer el string
    }
    buffer[nchars]='\0';      //Inserto al final string que lee el archivo
    fichero->datos = buffer;  //Paso el buffer a la estructura tipo ficheroCSV
    fclose(pf);               //Cierro el archivo
    return fichero;           //Devuelvo el fichero
}

//  FUNCIÓN: Carga toda la estructura del ficheroCSV en un Dataframe.
//  RETORNO: Devuelve el dataframe cargado.
//REVISAR CASO FALLOS EN 1ERA FILA Y 2DA, CON LIBERACIÓN DINÁMICA
Dataframe* cargar_dataframe(ficheroCSV* fichero)
{
    char* dato = fichero->datos, *aux, elemento[100], **pch; //pch: Puntero auxiliar, aux: Puntero de recorrido string, elemento: elemento individual extraido del fichero
    double* pnum; //Puntero auxiliar
    Fecha* pdate; //Puntero auxiliar
    int iFilas=0,iColumnas=0; //iFilas: index filas, iColumnas: index columnas

    //Reservo memoria para el dataframe
    Dataframe* df = (Dataframe*)malloc(sizeof(Dataframe)); if(!df) printf(ROJO S EAM S);  
    //Reservo memoria para las columnas (columnas != columna)
    Columna* columna = (Columna*)malloc(sizeof(Columna)*fichero->numColumnas); if(!columna) printf(ROJO S EAM S);
    //Reservo espacio para el vector índice
    df->indice = (int*)malloc(sizeof(int)*(fichero->numFilas-1)); if(!df->indice) printf(ROJO S EAM S);
    df->numColumnas=fichero->numColumnas;
    df->numFilas=(fichero->numFilas-1);

    for(iFilas=0;iFilas<fichero->numFilas;iFilas++) // Recorro por filas
    {
        for(iColumnas=0;iColumnas<fichero->numColumnas;iColumnas++) // Recorro por columnas
        {
            if(iColumnas==(fichero->numColumnas-1)) 
                aux =strchr(dato,'\n');             // Tomo hasta el salto de línea '\n'
            else
                aux =strchr(dato,',');              // Tomo hasta la próxima coma ','
            strncpy(elemento,dato,aux-dato);        // Copio en un string el recorte
            elemento[aux-dato]='\0';                // Agrego manualmente el caracter nulo
            dato=aux+1;                             // Avanzo el puntero para salir de la coma o salto anterior
            trim(elemento);                         // Para evitar errores le quito los espacios en los extremos
            if(iFilas == 0)       // Caso de la primera fila (nombres)
            {
                columna[iColumnas].numFilas = (fichero->numFilas-1); // Indico la cantidad de filas de la columna(tiene que ser igual a la del dataframe)
                // Reservo espacio para el vector de nulos
                columna[iColumnas].esNulo = (unsigned char*)malloc(sizeof(unsigned char)*(columna[iColumnas].numFilas)); if(!columna[iColumnas].esNulo) printf(ROJO S EAM S);
                if(!elemento[0]) // Indico error de valores nulos en primera fila
                {
                    printf(ROJO S "ERROR: Los valores nulos no son aceptados en la primera ni segunda fila" S);
                    //Libero memoria
                    while(iColumnas>=0)
                    {
                        free(columna[iColumnas].esNulo);
                        iColumnas--;
                    }
                    free(columna);
                    free(df->indice);
                    free(df);
                    free(fichero->datos);
                    free(fichero);
                    return NULL;
                }
                strcpy(columna[iColumnas].nombre,elemento); // Copio el elemnto en el nombre de la columna
            }          
            else if(iFilas == 1) // Caso de la segunda fila (determino tipos y relleno datos)
            {
                if(!elemento[0]) // Indico error de valores nulos en segunda fila
                {
                    printf(ROJO S "ERROR: Los valores nulos no son aceptados en la primera ni segunda fila" S);
                    //Libero memoria
                    iColumnas--;
                    while(iColumnas>=0)
                    {
                        free(columna[iColumnas].datos);
                        iColumnas--;
                    }
                    iColumnas=(df->numColumnas);
                    while(iColumnas>=0)
                    {
                        free(columna[iColumnas].esNulo);
                        iColumnas--;
                    }
                    free(columna);
                    free(df->indice);
                    free(df);
                    free(fichero->datos);
                    free(fichero);
                    return NULL;
                }
                else if(validar_fecha(elemento)) // Certifico que sea una fecha
                {
                    columna[iColumnas].tipo = FECHA;
                    // Reservo una columna de datos del tipo Fecha con la cantidad de filas del dataframe
                    columna[iColumnas].datos = (Fecha*)malloc(sizeof(Fecha)*(columna[iColumnas].numFilas)); if(!columna[iColumnas].datos) printf(ROJO S EAM S);
                }
                else if(validar_numero(elemento))// Certifico que sea un número
                {
                    columna[iColumnas].tipo = NUMERICO;
                    // Reservo una columna de datos del tipo double con la cantidad de filas del dataframe
                    columna[iColumnas].datos = (double*)malloc(sizeof(double)*(columna[iColumnas].numFilas)); if(!columna[iColumnas].datos) printf(ROJO S EAM S);
                }
                else
                {
                    columna[iColumnas].tipo = TEXTO;
                    // Reservo una columna de datos del tipo strings con la cantidad de filas del dataframe
                    columna[iColumnas].datos = (char**)malloc(sizeof(char*)*(columna[iColumnas].numFilas)); if(!columna[iColumnas].datos) printf(ROJO S EAM S);
                }
                columna[iColumnas].esNulo[iFilas-1]=0;
            }
            if(iFilas!=0) // Caso de las siguientes filas (relleno datos)
            {
                switch (columna[iColumnas].tipo)
                {
                case FECHA:
                    pdate = (Fecha*)columna[iColumnas].datos;
                    if(!validar_fecha(elemento)) // Si el tipo de dato no es igual al tipo de la columna, es nulo
                    {
                        columna[iColumnas].esNulo[iFilas-1]=1;
                        pdate[iFilas-1].tm_mday= 0;
                        pdate[iFilas-1].tm_mon= 0;
                        pdate[iFilas-1].tm_year= 0;
                    }
                    else
                    {
                        columna[iColumnas].esNulo[iFilas-1]=0;
                        pdate[iFilas-1].tm_mday= (elemento[8]-'0')*10 + elemento[9]-'0';
                        pdate[iFilas-1].tm_mon= ((elemento[5]-'0')*10 + elemento[6]-'0')-1;
                        pdate[iFilas-1].tm_year= ((elemento[0]-'0')*1000 + (elemento[1]-'0')*100 + (elemento[2]-'0')*10 + (elemento[3]-'0'))-1900;
                    }
                    break;
                
                case NUMERICO:
                    pnum = (double*)columna[iColumnas].datos;
                    if(!validar_numero(elemento)) // Si el tipo de dato no es igual al tipo de la columna, es nulo
                    {
                        columna[iColumnas].esNulo[iFilas-1]=1;
                        pnum[iFilas-1]=0;
                    }
                    else
                    {
                        columna[iColumnas].esNulo[iFilas-1]=0;
                        pnum[iFilas-1]= atof(elemento);
                    }
                    break;

                case TEXTO:
                    pch = (char**)columna[iColumnas].datos;
                    if(!elemento[0]) // Si el tipo de dato no es igual al tipo de la columna, es nulo
                    {
                        columna[iColumnas].esNulo[iFilas-1]=1;
                        pch[iFilas-1]= (char*)malloc(sizeof(char)); if(!pch[iFilas-1]) printf(ROJO S EAM S);
                        pch[iFilas-1][0]='\0';
                    }
                    else
                    {
                        columna[iColumnas].esNulo[iFilas-1]=0;
                        // Reservo memoria para los caracteres del string
                        pch[iFilas-1]= (char*)malloc(sizeof(char)*(strlen(elemento)+1)); if(!pch[iFilas-1]) printf(ROJO S EAM S);
                        strcpy(pch[iFilas-1],elemento);
                    }
                    break;
                
                default:
                    printf(ROJO S"ERROR: Mala asignación de tipos en las columnas del DATAFRAME"S);
                    break;
                }
            }
        }
        if(iFilas>0) //Relleno el vector indice
            df->indice[iFilas-1]=iFilas-1;
    }
    free(fichero->datos);
    free(fichero);
    df->columnas = columna;
    return df;
}

/*FUNCIONES DE ELIMINACIÓN DINÁMICA*/

//  FUNCIÓN: Elimina una columna gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.
void eliminar_columna(Dataframe* df,const char* nombre_columna)
{
    int iFilas,iColumnas,nColumna;
    char** pch=NULL;
    Columna* columna=df->columnas,aux;
    nColumna = columna_index(df,nombre_columna); // Columna a eliminar.
    if(nColumna==df->numColumnas)
    {   // Si no encuentra la columna, da error.
        printf(ROJO S ECNE S);
        return;
    }
    for(iColumnas=nColumna;iColumnas<df->numColumnas;iColumnas++)
    {   // Recorro por columnas.
        if(iColumnas==nColumna)
            aux=columna[iColumnas];
        columna[iColumnas]=columna[iColumnas+1]; // Muevo la columna a eliminar hacia el final.
        if(iColumnas==(df->numColumnas-1))
        {
            if(columna[iColumnas].tipo==TEXTO)
            {   // Libero los strings si es del tipo TEXTO.
                pch=(char**)columna[iColumnas].datos;
                for(iFilas=0;iFilas<(df->numFilas-1);iFilas++)
                    free(pch[iFilas]);
            }
            free(aux.datos);    // Libero filas.
            free(aux.esNulo);   // Libero vector de nulos.
            df->numColumnas--;  // Decremento la cantidad de columnas.
            // Reasigno las columnas.
            columna = (Columna*)realloc(df->columnas,sizeof(Columna)*df->numColumnas); if(!columna) printf(ROJO S EAM S);
        }
    }
}

//  FUNCIÓN: Elimina una fila gestionando dinámicamente la memoria utilizada.
//  RETORNO: No devuelve.
void eliminar_fila(Dataframe* df, int nFila)
{
    int iColumnas,iFilas,inderror=df->numFilas,*i=df->indice;
    double* pnum=NULL;
    char** pch=NULL, *aux=NULL;
    Fecha* pdate=NULL;
    Columna* columna=df->columnas;
    for(iColumnas=0;iColumnas<df->numColumnas;iColumnas++)
    {   // Recorro por columnas.
        for(iFilas=nFila;iFilas<(columna[iColumnas].numFilas-1);iFilas++)
        {   // Recorro por filas.
            switch (columna[iColumnas].tipo)
            {   // Manejo según el tipo.
            case TEXTO:
                pch =(char**)columna[iColumnas].datos;
                if(iFilas==nFila) // Libera el string de la fila a eliminar.
                    aux=pch[iFilas];
                pch[iFilas]=pch[iFilas+1]; // Corro la fila a eliminar al final.
                break;

            case FECHA:
                pdate =(Fecha*)columna[iColumnas].datos;
                pdate[iFilas]=pdate[iFilas+1]; // Corro la fila a eliminar al final.
                break;

            case NUMERICO:
                pnum =(double*)columna[iColumnas].datos;
                pnum[iFilas]=pnum[iFilas+1]; // Corro la fila a eliminar al final.
                break;                
                
            }
             // Corro la fila a eliminar al final.
            columna[iColumnas].esNulo[iFilas]=columna[iColumnas].esNulo[iFilas+1];
        }
        if(iColumnas==0) // Decremento la cantidad de filas.
            df->numFilas--;
        if(df->numFilas==0)
        {   // Si se vacía el dataframe, se avisa.
            printf(ROJO S"EL DATAFRAME HA QUEDADO VACIO"S);
            return;
        }
        columna[iColumnas].numFilas=df->numFilas; // Igualo el nº Filas para cada columna.
        // Reasigno la memoria para el vector de nulos.
        columna[iColumnas].esNulo=(unsigned char*)realloc(columna[iColumnas].esNulo,sizeof(unsigned char)*(columna[iColumnas].numFilas)); if(!columna[iColumnas].esNulo) printf(ROJO S EAM S);
        switch (columna[iColumnas].tipo)
        {   // Reasigno los datos según el tipo.
        case TEXTO:
            free(aux);  // Libero el string.
            pch =(char**)columna[iColumnas].datos;
            pch =(char**)realloc(pch,sizeof(char*)*(columna[iColumnas].numFilas)); if(!pch) printf(ROJO S EAM S);
            break;
            
        case FECHA:
            pdate =(Fecha*)columna[iColumnas].datos;
            pdate =(Fecha*)realloc(pdate,sizeof(Fecha)*(columna[iColumnas].numFilas)); if(!pdate) printf(ROJO S EAM S);
            break;
            
        case NUMERICO:
            pnum =(double*)columna[iColumnas].datos;
            pnum =(double*)realloc(pnum,sizeof(double)*(columna[iColumnas].numFilas)); if(!pnum) printf(ROJO S EAM S);
            break;
        }
    }
    for(iFilas=0;iFilas<=df->numFilas;iFilas++)
        if(i[iFilas]==nFila)
            inderror=iFilas;    // Encuentro el índice a tomar de referencia.
    for(iFilas=inderror;iFilas<df->numFilas;iFilas++)
        i[iFilas]=i[iFilas+1];  // Muevo los indices al final.
    for(iFilas=0;iFilas<df->numFilas;iFilas++)
        if(i[iFilas]>nFila)     // Decremento los índices para que sean consecutivos.
            i[iFilas]--;
    // Reasigno el vector de índices.
    i = (int*)realloc(i,sizeof(int)*df->numFilas); if(!i) printf(ROJO S EAM S);
    df->indice=i;           // Reapunto el índice.
    df->columnas=columna;   // Reapunto la columna.
}

//  FUNCIÓN: Libera toda la memoria reservada para el dataframe.
//  RETORNO: No devuelve.
void liberar_dataframe(Dataframe* df)
{
    int i,j;
    char** pch=NULL;
    for(i=0;i<df->numColumnas;i++)
    {   // Recorro por columnas.
        if(df->columnas[i].tipo==TEXTO)
        {   // Libero strings para las columnas tipo TEXTO.
            pch=(char**)df->columnas[i].datos;
            for(j=0;j<(df->numFilas-1);j++)
                free(pch[j]); 
        }
        free(df->columnas[i].datos);   // Libero todos los datos.    
        free(df->columnas[i].esNulo);  // Libero los vectores de nulos.
    }
    free(df->columnas); // Libero las columnas.
    free(df->indice);   // Libero el índice.
    free(df);           // Libero el dataframe.
}

//  FUNCIÓN: Libera toda la memoria reservada para la lista de dataframes, incluyéndolos.
//  RETORNO: No devuelve.
void liberar_lista(Lista* lista)
{
    Nodo* nodo_aux=NULL;
    while((*lista).numDFs)
    {   // Recorro toda la lista de dataframes.
        nodo_aux = recorrer_lista(lista,((*lista).numDFs-1));
        if(nodo_aux->df)
            liberar_dataframe(nodo_aux->df);// Libero dataframe actual.
        free(nodo_aux);                     // Libero el nodo.
        (*lista).numDFs-=1;                 // Decremento la cantidad de dataframes.
    }
    free(lista);                            // Libero la lista.
}
