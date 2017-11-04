//                              -*- Mode: C++ -*-
//
// pcapurlstats.cc -- search pcap file for URL hostname stats
//
// $Id$
//
// Copyright (C) 2007  Network Physics Inc.  All rights reserved.
//
// Creator         : Tom Pavel  <pavel>     Mon Feb 24 18:41:28 2003
// Modifier        : Tom Pavel  <tom>     Sat May 12 16:35:13 2007
// Update Count    : 34
// Status          : Unknown, Use with caution!
//


// C++ headers
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;


// C headers
#include <unistd.h>
#include <string.h>
#include <pcap.h>

// use the BSD-style tcphdr:
#ifdef __linux__
#define __FAVOR_BSD
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>	// for n_long typedef on FreeBSD
#include <net/ethernet.h>
#ifdef __FreeBSD__
#include <net/if.h>
#include <net/if_vlan_var.h>
#endif
#include <netinet/ip.h>
#include <netinet/tcp.h>



size_t
getHdrLen (const u_char* pktData, size_t capLen)
{
  size_t hdrLen = 0;
  const ether_header* eh = reinterpret_cast<const ether_header*>(pktData);
  if (capLen < sizeof(*eh)) {
    cerr << "getHdrLen(): short pkt" << endl;
    abort();
  }

  struct ip *iph = 0;
  if ((ntohs(eh->ether_type)== ETHERTYPE_IP)) {
    hdrLen += sizeof(*eh);
    capLen -= sizeof(*eh);

    // Advance the pointer to the end of the ether header and look at IP:
    iph = (struct ip *) (eh + 1);

#ifdef __FreeBSD__
  } else if (ntohs(eh->ether_type) == ETHERTYPE_VLAN) {
    const ether_vlan_header* evh =
      reinterpret_cast<const ether_vlan_header*>(eh);
    hdrLen += sizeof(*evh);
    capLen -= sizeof(*evh);

    if ( ntohs(evh->evl_proto) != ETHERTYPE_IP ) {
      cerr << "getHdrLen(): unknown pkt type, check pcap filter!" << endl;
      abort();
    } 

    iph = (struct ip *)(evh + 1);
#endif

  } else {
    cerr << "getHdrLen(): unknown pkt type, check pcap filter!" << endl;
    abort();
  }

  if (capLen < sizeof(*iph)) {
    cerr << "getHdrLen(): short pkt" << endl;
    abort();
  }

  size_t ipLen = 4*iph->ip_hl;
  hdrLen += ipLen;
  capLen -= ipLen;
  struct tcphdr* tph = (tcphdr*) ((char*)iph + ipLen);

  if (capLen < sizeof(*tph)) {
    cerr << "getHdrLen(): short pkt" << endl;
    abort();
  }

  hdrLen += 4 * tph->th_off;
  return hdrLen;
}


void
findHostname (const u_char* pktData, size_t len)
{
  char* pStart = (char*)pktData; // get consistent typing
  size_t hdrOffset = getHdrLen (pktData, len);

#ifdef __FreeBSD__
  char* hl = strnstr (pStart + hdrOffset, "Host:", len-hdrOffset);
#else
  char* hl = strstr (pStart + hdrOffset, "Host:");
#endif

  // protect against problems...
  if (hl == 0) {
    cout << "Host: not found!" << endl;
    return;
  }

  char* end = (char*) memchr (hl, '\n', len - (hl - pStart));

  size_t hostlen = end - hl;
  char buf[128];
  if (hostlen > sizeof(buf)) {
    strncpy (buf, hl, sizeof(buf));
    buf[sizeof(buf)] = '\0';
  } else {
    strncpy (buf, hl, hostlen);
    buf[hostlen-1] = '\0';
  }
 
  cout << setw(4) << (hl - pStart) << " "
       << setw(4) << (end - pStart) << "  "
       << buf << endl;
}



void
Usage (const char* progname)
{
    cerr << "usage: " << progname << " [-d] file1" << endl;
    exit(1);
}

int
main (int argc, char** argv)
{
    // command-line parsing
    int debug = 0;
    const char* outFile = "-";
    const char* progname = argv[0];

    int ch;
    while ((ch = getopt(argc, argv, "do:")) != EOF) {
      switch ((char)ch) {
      case 'd':
	++debug;
	break;
      case 'o':
	outFile = optarg;
	break;
      case '?':
      default:
	Usage(progname);
      }
    }

    argc -= optind;
    argv += optind;
    if (argc != 1)
      Usage(progname);


    // open inputs
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* inHndl = pcap_open_offline (argv[0], errbuf);
    if (inHndl == 0) {
      cerr << progname << ": " << errbuf << endl;
      return 1;
    }


    // setup filter express to get request pkts only
    struct bpf_program filter;
    if (pcap_compile(inHndl, &filter,
		     "tcp dst port 80 and len > 80 and "
		     "tcp[tcpflags] & (tcp-rst|tcp-syn|tcp-fin) == 0",
		     1, 0) < 0) {
	cerr << "pcap_compile: " << pcap_geterr(inHndl)
	     << endl;
	return 1;
    }
    if (pcap_setfilter(inHndl, &filter) < 0) {
	cerr << "pcap_setfilter: " << pcap_geterr(inHndl)
	     << endl;
	return 1;
    }


    // infinite loop (until EOF)
    while (true) {
      pcap_pkthdr* pktHdr;
      const u_char* pktData;

      int rc = pcap_next_ex(inHndl, &pktHdr, &pktData);
      if (rc == -1) {
	cerr << "pcap_next_ex: " << pcap_geterr(inHndl)
	     << endl;
	break;
      }
      if (rc == -2) {
	// EOF
	pcap_close (inHndl);
	break;
      }

      // process pkt
      findHostname (pktData, pktHdr->caplen);
    }

    return 0;
}
