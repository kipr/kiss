execute_process(COMMAND uname -a OUTPUT_VARIABLE UNAME_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE)
file(WRITE build_defines.h "
  #define UNAME \"${UNAME_OUTPUT}\"
  #define 
")
