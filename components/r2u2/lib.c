#include "internals/config.h"
#include "r2u2.h"
#include "internals/debug.h"
#include "internals/process_binary.h"
#include "engines/engines.h"
#include <string.h>

#if R2U2_DEBUG
FILE* r2u2_debug_fptr = NULL;
#endif

r2u2_status_t r2u2_csv_load_next_signals(r2u2_csv_reader_t* csv_reader, r2u2_monitor_t* monitor) {
  char* signal;
  uintptr_t i;

  // Read in next line of trace to internal buffer for processing
  if(fgets(csv_reader->in_buf, sizeof(csv_reader->in_buf), csv_reader->input_file) == NULL) return R2U2_END_OF_TRACE;

  // Skip header row, if it exists - note we only check for this on the first line
  if (monitor->time_stamp == 0 && csv_reader->in_buf[0] == '#') {
    if(fgets(csv_reader->in_buf, sizeof(csv_reader->in_buf), csv_reader->input_file) == NULL) return R2U2_END_OF_TRACE;
  }
  
  for(i = 0, signal = strtok(csv_reader->in_buf, ",\n"); signal; i++, signal = strtok(NULL, ",\n")) {
    // Follow the pointer to the signal vector, then assign the ith element
    // Note this is a pointer into the r2u2_csv_reader_t in_buf which must
    // stay in place while the signal vector is live

    //if the term starts with an '@' then store it in the monitor's time_stamp
    if (i == 0 && signal[0] == '@'){
      char* temp_var;
      char* timechar = strtok_r(signal+1, " ", &temp_var);
      uint32_t time_signature;
      if(sscanf(timechar,"%lu",&time_signature) != 1 ) return R2U2_END_OF_TRACE;
      (monitor->time_stamp) = time_signature;
      R2U2_DEBUG_PRINT("Event: %u\n",monitor->time_stamp);
      signal = strtok_r(NULL, " ", &temp_var);
    }
    r2u2_load_string_signal(monitor, i, signal);
  }

  return R2U2_OK;
}

r2u2_status_t r2u2_load_specification(uint8_t* spec, r2u2_monitor_t* monitor) {
    // Memory resets....
    r2u2_monitor_soft_reset(monitor); // NOTE: Does not reset SCQ arena.
    // Optional: Use the following hard reset instead to also reset SCQ arena, 
    // but the monitor MUST be R2U2_DEFAULT_MONITOR
    // r2u2_monitor_hard_reset(monitor);

    // Populate instruction table from binary spec in instruction memory
    if (r2u2_process_binary(monitor, spec) != R2U2_OK) {
      return R2U2_BAD_SPEC;
    }

    return R2U2_OK;
}

r2u2_status_t r2u2_step(r2u2_monitor_t* monitor){
  return r2u2_engine_step(monitor);
}

void r2u2_load_bool_signal(r2u2_monitor_t* monitor, size_t index, r2u2_bool value){
  if (monitor->bz_program_count.max_program_count == 0) {
    monitor->atomic_buffer[index] = value;
  } else {
    monitor->signal_vector[index].i = (r2u2_int) value;
  }
}

void r2u2_load_int_signal(r2u2_monitor_t* monitor, size_t index, r2u2_int value){
  if (monitor->bz_program_count.max_program_count == 0) {
    monitor->atomic_buffer[index] = value != 0;
  } else {
    monitor->signal_vector[index].i = value;
  }
}

void r2u2_load_float_signal(r2u2_monitor_t* monitor, size_t index, r2u2_float value){
  if (monitor->bz_program_count.max_program_count == 0) {
    monitor->atomic_buffer[index] = value >= R2U2_FLOAT_EPSILON || value <= -R2U2_FLOAT_EPSILON;
  } else {
    monitor->signal_vector[index].f = value;
  }
}

void r2u2_load_string_signal(r2u2_monitor_t* monitor, size_t index, char* value){
  if (monitor->bz_program_count.max_program_count == 0) {
    monitor->atomic_buffer[index] = (strcmp(value,"0") != 0);
  } else {
    //Note that to be interpreted as a float, the string value must include a decimal point
    if (strchr(value, '.') != NULL){
      sscanf(value, "%lf", &(monitor->signal_vector[index].f));
    } else {
      sscanf(value, "%ld", &(monitor->signal_vector[index].i));
    }
  }
}
