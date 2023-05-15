#include<iostream>
#include<queue>
#include<vector>

using namespace std;

void BFS(vector<int> adjList[], int inicio, int n){
    bool visitado[n];
    queue<int> cola;

    for(int i=0; i<n; i++) visitado[i]=false;

    visitado[inicio]=true;
    cola.push(inicio);

    while(!cola.empty()){
        int u = cola.front();
        cola.pop();
        cout<<u<<" ";

        for (int v: adjList[u]){
            if (visitado[v]==false){
                visitado[v]=true;
                cola.push(v);
            }
        }
    }
}

int main(){
    int n, m, x, y;
    cin>>n>>m;

    vector<int> adjList[n+1];

    for(int i=0; i<m; i++){
        cin>>x>>y;
        adjList[x].push_back(y);
        adjList[y].push_back(x);
    }

    for (int i=1; i<=n; i++){
        cout<<"Nodo "<<i<<": ";
        BFS(adjList, i, n+1);
        cout<<"\n";
    }
    return 0;
}

//-------------------------------------------------------

vector BFS(vector<vector>& listaDeAdy, int nodoInicial) {
vector distancia(listaDeAdy.size(), -1);
queue cola;
cola.push(nodoInicial);
distancia[nodoInicial] = 0;
while (!cola.empty()) {
    int nodoActual = cola.front();
    cola.pop();

    for (int nodoVecino : listaDeAdy[nodoActual]) {
        if (distancia[nodoVecino] == -1) {
            cola.push(nodoVecino);
            distancia[nodoVecino] = distancia[nodoActual] + 1;
        }
    }
}

return distancia;
}

int main(int argc, char **argv){
string test_in = argv[1];
vector<vector> lista = procesarEntrada(test_in);
for (int i = 0; i < lista.size(); i++) {
    vector<int> distancia = BFS(lista, i);
    cout << "Distancias para el nodo " << (i+1) << ":" << endl;
    for (int j = 0; j < distancia.size(); j++) {
        cout << "Nodo " << (j+1) << ": " << distancia[j] << endl;
    }
    cout << endl;
}

return 0;
}