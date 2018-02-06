#include "stddef.h"
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;
#ifndef __MONTICULOFIBONACCI_H
#define __MONTICULOFIBONACCI_H


template <class Valor>
class MonticuloFib {
public:

	enum parientes {hDrc,hIzq,pdr,des};

private:

	/*Subclase interna que representa la unidad minima del monticulo.*/
	class Nodo {
	private:

		/*Atributos de la clase:*/

		 /*Informacion contenida dentro del nodo.*/
		 Valor _valor;
		 /*Puntero al nodo que esta "situado" a su izquierda.*/
		 Nodo *_hermanoIzq;
		 /*Puntero al nodo que esta "situado" a su derecha.*/
		 Nodo *_hermanoDrc;
		 /*Puntero a uno de sus descendientes.*/
		 Nodo *_descendientes;
		 /*Puntero a su padre.*/
		 Nodo *_padre;
		 /*Booleano que indica si ese nodo ha perdido un hijo en alguna de las anteriores operaciones ejecutadas sobre el monticulo.*/
		 bool _marca;
		 /*Entero que nos indica que altura tiene el sub-Arbol apartir de ese Nodo,sin contabilizar este.*/
		 int _grado;

	public:

		/* Constructores. */

		/*Contructor Basico,sirve para todos los nodos.*/
		Nodo(const Valor &valor,Nodo *hermanoIzq,Nodo *hermanoDrc,Nodo *descendientes,Nodo *padre,bool marca,int  grado) :
		 _valor(valor),_hermanoIzq(hermanoIzq),_hermanoDrc(hermanoDrc),_descendientes(descendientes),_padre(padre),_marca(marca),_grado(grado){};

		/*Constructor Especial,sirve solo para los nodos que estan en la espina central del Monticulo.*/
		Nodo(const Valor &valor,Nodo *hermanoIzq,Nodo *hermanoDrc,int grado):
		 _valor(valor),_hermanoIzq(hermanoIzq),_hermanoDrc(hermanoDrc),_descendientes(NULL),_padre(NULL),_marca(false),_grado(grado) {};

		/*Getters*/

		/*Operacion observadora que duelve el valor contenido en un Nodo.*/
		Valor getValor(){
			return _valor;
		}

		/*Operacion observadora que duelve el puntero del pariente que desees de un nodo especifico.*/
		Nodo* getPariente(parientes pariente){
			switch(pariente){
				case hDrc: return _hermanoDrc;
				case hIzq: return _hermanoIzq;
				case pdr: return _padre;
				case des: return _descendientes;
				default: return NULL;
			}
		}

		/*Operacion observadora que nos indica si un nodo esta marcado o no.
		 *	Esta operacion tiene especial interes a la hora de realizar un decrecimiento de un Nodo.
		 *	Mas adelante será explicado con mayor detalle.
		*/
		bool getMarca(){
			return _marca;
		}

		/*Operacion observadora que nos informa del grado.*/
		int getGrado(){
			return _grado;
		}

		/*Setters*/

		/*Operacion modificadora que asigna un nuevo valor al campo _valor dentro de un Nodo.*/
		void setValor(Valor a){
			_valor = a;
		}

		/*Operacion modificadora,cambia una "relacion familiar" de un Nodo.*/
		void setPariente(parientes pariente,Nodo* nuevoValor){
			switch(pariente){
				case hDrc: _hermanoDrc = nuevoValor; break;
				case hIzq: _hermanoIzq = nuevoValor; break;
				case pdr: _padre = nuevoValor; break;
				case des: _descendientes = nuevoValor; break;
				default: break;
			}
		}

		/*Operacion modificadora,cambia el grado de un Nodo. */
		void setGrado(int g){
			_grado = g;
		}

		/*Operacion modificadora,cambia la marca de un Nodo.*/
		void setMarca(bool marca){
			_marca = marca;
		}

		/* Aclaraciones:
		 *
		 * 1º Si nos fijamos,en la implementacion de Nodo,podriamos creeer que en realidad todos los nodos deben tener padre,
		 * descendientes y hermanos obligatoriamente,nada mas lejos de la realidad.Para solucionar este pequeño contratiempo,
		 * consideraremos que si el nodo no posee descendientes,el puntero _descendientes apuntara al nodo que lo contiene,del mismo modo
		 * haremos con _hermanos y con padre.La razon de tomar esta decision de diseño esta fomentada por dos motivaciones,la primera es
		 * que el monticulo cumpla con sus costes y funcionalidad,y la segunda razon es que la volatilidad de esta sea lo mas pequeña posible.
		 * Si hubieramos decidido,por ejemplo,haber hecho que estos punteros valieran NULL,
		 * la volatilidad del TAD hubiera aumentado dando lugar a una mayor probabilidad de que se produjera un fallo de ejecucion relativo a la
		 * memoria.
		 *
		 * 2º El puntero descendientes puede ser un poco confuso,si tengo acceso a los descendientes de un nodo,¿Porque esta variable es un solo
		 * puntero a un Nodo y no un puntero de punteros?.La respuesta es sencilla,tienes acceso a ellos pero atraves de un hijo del nodo,
		 * en concreto,el primer hijo que tubo ese nodo en cuestion.
		 * De tal modo que un nodo puede tener muchos descendientes,pero no queda reflejado en la informacion del nodo de manera explicita.
		 */
	};

	/*Subclase interna que nos ayuda a la hora de inspeccionar el monticulo de una manera mas sencilla,eficiente y clarificadora.*/
	class Iterador {

	private:

		Nodo* _act;

	public:

		/* Constructores. */

		/*	Constructor que dado un nodo en concreto,comienza la iteracion a traves de el.*/
		Iterador(Nodo *nodo)
				:_act(nodo) { }

		/*	Operacion modificadora que "avanza" el puntero al siguiente nodo.
		 *  En nuestro caso consideramos que el siguiente nodo a revisar es el hermano derecho del actual.
		 */
		void next() {
			_act = _act->getPariente(hDrc);
		}

		/*Sobrecarga de operadores.Para hacer del manejo de los punteros algo mas dinamico y sencillo*/
		bool operator==(const Iterador &other) const {
			return _act == other._act;
		}

		bool operator!=(const Iterador &other) const {
			return !(this->operator==(other));
		}

		/*Operacion observadora que nos indica en que nodo estamos actualmente,de entre todos los que estamos recorriendo.*/
		Nodo* get_act(){
			return _act;
		}

		/*Operacion modificadora,asigna un nuevo actual*/
		void set_act(Nodo* nuevo){
			_act = nuevo;
		}

	};

	/*Devuelve el iterador al principio del monticulo,en nuestro caso,consideramos que el primer elemento sera el minimo.
	 *	 Al ser una lista circular,el principio coincide con el final,por tanto no es necesario implementar una funcion end().
	 *	 @return iterador al principio de la lista;
	 */
	Iterador begin() {
		return Iterador(_min);
	}

	/*Devuelve el iterador apuntando al primer descendiente de un nodo recibido.
	 *  Pasa igual que con la funcion anteriro,al ser una lista circular,el principio coincidira con el final.*/
	Iterador descendientesBegin(Nodo *nodo) {
		return Iterador(nodo->getPariente(des));
	}

	/*Atributos de la clase: */

	/*Puntero al nodo cuyo valor es el mas pequeño de todos los del monticulo.*/
	Nodo *_min;
	/*Entero que nos dice cuantos nodos hay dentro del monticulo.*/
	int _numNodos;

	/*Operacion modificadora.Esta operacion complementa a la funcion borraMin().
	 * El objetivo de esta funcion es fundir los nodos creando nuevas relaciones familiares entre ellos
	 * con el objetivo de reducir el coste de futuras llamadas a la funcion borraMin().
	 * En esta compactacion que se realiza,se garantiza que los nuevos nodos que compongan la espina principal
	 * del monticulo tengan un grado unico de modo que que tras realizar esta operacion,haya como maximo
	 * 2*log(_numNodos) en la espina principal.
	 * Esto reduce enormemente el coste en las futuras llamadas a la funcion de borrarMin() y la posterior
	 * localizacion del nuevo nodo que sera el min del monticulo.
	 */
	void compactar(){
		MonticuloFib<int>::Iterador it = begin();
		vector<Nodo*> grados(2*log(_numNodos));
		Nodo *aux;
		Nodo *aux2;
		Nodo *volcador;
		Nodo *siguiente;
		int i = 0;

		/*Recorremos la espina principal del monticulo y vamos colgando los nodos,en funcion del valor que tuvieran.*/
		do{

			if(!tiene(it.get_act()->getPariente(pdr),it.get_act())){
				it.next();
			}else{
				it = siguiente;
			}
			siguiente = NULL;
			aux = it.get_act();
			i = aux->getGrado();

			while(grados[i] != NULL){
				aux2 = grados[i];
				if(aux->getValor() > aux2->getValor()){
					volcador = aux;
					aux = aux2;
					aux2 = volcador;
				}

				if(siguiente == NULL)
					siguiente = it.get_act()->getPariente(hDrc);

				colgar(aux2,aux);
				grados[i] = NULL;
				i++;
			}
			grados[i] = aux;

		}while(it != begin());

		_min = NULL;

		/*Una vez compactados de manera local,procedemos a unirlos entre si formando ya el futuro monticulo.
		 * En esta fase del borrado del minimo,tambien se busca cual sera el nuevo minimo.
		 */
		for(unsigned int b = 0; b < grados.size();b++){
			if(grados[b] != NULL){
				superInsertar(grados[b]);
				if(_min == NULL || grados[b]->getValor() < _min->getValor()){
					_min = grados[b];
				}
			}
		}
	}

	/*Operacion modificadora.Dados dos nodos cuelga el nodo y del x.*/
	void colgar(Nodo *y,Nodo *x){
		//Desenlazamos el nodo de donde estaba.
		desenlaza(y);
		//Colgamos el hijo(si tenia hijos el nodo x,hay que hacer arreglos).
		if(!tiene(x->getPariente(des),x)){
			x->setPariente(des,y);
			y->setPariente(hDrc,y);
			y->setPariente(hIzq,y);
		}else{
			y->setPariente(hIzq,x->getPariente(des)->getPariente(hIzq));
			y->setPariente(hDrc,x->getPariente(des));
			x->getPariente(des)->getPariente(hIzq)->setPariente(hDrc,y);
			x->getPariente(des)->setPariente(hIzq,y);
		}
		//Por ultimo hacemos unos arreglos(x ahora es padre de y y ademas hay que actualizar el grado de x).
		if(x->getGrado() <= y->getGrado()){
			x->setGrado(y->getGrado()+1);
		}

		y->setPariente(pdr,x);

		//desmarcamos el nodo
		y->setMarca(false);
	}

	/* Operacion auxiliar modificadora.Dado un nodo lo "extrae" de donde estubiera.
	 *	Elimina todas las relaciones de sus antiguos hermanos.
	 */
	void desenlaza(Nodo *nodo){
		nodo->getPariente(hIzq)->setPariente(hDrc,nodo->getPariente(hDrc));
		nodo->getPariente(hDrc)->setPariente(hIzq,nodo->getPariente(hIzq));
	}

	/*Operacion modificadora.Dado un padre y un hijo elimina la relacion entre ambos e inserta el nodoHijo en la espina central del monticulo.*/
	void cortar(Nodo *nodoHijo,Nodo *nodoPadre){

		//Si tenia mas de un descendiente
		if(tiene(nodoHijo->getPariente(hIzq),nodoHijo)){
			nodoPadre->setPariente(des,nodoHijo->getPariente(hIzq));
			desenlaza(nodoHijo);
		}else{
			nodoPadre->setPariente(des,nodoPadre);
		}
		superInsertar(nodoHijo);
		nodoHijo->setPariente(pdr,nodoHijo);
		nodoHijo->setMarca(false);
		nodoPadre->setGrado(nodoPadre->getGrado()-1);
	}

	/*Operacion modificadora.Analiza si el padre del nodo en cuestion estaba marcado.De ser asi debemos seguir podando el monticulo.
	 *	Si no tenia marca previa,simplemente lo marcamos.
	 *	Si ya tenia,se llama otra vez a cortar para cotar ese nodo y posibles hijos susyos.
	 */
	void cortarEnCascada(Nodo *padreNn){

		if(!padreNn->getPariente(pdr)->getMarca()){
			padreNn->getPariente(pdr)->setMarca(true);
		}else{
			cortar(padreNn,padreNn->getPariente(pdr));
		}
	}

	/*Operacion destructora.Recorre una espina concreta y va liberando los nodos de esta.*/
	void libera(Iterador it){
		do{
			/*Si el nodo por el que vamos tiene descendientes los liberamos gracias a la funcion liberaDescendientes.*/
			if(tiene(it.get_act()->getPariente(des),it.get_act()) && (it.get_act() == it.get_act()->getPariente(des)->getPariente(pdr))){

				liberaDescendientes(descendientesBegin(it.get_act()));
				//Al no poder comprobar de manera explicita si el nodo ha sido liberado,sabemos que si tiene un grado negativo es que fue liberado.
				//Es una medida que he tomado un poco a regañadientes por falta de opciones,que yo conozca y no haya encontrado,para solucionarlo.
				if(it.get_act()->getGrado() < 0){
					return;
				}
			}
			Nodo *anterior;
			anterior = it.get_act();
			//Si el siguiente Nodo es el mismo,tenemos que subir hacia el padre(tambien si ya hemos pasado por el hermDrc de este nodo).
			if(tiene(it.get_act()->getPariente(hDrc),it.get_act()) && (it.get_act() == it.get_act()->getPariente(hDrc)->getPariente(hIzq))){
				it.next();
			}else{
				if(tiene(it.get_act()->getPariente(pdr),it.get_act())){
					it.set_act(it.get_act()->getPariente(pdr)->getPariente(hDrc));
				}else{
					//Si no,simplemente si no es el minimo,avanzamos para eliminar el siguiente.
					if(it.get_act() != _min){
						it.next();
					}
				}
			}
			/*Una vez terminado todo,finalmente liberamos el nodo en cuestion*/
			free(anterior);
		}while(it != begin());
	}

	/*Operacion destructora que dado un nodo destruye sus descendientes.*/
	void liberaDescendientes(Iterador it){

		/*Si ese nodo descendiente,a su vez tiene mas descendientes,debemos eliminar toda esa sub-espina de hermanos*/
		if(tiene(it.get_act()->getPariente(des),it.get_act())){
			liberaDescendientes(descendientesBegin(it.get_act()->getPariente(des)));
		}
		/*Mientras este descendiente tenga hermanos,debemos seguir liberando*/
		if(tiene(it.get_act()->getPariente(hDrc)->getPariente(des),it.get_act()->getPariente(hDrc))){
			libera(descendientesBegin(it.get_act()->getPariente(hDrc)));
			if(it.get_act()->getGrado() < 0){
				return;
			}
		}
		/*Una vez hemos terminado,liberamos el nodo.*/
		free(it.get_act());
	}

	/*Operacion observadora auxiliar que nos indica si ese nodo posee una relacion familiar con el nodo parentesco.
	* En caso de que no posea ningun parentesco del tipo que preguntemos,ese el puntero a ese nodo coincidira
	* con el parentesco por el cual preguntamos.
	* Si la resta entre ambos da distinta a cero,significa que son dos direcciones de memora distintas y que por lo tanto
	* tiene un pariente del tipo que preguntamos.
	*/
	bool tiene(Nodo *parentesco,Nodo *nodo){
		return  (parentesco - nodo);
	}

	/*Operación generadora que añade un nuevo Nodo al monticulo.
	 * Esta operacion se usa a la hora de realizar un decremento de un valor.Realmente es igual que la operacion de insertar
	 * que el usuario puede realizar,pero esta inserta un nodo que ya existia y el insertar del usuario añade un nodo completamente
	 * nuevo.
	*/
	void superInsertar(Nodo *nodo){


		//al insertar en la espina central,el nodo pasa a no tener padre,o a ser padre de si mismo, que es lo mismo.
		nodo->setPariente(pdr,nodo);
		if(_min != NULL){
			//arreglamos las conexiones del nodo
			nodo->setPariente(hDrc,_min);
			nodo->setPariente(hIzq,_min->getPariente(hIzq));
			//arreglamos la conexion con el minimo y su antiguo hermano izquierdo
			_min->getPariente(hIzq)->setPariente(hDrc,nodo);
			_min->setPariente(hIzq,nodo);
		}else{
			nodo->setPariente(hDrc,nodo);
			nodo->setPariente(hIzq,nodo);
			_min = nodo;
		}

	}


public:

	/*Constructor,construye un monticulo vacio*/
	MonticuloFib(): _min(NULL),_numNodos(0){}

	/*Destructor; libera todos los nodos */
	~MonticuloFib() {

		/*Incia un desencadenamiento de llamadas que provocaran la destruccion de todos los nodos.*/
		MonticuloFib<int>::Iterador it = begin();
		if(it != NULL){
			libera(it);
		}
		_numNodos = 0;
	}

	/*Operación generadora que añade un nuevo Nodo al monticulo.
	 *	 Siempre insertaremos a la izquierda de _min.
	 *	  Si el valor es menor que el valor del Nodo _min,debemos sustituirlo.
	 *
	 */
	void insertar(const Valor &valor) {

		//es un monticulo vacio?
		if(_numNodos != 0){
			//creamos el nuevo nodo
			Nodo *nuevoNodo = new Nodo(valor,_min->getPariente(hIzq),_min,0);
			nuevoNodo->setPariente(des,nuevoNodo);
			nuevoNodo->setPariente(pdr,nuevoNodo);

			//arreglamos por la izquierda
			_min->getPariente(hIzq)->setPariente(hDrc,nuevoNodo);

			//arreglamos las conexiones por la derecha del nuevoNodo
			_min->setPariente(hIzq,nuevoNodo);

			//si el min es mejorado por el nuevoNodo,lo sustituimos
			if(valor < _min->getValor()){
				_min = nuevoNodo;
			}
		}else{
			//si era vacio,debemos darle una nueva instancia al _min;
			_min = new Nodo(valor,NULL,NULL,0);
			_min->setPariente(des,_min);
			_min->setPariente(hDrc,_min);
			_min->setPariente(hIzq,_min);
			_min->setPariente(pdr,_min);
		}
		_numNodos++;
	}

	/*Operación observadora que nos muestra si el monticulo esta vacio.*/
	bool vacio() const {
		return _numNodos == 0;
	}

	/*Operacion destructora que elimina el Nodo minimo del Monticulo*/
	void borraMin(){

		if(_min != NULL){

			if(tiene(_min->getPariente(des),_min)){

				//metemos a los descendientes del _min en la espina central
				//para ello,debemos encontrar los bordes de la sub-espina de los descendientes de _min
				MonticuloFib<int>::Iterador it = descendientesBegin(_min);
				Nodo *bordeIzq;
				Nodo *bordeDrc;
				bool encontrado = false;
				do{
					it.get_act()->setPariente(pdr,it.get_act());

					if(it.get_act()->getPariente(hDrc) == _min->getPariente(des)){
						bordeIzq = it.get_act();
						bordeDrc = bordeIzq->getPariente(hIzq);
						encontrado = true;
					}
					it.next();
				}while(it != descendientesBegin(_min) && !encontrado);

				//Una vez encontrados dichos bordes,procedemos a insertarlo en funcion de si el _min tenia hermanos o era hijo unico.

				//Arreglamos por la izquierda.
				if(tiene(_min->getPariente(hIzq),_min)){
					bordeIzq->setPariente(hIzq,_min->getPariente(hIzq));
					_min->getPariente(hIzq)->setPariente(hDrc,bordeIzq);
				}else{
					bordeIzq->setPariente(hIzq,_min);
					_min->setPariente(hDrc,bordeIzq);
				}
				//Arreglamos por la derecha.
				bordeDrc->setPariente(hDrc,_min);
				_min->setPariente(hIzq,bordeDrc);

			}
			//Eliminamos el min.
			desenlaza(_min);
			//Si al eliminar no quedan mas nodos,implica que el monticulo esta vacio.
			if(!tiene(_min->getPariente(hDrc),_min)){
				_min = NULL;
			}else{
				/*Si despues de eliminar el min le quedan nodos,debemos reasignar el min,no siempre se hara bien.
				 *	La funcion compactar se encargara de que el min sea el correcto despues de hacer unos arreglos.
				 *	En la implementacion a esta funcion se explicara con mas detalle,pero la idea de compactar es que
				 *	reestructura el monticulo de tal manera que junta nodos creando realciones familiares entre ellos para asi
				 *	cuando realize futuras operaciones borraMin() el coste de la funcion sea mucho menor.
				 */
				_min = _min->getPariente(hDrc);
				if(tiene(_min->getPariente(hIzq),_min)){
					compactar();
				}
			}
			_numNodos--;
		}
	}

	/*Operacion modificadora que cambia el valor de un nodo,decreciendolo.*/
	void decrecerValor(Nodo *nodo,int nuevoValor){
		//si el nuevoValor no decrece el valor antiguo,terminamos ya que no tiene ningun sentido
		if(nuevoValor > nodo->getValor()){
			return;
		}
		nodo->setValor(nuevoValor);
		/*Si el nodo tiene padre,significa que no estaba en la espina central.
		 * Si ese nodo no estaba en la espina principal significa que puede que tengamos que realizar mas cambios.
		 * Por ello,si el nodo que decrecimos y que ya sabemos que no esta en la espina princpial tiene una mala
		 * relacion con su padre,tiene un valor menor que su padre,debemos arreglarlo.*/
		if(tiene(nodo->getPariente(pdr),nodo) && nodo->getValor() < nodo->getPariente(pdr)->getValor()){
			Nodo *auxP;
			auxP = nodo->getPariente(pdr);
			cortar(nodo,nodo->getPariente(pdr));
			cortarEnCascada(auxP);
		}
		/*Finalmente observamos si hay que sustituir el min.*/
		if(nodo->getValor() < _min->getValor()){
			_min = nodo;
		}
	}

	/*Operacion auxiliar.Nos devuelve un nodo de manera pseudo-aleatoria a partir del numero de avances que reciba.
	 * En cada avance que realice,tiene una probabilidad del 30% de que en vez de seguir en la espina que estaba,
	 * descienda a uno de los descendientes de ese nodo y baje a la espina inferiror.*/
	Nodo* nodoAleatorio(int numAvances){

		MonticuloFib<int>::Iterador it = begin();
		int ran;

		while(numAvances > 0){
			it.next();
			ran = rand() % 10;
			/*En cada uno de los avances,que realizamos,tenemos un 40% de seguir avanzando por los descendientes de el nodo actual.*/
			if(ran < 3){
				it = descendientesBegin(it.get_act());
			}
			numAvances--;
		}
		return it.get_act();
	}

	 /*Operacion auxiliar que lanza una bateria de pruebas sobre el monticulo.
	 * Este subprograma tiene como finalidad ayudar al codificador a hacer pruebas del tamaño que desee para depurar
	 * el codigo del monticulo.
	 * La idea es ir mostrado lo que se hace en cada una de las operacion e ir mirando el estado del monticulo,en modo de depuracion,
	 * para ir comprobando que la operacion se ha hecho correctamente.
	 * La bateria intenta ser lo mas aleatoria posible para que surjan el mayor numero de casos posibles y asi hacer mas efectiva la
	 * busqueda de posibles errores en el codigo.
	 */
	void bateriaAleatoria(int numPruebas){
		srand(numPruebas); //semilla cambiada
		int opRand;
		int numRand;
		int numAvances;
		Nodo *aux;
		int i = 0;
		cout << "Numero de Prubas: " << numPruebas << endl;
		while(i < numPruebas){
			opRand = rand() % 3;
			cout << "Prueba " << i << " ->";
			switch(opRand){
				case 0:	numRand = rand()%100 ;
						cout << "Inserta: " <<numRand << endl;
						insertar(numRand);
						break;
				case 1:	if(!vacio()){
							cout << "Borra el Min: " << _min->_valor << endl;
							borraMin();
						}else{
							numRand = rand()%100;
							cout << "Inserta: " << numRand << " y ";
							insertar(numRand);
							cout << "Borra el Min: " << _min->_valor << endl;
							borraMin();
						}
						break;
				case 2:

					if(_numNodos > 1){
						cout << "Decrecer nodo : ";
						numAvances = (rand() + 1) % _numNodos;
						aux = nodoAleatorio(numAvances);
						numRand = (rand() + 1) % 21;
						cout << aux->_valor << " " << numRand << endl;
						decrecerValor(aux,numRand);
					}else if(_numNodos == 1){
						decrecerValor(_min,numRand);
					}else{
						cout << "Inserta: " << numRand << " y Decrecer nodo";
						insertar(numRand);
						decrecerValor(_min,numRand);
					}

			}
			i++;
		}

	}

};

#endif // __MONTICULOFIBONACCI_H
