#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <queue>
#include <algorithm>

using namespace std;

int n;
int m;

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

void BFS(int d, vector<vector<int>> &pares_de_caida, vector<int> &caidos){
    //int hijos_caidos = 0;
    caidos[d] = 0;
    queue<int> cola;
    cola.push(d);

    while(!cola.empty()){
        int u = cola.front();
        cola.pop();

        for(auto v : pares_de_caida[u]){
            if(caidos[v] == 0)
                continue;  
            caidos[v] = 0;
            //hijos_caidos++;
            //distancia[v] = distancia[d] + 1;
            cola.push(v);
        }
    }
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

        //si pudo visitar a alguien lo pusheo
        BFS(inicio, pares_de_caida, caidos);
        optimo.back().push_back(inicio);
}

void domino(vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){
    //no tiene sentido empezar bfs desde nodos que tengan padre, pues alguien los va a tirar
    //tampoco tiene sentido empezar bfs desde un nodo que no tiene padre
    caidos.assign(n, 1);
    optimo.push_back({});
    for(int i = 0; i < n; i++){
        if(tiene_padre[i])
            continue;
        ronda(i, pares_de_caida, caidos, optimo);
    }

    for(int i = 0; i < n; i++){
        if(pares_de_caida[i].size() != 0 && caidos[i])
            ronda(i, pares_de_caida, caidos, optimo);
    }
}

void menorConjOptimo(vector<vector<int>> &optimo){
    vector<int> menor_lex = optimo[0];
   // imprimirListaVectores(optimo);

    sort(menor_lex.begin(), menor_lex.end());


    for(int i = 1; i < optimo.size(); i++){
        if(optimo[i].size() < menor_lex.size()) 
            menor_lex = optimo[i];
        else if(optimo[i].size() == menor_lex.size()){
            sort(optimo[i].begin(), optimo[i].end());
            if(optimo[i] < menor_lex)
                menor_lex = optimo[i];
        }
    }

    cout << menor_lex.size() << endl;
    printVector(menor_lex);
}


vector<vector<int>> procesarEntrada(string test_in){
    ifstream entrada;

    entrada.open(test_in);
    entrada >> n >> m;
    vector<vector<int>> pares_de_caida(n);
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
    vector<vector<int>> pares_de_caida = procesarEntrada(test_in);

    domino(pares_de_caida, caidos, optimo);
    //imprimirListaVectores(optimo);

    menorConjOptimo(optimo);
    //imprimirListaVectores(pares_de_caida);
    
    return 0;
}

//Diremos que un conjunto óptimo B1 es menor a otro B2 si B1 visto como secuencia ordenada es menor lexicográficamente que B2 visto como secuencia ordenada

// test2-1 expected out:    3
//                          1 4 7