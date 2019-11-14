# set name of top level function
set_top secondip

# get absolute path to visionaray
set include [file normalize ../../../include/]

# source files
add_files $src_dir/secondip.cpp -cflags "-std=c++11 -I${include}"

# testbench files
add_files -tb $src_dir/testbench.cpp -cflags "-std=c++11 -I${include}"
