// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_RDSPARSER_H
#define INCLUDED_GR_RUNTIME_RDSPARSER_H

#include <gnuradio/analog/api.h>
#include <gnuradio/filter/freq_xlating_fir_filter_fcc.h>
#include <gnuradio/filter/pfb_arb_resampler_ccc.h>
#include <gnuradio/filter/fir_filter_ccc.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/blocks/keep_one_in_n.h>
#include <gnuradio/digital/diff_decoder_bb.h>

#include "gr_psk_demod.h"

namespace gr
{
namespace analog
{

class ANALOG_API rds_parser : public hier_block2
{
public:
  typedef boost::shared_ptr<rds_parser> sptr;

  static sptr make();

  ~rds_parser();

private:
  rds_parser(void);
  //rds_parser();

  void init_block();

  gr::filter::freq_xlating_fir_filter_fcc::sptr filter;
  gr::filter::pfb_arb_resampler_ccc::sptr resampler;
  gr::filter::fir_filter_ccc::sptr rrc_filter;
  //gr::filter::firdes::root_raised_cosine rrc_des;
  //gr::blocks::keep_one_in_n koin;
  //gr::digital::diff_decoder_bb diff_decoder;
};

} // namespace analog
} // namespace gr

#endif