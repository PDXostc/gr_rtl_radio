// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Jason Anderson (jander10@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_FMDEEMPH_H
#define INCLUDED_GR_RUNTIME_FMDEEMPH_H

#include <gnuradio/analog/api.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/filter/iir_filter_ffd.h>

namespace gr
{
namespace analog
{

class ANALOG_API fm_deemph : public hier_block2
{
  public:
    typedef boost::shared_ptr<fm_deemph> sptr;

    static sptr make(float audio_rate);

    ~fm_deemph();

  private:
    fm_deemph(void) {}
    fm_deemph(float audio_rate);

    gr::filter::iir_filter_ffd::sptr iirfilt;

};

} // namespace analog
} // namespace gr

#endif