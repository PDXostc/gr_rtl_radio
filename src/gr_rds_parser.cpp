// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

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
    
}

rds_parser::rds_parser()
    : hier_block2(
          "rds_parser",
          io_signature::make(1, 1, sizeof(gr_complex)),
          io_signature::make(1, 1, 0))
{
    init_block();
}

rds_parser::sptr rds_parser::make()
{
    return gnuradio::get_initial_sptr(new rds_parser());
}

} // namespace analog
} // namespace gr
