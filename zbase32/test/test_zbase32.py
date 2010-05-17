#!/usr/bin/env python
#
# Copyright (c) 2002-2005 Zooko "Zooko" Wilcox-O'Hearn
# mailto:zooko@zooko.com
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this work to deal in this work without restriction (including the rights
# to use, modify, distribute, sublicense, and/or sell copies).

import random, unittest

# http://zooko.com/repos/pyutil
from pyutil import randutil

from zbase32 import *
from zbase32 import b2a

def insecurerandstr(n):
    return ''.join(map(chr, map(random.randrange, [0]*n, [256]*n)))

class base32TestCase(unittest.TestCase):
    def test_ende(self):
        bs = insecurerandstr(2**3)
        cs=b2a(bs)
        bs2=a2b(cs)
        assert bs2 == bs, "bs2: %s, bs: %s" % (`bs2`, `bs`,)

    def test_ende_long(self):
        bs = insecurerandstr(2**3)
        cs=b2a_long(bs)
        bs2=a2b_long(cs)
        assert bs2 == bs, "bs2: %s, bs: %s" % (`bs2`, `bs`,)

    def test_both(self):
        bs = insecurerandstr(2**3)
        cs=b2a(bs)
        asl=b2a_long(bs)
        assert cs == asl, "cs: %s, asl: %s" % (`cs`, `asl`,)
        cs=b2a(bs)
        bs2=a2b(cs)
        bs2l=a2b_long(cs)
        assert bs2 == bs2l
        assert bs2 == bs

    def test_big(self):
        bs = insecurerandstr(2**9)
        cs=b2a(bs)
        asl=b2a_long(bs)
        assert cs == asl, "cs: %s, asl: %s" % (`cs`, `asl`,)
        cs=b2a(bs)
        bs2=a2b(cs)
        bs2l=a2b_long(cs)
        assert bs2 == bs2l
        assert bs2 == bs

    def DISABLED_test_odd_sizes_violates_preconditions(self):
        """
        The Python implementation of b2a_l() actually works if you pass any lengthinbits, and if you don't zero out the unused least significant bits.  The precondition assertions are (a) because the high speed C implementation does not work in those cases and (b) because it might help people discover errors in their own code.  Anyway, if you have assertion-checking turned off, then this test should pass, too.
        """
        for j in range(2**6):
            lib = random.randrange(1, 2**8)
            bs = insecurerandstr(random.randrange(1, 2**5))
            cs = b2a_l(bs, lib)
            assert len(cs) == (lib+4)/5 # the size of the base-32 encoding must be just right
            asl = b2a_l_long(bs, lib)
            assert len(asl) == (lib+4)/5 # the size of the base-32 encoding must be just right
            assert cs == asl
            bs2 = a2b_l(cs, lib)
            assert len(bs2) == (lib+7)/8 # the size of the result must be just right
            bs2l = a2b_l_long(cs, lib)
            assert len(bs2l) == (lib+7)/8 # the size of the result must be just right
            assert bs2 == bs2l
            assert trimnpad(bs, lib) == bs2, "trimnpad(%s, %s): %s, bs2: %s" % (`bs`, lib, `trimnpad(bs, lib)`, `bs2`,)

    def test_odd_sizes(self):
        for j in range(2**6):
            lib = random.randrange(1, 2**8)
            bs = insecurerandstr((lib+7)/8)
            # zero-out unused least-sig bits
            if lib%8:
                b=ord(bs[-1])
                b = b >> (8 - (lib%8))
                b = b << (8 - (lib%8))
                bs = bs[:-1] + chr(b)
            cs = b2a_l(bs, lib)
            assert len(cs) == (lib+4)/5 # the size of the base-32 encoding must be just right
            asl = b2a_l_long(bs, lib)
            assert len(asl) == (lib+4)/5 # the size of the base-32 encoding must be just right
            assert cs == asl
            bs2 = a2b_l(cs, lib)
            assert len(bs2) == (lib+7)/8 # the size of the result must be just right
            bs2l = a2b_l_long(cs, lib)
            assert len(bs2l) == (lib+7)/8 # the size of the result must be just right
            assert bs2 == bs2l
            assert trimnpad(bs, lib) == bs2, "trimnpad(%s, %s): %s, bs2: %s" % (`bs`, lib, `trimnpad(bs, lib)`, `bs2`,)

    def test_n8mo4_is_an_encoding(self):
        self.failUnless(could_be_base32_encoded_l('n8mo4', 25))

    def test_could_be(self):
        # base-32 encoded strings could be
        for j in range(2**9):
            rands = insecurerandstr(random.randrange(1, 2**7))
            randsenc = b2a(rands)
            assert could_be_base32_encoded(randsenc), "rands: %s, randsenc: %s, a2b(randsenc): %s" % (`rands`, `randsenc`, `a2b(randsenc)`,)

        # base-32 encoded strings with unusual bit lengths could be, too
        for j in range(2**9):
            bitl = random.randrange(1, 2**7)
            bs = insecurerandstr((bitl+7)/8)
            # zero-out unused least-sig bits
            if bitl%8:
                b=ord(bs[-1])
                b = b >> (8 - (bitl%8))
                b = b << (8 - (bitl%8))
                bs = bs[:-1] + chr(b)
            assert could_be_base32_encoded_l(b2a_l(bs, bitl), bitl)

        # anything with a bogus character couldn't be
        s = b2a(insecurerandstr(random.randrange(3, 2**7)))
        assert not could_be_base32_encoded('\x00' + s)
        assert not could_be_base32_encoded(s + '\x00')
        assert not could_be_base32_encoded(s[:1] + '\x00' + s[1:])
        assert not could_be_base32_encoded(s[:1] + '\x00' + s[2:])

        # a base-32 encoded string with an alleged lengthinbits of 16 but with 1-bits in the 18th, 19th,
        # and 20th location couldn't be.
        assert not could_be_base32_encoded_l('yyz', 16)

def _help_bench_e(N):
    return b2a(insecurerandstr(N))
def _help_bench_ed(N):
    return a2b(b2a(insecurerandstr(N)))
def _help_bench_e_l(N):
    return b2a_long(insecurerandstr(N))
def _help_bench_ed_l(N):
    return a2b_long(b2a_long(insecurerandstr(N)))

def benchem():
    import benchfunc # from pyutil
    print "e: "
    benchfunc.bench(_help_bench_e, TOPXP=13)
    print "ed: "
    benchfunc.bench(_help_bench_ed, TOPXP=13)
    print "e_l: "
    benchfunc.bench(_help_bench_e_l, TOPXP=13)
    print "ed_l: "
    benchfunc.bench(_help_bench_ed_l, TOPXP=13)

def suite():
    suite = unittest.makeSuite(base32TestCase, 'test')
    return suite

if __name__ == "__main__":
    unittest.main()

