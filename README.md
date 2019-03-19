# FreeBSD like PIE for ns-3

    PIE calculates the queuing delay by using Little’s Law. Another alternative approach to
    calculate queuing delay in PIE is to use CoDel-like timestamping approach. RFC 8033
    recommends any one of the above mentioned approaches to calculate queuing delay in PIE.
    Currently, all the implementations of PIE (except in FreeBSD) use Little’s Law for calculating
    queueing delay. In these projects, the main aim is to understand how FreeBSD using
    timestamping based approach to calculate queuing delay in PIE and implement the same in
    ns-3
