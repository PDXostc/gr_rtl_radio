// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#ifdef __cplusplus
extern "C" {
#endif

#define STATION_NAME_MAX_LEN 10
#define STATION_GENRE_MAX_LEN 30

// Opaque context to pass to C
typedef struct rtl_ctx rtl_ctx_t;

typedef struct __attribute__((packed))
station_info {
    char name[STATION_NAME_MAX_LEN];
    char genre[STATION_GENRE_MAX_LEN];
    double frequency;
} station_info_t;

rtl_ctx_t* rtl_create_tuner();
void rtl_destroy_tuner(rtl_ctx_t* this_tuner);

void rtl_add_audio_sink(rtl_ctx_t* this_tuner, const char* device, int sampling_rate);
void rtl_add_wav_sink(rtl_ctx_t* this_tuner, const char* file_name, int sampling_rate);

void rtl_start_fm(rtl_ctx_t* this_tuner);
void rtl_stop_fm(rtl_ctx_t* this_tuner);

unsigned int rtl_get_fm_stations(rtl_ctx_t* this_tuner, station_info_t* stations_out);

void rtl_set_fm(rtl_ctx_t* this_tuner, double freq);
double rtl_get_fm(rtl_ctx_t* this_tuner);

#ifdef __cplusplus
}
#endif
