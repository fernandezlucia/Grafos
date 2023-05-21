#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <queue>

using namespace std;

int n;
int m;

int c;
vector<bool> tiene_padre;

void imprimirListaVectores(const vector<vector<int>>& lista) {
    for (const auto& vec : lista) {
        for (const auto& elem : vec) {
            cout << elem + 1 << " ";
        }
        cout << endl;
    }
}

void printVector(vector<int> v){
    for (const auto& elem : v) {
        cout << elem+1 << " ";
    }
    cout << endl;
}

void printVectorBool(vector<bool> v){
    for (const auto& elem : v) {
        cout << elem << " ";
    }
    cout << endl;
}

int BFS(int d, vector<vector<int>> &pares_de_caida, vector<int> &caidos){
    int hijos_caidos = 0;
    queue<int> cola;
    cola.push(d);

    while(!cola.empty()){
        int u = cola.front();
        cola.pop();

        for(auto v : pares_de_caida[u]){
            if(caidos[v] == 0)
                continue;  
            caidos[v] = 0;
            hijos_caidos++;
            //distancia[v] = distancia[d] + 1;
            cola.push(v);
        }
    }

    return hijos_caidos;
}
//si todos sus hijos estaban caidos o si no tiene hijos no lo quiero pushear
//si no puedo tirar a nadie y nadie me puede tirar, me tiro
//1 6
//2,3,6
//3,6
//4,6
//6
//6

void ronda(int inicio, vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){
    for(int i = inicio; i < caidos.size() + inicio; i++){
        if(caidos[i%n] == 0)
            continue;

        //si pudo visitar a alguien lo pusheo
        if(BFS(i%n, pares_de_caida, caidos) != 0){
            caidos[i%n] = 0;
            optimo.back().push_back(i%n);
        }

        //es una componente conexa de un solo nodo
        if(!tiene_padre[i%n] && pares_de_caida[i%n].size() == 0)
            optimo.back().push_back(i%n);
    }
}


void domino(vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){
    for(int i = 0; i < n; i++){
        //si tiene padre pero no tiene hijos no va a visitar a nadie
        if(tiene_padre[i] && pares_de_caida[i].size() == 0)
            continue;

        caidos.assign(n, 1);
        optimo.push_back({});
        ronda(i, pares_de_caida, caidos, optimo);
    }
}

vector<vector<int>> procesarEntrada(string test_in, vector<bool> &tiene_padre){
    ifstream entrada;
    vector<vector<int>> pares_de_caida(n);

    entrada.open(test_in);
    if(!entrada.is_open()){
        //falla
    }

    entrada >> n >> m;
    tiene_padre.assign(n, 0);

    for(int i=0; i < m; ++i){
        int nodo1, nodo2;
        entrada >> nodo1 >> nodo2;
        pares_de_caida[nodo1 -1].push_back(nodo2-1);
        tiene_padre[nodo2-1] = true;
    }

    entrada.close();

    return pares_de_caida;
}

int main(int argc, char **argv){
    string test_in = argv[1];
    vector<int> caidos(n, 1);
    vector<vector<int>> optimo(0);
    vector<vector<int>> pares_de_caida = procesarEntrada(test_in, tiene_padre);
    
    domino(pares_de_caida, caidos, optimo);
    imprimirListaVectores(optimo);
    //imprimirListaVectores(pares_de_caida);
    
    return 0;
}

//Diremos que un conjunto óptimo B1 es menor a otro B2 si B1 visto como secuencia ordenada es menor lexicográficamente que B2 visto como secuencia ordenada

// test2-1 expected out:    3
//                          1 4 7