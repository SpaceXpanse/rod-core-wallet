#!/usr/bin/env python3
# Copyright (c) 2018 The Xaya developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Script to process the raw data for the ROD airdrop to holders of
# Huntercoin balances at the time of the snapshot.  It reads the raw
# data in snapshot-balances.json, converts the addresses (HUC->ROD
# based on the same pubkey hash / private key) and computes the
# ROD balance to be assigned to each of the addresses.  The output is
# printed as JSON to stdout.
#
# The base58 processing is done using pybitcointools, which must be on the
# PYTHONPATH and can be obtained here:
#
#   https://github.com/vbuterin/pybitcointools
#
# This script needs to be run from within the contrib/spacexpanse/huc-snapshot
# directory.

from decimal import Decimal
import json

from bitcoin import b58check_to_hex, hex_to_b58check

ROD_ADDRESS_VERSION = 28
INPUT_FILE = 'snapshot-balances.json'
PRECISION = Decimal ('1.00000000')
ROD_PER_HUC = Decimal ('0.23338000')

with open (INPUT_FILE) as f:
  hucBalances = json.load (f)

output = []
totalRod = Decimal ('0.00000000')
for hucAddr, val in hucBalances['addresses'].items ():
  keyHex = b58check_to_hex (hucAddr)
  rodAddr = hex_to_b58check (keyHex, ROD_ADDRESS_VERSION)

  hucValue = Decimal (val).quantize (PRECISION)
  hucRounded = int (val)
  rodValue = hucRounded * ROD_PER_HUC
  
  obj = {
    "address":
      {
        "huc": hucAddr,
        "rod": rodAddr,
      },
    "amount":
      {
        "huc": float (hucValue),
        "full_huc": hucRounded,
        "rod": float (rodValue),
      },
  }
  output.append (obj)
  totalRod += rodValue

print (json.dumps (output, indent=2))
#print (totalRod)
