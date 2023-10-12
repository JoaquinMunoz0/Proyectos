#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

struct Guardian
{
    char nombre[60];
    char tipo[20];
    int PV;
    int PA;
    int PD;
};

struct Partida
{
    char ganador[50];
    int puntosJugador1;
    int puntosJugador2;
};

void mostrarMenu();
void crearNuevaCarta(struct Guardian cartas[], int *numCartas);
void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void iniciarPartida(struct Guardian cartas[], struct Guardian manoJugador[], struct Guardian manoComputadora[], int *numCartas, int *vidasJugador, int *vidasComputadora);
void mostrarMano(struct Guardian mano[], int numCartas);
int seleccionarGuardianAleatorio(int numCartas);

int main()
{
    struct Guardian cartas[60];
    struct Guardian manoJugador[3];
    struct Guardian manoComputadora[3];
    int numCartas = 0;
    int vidasJugador = 5;
    int vidasComputadora = 5;
    srand(time(NULL));

    int opcion;
    do
	{
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                crearNuevaCarta(cartas, &numCartas);
                break;
            case 2:
                iniciarPartida(cartas, manoJugador, manoComputadora, &numCartas, &vidasJugador, &vidasComputadora);
                break;
            case 3:
                cargarCartasDesdeArchivo(cartas, &numCartas);
                break;
            case 4:
                // Mostrar historial
                break;
            case 5:
                printf("Cerrando el Juego...\n");
                break;
            default:
                printf("Opcion no valida. Intentalo de nuevo.\n");
                break;
        }
    } while (opcion != 5);

    return 0;
}

//FUNCION PARA CARGAR CARTAS DESDE .TXT
void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas) //COMPLETADA Y FUNCIONANDO
{
    system("cls");
    FILE *archivo;
    char nombreArchivo[100];
    printf("Ingrese el nombre del archivo a cargar (incluyendo la extension .txt): ");
    scanf("%s", nombreArchivo);

    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        printf("\nPresiona una tecla para continuar...");
    	getch();
    	system("cls");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        char nombre[60], tipo[20];
        int PV, PA, PD;
        sscanf(linea, "%[^,],%[^,],%d,%d,%d", nombre, tipo, &PV, &PA, &PD);
        
        strcpy(cartas[*numCartas].nombre, nombre);
        strcpy(cartas[*numCartas].tipo, tipo);
        cartas[*numCartas].PV = PV;
        cartas[*numCartas].PA = PA;
        cartas[*numCartas].PD = PD;

        (*numCartas)++;
        if(*numCartas >= 60)
        {
            printf("Ya se han cargado 60 cartas.\n");
            break;
        }
    }

    fclose(archivo);
    printf("Se han cargado las cartas desde el archivo.\n\n");
    for(int i = 0; i < *numCartas; i++)
    {
        printf("Nombre: %s, Tipo: %s, PV: %d, PA: %d, PD: %d\n",
               cartas[i].nombre, cartas[i].tipo, cartas[i].PV, cartas[i].PA, cartas[i].PD);
    }
    
    printf("\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

//FUNCION PARA CREAR CARTAS
void crearNuevaCarta(struct Guardian cartas[], int *numCartas)//COMPLETADA Y FUNCIONANDO
{
	system("cls");
    if(*numCartas >= 60)
	{
        printf("Se ha alcanzado el limite de cartas.\n");
        return;
    }

    struct Guardian nuevaCarta;
    printf("Ingrese el nombre del Guardian (sin espacios, máximo 59 caracteres): ");
    scanf("%s", nuevaCarta.nombre);
    printf("Ingrese los puntos de vida (PV): ");
    scanf("%d", &nuevaCarta.PV);
    printf("Ingrese los puntos de ataque (PA): ");
    scanf("%d", &nuevaCarta.PA);
    printf("Ingrese los puntos de defensa (PD): ");
    scanf("%d", &nuevaCarta.PD);

    if(strlen(nuevaCarta.nombre) > 59 || nuevaCarta.PV < 0 || nuevaCarta.PA < 0 || nuevaCarta.PD < 0)
	{
        printf("Los atributos del guardian son invalidos. Intentalo de nuevo.\n");
        return;
    }

    printf("Seleccione el tipo de guardian:\n");
    printf("1. Mago\n");
    printf("2. Vikingo\n");
    printf("3. Nigromante\n");
    printf("4. Bestia\n");
    int tipoGuardian;
    scanf("%d", &tipoGuardian);
    if(tipoGuardian < 1 || tipoGuardian > 4)
	{
        printf("Tipo de Guardian Inexistente.\n");
        return;
    }
    
    switch(tipoGuardian)
	{
        case 1:
            strcpy(nuevaCarta.tipo, "Mago");
            break;
        case 2:
            strcpy(nuevaCarta.tipo, "Vikingo");
            break;
        case 3:
            strcpy(nuevaCarta.tipo, "Nigromante");
            break;
        case 4:
            strcpy(nuevaCarta.tipo, "Bestia");
            break;
        default:
            printf("Tipo de guardian Inexistente.\n");
            return;
    }

    cartas[*numCartas] = nuevaCarta;
    (*numCartas)++;
    printf("Carta creada exitosamente.\n");
    printf("\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

//FUNCION MOSTRAR MENU
void mostrarMenu()//COMPLETA Y FUNCIONANDO (SERIA RARO QUE NO FUNCIONARA PERO BUENO)
{
    printf("Bienvenido a The Clash Of The Guardians!\n");
    printf("1. Crear nueva carta\n");
    printf("2. Jugar partida\n");
    printf("3. Cargar cartas desde archivo\n");
    printf("4. Historial de la partida\n");
    printf("5. Salir\n");
    printf("Seleccione una opcion: ");
}

//FUNCION ENCARGADA DEL BUCLE DEL JUEGO
void iniciarPartida(struct Guardian cartas[], struct Guardian manoJugador[], struct Guardian manoComputadora[], int *numCartas, int *vidasJugador, int *vidasComputadora)//AUN FALTAN DETALLES
{
	system("cls");
    for(int i = 0; i < 3; i++)
	{
        int indiceAleatorio = rand() % *numCartas;

        manoJugador[i] = cartas[indiceAleatorio];
        for(int j = indiceAleatorio; j < *numCartas - 1; j++)
		{
            cartas[j] = cartas[j + 1];
        }
        (*numCartas)--;

        indiceAleatorio = rand() % *numCartas;
        manoComputadora[i] = cartas[indiceAleatorio];
        for (int j = indiceAleatorio; j < *numCartas - 1; j++)
		{
            cartas[j] = cartas[j + 1];
        }
        (*numCartas)--;
    }
    
    *vidasJugador = 100;
    *vidasComputadora = 100;

    printf("La Partida a comenzado. El Jugador y la Computadora han recibido sus cartas.\n");

    int turno = 1;
    while(*vidasJugador > 0 && *vidasComputadora > 0)
	{
        printf("\nEstado actual de la partida:\n");
        printf("Vidas del Jugador: %d\n", *vidasJugador);
        printf("Vidas de la Computadora: %d\n", *vidasComputadora);

        if (turno == 1)
		{
            printf("\nTurno del Jugador:\n");
            mostrarMano(manoJugador, 3);
            int seleccionGuardian;
            do
			{
                printf("Que Guardian enviaras al campo de batalla? (1-3): ");
                scanf("%d", &seleccionGuardian);
            } while (seleccionGuardian < 1 || seleccionGuardian > 3);
            
            int dano = manoJugador[seleccionGuardian - 1].PA - manoComputadora[0].PD;
            if(dano < 0)
			{
                dano = 0;
            }

            printf("¡%s ataca a la Computadora por %d puntos de dano!\n", manoJugador[seleccionGuardian - 1].nombre, dano);
            *vidasComputadora -= dano;

            if(*vidasComputadora <= 0)
			{
                printf("¡%s ha derrotado al Guardian de la Computadora!\n", manoJugador[seleccionGuardian - 1].nombre);
                break;
            }
        }
        
        else
		{
            printf("\nTurno de la Computadora:\n");
            int seleccionGuardian = seleccionarGuardianAleatorio(3);
            int dano = manoComputadora[seleccionGuardian].PA - manoJugador[0].PD;
            if(dano < 0)
			{
                dano = 0;
            }

            printf("¡%s ataca al Jugador por %d puntos de dano!\n", manoComputadora[seleccionGuardian].nombre, dano);
            *vidasJugador -= dano;

            if(*vidasJugador <= 0)
			{
                printf("¡%s ha derrotado al guardian del Jugador!\n", manoComputadora[seleccionGuardian].nombre);
                break;
            }
        }
        
        turno = (turno == 1) ? 2 : 1;
    }

    if(*vidasJugador <= 0)
	{
        printf("\nLa Computadora ha ganado la partida\n");
    }
	else
	{
        printf("\nHas ganado la partida\n");
    }
    printf("\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

//FUNCION ENCARGADA DE MOSTAR MANO
void mostrarMano(struct Guardian mano[], int numCartas)
{
    printf("Tus guardianes en mano:\n");
    for(int i = 0; i < numCartas; i++)
	{
        printf("%d. Nombre: %s, PV: %d, PA: %d, PD: %d, Tipo: %s\n", i + 1, mano[i].nombre, mano[i].PV, mano[i].PA, mano[i].PD, mano[i].tipo);
    }
}

int seleccionarGuardianAleatorio(int numGuardianes)
{
    return rand() % numGuardianes;
}