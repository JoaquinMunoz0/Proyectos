#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

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
    if(nuevoGuardian == NULL)
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

//DECLARACION DE FUNCIONES
void mostrarMenu();
void crearNuevaCarta(Guardian** pila);
void cargarCartasDesdeArchivo(Guardian** pila);
void mostrarCartas(Guardian* pila);
void revolverCartas(Guardian** pila);
int repartirCartas(Guardian** pila, Guardian** pilaJugador, Guardian** pilaComputadora, Guardian** manoJugador, Guardian** manoComputadora);
void BucleJuego(Guardian* pilaJugador, Guardian* pilaComputadora, Guardian* manoJugador, Guardian* manoComputadora);
void mostrarManoJugador(Guardian* manoJugador);
void mostrarManoComputadora(Guardian* manoComputadora);
void push(Guardian** pila, Guardian* nuevaCarta);
void enviarAlCampo(Guardian* manoJugador, Guardian* campoBatallaJugador[]);
void TurnoComputadora(Guardian* manoComputadora, Guardian* campoBatallaJugador[], Guardian* campoBatallaComputadora[], int* VidasJugador, Guardian** pilaComputadora, int* numCartasManoComputadora);
void mostrarCampoBatalla(Guardian* campoBatallaJugador[], Guardian* campoBatallaComputadora[]);
Guardian* pop(Guardian** mazo);

int main()
{
    Guardian* pila = NULL;
	Guardian* pilaJugador = NULL;
	Guardian* pilaComputadora = NULL;
	Guardian* manoJugador = NULL;
	Guardian* manoComputadora = NULL;
    bool cartasDisponibles = false;

    int opcion;
    do
    {
        mostrarMenu();
        scanf("%d", &opcion);

        switch(opcion)
        {
        case 1:
        	system("cls");
            crearNuevaCarta(&pila);
            cartasDisponibles = true;
            break;
        case 2:
		    system("cls");
		    if (cartasDisponibles)
			{
		        revolverCartas(&pila);
		        printf("Las cartas han sido revueltas ");
		        repartirCartas(&pila, &pilaJugador, &pilaComputadora, &manoJugador, &manoComputadora);
		        printf("y repartidas.\nSe te han otorgado 15 cartas de las cuales 3 fueron a tu mano.\n");
		        BucleJuego(pilaJugador, pilaComputadora, manoJugador, manoComputadora);
		    }
			else
			{
		        printf("No hay cartas disponibles para jugar. Crea nuevas cartas o carga desde un archivo.\n");
		    }
		    printf("\nPresiona una tecla para continuar...");
		    getch();
		    system("cls");
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
            // IMPLEMENTAR HISTORIAL
            break;
        case 6:
            printf("Cerrando el Juego...\n");
            break;
        default:
            printf("Opcion no valida. Intentalo de nuevo.\n");
            break;
        }
    } while (opcion != 6);

    // LIBERAR CARTAS DE LA PILA
    Guardian* temp;
    while(pila != NULL)
    {
        temp = pila;
        pila = pila->siguiente;
        free(temp);
    }

    return 0;
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

//FUNCION PARA CREAR UNA CARTA NUEVA Y AGREGARLA A LA PILA
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
    switch(tipoNumero)
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

//Cargar Cartas DESDE ARCHIVOS
void cargarCartasDesdeArchivo(Guardian** pila)
{
    FILE *archivo;
    char nombreArchivo[100];
    printf("Ingrese el nombre del archivo a cargar (incluyendo la extension .txt): ");
    scanf("%s", nombreArchivo);

    archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        printf("\nPresiona una tecla para continuar...");
    	getch();
    	system("cls");
        return;
    }

    char linea[256];
    while(fgets(linea, sizeof(linea), archivo) != NULL)
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
            printf("Error encontrado en la Linea de texto: %s\n", linea);
        }
    }

    fclose(archivo);
    printf("\nSe han cargado las cartas\nPresiona una tecla para continuar...");
    getch();
    system("cls");
}

//FUNCION PARA AGREGAR CARTAS A LA PILA
void push(Guardian** pila, Guardian* nuevaCarta)
{
    nuevaCarta->siguiente = *pila; // La siguiente carta en el pila es la actual primera carta
    *pila = nuevaCarta; // La nueva carta se convierte en la primera carta del pila
}

//FUNCION PARA QUITAR CARTAS A LA PILA
Guardian* pop(Guardian** mazo)
{
    if(*mazo == NULL)
	{
        printf("El mazo está vacío.\n");
        return NULL;
    }

    Guardian* carta = *mazo;
    *mazo = (*mazo)->siguiente;
    carta->siguiente = NULL;

    return carta;
}

//FUNCION PARA MOSTRAR LAS CARTAS COMPLETAS
void mostrarCartas(Guardian* pila)
{
    Guardian* temp = pila;
    int contador = 1;

    while (temp != NULL)
    {
        printf("%d. Nombre: %s, Tipo: %s, PV: %d, PA: %d, PD: %d\n",
               contador, temp->nombre, temp->tipo, temp->PV, temp->PA, temp->PD);
        temp = temp->siguiente;
        contador++;
    }
}

//FUNCION PARA REVOLVER LAS CARTAS
void revolverCartas(Guardian** pila)
{
	
    Guardian* cartasArray[60];
    int numCartas = 0;
    Guardian* temp = *pila;
    while (temp != NULL)
	{
        cartasArray[numCartas] = temp;
        temp = temp->siguiente;
        numCartas++;
    }
    
    srand(time(NULL));
    for(int i = numCartas - 1; i > 0; i--)
	{
        int j = rand() % (i + 1);
        Guardian* temp = cartasArray[i];
        cartasArray[i] = cartasArray[j];
        cartasArray[j] = temp;
    }

    *pila = NULL;
    for(int i = 0; i < numCartas; i++)
	{
        cartasArray[i]->siguiente = *pila;
        *pila = cartasArray[i];
    }
}

//FUNCION PARA REPARTIR LAS CARTAS
int repartirCartas(Guardian** pila, Guardian** pilaJugador, Guardian** pilaComputadora, Guardian** manoJugador, Guardian** manoComputadora)
{
    int contador = 1;
    Guardian* temp = *pila;
    while (temp != NULL && contador <= 30)
	{
        Guardian* carta = crearGuardian(temp->nombre, temp->PV, temp->PA, temp->PD, temp->tipo);
        if(contador <= 15)
		{
            push(pilaJugador, carta);
        }
		else
		{
            push(pilaComputadora, carta);
        }
        temp = temp->siguiente;
        contador++;
    }

    for(int i = 1; i <= 30; i++)
	{
        Guardian* temp = *pila;
        *pila = (*pila)->siguiente;
        free(temp);
    }
    
    for(int i = 0; i < 3; i++)
	{
        Guardian* carta = pop(pilaJugador);
        if(carta != NULL)
		{
            push(manoJugador, carta);
        }
    }
    
    for(int i = 0; i < 3; i++)
	{
        Guardian* carta = pop(pilaComputadora);
        if (carta != NULL)
		{
            push(manoComputadora, carta);
        }
    }

    return contador - 1;
}

//MOSTRAR LA MANO DEL JUGADOR
void mostrarManoJugador(Guardian* manoJugador)
{
    Guardian* temp = manoJugador;
    int contador = 1;

    printf("Cartas en tu mano:\n");
    while (temp != NULL) {
        printf("%d. Nombre: %s, Tipo: %s, PV: %d, PA: %d, PD: %d\n",
               contador, temp->nombre, temp->tipo, temp->PV, temp->PA, temp->PD);
        temp = temp->siguiente;
        contador++;
    }
}

//MOSTAR LA MANO DE LA COMPUTADORA
void mostrarManoComputadora(Guardian* manoComputadora)
{
    Guardian* temp = manoComputadora;
    int contador = 1;

    printf("Cartas en la mano de la computadora:\n");
    while (temp != NULL)
	{
        printf("%d. Nombre: %s, Tipo: %s, PV: %d, PA: %d, PD: %d\n",
               contador, temp->nombre, temp->tipo, temp->PV, temp->PA, temp->PD);
        temp = temp->siguiente;
        contador++;
    }
}

//BUCLE DEL JUEGO
void BucleJuego(Guardian* pilaJugador, Guardian* pilaComputadora, Guardian* manoJugador, Guardian* manoComputadora)
{
    Guardian* campoBatallaJugador[3] = {NULL, NULL, NULL};
    Guardian* campoBatallaComputadora[3] = {NULL, NULL, NULL};
    int numCartasManoComputadora = 3;
    int cartaAtacante, cartaObjetivo;
    int opcionJugador;
    int VidasJugador = 5;
    int VidasComputadora = 5;

    do
	{
        printf("Es tu turno.\n");
        mostrarManoJugador(manoJugador);
        printf("\nQue Deseas hacer?\n");
        printf("1. Atacar\n");
        printf("2. Enviar carta\n");
        printf("3. Sacar carta\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcionJugador);

        switch(opcionJugador)
		{
            case 1:
            	system("cls");
            	mostrarCampoBatalla(campoBatallaJugador, campoBatallaComputadora);
                printf("\nSelecciona con que guardian atacar(1-3): ");
                scanf("%d", &cartaAtacante);
                printf("Ahora cual sera tu objetivo? (1-3): ");
                scanf("%d", &cartaObjetivo);
                if (cartaAtacante >= 1 && cartaAtacante <= 3 && cartaObjetivo >= 1 && cartaObjetivo <= 3 &&
                    campoBatallaJugador[cartaAtacante - 1] != NULL && campoBatallaComputadora[cartaObjetivo - 1] != NULL)
                {
                    Guardian* atacante = campoBatallaJugador[cartaAtacante - 1];
                    Guardian* objetivo = campoBatallaComputadora[cartaObjetivo - 1];
                    int danio = atacante->PA > objetivo->PD ? atacante->PA - objetivo->PD : 0;
                    objetivo->PV -= danio;
                    printf("Has atacado a %s con %s. puntos de PA inflingidos: %d\n", objetivo->nombre, atacante->nombre, danio);
                    if(objetivo->PV <= 0)
                    {
                        printf("%s ha sido Vencido\n", objetivo->nombre);
                        campoBatallaComputadora[cartaObjetivo - 1] = NULL;
                    	(VidasComputadora)--;
                    }
                }
                else
                {
                    printf("Eleccion invalida.\n");
                }
                break;
            case 2:
			    enviarAlCampo(manoJugador, campoBatallaJugador);
			    break;
			case 3:
			    // Lógica para sacar carta del mazo
			    break;
            default:
                printf("Opcion invalida.\n");
                continue;
        }
        TurnoComputadora(manoComputadora, campoBatallaJugador, campoBatallaComputadora, &VidasJugador, &pilaComputadora, &numCartasManoComputadora);
        mostrarCampoBatalla(campoBatallaJugador, campoBatallaComputadora);

        // Verificar condiciones de fin del juego (implementación requerida)
        
        printf("\nPresiona una tecla para ir al siguiente turno...\n");
        getch();
        system("cls");
    }while(VidasComputadora > 0 && VidasJugador > 0);
}

//ESTA FUNCION MUESTRA EL CAMPO DE BATALLA
void mostrarCampoBatalla(Guardian* campoBatallaJugador[], Guardian* campoBatallaComputadora[])
{
    printf("Campo de Batalla:\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 3; i++)
    {
        printf("%d. %-30s", i + 1, campoBatallaJugador[i] ? campoBatallaJugador[i]->nombre : " ");
    }
    printf("\n");
    
    printf("-----------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 3; i++)
    {
        printf("%d. %-30s", i + 1, campoBatallaComputadora[i] ? campoBatallaComputadora[i]->nombre : " ");
    }
    printf("\n");
    
    printf("-----------------------------------------------------------------------------------------------\n");
}

//FUNCION CON LA LOGICA PARA ENVIAR CARTAS AL CAMPO DE BATALLA PARA EL JUGADOR
void enviarAlCampo(Guardian* manoJugador, Guardian* campoBatallaJugador[])
{
    int posicion;
    printf("Selecciona una carta de tu mano para enviar al campo de batalla (1-3): ");
    scanf("%d", &posicion);

    if (posicion < 1 || posicion > 3)
    {
        printf("Posicion invalida.\n");
        return;
    }

    if (campoBatallaJugador[posicion - 1] != NULL)
    {
        printf("Ya hay una carta en esa posicion.\n");
        return;
    }

    Guardian* cartaSeleccionada = NULL;
    Guardian* temp = manoJugador;
    int contador = 1;
    while (temp != NULL)
    {
        if (contador == posicion)
        {
            cartaSeleccionada = temp;
            break;
        }
        temp = temp->siguiente;
        contador++;
    }

    if (cartaSeleccionada == NULL)
    {
        printf("No hay ninguna carta en esa posicion.\n");
        return;
    }

    campoBatallaJugador[posicion - 1] = cartaSeleccionada;
    printf("\nHas enviado \"%s\" al campo de batalla.\n", cartaSeleccionada->nombre);
}

//FUNCION ENCARGADA DE LOS TURNOS DE LA COMPUTADORA
void TurnoComputadora(Guardian* manoComputadora, Guardian* campoBatallaJugador[], Guardian* campoBatallaComputadora[], int* VidasJugador, Guardian** pilaComputadora, int* numCartasManoComputadora)
{
    printf("\nTurno de la Computadora...\n");
    int turnoComputadora = 1;
    
    int opcion = rand() % 3 + 1;
	switch (opcion)
	{
        case 1:
            for (int i = 0; i < 3; i++)
		    {
		        if (campoBatallaComputadora[i] != NULL)
		        {
		            int cartaAtacante = i + 1;
		            int cartaObjetivo = rand() % 3 + 1;
		
		            Guardian* atacante = campoBatallaComputadora[cartaAtacante - 1];
		            Guardian* objetivo = campoBatallaJugador[cartaObjetivo - 1];
		
		            if (objetivo != NULL)
		            {
		                int danio = atacante->PA > objetivo->PD ? atacante->PA - objetivo->PD : 0;
		                objetivo->PV -= danio;
		                printf("La Computadora ha atacado a \"%s\" con \"%s\". Puntos de PA inflingidos: %d\n", objetivo->nombre, atacante->nombre, danio);
		
		                if (objetivo->PV <= 0)
		                {
		                    printf("\"%s\" Ha sido Vencido\n", objetivo->nombre);
		                    campoBatallaJugador[cartaObjetivo - 1] = NULL;
		                    (*VidasJugador)--;
		                }
		                turnoComputadora--;
		            }
		        }
		    }
            break;
        case 2:
        	for(int i = 0; i < 3; i++)
        	{
			    if (manoComputadora != NULL && campoBatallaComputadora[i] == NULL && *numCartasManoComputadora > 0)
			    {
			        Guardian* cartaSeleccionada = pop(&manoComputadora);
			        campoBatallaComputadora[i] = cartaSeleccionada;
			        printf("La Computadora ha enviado \"%s\" al campo de batalla.\n", cartaSeleccionada->nombre);
			        (*numCartasManoComputadora)--;
			        turnoComputadora--;
			    }
			}
            break;
        case 3:
            if (*numCartasManoComputadora < 3 && *pilaComputadora != NULL)
		    {
		        Guardian* cartaNueva = pop(pilaComputadora);
		        push(&manoComputadora, cartaNueva);
		        printf("La Computadora ha sacado una nueva carta del mazo.\n");
		        (*numCartasManoComputadora)++;
		        turnoComputadora--;
		    }
            break;
        default:
            for (int i = 0; i < 3; i++)
		    {
		        if (campoBatallaComputadora[i] != NULL)
		        {
		            int cartaAtacante = i + 1;
		            int cartaObjetivo = rand() % 3 + 1;
		
		            Guardian* atacante = campoBatallaComputadora[cartaAtacante - 1];
		            Guardian* objetivo = campoBatallaJugador[cartaObjetivo - 1];
		
		            if (objetivo != NULL)
		            {
		                int danio = atacante->PA > objetivo->PD ? atacante->PA - objetivo->PD : 0;
		                objetivo->PV -= danio;
		                printf("La Computadora ha atacado a \"%s\" con \"%s\". Puntos de PA inflingidos: %d\n", objetivo->nombre, atacante->nombre, danio);
		
		                if (objetivo->PV <= 0)
		                {
		                    printf("\"%s\" Ha sido Vencido\n", objetivo->nombre);
		                    campoBatallaJugador[cartaObjetivo - 1] = NULL;
		                    (*VidasJugador)--;
		                }
		                turnoComputadora--;
		            }
		        }
		    }
            break;
    }
}