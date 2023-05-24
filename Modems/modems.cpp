#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <queue>
#include <algorithm>

using namespace std;
int cant_oficinas, cant_modems, cota_UTP, costo_UTP, costo_fibra;
double gasto_UTP = 0;
double gasto_fibra = 0;


void imprimirVectorDeTuplas(const std::vector<std::tuple<int, int, double>>& vectorTuplas){
    for (const auto& tupla : vectorTuplas){
        int elemento1 = std::get<0>(tupla);
        int elemento2 = std::get<1>(tupla);
        double elemento3 = std::get<2>(tupla);

        std::cout << "(" << elemento1 << ", " << elemento2 << ", " << elemento3 << ")" << std::endl;
    }
}

bool compararTuplas(const std::tuple<int, int, double>& tupla1, const std::tuple<int, int, double>& tupla2){
    return std::get<2>(tupla1) < std::get<2>(tupla2);
}

void ordenarVectorDeTuplas(std::vector<std::tuple<int, int, double>>& vectorTuplas){
    std::sort(vectorTuplas.begin(), vectorTuplas.end(), compararTuplas);
}

struct DSU{
    DSU(int n){
        padre = rank = vector<int>(n);
        for(int v = 0; v < n; v++) 
            padre[v] = v;
    }

    int find(int v){
        if(v == padre[v]) 
            return v;
        return padre[v] = find(padre[v]);
    }

    void unite(int u, int v){
        u = find(u), v = find(v);
        if(u == v) 
            return;
        
        if(rank[u] < rank[v]) 
            swap(u,v);
        
        padre[v] = padre[u];
        rank[u] = max(rank[u],rank[v]+1);
    }

    vector<int> padre;
    vector<int> rank;
};


double distanciaEuclideana(pair<int, int> a, pair<int, int> b){
    return sqrt(pow(b.first - a.first, 2) + pow(b.second - a.second, 2));
}

//recorrer la matriz triangularmente
vector<tuple<int, int, double>> calcularDistancias(vector<pair<int, int>> &posiciones){
vector<tuple<int, int, double>> distancias(0);
pair<int, int> posicion_i;
pair<int, int> posicion_j;

    for(int i = 0; i < posiciones.size(); i++){
        posicion_i = posiciones[i];
        for(int j = i+1; j < posiciones.size(); j++){
            posicion_j = posiciones[j];
            tuple<int, int, double> arista = make_tuple(i, j, distanciaEuclideana(posicion_i, posicion_j));
            distancias.push_back(arista);
        }
    }
    return distancias;
}


void ponerModems(vector<tuple<int, int, double>> &AGM){
    cant_modems--;
    
    int longitud = AGM.size()-1;
    for(int i = longitud; i > longitud-cant_modems; i--){
        if(get<2>(AGM[i]) <= cota_UTP)
            gasto_UTP -= get<2>(AGM[i])*costo_UTP;
        else
            gasto_fibra -= get<2>(AGM[i])*costo_fibra;
        AGM.pop_back();
    }

    cout << gasto_UTP << " " << gasto_fibra  << endl;
    imprimirVectorDeTuplas(AGM);
}



vector<tuple<int, int, double>> kruskal(vector<tuple<int, int, double>> &distancias){
    ordenarVectorDeTuplas(distancias);
    vector<tuple<int, int, double>> AGM(0);

    DSU dsu(cant_oficinas);
    
    for(auto nodo : distancias){
        //si (a, b) es arista segura
        int nodo_a = get<0>(nodo);
        int nodo_b = get<1>(nodo);
        double distancia_entre_ofis = get<2>(nodo);

        if(dsu.find(nodo_a) != dsu.find(nodo_b)){
            // agregar
            if(distancia_entre_ofis <= cota_UTP)
                gasto_UTP += costo_UTP*distancia_entre_ofis; //pongo cables utp
            else    
                gasto_fibra += costo_fibra*distancia_entre_ofis;//pongo cables de fibra optica

            dsu.unite(nodo_a, nodo_b);
            AGM.push_back(nodo);
        }
    }

    return AGM;
}

void procesarEntrada(string test_in){
    ifstream entrada;
    int c;

    entrada.open(test_in);
    entrada >> c;

    for(int i = 0; i < c; i++){
        entrada >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;   
         
        vector<pair<int, int>> posicion(cant_oficinas);
        vector<tuple<int, int, double>> distancias;
        vector<tuple<int, int, double>> AGM;

        for(int i = 0; i < cant_oficinas; i++){
            int x, y;
            entrada >> x >> y;
            pair<int, int> posicion_i = make_pair(x, y);
            posicion[i] = posicion_i;
        }

        distancias = calcularDistancias(posicion);
        AGM = kruskal(distancias);
        ponerModems(AGM);
    }
    entrada.close();
}

int main(int argc, char **argv){
    string test_in = argv[1];
    procesarEntrada(test_in);
    return 0;
}
