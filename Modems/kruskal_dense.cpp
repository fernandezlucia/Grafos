#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iomanip> 
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

int cant_oficinas, cant_modems;
long double costo_UTP, costo_fibra, cota_UTP, gasto_fibra, gasto_UTP;

typedef tuple<int, int, long double> arista;

void imprimirVectorDeTuplas(const vector<tuple<int, int, double>>& vectorTuplas){
    for (const auto& tupla : vectorTuplas){
        int elemento1 = get<0>(tupla);
        int elemento2 = get<1>(tupla);
        double elemento3 = get<2>(tupla);

        std ::  cout << "(" << elemento1 << ", " << elemento2 << ", " << elemento3 << ")" << endl;
    }
}

double distanciaEuclideana(pair<long double, long double> a, pair<long double, long double> b){
    return sqrt((b.first - a.first)*(b.first - a.first) + (b.second - a.second)*(b.second - a.second));
}

void calcularDistanciaCables(vector<pair<long double, long double>> &posicion_oficinas,vector<vector<long double>> &matriz){
    
    for(int i = 0; i < posicion_oficinas.size(); i++){
        pair<int, int> pos_oficina_i = posicion_oficinas[i];

        for(int j = 0; j < posicion_oficinas.size(); j++){
            pair<int, int> pos_oficina_j = posicion_oficinas[j];
            matriz[i][j] = distanciaEuclideana(pos_oficina_i, pos_oficina_j);
        }
    }
}

void encontrarOficinaMasCercana(vector<vector<long double>> &matriz, vector<long double> &oficina_mas_cercana){
    for(int i = 0; i < matriz.size(); i++){
        for(int j = 0; j < matriz.size(); j++){
            //pongo en oficina mas cercana la arista incidente a i de menor costo
            oficina_mas_cercana[i] = min(oficina_mas_cercana[i], matriz[i][j]);
        }
    }
}

arista menorDistancia(vector<vector<long double>>& matriz, vector<long double> &distancia_oficina_mas_cercana, vector<bool> &representantes) {
    int mejor_representante = 0; //idx
    
    while (!representantes[mejor_representante])
        mejor_representante++;
  
    //encuentra el representante con la menor arista incidente a el
    for (int i = mejor_representante + 1; i < cant_oficinas; ++i) {
        if (!representantes[i]) 
            continue;
        if (distancia_oficina_mas_cercana[i] < distancia_oficina_mas_cercana[mejor_representante]) 
            mejor_representante = i;
    }
    
    long double dist = distancia_oficina_mas_cercana[mejor_representante];
    int ofi_mas_cercana = 0;
    
    //encuentra el nodo que conecta la menor distancia
    for (int i = 0; i < cant_oficinas; ++i) {
        if (!representantes[i]) 
            continue;
        if (matriz[mejor_representante][i] == dist) {
            ofi_mas_cercana = i;
            break;
        }
    }

    return make_tuple(mejor_representante, ofi_mas_cercana, dist);
}

void recalcularOfiMasCerecana(arista cable, vector<vector<long double>> &matriz, vector<long double> &oficina_mas_cercana, vector<bool> &representante) {
    int oficina_1 = get<0>(cable);
    int oficina_2 = get<1>(cable);

    representante[oficina_2] = false;
    oficina_mas_cercana[oficina_1] = INFINITY;

    for (int i = 0; i < cant_oficinas; ++i) {
        if (!representante[i]) 
            continue;
        if (i == oficina_1) 
            continue;
        matriz[i][oficina_1] = min(matriz[oficina_1][i], matriz[oficina_2][i]);
        oficina_mas_cercana[oficina_1] = min(oficina_mas_cercana[oficina_1], matriz[oficina_1][i]);
    }
}


void kruskal_denso(vector<vector<long double>> &matriz, vector<long double> &oficina_mas_cercana, vector<arista> &AGM){
    int restantes = cant_oficinas - 1;
    vector<bool> representantes(cant_oficinas, true);

    encontrarOficinaMasCercana(matriz, oficina_mas_cercana);
    while(restantes > 0){
        arista cable = menorDistancia(matriz, oficina_mas_cercana, representantes);
        
        long double longitud_cable = get<2>(cable);
        if( longitud_cable <= cota_UTP){
            gasto_UTP += costo_UTP*longitud_cable;
        } else {
            gasto_fibra += costo_fibra*longitud_cable;
        }
        AGM.push_back(cable);
        recalcularOfiMasCerecana(cable, matriz, oficina_mas_cercana, representantes);
    }
}

void ponerModems(vector<arista> &AGM, int test){
    long double distancia_entre_ofis;

    //por cada modem adicional al primero
    for(int i = 1; i < cant_modems; i++){
        // obtener cable mas caro (esta ordenado)
        distancia_entre_ofis = get<2>(AGM[AGM.size()-1]);

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



int main(int argc, char** argv){
    string test_nombre = "";
    ifstream archivo;
    int c;
    long double x, y;
    
    //si hay parametro, es archivo de tests
   if (argc > 1){
        test_nombre = argv[1];
        std::cout << test_nombre << endl;
        archivo.open(test_nombre);
        std::cout << test_nombre << endl;
            if (!archivo.is_open()) {
                std::cout << "No se pudo abrir el archivo." << endl;
                return 1;
            }
    }

    //obtener cantidad de tests
    if(test_nombre.empty())
        std :: cin >> c;
    else
        archivo >> c;

    //por cada test
    for(int i = 0; i < c; i++){
        //leer datos importantes del test
        if(test_nombre.empty())
          std :: cin >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;
        else
            archivo >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;

        //creamos estructuras para algoritmos
        vector<pair<long double, long double>> posicion(cant_oficinas);
        vector<vector<long double>> matriz_ady(cant_oficinas, vector<long double>(cant_oficinas, -1));
        vector<long double> oficina_mas_cercana(cant_oficinas, INFINITY);
        vector<arista> AGM;
        
        //reseteamos
        gasto_fibra = 0; 
        gasto_UTP = 0;

        for(int j = 0; j < cant_oficinas; j++){
            //leer posicion de nodos
           if(test_nombre.empty())
               std :: cin >> x >> y;
            else
                archivo >> x >> y;

            posicion[j] = make_pair(x, y);
        }

        //calculamos aristas (cables entre officinas)
        calcularDistanciaCables(posicion, matriz_ady);
        kruskal_denso(matriz_ady, oficina_mas_cercana, AGM);
        ponerModems(AGM, i);
    }

    return 0;
}