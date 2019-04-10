// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#ifdef __cplusplus
extern "C" {
#endif

// Types of audio sinks that the tuner will send samples to
typedef enum {
    SINK_ALSA,
    SINK_WAV,
    SINK_UDP
} audio_sink_t;

// Opaque context to pass to C
typedef struct rtl_ctx rtl_ctx_t;

rtl_ctx_t* rtl_create_tuner(audio_sink_t sink);
void rtl_destroy_tuner(rtl_ctx_t* this_tuner);

void rtl_start_fm(rtl_ctx_t* this_tuner);
void rtl_stop_fm(rtl_ctx_t* this_tuner);

unsigned int rtl_get_fm_stations(rtl_ctx_t* this_tuner, double* stations_out);

void rtl_set_fm(rtl_ctx_t* this_tuner, double freq);
double rtl_get_fm(rtl_ctx_t* this_tuner);


#ifdef __cplusplus
}
#endif
