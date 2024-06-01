#!/usr/bin/python
import sys
import subprocess
import os
from pathlib import Path


if len(sys.argv) == 3 and sys.argv[1] == "script_fie":
    filename = sys.argv[2]
    base_name = filename.replace('.o','').replace('.','_').replace('/','_').replace('-','_')
    f = filename.replace('.o','').replace('.','_').replace('/','_').replace('-','_')
    code_name = Path(sys.argv[2]).stem 

    chunk_size = 16
    c_array = '\n'
    with open(filename, 'rb') as binary_file:
         while True:
             c_array += '\t'
             chunk = binary_file.read(chunk_size)
             if not chunk:
                    c_array += '\n'
                    break  
             c_array += ', '.join([f'0x{byte:02X}' for byte in chunk])
             if len(chunk) == chunk_size:
                c_array += ',\n'


    filename= filename.replace('.o','.c')

    with open(filename + '', 'w') as header_file:
        header_file.write(f"#ifndef " +  base_name + "_H\n#define " + base_name + "_H\n\n")
        header_file.write(f"#include \"luasrc.h\"\n\n")    
        header_file.write(f"static unsigned char binary_data[] = {{{c_array}}};\n\n")
        header_file.write(f"struct luasrc {f} = {{ .code = binary_data, .size = sizeof (binary_data), .name = \"{code_name}\" }};\n\n")
        header_file.write(f"#endif")


    command = 'echo \'#include "' +filename + '"\'   >> lua_fw.h'
    try:
        output = subprocess.check_output(command, shell=True, universal_newlines=True)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with error code {e.returncode}")
        print("Error output:")
        print(e.output)
elif sys.argv[1] == "joint_file":
    with open("joint_file.c", 'w') as header_file:
        header_file.write(f"#include \"luasrc.h\"\n\n") 
        for  i in range(2,len(sys.argv),1):
            base_name = sys.argv[i].replace('.c','').replace('.','_').replace('/','_').replace('-','_')            
            header_file.write(f"extern struct luasrc  {base_name}; \n")

        header_file.write(f"struct luasrc *luasrc [] = {{\n")
        for  i in range(2,len(sys.argv),1):
            base_name = sys.argv[i].replace('.c','').replace('.','_').replace('/','_').replace('-','_')
            header_file.write(f"&{base_name},\n")
        header_file.write(f"{0},")
 
        header_file.write(f"}};\n")
        header_file.write(f"struct luasrc ** luasrc_get() {{return luasrc;}} ;")
        
	


else:
    print(sys.argv[1])    
    print("Usage: python lua_fw_to_c <lua binary file>")
    print(sys.argv[2])
    


