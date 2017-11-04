#! /usr/bin/perl

foreach $i (@ARGV) {

    printf ("%d\n", ($i + 16384) % 32768 + 32768 );
}
