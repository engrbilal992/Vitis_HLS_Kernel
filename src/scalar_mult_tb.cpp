#include <iostream>
#include "params.h"

using namespace std;

int main () {
    
    // Data input and output Arrays
    int data_in[H * W];
    int data_out[H * W];
    int sca;

    // Initializer for Random Input image
    int init;
    cout << "--------------------------Enter Input Initializer--------------------------" << endl;
    cin >> init;
    cout << "--------------------------Loaidng Input Data--------------------------" << endl;

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            data_in[(i * W) + j] = rand() % 256 * init;
        }
    }

    cout << "--------------------------Printing Input Data--------------------------" << endl;

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << data_in [(i * W) + j] << " ";
        }
        cout << endl;
    }

    cout << "--------------------------Enter Input Scalar--------------------------" << endl;
    cin >> sca;

    cout << "--------------------------Calling Kernel--------------------------" << endl;
    
    scalar_mult (data_in, data_out, sca);

    cout << "--------------------------Printing Output Data--------------------------" << endl;

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << data_out[(i * W) + j] << " ";
        }
        cout << endl;
    }

    bool success = true;
    
    cout << "--------------------------Verification--------------------------" << endl;

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int idx = i * W + j;
            int expected = data_in[idx] * sca;
            if (data_out[idx] != expected) {
                cout << "Mismatch at (" << i << ", " << j << "): ";
                cout << "Expected " << expected << ", Got " << data_out[idx] << endl;
                success = false;
            }
        }
    }

    if (success) {
        cout << "------------------------Test Passed!!!--------------------------" << endl;
    } else {
        cout << "------------------------Test Failed!!!--------------------------" << endl;
    }
}