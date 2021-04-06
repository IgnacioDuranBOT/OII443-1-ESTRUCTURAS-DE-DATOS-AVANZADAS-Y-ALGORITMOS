
#include<stdlib.h>
#include<sstream>
#include<stdio.h>
#include<fstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <list>
#include <stack>

#define CASOS 10
#define PESOMAX 15

using namespace std;

//-------------------------------------------------------
class Action{
	//id del objeto a insertar 
	public:
		int id;
};

class State{
	//estado
	public:
		list <int> objetos;
		int cargaAcutal;
};

struct objeto{
	int id;
	int peso;
	int valor;
};
//-------------------funciones------------------------------------

void agregarPeso(State &s, int peso){
	s.cargaAcutal += peso;
}
void quitarPeso(State &s, int peso){
	s.cargaAcutal -= peso;
}
void cambiarObjetos(State &s,list<int>::iterator &it,objeto o ){
	s.objetos.insert(it,1,o.id);
}

void agregarObjeto(State &s, objeto o){
	s.objetos.push_front(o.id);
	agregarPeso(s,o.peso);
}

void quitarObjeto(State &s, objeto o){
	s.objetos.pop_back();
	quitarPeso(s,o.peso);
}

float promedioMochila(State s, objeto o[]){
	int valor = 0;
	float prom = 0 ;
	for(int a : s.objetos){
		valor = valor + o[a].valor;
	}
	prom = valor/s.cargaAcutal;
	return prom;
}

float promedioObjeto(objeto o){
	float prom = o.valor / o.peso;
	return prom;
}
int estaEnMochila(State s,objeto o){
	 for(int a : s.objetos){
		 if(a == o.id)return 1;
	 }
	 return 0;
}

void llenador(State &s, objeto n[]){
	int x=0;
	if(s.objetos.empty()){
		for(int i = 0 ; i < CASOS; i++){
			if(s.cargaAcutal < PESOMAX ){
				if((s.cargaAcutal + n[i].peso) <= PESOMAX){
					agregarObjeto(s,n[i]);
					x= 1;
				}
			}
		}
	}else{
		for(int i = 0 ; i < CASOS; i++){
			if(estaEnMochila(s,n[i]) == 0 && (s.cargaAcutal + n[i].peso) < PESOMAX ){
				agregarObjeto(s,n[i]);
				x= 1;
			}
		}

	}
	if(x==1){
		cout<<"SE LLENO LA MOCHILA"<<endl;
	}else{
		cout<<"NO SE PUDO LLENAR LA MOCHILA"<<endl;
	}
}

objeto buscador(int id, objeto o[]){
	for(int i = 0 ; i < CASOS ; i++){
		if(o[i].id == id) return o[i];
	} 
}

int is_final_state(State &s, int a ){
	
	if(s.cargaAcutal != 0 && s.cargaAcutal <= PESOMAX && a == 1) return 1;
	return 0 ;
}
int is_valid_state(State &s){
	
	if(s.cargaAcutal > PESOMAX || s.cargaAcutal == 0) return 0;
	return 1;
}

State transition(State &s, objeto o){
    s.objetos.push_front(o.id);
    agregarPeso(s,o.peso);
    return s;
}

list<Action> get_actions(State &s,objeto objetos[], int &fl){
	list<Action> acciones;
	State estado= s;
	Action accion;
	int i=0;
	
	llenador(estado,objetos);// llenamos la mochila
	if(is_valid_state(estado)==0){
		return acciones;
	}

	if(!estado.objetos.empty()){	
		int copyO;
		State nEstado,copia;
		
			for(int n=0; n<CASOS; n++){
				if(promedioMochila(estado,objetos) <= promedioObjeto(objetos[n]) && objetos[n].peso <= PESOMAX){
					estado.cargaAcutal=0;
					estado.objetos.clear();
					agregarObjeto(estado,objetos[n]);
					if(objetos[n].peso < PESOMAX) llenador(s,objetos);
				}
			}
			copia = estado;
			for(int x= 0 ; x < CASOS ; x++){	
				if(estaEnMochila(copia,objetos[x])==0 ){
					if(objetos[copyO].peso >= objetos[x].peso && promedioObjeto(objetos[copyO]) < promedioObjeto( objetos[x]) ){
						agregarObjeto(nEstado,objetos[x]);
						quitarObjeto(copia,objetos[copyO]);
						agregarObjeto(copia,objetos[x]);
						if(copia.cargaAcutal < PESOMAX) llenador(copia,objetos);
						copyO = copia.objetos.back();
						
					}else{
						agregarObjeto(nEstado,objetos[copyO]);
						copyO = copia.objetos.back();
					}
				}
			}
			estado = nEstado;
			if(estado.cargaAcutal < PESOMAX) llenador(estado,objetos);
		
			fl = 1;
			if(is_valid_state(estado) == 1){
				for(int a : estado.objetos){
					
					accion.id = a;
					acciones.push_front(accion);
				}
			}
		
	}	
	return acciones;
}

State buscarSol(State & inicial, objeto o[]){
	int b = 0; // flag, indica si todos los elementos del arreglo fueron recorridos
	stack<State> S;
  	S.push(inicial);
  	
  	
  	while(!S.empty()){
    	State s= S.top(); S.pop();
    	
    	if(is_final_state(s,b) == 1){
			return s;
		}
    	list<Action> actions = get_actions(s,o,b);
    	
     	for( Action a : actions){
        	transition(s,buscador(a.id,o));
     	}
		 S.push(s);
     	
    }
	State s;
	return s;
}

void lectura(objeto[]);

//----------------------main---------------------------------

int main(int argc, char *argv[]){
	
	objeto cObjetos[CASOS];
	lectura(cObjetos);
	int valorTotal=0;

	/* muestra lo objetos 
	for(int j = 0; j < CASOS; j++){
		cout<<"----------------- "<<endl;
		cout<<cObjetos[j].id<<endl;
		cout<<cObjetos[j].peso<<endl;
		cout<<cObjetos[j].valor<<endl;
	};
	*/
	
	State inicial; // mochila vacia, como primer caso
	inicial.cargaAcutal=0;
	State i = buscarSol(inicial,cObjetos);
	if(i.objetos.empty()){
		cout<<"NO SE ENCONTRO UN ESTADO POSIBLE"<<endl;
	}else{
		cout<<"SE ENCONTRO UN ESTADO POSIBLE"<<endl;
			cout<<"---------ESTADO---------"<<endl;
			for(int a : i.objetos){
				cout<<"---------OBJETO---------"<<endl;
				cout<<"ID :"<<a<<endl;
				cout<<"PESO : "<<cObjetos[a].peso<<endl;
				cout<<"VALOR : "<<cObjetos[a].valor<<endl;
				valorTotal = valorTotal + cObjetos[a].valor;
			}
			cout<<"------------------"<<endl;
			cout<<"CARGA DE LA MOCHILA : "<<i.cargaAcutal<<endl;
			cout<<"VALOR TOTAL : "<<valorTotal<<endl;
			
	}
	system("PAUSE");
    return EXIT_SUCCESS;
}
//-------------------------------------------------------
void lectura(objeto objetos[]){ //lectura de archivos
	ifstream archivo;
	string texto,num;
	int i=0, z=0;
	archivo.open("instacias 10.txt",ios::in);
	if(archivo.fail()){
		cout<<"no se pudo abrir";
		exit(1);
	}
	
	while(getline(archivo,texto)){
		stringstream stream(texto);
		if(z == 0){	
			while(getline(stream, num, ',')){
				
				objetos[i].id = i;
				objetos[i].peso = stoi(num);
				i++;
			}
			z= 1;
		}else{
			i = 0;
			while(getline(stream, num, ',')){
				objetos[i].valor = stoi(num);
				i++;
			}	
		}	
	}
	archivo.close();
}

