#! /usr/bin/perl

use strict;

my @CHARS = qw(A B C D E F G H I J K L);


# read in candidate tourney
my @LIST = (<>);
my $target = ($#LIST + 1)*4 / ($#CHARS + 1);

# check all singles
foreach my $i (@CHARS) {
    my $count = grep(/$i/, @LIST);
    print "$i: ", $count, "\n" if ($count != $target);
}
print "\n";

# check all pairs
foreach my $i (@CHARS) {
    foreach my $j (@CHARS) {

	next unless ($i lt $j);

	my $count = grep(/($i.*$j|$j.*$i)/, @LIST);
	print "$i$j: ", $count, "\n";
    }
}
