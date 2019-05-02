// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#include "gr_psk_demod.h"

namespace gr
{
namespace digital
{

psk_demod::~psk_demod()
{
}

void psk_demod::init_block(psk_demod_params_t params)
{
    
}

psk_demod::psk_demod(psk_demod_params_t params)
    : hier_block2(
          "psk_demod",
          io_signature::make(1, 1, sizeof(gr_complex)),
          io_signature::make(1, 1, sizeof(char)))
{
    init_block(params);
}

psk_demod::sptr psk_demod::make(psk_demod_params_t params)
{
    return gnuradio::get_initial_sptr(new psk_demod(params));
}

} // namespace digital
} // namespace gr