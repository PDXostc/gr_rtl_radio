// LICENCE

// set sample rate - at least 44100 * 2
// udp_source_block(sizeof(float), "IP", port, payload_size=1472, (true?))
// audio_sink(44100, '', True)

// audio
// blocks

#include <gnuradio/top_block.h>
#include <gnuradio/blocks/udp_source.h>
#include <gnuradio/blocks/audio/sink.h>

struct recv_ctx {
    gr::top_block_sptr top_block;
    gr::blocks::udp_source udp_in;
    gr::audio::sink audio_out;
}

int main(int argc, char* argv[]) {
    return 0;
}
