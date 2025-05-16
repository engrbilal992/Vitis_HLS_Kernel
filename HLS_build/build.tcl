open_project -reset scalar_mult_prj
set_top scalar_mult
add_files -tb ../src/scalar_mult_tb.cpp -cflags "-I../src"
add_files     ../src/scalar_mult.cpp -cflags "-I../src"
add_files     ../src/params.h -cflags "-I../src"
open_solution "solution"
set_part {xcu200-fsgd2104-2-e}
create_clock -period 3.33333333 -name default
config_flow -target vitis
csim_design
csynth_design
cosim_design -trace_level all -enable_dataflow_profiling
export_design -format xo
exit