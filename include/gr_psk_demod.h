// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author: Tim Rice (trice2@jaguarlandrover.com)

#ifndef INCLUDED_GR_RUNTIME_PSK_DEMOD_H
#define INCLUDED_GR_RUNTIME_PSK_DEMOD_H

#include <gnuradio/digital/api.h>
#include <gnuradio/hier_block2.h>

namespace gr
{
namespace digital
{

enum class mod_code {
  GRAY_CODE,
  SET_PARTITION_CODE,
  NO_CODE
};

class gray_code_generator {
public:
  std::vector<int> get_gray_code(unsigned int length);
  void generate_new_gray_code(unsigned int length);
private:
  std::vector<int> _gcs = {0,1};
  int _lp2 = 2;
  int _np2 = 4;
  int _i = 2;
};

typedef struct psk_demod_params {
  int constellation_points;
  enum mod_code mod_code;
  int differential;
  float samples_per_symbol;
  int pre_diff_code;
  float excess_bw;
  float freq_bw;
  float timing_bw;
  float phase_bw;
} psk_demod_params_t;

class DIGITAL_API psk_demod : public hier_block2
{
public:
  typedef boost::shared_ptr<psk_demod> sptr;

  static sptr make(psk_demod_params_t params);

  ~psk_demod();

private:
  gray_code_generator gcg;
  psk_demod(void) {}
  psk_demod(psk_demod_params_t params);

  void init_block(psk_demod_params_t params);
};

} // namespace digital
} // namespace gr

#endif