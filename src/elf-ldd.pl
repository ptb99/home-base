#! /usr/bin/perl

###
### This does the equivalent of ldd, but includes shlibs.  Especially
### useful for tracking down which shlib of a whole net of
### dependencies is responsible for an out-of-date shlib reference.
###


# turn on autoflush to stdout:
$| = 1;


my @Default_RPATH = qw(/usr/X11R6/lib /usr/local/lib /usr/lib);

sub getRpath {
   my $file = shift;

   open (IN, "objdump -x $file 2>/dev/null |")
     or die ("failed to objdump: $!");
   while (<IN>) {
      if (/^\s*RPATH/) {
	 my @F = split;
	 return split(/:/, @F[1]);
      }
   }
   close (IN);

   # if no RPATH found, return a defualt list
   return @Default_RPATH;
}



sub getDepends {
   my $file = shift;
   my @libs = ();

   open (IN, "objdump -x $file 2>/dev/null |")
     or die ("failed to objdump: $!");
   while (<IN>) {
      if (/^\s*NEEDED/) {
	 my @F = split();
	 my $lib = $F[1];
	 push (@libs, $lib);
      }
   }
   close (IN);
   return @libs;
}


# global list of libs we've checked
my %DEPS = ();

sub checkFile {
   my ($lib, @rpath) = @_;

   return 1 if ($DEPS{$lib} == 1);

   # After we're through, register this lib (found or not);
   $DEPS{lib} = 1;

   ##print "checkFile $lib in ", join(' ', @rpath), "\n";

   # check for absolute paths (or lib in . ):
   return 1 if ( -f $lib);

   for my $dir (@rpath) {
      if ( -f $dir . "/" . $lib ) {
	 ##print "checkFile found $lib in $dir\n";
	 return 1 ;
      }
   }

   # This is the not found case.
   return 0;
}


sub checkAllDeps {
   my ($msg, $lib, @rpath) = @_;

   if (! checkFile ($lib, @rpath) ) {
      print $msg, " -> ", $lib, " not found\n";
   }

   for my $i ( getDepends($lib) ) {
      ##print "checking $i\n";
      checkAllDeps ("$lib", $i, @rpath);
   }
}



###
### Main program
###


for my $i (@ARGV) {

   print $i, ":\n";
   my @RPATH = getRpath ($i);
   push (@RPATH, @Default_RPATH);
   checkAllDeps ($i, $i, @RPATH);
}
