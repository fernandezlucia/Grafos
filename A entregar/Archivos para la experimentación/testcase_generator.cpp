#include <cstdio>
#include <random>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iomanip> 
#include <algorithm>
#include <chrono>

using namespace std;

typedef pair<int, int> edge;
int N, R, W, U, V;
int x, y;
int c = 100;

int main(int argc, char** argv){
    string casos_test = argv[1];
    ofstream salida;
    salida.open(casos_test);
    salida << c << endl;
    random_device rd;
    mt19937 gen(rd());

    for(int i = 0; i < c; i++){
        uniform_int_distribution<int> random_N(100, 1000);
        uniform_int_distribution<int> random_R(1, 10000);
        uniform_int_distribution<int> random_V(1, 10);
        N = random_N(gen);
        R = random_R(gen);
        uniform_int_distribution<int> random_W(1, N-1);
        W = random_W(gen);
        V = random_V(gen);
        uniform_int_distribution<int> random_U(1, V);
        U = random_U(gen); 

        salida << N << " " << R << " " << W << " " << U << " " << V << endl;

        for(int j = 0; j < N; j++){
            uniform_int_distribution<int> random_XY(-10000, 10000);
            x = random_XY(gen);
            y = random_XY(gen);
            salida << x << " " << y << endl;
        }
      std:: cout << "Generó caso: " << i+1  << " de test" << endl;
    }

    salida.close();

    return 0;
}
