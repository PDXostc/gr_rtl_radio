// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#include <cmath>
#include <vector>

#include "gr_psk_demod.h"

namespace gr
{
namespace digital
{

std::vector<int> invert_code(std::vector<int> code) {
    std::vector<std::pair<int,int>> ic;
    for (int i = 0; i < code.size(); ++i) {
        ic.push_back(std::make_pair(code[i], i));
    }
    std::sort(ic.begin(), ic.end());
    std::vector<int> inverted;
    for (int i = 0; i < code.size(); ++i) {
        inverted.push_back(ic[i].first);
    }
    return inverted;
}

std::vector<int> gray_code_generator::get_gray_code(unsigned int length)
{
    if (_gcs.size() < length) {
        this->generate_new_gray_code(length);
    }
    std::vector<int> gcs_trunc = _gcs;
    gcs_trunc.resize(length);
    return gcs_trunc;
}

void gray_code_generator::generate_new_gray_code(unsigned int length) {
    int result;
    while (_gcs.size() < length) {
        if (_i == _lp2) {
            result = _i + _i / 2;
        }
        else {
            result = _gcs[2 * _lp2 - 1 - _i] + _lp2;
        }
        _gcs.push_back(result);
        _i++;
        if (_i == _np2) {
            _lp2 = _i;
            _np2 = _i * 2;
        }
    }
}

psk_demod::~psk_demod()
{
}

void psk_demod::init_block(psk_demod_params_t params)
{
    if (ceil(log2(params.constellation_points)) == floor(log2(params.constellation_points))) {
        throw std::runtime_error("Number of constellation points must be a power of two.");
    }
    std::vector<gr_complex> points;
    for (int i = 0; i < params.constellation_points; ++i) {
        gr_complex j(0, 1);
        points.push_back(exp(2 * i * (float)M_PI * j) / (float)params.constellation_points);
    }
    std::vector<int> pre_diff_code;
    std::vector<int> post_diff_code;
    bool post_diff_code_none = true;
    if (params.mod_code == mod_code::GRAY_CODE) {
        if (params.differential) {
            pre_diff_code = gcg.get_gray_code(params.constellation_points);
        }
        else {
            post_diff_code = gcg.get_gray_code(params.constellation_points);
            post_diff_code_none = false;
        }
    }
    if (!post_diff_code_none) {
        std::vector<int> inverse_post_diff_code = invert_code(post_diff_code);
        std::vector<gr_complex> points_inv;
        for (int i = 0; i < inverse_post_diff_code.size(); ++i) {
            points_inv.push_back(points[i]);
        }
        points = points_inv;
    }
    auto psk = gr::digital::constellation_psk::make(points, pre_diff_code, params.constellation_points);

    //super(psk_demod, self).__init__(constellation, differential, *args, **kwargs)
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