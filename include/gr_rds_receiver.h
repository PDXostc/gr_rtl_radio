// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_RDSRECEIVER_H
#define INCLUDED_GR_RUNTIME_RDSRECEIVER_H

#include <gnuradio/analog/api.h>

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

private:
  rds_receiver(void);

  void init_block();
};

} // namespace analog
} // namespace gr

#endif