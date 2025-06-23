#include "memory_listener.h"

int main() {

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
    return 0;
}
