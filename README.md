#  FreeBSD like PIE for ns-3

## About

PIE calculates the queuing delay by using Little’s Law. Another alternative approach to
calculate queuing delay in PIE is to use CoDel-like timestamping approach. RFC 8033
recommends any one of the above-mentioned approaches to calculating queuing delay in PIE.
Currently, all the implementations of PIE (except in FreeBSD) use Little’s Law for calculating
queueing delay. In these projects, the main aim is to understand how FreeBSD using
timestamping based approach to calculate queuing delay in PIE and implement the same in ns-3

## Week-1 Synopsis
* [ Understanding PIE AQM Algorithm ](https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-1-Synopsis#pie-proportional-integral-controller-enhanced)
* [ Understanding PIE Implementation in ns-3 ](https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-1-Synopsis#implementation-of-pie-in-ns-3)
* [ Understanding PIE Implementation in FreeBSD ](https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-1-Synopsis#implementation-of-pie-in-freebsd )
* [How PIE Implementation in FreeBSD is different from ns-3]( https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-1-Synopsis#difference-between-ns-3--freebsd-pie-implementation)

## Week-2 Synopsis
* [ Implementation of TimeStamp method of delay calculation in ns-3. ](https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-2-Synopsis#implementation)
* [ Testing the Implementation against departure rate calculation method. ](https://github.com/siddeshlc8/freebsd-pie-in-ns3/wiki/Week-2-Synopsis#testing)

## Recommended Reading
- (RFC 8033) Proportional-Integral Controller Enhanced (PIE): A Lightweight Control
Scheme to Address the Bufferbloat Problem (Link: https://tools.ietf.org/html/rfc8033)
- PIE: A Lightweight Control Scheme to Address the Bufferbloat Problem (Link:
http://ieeexplore.ieee.org/document/6602305/)
- FreeBSD code of PIE 

  [`/sys/netpfil/ipfw`](https://github.com/freebsd/freebsd/tree/master/sys/netpfil/ipfw)
  ```
  ├── dn_aqm_pie.c 
  ├── dn_aqm_pie.h 
  ├── dn_sched_fq_pie.c 
  ```
- ns-3 code of PIE 

  [`/src/traffic-control/model`](https://github.com/nsnam/ns-3-dev-git/tree/master/src/traffic-control/model)
  ```
  ├── pie-queue-disc.h
  ├── pie-queue-disc.cc
  ```
