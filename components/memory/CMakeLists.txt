idf_component_register(SRCS "monitor.c" "shared_connection_queue.c"
                    REQUIRES "internals" "instructions"
                    INCLUDE_DIRS "include")