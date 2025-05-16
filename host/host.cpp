#include <iostream>
#include <cstring>
#include <chrono>  // For timing
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>

#define IP_START 0x1
#define IP_DONE  0x2
#define IP_IDLE  0x4

#define INPUT_OFFSET  0x10
#define OUTPUT_OFFSET 0x1c
#define SCALAR_OFFSET 0x28

#define H 32
#define W 32
#define DATA_SIZE (H * W)

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <xclbin>" << std::endl;
        return 1;
    }

    std::string binaryFile = argv[1];
    auto xclbin = xrt::xclbin(binaryFile);
    auto device = xrt::device(0);
    auto uuid = device.load_xclbin(xclbin);

    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    std::cout << "Creating kernel handle...\n";
    auto scalar_ip = xrt::ip(device, uuid, "scalar_mult");

    // Allocate buffers
    auto input_bo = xrt::bo(device, vector_size_bytes, 1);
    auto output_bo = xrt::bo(device, vector_size_bytes, 1);

    auto input_ptr = input_bo.map<int*>();
    auto output_ptr = output_bo.map<int*>();

    // Initialize input and output
    for (int i = 0; i < DATA_SIZE; ++i)
        input_ptr[i] = i;

    int scalar = 5;
    for (int i = 0; i < DATA_SIZE; ++i)
        output_ptr[i] = 0;

    // Print input data
    std::cout << "Input data:\n";
    for (int i = 0; i < DATA_SIZE; ++i) {
        std::cout << input_ptr[i] << " ";
        if ((i + 1) % W == 0) std::cout << "\n";
    }
    std::cout << std::endl;

    input_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    output_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    uint64_t input_addr = input_bo.address();
    uint64_t output_addr = output_bo.address();

    // Write addresses and scalar to control registers
    scalar_ip.write_register(INPUT_OFFSET, input_addr);
    scalar_ip.write_register(INPUT_OFFSET + 4, input_addr >> 32);

    scalar_ip.write_register(OUTPUT_OFFSET, output_addr);
    scalar_ip.write_register(OUTPUT_OFFSET + 4, output_addr >> 32);

    scalar_ip.write_register(SCALAR_OFFSET, scalar);

    // Start the timer
    auto start_time = std::chrono::high_resolution_clock::now();

    // Start the kernel
    scalar_ip.write_register(0x00, IP_START);

    // Wait for completion
    while ((scalar_ip.read_register(0x00) & IP_DONE) != IP_DONE) {
        // Optional: comment out to avoid printing during timing
        // std::cout << "Waiting for kernel to finish...\n";
    }

    // Stop the timer
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Kernel finished in " << elapsed.count() * 1000.0 << " ms\n";

    // Calculate throughput in MB/s (input + output data transferred)
    double throughput = (vector_size_bytes * 2) / (elapsed.count() * 1024 * 1024);
    std::cout << "Throughput: " << throughput << " MB/s\n";

    // Get results back
    output_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Print output data
    std::cout << "Output data:\n";
    for (int i = 0; i < DATA_SIZE; ++i) {
        std::cout << output_ptr[i] << " ";
        if ((i + 1) % W == 0) std::cout << "\n";
    }
    std::cout << std::endl;

    // Validate
    bool success = true;
    for (int i = 0; i < DATA_SIZE; ++i) {
        if (output_ptr[i] != input_ptr[i] * scalar) {
            std::cerr << "Mismatch at index " << i
                      << ": " << output_ptr[i] << " != " << input_ptr[i] * scalar << "\n";
            success = false;
            break;
        }
    }

    if (success)
        std::cout << "TEST PASSED\n";
    else
        std::cerr << "TEST FAILED\n";

    return success ? 0 : 1;
}
