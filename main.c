#include<stdio.h>
#include <ctype.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"
#include "list.h"


//----------------------Estructuras----------------------
typedef struct{
    char nombre[100];
    void* ejercicios;
    long calorias;
    int id;
    int tiempo;
}Rutina;

typedef struct{
    char nombre[50];
    char tipo[20];
    void* dificultades;
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

//Funciones de construccion de estructuras

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

Rutina* createRutina(char* nombre, int id, int tiempo,void* ejercicios, long calorias){
    Rutina* r = (Rutina*)malloc(sizeof(Rutina));
    strcpy(r->nombre,nombre);
    r->id = id;
    r->tiempo = tiempo;
    r->calorias=calorias;
    r->ejercicios = ejercicios;
    return r;
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

//Calcula el IMC del usuario
void CalcularIMC(Stack *HistorialI){

    float p;
    float h;
    printf("Antes de comenzar\nPor favor ingrese\n");
    printf("Su masa corporal: ");
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

//Muestra el historial del IMC del usuario
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

//Muestra el tiempo total invertido en la aplicacion y da posibilidad de mostrar el Historial de IMC
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
void realizaRutina(Stack *stackIMC, Stack *stackH, List *rutinas)
{
   // int cont =1, i,op;
    CalcularIMC(stackIMC);
    /*Rutina *r = first(rutinas);
    while (r)
    {
        printf("%d.- %s",cont,r->nombre);
        cont++;
        r = next(rutinas);
    }*/
}

//Muestra todos los ejercicios disponibles, ordenados por tipo
void mostrarEjercicios(HashMap *mapTipo)
{
    int cont =0;
    printf("-----------------------------------------------------------------------\n");
    printf("                    MOSTRAR TODOS LOS EJERCICIOS       \n");
    printf("-----------------------------------------------------------------------\n");
    List *aux = firstMap(mapTipo);
    while (aux)
    {
        //printf(" NOMBRE     TIPO    NIVEL      REPETICIONES     SERIES    TIEMPO\n");
        Ejercicio *eje = first(aux);
        while (eje)
        {
            printf("Nombre: %s\n",eje->nombre);
            printf("Tipo: %s\n",eje->tipo);
            Dificultad *d = first(eje->dificultades);
            //while (d)
            if(d)
            {
                printf("Nivel: %s\n",d->nivel);
                printf("Repeticiones: %d\n",d->repeticiones);
                printf("Series: %d\n",d->series);
                printf("Tiempo aproximado por serie: %d segundos\n",d->tiempo);
                printf("-----------------------------------------------------------------------\n");
                printf("Descripcion: %s\n",d->descripcion);
                printf("-----------------------------------------------------------------------\n");
                //d = next(eje->dificultades);
            }
            cont++;
            eje = next(aux);
        }
        aux = nextMap(mapTipo);
    }
   // printf("CONTADOR = %d",cont); 
}

//Muestra el Historial de rutinas realizadas por el usuario
void HistorialRutinas(Stack *s)
{
    printf("------------------------------------------------------\n");
    if(get_size(s)==0) printf("            NO HA REALIZADO RUTINAS\n");
    if(get_size(s)!=0)printf("             Sus Rutinas realizadas son\n");
    printf("------------------------------------------------------\n");
    char i[50];
    Stack *s2 = createStack();
    
    while (get_size(s)!=0)
    {
        strcpy(i,topStack(s)); 
        pushStack(s2,topStack(s));
        popStack(s);
        printf("Nombre Rutina : %s\n",i);
    }
    //printf("------------------------------------------------------\n");
    while (get_size(s2)!=0)
    {
        pushStack(s,topStack(s2));
        popStack(s2);
    }
}


void crearRutina(HashMap* Ejercicios,List* Rutinas, HashMap* Tipos){
    char nombre[41],answer[3];
    int cant=0,cont=0,tiempo=0,id,calorias=0;
    bool flag=true;
    printf("-----------------------------------------------------------------------\n");
    printf("                          CREAR TU RUTINA                              \n");
    printf("-----------------------------------------------------------------------\n");
    printf("Ingresa un nombre a tu rutina:");
    getchar();
    scanf("%40[^\n]s", nombre);
    getchar();
    printf("%s\n",nombre);
    printf("A continuacion se desplegaran todos los ejercicios disponibles para agregar a tu rutina\n");
    mostrarEjercicios(Tipos);
    printf("Ingresa la cantidad de ejercicios que deseas agregar a tu rutina ");
    scanf("%d",&cant);
    
    Cola* ejerRutina=createCola();
    do{
        char nombreEjercicio[30],dificultad[10];
        int n=0;
        getchar();
        printf("Ingrese el nombre del ejercicio: ");
        scanf("%29[^\n]s", nombreEjercicio);
        Ejercicio* ejercicio = searchMap(Ejercicios,nombreEjercicio);
        if(ejercicio){
            printf("Ingrese la dificultad:");
            scanf("%s",&dificultad);
            //Convertimos en mayuscula la dificultad ingresada por el usuario para mantener el formato del almacenamiento
            for(int i=0;i<strlen(dificultad);i++)dificultad[i]=toupper(dificultad[i]);
            
            Ejercicio* new=createEjercicio(ejercicio->nombre,ejercicio->tipo);
            Dificultad* d = first(ejercicio->dificultades);
            
            for(int i=0;i<3;i++){
                if(!strcmp(d->nivel,dificultad)){
                    new->dificultades=d;
                    tiempo+=(d->tiempo * d->repeticiones);
                    break;
                }
            }
            pushCola(ejerRutina,new);

            cont++;
            if(cant==cont){
                printf("¿Quieres agregar mas ejercicios? escribe SI o NO ");
                scanf("%s",&answer);
                if(!strcmp(answer,"SI")){
                    printf("Ingrese la cantidad de los ejercicios a agregar \n");
                    scanf("%d\n",&n);
                    cant+=n;
                }
                else flag=false;
            }
        }
        else printf("El nombre no se encuentra o no esta correctamente escrito, intente nuevamente\n");

    }while(flag);
    id=get_size(Rutinas)+1;
    Rutina* rutina = createRutina(nombre,id, tiempo,ejerRutina,calorias);
    pushBack(Rutinas,rutina);
    printf("Su rutina se ha creado con exito\n");
}


void borrarRutina(List *Rutinas){
    int id;
    printf("Ingrese la id de la rutina a eliminar: \n");
    scanf("%d\n",&id);

    Rutina* aux=first(Rutinas);
    while(aux){
        if(aux->id==id){
            popCurrent(Rutinas);
            printf("La rutina se elimino correctamente\n");
            return;
        }
    }
}

void detalleRutina(Rutina *r)
{
    int cont=0;
    //Rutina *r = first(Ruti);
    printf("-----------------------------------------------------------------------\n");
    printf("DETALLES DE LA RUTINA\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Id: %d\n",r->id);
    printf("Nombre: %s\n",r->nombre);
    printf("Tiempo: %d\n",r->tiempo);
    printf("Calorias: %ld\n",r->calorias);
    Ejercicio *eje = first(r->ejercicios);
    printf("-----------------------------------------------------------------------\n");
    printf("EJERCICIOS\n");
    printf("-----------------------------------------------------------------------\n");
    while (eje)
    {
        printf("Nombre: %s\n",eje->nombre);
        printf("Tipo: %s\n",eje->tipo);
        printf("-----------------------------------------------------------------------\n");
        cont++;
        eje = next(r->ejercicios);
    }
}

void mostrarRutinas(List* Rutinas){

    int caso=1;
    while(caso!=0){
        printf("1. Modificar rutina\n");
        printf("2. Eliminar rutina\n");
        printf(" 0. Volver al menu principal\n");
        scanf("%d", &caso);

        switch(caso){
            //case 1:modificarRutina(Rutinas);break;
            case 2:borrarRutina(Rutinas);break;
            
        }
    }
}

//void modificarRutinas(List* Rutinas){

//}

int main(){
    HashMap* Ejercicios=createMap(100);
    HashMap* Tipos=createMap(10);
    int masaCorporal = 0;
    Stack *stackIMC = createStack();
    Stack *stackH = createStack();
    List* Rutinas=createList();
    
    int caso=2;
    Cargar(Ejercicios,Tipos);
    printf("-----------------------------------------------------------------------\n");
    printf("                            TRAININGSAPP                             \n");
    printf("-----------------------------------------------------------------------\n");
    printf("Hola, te damos la bienvenida a TrainingsApp una aplicacion que te ayudara a mantener tu entrenamiento al dia\n");
    printf("Por favor ingresa tu peso en kg sin decimales:\n");
    scanf("%d",&masaCorporal);
    //printf("%d\n",masaCorporal);
    //CalcularIMC(stackIMC);
    printf("-----------------------------------------------------------------------\n");
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
            case 1:crearRutina(Ejercicios,Rutinas,Tipos);break;
            case 2:realizaRutina(stackIMC, stackH, Rutinas);break;
            case 3:mostrarRutinas(Rutinas);break;
            case 4:HistorialRutinas(stackH);
            case 5:break;
            case 6:break;
            case 7:break;
            case 8:desempenoEntrenamiento(stackIMC);break;
        }
    }
    return 0;
}