#ifndef DRIFT_MATH_H
#define DRIFT_MATH_H

#define Bytes(n)     (n)
#define Kilobytes(n) (Bytes(n)*1024LL)
#define Megabytes(n) (Kilobytes(n)*1024LL)
#define Gigabytes(n) (Megabytes(n)*1024LL)
#define Terabytes(n) (Gigabytes(n)*1024LL)

#define ArraySize(arr)        (sizeof(arr) / sizeof((arr)[0]))
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))
#define AbsoluteValue(a)      ((a) > 0 ? (a) : -(a))
#define Lerp(a, b, t)         (a + (b - a) * t)
#define Clamp01(val)          Min(1.f, Max(0.f, (val)))
#define Clamp(min, max, val)  Min((max), Max((min), (val)))
#define Square(a)             ((a) * (a))

#endif
