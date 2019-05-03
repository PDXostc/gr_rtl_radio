// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_RDSPARSER_H
#define INCLUDED_GR_RUNTIME_RDSPARSER_H

#include <gnuradio/analog/api.h>

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

  void init_block();
};

} // namespace analog
} // namespace gr

#endif