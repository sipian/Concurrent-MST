#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
  int n = 10;
  cout << "Enter no of vertices :: ";
  cin >> n;
  ofstream fout;
  fout.open("complete.txt");
  fout << n << "\n" << ((n*(n-1))/2) << "\n";

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; ++j)
    {
      fout << i << " " << j << " " << rand()%5000 << "\n";
    }
  }


  /* code */
  return 0;
}