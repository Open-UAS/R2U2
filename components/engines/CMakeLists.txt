idf_component_register(SRCS "aux_info.c" "booleanizer.c" "engines.c" "mltl.c"
                    REQUIRES "memory" "instructions" "internals"
                    INCLUDE_DIRS "include")