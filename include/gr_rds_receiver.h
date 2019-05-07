// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_RDSRECEIVER_H
#define INCLUDED_GR_RUNTIME_RDSRECEIVER_H

#include <gnuradio/analog/api.h>

// FOR DEBUGGING
#include "gnuradio/analog/probe_avg_mag_sqrd_c.h"
#include "gnuradio/blocks/message_debug.h"

#include "gr_psk_demod.h"
#include "gr_rds_sink.h"

namespace gr
{
namespace analog
{

class ANALOG_API rds_receiver : public hier_block2
{
public:
  typedef boost::shared_ptr<rds_receiver> sptr;

  static sptr make();

  ~rds_receiver();

  gr::rds::rds_sink::sptr rds_sink;

  // FOR DEBUGGING
  gr::analog::probe_avg_mag_sqrd_c::sptr avg_magnitude_c;
  gr::digital::psk_demod::sptr psk_demod;
  gr::blocks::message_debug::sptr msg;

private:
  rds_receiver(void);

  void init_block();
};

} // namespace analog
} // namespace gr

#endif