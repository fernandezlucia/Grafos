#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 

using namespace std;

#define BLANCO 0
#define GRIS 1
#define NEGRO 2
long long int n, m;

//recorre todos las adyacencias de todos los nodos
void DFS(vector<vector<int>> &lista_de_ady, int u, int tiempo_actual, vector<int> &visitado, vector<int> &padre, vector<int> &componentes,vector<int> &time_in) {
    visitado[u] = GRIS;
    time_in[u] = tiempo_actual++;
    componentes.back()++;
    if(u == 0)
        padre[u] = u;
    for(int v : lista_de_ady[u]){
        if (visitado[v] == BLANCO){
            padre[v] = u;
            DFS(lista_de_ady, v, tiempo_actual, visitado, padre, componentes, time_in);
        } 
    }
    visitado[u] = NEGRO;
}

//deteccion de puentes, recorre todos las adyacencias de todos los nodos
int detectarPuentes(vector<vector<int>> &lista_de_ady, int v, vector<int>& time_in, const vector<int>& padre, vector<bool>& puente_con_parent){
    int cantidad = 0;
    for(int u : lista_de_ady[v]){
        if(padre[u] == v )
            cantidad += detectarPuentes(lista_de_ady, u, time_in, padre, puente_con_parent);
        else {
            if(time_in[v]>time_in[u] && padre[v] != u)
                cantidad++;
            if(time_in[v]<time_in[u])
                cantidad--;
        }
    }
    if(cantidad == 0 && padre[v] != v){
        if(time_in[v] != 0)
            puente_con_parent[v] = true;
    }
    return cantidad;
}

void sacarPuentes(vector<int> &padre, vector<bool> &puente_con_padre, vector<vector<int>> &lista_de_ady){
    for(int hijo = 0; hijo < puente_con_padre.size(); hijo++){
        if(!puente_con_padre[hijo])
            continue;
        int parent = padre[hijo];
        //elimino la arista del padre
        for(int j = 0; j < lista_de_ady[parent].size(); j++){
            if(lista_de_ady[parent][j] == hijo)
                lista_de_ady[parent].erase(lista_de_ady[parent].begin() + j);
        }
        //elimino la arista del hijo
        for(int j = 0; j < lista_de_ady[hijo].size(); j++){
            if(lista_de_ady[hijo][j] == parent)
                lista_de_ady[hijo].erase(lista_de_ady[hijo].begin() + j);
        }
    }
}

//deteccion de componentes conexas, recorre todos los nodos
void detectarComponentes(vector<vector<int>> &lista_de_ady, vector<int> &visitado, vector<int> &padre, vector<int> &componentes, vector<int> &time_in){
    int tiempo_actual = 0;
    for (int v = 0; v < n; v++) {
        if (!visitado[v]) {
            componentes.push_back(0);
            DFS(lista_de_ady, v, tiempo_actual, visitado, padre, componentes, time_in);
        }  
    }
}

int main(){
    cin >> n >> m;

    vector<vector<int>> lista_de_ady(n);
    for(int i=0; i < m; ++i){
        int nodo1, nodo2;
        cin >> nodo1 >> nodo2;

        lista_de_ady[nodo1 -1].push_back(nodo2-1);
        lista_de_ady[nodo2 -1].push_back(nodo1-1);
    }

    vector<int> visitado(n, BLANCO);
    vector<int> padre(n, -1);
    vector<int> componentes(1,0);
    vector<int> time_in(n, 0);
    vector<bool> puente_con_parent(n, false);

    DFS(lista_de_ady, 0, 0, visitado, padre, componentes, time_in);
    int res = detectarPuentes(lista_de_ady, 0, time_in, padre, puente_con_parent);
    sacarPuentes(padre, puente_con_parent, lista_de_ady);
    visitado.assign(n, BLANCO);
    componentes.clear();
    detectarComponentes(lista_de_ady, visitado, padre, componentes, time_in);

    long long int formas_ganar = 0;
    for(int i = 0; i < componentes.size(); i++){
        int k = componentes[i];
        if(k == 1){
            continue;
        } else {
            formas_ganar += (k*(k-1))/2;
        }
    }
    
    long long int formas_jugar = (n*(n-1))/2;
    long double division = (formas_ganar*1.0) / formas_jugar;
    long double proba_perder = 1 - division;
    cout << fixed;
    cout << setprecision(5);
    cout << proba_perder << endl;

    return 0;
}