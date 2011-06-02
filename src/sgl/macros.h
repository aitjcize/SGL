#ifndef __SGL_MACROS_H__
#define __SGL_MACROS_H__

/* Clamp X to [MIN,MAX] */
#define CLAMP( X, MIN, MAX )  ( (X)<(MIN) ? (MIN) : ((X)>(MAX) ? (MAX) : (X)) )

/* Minimum of two values: */
#define MIN2( A, B )   ( (A)<(B) ? (A) : (B) )

/* Maximum of two values: */
#define MAX2( A, B )   ( (A)>(B) ? (A) : (B) )

/* Dot product of two 2-element vectors */
#define DOT2( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] )

/* Dot product of two 3-element vectors */
#define DOT3( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2] )

/* Dot product of two 4-element vectors */
#define DOT4( a, b )  ( (a)[0]*(b)[0] + (a)[1]*(b)[1] + \
            (a)[2]*(b)[2] + (a)[3]*(b)[3] )

/* Dot product of two 4-element vectors */
#define DOT4V(v,a,b,c,d) (v[0]*(a) + v[1]*(b) + v[2]*(c) + v[3]*(d))


/* Cross product of two 3-element vectors */
#define CROSS3(n, u, v)             \
do {                        \
   (n)[0] = (u)[1]*(v)[2] - (u)[2]*(v)[1];  \
   (n)[1] = (u)[2]*(v)[0] - (u)[0]*(v)[2];  \
   (n)[2] = (u)[0]*(v)[1] - (u)[1]*(v)[0];  \
} while (0)


/* Normalize a 3-element vector to unit length. */
#define NORMALIZE_3FV( V )          \
do {                        \
   GLfloat len = (GLfloat) LEN_SQUARED_3FV(V);  \
   if (len) {                   \
      len = INV_SQRTF(len);         \
      (V)[0] = (GLfloat) ((V)[0] * len);    \
      (V)[1] = (GLfloat) ((V)[1] * len);    \
      (V)[2] = (GLfloat) ((V)[2] * len);    \
   }                        \
} while(0)

#define LEN_3FV( V ) (SQRTF((V)[0]*(V)[0]+(V)[1]*(V)[1]+(V)[2]*(V)[2]))
#define LEN_2FV( V ) (SQRTF((V)[0]*(V)[0]+(V)[1]*(V)[1]))

#define LEN_SQUARED_3FV( V ) ((V)[0]*(V)[0]+(V)[1]*(V)[1]+(V)[2]*(V)[2])
#define LEN_SQUARED_2FV( V ) ((V)[0]*(V)[0]+(V)[1]*(V)[1])

#endif /* __SGL_MACROS_H__ */
