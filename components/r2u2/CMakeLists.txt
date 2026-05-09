idf_component_register(SRCS "lib.c"
                    REQUIRES "internals" "engines"
                    PRIV_REQUIRES "csv_trace"
                    INCLUDE_DIRS "include")