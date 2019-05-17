// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_WFMRCV_H
#define INCLUDED_GR_RUNTIME_WFMRCV_H

#include <gnuradio/analog/api.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/analog/quadrature_demod_cf.h>
#include <gnuradio/filter/fir_filter_fff.h>

#include "gr_fm_deemph.h" 

namespace gr
{
namespace analog
{

class ANALOG_API wfmrcv : public hier_block2
{
public:
    typedef boost::shared_ptr<wfmrcv> sptr;

    static sptr make(
        float quad_rate,
        float audio_decimation);

    ~wfmrcv();

private:
    wfmrcv(void) {}
    wfmrcv(
        float quad_rate,
        float audio_decimation);

    void init_block(
        float quad_rate,
        float audio_decimation);

    quadrature_demod_cf::sptr fm_demod;
    fm_deemph::sptr deemph;
    std::vector<float> audio_coeffs;
    gr::filter::fir_filter_fff::sptr audio_filter;
};

} // namespace analog
} // namespace gr

#endif