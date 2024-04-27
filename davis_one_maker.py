import os
#################################################
###########         CONSTANTS         ###########
#################################################
START_DAVIS_H = """#ifndef DAVIS_H_
#define DAVIS_H_

"""
END_DAVIS_H = """
#endif // DAVIS_H_"""

START_DAVIS_CPP = """#include "davis.h"

"""
START_NAME_SPACE_DAVIS = """namespace davis {
"""
END_NAME_SPACE_DAVIS = """
} // namespace davis end"""

START_NAME_SPACE = """namespace {
"""
END_NAME_SPACE = """
}// namespace end
"""
OUTPUT_H_FILE_NAME = "davis_one/davis.h"
OUTPUT_CPP_FILE_NAME = "davis_one/davis.cpp"
SRC_LIST_FILE = "davis_files.txt"
START_GRAB_INCLUDES = """#START_GRAB_TO_INCLUDES_LIST"""
STOP_GRAB_INCLUDES = """#STOP_GRAB_TO_INCLUDES_LIST"""
START_GRAB_DAVIS_NAMESPACE = """#START_GRAB_TO_DAVIS_NAMESPACE"""
STOP_GRAB_DAVIS_NAMESPACE = """#STOP_GRAB_TO_DAVIS_NAMESPACE"""
START_GRAB_NAMESPACE = """#START_GRAB_TO_NAMESPACE"""
STOP_GRAB_NAMESPACE = """#STOP_GRAB_TO_NAMESPACE"""

#################################################
###########         FUNCTIONS         ###########
#################################################
def delete_file_if_exists(file_name):
 if os.path.exists(file_name):
    os.remove(file_name)

def grab_namespace_code(namespace_text_start,
                        namespace_text_end,
                        start_grab_text,
                        stop_grab_text,
                        files):
  file_h.write(namespace_text_start)
  file_cpp.write(namespace_text_start)
  is_grab_davis = False
  for el in files:
       file_src = open(el).read().splitlines()
       for f in file_src:
         if start_grab_text in f:
            is_grab_davis = True
            continue
         if stop_grab_text in f:
            is_grab_davis = False
            continue
         if is_grab_davis:
            if '.h' in el:
              file_h.write(f + "\n")
            if '.cpp' in el:
              file_cpp.write(f + "\n")
  file_h.write(namespace_text_end)
  file_cpp.write(namespace_text_end)

print("""
########################################
########   DAVIS MAKER SCRIPT   ########
########################################
""")

includes_h_set = set("")
includes_cpp_set = set("")

delete_file_if_exists(OUTPUT_H_FILE_NAME)
delete_file_if_exists(OUTPUT_CPP_FILE_NAME)

file_h = open(OUTPUT_H_FILE_NAME, 'a')
file_h.write(START_DAVIS_H)
file_cpp = open(OUTPUT_CPP_FILE_NAME, 'a')
file_cpp.write(START_DAVIS_CPP)
is_grab_includes = False

file_src_list = open(SRC_LIST_FILE,'r').read().splitlines()
for el in file_src_list:
 #print(el)
 file_src = open(el).read().splitlines()
 for f in file_src:
  if START_GRAB_INCLUDES in f:
     is_grab_includes = True
     continue
  if STOP_GRAB_INCLUDES in f:
     is_grab_includes = False
     continue
  if is_grab_includes:
     if '.h' in el:
      includes_h_set.add(f)
      #print('h: -->',f)
     if '.cpp' in el:
      includes_cpp_set.add(f)
      #print('cpp: -->',f)


print("ALL H INCLUDES: ")
for hs in includes_h_set:
 print(hs)
 file_h.write(hs+"\n")

print("ALL CPP INCLUDES: ") 
for cpps in includes_cpp_set:
 print(cpps)
 file_cpp.write(cpps+"\n")

grab_namespace_code(START_NAME_SPACE,
                    END_NAME_SPACE,
                    START_GRAB_NAMESPACE,
                    STOP_GRAB_NAMESPACE,
                    file_src_list)

grab_namespace_code(START_NAME_SPACE_DAVIS,
                    END_NAME_SPACE_DAVIS,
                    START_GRAB_DAVIS_NAMESPACE,
                    STOP_GRAB_DAVIS_NAMESPACE,
                    file_src_list)


file_h.write(END_DAVIS_H) #DAVIS HEADER_GUARD END
file_h.close()
file_cpp.close()
