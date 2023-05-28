#include <cstdio>
#include <random>
#include <functional>
#include <iostream>
using namespace std;

typedef pair<int, int> edge;

int main(int argc, char** argv) {
  int n = argc > 1 ? atoi(argv[1]) : 1000;
  double p = 0.99; // probability of an edge
  bernoulli_distribution d(p);
  uniform_int_distribution<int> w(1, 100);

  default_random_engine gen;
  auto f = bind(d, ref(gen));
  auto g = bind(w, ref(gen));

  vector<vector<edge>> A(n);
  int m = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (f()) {
        int c = g();
        A[i].push_back(make_pair(c, j));
        A[j].push_back(make_pair(c, i));
        ++m;
      }
    }
  }

  cout << n << " " << m << endl;
  int i = 0;
  for (const auto& v : A) {
    for (const auto& e : v) {
      cout << e.first << " " << e.second << " " << i << endl;
    }
    ++i;
  }
}