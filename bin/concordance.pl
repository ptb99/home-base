#! /usr/bin/env perl

my %dict;

while (<>) {

    my @F = split;

    for (my $i=0; $i<@F; $i++) {
	$dict{ $F[$i] }++;
    };

};

for my $i (sort {$dict{$a} <=> $dict{$b}} keys %dict) {
    print "$i $dict{$i}\n";
};
