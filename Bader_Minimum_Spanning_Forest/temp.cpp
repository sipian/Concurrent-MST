
#include <bits/stdc++.h>

using namespace std;

//deletes all duplicates of edge 'e' in the vector 'v'
void deleteAll(vector<int> &v, int e) {
  auto it = v.begin();
  while (it != v.end()) {
    if (*it == e) {
      *it = v.back();
      v.pop_back();
      continue;
    }
    it++;
  }
}

int main(int argc, char const *argv[])
{
  int array[] = {1, 2, 1, 2, 2, 3, 3, 3,  2, 2, 6, 9, 2, 1};
  std::vector<int> v (array, array + sizeof(array)/sizeof(array[0]));
  deleteAll(v, 2);
  for (auto it : v) {
    cout << it << " , ";
  }
  return 0;
}