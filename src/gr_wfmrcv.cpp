// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#include "gr_wfmrcv.h"
#include <gnuradio/filter/firdes.h>

namespace gr
{
namespace analog
{

wfmrcv::~wfmrcv()
{
}

void wfmrcv::init_block(float quad_rate, float audio_decimation)
{
    float max_dev = 75.0e3;
    float fm_demod_gain = quad_rate / (2 * M_PI * max_dev);
    float audio_rate = quad_rate / audio_decimation;

    fm_demod = quadrature_demod_cf::make(fm_demod_gain);
    deemph = fm_deemph::make(audio_rate);

    double width = audio_rate / 32.0;
    audio_coeffs = filter::firdes::low_pass(
        1.0,
        quad_rate,
        audio_rate / 2.0 - width,
        width,
        filter::firdes::WIN_HAMMING);

    audio_filter = filter::fir_filter_fff::make(
      audio_decimation,
      audio_coeffs
    );

    connect(self(), 0, fm_demod, 0);
    connect(fm_demod, 0, audio_filter, 0);
    connect(audio_filter, 0, deemph, 0);
    connect(deemph, 0, self(), 0);
}

wfmrcv::wfmrcv(float quad_rate, float audio_decimation)
    : hier_block2(
          "wfmrcv",
          io_signature::make(1, 1, sizeof(gr_complex)),
          io_signature::make(1, 1, sizeof(float)))
{
    init_block(quad_rate, audio_decimation);
}

wfmrcv::sptr wfmrcv::make(float quad_rate, float audio_decimation)
{
    return gnuradio::get_initial_sptr(new wfmrcv(quad_rate, audio_decimation));
}

} // namespace analog
} // namespace gr