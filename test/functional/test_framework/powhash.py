#!/usr/bin/env python3
# Copyright (c) 2018 The Xaya developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Computes the PoW hash for SpaceXpanse using the spacexpanse-hash CLI tool."""

import codecs
import subprocess

# Path to the spacexpanse-hash binary.  Is set from the main routine after parsing
# the command-line options.
spacexpansehash = None

def forHeader (algo, hdrData):
  """Computes the PoW hash for the header given as bytes."""
  hexStr = codecs.encode (hdrData, 'hex_codec')
  args = [spacexpansehash, algo, hexStr]
  process = subprocess.Popen (args, stdin=subprocess.PIPE,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                              universal_newlines=True)
  out, err = process.communicate ()
  returncode = process.poll ()
  if returncode:
    raise subprocess.CalledProcessError (returncode, spacexpansehash, output=err)

  res = codecs.decode (out.rstrip (), 'hex_codec')
  return res[::-1]
