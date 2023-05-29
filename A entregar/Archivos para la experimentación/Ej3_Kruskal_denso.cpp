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

int cant_oficinas, cant_modems;
long double costo_UTP, costo_fibra, cota_UTP, gasto_fibra, gasto_UTP;

typedef tuple<int, int, long double> arista;

////////////////////////// distancias //////////////////////////

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

////////////////////////// auxiliares de kruskal //////////////////////////

void encontrarOficinaMasCercana(vector<vector<long double>> &matriz, vector<long double> &oficina_mas_cercana){
    for(int i = 0; i < matriz.size(); i++){
        //pongo en oficina mas cercana la arista incidente a i de menor costo
        for(int j = i+1; j < matriz.size(); j++)
            oficina_mas_cercana[i] = min(oficina_mas_cercana[i], matriz[i][j]);
    }
}

arista menorDistancia(vector<vector<long double>>& matriz, vector<long double> &distancia_oficina_mas_cercana, vector<bool> &representantes) {
    int mejor_representante = 0; 
    int ofi_mas_cercana = 0;
    long double dist = 0;

    //encontramos el primer representante, es decir la raiz de una componente conexa
    while (!representantes[mejor_representante])    
        ++mejor_representante;    
    
    //encuentra el representante con la menor arista incidente a el
    for (int i = mejor_representante + 1; i < cant_oficinas; ++i) {
        if (!representantes[i]) 
            continue;
        if (distancia_oficina_mas_cercana[i] < distancia_oficina_mas_cercana[mejor_representante]) 
            mejor_representante = i;
    }
    
    dist = distancia_oficina_mas_cercana[mejor_representante];

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
        matriz[i][oficina_1] = matriz[oficina_1][i] = min(matriz[oficina_1][i], matriz[oficina_2][i]);
        oficina_mas_cercana[oficina_1] = min(oficina_mas_cercana[oficina_1], matriz[oficina_1][i]);
    }
}

////////////////////////// kruskal //////////////////////////

void kruskal_denso(vector<vector<long double>> &matriz, vector<long double> &oficina_mas_cercana, vector<arista> &AGM){
    int restantes = cant_oficinas - 1;
    vector<bool> representantes(cant_oficinas, true);

    encontrarOficinaMasCercana(matriz, oficina_mas_cercana);

    while(restantes > 0){
        arista cable = menorDistancia(matriz, oficina_mas_cercana, representantes);
        long double longitud_cable = get<2>(cable);

        if( longitud_cable <= cota_UTP)
            gasto_UTP += costo_UTP*longitud_cable;
        else
            gasto_fibra += costo_fibra*longitud_cable;

        AGM.push_back(cable);
        recalcularOfiMasCerecana(cable, matriz, oficina_mas_cercana, representantes);
        restantes--;
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



int main(int argc, char** argv){
    ifstream archivo_in;
    ofstream archivo_out;
    string test_in = "";
    string test_out = "";
    int c;
    long double x, y;

    if(argc > 1){
        test_in = argv[1];
        test_out = argv[2];
        archivo_in.open(test_in);
        archivo_out.open(test_out);
    }

    if(test_in.empty())
        std::cin >> c;
    else
        archivo_in >> c;

    for(int i = 0; i < c; i++){
        if(test_in.empty())
            std :: cin >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;
        else  
            archivo_in >> cant_oficinas >> cota_UTP >> cant_modems >> costo_UTP >> costo_fibra;

        vector<vector<long double>> matriz(cant_oficinas, vector<long double>(cant_oficinas, INFINITY));
        vector<long double> oficina_mas_cercana(cant_oficinas, INFINITY);
        vector<pair<long double, long double>> posicion(cant_oficinas);
        vector<arista> AGM;

        gasto_fibra = 0; 
        gasto_UTP = 0;
        
        for(int j = 0; j < cant_oficinas; j++){
            if(test_in.empty())
                std::cin >> x >> y;
            else  
                archivo_in >> x >> y;

            posicion[j] = make_pair(x, y);
        }
        
        calcularDistanciaCables(posicion, matriz);
        auto start = chrono::system_clock::now();
        
        kruskal_denso(matriz, oficina_mas_cercana, AGM);
        
        auto end = chrono::system_clock::now();
        chrono::duration<float,milli> duration = end - start;

        archivo_out << cant_oficinas << "; " << duration.count() << endl;
        
        ponerModems(AGM, i);
    }

    archivo_in.close();
    archivo_out.close();
    
    return 0;
}   
