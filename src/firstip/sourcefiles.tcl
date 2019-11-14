# set name of top level function
set_top firstip

# get absolute path to visionaray
set include [file normalize ../../../include/]

# source files
add_files $src_dir/firstip.cpp -cflags "-std=c++11 -I${include}"

# testbench files
add_files -tb $src_dir/testbench.cpp -cflags "-std=c++11 -I${include}"
