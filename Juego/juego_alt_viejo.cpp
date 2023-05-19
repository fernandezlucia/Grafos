#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>

using namespace std;

#define BLANCO 0
#define GRIS 1
#define NEGRO 2
int n, m;
// formas de perder: cantidad de jugadas de tuki q permiten a kitu ganar
// proba de perder = #formas de perder  / #formas de jugar ((n^2)-n)/2
// proba de perder = 1 - (#formas de ganar / #formas de jugar)
// si estan en un ciclo gana tuki


void imprimirListaVectores(const vector<vector<int>>& lista) {
    for (const auto& vec : lista) {
        for (const auto& elem : vec) {
            cout << elem +1<< " ";
        }
        cout << endl;
    }
}

void printVector(vector<int> v){
    for (const auto& elem : v) {
        cout << elem << " ";
    }
    cout << endl;
}

void printVectorBool(vector<bool> v){
    for (const auto& elem : v) {
        cout << elem << " ";
    }
    cout << endl;
}

void DFS(int u, vector<int> &visitado, vector<int> &padre, vector<vector<int>> &contar_ciclos, vector<vector<int>> &lista_de_ady,vector<int> &esta_en_ciclo) {
    visitado[u] = GRIS;
    for(int v : lista_de_ady[u]){
        if (visitado[v] == BLANCO) {
            padre[v] = u;
            DFS(v, visitado, padre, contar_ciclos, lista_de_ady, esta_en_ciclo);
        } 
        else if(visitado[v] == GRIS && padre[u] != v) { // es una backedge;
            esta_en_ciclo[v]++;
            int actual = u;
            contar_ciclos.push_back({v});
            while (actual != v) {
                contar_ciclos.back().push_back(actual);
                esta_en_ciclo[actual]++;
                actual = padre[actual];   //reinicio el ciclo con el padre del actual
            }
        }
    }
    visitado[u] = NEGRO;
}


//timer
void dfs_timer(const vector<vector<int>>& lista_de_ady, const int source, int current_time, vector<int>& time_in, vector<int>& color){
    color[source] = 1;
    time_in[source] = current_time++;
    for (int u : lista_de_ady[source]){
        if(color[u]==0){
            dfs_timer(lista_de_ady, u, current_time, time_in, color);
        }
    }
    color[source]=2;
}


//deteccion de puentes
int dp(const vector<vector<int>> &ady, int source, const vector<int>& time_in, const vector<int>& padre, vector<bool>& puente_con_parent){
    int cantidad = 0;
    //vector<vector<int>> puentes;
    for(int u:ady[source]){
        
        if(padre[u]==source){
            //vector<vector<int>> temp = dp(ady, u, time_in, padre, puente_con_parent);
            //for(vector<int> k : temp){
            //    puentes.push_back(k);

            cantidad += dp(ady, u, time_in, padre, puente_con_parent);
            //}
            //puentes.insert(puentes.end(), dp(ady, u, time_in, padre, puente_con_parent).begin(), dp(ady, u, time_in, padre, puente_con_parent).end());
        } else {
            if(time_in[source]>time_in[u] && padre[source] != u){
                cantidad++;
                //printVector(temp2);
                //cout << "entro"<< endl;
                //cout << "source es: " << source << ", u es: " << u << endl;
            }
            if(time_in[source]<time_in[u]){
                cantidad--;
                //puentes.erase(puentes.end()-1);
            }
        }
    }
    if(cantidad==0 && padre[source]!=source){
        puente_con_parent[source] = true;
    }

    return cantidad;
}

void sacarPuentes(vector<int> &padre, vector<bool> &puente_con_padre, vector<vector<int>> &lista_de_ady){
    for(int hijo = 0; hijo < puente_con_padre.size(); hijo++){
        if(!puente_con_padre[hijo])
            continue;
        
        int padre = padre[hijo];
        //elimino la arista del padre
        for(int j = 0; j < lista_de_ady[padre].size(); j++){
            if(lista_de_ady[padre][j] == hijo)
                lista_de_ady[padre].erase(j);
        }
        //elimino la arista del hijo
        for(int j = 0; j < lista_de_ady[hijo].size(); j++){
            if(lista_de_ady[hijo][j] == padre)
                lista_de_ady[hijo].erase(j);
        }
    }
}


vector<vector<int>> procesarEntrada(string test_in){
    ifstream entrada;
    entrada.open(test_in);
    entrada >> n >> m;

    vector<vector<int>> lista_de_ady(n);
    
    for(int i=0; i < m; ++i){
        int nodo1, nodo2;
        entrada >> nodo1 >> nodo2;

        lista_de_ady[nodo1 -1].push_back(nodo2-1);
        lista_de_ady[nodo2 -1].push_back(nodo1-1);
    }
    entrada.close();
    return lista_de_ady;
}

int main(int argc, char **argv){
    string test_in = argv[1];

    vector<vector<int>> lista_de_ady = procesarEntrada(test_in);
    vector<int> visitado(n, BLANCO);
    vector<int> padre(n, -1);
    vector<vector<int>> contar_ciclos(0);
    vector<int> esta_en_ciclo(n,0);

    DFS(lista_de_ady[0][0], visitado, padre, contar_ciclos, lista_de_ady, esta_en_ciclo);

    vector<bool> puente_con_parent(n, false);
    vector<int> time_in(n, 0);
    vector<int> color(n, BLANCO);
    dfs_timer(lista_de_ady, 0, 0, time_in, color);
    //printVector(esta_en_ciclo);
    //imprimirListaVectores(contar_ciclos);
    //printVector(time_in);
    int res = dp(lista_de_ady, 0, time_in, padre, puente_con_parent);
    //imprimirListaVectores(res);
    //printVector(padre);
    printVector(padre);
    printVectorBool(puente_con_parent);
    imprimirListaVectores(lista_de_ady);
    return 0;
}
//(1,7) (3,10) (3,2) (8,11)
// si un ciclo comparte al menos un nodo con otro ciclo -> sumo la cantidad de nodos en esos ciclos en k
// combinatorio (k 2)
//tengo la cantidad de nodos en ciclos

/*

vector<pair<int, int>> findBridges(int n) {
    vector<pair<int, int>> bridges;
    timer = 0;
    for (int i = 1; i <= n; i++) {
        visited[i] = false;
        discovery[i] = 0;
        low[i] = 0;
    }

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            dfs(i, -1, bridges);
        }
    }

    return bridges;
}


void dfs(int u, int parent, vector<pair<int, int>>& bridges) {
    visited[u] = true;
    discovery[u] = low[u] = ++timer;

    for (int v : adj[u]) {
        if (v == parent)
            continue;

        if (!visited[v]) {
            dfs(v, u, bridges);
            low[u] = min(low[u], low[v]);

            if (low[v] > discovery[u]) {
                bridges.push_back({u, v});
            }
        } else {
            low[u] = min(low[u], discovery[v]);
        }
    }
}
*/
