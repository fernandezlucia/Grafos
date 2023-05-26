#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <tuple>
#include <limits>
#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

using ll = long long;
const ll inf = 1e18;

vector<vector<int>> loc;
int nro_test;
int test;
int n, r, w, u, v;
vector<tuple<double, int, int, int>> E;
int componentes;
double Pu, Pv;

struct DSU
{

    DSU(int n)
    {
        padre = rank = vector<int>(n); // Inicializo vector de padres y su distancia al padre
        for (int v = 0; v < n; v++)
            padre[v] = v; // cada nodo es su propio padre
    }

    int find(int v)
    { // Encuentra el padre de v
        if (v == padre[v])
            return v;
        return padre[v] = find(padre[v]);
    }

    void unite(int u, int v)
    { //
        u = find(u), v = find(v);
        if (u == v)
            return;
        if (rank[u] < rank[v])
            swap(u, v); // agarro el de mayor ranking
        padre[v] = padre[u];
        rank[u] = max(rank[u], rank[v] + 1); // me aumenta la distancia a del mas chico
    }

    vector<int> padre;
    vector<int> rank;
};

void printVector(const std::vector<std::vector<int>> &vec)
{
    for (const auto &innerVec : vec)
    {
        for (const auto &element : innerVec)
        {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

void printTripla(const std::vector<std::tuple<ll, int, int>> &vec)
{
    for (const auto &element : vec)
    {
        ll value = std::get<0>(element);
        int secondValue = std::get<1>(element);
        int thirdValue = std::get<2>(element);

        std::cout << value << " " << secondValue << " " << thirdValue << std::endl;
    }
}

double distancia_euclidea(double x0, double x1, double y0, double y1)
{
    double deltaX = x0 - y0;
    double deltaY = x1 - y1;

    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

vector<tuple<double, int, int, int>> crear_grafo(vector<vector<int>> nodos)
{
    vector<tuple<double, int, int, int>> E;
    double c;        // precio del cable
    int tipoDeCable; // 0 si es UTP, 1 si es Fibra Optica
    for (int i = 0; i < nodos.size(); i++)
    {
        for (int j = i + 1; j < nodos.size(); j++)
        {
            if (i != j)
            {
                int x1 = nodos[i][0];
                int y1 = nodos[i][1];

                int x2 = nodos[j][0];
                int y2 = nodos[j][1];
                int d = distancia_euclidea(x1, y1, x2, y2);
                if (d <= r)
                {
                    c = u * d;
                    tipoDeCable = 0;
                }
                else
                {
                    c = v * d;
                    tipoDeCable = 1;
                }
                E.push_back({c, i, j, tipoDeCable});
            }
        }
    }
    return E;
}

void kruskal()
{

    sort(E.begin(), E.end());
    double res = 0;
    int aristas = 0;
    DSU dsu(n);
    for (auto [c, u, v, tc] : E)
    {
        // si (u,v) es arista segura
        if (dsu.find(u) != dsu.find(v))
        {
            // agregar
            dsu.unite(u, v);
            if (tc == 0)
            {
                Pu += c;
            }
            else
            {
                Pv += c;
            }
            aristas++;
            res += c;
            componentes--; // me disminuye la cantidad de componentes
            if (componentes == w)
            { // salgo del for
                break;
            }
        }
    }

    if (aristas == n - w)
        std::cout << std::fixed << std::setprecision(3) << "Caso #" << test << ": " << Pu << " " << Pv << endl;
    else
        std::cout << "IMPOSSIBLE\n";
}

int main()
{
    cin >> nro_test;

    for (int i = 0; i < nro_test; i++)
    {
        test++;
        Pu = 0;
        Pv = 0;
        cin >> n >> r >> w >> u >> v;
        componentes = n;
        for (int i = 0; i < n; i++)
        {
            int x, y;
            cin >> x >> y;
            loc.push_back({x, y});
        }
        E = crear_grafo(loc);
        kruskal();
    }

    // printTripla(E);
    // cout<<componentes<<endl;

    // printTripla(crear_grafo(loc));
    return 0;
}