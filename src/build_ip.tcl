set target_ip_path  "[file normalize [lindex [lindex $argv 2] 0]]"
set src_dir         "[file normalize [lindex [lindex $argv 2] 1]]"
set build_dir       "[file normalize [lindex [lindex $argv 2] 2]]"
set target_board    "[lindex [lindex $argv 2] 3]"
set current_dir     [pwd]
set vivado_version  [version -short]

set destination     "[file normalize "$build_dir/$vivado_version/repo"]"
set working_dir     "[file normalize "$build_dir/$vivado_version/hls"]"
set csim            1
set exec_csim       1
set synth           1

if { $target_board == "arty" } {
    set target_part xc7a35ticsg324-1L
    set board_property digilentinc.com:arty:part0:1.1
} elseif { $target_board == "arty-a7-100" } {
    set target_part xc7a100tcsg324-1
    set board_property digilentinc.com:arty-a7-100:part0:1.0
} elseif { $target_board == "vc709" } {
    set target_part xc7vx690tffg1761-2
    set board_property xilinx.com:vc709:part0:1.7
} elseif { $target_board == "vcu118" } {
    set target_part xcvu9p-flga2104-2L-e
    set board_property xilinx.com:vcu118:part0:2.0
} else {
    puts "ERROR! Selected board '$target_board' is not supported."
    exit
}

puts "Sourcing IP specific settings.tcl"
source $target_ip_path/settings.tcl

file mkdir $working_dir
cd $working_dir
open_project $name

puts "Sourcing IP specific sourcefiles.tcl"
source $target_ip_path/sourcefiles.tcl

set solution "solution1_${clock}_32Bit"
open_solution -reset $solution
set_part $target_part
create_clock -period $clock -name default

#config_interface -m_axi_offset off
config_interface -m_axi_offset off -register_io off -m_axi_addr64

if { $csim == 1 } {
    if { $exec_csim == 1 } { 
        csim_design -compiler clang
    } else {
        csim_design -setup -compiler clang
    }
}

if { $synth == 1 } {
    csynth_design

    # export design
    export_design \
        -format ip_catalog \
        -description $solution-$target_part \
        -library $library \
        -vendor $vendor \
        -version $version_h.$version_l \
        -display_name $display_name

    # copy to repo
    puts "Copying IP ($vendor\_$library\_$name\_$version_h\_$version_l\_$clock\.zip) to repository ($destination/$name)"
    file mkdir $destination/$name
    file copy -force $working_dir/$name/$solution/impl/ip/$vendor\_$library\_$name\_$version_h\_$version_l\.zip $destination/$name/$vendor\_$library\_$name\_$version_l\_$version_h\_$clock\.zip
    puts "* please unpack for usage ($vendor\_$library\_$name\_$version_h\_$version_l\_$clock\.zip)"
}

cd $current_dir
exit
