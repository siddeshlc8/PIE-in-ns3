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
- [(RFC 8033) Proportional-Integral Controller Enhanced (PIE): A Lightweight Control
Scheme to Address the Bufferbloat Problem ](https://tools.ietf.org/html/rfc8033)
- [PIE: A Lightweight Control Scheme to Address the Bufferbloat Problem ](http://ieeexplore.ieee.org/document/6602305/)
- FreeBSD code of PIE 

  [`/sys/netpfil/ipfw`](https://github.com/freebsd/freebsd/tree/master/sys/netpfil/ipfw)
  ```
  ├── ...
  ├── dn_aqm_pie.c 
  ├── dn_aqm_pie.h 
  ├── dn_sched_fq_pie.c 
  └── ...
  ```
- ns-3 code of PIE 

  [`/src/traffic-control/model`](https://github.com/nsnam/ns-3-dev-git/tree/master/src/traffic-control/model)
  ```
  ├── ...
  ├── pie-queue-disc.h
  ├── pie-queue-disc.cc
  └── ...
  ```
## Contents
```
.
├── results
|    ├── testcase-1
│    │   ├── gnuPlotScript
│    │   ├── Inst-Queue-First.png
│    │   ├── pie-queue-delay-first-DepRate.plotme
│    │   ├── pie-queue-delay-first-UseTsp.plotme
│    │   ├── pie-queue-size-first-DepRate.plotme
│    │   ├── pie-queue-delay-first-UseTsp.plotme
│    │   ├── pie-throughput-first-DepRate.plotme
│    │   ├── pie-throughput-first-UseTsp.plotme
│    │   ├── Queue-Delay-First.png
│    │   ├── Throughput-First.png
│    │   ├── Topology-First-running.png
│    │   └── Topology-First.png
|    | 
│    ├── testcase-2
│    │   ├── gnuPlotScript
│    │   ├── Inst-Queue-Second.png
│    │   ├── pie-queue-delay-second-DepRate.plotme
│    │   ├── pie-queue-delay-second-UseTsp.plotme
│    │   ├── pie-queue-size-second-DepRate.plotme
│    │   ├── pie-queue-delay-second-UseTsp.plotme
│    │   ├── pie-throughput-second-DepRate.plotme
│    │   ├── pie-throughput-second-UseTsp.plotme
│    │   ├── Queue-Delay-Second.png
│    │   ├── Throughput-Second.png
│    │   ├── Topology-Second-running.png
|    │   └── Topology-Second.png
|    |
|    ├── ...
|    └── Readme.md
├── src
│   └── traffic-control
│       └── model
│           ├── pie-queue-disc.cc
|           └── pie-queue-disc.h
|
├── test-cases
│   ├── first-bulksend.cc
|   └── second-bulksend.cc
|
├── .gitignore
└── README.md
```