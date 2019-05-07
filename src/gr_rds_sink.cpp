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

void rds_sink::reset()
{
    _curr_station.clear();
    _curr_station_type.clear();
}

const std::string rds_sink::get_curr_station()
{
    return _curr_station;
}

const std::string rds_sink::get_curr_station_type()
{
    return _curr_station_type;
}

// NOTE: this formula is U.S.A.-sepcific, and it does not work in every case (there are a number of exceptions)
// In reality, we would need to leverage some sort of database to reliably extract the callsign from the PI field.
// We may be able to use the RT+ field for this info instead, but gr-rds doesn't currently parse it.
std::string convert_to_callsign(const std::string& picode_str)
{
    int picode = std::stoi(picode_str, 0, 16);
    int code;
    std::string csign = "";
    if (picode > 4095 && picode < 39247) {
        if (picode > 21671) {
            csign += 'W';
            code = picode - 21672;
        }
        else {
            csign += 'K';
            code = picode - 4096;
        }
        int call2 = code / 676;
        code = code - (676 * call2);
        int call3 = code / 26;
        int call4 = code - (26 * call3);
        csign += char(call2 + 65);
        csign += char(call3 + 65);
        csign += char(call4 + 65);
    }
    return csign;
}

// TODO: there are a lot more RDS fields that we could access here
void rds_sink::rds_rcv(pmt::pmt_t msg)
{
    //printf("===RECEIVED RDS===\n");
    //pmt::print(msg);
    //printf("===END RECEIVED RDS===\n");
    if (pmt::is_tuple(msg)) {
        long msg_type = pmt::to_long(pmt::tuple_ref(msg, 0));
        std::string msg_val = pmt::symbol_to_string(pmt::tuple_ref(msg, 1));
        switch(msg_type) {
            case 0: // PI - Program identifier
                _curr_station = convert_to_callsign(msg_val);
                break;
            case 2: // PTY - Program type
                _curr_station_type = msg_val;
                break;
        }
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
