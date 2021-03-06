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
    Cola* ejercicios;
    int id;
    int tiempo;
}Rutina;

typedef struct{
    char nombre[50];
    char tipo[20];
    List* dificultades;
}Ejercicio;

typedef struct{
    int nivel;
    char descripcion[1000];
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

Dificultad* createDificultad(int nivel,char* descripcion,int tiempo, int series,int repeticiones){
    Dificultad *d = (Dificultad*)malloc(sizeof(Dificultad));
    d->tiempo=tiempo;
    d->series=series;
    d->repeticiones=repeticiones;
    d->nivel=nivel;
    strcpy(d->descripcion,descripcion);
    return d;
}

Rutina* createRutina(char* nombre, int id, int tiempo,void* ejercicios){
    Rutina* r = (Rutina*)malloc(sizeof(Rutina));
    strcpy(r->nombre,nombre);
    r->id = id;
    r->tiempo = tiempo;
    r->ejercicios = ejercicios;
    return r;
}

Comida* createComida(char* nombre, long kc, long fats, long carbs, long prots){
    Comida* c = (Comida*) malloc(sizeof(Comida));
    strcpy(c->nombre,nombre);
    c->calorias=kc;
    c->grasas=fats;
    c->carbohidratos=carbs;
    c->proteinas=prots;
    return c;
}

//Funcion para leer el k-esimo elemento de un string (separado por comas) nos ayuda para cargar el archivo
char*get_csv_field (char * tmp, int k){
    int open_mark = 0;
    char* ret=(char*) malloc(1000*sizeof(char));
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
        char *nombre,*tipo,*descripcion;
        int series,repeticiones,tiempo,dificultad;
        for(int i=0;i<7;i++){
                char *aux = get_csv_field(linea, i); 
                if(i==0)nombre=aux;
                if(i==1)tipo=aux;
                if(i==2){
                    if(!strcmp(aux,"PRINCIPIANTE"))dificultad=1;
                    if(!strcmp(aux,"MEDIO"))dificultad=2;
                    if(!strcmp(aux,"DIFICIL"))dificultad=3;
                }
                if(i==3)tiempo=conversorInt(aux);
                if(i==4)repeticiones=conversorInt(aux);
                if(i==5)series=conversorInt(aux);
                if(i==6)descripcion=aux;
        } 

        //Guardamos los datos de los ejercicios en el mapa de ejercicios por nombre
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
            Dificultad* auxiliar = first(Ejercicio->dificultades);
            while(auxiliar){
                auxiliar=next(Ejercicio->dificultades);
            }
        }

        //Creamos el mapa por tipo
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
//Funcion que carga la comida de un archivo, las guarda en un mapa para su uso en funciones y en una lista para permitir que se actualize dicho archivo cuando se finalize el uso de la app.
void cargarComida(HashMap* mapComida,List* Alimentos){
    FILE *archivo;
    archivo = fopen("Alimentos.csv", "r" );
    if(!archivo)return;
    char *name, *aux;
    int kc, fats, carbs, prots, i;
    char linea[100];
    //Recibe la primera linea con informacion no pertinente.
    fgets(linea,99,archivo);
    //Pasa por cada linea y por cada seccion separada por ; para leer cada variable del struct comida.
    while(fgets(linea, 99, archivo) != NULL){
        for(i=0;i<5;i++){
            aux = get_csv_field(linea,i);
            if(i==0)name=aux;
            if(i==1)kc=atol(aux);
            if(i==2)fats=atol(aux);
            if(i==3)carbs=atol(aux);
            if(i==4)prots=atol(aux);
        }
        //se crea el struct comida con las variables previamente leidas y se guarda en el mapa para su busqueda por el usuario y en una lista, la cual permite reescribir el archivo con
        //nueva informacion a??adida o editada por el usuario.
        Comida* Alimento;
        Alimento = createComida(name,kc,fats,carbs,prots);
        insertMap(mapComida,name,Alimento);
        pushBack(Alimentos,Alimento);
    }
    //Se cierra el archivo
    fclose(archivo);
}

//Calcula el IMC del usuario, pidiendo su altura y su masa corporal
float CalcularIMC(Stack *HistorialI){

    float p;
    float h;
    printf("------------------------------------------------------\n");
    printf("Antes de comenzar, por favor ingrese\n");
    printf("------------------------------------------------------\n");
    printf("Su masa corporal en kg: ");
    scanf("%f",&p);
    printf("------------------------------------------------------\n");
    printf("Su Altura en metro: ");
    scanf("%f",&h);
    float * IMC = malloc (sizeof(float)); 
    *IMC = p/(h*h);
    printf("------------------------------------------------------\n");
    printf("         Su IMC actual es: %f\n",*IMC);
    //Se insertan los IMC's calculados en la pila
    pushStack(HistorialI,IMC);
    return p;
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
    //Se imprimen los IMC,s guardados y ademas se apilan en una pila con el fin de no perder los datos
    while (get_size(HistorialI)!=0)
    {
        i= *((float*)topStack(HistorialI)); 
        pushStack(s2,topStack(HistorialI));
        popStack(HistorialI);
        printf("IMC : %f\n",i);
    }
    //Se vuelven a copiar los valores de los IMC,s en la pila original
    while (get_size(s2)!=0)
    {
        pushStack(HistorialI,topStack(s2));
        popStack(s2);
    }
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
        //Accede a los ejercicios separados por tipo en el mapa
        Ejercicio *eje = first(aux);
        printf("------------------------------------------------------------------------------------------\n");
        printf("                               EJERCICIOS PARA %s\n",eje->tipo);
        printf("------------------------------------------------------------------------------------------\n");
        while (eje)
        {
            printf("Nombre: %s\n",eje->nombre);
            Dificultad *d = first(eje->dificultades);
            
            if(d)
            {
                printf("Nivel: ");
                if(d->nivel==1)printf("PRINCIPIANTE\n");
                if(d->nivel==2)printf("MEDIO\n");
                if(d->nivel==3)printf("DIFICIL\n");
                printf("Repeticiones: %d\n",d->repeticiones);
                printf("Series: %d\n",d->series);
                printf("Tiempo aproximado por serie: %d segundos\n",d->tiempo);
                printf("-----------------------------------------------------------------------\n");
                printf("Descripcion: %s\n",d->descripcion);
                printf("-----------------------------------------------------------------------\n");

            }
            cont++;
            eje = next(aux);
        }
        aux = nextMap(mapTipo);
    }

}

//Muestra el Historial de rutinas realizadas por el usuario
void HistorialRutinas(Stack *s)
{
    int tiempo;
    printf("------------------------------------------------------\n");
    if(get_size(s)==0) printf("            NO HA REALIZADO RUTINAS\n");
    if(get_size(s)!=0)printf("             Sus Rutinas realizadas son\n");
    printf("------------------------------------------------------\n");
    char i[50];
    Stack *s2 = createStack();
    //Recorremos la pila con el historial de rutinas utilizando una pila y ademas se apilan los datos en una pila auxiliar
    while (get_size(s)!=0)
    {
        Rutina *r = topStack(s);
        tiempo = r->tiempo;
        if(tiempo<60) printf("La Rutina %s tiene un tiempo de %d segundos aproximadamente\n",r->nombre,r->tiempo);
        if(tiempo>59) printf("La Rutina %s tiene un tiempo de %d minutos aproximadamente\n",r->nombre,(r->tiempo)/60);
        pushStack(s2,topStack(s));
        popStack(s);
        printf("------------------------------------------------------\n");
    }
    //Se apilan los datos copiados en la pila auxiliar en la pila original
    while (get_size(s2)!=0)
    {
        pushStack(s,topStack(s2));
        popStack(s2);
    }
}

//Funcion que crea una rutina a partir de los ejercicios almacenados
void crearRutina(HashMap* Ejercicios,List* Rutinas, HashMap* Tipos){
    char nombre[100];
    int cant=0,cont=0,tiempo=0,id,calorias=0;
    bool flag=true;
    //Se solicita al usuario inglresar el nombre que le dara a la rutina y la cantidad de ejercicios que esta tendra
    printf("-----------------------------------------------------------------------\n");
    printf("                          CREAR TU RUTINA                              \n");
    printf("-----------------------------------------------------------------------\n");
    printf("Ingresa un nombre a tu rutina:");
    getchar();
    scanf("%99[^\n]s", nombre);
    getchar();
    printf("%s\n",nombre);
    printf("A continuacion se desplegaran todos los ejercicios disponibles para agregar a tu rutina\n");
    mostrarEjercicios(Tipos);
    printf("Ingresa la cantidad de ejercicios que deseas agregar a tu rutina ");
    scanf("%d",&cant);
    
    //Para crear la rutina la almacenamos en una cola asi al ir realizando la rutina podemos acceder en el orden que el usuario estimo conveniente en la creacion
    Cola* ejerRutina=createCola();
    do{
        //Se muestran las opciones de ejercicios que tiene el usuario para elegir, este selecciona el numero del ejercicios ya que si el ingreso fuese por nombre podrian
        //existir muchos errores de tipeo por el usuario 
        char nombreEjercicio[30];
        int n=0,dificultad=0;
        getchar();
        printf("--------------------------------------------------------------------\n");
        printf("Ingrese el numero del nombre del ejercicio que desea seleccionar: \n");
        printf("--------------------------------------------------------------------\n");
        while(n==0){
            printf("1.Flexiones de brazos\n");
            printf("2.Abdominales\n");
            printf("3.Elevacion invertida\n");
            printf("4.Plancha lateral\n");
            printf("5.Bicicleta\n");
            printf("6.Plancha horizontal\n");
            printf("7.Zancadas\n");
            printf("8.Crunch\n");
            printf("9.Sentadillas\n");
            printf("10.Superman\n");
            printf("11.Comandos\n");
            printf("12.Elevacion de pelvis\n");
            printf("13.Elevacion de piernas\n");
            scanf("%d",&n);

            switch(n)
            {
                case 1:n=1;break;
                case 2:n=2;break;
                case 3:n=3;break;
                case 4:n=4;break;
                case 5:n=5;break;
                case 6:n=6;break;
                case 7:n=7;break;
                case 8:n=8;break;
                case 9:n=9;break;
                case 10:n=10;break;
                case 11:n=11;break;
                case 12:n=12;break;
                case 13:n=13;break;

            }
        }

        if(n==1)strcpy(nombreEjercicio,"Flexiones de brazos");
        if(n==2)strcpy(nombreEjercicio,"Abdominales");
        if(n==3)strcpy(nombreEjercicio,"Elevacion invertida");
        if(n==4)strcpy(nombreEjercicio,"Plancha lateral");
        if(n==5)strcpy(nombreEjercicio,"Bicicleta");
        if(n==6)strcpy(nombreEjercicio,"Plancha horizontal");
        if(n==7)strcpy(nombreEjercicio,"Zancadas");
        if(n==8)strcpy(nombreEjercicio,"Crunch");
        if(n==9)strcpy(nombreEjercicio,"Sentadillas");
        if(n==10)strcpy(nombreEjercicio,"Superman");
        if(n==11)strcpy(nombreEjercicio,"Comandos");
        if(n==12)strcpy(nombreEjercicio,"Elevacion de pelvis");
        if(n==13)strcpy(nombreEjercicio,"Elevacion de piernas");

        printf("- Usted selecciono el ejercicio %s\n",nombreEjercicio);
        printf("------------------------------------------------------\n");
        printf("Ingrese el numero de la dificultad que desee seleccionar\n");
        Ejercicio* ejercicio = searchMap(Ejercicios,nombreEjercicio);
        //El usuario selecciona la dificultad que quiere por ejercicio
        while(dificultad==0){
            printf("1.PRINCIPIANTE\n");
            printf("2.MEDIO\n");
            printf("3.DIFICIL\n");
            scanf("%d",&dificultad);

            switch (dificultad)
            {
                case 1:dificultad=1;break;
                case 2:dificultad=2;break;
                case 3:dificultad=3;break;
            }
        }
        printf("------------------------------------------------------\n");
        printf("\n");
        Ejercicio* new=createEjercicio(ejercicio->nombre,ejercicio->tipo);
        Dificultad* d = first(ejercicio->dificultades);
            
        while(d){
            if(d->nivel==dificultad){
                List* listEjer=createList();
                pushBack(listEjer,d);
                new->dificultades=listEjer;
                tiempo+=(d->tiempo * d->series);
                break;
            }
            d=next(ejercicio->dificultades);
        }
        //Finalmente se almacenan los ejercicios en una cola para que el usuario realice la rutina en el orden que selecciono los ejercicios
        pushCola(ejerRutina,new);

        cont++;
            
        if(cant==cont)flag=false;

    }while(flag);

    id=get_size(Rutinas)+1;
    Rutina* rutina = createRutina(nombre,id, tiempo,ejerRutina);
    pushBack(Rutinas,rutina);
    printf("-----------------------------------------------------------------------\n");
    printf("*******************Su rutina se ha creado con exito********************\n");
    printf("-----------------------------------------------------------------------\n");
    printf("\n");
}

//Funcion que elimina una rutina solo del almacenamiento rutinas
void borrarRutina(List *Rutinas){
    printf("--------------------------------------------------------------------\n");
    int id;
    printf("Ingrese la id de la rutina a eliminar: \n");
    scanf("%d",&id);

    Rutina* aux=first(Rutinas);
    while(aux){
        if(aux->id==id){
            popCurrent(Rutinas);
            printf("La rutina se elimino correctamente\n");
            printf("--------------------------------------------------------------------\n");
            return;
        }
        aux=next(Rutinas);
    }
    printf("La rutina no se encontro\n");
    printf("--------------------------------------------------------------------\n");
    return;
}

//Funcion para limpiar la cadena de caracteres al volver a declarar su contenido
char* limpiarChar(char *palabra){
    for(int i=0;i<strlen(palabra);i++){
        palabra[i]=0;
    }
    return palabra;
}

//Funcion que modifica el nombre de una rutina parte del menu que muestra todas las rutinas
void modificarRutina(List* Rutinas){
    printf("--------------------------------------------------------------------\n");
    int id;
    printf("Ingrese la id de la rutina a modificar: ");
    scanf("%d",&id);
    printf("--------------------------------------------------------------------\n");
    Rutina* aux=first(Rutinas);
    while(aux){
        if(aux->id==id){
            int caso=1;
            //Encontramos la rutina para su modificacion y se da la opcion de modificar su nombre o volver al menu anterior
            printf("Usted selecciono la rutina << %s >>\n",aux->nombre);
            while(caso!=0){
                printf("--------------------------------------------------------------------\n");
                printf("\n");
                printf("1. Modificar nombre\n");
                printf("0. Volver al menu de rutinas\n");
                printf("--------------------------------------------------------------------\n");
                scanf("%d", &caso);

                switch(caso){
                    case 1: printf("--------------------------------------------------------------------\n");
                            printf("\nEscriba el nuevo nombre: ");
                            char nombre[100];
                            getchar();
                            scanf("%99[^\n]s", nombre);
                            limpiarChar(aux->nombre);
                            strcpy(aux->nombre,nombre);
                            printf("el nuevo nombre es << %s >>\n",aux->nombre);
                            printf("--------------------------------------------------------------------\n");
                            break;
                }
            }
            return;
        }
    }
}

//Esta funcion muestra la informacion de cada rutina con sus ejercicios y el detalle de cada uno
void detalleRutina(Rutina *r)
{
    int cont=0;
    //Rutina *r = first(Ruti);
    printf("-----------------------------------------------------------------------\n");
    printf("DETALLES DE LA RUTINA\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Id: %d\n",r->id);
    printf("Nombre: %s\n",r->nombre);
    printf("Tiempo: %d min\n",(r->tiempo/60));
    Ejercicio *eje = front(r->ejercicios);
    Ejercicio *aux = front(r->ejercicios);
    //No se utiliza una estructura auxiliar sino que la misma cola para realizar las operaciones de mostrar ejercicios esto se debe a las 
    //caracteristicas que provee este tda
    printf("-----------------------------------------------------------------------\n");
    printf("EJERCICIOS\n");
    printf("-----------------------------------------------------------------------\n");
    do
    {
        printf("Nombre: %s\n",eje->nombre);
        printf("Tipo: %s\n",eje->tipo);
        printf("Nivel: ");
        Dificultad* d=first(eje->dificultades);
        if(d->nivel==1)printf("PRINCIPIANTE\n");
        if(d->nivel==2)printf("MEDIO\n");
        if(d->nivel==3)printf("DIFICIL\n");
        printf("Tiempo estimado por serie: %d segundos\n",d->tiempo);
        printf("Cantidad de repeticiones durante la serie: %d\n",d->repeticiones);
        printf("Cantidad de series a realizar: %d\n",d->series);
        printf("-----------------------------------------------------------------------\n");
        cont++;
        pushCola(r->ejercicios,eje);
        popCola(r->ejercicios);
        eje = front(r->ejercicios);
    }while(strcmp(eje->nombre,aux->nombre));
}


//Funcion que calcula las calorias quemadas a partir del tiempo en el que se realizo actividad fisica y la masa corporal del usuario
int calcularCalorias(int tiempo, int masa){
    int IMC = round((tiempo/60)*masa*0.0175);
    return IMC;
}

//Esta funcion permite que el usuario realice una rutina creada por el anteriormete asi se calculara su avance y permitira al usuario realizar varias veces la misma rutina
void realizarRutina(Stack *stackIMC, Stack *stackH, List *rutinas)
{
    int cont =0, i,op=2;
    if(get_size(rutinas)!=0)
    {
        //En efecto de la realizacion de la rutina se calcula el IMC del usuario para posteriormente determinar su avance
        float mc = CalcularIMC(stackIMC);
        Rutina *r = first(rutinas);

        while (op != 0)
        {
           printf("-----------------------------------------------------------------------\n");
           printf("SUS RUTINAS\n");
           printf("-----------------------------------------------------------------------\n");
           while (r)
           {
               cont++;
               printf("%d.- %s\n",cont,r->nombre);
               r = next(rutinas);
           }
           printf("-----------------------------------------------------------------------\n");
           printf("INGRESE EL NUMERO DE LA RUTINA QUE DESEA REALIZAR\n");
           printf("-----------------------------------------------------------------------\n");
           op = cont*3;
           while (op>=cont+1)
           {
               scanf("%d",&op);
               if(op>=cont+1)printf("INGRESE NUEVAMENTE EL NUMERO\n");
           }
           r = first(rutinas);
           detalleRutina(r);
           for(i=1 ; i<=cont+1 ; i++)
           {
               if(op == i)
               {
                   detalleRutina(r);
                   pushStack(stackH,r);
                   break;
               }
               r = next(rutinas);
           }
           printf("Cuando hayas terminado presione '0'\n");
           scanf("%d",&op);
           cont = 0;
           printf("En total quemo %d calorias durante la rutina\n",calcularCalorias(r->tiempo,mc));
           printf("----------------------------------------------------------------------------\n");
       }
    }
    else 
    {
        printf("-----------------------------------------------------------------------\n");
        printf("AUN NO HA CREADO RUTINAS\n");
        printf("-----------------------------------------------------------------------\n");
        return;
    }
    
}

//Funcion que mostrara todas las rutinas que ha creado el usuario y permitira modificar su nombre o eliminarlas de este almacenamiento
void mostrarRutinas(Stack *stackIMC, Stack *stackH, List* Rutinas){
    Rutina* aux = first(Rutinas);
    while (aux){
        detalleRutina(aux);
        aux=next(Rutinas);
    }
    int caso=1;
    printf("--------------------------------------------------------------------\n");
    while(caso!=0){
        printf("1. Modificar rutina\n");
        printf("2. Eliminar rutina\n");
        printf("3. Realizar rutina\n");
        printf("0. Volver al menu principal\n");
        printf("--------------------------------------------------------------------\n");
        scanf("%d", &caso);

        switch(caso){
            case 1:modificarRutina(Rutinas);break;
            case 2:borrarRutina(Rutinas);break;
            case 3:realizarRutina(stackIMC, stackH, Rutinas);break;
            
        }
    }
    printf("--------------------------------------------------------------------\n");
}

//Muestra el tiempo total invertido en la aplicacion y da posibilidad de mostrar el Historial de IMC
void desempenoEntrenamiento (Stack *s, Stack *s2)
{
    int caso=3;
    while (caso!=0)
    {
        printf("           ESCOJA UNA OPCION         \n");
        printf("------------------------------------------------------\n");
        printf("1. Mostrar Historial IMC\n");
        printf("2. Mostrar Historial de Rutinas\n");
        printf("0. Salir\n");
        scanf("%d",&caso);
        switch (caso)
        {
            case 1: HistorialIMC(s);break;
            case 2: HistorialRutinas(s2);break;
        }
        printf("------------------------------------------------------\n");
    }
}

//Esta funcion ocupa los stacks para mostrar en orden los alimentos a??adidos a la dieta del dia
//Ademas de mostrar un resumen de las calorias totales y un detalle de grasas, carbs y proteinas.
void mostrarDieta(Stack* s){
    //Segundo stack usado para invertir el original y poder imprimirlo en orden.
    Stack* s2=createStack();
    Comida* a;
    long i=0;
    if(get_size(s)==0){
        printf("No ha agregado nada a su dieta de hoy.\n");
        return;
    }
    //Se imprime al voltear denuevo el stack para que se imprima en el orden en el cual fueron a??adidos.
    while(get_size(s)!=0){
        pushStack(s2,topStack(s));
        popStack(s);  
    }
    printf("Su dieta de hoy a sido: \n");
    //Pasa por toda el stack invertido mostrando los alimentos ingeridos.
    while(get_size(s2)!=0){
        a=topStack(s2);
        printf("%s: ",a->nombre);
        printf("%ldkc G=%ldmg C=%ldmg P=%ldmg\n",a->calorias,a->grasas,a->carbohidratos,a->proteinas);
        i+=a->calorias;
        pushStack(s,topStack(s2));
        popStack(s2);
    }
    //Se da el total de calorias en forma de resumen.
    printf("Para un total de %ld calorias.\n",i);
}
//Es la abreviacion de crear comida custom, pregunta al usuario la informacion de un alimento a crear y lo a??ade,
//tanto a la dieta del dia, la lista de alimentos y el mapa de alimentos.
void crearcc(char nombre[50], HashMap* map, Stack* s,List* lista){
    Comida* Alimento;
    int kc, fats, carbs, prot;
    printf("             Favor introducir las calorias del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%d",&kc);
    printf("      Favor introduzca los miligramos de grasas del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%d",&fats);
    printf("    Favor intrduzca los miligramos de carbohidratos del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%d",&carbs);
    printf("     Favor introduzca los miligramos de proteinas del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%d",&prot);
    Alimento = createComida(nombre,kc,fats,carbs,prot);
    //Se inserta el alimento recien creado en el mapa,la lista y el stack.
    insertMap(map,nombre,Alimento);
    pushStack(s,Alimento);
    pushBack(lista,Alimento);
}
//Una variante de crear comida custom, el cual es ocupado en otra funcion, de igual manera busca si el nombre que se le quiere dar a la comida existe o no.
//En caso de existir permite editar el alimento en cuestion.Mientras que si no existe, lo crea.
void crearcc2(HashMap* map,List* lista){
    //Se previene un error de lectura que se encontro en otras funciones que seguian despues del switch con la lectura.
    getchar();
    Comida* Alimento;
    Comida* Alimento2;
    char nombre[50];
    long kc, fats, carbs, prot;
    int i=0;
    printf("Favor ingrese el nombre del alimento que quiere crear\n");
    scanf("%[^\n]",nombre);
    //Se revisa si el nombre del alimento que se quiere crear existe, en caso de existir se da la opcion de remplazar sus valores.
    Alimento2=searchMap(map,nombre);
    if(Alimento2){
        printf("El alimento ya existe, desea remplazarlo?\n 1. Si\n 2. No\n");
        scanf("%d",&i);
        if(i==1){
            printf("            Favor introducir las calorias del alimento\n");
            printf("-----------------------------------------------------------------------\n");
            scanf("%ld",&Alimento2->calorias);
            printf("      Favor introduzca los miligramos de grasas del alimento\n");
            printf("-----------------------------------------------------------------------\n");
            scanf("%ld",&Alimento2->grasas);
            printf("   Favor intrduzca los miligramos de carbohidratos del alimento\n");
            printf("-----------------------------------------------------------------------\n");
            scanf("%ld",&Alimento2->carbohidratos);
            printf("     Favor introduzca los miligramos de proteinas del alimento\n");
            printf("-----------------------------------------------------------------------\n");
            scanf("%ld",&Alimento2->proteinas);
            return;
        }else return;
    }
    //Pide las variables para el alimento.
    printf("           Favor introducir las calorias del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%ld",&kc);
    printf("      Favor introduzca los miligramos de grasas del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%ld",&fats);
    printf("    Favor intrduzca los miligramos de carbohidratos del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%ld",&carbs);
    printf("     Favor introduzca los miligramos de proteinas del alimento\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%ld",&prot);
    //Se crea el alimento, al igual que se inserta tanto en el mapa como en la lista, para que al cerrarse el programa se guarde en el archivo y no se pierda.
    Alimento = createComida(nombre,kc,fats,carbs,prot);
    insertMap(map,nombre,Alimento);
    pushBack(lista,Alimento);
}

//Esta funcion permite al usuario a??adir comida a su dieta diaria, en caso de que se ingrese un nombre no encontrado, se le pregunta si desea crearlo.
//En caso de existir se a??ade al stack dieta y vuelve al menu comida.
void anadirComida(Stack* s, HashMap* map,List* lista){
    Comida* Alimento;
    int x;
    //Existia un problema con la lectura despues del switch de menuComida, la cual se arreglo con el getchar.
    getchar();
    char nombre[50];
    printf("-----------------------------------------------------------------------\n");
    printf("Favor ingrese el nombre del alimento que desea anadir a su dieta de hoy\n");
    printf("-----------------------------------------------------------------------\n");
    scanf("%[^\n]",nombre);
    //Se revisa si el nombre existe, en caso de no existir se le da la opcion al usuario de crearlo.
    Alimento = searchMap(map, nombre);
    if(!Alimento){
        printf("El alimento que busco no existe.\n ??Desea crearlo?\n");
        printf(" 1. Continuar\n");
        printf(" 0. Salir\n");
        scanf("%d",&x);
        switch(x){
            case 1:crearcc(nombre, map, s,lista);break;
            case 0:return;
        }
    }else{
        //En caso de existir simplemente se agrega al Stack.
        pushStack(s, Alimento);
    }
}
//Es el menu el cual lleva a todas las opciones que tengan relacion con alimentos y/o comida.
void menuComida(HashMap* mapComida, Stack* Dieta,List* lista){
    int caso=2;;
    printf("-----------------------------------------------------------------------\n");
    printf("                            Menu de Comida\n");
    printf("-----------------------------------------------------------------------\n");
    while(caso!=0){
        printf("          Favor ingrese la accion que quiere ejercer\n");
        printf("-----------------------------------------------------------------------\n");
        printf(" 1. Anadir Comida\n");
        printf(" 2. Ver dieta de hoy\n");
        printf(" 3. Crear alimento\n");
        printf(" 0. Salir\n");
        scanf("%d",&caso);
        switch(caso){
            case 1:anadirComida(Dieta, mapComida,lista);break;
            case 2:mostrarDieta(Dieta);break;
            case 3:crearcc2(mapComida,lista);break;
        }
    }
}

//Esta funcion mostrara las calorias quemadas por el usuario tras realizar rutinas, asi este sabra cuanto ha trabajado fisicamente y permitira visualizar su avance
void caloriasQuemadas(Stack* Historial,int masaC){
    printf("-----------------------------------------------------------------------\n");
    printf("***************************CALORIAS QUEMADAS***************************\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Estas son las calorias quemadas durante todo tu uso de la app\n");
    long calorias=0;
    int tiem=0;
    Rutina* r = topStack(Historial);
    Stack* aux=createStack();
    if(!r){
        printf("Aun no has quemado calorias porque no has realizado ninguna rutina\n");
        printf("-----------------------------------------------------------------------\n");
        return;
    }
    while(r){
        tiem+=r->tiempo;
        pushStack(aux,r);
        popStack(Historial);
        r=topStack(Historial);
    }
    int IMC = calcularCalorias(tiem,masaC);
    printf("Has quemado %d calorias durante todas tus rutinas realizadas\n",IMC);

    //Llenamos la pila inicial con las rutinas almacenadas en la auxiliar
    r=topStack(aux);
    while(r){
        pushStack(Historial,r);
        popStack(aux);
        r=topStack(aux);
    }

    //Liberamos la memoria utilizada por la auxiliar
    free(aux);
}
//Esta funcion ocupa la lista en la cual se guardan los alimentos que se leyeron en el archivo, como los editados e ingresados por el usuario.
//Reescribiendolos en el mismo documento.
void reescribir(List* lista){
    FILE* file;
    int i,t;
    Comida* Alimento;
    Alimento = first(lista);
    file = fopen("Alimentos.csv", "w" );
    fprintf(file,"%s;%s;%s;%s;%s\n","NOMBRE","CALORIAS","GRASAS","CARBOHIDRATOS","PROTEINAS");
    t = get_size(lista);
    //Se pasa por toda la lista de alimentos escribiendo en el archivo.
    for(i = 0 ; i<t ; i++){
        fprintf(file,"%s;%ld;%ld;%ld;%ld\n",Alimento->nombre,Alimento->calorias,Alimento->grasas,Alimento->carbohidratos,Alimento->proteinas);
        Alimento = next(lista);
    }
    fclose(file);
}

int main(){
    //Creamos las estructuras de almacenamiento de datos que nos ayudaran a mostrar informacion de entrenamiento al usuario y ayudarlo a realizar ejercicio complementado con un seguimiento personal
    HashMap* Ejercicios=createMap(100);
    HashMap* Tipos=createMap(10);
    HashMap* Comidas=createMap(100);
    int masaCorporal = 0;
    Stack *stackIMC = createStack();
    Stack *Dieta = createStack();
    Stack *stackH = createStack();
    List* Rutinas=createList();
    List* Alimentos=createList();
    
    //Cargamos los datos e iniciamos la interfaz que el usuario visualizara
    int caso=2;
    Cargar(Ejercicios,Tipos);
    cargarComida(Comidas,Alimentos);
    printf("-----------------------------------------------------------------------\n");
    printf("                            TRAININGSAPP                             \n");
    printf("-----------------------------------------------------------------------\n");
    printf("Hola, te damos la bienvenida a TrainingsApp una aplicacion que te ayudara a mantener tu entrenamiento al dia\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Por favor ingresa tu peso en kg sin decimales:");
    scanf("%d",&masaCorporal);

    printf("-----------------------------------------------------------------------\n");

    //Se muestran opciones del menu al usuario
    //**************************************MENU PRINCIPAL**********************************************
    while(caso!=0)
    {
        printf(" 1. Crear rutina\n");
        printf(" 2. Realizar rutina\n");
        printf(" 3. Rutinas\n");
        printf(" 4. Calorias quemadas\n");
        printf(" 5. Comidas \n");
        printf(" 6. Desempeno de entrenamiento\n");
        printf(" 0. Salir\n");
        scanf("%d", &caso);

        switch(caso)
        {
            case 1:crearRutina(Ejercicios,Rutinas,Tipos);break;
            case 2:realizarRutina(stackIMC, stackH, Rutinas);break;
            case 3:mostrarRutinas(stackIMC, stackH,Rutinas);break;
            case 4:caloriasQuemadas(stackH,masaCorporal);break;
            case 5:menuComida(Comidas,Dieta,Alimentos);break;
            case 6:desempenoEntrenamiento(stackIMC, stackH);break;
        }
    }
    reescribir(Alimentos);
    return 0;
}