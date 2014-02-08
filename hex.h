#ifndef UNHEX_H
#define UNHEX_H

#include <boost/preprocessor.hpp>
#include "hex_gen.h"

/*
    BPP_UNHEX(hex1) -> sequence of 8 bits
    BPP_HEX(seq) -> hex number
    BPP_XOR(hex1, hex2) -> hex1 ^ hex2
    BPP_GFMUL2(hex1) -> hex1 * 2 in GF(2,8) with 0x11b
    BPP_GFMUL3(hex1) -> hex1 * 3 in GF(2,8) with 0x11b
*/

#define BPP_UNHEX(x) BPP_UNHEX_I(x)
#define BPP_UNHEX_I(x) BPP_UNHEX_N_ ## x

#define BPP_HEX(x) BPP_HEX_I(BOOST_PP_SEQ_CAT(x))
#define BPP_HEX_I(x) BPP_HEX_II(x)
#define BPP_HEX_II(x) BPP_HEX_N_ ## x

#define BPP_XOR_II(z, n, d) \
    (BOOST_PP_BITXOR(BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(0, d)), BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1, d))))

#define BPP_XOR_I(a, b) \
    BOOST_PP_REPEAT(8, BPP_XOR_II, (a)(b))

#define BPP_XOR(a, b) \
    BPP_HEX(BPP_XOR_I(BPP_UNHEX(a), BPP_UNHEX(b)))

#define BPP_GFMUL2_II(h, t) \
    BOOST_PP_IIF(h, BPP_XOR_I(t, (0)(0)(0)(1)(1)(0)(1)(1)), t)

#define BPP_GFMUL2_I(a) \
    BPP_GFMUL2_II(BOOST_PP_SEQ_HEAD(a), BOOST_PP_SEQ_TAIL(a)(0))

#define BPP_GFMUL2(a) \
    BPP_HEX(BPP_GFMUL2_I(BPP_UNHEX(a)))

#define BPP_GFMUL3_I(a) \
    BPP_XOR_I(a, BPP_GFMUL2_I(a))

#define BPP_GFMUL3(a) \
    BPP_HEX(BPP_GFMUL3_I(BPP_UNHEX(a)))

#endif