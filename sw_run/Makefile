CXX:=g++
######### Check the version of gcc available and select
ifneq ($(shell expr $(shell g++ -dumpversion) \>= 5), 1)
	ifndef XILINX_VIVADO
		$(error [ERROR]: g++ version older. Please use 5.0 or above.)
	else
		CXX := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/bin/g++
		$(warning [WARNING]: g++ version is older. using g++ provided by the Vitis tool : $(CXX))
	endif
endif

APP = scalar_mult.exe

# ---------------------------------------------------------------------------------------------------

SRC = /media/muhammadbilal/E6129CBB129C9263/vitis_kernel_flow_feature_tutorials/my_practice/src

APP_SOURCE_FILES += ${SRC}/scalar_mult.cpp
APP_SOURCE_FILES += ${SRC}/scalar_mult_tb.cpp
APP_HEADER_FILES += ${SRC}/*.h

APP_COMP_OPTIONS += -std=c++14
APP_LINK_OPTIONS += -lpthread -lrt -lstdc++ -fopenmp

# ---------------------------------------------------------------------------------------------------

# Compile and build the host application (.exe file)

${APP}: ${APP_SOURCE_FILES} ${APP_HEADER_FILES}
		$(CXX) -O3 -o $@ ${APP_SOURCE_FILES} ${APP_COMP_OPTIONS} ${APP_LINK_OPTIONS}

app: ${APP}

clean:
	rm -rf ${APP} emconfig.json *csv vitis* xclbin.run_summary xrt.ini *log