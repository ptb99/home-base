#! /usr/bin/perl


sub printheader {
  print "  %    self                self\n";
  print " time seconds    calls   us/call  name\n";
  print "----- ------  --------  -------  --------------------------------------
---\n";
}


$doprinting = 0;

while (<>) {

  if (/^flat profile:/) {
    printheader;
    $doprinting = 2;
  }

  if ($doprinting && /^granularity:.* ([\d\.]+) seconds$/) {
    $total = $1 ;
    ##print "total of $total found\n";
  }


  if ($doprinting && /^ *\d/) {
    my @F = split;
    my ($time, $calls) = ($F[2], $F[3]);
    my $label = substr($_, 54);

    if ($label =~ /mcount/) {
      $total -= $time;
    } else {
      my $tper = ($calls > 0) ? $time/$calls*1e6 : 0;
      printf ("%5.2f %6.2f %9d %9.3f %s",
              $time/$total*100, $time, $calls, $tper, $label);
    }
  }

  $doprinting-- if ($doprinting && /^\014/);

}
