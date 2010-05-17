/**
 * copyright 2002-2007 Zooko "Zooko" Wilcox-O'Hearn
 * mailto:zooko@zooko.com
 *
 * Permission is hereby granted to any person obtaining a copy of this work to
 * deal in this work without restriction (including the rights to use, modify,
 * distribute, sublicense, and/or sell copies).
 */
#include "base32.h"

#include <math.h>
#include <string.h>
#include <stdio.h> /* XXX only for debug printfs */

static const char*const chars="ybndrfg8ejkmcpqxot1uwisza345h769";
/* revchars: index into this table with the ASCII value of the char.  The result is the value of that quintet. */
static const zbyte revchars[]={ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 18, 255, 25, 26, 27, 30, 29, 7, 31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 24, 1, 12, 3, 8, 5, 6, 28, 21, 9, 10, 255, 11, 2, 16, 13, 14, 4, 22, 17, 19, 255, 20, 15, 0, 23, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };

#undef b2alen
size_t const
b2alen(const size_t lengthinbits) {
	return divceil(lengthinbits, 5);
}

#undef B2ALEN
size_t const
B2ALEN(const size_t lengthinbits) {
	return divceil(lengthinbits, 5);
}

zstr
b2a(const czstr os) {
	return b2a_l(os, os.len*8);
}

#undef B2A
zstr
B2A(const czstr os) {
	return b2a_l(os, os.len*8);
}

zstr
b2a_l(const czstr os, const size_t lengthinbits) {
		return b2a_l_extra_Duffy(os, lengthinbits);
}

zstr
b2a_l_extra_Duffy(const czstr os, const size_t lengthinbits) {
	zstr result = new_z(DIVCEIL(os.len*8, 5)); /* if lengthinbits is not a multiple of 8 then this is allocating space for 0, 1, or 2 extra quintets that will be truncated at the end of this function if they are not needed */
#ifndef Z_EXHAUST_EXIT
	if (result.buf == NULL)
		return result;
#endif /* #ifndef Z_EXHAUST_EXIT */

	zbyte* resp = result.buf + result.len; /* pointer into the result buffer, initially pointing to the "one-past-the-end" quintet */
	const zbyte* osp = os.buf + os.len; /* pointer into the os buffer, initially pointing to the "one-past-the-end" octet */

	/* Now this is a real live Duff's device.  You gotta love it. */
	unsigned long x=0; /* to hold up to 32 bits worth of the input */
	switch ((osp - os.buf) % 5) {
	case 0:
		do {
			x = *--osp;
			*--resp = chars[x % 32]; /* The least sig 5 bits go into the final quintet. */
			x /= 32; /* ... now we have 3 bits worth in x... */
	case 4:
			x |= ((unsigned long)(*--osp)) << 3; /* ... now we have 11 bits worth in x... */
			*--resp = chars[x % 32];
			x /= 32; /* ... now we have 6 bits worth in x... */
			*--resp = chars[x % 32];
			x /= 32; /* ... now we have 1 bits worth in x... */
	case 3:
			x |= ((unsigned long)(*--osp)) << 1; /* The 8 bits from the 2-indexed octet.  So now we have 9 bits worth in x... */
			*--resp = chars[x % 32];
			x /= 32; /* ... now we have 4 bits worth in x... */
	case 2:
			x |= ((unsigned long)(*--osp)) << 4; /* The 8 bits from the 1-indexed octet.  So now we have 12 bits worth in x... */
			*--resp = chars[x%32];
			x /= 32; /* ... now we have 7 bits worth in x... */
			*--resp = chars[x%32];
			x /= 32; /* ... now we have 2 bits worth in x... */
	case 1:
			x |= ((unsigned long)(*--osp)) << 2; /* The 8 bits from the 0-indexed octet.  So now we have 10 bits worth in x... */
			*--resp = chars[x%32];
			x /= 32; /* ... now we have 5 bits worth in x... */
			*--resp = chars[x];
		} while (osp > os.buf);
	} /* switch ((osp - os.buf) % 5) */

	/* truncate any unused trailing zero quintets */
	result.len = DIVCEIL(lengthinbits, 5);
	result.buf[result.len] = '\0';
	return result;
}

zstr
a2b(const czstr cs) {
	return a2b_l(cs, ((cs.len*5+3)/8)*8);
}

#undef A2B
zstr
A2B(const czstr cs) {
	return a2b_l(cs, ((cs.len*5+3)/8)*8);
}

zstr
a2b_l(const czstr cs, const size_t lengthinbits) {
	return a2b_l_very_Duffy(cs, lengthinbits);
}

zstr
a2b_l_very_Duffy(const czstr cs, const size_t lengthinbits) {
	unsigned long x=0; /* to hold up to 32 bits worth of the input */
	zstr result = new_z(divceil(cs.len*5, 8)); /* if lengthinbits is not a multiple of 5 then this is allocating space for 0 or 1 extra octets that will be truncated at the end of this function if they are not needed */
#ifndef Z_EXHAUST_EXIT
	if (result.buf == NULL)
		return result;
#endif /* #ifndef Z_EXHAUST_EXIT */

	zbyte* resp = result.buf + result.len; /* pointer into the result buffer, initially pointing to the "one-past-the-end" octet */
	const zbyte* csp = cs.buf + cs.len; /* pointer into the cs buffer, initially pointing to the "one-past-the-end" character */

	fflush(stdout);

	/* Now this is a real live Duff's device.  You gotta love it. */
	switch ((csp - cs.buf) % 8) {
	case 0:
		fflush(stdout);
		do {
			fflush(stdout);
			x = revchars[*--csp]; /* 5 bits... */
	case 7:
			fflush(stdout);
			x |= revchars[*--csp] << 5; /* 10 bits... */
			*--resp = x % 256;
			x /= 256; /* 2 bits... */
	case 6:
			fflush(stdout);
			x |= revchars[*--csp] << 2; /* 7 bits... */
	case 5:
			fflush(stdout);
			x |= revchars[*--csp] << 7; /* 12 bits... */
			*--resp = x % 256;
			x /= 256; /* 4 bits... */
	case 4:
			x |= revchars[*--csp] << 4; /* 9 bits... */
			*--resp = x % 256;
			fflush(stdout);
			x /= 256; /* 1 bit... */
	case 3:
			x |= revchars[*--csp] << 1; /* 6 bits... */
			fflush(stdout);
	case 2:
			x |= revchars[*--csp] << 6; /* 11 bits... */
			*--resp = x % 256;
			fflush(stdout);
			x /= 256; /* 3 bits... */
	case 1:
			x |= revchars[*--csp] << 3; /* 8 bits... */
			*--resp = x % 256;
			fflush(stdout);
		} while (csp > cs.buf);
	} /* switch ((csp - cs.buf) % 8) */

	/* truncate any unused trailing zero octets */
	result.len = DIVCEIL(lengthinbits, 8);
	result.buf[result.len] = '\0';
	return result;
}

