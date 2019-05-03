// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#include <gnuradio/filter/freq_xlating_fir_filter_fcf.h>
#include <gnuradio/filter/pfb_arb_resampler_ccf.h>
#include <gnuradio/filter/fir_filter_ccf.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/blocks/keep_one_in_n.h>
#include <gnuradio/digital/diff_decoder_bb.h>
#include <rds/decoder.h>
#include <rds/parser.h>

#include "gr_rds_parser.h"

namespace gr
{
namespace analog
{

rds_parser::~rds_parser()
{
}

void rds_parser::init_block()
{
    int decimation = 1;
    auto taps = filter::firdes::low_pass(2500.0, 250000, 2.6e3, 2e3, filter::firdes::WIN_HAMMING);
    double center_freq = 57e3;
    double sampling_freq = 250000;
    auto filt = gr::filter::freq_xlating_fir_filter_fcf::make(decimation, taps, center_freq, sampling_freq);

    float rate = 19000/250e3;
    unsigned int filter_size = 32;
    auto resampler = gr::filter::pfb_arb_resampler_ccf::make(rate, {}, filter_size);

    double gain = 1;
    double sampling_freq_rrc = 19000;
    double symbol_rate = 2375;
    double alpha = 0.35;
    int ntaps = 100;
    auto rrc_filt = gr::filter::firdes::root_raised_cosine(gain, sampling_freq_rrc, symbol_rate, alpha, ntaps);
    int decimation_fir = 2;
    auto fir_filt = gr::filter::fir_filter_ccf::make(decimation_fir, rrc_filt);

    struct gr::digital::psk_demod_params params;
    params.constellation_points = 2;
    params.differential = false;
    params.samples_per_symbol = 4;
    params.excess_bw = 0.35;
    params.freq_bw = 6.28/100.0;
    params.timing_bw = 6.28/100.0;
    params.phase_bw = 6.28/100.0;
    params.mod_code = gr::digital::mod_code::GRAY_CODE;
    auto psk_demod = gr::digital::psk_demod::make(params);

    auto keep_one = gr::blocks::keep_one_in_n::make(sizeof(char), 2);

    auto diff_decoder = gr::digital::diff_decoder_bb::make(2);

    auto rds_decoder = gr::rds::decoder::make(false, false);

    unsigned char pty_locale = 0; // 0 = Europe, 1 = North America
    auto rds_parser = gr::rds::parser::make(false, false, pty_locale);

    connect(self(), 0, filt, 0);
    connect(filt, 0, resampler, 0);
    connect(resampler, 0, fir_filt, 0);
    connect(fir_filt, 0, psk_demod, 0);
    connect(psk_demod, 0, keep_one, 0);
    connect(keep_one, 0, diff_decoder, 0);
    connect(diff_decoder, 0, rds_decoder, 0);
    connect(rds_decoder, 0, rds_parser, 0);
    connect(rds_parser, 0, self(), 0);
}

rds_parser::rds_parser()
    : hier_block2(
          "rds_parser",
          io_signature::make(1, 1, sizeof(gr_complex)),
          io_signature::make(0, 0, 0))
{
    init_block();
}

rds_parser::sptr rds_parser::make()
{
    return gnuradio::get_initial_sptr(new rds_parser());
}

} // namespace analog
} // namespace gr
