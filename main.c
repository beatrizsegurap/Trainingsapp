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


int main(){
    HashMap* Ejercicios=createMap(100);

    int caso=2;
    printf("-----------------------------------------------------------------------\n");
    printf("                            TRAININGSAPP                             \n");
    printf("-----------------------------------------------------------------------\n");
    while(caso!=0)
    {
        printf(" 1. Crear rutina\n");
        printf(" 2.Realizar rutina\n");
        printf(" 3. Rutinas\n");
        printf(" 4. Historial de rutinas\n");
        printf(" 5. Calorias quemadas\n");
        printf(" 6. Balance de calorias\n");
        printf(" 7. Comidas \n");
        printf(" 8. Desempeño de entrenamiento diario\n");
        printf(" 0. Salir\n");
        scanf("%d", &caso);

        switch(caso)
        {
            case 1:break;
            case 2:break;
            case 3:break;
            case 4:break;
            case 5:break;
            case 6:break;
            case 7:break;
            case 8:break;
        }
    }
    return 0;
}