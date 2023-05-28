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

    printVector(esta_en_ciclo);
    imprimirListaVectores(contar_ciclos);
    //printVector(padre);
    //imprimirListaVectores(lista_de_ady);
    return 0;
}
//(1,7) (3,10) (3,2) (8,11)
// si un ciclo comparte al menos un nodo con otro ciclo -> sumo la cantidad de nodos en esos ciclos en k
// combinatorio (k 2)
//tengo la cantidad de nodos en ciclos