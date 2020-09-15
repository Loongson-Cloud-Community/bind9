#!/usr/bin/perl
#
# Copyright (C) Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at https://mozilla.org/MPL/2.0/.
#
# See the COPYRIGHT file distributed with this work for additional
# information regarding copyright ownership.

use warnings;
use strict;
use Time::Piece;

if (@ARGV < 1) {
	print STDERR <<'END';
usage:
    perl docbook-zoneopt.pl zoneopt_file [YYYY]
END
	exit 1;
}

my $FILE = shift;

my $t = Time::Piece->new();
my $year;
$year = `git log --max-count=1 --date=format:%Y --format='%cd' -- $FILE` or $year = $t->year;
chomp $year;

open (FH, "<", $FILE) or die "Can't open $FILE";

print <<END;
<!--
 - Copyright (C) 2004-$year  Internet Systems Consortium, Inc. ("ISC")
 -
 - This Source Code Form is subject to the terms of the Mozilla Public
 - License, v. 2.0. If a copy of the MPL was not distributed with this
 - file, You can obtain one at http://mozilla.org/MPL/2.0/.
-->

<!-- Generated by doc/misc/docbook-zoneopt.pl -->
<programlisting>
END

while (<FH>) {
	if (m{// not.*implemented} || m{// obsolete} || m{// test.*only}) {
		next;
	}

	s{ // not configured}{};
	s{ // may occur multiple times}{};
	s{<([a-z0-9_-]+)>}{<replaceable>$1</replaceable>}g;
	s{^(\s*)([a-z0-9_-]+)\b}{$1<command>$2</command>};
	s{[[]}{[}g;
	s{[]]}{]}g;
	s{        }{\t}g;

	print;
}

print <<END;
</programlisting>
END
