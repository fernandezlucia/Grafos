#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <queue>
#include <algorithm>
#include <tuple>
#include <chrono>

using namespace std;

////////////////////////// variables //////////////////////////

typedef tuple<int, int, long double> arista;

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
        u = find(u);
        v = find(v);
        if(u == v) 
            return;
        
        if(rank[u] < rank[v])
            swap(u,v);
        
        padre[v] = padre[u];
        rank[u] = max(rank[u],rank[v]+1);
    }

    vector<int> padre, rank;
};

int cant_oficinas, cant_modems;
long double costo_UTP, costo_fibra, cota_UTP;
long double gasto_UTP, gasto_fibra;

////////////////////////// distancias //////////////////////////

double distanciaEuclideana(pair<long double, long double> a, pair<long double, long double> b){
    return sqrt((b.first - a.first)*(b.first - a.first) + (b.second - a.second)*(b.second - a.second));
}

void calcularDistanciaCables(vector<pair<long double, long double>> &posicion_oficinas, vector<arista> &cables){

    for(int i = 0; i < posicion_oficinas.size(); i++){
        pair<int, int> pos_oficina_i = posicion_oficinas[i];
        for(int j = i + 1; j < posicion_oficinas.size(); j++){
            pair<int, int> pos_oficina_j = posicion_oficinas[j];

            cables.push_back(make_tuple(i, j, distanciaEuclideana(pos_oficina_i, pos_oficina_j)));
        }
    }
}

////////////////////////// kruskal //////////////////////////

bool compararAristas(const arista& tupla1, const arista& tupla2){
    return get<2>(tupla1) < get<2>(tupla2);
}

void kruskal(vector<arista> &cables, vector<arista> &AGM){
    sort(cables.begin(), cables.end(), compararAristas);

    DSU dsu(cant_oficinas);
    
    for(arista cable : cables){

        int oficina_a = get<0>(cable);
        int oficina_b = get<1>(cable);
        long double distancia_entre_ofis = get<2>(cable);

        //si a no conecta con b
        if(dsu.find(oficina_a) != dsu.find(oficina_b)){
            //sumar a gastos
            if(distancia_entre_ofis <= cota_UTP)
                gasto_UTP += costo_UTP * distancia_entre_ofis;
            else 
                gasto_fibra += costo_fibra * distancia_entre_ofis;

            //conectar oficinas
            dsu.unite(oficina_a, oficina_b);

            //agregar cable
            AGM.push_back(cable);
        }
    }
}

////////////////////////// poner modems //////////////////////////

void ponerModems(vector<arista> &AGM, int test){
    long double distancia_entre_ofis;

    //por cada modem adicional al primero
    for(int i = 1; i < cant_modems; i++){
        // obtener cable mas caro (esta ordenado)
        distancia_entre_ofis = get<2>(AGM.back());

        //saco costo del cable
        if(distancia_entre_ofis <= cota_UTP)
            gasto_UTP -= costo_UTP*distancia_entre_ofis;
        else 
            gasto_fibra -= costo_fibra*distancia_entre_ofis;

        //saco cable
        AGM.pop_back();
    }

    //output
    std::cout << fixed << setprecision(3) << "Caso #" << test+1 << ": " << gasto_UTP << " " << gasto_fibra << endl;
}

////////////////////////// main //////////////////////////

int main(int argc, char** argv){
    ifstream archivo_in;
    ofstream archivo_out;
    string test_in = "";
    string test_out = "";
    
    int c;
    long double x, y;
    
    //si hay parametro, es archivo de tests
    if (argc > 1){
        test_in = argv[1];
        test_out = argv[2];
        archivo_in.open(test_in);
        archivo_out.open(test_out);
    }


    //obtener cantidad de tests
    if(test_in.empty())
        std :: cin >> c;
    else
        archivo_in >> c;

    //por cada test
    for(int i = 0; i < c; i++){
        //leer datos importantes del test
        if(test_in.empty())
            std :: cin >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;
        else
            archivo_in >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;

        //creamos estructuras para algoritmos
        vector<pair<long double, long double>> posicion(cant_oficinas);
        vector<arista> cables;
        vector<arista> AGM;
        
        //reseteamos
        gasto_fibra = 0; 
        gasto_UTP = 0;

        for(int j = 0; j < cant_oficinas; j++){
            //leer posicion de nodos
            if(test_in.empty())
                std :: cin >> x >> y;
            else
                archivo_in >> x >> y;

            posicion[j] = make_pair(x, y);
        }

        //calculamos aristas (cables entre officinas)
        calcularDistanciaCables(posicion, cables);
        auto start = chrono::system_clock::now();
        
        kruskal(cables, AGM);
        
        auto end = chrono::system_clock::now();
        chrono::duration<float,milli> duration = end - start;

        archivo_out << cant_oficinas << "; " << duration.count() << endl;
        ponerModems(AGM, i);


    }
    
    archivo_in.close();
    archivo_out.close();
    return 0;
}
