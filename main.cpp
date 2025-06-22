#include "memory_listener.h"

class Test {
public:
    Test(int, long, bool) {
        return;
    }
};


int main() {
    Test* hello = new Test(0, 5, false);
    delete hello;
    int *why = new int[5];
    delete[] why;
    return 0;
}
