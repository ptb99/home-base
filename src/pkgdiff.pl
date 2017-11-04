#! /usr/bin/perl

$olddir = "/opt/var-db-old/pkg";

open (PKGS, "pkg_info -Ia|") || die "pkg_info failed: $!";
while (<PKGS>) {
  ($a, $b) = split (' ', $_, 9999);
  #print "$a\n";
  $a =~ s/(.*)-[^-]+/$1/;
  #print "$a\n";
  $array{$a} = $_;
  #print "$array{$a}";
}

#print %array;

opendir (OPKGDIR, $olddir) || die "can't opendir $olddir: $!";
while ($f = readdir(OPKGDIR)) {
  $c = $f;
  #print "$c\n";
  $c =~ s/(.*)-[^-]+/$1/;
  #print "$c\n";
  if ($array{$c}) {
    ($tag, $b) = split (' ', $array{$c}, 9999);
    printf "%-30s %-30s\n", $f, $tag;
  } else {
    printf "%-30s %-30s\n", $f, "NOT UPDATED";
  }
}


