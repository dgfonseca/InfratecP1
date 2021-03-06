// Proyecto 1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

/*
PROYECTO 1 FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - 201820
David Fonseca - 201729497--dg.fonseca	
Allan Corinaldi - 201712677--ar.corinaldi
*/

#include "pch.h"

//-- Definicion de la estructura para los datos del archivo de entrada y el de salida
typedef struct datos
{
	int tamanio;
	unsigned char *informacion;
} Datos;


//-- Prototipos de las funciones
int readFile(Datos * archivo, char *);
void writeFile(int, Datos * archivoEnOctal, char *);
void convertirAOctal(Datos * archivo, Datos * archivoEnOctal);
unsigned char pasarTriplaToASCII(unsigned char tripla);


//-- Funciones

// Esta funcion se encarga de abrir un datos y leerlo en el vector informacion.
// Retorna el numero de bytes leidos.
// No hay que completar nada en esta funcion.
int readFile(Datos * archivo, char * nombreArchivo)
{
	FILE *file;
	int n;
	int x;

	file = fopen(nombreArchivo, "rb");

	if (!file)
	{
		printf("No se pudo abrir el archivo para leer: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	n = ftell(file);
	fseek(file, 0, SEEK_SET);

	archivo->tamanio = n;
	archivo->informacion = (unsigned char *)calloc(n, sizeof(unsigned char));

	for (x = 0; x < n; x++)
	{
		fread(&archivo->informacion[x], sizeof(unsigned char), 1, file);
	}
	archivo->informacion[x] = '\0';

	fclose(file);

	return n;
}


// Esta funcion se encarga de escribir un datos a partir del vector datos.
// El numero de bytes que se deben escribir viene en el parametro n.
// No hay que completar nada en esta funcion.
void writeFile(int n, Datos * archivoEnOctal, char *nombreArchivo)
{
	FILE *file;

	file = fopen(nombreArchivo, "wb");

	if (!file) {
		printf("No se pudo abrir el archivo para escribir: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}

	fwrite((archivoEnOctal->informacion), 1, n, file);

	fclose(file);
}


// Esta funcion se encarga de convertir a octal cada uno de los bytes que se 
//encuentran en la estructura datosBin->informacion y asignar los nuevos valores a la estructura datosOct->informacion.
// Deben desarrollar esta funcion en su totalidad.
void convertirAOctal(Datos * datosBin, Datos * datosOct)
{
	//TODO: COMPLETAR EL DESARROLLO DE LA FUNCION.
	int tamanOctal = 0;
	int j = -1; //donde apunta datosOct->informacion.
	unsigned char sobra1 = 0;
	unsigned char sobra2 = 0;
	unsigned char tripla1, tripla2, tripla3, tripla4;

	for (int i = 0; i < datosBin->tamanio; i++)
	{
		unsigned char byte = *datosBin->informacion;
		//byte numero entre 0 y 255. 
		

		if (i % 3 == 0) 
		{
			/*
				i % 3 == 0.
				byte donde se agrupa de la siguiente manera:
				110 - 101 - 01
				Tripla1: 00000 - 110 
				Tripla2: 00000 - 101 
				sobra2: 01 - 000000
			*/

			tripla1 = byte >> 5;

			tripla2 = byte << 3;
			tripla2 = tripla2 >> 5;
			
			sobra2 = byte << 6;
			
			if (datosBin->tamanio - 1 == i)
			{
				/*
					En caso de que sea el ultimo byte, se rellena con un 0 a la derecha sobra2.
					si sobra2: 01 -> 010
					010 se vuelve una tripla, tripla3: 00000 - 010
				*/
				tripla3 = sobra2 >> 5;
			}
		}
		else if ( i % 3 == 1 )
		{
			/*
				Recordar sobra2: 01 - 000000
				i % 3 == 1.
				byte donde se agrupa de la siguiente manera:
				[fff] := 1 - 101 - 010 - 1
				
				El primer bit de [fff] se agrupan con los dos bits de sobra2.
				se usa -> (sobra2 | [fff] >> 2), para anadir los bits que sobraron.
				(01)1 - 101 - 01 (bits en parentesis son los de sobra2).
				Hago corrimiento de 5 a la derecha para tener la tripla1.
				Tripla1: 00000 - 011

				Corrimiento de 1 a la izquierda a [fff] para quitar el bit que se uso en la tripla1. 
				(101)0101 y corro 5 a la derecha.
				Tripla2: 00000 - 101.

				Corrimiento de 4 a la izquierda a [fff].
				(010)10000 luego corrimiento de 5 a la derecha.
				Tripla3: 00000 - 010

				sobra1: 1 - 0000000
				
			*/

			unsigned char agrego = sobra2 | (byte >> 2);
			tripla1 = agrego >> 5;

			tripla2 = byte << 1;
			tripla2 = tripla2 >> 5;

			tripla3 = byte << 4;
			tripla3 = tripla3 >> 5;

			sobra1 = byte << 7;

			if (datosBin->tamanio - 1 == i)
			{
				/*
					En caso de que sea el ultimo byte, se rellena con dos 0 a la derecha sobra1.
					si sobra1: 1 -> 100
					100 se vuelve una tripla, tripla3: 00000 - 100
				*/
				tripla4 = sobra1 >> 5;
			}
				
		}
		else if( i%3 == 2 )
		{
			/*
				Recordar sobra1: 1 - 0000000
				i % 3 == 2.
				byte donde se agrupa de la siguiente manera:
				[fff] := 11 - 010 - 101

				Los dos primeros bits de [fff] se agrupan con sobra1.
				se usa -> (sobra1 | [fff] >> 1), para anadir los bits que sobraron.
				(1)11 - 101 - 01 (bit en parentesis el de sobra1).
				Hago corrimiento de 5 a la derecha para tener la tripla1.
				Tripla1: 00000 - 011

				Corrimiento de 2 a la izquierda a [fff] para quitar los bits que se usaron en la tripla1.
				(010)10100 y corro 5 a la derecha.
				Tripla2: 00000 - 010.

				Corrimiento de 5 a la izquierda a [fff].
				(101)00000 luego corrimiento de 5 a la derecha.
				Tripla3: 00000 - 101
			*/
			unsigned char agrego = sobra1 | (byte >> 1);
			tripla1 = agrego >> 5;

			tripla2 = byte << 2;
			tripla2 = tripla2 >> 5;

			tripla3 = byte << 5;
			tripla3 = tripla3 >> 5;

		}

		tripla1 = pasarTriplaToASCII(tripla1);
		datosOct->informacion[++j] = tripla1; //Guarda la tripla en la direccion de memoria.
		tamanOctal++; //TamanOctal es el tamanio en bytes del archivo.

		tripla2 = pasarTriplaToASCII(tripla2);
		datosOct->informacion[++j] = tripla2;
		tamanOctal++;

		if ((i == datosBin->tamanio - 1 && i % 3 == 0) || (i % 3 == 2) || (i % 3 == 1)) //Excepcion de cuando hay una tercera tripla
		{
			tripla3 = pasarTriplaToASCII(tripla3);
			datosOct->informacion[++j]= tripla3;
			tamanOctal++;
		}
		if (i % 3 == 1 && i == datosBin->tamanio - 1) //Excepcion de cuando hay una cuarta tripla
		{
			tripla4 = pasarTriplaToASCII(tripla4);
			datosOct->informacion[++j] = tripla4;
			tamanOctal++;
		}

		datosBin->informacion++;
	}
	datosOct->tamanio = tamanOctal;
}

/**
*	tripla esta del 0 al 7 en ascii.
*	retorna un numero del 48 al 55 que es la representacion en ascii de los numeros del 0 al 7. 
**/
unsigned char pasarTriplaToASCII(unsigned char tripla)
{
	unsigned char rta = tripla;
	if (rta == 0) rta = 48;
	else if (rta == 1) rta = 49;
	else if (rta == 2) rta = 50;
	else if (rta == 3) rta = 51;
	else if (rta == 4) rta= 52;
	else if (rta == 5) rta = 53;
	else if (rta == 6) rta = 54;
	else if (rta == 7) rta = 55;

	return rta;
}


//-- Funcion main de la aplicacion
// No hay que completar nada en esta funcion.
int main()
{
	// Mensaje inicial
	printf("PROYECTO 1 - INFRATEC (ISIS-1304)");
	// Declaracion de todas las variables necesarias.
	int tamanio;
	int tamanioRepOctales;
	char nombreArchivo[200];
	char nombreArchivoOctales[200];
	Datos * archivo = (Datos *)malloc(sizeof(Datos));
	Datos * archivoEnOctal = (Datos *)malloc(sizeof(Datos));

	printf("\n\nIngrese el nombre del archivo a leer (incluya el formato. i.e. archivo.txt): \n");
	scanf("%s", nombreArchivo);
	tamanio = readFile(archivo, nombreArchivo);
	printf("\nEl tamanio del archivo es de %d bytes\n", tamanio);
	tamanioRepOctales = ((tamanio * 8) / 3) + ((tamanio % 3 == 0) ? 0 : 1);
	archivoEnOctal->informacion = (unsigned char *)calloc(tamanioRepOctales, sizeof(unsigned char));
	printf("\nIngrese el nombre del archivo para guardar la representacion en octales del archivo origen (incluya el .txt): \n");
	scanf("%s", &nombreArchivoOctales);
	convertirAOctal(archivo, archivoEnOctal);
	writeFile(tamanioRepOctales, archivoEnOctal, nombreArchivoOctales);
	printf("\nEl archivo se transformo en su representacion a octal exitosamente!\n\n");
	printf("Revisa el archivo con nombre '%s' para ver el resultado!\n", nombreArchivoOctales);

	// Terminar
	system("PAUSE");
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
