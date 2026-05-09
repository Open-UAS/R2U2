#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "r2u2.h"
#include "csv_trace.h"

void app_main(void) {
    r2u2_csv_reader_t r2u2_csv_reader = {0};

    int spec_file = -1; // stays as -1 if there's an error opening the file
    struct stat fd_stat;
    r2u2_status_t err_cond;

    uint8_t* spec;

    spec_file = open("spec.bin", O_RDONLY, 0);

    if( fstat( spec_file, &fd_stat ) != 0 ) {
    perror("Error reading specification file");
    }

    spec = (uint8_t*)malloc((size_t)(fd_stat.st_size));
    //  if (spec == NULL) {
    //  // Handle allocation failure
    //  }

    lseek(spec_file, 0, SEEK_SET);

    // read buffer to spec
    ssize_t bytes_read = read(spec_file, spec, (size_t)fd_stat.st_size);
    // if (bytes_read != fd_stat.st_size) {
    //     // Handle partial read or error
    // }

    if (close(spec_file) != 0) {
    // This isn't a fatal error, just warn
    perror("Spec file did not close cleanly");
    }

    r2u2_monitor_t r2u2_monitor = R2U2_DEFAULT_MONITOR;
    r2u2_load_specification(spec, &r2u2_monitor);

    free(spec);

    r2u2_monitor.out_file = stdout;
    if(r2u2_monitor.out_file == NULL) {
    perror("Cannot open output log");
    }

    r2u2_csv_reader.input_file = fopen("simple.csv", "r");

    do {
    err_cond = r2u2_csv_load_next_signals(&r2u2_csv_reader, &r2u2_monitor);

    if ((err_cond != R2U2_OK)) break;

    err_cond = r2u2_step(&r2u2_monitor);

    } while (err_cond == R2U2_OK);

    if (err_cond == R2U2_END_OF_TRACE) {
    // Traces are allowed to end, exit cleanly
    err_cond = R2U2_OK;
    }

    // Cleanup
    if (fclose(r2u2_monitor.out_file) != 0) {
    // We didn't close the output file sucessfully
    // handling this is out of scope but we should notify the user
    perror("Output file did not close cleanly");
    }

    if (err_cond != R2U2_OK) {
    /* Prints R2U2 Status string if built with debugging enabled */
    R2U2_DEBUG_PRINT("%s\n", r2u2_status_string(err_cond));
    }

    printf("%zd\n",bytes_read);
}
