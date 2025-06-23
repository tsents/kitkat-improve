#include "memory_listener.h"

class Test {
public:
    Test(int, long, bool) {
        return;
    }
};

int main() {
    Test* hello = new Test(0, 5, false);
    int* why = new int[5];

    int** arrTest = new int*[100];
    for (int i = 0; i < 100; i++) {
        arrTest[i] = new int(5);
    }
    delete arrTest[50];
    MemoryAction::printListSummery();
    for (int i = 0; i < 100; i++) {
        if (i != 50) {
            delete arrTest[i];
        }
    }
    delete[] arrTest;
    delete[] why;
    delete hello;
    return 0;
}
