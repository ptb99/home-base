#! /usr/bin/perl

#use strict;
use Socket;
use FileHandle;
 


$nocache = 0;
$timeout = 60;

# For the user "From:" string:
chop($user = `id -un`);
chop($thishost = `hostname`);

$port = "80";
$request = "/";
$httplevel = "1.0";
$pragma = $nocache ? "Pragma: no-cache\r\n" : "";
$useragent  =
  "User-Agent: Mozilla/3.0 (just kidding... httpd-info.pl robot)\r\n";
$from       = "From: $user\@$thishost\r\n";
$mimeaccept = "Accept: */*\r\n";     #-- accept any mime type
$keepalive  = "Connection: keep-alive\r\n";
$encoding   = "Accept-Encoding: identity\r\n";



while ($host = shift(@ARGV)) {

  if ($host =~ /(.+):(\d+)/) {
      $host = $1;
      $port = $2;
  }

  @REQ = ("HEAD $request HTTP/$httplevel\r\n",
	  "Host: $host\r\n",
	  "$useragent",
	  "$from",
	  "$mimeaccept",
	  "$pragma",
	  "\r\n");

  my $header = join ('', @REQ);

  ## Debug
  ##print "sending:\n$header";

  $remoteaddr = gethostbyname($host);
  $remote = sockaddr_in($port, $remoteaddr);
  $proto = getprotobyname('tcp');
  socket(FS, &Socket::AF_INET, &Socket::SOCK_STREAM, $proto) ||
    die "socket $!";

  connect(FS, $remote) || die ("connect failed: $!");
  ##print "connected to $host\n";
  FS->autoflush(1);

  print FS "$header\r\n";

  while (<FS>) {
    print $_;
  }
  close FS;

}
