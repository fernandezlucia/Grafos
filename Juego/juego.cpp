#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>

using namespace std;

#define BLANCO 0
#define GRIS 1
#define NEGRO 2



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



//CHAT GPT ME DIO ESTO SUPUESTAMENTE ES O(M+N)
//----------------------------------------------

//const int MAXN = 100000;
//
//vector<int> adj[MAXN + 1];
//int vis[MAXN + 1];
//int parent[MAXN + 1];
//vector<int> cycle_edges;
//
//void dfs(int u) {
//    vis[u] = 1;
//    for (int v : adj[u]) {
//        if (vis[v] == 0) {
//            parent[v] = u;
//            dfs(v);
//        } else if (vis[v] == 1 && parent[u] != v) {
//            int cur = u;
//            cycle_edges.push_back(1);
//            while (cur != v) {
//                cycle_edges.back()++;
//                cur = parent[cur];
//            }
//        }
//    }
//    vis[u] = 2;
//}
//

void DFS(vector<vector<int>> &grafo, vector<int> &visitados, vector<int> &ciclos, vector<int> &padre, int nodo){
    visitados[nodo] = GRIS;
    
    //visitamos a sus hijos
    for(int hijo = 0; hijo < grafo[nodo].size(); hijo++){
        padre[hijo] = nodo;
        if(visitados[hijo] == BLANCO)
            DFS(grafo, visitados, ciclos, padre, hijo);
        if(visitados[hijo] == GRIS && hijo != padre[hijo]) //hay una backedge
            ciclos.push_back();
    }
}


vector<vector<int>> procesarEntrada(string test_in){
    int n, m;
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
    vector<vector<int>> lista = procesarEntrada(test_in);

    imprimirListaVectores(lista);
    return 0;
}