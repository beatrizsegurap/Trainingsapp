#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"
#include "list.h"


//----------------------Estructuras----------------------
typedef struct{
    char nombre;
    void* ejercicios;
    long calorias;
    long id;
    int tiempo;
}Rutina;

typedef struct{
    char nombre[50];
    char tipo[20];
    List* dificultades;
}Ejercicio;

typedef struct{
    char nivel[12];
    char descripcion[2000];
    int tiempo;
    int series;
    int repeticiones;
}Dificultad;

typedef struct{
    char nombre[50];
    long carbohidratos;
    long grasas;
    long proteinas;
    long calorias;
}Comida;

//-------------------------------------------------------

Ejercicio* createEjercicio(char *nombre, char* tipo){
    Ejercicio* e = (Ejercicio*) malloc(sizeof(Ejercicio));
    strcpy(e->nombre,nombre);
    strcpy(e->tipo,tipo);
    return e;
}

Dificultad* createDificultad(char * nivel,char* descripcion,int tiempo, int series,int repeticiones){
    Dificultad *d = (Dificultad*)malloc(sizeof(Dificultad));
    strcpy(d->nivel,nivel);
    strcpy(d->descripcion,descripcion);
    d->tiempo=tiempo;
    d->series=series;
    d->repeticiones=repeticiones;
    return d;
}

//Funcion para leer el k-esimo elemento de un string (separado por comas) nos ayuda para cargar el archivo
char*get_csv_field (char * tmp, int k){
    int open_mark = 0;
    char* ret=(char*) malloc(2000*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }
        if(open_mark || tmp[i]!= ';'){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }
        if(tmp[i]== ';'){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }
        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}

//Conversor de char a int
int conversorInt(char * cadena){
    int i=cadena[0]-'0';
    if(cadena[1]-'0'>=0){
        i=i*10;
        i+=cadena[1]-'0';
    }
    return i;
}

//Funcion que carga ejercicios a partir de archivo .csv
void Cargar(HashMap * mapEjercicios, HashMap * mapTipos){
    //Abrimos el archivo con los ejercicios separados por ;
    FILE *file;
    file = fopen ("Ejercicios.csv", "r" ); 
    if(!file)return;

    // Cadena para guardar la linea completa del archivo csv
    char linea[3000];
    //Obtenemos la primera linea del archivo sin datos y copiamos en el nuevo 
    fgets (linea, 1023, file);

    while (fgets (linea, 2999, file) != NULL){
        char *nombre,*tipo,*descripcion,*dificultad;
        int series,repeticiones,tiempo;
        for(int i=0;i<7;i++){
                char *aux = get_csv_field(linea, i); 
                if(i==0)nombre=aux;
                if(i==1)tipo=aux;
                if(i==2)dificultad=aux;
                if(i==3)tiempo=conversorInt(aux);
                if(i==4)repeticiones=conversorInt(aux);
                if(i==5)series=conversorInt(aux);
                if(i==6)descripcion=aux;
        } 
        Ejercicio *Ejercicio = searchMap(mapEjercicios,nombre);
        List *Tipo = searchMap(mapTipos,tipo);
        if(!Ejercicio){
            Ejercicio = createEjercicio(nombre,tipo);
            List * dificultades = createList();
            Dificultad* d = createDificultad(dificultad,descripcion,tiempo,series,repeticiones);
            pushBack(dificultades,d);
            Ejercicio->dificultades=dificultades;
            insertMap(mapEjercicios,nombre,Ejercicio);
        }
        else{
            Dificultad* d = createDificultad(dificultad,descripcion,tiempo,series,repeticiones);
            pushBack(Ejercicio->dificultades,d);
        }

        if(!Tipo){
            Tipo = createList();
            pushBack(Tipo,Ejercicio);
            insertMap(mapTipos,tipo,Tipo);
        }
        else{
            pushBack(Tipo,Ejercicio);
        }
    }
    fclose(file);
}

void CalcularIMC(Stack *HistorialI){

    float p;
    float h;
    printf("Antes de comenzar\nPor favor ingrese\n");
    printf("Su Peso: ");
    scanf("%f",&p);
    printf("Su Altura: ");
    scanf("%f",&h);
    float * IMC = malloc (sizeof(float)); 
    *IMC = p/(h*h);
    printf("------------------------------------------------------\n");
    printf("         Su IMC actual es: %f\n",*IMC);
    printf("------------------------------------------------------\n");
    pushStack(HistorialI,IMC);
}
void HistorialIMC(Stack *HistorialI)
{
    printf("------------------------------------------------------\n");
    if(get_size(HistorialI)==0) printf("            NO HAY DATOS INGRESADOS\n");
    if(get_size(HistorialI)!=0)printf("             Sus IMC's ingresados son\n");
    printf("------------------------------------------------------\n");
    float i;
    Stack *s2 = createStack();
    
    while (get_size(HistorialI)!=0)
    {
        i= *((float*)topStack(HistorialI)); 
        pushStack(s2,topStack(HistorialI));
        popStack(HistorialI);
        printf("IMC : %f\n",i);
    }
    //printf("------------------------------------------------------\n");
    while (get_size(s2)!=0)
    {
        pushStack(HistorialI,topStack(s2));
        popStack(s2);
    }
}
void desempenoEntrenamiento (Stack *s)
{
    int caso=3;
    while (caso!=0)
    {
        printf("           ESCOJA UNA OPCION         \n");
        printf("------------------------------------------------------\n");
        printf("1. Mostrar Historial IMC\n");
        printf("0. Salir\n");
        scanf("%d",&caso);
        switch (caso)
        {
            case 1: HistorialIMC(s);break;
        }
        printf("------------------------------------------------------\n");
    }
}
void realizaRutina(Stack *stackIMC)
{
    CalcularIMC(stackIMC);
}


int main(){
    HashMap* Ejercicios=createMap(100);
    HashMap* Tipos=createMap(10);
    int masaCorporal = 0;
    Stack *stackIMC = createStack();

    int caso=2;
    Cargar(Ejercicios,Tipos);
    printf("-----------------------------------------------------------------------\n");
    printf("                            TRAININGSAPP                             \n");
    printf("-----------------------------------------------------------------------\n");
    printf("¡Hola! te damos la bienvenida a TrainingsApp una aplicacion que te ayudara a mantener tu entrenamiento al dia\n");
    printf("Por favor ingresa tu peso en kg sin decimales:\n");
    scanf("%d",&masaCorporal);
    printf("%d\n",masaCorporal);
    //CalcularIMC(stackIMC);
    while(caso!=0)
    {
        printf(" 1. Crear rutina\n");
        printf(" 2. Realizar rutina\n");
        printf(" 3. Rutinas\n");
        printf(" 4. Historial de rutinas\n");
        printf(" 5. Calorias quemadas\n");
        printf(" 6. Balance de calorias\n");
        printf(" 7. Comidas \n");
        printf(" 8. Desempeno de entrenamiento diario\n");
        printf(" 0. Salir\n");
        scanf("%d", &caso);

        switch(caso)
        {
            case 1:break;
            case 2:realizaRutina(stackIMC);break;
            case 3:break;
            case 4:break;
            case 5:break;
            case 6:break;
            case 7:break;
            case 8:desempenoEntrenamiento(stackIMC);break;
        }
    }
    return 0;
}