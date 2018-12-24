#include <iostream>

using namespace std;

int main(int nArg, char* args[]) {
    for (int i = 0; i < nArg; ++i) {
        cout << args[i] << endl;
    }

    return 0;
}
