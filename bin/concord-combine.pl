#! /usr/bin/perl

my %dict;

while (<>) {
    # presume format of "tag count":
    my @F = split;

    $dict{ $F[$0] } += $F[1];
};

for my $i (sort {$dict{$b} <=> $dict{$a}} keys %dict) {
    print "$i $dict{$i}\n";
};
