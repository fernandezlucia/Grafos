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
        } else {
            //si no tiro a nadie y no tiene padre, entonces nadie lo puede tirar
            //si no tiro a nadie y tiene padre, entonces alguien lo va a tirar
            if(!tiene_padre[i%n]){
                optimo.back().push_back(i%n);
                caidos[i%n] = 0;
            }
        }
    }
}


void domino(vector<vector<int>> &pares_de_caida, vector<int> &caidos, vector<vector<int>> &optimo){
    for(int i = 0; i < n; i++){
        //si tiene padre pero no tiene hijos no va a visitar a nadie
        caidos.assign(n, 1);
        optimo.push_back({});
        ronda(i, pares_de_caida, caidos, optimo);
    }
}


void menorConjOptimo(vector<vector<int>> &optimo){
    vector<int> menor_lex = optimo[0];

    for(int i = 1; i < optimo.size(); i++){
        if((optimo[i].size() < menor_lex.size()) || (optimo[i].size() == menor_lex.size() && optimo[i] < menor_lex))
            menor_lex = optimo[i];
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
    //imprimirListaVectores(optimo);
    //imprimirListaVectores(pares_de_caida);
    
    return 0;
}

//Diremos que un conjunto óptimo B1 es menor a otro B2 si B1 visto como secuencia ordenada es menor lexicográficamente que B2 visto como secuencia ordenada

// test2-1 expected out:    3
//                          1 4 7