#ifndef PARAMS
#define PARAMS

#define H 32    // Heigth 
#define W 32    // Width

// Function Prototye
extern "C" {
    void scalar_mult (  
                        int* data_input, 
                        int* data_output, 
                        int scalar
                    ); 
            }

#endif