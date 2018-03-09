#!/bin/sh
#
# Copyright (C) 2000, 2001, 2004, 2007, 2012, 2014, 2016, 2018  Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

SYSTEMTESTTOP=..
. $SYSTEMTESTTOP/conf.sh

if $PERL -e 'use Net::DNS;' 2>/dev/null
then
    if $PERL -e 'use Net::DNS; die if ($Net::DNS::VERSION eq "0.76" || $Net::DNS::VERSION eq "0.77");' 2>/dev/null
    then
	:
    else
	echo_i "Net::DNS version 0.76 and 0.77 have a bug that causes this test to fail: please update." >&2
	exit 1
    fi
else
    echo_i "This test requires the Net::DNS library." >&2
    exit 1
fi

exec $SHELL ../testcrypto.sh
