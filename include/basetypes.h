ifdef(`TYPES_H',, `

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef long int32_t;
typedef unsigned long uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

/* TODO: this needs to be changed! it is actually total nonsense, but
 * i want to use size_t, so lets try to have a good interface and change
 * the implementation later. */
typedef unsigned int size_t;

') /* TYPES_H */