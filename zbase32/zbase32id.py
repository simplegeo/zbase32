#  Copyright (c) 2002, 2003 Zooko "Zooko" Wilcox-O'Hearn
#  portions Copyright (c) 2001 Autonomous Zone Industries
#  This file is licensed under the
#    GNU Lesser General Public License v2.1.
#    See the file COPYING or visit http://www.gnu.org/ for details.

import string

from pyutil import humanreadable

import zbase32 

printableascii = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=+!@#$%^&*()`~[]\{}|;':\",./<>? \t" # I just typed this in by looking at my keyboard.  It probably doesn't matter much if I missed some, because I only use it to guess whether a 20-byte string should be represented as a string or as an ID.  If all of the characters in the string are found `printableascii', then we guess that it is a string, not an id.
nulltrans = string.maketrans('', '')

def abbrev_bare(id, b2a_l=zbase32.b2a_l, trimnpad=zbase32.trimnpad):
    return b2a_l(trimnpad(id[:4], 25), 25)

def abbrev(id):
    return '<' + abbrev_bare(id) + '>'

def to_base32(id, b2a_l=zbase32.b2a_l):
    return b2a_l(id, 160)

def could_be_id(id, could_be_base32_encoded_l=zbase32.could_be_base32_encoded_l):
    return could_be_base32_encoded_l(id, 160)

def could_be_abbrev(id, could_be_base32_encoded_l=zbase32.could_be_base32_encoded_l):
    return could_be_base32_encoded_l(id, 25)

class AbbrevRepr(humanreadable.BetterRepr):
    def __init__(self):
        humanreadable.BetterRepr.__init__(self)

    def repr_str(self, obj, level, could_be_base32_encoded_l=zbase32.could_be_base32_encoded_l, nulltrans=nulltrans, printableascii=printableascii, abbrev=abbrev):
        if len(obj) == 20:
            # But maybe it was just a 20-character human-readable string, like "credit limit reached", so this is an attempt to detect that case.
            if len(obj.translate(nulltrans, printableascii)) == 0:
                if self.maxstring >= 22:
                    return `obj`

                # inlining repr.repr_string() here...
                s = `obj[:self.maxstring]`
                if len(s) > self.maxstring:
                    i = max(0, (self.maxstring-3)/2)
                    j = max(0, self.maxstring-3-i)
                    s = `obj[:i] + obj[len(obj)-j:]`
                    s = s[:i] + '...' + s[len(s)-j:]
                    # ... done inlining `repr.repr_string()'
                return s

            return abbrev(obj)
        elif could_be_base32_encoded_l(obj, 160):
            # new "z-base-32" base-32 encoding
            return '<' + obj[:5] + '>'
        else:
            # inlining repr.repr_string() here...
            s = `obj[:self.maxstring]`
            if len(s) > self.maxstring:
                i = max(0, (self.maxstring-3)/2)
                j = max(0, self.maxstring-3-i)
                s = `obj[:i] + obj[len(obj)-j:]`
                s = s[:i] + '...' + s[len(s)-j:]
            # ... done inlining `repr.repr_string()'
            return s

# use base32id repr to pretty-print ids embedded in structures
humanreadable.brepr = AbbrevRepr()

