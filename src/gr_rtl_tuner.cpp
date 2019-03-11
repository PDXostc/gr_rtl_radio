// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#include "gr_rtl_tuner.h"

#include <chrono>
#include <ctime>

#include <thread>
#include <numeric>

#include "gnuradio/top_block.h"
#include "osmosdr/source.h"
#include "gnuradio/filter/rational_resampler_base_ccc.h"
#include "gnuradio/filter/rational_resampler_base_fff.h"
#include "gnuradio/filter/fir_filter_ccf.h"
#include "gnuradio/filter/firdes.h"
#include "gnuradio/audio/sink.h"
#include "gnuradio/hier_block2.h"
#include "gnuradio/gr_complex.h"
#include "gnuradio/analog/quadrature_demod_cf.h"
#include "gnuradio/filter/iir_filter_ffd.h"
#include "gnuradio/filter/fir_filter_fff.h"
#include "gnuradio/blocks/wavfile_source.h"
#include "gr_wfmrcv.h"

//std::vector<float>

void create_fm_device()
{
    int mltpl = 1e6;
    int volume = 20;
    int transition = 1e6;
    int samp_rate = 2e6;
    int quadrature = 500e3;
    double freq = 101.9;
    int cutoff = 100e3;
    int audio_dec = 10;
    int max_dev = 75e3;

    gr::top_block_sptr tb = gr::make_top_block("top");
    osmosdr::source::sptr rtlsrc = osmosdr::source::make("numchan=1 rtl=0");
    //rtlsrc->set_time_now(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), osmosdr::ALL_MBOARDS);
    rtlsrc->set_sample_rate(samp_rate);
    rtlsrc->set_center_freq(freq * 1e6);
    rtlsrc->set_freq_corr(0, 0);
    rtlsrc->set_dc_offset_mode(2, 0);
    rtlsrc->set_iq_balance_mode(1, 0);
    rtlsrc->set_gain_mode(false, 0);
    rtlsrc->set_gain(20, 0);
    rtlsrc->set_if_gain(20, 0);
    rtlsrc->set_bb_gain(20, 0);
    rtlsrc->set_antenna("", 0);
    rtlsrc->set_bandwidth(0, 0);

    int dec1 = int(samp_rate / quadrature);
    printf("dec1: %d \n", dec1);

    gr::filter::rational_resampler_base_ccc::sptr rresamp1;

    double d = 1.0;

    double inter = floor(1.0 / d);
    double deci = floor(double(dec1) / d);

    printf("inter: %f \n", inter);

    double fractional_bw = 0.4;
    double beta = 7.0;
    double halfband = 0.5;
    double rate = 1.0 / deci;
    double trans_width = 0.0;
    double mid_transition_band = 0.0;

    if (rate >= 1.0)
    {
        trans_width = halfband - fractional_bw;
        mid_transition_band = halfband - trans_width / 2.0;
        printf("mid_transition_band: %f \n", mid_transition_band);
    }
    else
    {
        trans_width = rate * (halfband - fractional_bw);
        printf("trans_width: %f \n", trans_width);
        mid_transition_band = rate * halfband - trans_width / 2.0;
        printf("mid_transition_band2: %f \n", mid_transition_band);
    }

    std::vector<float> taps1 = gr::filter::firdes::low_pass(
        (inter),
        (inter),
        (mid_transition_band),
        (trans_width),
        gr::filter::firdes::WIN_KAISER,
        (beta));

    std::vector<gr_complex> taps_comp;

    std::vector<float>::iterator iter = taps1.begin();
    for (; iter != taps1.end(); ++iter)
    {
        taps_comp.push_back(gr_complex(*iter));
    }

    rresamp1 = gr::filter::rational_resampler_base_ccc::make(
        1.0,
        dec1,
        taps_comp);

    int dec2 = int(quadrature / 1e3 / audio_dec);
    printf("dec2: %d \n", dec2);
    gr::filter::rational_resampler_base_fff::sptr rresamp0;

    d = 2.0;

    inter = floor(48.0 / d);
    printf("inter: %f \n", inter);

    deci = floor(dec2 / d);

    fractional_bw = 0.4;
    beta = 7.0;
    halfband = 0.5;
    rate = 1 / deci;
    trans_width = 0.0;
    mid_transition_band = 0.0;

    if (rate >= 1.0)
    {
        trans_width = halfband - fractional_bw;
        mid_transition_band = halfband - trans_width / 2.0;
        printf("mid_transition_band: %f \n", mid_transition_band);
    }
    else
    {
        trans_width = rate * (halfband - fractional_bw);
        printf("trans_width: %f \n", trans_width);
        mid_transition_band = rate * halfband - trans_width / 2.0;
        printf("mid_transition_band2: %f \n", mid_transition_band);
    }

    std::vector<float> taps = gr::filter::firdes::low_pass(
        (inter),
        (inter),
        float(mid_transition_band),
        float(trans_width),
        gr::filter::firdes::WIN_KAISER,
        float(beta));

    rresamp0 = gr::filter::rational_resampler_base_fff::make(
        48.0,
        dec2,
        taps);

    gr::filter::fir_filter_ccf::sptr lp0 = gr::filter::fir_filter_ccf::make(1,
                                                                            gr::filter::firdes::low_pass(
                                                                                1,
                                                                                samp_rate,
                                                                                cutoff,
                                                                                transition));

    gr::audio::sink::sptr audsink = gr::audio::sink::make(44100);

    gr::analog::wfmrcv::sptr wfm = gr::analog::wfmrcv::make(
      quadrature,
      audio_dec
    );


    gr::hier_block2_sptr wfmrcv = gr::make_hier_block2(
        "wfm_rcv",
        gr::io_signature::make(1, 1, sizeof(gr_complex)),
        gr::io_signature::make(1, 1, sizeof(float)));

    float fm_demod_gain = quadrature / (2 * M_PI * max_dev);
    float audio_rate = quadrature / audio_dec;

   

    /*gr::blocks::wavfile_source::sptr filesink = gr::blocks::wavfile_source::make(
        "/home/jlruser/yocto/qcom-linux-guest/apps_proc/audio/mm-audio/audio-listen/sva/res/raw/succeed.wav"
    );*/

    tb->connect(
        rtlsrc, 0,
        rresamp1, 0);

    tb->connect(
        rresamp1, 0,
        lp0, 0);

    tb->connect(
        lp0, 0,
        wfm, 0);

    tb->connect(
        wfm, 0,
        rresamp0, 0);

    tb->connect(
        rresamp0, 0,
        audsink, 0);

    tb->start();
    tb->wait();
}

void create_tuner()
{

    std::thread t1(create_fm_device);
    t1.detach();
}
