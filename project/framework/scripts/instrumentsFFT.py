#! /usr/bin/env python
#-*- coding=utf-8 -*-
##
# \file
# script for instrument conversion

import sys
import re
from pylab import *
from numpy import arange
from optparse import OptionParser
from scikits.audiolab import wavread
from math import ceil, log10

#											#
#	http://xoomer.virgilio.it/sam_psy/psych/sound_proc/sound_proc_python.html	#
#											#


# parse options
usage = """usage: %prog -i <wav_file>

Generate synusoidal model data for demo.
output files are waw_file_name.c and waw_file_name.h
"""
parser = OptionParser(usage=usage)
parser.add_option("-i", "--input", dest="input", help="input wav file", metavar="<INPUT_FILE>")
(options, args) = parser.parse_args()


baseName = re.sub(".[^.]*$", "", options.input)

safeName = options.input.lower()
safeName = re.sub(".[^.]*$", "", safeName)
safeName = re.sub("^.*/", "", safeName)
safeName = re.sub("[ -.,;/]", "_", safeName)
safeName = safeName + "_instrument"
print safeName

#open wave
if(options.input):
	(snd, sampFreq, nBits) = wavread(options.input)

#create time line
timeArray = arange(0, snd.shape[0] * 1.0, 1)
timeArray = timeArray / sampFreq
timeArray = timeArray * 1000 #scale to ms

# start fourier
n = len(snd)
p = fft(snd) # take the fourier transform


nUniquePts = ceil((n+1)/2.0)
p = p[0:nUniquePts]
p = abs(p)


p = p / float(n) # scale by the number of points so that
                 # the magnitude does not depend on the length
                 # of the signal or on its sampling frequency
p = p**2  # square it to get the power

# odd nfft excludes Nyquist point
if n % 2 > 0: # we've got odd number of points fft
	p[1:len(p)] = p[1:len(p)] * 2
else:
	p[1:len(p) - 1] = p[1:len(p) - 1] * 2 # we've got even number of points fft


tuple = []
#convert freq array to Hz and p to Bel
freqArray = arange(0, nUniquePts, 1.0) * (sampFreq / (n * 1.0)); # containing freq in Hz
for index in xrange(0, len(p), 1):
	tuple.append([log10(p[index]), freqArray[index]]) # (bel, Hz) values

#scaling to one Hz
maximum = max(tuple)[1]
for index in xrange(0, len(tuple), 1):
	tuple[index][1] *= (1 / maximum)

#isolating 512 samples
result = []
volumeSum = 0
for index in xrange(0, 64, 1):
	result.append([10**max(tuple)[0], max(tuple)[1]])
	volumeSum += 10**max(tuple)[0]
	tuple.remove(max(tuple))


#normalize output
ns = 0
for index in xrange(0, 64, 1):
	result[index][0] /= volumeSum


#print output
h_file = baseName + "_instrument.h"
c_file = baseName + "_instrument.c"
hout = open(h_file, "w")
print >> hout, """#ifndef _""" + safeName.upper() + """_H_
#define _""" + safeName.upper() + """_H_

#include \"framework/musicmanager/musicmanager.h\"

extern struct S_instrumentFreq """ + safeName + """[];

#endif
"""

cout = open(c_file, "w")
print >> cout, """#include \"""" + h_file + """\"

struct S_instrumentFreq """ + safeName + """[] = {"""
for res in result:
	print >> cout, """{""" + str(res[1]) + """, """ + str(res[0]) + """}, """
print >> cout, """
{0.0, 0.0} // terminator
};
"""

hout.close()
cout.close()