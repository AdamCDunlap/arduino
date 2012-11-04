#ifndef RUNEVERY_H
#define RUNEVERY_H

#define _RETYPE(t) uint16_t
#define _runEveryBase(t,f) for \
    (static _RETYPE(t) _lasttime;\
    (_RETYPE(t))((_RETYPE(t))f - _lasttime) >= (t);\
     _lasttime += (t))

#define runEvery(t) _runEveryBase(t,millis())

#define runEveryMicros(t) _runEveryBase(t,micros())

#endif // RUNEVERY_H
