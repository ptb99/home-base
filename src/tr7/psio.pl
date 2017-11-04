#! /usr/bin/perl

###
### using the ps command, track the top I/O processes
###



use strict;

# Make output flush properly:
use FileHandle;
autoflush STDOUT 1;


### params:
my $TOP = 10;
my $DT = 10;


my %NEW;
my %OLD;
my %CMD;


# run ps command, get I/O stats
sub getProcs {

    # copy new to old:
    %OLD = %NEW; 

    open (PS, "ps -axw -o pid,inblk,oublk,command|") or
	die ("Failed to run ps: $!");

    while (<PS>) {
	my @F = split;
	next unless ($F[0] =~ /\d+/);

	my $pid = shift(@F);
	$NEW{$pid} = [ shift(@F), shift(@F) ];
	$CMD{$pid} = join (' ', @F);
    }

    close (PS);
}



### start of main

while (1) {
    getProcs();

    my %RW = ();
    foreach my $i (keys %NEW) {
	next unless (exists $OLD{$i});

	my $next = $NEW{$i};
	my $last = $OLD{$i};

	my $r = $next->[0] - $last->[0];
	my $w = $next->[1] - $last->[1];

	$RW{$i} = [ $r, $w, ($r + $w) ];
    }

    print (scalar localtime() . "\n") unless (keys(%RW) == 0);

    my $count = $TOP;
    foreach my $i (sort { $RW{$b}->[2] <=> $RW{$a}->[2] } keys %RW) {
	my $io = $RW{$i};
	last if ($io->[2] == 0);

	printf ("%5d %4d R %4d W - %s\n", $i, $io->[0], $io->[1], $CMD{$i});
	--$count;

	last if ($count == 0);
    }
    print "\n";

    sleep($DT);
}
