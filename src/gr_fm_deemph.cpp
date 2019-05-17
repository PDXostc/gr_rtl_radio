// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#include "gr_fm_deemph.h"

namespace gr
{
namespace analog
{

fm_deemph::~fm_deemph()
{
}

fm_deemph::sptr fm_deemph::make(float audio_rate)
{
    return gnuradio::get_initial_sptr(new fm_deemph(audio_rate));
}

fm_deemph::fm_deemph(float audio_rate)
    : hier_block2(
        "fm_deemph",
        io_signature::make(1, 1, sizeof(float)),
        io_signature::make(1, 1, sizeof(float)))
{
    double tau = 75.0e-6;
    double w_c = 1.0 / tau;
    double w_ca = 2.0 * audio_rate * tan(w_c / (2.0 * audio_rate));
    double k = -w_ca / (2.0 * audio_rate);
    double z1 = -1.0;
    double p1 = (1.0 + k) / (1.0 - k);
    double b0 = -k / (1.0 - k);

    std::vector<double> ataps;
    std::vector<double> btaps;

    btaps.push_back(b0 * 1.0);
    btaps.push_back(b0 * -z1);

    ataps.push_back(1.0);
    ataps.push_back(-p1);

    iirfilt = gr::filter::iir_filter_ffd::make(
        btaps,
        ataps,
        false);

    connect(self(), 0, iirfilt, 0);

    connect(iirfilt, 0, self(), 0);
}

} // namespace analog
} // namespace gr