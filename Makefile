# Compiler check
CXX := g++
ifeq ($(shell expr $(shell $(CXX) -dumpversion) \>= 5), 0)
    ifndef XILINX_VIVADO
        $(error [ERROR]: g++ version too old. Please use g++ 5.0 or above.)
    else
        CXX := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/bin/g++
        $(warning [WARNING]: g++ version is older. Using Vitis-provided compiler: $(CXX))
    endif
endif

# Platform
PLATFORM := /opt/xilinx/platforms/xilinx_u200_gen3x16_xdma_2_202110_1/xilinx_u200_gen3x16_xdma_2_202110_1.xpfm
TARGET := hw_emu

# Names
APP := scalar_mult.exe
XO := HLS_build/scalar_mult_prj/solution/impl/export.xo
XCLBIN := xclbin/fpgabinary.$(TARGET).xclbin

# Paths
SRC_DIR := src
HOST_DIR := host
XCLBIN_DIR := xclbin
BUILD_TCL := HLS_build/build.tcl

# XRT paths
XRT_INCLUDE := /opt/xilinx/xrt/include
XRT_LIB := /opt/xilinx/xrt/lib

# Source and flags
APP_SOURCE_FILES := $(wildcard $(HOST_DIR)/*.cpp)
APP_HEADER_FILES := $(wildcard $(SRC_DIR)/*.h)
APP_COMP_OPTIONS := -std=c++14 -I$(XRT_INCLUDE)
APP_LINK_OPTIONS := -lpthread -lrt -lstdc++ -fopenmp -L$(XRT_LIB) -lxrt_coreutil

# Targets
all: emconfig.json $(XO) $(XCLBIN) $(APP)

$(XO):
	cd HLS_build && vitis_hls -f build.tcl

$(XCLBIN): $(XO)
	mkdir -p $(XCLBIN_DIR)
	v++ -l --platform $(PLATFORM) --target $(TARGET) --save-temps \
		--temp_dir temp_dir --log_dir log_dir \
		--kernel scalar_mult -o $@ $(XO)

$(APP): $(APP_SOURCE_FILES) $(APP_HEADER_FILES)
	$(CXX) -O3 -o $@ $(APP_SOURCE_FILES) $(APP_COMP_OPTIONS) $(APP_LINK_OPTIONS)

emconfig.json:
	emconfigutil --platform $(PLATFORM) --od .

run: all
	XCL_EMULATION_MODE=$(TARGET) ./$(APP) $(XCLBIN)

clean:
	rm -rf $(APP) emconfig.json *.log *.jou *.wdb .Xil *.csv vitis* *.summary *.ini *.ext_metadata.json\
		$(XCLBIN_DIR) temp_dir log_dir
