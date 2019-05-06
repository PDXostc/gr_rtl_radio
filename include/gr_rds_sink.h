// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_RDSSINK_H
#define INCLUDED_GR_RUNTIME_RDSSINK_H

#include <rds/api.h>
#include <gnuradio/block.h>

namespace gr
{
namespace rds
{

class RDS_API rds_sink : public gr::block
{
public:
  typedef boost::shared_ptr<rds_sink> sptr;

  static sptr make();

  void rds_rcv(pmt::pmt_t msg);

  const std::string get_latest_packet();

  ~rds_sink();

private:
  rds_sink(void);

  void init_block();

  std::string _latest_packet;
};

} // namespace rds
} // namespace gr

#endif