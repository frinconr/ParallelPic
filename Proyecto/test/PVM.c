/*
	Calcular producto VectorxMatriz en paralelo.
	La dimension de los vectores debe ser un multiplo de la cantidad de procesos.

	La matriz se representa como un arreglo lineal.
*/


/***************************************
PPP.c:
   CI0202 - PRINCIPIOS DE INFORMATICA
   Prof.: Juan Carlos Saborío
   Grupo 02

   Ejercicio de programación: Calcular el producto vector-Matriz en paralelo utilizando MPI.

1. Agregar las líneas correspondientes en el 
método "main", siguiendo los comentarios con la
palabra HACER.

2. Completar el contenido de la función prodM.

3. Compilar y ejecutar.  Probar diferentes tamaños
de vectores.

   Para compilar utilice el comando:
   $mpicc.openmpi PVM.c -o [nombre binario]

   Para ejecutar utilice el comando
   $mpiexec.openmpi -n [np] ./[nombre binario]

donde 'np' es el número de procesadores, entre 1 y 4.
****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

void inicializarV(int * v, long l);
void inicializarM(int * v, long n, long m);
void mostrarV(int * v, long l);
void mostrarM(int * M, long n, long m);
void prodM(int * v, int * M, long n, long m, int * y);
void despl(char * mensaje, int id);

int main(int argc, char ** argv){

	int procs, id;
	long N=10;
	long M=10;
	long N_local=10;
	int * v; //Puntero para el vector
	int * m; //Puntero para la matriz

//Inicializar ambiente MPI
	MPI_Init(&argc, &argv); //Iniciar MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &id); //Determinar id de cada proceso
	MPI_Comm_size(MPI_COMM_WORLD, &procs); //Determinar cantidad de procesos

   	if (id == 0) {
		srand(time(NULL));
		if(argc > 1)
			N = atol(argv[1]);
		M = N;

		printf("%i procesos.\nUtilizando vector de dimension %i y matriz %ix%i\n", procs, N, N, M);
		double mem = (double) (((N+N*M)*sizeof(int)) + ((N/procs)*M+N)*sizeof(int)*procs + (N/procs * sizeof(int) * procs));
		mem = (double) mem / (1024*1024);
		printf("Utilizando aprox. %3.2f MB RAM.\n", mem);

//Reservar memoria para el vector y la matriz
		v = (int *) malloc(N * sizeof(int));//v_;
		m = (int *) malloc(N * M * sizeof(int));//m_;

		printf("Inicializando arreglo.\n");
		inicializarV(v, N);

		printf("Inicializando matriz.\n");
		inicializarM(m,N,M);

		printf("Preparacion finalizada.\n");		
	}

/* HACER: Difundir el valor de N y M*/
	if(id == 0) printf("Repartiendo N = %i, N_local = %i y M = %i.\n", N, N/procs, M);

	MPI_Barrier(MPI_COMM_WORLD);
	double t1 = MPI_Wtime();

//HACER: Broadcast de N
	MPI_Bcast(*direccion de N*, *cantidad de elementos*, MPI_INT, *origen*, MPI_COMM_WORLD);
	MPI_Bcast(*direccion de M*, *cantidad de elementos*, MPI_INT, *origen*, MPI_COMM_WORLD);

//HACER: cada proceso calcula N_local

/* Distribuir el vector y la matriz */

	//Los procesos mayores que 0 declaran arreglo v
	if(id > 0) v = (int *) malloc(N * sizeof(int));
	//Por uniformidad de llamados MPI, hacer puntero 'a' que apunte  a 'v'
	int * a = v;
	//Declarar y reservar espacio para submatriz B

	//Hacer: calcular tamano de matriz B
	int tam_B = **;
	int * B = (int *) malloc((tam_B)*sizeof(int));

	despl("Distribuyendo vector.", id);

	MPI_Barrier(MPI_COMM_WORLD);
//Difundir valor de vector 'a' todos los procesos
	MPI_Bcast(*direccion de a*, *cantidad de elementos*, MPI_INT, *origen*, MPI_COMM_WORLD);	

	despl("Distribuyendo Matriz", id);
	MPI_Barrier(MPI_COMM_WORLD);
//Distribuir filas de matriz 'm' en submatriz local 'B'
	MPI_Scatter(*matriz m*, *cantidad de elementos*, MPI_INT, *submatriz B*, *cantidad de elementos*, MPI_INT, *origen*, MPI_COMM_WORLD);

/* Calcular producto vector submatriz */
	despl("Calcular producto en paralelo...", id);

//Hacer: Determinar longitud del arreglo 'y_local' para el resultado del producto parcial.
	int tam_yLocal = **;
	int * y_local = (int *) malloc(tam_yLocal*sizeof(int));

//Hacer: calcular el producto vector*subMatriz y guardar en y_local
	prodM(*direccion de vector*, *direccion de subMatriz*, *dimension N (submatriz)*, *dimension M*, *direccion de vector resultado*);
	despl("Producto finalizado.\n", id);

/* Construir el vector resultado, reutilizando el vector 'a' */
	int * y = a;
//Hacer: Construir 'y' a partir de los 'y_local', guardando todo en proceso 0
	MPI_Gather(*direccion y_local*, *cantidad de elementos*, MPI_INT, *direccion y*, 'cantidad de elementos', MPI_INT, *proceso que recibe resultados*, MPI_COMM_WORLD);

/* Mostrar el resultado y finalizar */
	MPI_Barrier(MPI_COMM_WORLD);
	double t2 = MPI_Wtime();

	if(id == 0){
		printf("El primer elemento de y es: %i",y[0]);
		printf("\nt = %3.4f\n", (t2-t1));
	}

	MPI_Finalize();

}

/*
   CUERPO DE LOS METODOS
*/

void despl(char * mensaje, int id){
	if(id == 0){
		printf(mensaje);
		printf("\n");
	}
}

void inicializarV(int * v, long l){
//Hacer: inicializar VECTOR
}

void inicializarM(int * M, long n, long m){
//Hacer: inicializar Matriz como arreglo lineal
}


void prodM(int * V, int * M, long n, long m, int * y){
//Hacer: calcular producto vector matriz, donde la matriz es un arreglo lineal.
//La matriz es nxm y el arreglo m.
//El resultado queda en el vector 'y'.
}
