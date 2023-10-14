#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

struct Guardian
{
    char nombre[60];
    char tipo[20];
    int PV;
    int PA;
    int PD;
    struct Guardian* siguiente;
};

typedef struct Guardian Guardian;

Guardian* crearGuardian(char nombre[], int PV, int PA, int PD, char tipo[])
{
    Guardian* nuevoGuardian = (Guardian*)malloc(sizeof(Guardian));
    if (nuevoGuardian == NULL)
    {
        printf("Error: No se pudo asignar Memoria para el nuevo guardian.\n");
        exit(1);
    }
    strcpy(nuevoGuardian->nombre, nombre);
    nuevoGuardian->PV = PV;
    nuevoGuardian->PA = PA;
    nuevoGuardian->PD = PD;
    strcpy(nuevoGuardian->tipo, tipo);
    nuevoGuardian->siguiente = NULL;
    return nuevoGuardian;
}

//DECLARACION DE LA FUNCION MOSTRARCARTAS
void mostrarCartas(Guardian* pila);

//FUNCION PARA AGREGAR CARTAS A LA PILA.
void push(Guardian** pila, Guardian* nuevoGuardian)
{
    nuevoGuardian->siguiente = *pila;
    *pila = nuevoGuardian;
}

//FUNCION PARA EL MENU (NO HAY MUCHO QUE DECIR)
void mostrarMenu()
{
    printf("Bienvenido a The Clash Of The Guardians!\n");
    printf("1. Crear nueva carta\n");
    printf("2. Jugar partida\n");
    printf("3. Cargar cartas desde archivo\n");
    printf("4. Ver Cartas Disponibles\n");
    printf("5. Historial de la partida\n");
    printf("6. Salir\n");
    printf("Seleccione una opcion: ");
}

//FUNCION PARA CREAR UNA CARTA NUEVA Y AGREGARLA A LA PILA ACTUALIZADA Y FUNCIONANDO
void crearNuevaCarta(Guardian** pila)
{
    system("cls");
    char nombre[60];
    int PV, PA, PD;
    int tipoNumero;

    fflush(stdin);

    printf("Ingrese el nombre del Guardian: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    printf("Ingrese los puntos de vida (PV): ");
    scanf("%d", &PV);

    printf("Ingrese los puntos de ataque (PA): ");
    scanf("%d", &PA);

    printf("Ingrese los puntos de defensa (PD): ");
    scanf("%d", &PD);

    printf("Seleccione el tipo de guardian:\n");
    printf("1. Mago\n");
    printf("2. Vikingo\n");
    printf("3. Nigromante\n");
    printf("4. Bestia\n");
    scanf("%d", &tipoNumero);

    char tipo[20];
    switch (tipoNumero)
    {
    case 1:
        strcpy(tipo, "Mago");
        break;
    case 2:
        strcpy(tipo, "Vikingo");
        break;
    case 3:
        strcpy(tipo, "Nigromante");
        break;
    case 4:
        strcpy(tipo, "Bestia");
        break;
    default:
        printf("Secreto Encontrado.\n");
        strcpy(tipo, "Samurai");
        break;
    }

    Guardian* nuevoGuardian = crearGuardian(nombre, PV, PA, PD, tipo);
    push(pila, nuevoGuardian);

    printf("Nueva carta creada y agregada al mazo.\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

//Cargar Cartas DESDE ARCHIVOS ACTUALIZADA Y FUNCIONANDO
void cargarCartasDesdeArchivo(Guardian** pila)
{
    FILE *archivo;
    char nombreArchivo[100];
    printf("Ingrese el nombre del archivo a cargar (incluyendo la extension .txt): ");
    scanf("%s", nombreArchivo);

    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        char nombre[60], tipo[20];
        int PV, PA, PD;
        if (sscanf(linea, "%[^,],%[^,],%d,%d,%d", nombre, tipo, &PV, &PA, &PD) == 5)
        {
            Guardian* nuevoGuardian = crearGuardian(nombre, PV, PA, PD, tipo);
            push(pila, nuevoGuardian);
        }
        else
        {
            printf("Error encontrado en la Linea: %s\n", linea);
        }
    }

    fclose(archivo);
    printf("\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

int main()
{
    Guardian* pila = NULL;
    bool cartasDisponibles = false;

    int opcion;
    do
    {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        	system("cls");
            crearNuevaCarta(&pila);
            cartasDisponibles = true;
            break;
        case 2:
            // Implementar jugar partida
            break;
        case 3:
        	system("cls");
            cargarCartasDesdeArchivo(&pila);
            cartasDisponibles = true;
            break;
        case 4:
        	system("cls");
            if (cartasDisponibles)
			{
                mostrarCartas(pila);
            } else
			{
                printf("No hay cartas disponibles.\n");
            }
            printf("\nPresiona una tecla para continuar...");
    		getch();
    		system("cls");
            break;
        case 5:
            // Implementar historial de la partida
            break;
        case 6:
            printf("Cerrando el Juego...\n");
            break;
        default:
            printf("Opcion no valida. Intentalo de nuevo.\n");
            break;
        }
    } while (opcion != 6);

    // Liberar cartas de la pila
    Guardian* temp;
    while (pila != NULL)
    {
        temp = pila;
        pila = pila->siguiente;
        free(temp);
    }

    return 0;
}

//FUNCION PARA MOSTRAR LAS CARTAS DE LA PILA
void mostrarCartas(Guardian* pila)
{
    Guardian* temp = pila;
    int contador = 1;

    printf("Cartas en la pila:\n");
    while (temp != NULL)
    {
        printf("%d. Nombre: %s, Tipo: %s, PV: %d, PA: %d, PD: %d\n",
               contador, temp->nombre, temp->tipo, temp->PV, temp->PA, temp->PD);

        temp = temp->siguiente;
        contador++;
    }
}