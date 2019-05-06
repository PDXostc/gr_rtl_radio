// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#include "gr_rds_sink.h"

namespace gr
{
namespace rds
{

rds_sink::~rds_sink()
{
}

const std::string rds_sink::get_latest_packet()
{
    return _latest_packet;
}

void rds_sink::rds_rcv(pmt::pmt_t msg)
{
    printf("RECEIVED RDS");
    pmt::print(msg);
    if (pmt::is_tuple(msg)) {
        long msg_type = pmt::to_long(pmt::tuple_ref(msg, 0));
        _latest_packet = pmt::symbol_to_string(pmt::tuple_ref(msg, 1));
    }
}

void rds_sink::init_block()
{
    message_port_register_in(pmt::mp("in"));
    set_msg_handler(pmt::mp("in"), boost::bind(&rds_sink::rds_rcv, this, _1));
    printf ("RDS SINK INITIALIZED!\n");
    //message_port_register_out(pmt::mp("out"));
}

rds_sink::rds_sink()
    : gr::block(
          "rds_receiver",
          io_signature::make(0, 0, 0),
          io_signature::make(0, 0, 0))
{
    init_block();
}

rds_sink::sptr rds_sink::make()
{
    return gnuradio::get_initial_sptr(new rds_sink());
}

} // namespace rds
} // namespace gr
