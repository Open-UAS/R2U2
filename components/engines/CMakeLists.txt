idf_component_register(SRCS "aux_info.c" "engine_booleanizer.c" "engines.c" "engine_mltl.c"
                    REQUIRES "memory" "instructions" "internals"
                    INCLUDE_DIRS "include")