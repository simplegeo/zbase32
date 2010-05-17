/**
 * copyright 2002-2007 Zooko "Zooko" Wilcox-O'Hearn
 * mailto:zooko@zooko.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this work to deal in this work without
 * restriction (including the rights to use, modify, distribute,
 * sublicense, and/or sell copies).
 */
#ifndef __INCL_base32_h
#define __INCL_base32_h

static int const base32_vermaj = 0;
static int const base32_vermin = 9;
static int const base32_vermicro = 14;
static char const* const base32_vernum = "0.9.14";

#include "zstr.h" /* http://zooko.com/repos/libzstr */
#include "zutil.h" /* http://zooko.com/repos/libzutil */

#include <assert.h>
#include <stddef.h>

/**
 * @param lengthinbits the length of the data in bits
 *
 * @return the length of the base-32 encoding of the data in characters
 *
 * @precondition lengthinbits must not be greater than
 */
size_t const b2alen(const size_t lengthinbits);
size_t const B2ALEN(const size_t lengthinbits);
#ifdef NDEBUG
#define b2alen(lengthinbits) ldivceil(lengthinbits, 5)
#define B2ALEN(lengthinbits) (LDIVCEIL((lengthinbits), 5))
#endif /* #ifdef NDEBUG */

/**
 * @param os the data to be encoded in a zstr
 *
 * @return the contents of `os' in base-32 encoded form in a (newly allocated) zstr
 */
zstr b2a(const czstr os);
#ifndef NDEBUG
zstr B2A(const czstr os);
#else
#define B2A(x) b2a_l((x), (x).len*8)
#endif /* #ifndef NDEBUG */

/**
 * @param cs the base-32 encoded data (a string)
 *
 * @return the binary data that was encoded into `cs' (a string)
 */
zstr a2b(const czstr cs);
#ifndef NDEBUG
zstr A2B(const czstr cs);
#else
#define A2B(x) a2b_l((x), (((x).len*5+3)/8)*8)
#endif /* #ifndef NDEBUG */

/**
 * @param os the data to be encoded in a zstr
 * @param lengthinbits the number of bits of data in `os' to be encoded
 *
 * b2a_l() will generate a base-32 encoded string big enough to encode lengthinbits bits.  So for 
 * example if os is 2 bytes long and lengthinbits is 15, then b2a_l() will generate a 3-character-
 * long base-32 encoded string (since 3 quintets is sufficient to encode 15 bits).  If os is 2 bytes
 * long and lengthinbits is 16 (or None), then b2a_l() will generate a 4-character string.  Note 
 * that `b2a_l()' does not mask off unused least-significant bits, so for example if os is 2 bytes 
 * long and lengthinbits is 15, then you must ensure that the unused least-significant bit of os is 
 * a zero bit or you will get the wrong result.  This precondition is tested by assertions if 
 * assertions are enabled.
 *
 * Warning: if you generate a base-32 encoded string with `b2a_l()', and then someone else tries to 
 * decode it by calling `a2b()' instead of  `a2b_l()', then they will (probably) get a different 
 * string than the one you encoded!  So only use `b2a_l()' when you are sure that the encoding and 
 * decoding sides know exactly which `lengthinbits' to use.  If you do not have a way for the 
 * encoder and the decoder to agree upon the lengthinbits, then it is best to use `b2a()' and 
 * `a2b()'.  The only drawback to using `b2a()' over `b2a_l()' is that when you have a number of 
 * bits to encode that is not a multiple of 8, `b2a()' can sometimes generate a base-32 encoded 
 * string that is one or two characters longer than necessary.
 *
 * @return the contents of `os' in base-32 encoded form in a (newly allocated) zstr
 *
 * On memory exhaustion, if Z_EXHAUST_EXIT is not defined, return a "null" zstr with .buf == NULL 
 * and .len == 0.
 */
zstr b2a_l(const czstr cs, const size_t lengthinbits);
zstr b2a_l_extra_Duffy(const czstr cs, const size_t lengthinbits);

/**
 *
 * @param lengthinbits the number of bits of data in encoded into `cs'
 *
 * a2b_l() will return a result big enough to hold lengthinbits bits.  So for example if cs is 
 * 4 characters long (encoding at least 15 and up to 20 bits) and lengthinbits is 16, then a2b_l() 
 * will return a string of length 2 (since 2 bytes is sufficient to store 16 bits).  If cs is 
 * 4 characters long and lengthinbits is 20, then a2b_l() will return a string of length 3 (since 
 * 3 bytes is sufficient to store 20 bits).  Note that `b2a_l()' does not mask off unused least- 
 * significant bits, so for example if cs is 4 characters long and lengthinbits is 17, then you must
 *  ensure that all three of the unused least-significant bits of cs are zero bits or you will get 
 * the wrong result.  This precondition is tested by assertions if assertions are enabled.  
 * (Generally you just require the encoder to ensure this consistency property between the least 
 * significant zero bits and value of `lengthinbits', and reject strings that have a length-in-bits 
 * which isn't a multiple of 8 and yet don't have trailing zero bits, as improperly encoded.)
 *
 * Please see the warning in the documentation of `b2a_l()' regarding the use of `b2a()' versus 
 * `b2a_l()'.
 *
 * @return a newly allocated zstr containing the data encoded in `cs'
 *
 * @precondition `cs' must be possibly base32 encoded data.: could_be_base32_encoded_l(cs, lengthinbits)
 *
 * On memory exhaustion, if Z_EXHAUST_EXIT is not defined, return a "null" zstr with .buf == NULL 
 * and .len == 0.
 */
zstr a2b_l(const czstr cs, const size_t lengthinbits);
zstr a2b_l_very_Duffy(const czstr cs, const size_t lengthinbits);

#endif /* #ifndef __INCL_base32_h */

