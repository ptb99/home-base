#! /usr/bin/perl

use Socket;

foreach $i (@ARGV) {
    
    my @x = (substr($i,0,2),
	     substr($i,2,2),
	     substr($i,4,2),
	     substr($i,6,2));

    my @y = map { hex($_) } @x;
    print join('.', @y), "\n";
}
