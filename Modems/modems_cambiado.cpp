#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <algorithm>
#include <tuple>

using namespace std;

int c;
int cant_oficinas, cant_modems, costo_UTP_metros, costo_fibra_metros, cota_UTP_metros;
double gasto_UTP = 0.0;
double gasto_fibra = 0.0;
vector<tuple<double, double>> res(0);

void imprimirVectorDeTuplas(const vector<tuple<int, int, double>>& vectorTuplas){
    for (const auto& tupla : vectorTuplas){
        int elemento1 = get<0>(tupla);
        int elemento2 = get<1>(tupla);
        double elemento3 = get<2>(tupla);

        cout << "(" << elemento1 << ", " << elemento2 << ", " << elemento3 << ")" << endl;
    }
}

void imprimirVectorDeTuplasDobles(const vector<pair<int, int>>& vectorTuplas){
    for (const auto& tupla : vectorTuplas){
        int elemento1 = get<0>(tupla);
        int elemento2 = get<1>(tupla);

        cout << "(" << elemento1 << ", " << elemento2 << ")" << endl;
    }
}

bool compararTuplas(const tuple<int, int, double>& tupla1, const tuple<int, int, double>& tupla2){
    return get<2>(tupla1) < get<2>(tupla2); 
}

void ordenarVectorDeTuplas(std::vector<std::tuple<int, int, double>>& vectorTuplas){
    sort(vectorTuplas.begin(), vectorTuplas.end(), compararTuplas);  
} 


// DISJOINT UNION SET de la practica
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
vector<tuple<int, int, double>> calcularDistancias(vector<pair<int, int>> &posiciones){  // calcula la distancia entre todos los nodos
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
        if(get<2>(AGM[i]) <= cota_UTP_metros){
            gasto_UTP -= get<2>(AGM[i])*costo_UTP_metros;
        } else{
            gasto_fibra -= get<2>(AGM[i])*costo_fibra_metros;
        }
        AGM.pop_back();
    }
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
            if(distancia_entre_ofis <= cota_UTP_metros){
                gasto_UTP += costo_UTP_metros * distancia_entre_ofis; 
            }
            else{
                gasto_fibra += costo_fibra_metros*distancia_entre_ofis;
            }

            dsu.unite(nodo_a, nodo_b);
            AGM.push_back(nodo);
        }
    }

    return AGM;
}

void procesarEntrada(){
    cin >> c;

    for(int i = 0; i < c; i++){
        cin >> cant_oficinas >> cota_UTP_metros >> cant_modems >> costo_UTP_metros >> costo_fibra_metros;   

        gasto_UTP = 0.0;
        gasto_fibra = 0.0;
         
        vector<pair<int, int>> posicion(cant_oficinas); 
        vector<tuple<int, int, double>> distancias;  
        vector<tuple<int, int, double>> AGM;  

        for(int k = 0; k < cant_oficinas; k++){
            int x, y;
            cin >> x >> y;
            pair<int, int> posicion_k = make_pair(x, y);
            posicion[k] = posicion_k;
        }

        distancias = calcularDistancias(posicion);
        AGM = kruskal(distancias);
        ponerModems(AGM);
    
        pair<double, double> res1 = make_pair(gasto_UTP, gasto_fibra);
        res.push_back(res1);
    }
}

int main(){
    procesarEntrada();

    int f=1;
    for (const auto& tupla : res){
        double elemento1 = get<0>(tupla);
        double elemento2 = get<1>(tupla);

        cout << setprecision(3) << fixed;
        cout << "Caso #" << f << ": " << elemento1 << " " << elemento2 << endl;
        f++;
        }

    return 0;
}
