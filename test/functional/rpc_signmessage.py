#!/usr/bin/env python3
# Copyright (c) 2016-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test RPC commands for signing and verifying messages."""

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal

import base64

class SignMessagesTest(BitcoinTestFramework):
    def set_test_params(self):
        self.setup_clean_chain = True
        self.num_nodes = 1
        self.extra_args = [["-addresstype=legacy"]]

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def run_test(self):
        message = 'This is just a test message'

        self.log.info('test signing with priv_key')
        priv_key = 'b9Re9AqiHcCp1L1UB9QwFfSHdc5d9vS7km7PX6Vt3sPAjmCAQzYc'
        address = 'cZZY6ATUpST3PWrVnequMHTytE2S7uZGYL'
        expected_signature = 'H16OYOEyKo8Sz3UWB6Qc8kNn3omIw+a6yCtufZGG27d2em1k0Mw8a6L7Im8d/Nnpehv0xwjsAUkecRE0VlUg6/8='
        signature = self.nodes[0].signmessagewithprivkey(priv_key, message)
        assert_equal(expected_signature, signature)
        assert(self.nodes[0].verifymessage(address, signature, message))

        self.log.info('test signing with an address with wallet')
        address = self.nodes[0].getnewaddress()
        signature = self.nodes[0].signmessage(address, message)
        assert(self.nodes[0].verifymessage(address, signature, message))

        self.log.info('test verifying with another address should not work')
        other_address = self.nodes[0].getnewaddress()
        other_signature = self.nodes[0].signmessage(other_address, message)
        assert(not self.nodes[0].verifymessage(other_address, signature, message))
        assert(not self.nodes[0].verifymessage(address, other_signature, message))

        self.log.info('test extracting address from signature')
        res = self.nodes[0].verifymessage("", signature, message)
        assert_equal(res, {
            "valid": True,
            "address": address,
        })
        assert_equal(res["address"], address)

        self.log.info('test extracting address from invalid signature')
        res = self.nodes[0].verifymessage("", base64.b64encode(b"some data").decode("ascii"), message)
        assert_equal(res, {
            "valid": False,
        })

if __name__ == '__main__':
    SignMessagesTest().main()
