#ifndef AES_H
#define AES_H

#include <boost/preprocessor.hpp>
#include "sbox_gen.h"
#include "hex.h"

/*
    BPP_AES_SBOX(hex1) -> value of s-box cell
    BPP_AES_KEYSCHED(key_seq) -> scheduled key as sequence
    BPP_AES_ENCRYPT(shed_key_seq, data_seq) -> encrypted sequence
    BPP_AES_ENCRYPT_ARRAY(key_tuple, data_tuple) -> ECB encrypted data for array init
*/

#define BPP_AES_SBOX(x) BPP_AES_SBOX_I(x)
#define BPP_AES_SBOX_I(x) BPP_AES_SBOX_N_ ## x

#define BPP_AES_KEYSCHED_PHASE1(rcon, s) \
    (BPP_XOR(BPP_AES_SBOX(BOOST_PP_SEQ_ELEM(3, s)), BOOST_PP_SEQ_ELEM(12, s))) \
    (BPP_XOR(BPP_AES_SBOX(BOOST_PP_SEQ_ELEM(0, s)), BOOST_PP_SEQ_ELEM(13, s))) \
    (BPP_XOR(BPP_AES_SBOX(BOOST_PP_SEQ_ELEM(1, s)), BOOST_PP_SEQ_ELEM(14, s))) \
    (BPP_XOR(BPP_XOR(BPP_AES_SBOX(BOOST_PP_SEQ_ELEM(2, s)), rcon), BOOST_PP_SEQ_ELEM(15, s))) s

#define BPP_AES_KEYSCHED_PHASE2(s) \
    (BPP_XOR(BOOST_PP_SEQ_ELEM(0, s), BOOST_PP_SEQ_ELEM(12, s))) \
    (BPP_XOR(BOOST_PP_SEQ_ELEM(1, s), BOOST_PP_SEQ_ELEM(13, s))) \
    (BPP_XOR(BOOST_PP_SEQ_ELEM(2, s), BOOST_PP_SEQ_ELEM(14, s))) \
    (BPP_XOR(BOOST_PP_SEQ_ELEM(3, s), BOOST_PP_SEQ_ELEM(15, s))) s

#define BPP_AES_KEYSCHED_ROW(rcon, s) \
    BPP_AES_KEYSCHED_PHASE2(BPP_AES_KEYSCHED_PHASE2( \
    BPP_AES_KEYSCHED_PHASE2(BPP_AES_KEYSCHED_PHASE1(rcon, s))))

#define BPP_AES_KEYSCHED(s) \
    BOOST_PP_SEQ_REVERSE( \
    BPP_AES_KEYSCHED_ROW(0x36, \
    BPP_AES_KEYSCHED_ROW(0x1b, \
    BPP_AES_KEYSCHED_ROW(0x80, \
    BPP_AES_KEYSCHED_ROW(0x40, \
    BPP_AES_KEYSCHED_ROW(0x20, \
    BPP_AES_KEYSCHED_ROW(0x10, \
    BPP_AES_KEYSCHED_ROW(0x08, \
    BPP_AES_KEYSCHED_ROW(0x04, \
    BPP_AES_KEYSCHED_ROW(0x02, \
    BPP_AES_KEYSCHED_ROW(0x01, \
    BOOST_PP_SEQ_REVERSE(s))))))))))))

#define BPP_AES_ADD_ROUND_KEY_I(z, n, d) \
    (BPP_XOR(BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(0, d)), BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1, d))))

#define BPP_AES_ADD_ROUND_KEY(b, rk) \
    BOOST_PP_REPEAT(16, BPP_AES_ADD_ROUND_KEY_I, (b)(rk))

#define BPP_AES_SUB_BYTES_I(s, d, e) BPP_AES_SBOX(e)

#define BPP_AES_SUB_BYTES(s) \
    BOOST_PP_SEQ_TRANSFORM(BPP_AES_SUB_BYTES_I, ~, s)

#define BPP_AES_SHIFT_ROWS_II(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
    (a)(f)(k)(p)(e)(j)(o)(d)(i)(n)(c)(h)(m)(b)(g)(l)

#define BPP_AES_SHIFT_ROWS_I(s) BPP_AES_SHIFT_ROWS_II(s)

#define BPP_AES_SHIFT_ROWS(s) BPP_AES_SHIFT_ROWS_I(BOOST_PP_SEQ_ENUM(s))

#define BPP_AES_MIX_COLUMNS_IV(a,b,c,d) \
    (BPP_XOR(BPP_XOR(BPP_GFMUL2(a),BPP_GFMUL3(b)), BPP_XOR(c, d)))

#define BPP_AES_MIX_COLUMNS_III(a,b,c,d) \
    BPP_AES_MIX_COLUMNS_IV(a,b,c,d) \
    BPP_AES_MIX_COLUMNS_IV(b,c,d,a) \
    BPP_AES_MIX_COLUMNS_IV(c,d,a,b) \
    BPP_AES_MIX_COLUMNS_IV(d,a,b,c)

#define BPP_AES_MIX_COLUMNS_II(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
    BPP_AES_MIX_COLUMNS_III(a,b,c,d) \
    BPP_AES_MIX_COLUMNS_III(e,f,g,h) \
    BPP_AES_MIX_COLUMNS_III(i,j,k,l) \
    BPP_AES_MIX_COLUMNS_III(m,n,o,p)

#define BPP_AES_MIX_COLUMNS_I(s) BPP_AES_MIX_COLUMNS_II(s)

#define BPP_AES_MIX_COLUMNS(s) BPP_AES_MIX_COLUMNS_I(BOOST_PP_SEQ_ENUM(s))

#define BPP_AES_ROUND1(k, p, s) \
    BPP_AES_ADD_ROUND_KEY(s, BOOST_PP_SEQ_SUBSEQ(k, p, 16))

#define BPP_AES_ROUND2(k, p, s) \
    BPP_AES_ADD_ROUND_KEY(BPP_AES_MIX_COLUMNS(BPP_AES_SHIFT_ROWS(BPP_AES_SUB_BYTES(s))), \
    BOOST_PP_SEQ_SUBSEQ(k, p, 16))

#define BPP_AES_ROUND3(k, p, s) \
    BPP_AES_ADD_ROUND_KEY(BPP_AES_SHIFT_ROWS(BPP_AES_SUB_BYTES(s)), BOOST_PP_SEQ_SUBSEQ(k, p, 16))

#define BPP_AES_ENCRYPT(k, s) \
    BPP_AES_ROUND3(k, 160, \
    BPP_AES_ROUND2(k, 144, \
    BPP_AES_ROUND2(k, 128, \
    BPP_AES_ROUND2(k, 112, \
    BPP_AES_ROUND2(k, 96, \
    BPP_AES_ROUND2(k, 80, \
    BPP_AES_ROUND2(k, 64, \
    BPP_AES_ROUND2(k, 48, \
    BPP_AES_ROUND2(k, 32, \
    BPP_AES_ROUND2(k, 16, \
    BPP_AES_ROUND1(k, 0, s)))))))))))

#define BPP_AES_ENCRYPT_ARRAY(key, data) \
    BOOST_PP_SEQ_ENUM(BPP_AES_ENCRYPT(BPP_AES_KEYSCHED(BOOST_PP_TUPLE_TO_SEQ(16, key)), \
    BOOST_PP_TUPLE_TO_SEQ(16, data)))

#endif