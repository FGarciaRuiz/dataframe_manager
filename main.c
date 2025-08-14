#include "lib.h"

int main()
{
    char comando[500], *arg[5], nombres[51];
    int nTok, nDF;
    Lista* lista=(Lista*)malloc(sizeof(Lista));
    Nodo* nodo_act=NULL,*nodo_aux=NULL;
    double* pnum=NULL;

    lista->primero=NULL;
    lista->numDFs=0;

    while(strcmp(comando,"quit"))
    {
        for(nTok=0;nTok<5;nTok++)
            arg[nTok]=NULL;
        if(nodo_act==NULL)      // Prompt con lista vacía.
            printf(BLANCO"[?]:>");                      
        else                    // Prompt con dataframe activo.
            printf(BLANCO "[%s: %i,%i]:>",nodo_act->df->nombre,nodo_act->df->numFilas,nodo_act->df->numColumnas);
        gets(comando);                   // Inserto string de comando.
        if(!comando[0])
            continue;
        nTok=0;                          // Inicializo cantidad de trozos en 0.
        arg[nTok]=strtok(comando," ");   // Primer trozo del string.
        while(arg[nTok] != NULL)         // Trozos restantes del string.
        {
            nTok++;
            arg[nTok]=strtok(NULL," ");
        }
        trim(comando);                   // Evito espacios para el comando.
        if(!strcmp(arg[0],"load"))  
        {
            if(nTok>2)
                printf(ROJO S EDA S);    
            else if(nTok<2)
                printf(ROJO S EPA S);   
            else
            {
                nodo_aux = insertar_nodo_final(lista,importar_csv(arg[1])); 
                if(nodo_aux)             // Si el nodo es válido, los inserto como activo.
                    nodo_act=nodo_aux;
            }
        }
        else if(!strcmp(arg[0],"name"))  
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);  
            else if(nTok<2)
                printf(ROJO S EPA S);  
            else if(nTok>2)
                printf(ROJO S EDA S);
            else if(buscar_df_nombres(lista,arg[1])<lista->numDFs) // Si el nombre está tomado, da error.
                printf(ROJO S "ERROR: EL NOMBRE DEL DATAFRAME YA EXISTE" S);
            else
                strcpy(nodo_act->df->nombre,arg[1]); // Copio el nombre.
        }
        else if(!strcmp(arg[0],"list"))
        {
            if(nTok>1)
                printf(ROJO S EDA S);
            else if(!lista->primero) 
                printf(ROJO S "ERROR: LISTA VACIA"S);
            else
                mostrar_lista(lista);
        }
        else if(!strcmp(arg[0],"prefix"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S); 
            else if(nTok<4)
                printf(ROJO S EPA S); 
            else if(nTok>4)
                printf(ROJO S EDA S); 
            else if(!validar_numero_natural(arg[2]))    // Verifica que la cantidad de caracteres sea un número válido.
                printf(ROJO S "ERROR: EL SEGUNDO ARGUMENTO DEBE SER UN NUMERO NATURAL");
            else
                prefix_columna(nodo_act->df,arg[1],atoi(arg[2]),arg[3]);
        }
        else if(!strcmp(arg[0],"meta"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok>1)
                printf(ROJO S EDA S);
            else
                metadatos(nodo_act);
        }
        else if(!strcmp(arg[0],"help"))
        {
            comandos();
        }
        else if(!strcmp(arg[0],"view"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);  
            else if(nTok>2)
                printf(ROJO S EDA S);
            else
            {
                if(!arg[1])
                    mostrar_dataframe(nodo_act->df,10); // Si no hay argumento, muestro 10.
                else if(validar_numero_entero(arg[1]))
                {
                    if(arg[1][0]=='-') // Si el argumento del view es negativo, uso la otra función.
                        mostrar_df_ultimos(nodo_act->df,atoi(arg[1]));
                    else
                        mostrar_dataframe(nodo_act->df,atoi(arg[1]));
                }
            }
        }
        else if(!strcmp(arg[0],"quarter"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<3)
                printf(ROJO S EPA S);
            else if(nTok>3)
                printf(ROJO S EDA S);
            else
                quarter_columna(nodo_act->df,arg[1],arg[2]);
        }
        else if(!strcmp(arg[0],"delcolum"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<2)
                printf(ROJO S EPA S);
            else if(nTok>2)
                printf(ROJO S EDA S);
            else
                eliminar_columna(nodo_act->df,arg[1]);
        }
        else if(!strcmp(arg[0],"delnull"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<2)
                printf(ROJO S EPA S);
            else if(nTok>2)
                printf(ROJO S EDA S);
            else
                eliminar_filas_nulas(nodo_act->df,arg[1]);
        }
        else if(!strcmp(arg[0],"sort"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<2)
                printf(ROJO S EPA S);
            else if(nTok>3)
                printf(ROJO S EDA S);
            else if(arg[2]!=NULL && strcmp(arg[2],"asc") && strcmp(arg[2],"des"))
                printf(ROJO S "ERROR: TERCER ARGUMENTO NO VALIDO (asc/des)" S);
            else
            {
                if(nTok==2 || !strcmp(arg[2],"asc")) // Si no hay argumento de ordenado, es ascendente por predeterminación.
                    ordenar_asc(nodo_act->df,arg[1]);
                if(nTok==3 && !strcmp(arg[2],"des"))
                    ordenar_des(nodo_act->df,arg[1]);
            }
        }
        else if(!strcmp(arg[0],"filter"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<4)
                printf(ROJO S EPA S);
            else if(nTok>4)
                printf(ROJO S EDA S);
            else
                filtros(nodo_act->df,arg[1],arg[2],arg[3]);     
        }           
        else if(!strcmp(arg[0],"save"))
        {
            if(!nodo_act)
                printf(ROJO S EDFA S);
            else if(nTok<2)
                printf(ROJO S EPA S);
            else if(nTok>2)
                printf(ROJO S EDA S);
            else
                exportar_csv(nodo_act->df,arg[1]);
        }
        else
        {   // Recorre buscando los nombres de los dataframes, si lo encuentra lo coloca como activo.
            if(buscar_df_nombres(lista,arg[0])<lista->numDFs)
                nodo_act = recorrer_lista(lista,buscar_df_nombres(lista,arg[0]));
            else if(nTok>1)
            {
                strcat(comando,arg[1]);
                printf(ROJO"EL COMANDO NO ES CORRECTO"DS);
            }
            else if(strcmp(comando,"quit"))
                printf(ROJO"EL COMANDO NO ES CORRECTO"DS);
        }
    }
    liberar_lista(lista);   // Liberación del total de la memoria.
    printf(VERDE S "EXIT PROGRAM" S);
}
