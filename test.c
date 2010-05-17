#include "base32.h"
#include "zstr.h"
#include "zutil.h"

#undef NDEBUG
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


zstr randz(const size_t len)
{
	zstr result = new_z(len);
	size_t i;
	for (i=0; i<len; i++) {
		result.buf[i] = rand() % 256;
	}
	return result;
}

int test_rands()
{
	int i;
	czstr z = cz(randz(16));
	/*czstr z = cs_as_cz("8b8t");*/
	czstr a;
	czstr zrecovered;
	for (i=0; i<32768; i++) {
		a = cz(b2a(z));
		/* printf("a.len: %d, buf: %s\n", a.len, a.buf);*/
		assert (strlen(cz_as_cs(a)) == divceil(z.len*8, 5));
		assert (isgraph(a.buf[0]));
		zrecovered = cz(a2b(a));
		if (!zeq(z, zrecovered)) {
			printf("failed basic recovery test. z: %s, a: %s, zr: %s\n", repr(z).buf, a.buf, repr(zrecovered).buf);
			return -1;
		}
		free((void*)a.buf);
		free((void*)zrecovered.buf);
	}
	return 0;
}

int test_b2alen_doesnt_overflow()
{
	if (b2alen(SIZE_T_MAX) != (unsigned long)ceil((double)SIZE_T_MAX/(double)5))
		return -1;
	else
		return 0;
}

int bench_ed()
{
	unsigned i;
	for (i=0;i<32;i++)
		test_rands();
	return 0;
}

int main(int argc, char**argv)
{
	if (test_b2alen_doesnt_overflow()) {
		return -1;
	}
	return test_rands();
	/*return bench_ed();*/
}
