#include <bits/stdc++.h>
// #include <time.h>

using namespace std;

int main() {
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<double> distr(1, 200);
  
  for(int i = 0; i < 200; i++) {
    cout << i+1 << " - " << setprecision(4) << distr(eng) << "\n";
  }

  return 0;
}