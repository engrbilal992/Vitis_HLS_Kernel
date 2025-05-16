CXX = g++
CXXFLAGS = -g -std=c++17 -I$(XILINX_XRT)/include
LDFLAGS = -L$(XILINX_XRT)/lib -lxrt_core -lxrt_coreutil

SRC = host.cpp
APP = scalar_mult.exe

all: $(APP)

$(APP): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(APP)
