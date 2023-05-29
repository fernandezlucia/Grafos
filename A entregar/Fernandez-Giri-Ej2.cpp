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

void quickSort(vector<int>& vec, int izq, int der) {
    int i = izq;
    int j = der;
    int pivote = vec[(izq + der) / 2];
    while (i <= j) {
        while (vec[i] < pivote)
            i++;
        
        while (vec[j] > pivote) 
            j--;
        
        if (i <= j) {
            swap(vec[i], vec[j]);
            i++;
            j--;
        }
    }

    if (izq < j) 
        quickSort(vec, izq, j);
    if (i < der) 
        quickSort(vec, i, der);
}


void printVector(vector<int> v){
    for (const auto& elem : v) {
        cout << elem+1 << " ";
    }
    cout << endl;
}

void BFS(int d, vector<vector<int>> &pares_de_caida, vector<int> &caidos){
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
            cola.push(v);
        }
    }
}


void ronda(int inicio, vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){

        BFS(inicio, pares_de_caida, caidos);
        optimo.back().push_back(inicio);
}

void domino(vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){
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
    quickSort(menor_lex, 0, menor_lex.size() - 1);

    for(int i = 1; i < optimo.size(); i++){
        if(optimo[i].size() < menor_lex.size()) 
            menor_lex = optimo[i];
        else if(optimo[i].size() == menor_lex.size()){
            quickSort(optimo[i], 0, optimo[i].size() - 1);
            if(optimo[i] < menor_lex)
                menor_lex = optimo[i];
        }
    }

    cout << menor_lex.size() << endl;
    printVector(menor_lex);
}

vector<vector<int>> procesarEntrada(){
    cin >> n >> m;
    tiene_padre.assign(n, 0);

    vector<vector<int>> pares_de_caida(n);
    for(int i=0; i < m; ++i){
        int nodo1, nodo2;
        cin >> nodo1 >> nodo2;
        pares_de_caida[nodo1 -1].push_back(nodo2-1);
        tiene_padre[nodo2-1] = true;
    }

    return pares_de_caida;
}


int main(){
    vector<vector<int>> pares_de_caida = procesarEntrada();
    vector<int> caidos(n, 1);
    vector<vector<int>> optimo(0);
    
    domino(pares_de_caida, caidos, optimo);
    menorConjOptimo(optimo);
    
    return 0;
}