#include <iostream>
using namespace std;
typedef struct name
{
  int a;
} name;

void func(name &x) {
  x.a = 5;
}
int main(int argc, char const *argv[])
{
  name obj;
  obj.a = 10;
  cout << obj.a << "\n";
  func(obj);
  cout << obj.a << "\n";
  return 0;
}