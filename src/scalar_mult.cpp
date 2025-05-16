#include "params.h"

extern "C" {
    void scalar_mult (  
                        int* data_input, 
                        int* data_output, 
                        int scalar) 
    {
    
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=data_input max_read_burst_length = 256 depth=1024
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=data_output max_write_burst_length = 256 depth=1024

#pragma HLS INTERFACE s_axilite port=data_input  bundle=control
#pragma HLS INTERFACE s_axilite port=data_output  bundle=control

#pragma HLS INTERFACE s_axilite port=scalar  bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                #pragma HLS UNROLL
                data_output[(i * W) + j] = data_input [(i * W) + j] * scalar;
            }
        }
    }
}