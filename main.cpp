#include "MonticuloFibonacci.h"
#include <fstream>
#include <time.h>

int main(){


	/*Procedemos a crear los archivos donde quedaran registrados los tiempo que han tardado cada operacion en realizarse n veces*/
	MonticuloFib<int> mont;
	ofstream insertar("insertar.dat");
	ofstream decrecer("decrecer.dat");
	ofstream borrar("borrar.dat");

	int numPruebas = 10000;
	int totalIteraciones = 0;
	clock_t t_ini,t_fin;
	double segundos;
	/*Llamamos tantas veces como deseemos.
	 *	Hay que tener en cuenta que cada vuelta a este buble implica que hace 3 iteraciones cada una de ellas hasta i,
	 *	de modo que cada vuelta que damos debemos de multiplicarla por 3i.
	 *	Quedando como Sumatorio desde i = 1 hasta numPruebas de 3*(i)
	 */
	for(int i = 1;i < numPruebas;i += 1){


		totalIteraciones += i;
		//inserta() i veces
		insertar << i << " ";
		/*Empieza a correr el cronometro*/
		t_ini = clock();
		for(int a = 0; a < i;a++){
			mont.insertar(a);
		}
		/*Lo detenemos y comprobamos la diferencia de tiempos para saber cuanto ha tardado en realizar estas i operaciones.*/
		t_fin = clock();
		segundos = (double)(t_fin - t_ini)/ CLOCKS_PER_SEC;
		insertar << segundos*1000.0 << endl;

		/**************/
		totalIteraciones += i;
		decrecer << i << " ";
		t_ini = clock();
		//decreceNodo() i veces
		for(int b = 0; b < i;b++){
			mont.decrecerValor(mont.nodoAleatorio(2),-b);
		}
		t_fin = clock();
		segundos = (double)(t_fin - t_ini)/ CLOCKS_PER_SEC;
		decrecer << segundos*1000.0 <<endl;

		/************/
		totalIteraciones += i;
		borrar << i << " ";
		t_ini = clock();
		//borramin() i veces
		for(int c = 0; c < i;c++){
			mont.borraMin();
		}
		t_fin = clock();
		segundos = (double)(t_fin - t_ini)/ CLOCKS_PER_SEC;
		borrar << segundos*1000.0 << endl;
		cout  <<  i << endl;//<-Indice para mostrarnos por que iteracion va,tambien es un medidor de la velocidad a la que esta procesando
		//la informacion.Para un numero alto de iteraciones este numero puede ser bastate revelador y util.

		/*No hace falta llamar al destructor ya que el monticulo estara vacio.*/
	}
	cout << "Iteraciones en total: " << totalIteraciones << endl;
	insertar.close();
	decrecer.close();
	borrar.close();
	/*No hace falta llamar al destructor ya que el monticulo estara vacio.
	 *	Aun asi,c++ llamara al destructori despues de ejecutar la instruccion return
	*/
	return 0;
}




