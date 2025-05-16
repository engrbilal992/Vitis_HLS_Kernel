# Scalar Multiplication FPGA Kernel hw_emu (Alveo U200/U250)

This project demonstrates a simple HLS kernel (`scalar_mult`) accelerated on an AMD Alveo U200 or U250 card using the Vitis unified software platform.

## 📁 Directory Structure

```
top_dir/
│
├── host/                       # Host-side C++ code (XRT-based)
│   └── host.cpp                # Loads xclbin, allocates buffers, runs kernel
│
├── src/                        # Kernel source code (C++ HLS)
│   ├── scalar_mult.cpp         # Main kernel logic
|   |── scalar_mult_tb.cpp      # HLS Test Bench
│   └── params.h                # Parameters (e.g., size, scalar factor)
│
├── HLS_build/                  # HLS kernel compilation
│   ├── build.tcl               # TCL script to build .xo from kernel
│   └── scalar_mult_prj/       # Auto-generated HLS project
│       └── solution/impl/export.xo
│
├── xclbin/                     # Compiled binary containers
│   └── fpgabinary.hw_emu.xclbin   # Hardware emulation binary
│
├── Makefile                    # Build automation
└── README.md                   # Project documentation
```

## ⚙️ Requirements

* **AMD Alveo U200/U250 cards**
* **Xilinx Vitis 2021.1 or Higher**
* **Xilinx XRT runtime**
* **Vitis HLS 2021.1 or Higher**
* Linux-based OS (Ubuntu recommended)

## 🛠️ Build Instructions

1. **Generate HLS Kernel (.xo)**

   Run the following in the `your top root Dir` root directory :

   ```bash
   vitis_hls -f HLS_build/build.tcl
   ```

2. **Link and Build `.xclbin`**

   ```bash
   vitis \
     -l --platform xilinx_u200_gen3x16_xdma_2_202110_1 \
     -t hw \
     --temp_dir ./_vitis_temp \
     --output_dir ./xclbin \
     -o fpgabinary.hw.xclbin \
     ./HLS_build/scalar_mult_prj/solution/impl/export.xo \
     ./host/host.cpp
   ```

   *(Replace platform name with your installed U200/U250 platform)*

3. **Run the Host Application**

   ```bash
   export XCL_EMULATION_MODE=hw
   ./a.out fpgabinary.hw.xclbin
   ```

## 🧠 Kernel Function

The `scalar_mult` kernel performs a scalar multiplication on a 1024-element `int` array. Each element is multiplied by a fixed scalar defined in `params.h`.

```cpp
#define DATA_SIZE 1024
#define SCALAR 5
```

## 📌 Sample Output

```txt
Creating kernel handle...
Input data:
0 1 2 3 4 5 6 7 8 9 ... 1022 1023 

Kernel finished in (time) ms
Throughput: __ MB/s
Output data:
0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 ... 5115
```

Each output is the input multiplied by 5 (e.g., `input[i] * 5`). Change the Host Apllication to generate you own data, this is just an example.

## ✅ Expected Behavior

| Input Value | Output Value |
| ----------- | ------------ |
| 0           | 0            |
| 1           | 5            |
| 2           | 10           |
| 3           | 15           |
| ...         | ...          |
| 1023        | 5115         |

## Acknowledgment

If you use this work in academic research, commercial projects, or any derivative work,  
**please give credit to the original author**:

```
Muhammad Bilal Sajid  
Email: bilalsajid695@gmail.com  
GitHub: https://github.com/engrbilal992/YOLOv5s-Ghost-Inference-in-Cpp-from-scratch  
Contact: +92 314 5844461
```

Your acknowledgment is genuinely appreciated!

---
