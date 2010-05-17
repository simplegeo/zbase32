#!/usr/bin/env python

# Copyright (C) 2002-2008 Zooko Wilcox-O'Hearn
# mailto:zooko@zooko.com
# Permission is hereby granted to any person obtaining a copy of this work to
# deal in this work without restriction (including the rights to use, modify,
# distribute, sublicense, and/or sell copies).

import os, re, sys
import glob

try:
    from ez_setup import use_setuptools
except ImportError:
    pass
else:
    # On cygwin there was a permissions error that was fixed in 0.6c6.
    use_setuptools(min_version='0.6c6', download_delay=0)

from setuptools import Extension, find_packages, setup

trove_classifiers=[
    "Development Status :: 5 - Production/Stable",
    "Environment :: Console",
    "License :: OSI Approved :: BSD License", 
    "License :: DFSG approved",
    "Intended Audience :: Developers", 
    "Intended Audience :: End Users/Desktop",
    "Intended Audience :: System Administrators",
    "Operating System :: Microsoft",
    "Operating System :: Microsoft :: Windows",
    "Operating System :: Unix",
    "Operating System :: POSIX :: Linux",
    "Operating System :: POSIX",
    "Operating System :: MacOS :: MacOS X",
    "Operating System :: Microsoft :: Windows :: Windows NT/2000",
    "Operating System :: OS Independent", 
    "Natural Language :: English", 
    "Programming Language :: C", 
    "Programming Language :: Python", 
    "Programming Language :: Python :: 2",
    "Programming Language :: Python :: 2.4",
    "Programming Language :: Python :: 2.5",
    "Topic :: Utilities",
    "Topic :: System :: Systems Administration",
    "Topic :: Software Development :: Libraries",
    ]

setup_requires = []

PKG='zbase32'
VERSIONFILE = os.path.join(PKG, "_version.py")
verstr = "unknown"
try:
    verstrline = open(VERSIONFILE, "rt").read()
except EnvironmentError:
    pass # Okay, there is no version file.
else:
    VSRE = r"^verstr = ['\"]([^'\"]*)['\"]"
    mo = re.search(VSRE, verstrline, re.M)
    if mo:
        verstr = mo.group(1)
    else:
        print "unable to find version in %s" % (VERSIONFILE,)
        raise RuntimeError("if %s.py exists, it must be well-formed" % (VERSIONFILE,))

# darcsver is needed only if you want "./setup.py darcsver" to write a new
# version stamp in pyutil/_version.py, with a version number derived from
# darcs history.  http://pypi.python.org/pypi/darcsver
if 'darcsver' in sys.argv[1:]:
    setup_requires.append('darcsver >= 1.0.0')

# setuptools_darcs is required to produce complete distributions (such as with
# "sdist" or "bdist_egg"), unless there is a pyutil.egg-info/SOURCE.txt file
# present which contains a complete list of files that should be included.
# http://pypi.python.org/pypi/setuptools_darcs
setup_requires.append('setuptools_darcs >= 1.1.0')

def _setup(test_suite):
    setup(name=PKG,
          version=verstr,
          description='base32 encoder/decoder',
          long_description='An alternate base32 encoder (not RFC 3548 compliant).',
          author='Zooko O\'Whielacronx',
          author_email='zooko@zooko.com',
          url='http://allmydata.org/trac/' + PKG,
          license='BSD',
          packages=find_packages(),
          include_package_data=True,
          setup_requires=setup_requires,
          tests_require=['pyutil'],
          classifiers=trove_classifiers,
          test_suite=test_suite,
          zip_safe=False, # I prefer unzipped for easier access.
          )

test_suite_name=PKG+".test"
try:
    _setup(test_suite=test_suite_name)
except BaseException, le:
    # to work around a bug in Elisa v0.3.5
    if "test_suite must be a list" in str(le):
        _setup(test_suite=[test_suite_name])
    else:
        raise
