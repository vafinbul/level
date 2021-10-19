#include <iostream>
using namespace std;

int main() {
    #pragma omp parallel 
    cout << "AAA" << endl;
    return 0;
}