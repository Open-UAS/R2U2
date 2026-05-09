idf_component_register(SRCS "errors.c" "process_binary.c"
                    REQUIRES "memory" "internals" "engines"
                    INCLUDE_DIRS "include")