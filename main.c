#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"
#include "list.h"

typedef struct{
    char nombre[100];
    void* ejercicios;
    long calorias;
    long id;
    long tiempo;
}Rutina;

typedef struct{
    char nombre[50];
    char tipo[20];
    void* dificultad;
}Ejercicio;

typedef struct{
    char nivel[12];
    char descripcion[300];
    long tiempo;
    int repeticiones;
    long caloriasQuemadas;
}Dificultad;

typedef struct{
    char nombre[50];
    long carbohidratos;
    long grasas;
    long proteinas;
    long calorias;
}Comida;

void CalcularIMC(Stack *HistorialI)
{
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
    push(HistorialI,IMC);
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
        i= *((float*)top(HistorialI)); 
        push(s2,top(HistorialI));
        pop(HistorialI);
        printf("IMC : %f\n",i);
    }
    //printf("------------------------------------------------------\n");
    while (get_size(s2)!=0)
    {
        push(HistorialI,top(s2));
        pop(s2);
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
    Stack *stackIMC = createStack();

    int caso=2;
    printf("-----------------------------------------------------------------------\n");
    printf("                            TRAININGSAPP                             \n");
    printf("-----------------------------------------------------------------------\n");
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