////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//
// "Contains BIGDIGITS multiple-precision arithmetic code originally
// written by David Ireland, copyright (c) 2001-6 by D.I. Management
// Services Pty Limited <www.di-mgt.com.au>, and is used with
// permission."
//
////////////////////////////////////////////////////////////////////////////////

#define CRYPTO_RSA_C
#define HW_RSA2048

#include <secure/crypto_rsa.h>
#include <secure/crypto_func.h>
#include <secure/crypto_sha.h>
#include <common.h>
#include <MsSystem.h>
#include <MsDebug.h>
#include <MsTypes.h>
#include <secure/MsSecureCommon.h>
#include <drvAESDMA.h>

#if (!defined(CONFIG_SECURE_HW_IP)) && (!defined(CONFIG_SECURE_SMC_HW_IP))
#define MAX_DIGIT                   0xFFFFFFFFUL
#define MAX_HALF_DIGIT              0xFFFFUL	/* NB 'L' */
#define BITS_PER_DIGIT              32
#define HIBITMASK                   0x80000000UL

#define BITS_PER_HALF_DIGIT         (BITS_PER_DIGIT / 2)
#define BYTES_PER_DIGIT             (BITS_PER_DIGIT / 8)

#define LOHALF(x)                   ((DIGIT_T)((x) & MAX_HALF_DIGIT))
#define HIHALF(x)                   ((DIGIT_T)((x) >> BITS_PER_HALF_DIGIT & MAX_HALF_DIGIT))
#define TOHIGH(x)                   ((DIGIT_T)((x) << BITS_PER_HALF_DIGIT))

#define mpNEXTBITMASK(mask, n) do{if(mask==1){mask=HIBITMASK;n--;}else{mask>>=1;}}while(0)
#endif

#if defined(CONFIG_SECURE_SMC_HW_IP)
int SMC_RSA2048HWdecrypt(FastCryptoCalParam64 *CryptoPara)
{
    UBOOT_TRACE("IN\n");
    if((CryptoPara->input_address== 0)||(CryptoPara->output_address== 0))
    {
        UBOOT_ERROR("The input parameters are not correct\n");
        return -1;
    }

    flush_cache(CryptoPara, sizeof(FastCryptoCalParam64));
    flush_cache(CryptoPara->input_address, CryptoPara->input_size);
    flush_cache(CryptoPara->output_address, CryptoPara->output_size);
    flush_cache(CryptoPara->swkey, CryptoPara->swkey_len);

    __asm__ __volatile__(
        "ldr r0, =0xb2005860 \n\t"
        "mov r1,%0 \n\t"
        "smc #0 \n\t"
        :
        : "r"(CryptoPara)
        : "r0","r1"
    );

    UBOOT_TRACE("OK\n");
    return 0;
}

int RSA2048HWdecrypt(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_E, unsigned char *Sim_SignOut)
{
    UBOOT_TRACE("IN\n");
    uint8_t temp_rsa_256_result[SIGNATURE_LEN+16] = {0x00};
    uint8_t *rsa_256_result = temp_rsa_256_result;

    unsigned char *rsa_key260 = (unsigned char *)malloc(260 * sizeof(unsigned char));

    if( (MS_U32)temp_rsa_256_result % 0x10 != 0 )
        rsa_256_result = (MS_U32)rsa_256_result + (0x10-((MS_U32)temp_rsa_256_result % 0x10));


    UBOOT_DUMP((unsigned int)PublicKey_N, 32);
    UBOOT_DUMP((unsigned int)PublicKey_E, 32);

    memset(rsa_key260, 0, RSA_PUBLIC_KEY_LEN);
    memcpy(rsa_key260, PublicKey_N, RSA_PUBLIC_KEY_N_LEN);
    memcpy(rsa_key260+RSA_PUBLIC_KEY_N_LEN, PublicKey_E, RSA_PUBLIC_KEY_E_LEN);

    FastCryptoCalParam64 CryptoPara;
    memset(&CryptoPara, 0, sizeof(FastCryptoCalParam64));
    CryptoPara.mode = MODE_VERIFY;
    CryptoPara.secure_algo = ALG_RSA2048_PUBLIC;
    CryptoPara.key_type = KEYBANK_INDEX;
    CryptoPara.key_index = 1;
    CryptoPara.input_address = (uint64_t)Signature;
    CryptoPara.input_size = SIGNATURE_LEN;
    CryptoPara.output_address = (uint64_t)rsa_256_result;
    CryptoPara.output_size = SIGNATURE_LEN;
    CryptoPara.swkey = (uint64_t) rsa_key260;
    CryptoPara.swkey_len= RSA_PUBLIC_KEY_LEN;
    CryptoPara.iv = (uint64_t) PublicKey_N;
    CryptoPara.iv_len = 256;

    UBOOT_TRACE("IN\n");

    UBOOT_DEBUG("\nBefore\n");
    UBOOT_DUMP((unsigned int)Signature, 32);
    UBOOT_DUMP((unsigned int)Sim_SignOut, 32);
    UBOOT_DUMP((unsigned int)CryptoPara.input_address, 32);
    UBOOT_DUMP((unsigned int)CryptoPara.output_address, 32);
    UBOOT_DEBUG("\nKEY=\n");
    UBOOT_DUMP((unsigned int)CryptoPara.swkey, 260);

    SMC_RSA2048HWdecrypt(&CryptoPara);

    UBOOT_DEBUG("\nAfter\n");
    UBOOT_DUMP((unsigned int)Signature, 32);
    UBOOT_DUMP((unsigned int)Sim_SignOut, 32);
    UBOOT_DUMP((unsigned int)CryptoPara.input_address, 32);
    UBOOT_DUMP((unsigned int)CryptoPara.output_address, 32);

    memcpy(Sim_SignOut, CryptoPara.output_address, SIGNATURE_LEN);
    //UBOOT_DEBUG("\nAfter shift 224\n");
    //UBOOT_DUMP((unsigned int)Sim_SignOut, 32);
    //UBOOT_DUMP((unsigned int)CryptoPara.output_address, 256);
    free(rsa_key260);
    UBOOT_TRACE("OK\n");
    return 0;
}
#elif defined(CONFIG_SECURE_HW_IP)
void RSA_copy_reverse(unsigned char *Sim_SignOut, unsigned char *RSAOut, int len)
{
    int i=0;
    for(i=0; i<len; i++)
    {
        Sim_SignOut[i] = RSAOut[len-i-1];
    }
}

int RSA2048HWdecrypt(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_E, unsigned char *Sim_SignOut)
{
   UBOOT_TRACE("IN\n");
   if(NULL == Signature || NULL == PublicKey_N || NULL == PublicKey_E || NULL == Sim_SignOut)
   {
       UBOOT_ERROR("parameters Error\n");
       return -1;
   }
   DRVAESDMA_RESULT result = DRVAESDMA_OK;
   DrvAESDMA_RSASig RSASig;
   DrvAESDMA_RSAOut RSAOut;
   MDrv_AESDMA_Init(0x00, 0x20000000, 2); // wait the AESDMA.a
   memcpy(&RSASig,Signature,sizeof(RSASig));
   memset(&RSAOut,0,sizeof(RSAOut));
   DrvAESDMA_RSAKey RSAKey;
   memset(&RSAKey,0,sizeof(RSAKey));
   memcpy((unsigned char*)(RSAKey.u32KeyN),PublicKey_N,RSA_PUBLIC_KEY_N_LEN);
   memcpy((unsigned char*)(RSAKey.u32KeyE),PublicKey_E,RSA_PUBLIC_KEY_E_LEN);
   flush_cache((MS_U32)&RSASig,SIGNATURE_LEN);
   result=MDrv_RSA_Calculate(&RSASig,&RSAKey,E_DRVAESDMA_RSA2048_PUBLIC);
   if(DRVAESDMA_OK != result)
   {
      UBOOT_DEBUG("RSA HW decrypt error1\n");
      return -1;
   }
   memset((U8*)&RSAOut,0,sizeof(DrvAESDMA_RSAOut));
   while(DRVAESDMA_OK != MDrv_RSA_IsFinished());

   result= MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PUBLIC, &RSAOut);
   
   if(DRVAESDMA_OK != result)
   {
      UBOOT_DEBUG("RSA HW decrypt error2\n");
      return -1;
   }
   flush_cache((MS_U32)(&RSAOut),256);
   //memcpy(Sim_SignOut,&RSAOut,sizeof(RSAOut));
   
   // the output from MDrv_RSA_Output is reverse order
   // need to be reversed again to normal order.
   RSA_copy_reverse(Sim_SignOut,&RSAOut,sizeof(RSAOut));
   UBOOT_TRACE("OK\n");
   return 0;
}
#else
int _spMultiply(DIGIT_T p[2], DIGIT_T x, DIGIT_T y)
{
    /*  Computes p = x * y */
    /*  Ref: Arbitrary Precision Computation
    http://numbers.computation.free.fr/Constants/constants.html

         high    p1                p0     low
        +--------+--------+--------+--------+
        |      x1*y1      |      x0*y0      |
        +--------+--------+--------+--------+
               +-+--------+--------+
               |1| (x0*y1 + x1*y1) |
               +-+--------+--------+
                ^carry from adding (x0*y1+x1*y1) together
                        +-+
                        |1|< carry from adding LOHALF t
                        +-+  to high half of p0
    */
    DIGIT_T x0, y0, x1, y1;
    DIGIT_T t, u, carry;

    /*  Split each x,y into two halves
        x = x0 + B*x1
        y = y0 + B*y1
        where B = 2^16, half the digit size
        Product is
        xy = x0y0 + B(x0y1 + x1y0) + B^2(x1y1)
    */

    x0 = LOHALF(x);
    x1 = HIHALF(x);
    y0 = LOHALF(y);
    y1 = HIHALF(y);

    /* Calc low part - no carry */
    p[0] = x0 * y0;
    /* Calc middle part */
    t = x0 * y1;
    u = x1 * y0;
    t += u;
    if (t < u)
    {
        carry = 1;
    }
    else
    {
        carry = 0;
    }

    /*  This carry will go to high half of p[1]
        + high half of t into low half of p[1] */
    carry = TOHIGH(carry) + HIHALF(t);
    /* Add low half of t to high half of p[0] */
    t = TOHIGH(t);
    p[0] += t;
    if (p[0] < t)
    {
        carry++;
    }

    p[1] = x1 * y1;
    p[1] += carry;

    return 0;
}

static void _spMultSub(DIGIT_T uu[2], DIGIT_T qhat, DIGIT_T v1, DIGIT_T v0)
{
    /*  Compute uu = uu - q(v1v0)
        where uu = u3u2u1u0, u3 = 0
        and u_n, v_n are all half-digits
        even though v1, v2 are passed as full digits.
    */
    DIGIT_T p0, p1, t;

    p0 = qhat * v0;
    p1 = qhat * v1;
    t = p0 + TOHIGH(LOHALF(p1));
    uu[0] -= t;
    if (uu[0] > MAX_DIGIT - t)
    {
        uu[1]--;    /* Borrow */
    }
    uu[1] -= HIHALF(p1);
}

DIGIT_T _spDivide(DIGIT_T *q, DIGIT_T *r, const DIGIT_T u[2], DIGIT_T v)
{   /*  Computes quotient q = u / v, remainder r = u mod v
        where u is a double digit
        and q, v, r are single precision digits.
        Returns high digit of quotient (max value is 1)
        CAUTION: Assumes normalised such that v1 >= b/2
        where b is size of HALF_DIGIT
        i.e. the most significant bit of v should be one

        In terms of half-digits in Knuth notation:
        (q2q1q0) = (u4u3u2u1u0) / (v1v0)
        (r1r0) = (u4u3u2u1u0) mod (v1v0)
        for m = 2, n = 2 where u4 = 0
        q2 is either 0 or 1.
        We set q = (q1q0) and return q2 as "overflow"
    */
    DIGIT_T qhat, rhat, t, v0, v1, u0, u1, u2, u3;
    DIGIT_T uu[2], q2;

    /* Check for normalisation */
    if (!(v & HIBITMASK))
    {   /* Stop if assert is working, else return error */
        //assert(v & HIBITMASK);
        *q = *r = 0;
        return MAX_DIGIT;
    }

    /* Split up into half-digits */
    v0 = LOHALF(v);
    v1 = HIHALF(v);
    u0 = LOHALF(u[0]);
    u1 = HIHALF(u[0]);
    u2 = LOHALF(u[1]);
    u3 = HIHALF(u[1]);

    /* Do three rounds of Knuth Algorithm D Vol 2 p272 */

    /*  ROUND 1. Set j = 2 and calculate q2 */
    /*  Estimate qhat = (u4u3)/v1  = 0 or 1
        then set (u4u3u2) -= qhat(v1v0)
        where u4 = 0.
    */
/* [Replaced in Version 2] -->
    qhat = u3 / v1;
    if (qhat > 0)
    {
        rhat = u3 - qhat * v1;
        t = TOHIGH(rhat) | u2;
        if (qhat * v0 > t)
            qhat--;
    }
<-- */
    qhat = (u3 < v1 ? 0 : 1);
    if (qhat > 0)
    {   /* qhat is one, so no need to mult */
        rhat = u3 - v1;
        /* t = r.b + u2 */
        t = TOHIGH(rhat) | u2;
        if (v0 > t)
            qhat--;
    }

    uu[1] = 0;      /* (u4) */
    uu[0] = u[1];   /* (u3u2) */
    if (qhat > 0)
    {
        /* (u4u3u2) -= qhat(v1v0) where u4 = 0 */
        _spMultSub(uu, qhat, v1, v0);
        if (HIHALF(uu[1]) != 0)
        {   /* Add back */
            qhat--;
            uu[0] += v;
            uu[1] = 0;
        }
    }
    q2 = qhat;

    /*  ROUND 2. Set j = 1 and calculate q1 */
    /*  Estimate qhat = (u3u2) / v1
        then set (u3u2u1) -= qhat(v1v0)
    */
    t = uu[0];
    qhat = t / v1;
    rhat = t - qhat * v1;
    /* Test on v0 */
    t = TOHIGH(rhat) | u1;
    if ((qhat == (MAX_HALF_DIGIT + 1)) || (qhat * v0 > t))
    {
        qhat--;
        rhat += v1;
        t = TOHIGH(rhat) | u1;
        if ((rhat < (MAX_HALF_DIGIT + 1)) && (qhat * v0 > t))
            qhat--;
    }

    /*  Multiply and subtract
        (u3u2u1)' = (u3u2u1) - qhat(v1v0)
    */
    uu[1] = HIHALF(uu[0]);  /* (0u3) */
    uu[0] = TOHIGH(LOHALF(uu[0])) | u1; /* (u2u1) */
    _spMultSub(uu, qhat, v1, v0);
    if (HIHALF(uu[1]) != 0)
    {   /* Add back */
        qhat--;
        uu[0] += v;
        uu[1] = 0;
    }

    /* q1 = qhat */
    *q = TOHIGH(qhat);

    /* ROUND 3. Set j = 0 and calculate q0 */
    /*  Estimate qhat = (u2u1) / v1
        then set (u2u1u0) -= qhat(v1v0)
    */
    t = uu[0];
    qhat = t / v1;
    rhat = t - qhat * v1;
    /* Test on v0 */
    t = TOHIGH(rhat) | u0;
    if ((qhat == (MAX_HALF_DIGIT + 1)) || (qhat * v0 > t))
    {
        qhat--;
        rhat += v1;
        t = TOHIGH(rhat) | u0;
        if ((rhat < (MAX_HALF_DIGIT + 1)) && (qhat * v0 > t))
            qhat--;
    }

    /*  Multiply and subtract
        (u2u1u0)" = (u2u1u0)' - qhat(v1v0)
    */
    uu[1] = HIHALF(uu[0]);  /* (0u2) */
    uu[0] = TOHIGH(LOHALF(uu[0])) | u0; /* (u1u0) */
    _spMultSub(uu, qhat, v1, v0);
    if (HIHALF(uu[1]) != 0)
    {   /* Add back */
        qhat--;
        uu[0] += v;
        uu[1] = 0;
    }

    /* q0 = qhat */
    *q |= LOHALF(qhat);

    /* Remainder is in (u1u0) i.e. uu[0] */
    *r = uu[0];
    return q2;
}

int _mpSizeof(const DIGIT_T a[], int ndigits)
{   /* Returns size of significant digits in a */

    while(ndigits--)
    {
        if (a[ndigits] != 0)
            return (++ndigits);
    }
    return 0;
}

void _mpSetEqual(DIGIT_T a[], const DIGIT_T b[], int ndigits)
{
    /* Sets a = b */
    int i;

    for (i = 0; i < ndigits; i++)
    {
        a[i] = b[i];
    }
}

void _mpSetZero(DIGIT_T a[], int ndigits)
{
    /* Sets a = 0 */

    /* Prevent optimiser ignoring this */
    volatile DIGIT_T optdummy;
    DIGIT_T *p = a;

    while (ndigits--)
        a[ndigits] = 0;

    optdummy = *p;
}

void _mpSetDigit(DIGIT_T a[], DIGIT_T d, int ndigits)
{
    /* Sets a = d where d is a single digit */
    int i;

    for (i = 1; i < ndigits; i++)
    {
        a[i] = 0;
    }
    a[0] = d;
}

DIGIT_T _mpShiftLeft(DIGIT_T a[], const DIGIT_T *b, int shift, int ndigits)
{
    /* Computes a = b << shift */
    /* [v2.1] Modified to cope with shift > BITS_PERDIGIT */
    int i, y, nw, bits;
    DIGIT_T mask, carry, nextcarry;

    /* Do we shift whole digits? */
    if (shift >= BITS_PER_DIGIT)
    {
        nw = shift / BITS_PER_DIGIT;
        i = ndigits;
        while (i--)
        {
            if (i >= nw)
                a[i] = b[i-nw];
            else
                a[i] = 0;
        }
        /* Call again to shift bits inside digits */
        bits = shift % BITS_PER_DIGIT;
        carry = b[ndigits-nw] << bits;
        if (bits)
            carry |= _mpShiftLeft(a, a, bits, ndigits);
        return carry;
    }
    else
    {
        bits = shift;
    }

    /* Construct mask = high bits set */
    mask = ~(~(DIGIT_T)0 >> bits);

    y = BITS_PER_DIGIT - bits;
    carry = 0;
    for (i = 0; i < ndigits; i++)
    {
        nextcarry = (b[i] & mask) >> y;
        a[i] = b[i] << bits | carry;
        carry = nextcarry;
    }

    return carry;
}

DIGIT_T _mpShiftRight(DIGIT_T a[], const DIGIT_T b[], int shift, int ndigits)
{
    /* Computes a = b >> shift */
    /* [v2.1] Modified to cope with shift > BITS_PERDIGIT */
    int i, y, nw, bits;
    DIGIT_T mask, carry, nextcarry;

    /* Do we shift whole digits? */
    if (shift >= BITS_PER_DIGIT)
    {
        nw = shift / BITS_PER_DIGIT;
        for (i = 0; i < ndigits; i++)
        {
            if ((i+nw) < ndigits)
                a[i] = b[i+nw];
            else
                a[i] = 0;
        }
        /* Call again to shift bits inside digits */
        bits = shift % BITS_PER_DIGIT;
        carry = b[nw-1] >> bits;
        if (bits)
            carry |= _mpShiftRight(a, a, bits, ndigits);
        return carry;
    }
    else
    {
        bits = shift;
    }

    /* Construct mask to set low bits */
    /* (thanks to Jesse Chisholm for suggesting this improved technique) */
    mask = ~(~(DIGIT_T)0 << bits);

    y = BITS_PER_DIGIT - bits;
    carry = 0;
    i = ndigits;
    while (i--)
    {
        nextcarry = (b[i] & mask) << y;
        a[i] = b[i] >> bits | carry;
        carry = nextcarry;
    }

    return carry;
}

int _mpBitLength(const DIGIT_T d[], int ndigits)
/* Returns no of significant bits in d */
{
    int n, i, bits;
    DIGIT_T mask;

    if (!d || ndigits == 0)
        return 0;

    n = _mpSizeof(d, ndigits);
    if (0 == n)
        return 0;

    for (i = 0, mask = HIBITMASK; mask > 0; mask >>= 1, i++)
    {
        if (d[n-1] & mask)
            break;
    }

    bits = n * BITS_PER_DIGIT - i;

    return bits;
}

DIGIT_T _mpShortDiv(DIGIT_T q[], const DIGIT_T u[], DIGIT_T v, int ndigits)
{
    /*  Calculates quotient q = u div v
        Returns remainder r = u mod v
        where q, u are multiprecision integers of ndigits each
        and r, v are single precision digits.

        Makes no assumptions about normalisation.

        Ref: Knuth Vol 2 Ch 4.3.1 Exercise 16 p625
    */
    int j;
    DIGIT_T t[2], r;
    int shift;
    DIGIT_T bitmask, overflow, *uu;

    if (ndigits == 0)
        return 0;
    if (v == 0)
        return 0;   /* Divide by zero error */

    /*  Normalise first */
    /*  Requires high bit of V
        to be set, so find most signif. bit then shift left,
        i.e. d = 2^shift, u' = u * d, v' = v * d.
    */
    bitmask = HIBITMASK;
    for (shift = 0; shift < BITS_PER_DIGIT; shift++)
    {
        if (v & bitmask)
            break;
        bitmask >>= 1;
    }

    v <<= shift;
    overflow = mpFunc->ShiftLeft(q, u, shift, ndigits);
    uu = q;

    /* Step S1 - modified for extra digit. */
    r = overflow;   /* New digit Un */
    j = ndigits;
    while (j--)
    {
        /* Step S2. */
        t[1] = r;
        t[0] = uu[j];
        overflow = mpFunc->SpDivide(&q[j], &r, t, v);
    }

    /* Unnormalise */
    r >>= shift;

    return r;
}

int _mpEqual(const DIGIT_T a[], const DIGIT_T b[], int ndigits)
{
    /*  Returns true if a == b, else false
    */

    if (ndigits == 0)
        return -1;

    while (ndigits--)
        {
        if (a[ndigits] != b[ndigits])
            return 0;   /* False */
        }

    return 1;    /* True */
}

int _mpCompare(const DIGIT_T a[], const DIGIT_T b[], int ndigits)
{
    /*  Returns sign of (a - b)
    */

    if (ndigits == 0)
        return 0;

    while (ndigits--)
    {
        if (a[ndigits] > b[ndigits])
            return 1;   /* GT */
        if (a[ndigits] < b[ndigits])
            return -1;  /* LT */
    }

    return 0;   /* EQ */
}

DIGIT_T _mpAdd(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits)
{
    /*  Calculates w = u + v
        where w, u, v are multiprecision integers of ndigits each
        Returns carry if overflow. Carry = 0 or 1.

        Ref: Knuth Vol 2 Ch 4.3.1 p 266 Algorithm A.
    */

    DIGIT_T k;
    int j;

    //assert(w != v);

    /* Step A1. Initialise */
    k = 0;

    for (j = 0; j < ndigits; j++)
    {
        /*  Step A2. Add digits w_j = (u_j + v_j + k)
            Set k = 1 if carry (overflow) occurs
        */
        w[j] = u[j] + k;
        if (w[j] < k)
            k = 1;
        else
            k = 0;

        w[j] += v[j];
        if (w[j] < v[j])
            k++;

    }   /* Step A3. Loop on j */

    return k;   /* w_n = k */
}

int _mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits)
{
    /*  Computes product w = u * v
        where u, v are multiprecision integers of ndigits each
        and w is a multiprecision integer of 2*ndigits

        Ref: Knuth Vol 2 Ch 4.3.1 p 268 Algorithm M.
    */

    DIGIT_T k, t[2];
    int i, j, m, n;

    //assert(w != u && w != v);

    m = n = ndigits;

    /* Step M1. Initialise */
    for (i = 0; i < 2 * m; i++)
        w[i] = 0;

    for (j = 0; j < n; j++)
    {
        /* Step M2. Zero multiplier? */
        if (v[j] == 0)
        {
            w[j + m] = 0;
        }
        else
        {
            /* Step M3. Initialise i */
            k = 0;
            for (i = 0; i < m; i++)
            {
                /* Step M4. Multiply and add */
                /* t = u_i * v_j + w_(i+j) + k */
                mpFunc->SpMultiply(t, u[i], v[j]);
                t[0] += k;
                if (t[0] < k)
                    t[1]++;
                t[0] += w[i+j];
                if (t[0] < w[i+j])
                    t[1]++;

                w[i+j] = t[0];
                k = t[1];
            }
            /* Step M5. Loop on i, set w_(j+m) = k */
            w[j+m] = k;
        }
    }   /* Step M6. Loop on j */

    return 0;
}

static DIGIT_T _mpMultSub(DIGIT_T wn, DIGIT_T w[], const DIGIT_T v[], DIGIT_T q, int n)
{   /*  Compute w = w - qv
        where w = (WnW[n-1]...W[0])
        return modified Wn.
    */
    DIGIT_T k, t[2];
    int i;

    if (q == 0) /* No change */
        return wn;

    k = 0;

    for (i = 0; i < n; i++)
    {
        mpFunc->SpMultiply(t, q, v[i]);
        w[i] -= k;
        if (w[i] > MAX_DIGIT - k)
            k = 1;
        else
            k = 0;
        w[i] -= t[0];
        if (w[i] > MAX_DIGIT - t[0])
            k++;
        k += t[1];
    }

    /* Cope with Wn not stored in array w[0..n-1] */
    wn -= k;

    return wn;
}

static int _QhatTooBig(DIGIT_T qhat, DIGIT_T rhat, DIGIT_T vn2, DIGIT_T ujn2)
{
    //  Returns true if Qhat is too big
    //  i.e. if (Qhat * Vn-2) > (b.Rhat + Uj+n-2)
    //
    DIGIT_T t[2];

    mpFunc->SpMultiply(t, qhat, vn2);
    if (t[1] < rhat)
        return 0;
    else if (t[1] > rhat)
        return 1;
    else if (t[0] > ujn2)
        return 1;

    return 0;
}

int _mpDivide(DIGIT_T q[], DIGIT_T r[], const DIGIT_T u[], int udigits, DIGIT_T v[], int vdigits)
{
    /*  Computes quotient q = u / v and remainder r = u mod v
    where q, r, u are multiple precision digits
    all of udigits and the divisor v is vdigits.

    Ref: Knuth Vol 2 Ch 4.3.1 p 272 Algorithm D.

    Do without extra storage space, i.e. use r[] for
    normalised u[], unnormalise v[] at end, and cope with
    extra digit Uj+n added to u after normalisation.

    WARNING: this trashes q and r first, so cannot do
    u = u / v or v = u mod v.
    It also changes v temporarily so cannot make it const.
    */
    int shift;
    int n, m, j;
    DIGIT_T bitmask, overflow;
    DIGIT_T qhat, rhat, t[2];
    DIGIT_T *uu, *ww;
    int qhatOK, cmp;

    /* Clear q and r */
    _mpSetZero(q, udigits);
    _mpSetZero(r, udigits);

    /* Work out exact sizes of u and v */
    n = (int)_mpSizeof(v, vdigits);
    m = (int)_mpSizeof(u, udigits);
    m -= n;

    /* Catch special cases */
    if (n == 0)
        return -1;  /* Error: divide by zero */

    if (n == 1)
    {   /* Use short division instead */
        r[0] = mpFunc->ShortDiv(q, u, v[0], udigits);
        return 0;
    }

    if (m < 0)
    {   /* v > u, so just set q = 0 and r = u */
        _mpSetEqual(r, u, udigits);
        return 0;
    }

    if (m == 0)
    {   /* u and v are the same length */
        cmp = _mpCompare(u, v, (int)n);
        if (cmp < 0)
        {   /* v > u, as above */
            _mpSetEqual(r, u, udigits);
            return 0;
        }
        else if (cmp == 0)
        {   /* v == u, so set q = 1 and r = 0 */
            _mpSetDigit(q, 1, udigits);
            return 0;
        }
    }

    /*  In Knuth notation, we have:
        Given
        u = (Um+n-1 ... U1U0)
        v = (Vn-1 ... V1V0)
        Compute
        q = u/v = (QmQm-1 ... Q0)
        r = u mod v = (Rn-1 ... R1R0)
    */

    /*  Step D1. Normalise */
    /*  Requires high bit of Vn-1
        to be set, so find most signif. bit then shift left,
        i.e. d = 2^shift, u' = u * d, v' = v * d.
    */
    bitmask = HIBITMASK;
    for (shift = 0; shift < BITS_PER_DIGIT; shift++)
    {
        if (v[n-1] & bitmask)
            break;
        bitmask >>= 1;
    }

    /* Normalise v in situ - NB only shift non-zero digits */
    overflow = mpFunc->ShiftLeft(v, v, shift, n);

    /* Copy normalised dividend u*d into r */
    overflow = mpFunc->ShiftLeft(r, u, shift, n + m);
    uu = r; /* Use ptr to keep notation constant */

    t[0] = overflow;    /* Extra digit Um+n */

    /* Step D2. Initialise j. Set j = m */
    for (j = m; j >= 0; j--)
    {
        /* Step D3. Set Qhat = [(b.Uj+n + Uj+n-1)/Vn-1]
           and Rhat = remainder */
        qhatOK = 0;
        t[1] = t[0];    /* This is Uj+n */
        t[0] = uu[j+n-1];
        overflow = mpFunc->SpDivide(&qhat, &rhat, t, v[n-1]);

        /* Test Qhat */
        if (overflow)
        {   /* Qhat == b so set Qhat = b - 1 */
            qhat = MAX_DIGIT;
            rhat = uu[j+n-1];
            rhat += v[n-1];
            if (rhat < v[n-1])  /* Rhat >= b, so no re-test */
                qhatOK = 1;
        }
        /* [VERSION 2: Added extra test "qhat && "] */
        if (qhat && !qhatOK && _QhatTooBig(qhat, rhat, v[n-2], uu[j+n-2]))
        {   /* If Qhat.Vn-2 > b.Rhat + Uj+n-2
               decrease Qhat by one, increase Rhat by Vn-1
            */
            qhat--;
            rhat += v[n-1];
            /* Repeat this test if Rhat < b */
            if (!(rhat < v[n-1]))
            {
                if (_QhatTooBig(qhat, rhat, v[n-2], uu[j+n-2]))
                    qhat--;
            }
        }


        /* Step D4. Multiply and subtract */
        ww = &uu[j];
        overflow = _mpMultSub(t[1], ww, v, qhat, (int)n);

        /* Step D5. Test remainder. Set Qj = Qhat */
        q[j] = qhat;
        if (overflow)
        {   /* Step D6. Add back if D4 was negative */
            q[j]--;
            overflow = _mpAdd(ww, ww, v, (int)n);
        }

        t[0] = uu[j+n-1];   /* Uj+n on next round */

    }   /* Step D7. Loop on j */

    /* Clear high digits in uu */
    for (j = n; j < m+n; j++)
        uu[j] = 0;

    /* Step D8. Unnormalise. */

    mpFunc->ShiftRight(r, r, shift, n);
    mpFunc->ShiftRight(v, v, shift, n);

    return 0;
}

int _mpSquare(DIGIT_T w[], const DIGIT_T x[], int ndigits)
{
    /*  Computes square w = x * x
        where x is a multiprecision integer of ndigits
        and w is a multiprecision integer of 2*ndigits

        Ref: Menezes p596 Algorithm 14.16 with errata.
    */

    DIGIT_T k, p[2], u[2], cbit, carry, o1, o2;
    int i, j, t, i2, cpos;

    //assert(w != x);
    t = ndigits;

    /* 1. For i from 0 to (2t-1) do: w_i = 0 */
    i2 = t << 1;
    for (i = 0; i < i2; i++)
        w[i] = 0;
    carry = 0;
    cpos = i2-1;
    /* 2. For i from 0 to (t-1) do: */
    for (i = 0; i < t; i++)
    {
        /* 2.1 (uv) = w_2i + x_i * x_i, w_2i = v, c = u
           Careful, w_2i may be double-prec
        */
        i2 = i << 1; /* 2*i */

    	o1 = x[i] ;
    	o2 = x[i] ;

	   mpFunc->SpMultiply(p, o1, o2);
        p[0] += w[i2];
        if (p[0] < w[i2])
            p[1]++;
        k = 0;  /* p[1] < b, so no overflow here */
        if (i2 == cpos && carry)
        {
            p[1] += carry;
            if (p[1] < carry)
                k++;
            carry = 0;
        }
        w[i2] = p[0];
        u[0] = p[1];
        u[1] = k;

        /* 2.2 for j from (i+1) to (t-1) do:
           (uv) = w_{i+j} + 2x_j * x_i + c,
           w_{i+j} = v, c = u,
           u is double-prec
           w_{i+j} is dbl if [i+j] == cpos
        */

        k = 0;
        for (j = i+1; j < t; j++)
        {
            /* p = x_j * x_i */
            mpFunc->SpMultiply(p, x[j], x[i]);
            //_spMultiply(p, x[j], x[i]);
            /* p = 2p <=> p <<= 1 */
            cbit = (p[0] & HIBITMASK) != 0;
            k =  (p[1] & HIBITMASK) != 0;
            p[0] <<= 1;
            p[1] <<= 1;
            p[1] |= cbit;
            /* p = p + c */
            p[0] += u[0];
            if (p[0] < u[0])
            {
                p[1]++;
                if (p[1] == 0)
                    k++;
            }
            p[1] += u[1];
            if (p[1] < u[1])
                k++;
            /* p = p + w_{i+j} */
            p[0] += w[i+j];
            if (p[0] < w[i+j])
            {
                p[1]++;
                if (p[1] == 0)
                    k++;
            }
            if ((i+j) == cpos && carry)
            {   /* catch overflow from last round */
                p[1] += carry;
                if (p[1] < carry)
                    k++;
                carry = 0;
            }
            /* w_{i+j} = v, c = u */
            w[i+j] = p[0];
            u[0] = p[1];
            u[1] = k;
        }
        /* 2.3 w_{i+t} = u */
        w[i+t] = u[0];
        /* remember overflow in w_{i+t} */
        carry = u[1];
        cpos = i+t;
    }

    /* (NB original step 3 deleted in errata) */

    /* Return w */

    return 0;
}

/* Version 2: added new funcs with temps
to avoid having to alloc and free repeatedly
and added _mpSquare function for slight speed improvement
*/

static int _moduloTemp(DIGIT_T r[], const DIGIT_T u[], int udigits,
             DIGIT_T v[], int vdigits, DIGIT_T tqq[], DIGIT_T trr[])
{
    /*  Calculates r = u mod v
        where r, v are multiprecision integers of length vdigits
        and u is a multiprecision integer of length udigits.
        r may overlap v.

        Same as mpModulo without allocs & free.
        Requires temp mp's tqq and trr of length udigits each
    */

    mpFunc->Divide(tqq, trr, u, udigits, v, vdigits);

    /* Final r is only vdigits long */
    _mpSetEqual(r, trr, vdigits);

    return 0;
}

static int _modMultTemp(DIGIT_T a[], const DIGIT_T x[], const DIGIT_T y[],
              DIGIT_T m[], int ndigits,
              DIGIT_T temp[], DIGIT_T tqq[], DIGIT_T trr[])
{   /*  Computes a = (x * y) mod m */
    /*  Requires 3 x temp mp's of length 2 * ndigits each */

    /* Calc p[2n] = x * y */
    mpFunc->Multiply(temp, x, y, ndigits);

    /* Then modulo m */
    _moduloTemp(a, temp, ndigits * 2, m, ndigits, tqq, trr);

    return 0;
}

static int _modSquareTemp(DIGIT_T a[], const DIGIT_T x[],
              DIGIT_T m[], int ndigits,
              DIGIT_T temp[], DIGIT_T tqq[], DIGIT_T trr[])
{   /*  Computes a = (x * x) mod m */
    /*  Requires 3 x temp mp's of length 2 * ndigits each */

    /* Calc p[2n] = x^2 */
    mpFunc->Square(temp, x, ndigits);

    /* Then modulo m */
    _moduloTemp(a, temp, ndigits * 2, m, ndigits, tqq, trr);

    return 0;
}

static unsigned int t1[RSA_KEY_DIGI_LEN*2];
static unsigned int t2[RSA_KEY_DIGI_LEN*2];
static unsigned int t3[RSA_KEY_DIGI_LEN*2];
static unsigned int tm[RSA_KEY_DIGI_LEN];
static unsigned int y[RSA_KEY_DIGI_LEN];

int _mpModExp(DIGIT_T yout[], const DIGIT_T x[], const DIGIT_T e[], const DIGIT_T m[], int ndigits)
{
    /*  Computes y = x^e mod m */
    /*  Binary left-to-right method */
    DIGIT_T mask;
    unsigned int n, nn;
    //DIGIT_T *t1, *t2, *t3, *tm, *y;

    if (ndigits == 0)
        return -1;

    /* Create some temps */
    nn = ndigits * 2;

    _mpSetZero(t1, nn);
    _mpSetZero(t2, nn);
    _mpSetZero(t3, nn);
    _mpSetZero(tm, ndigits);
    _mpSetZero(y, ndigits);

    _mpSetEqual(tm, m, ndigits);

    /* Find second-most significant bit in e */
    n = _mpSizeof(e, ndigits);
    for (mask = HIBITMASK; mask > 0; mask >>= 1)
    {
        if (e[n-1] & mask)
            break;
    }
    mpNEXTBITMASK(mask, n);

    /* Set y = x */
    _mpSetEqual(y, x, ndigits);

    /* For bit j = k-2 downto 0 */

    while(n)
    {
        /* Square y = y * y mod n */
        _modSquareTemp(y, y, tm, ndigits, t1, t2, t3);
        if (e[n-1] & mask)
        {
            /*  if e(j) == 1 then multiply y = y * x mod n */
            _modMultTemp(y, y, x, tm, ndigits, t1, t2, t3);
        }

        /* Move to next bit */
        mpNEXTBITMASK(mask, n);
    }

    /* return y */
    _mpSetEqual(yout, y, ndigits);

    return 0;
}

int _mpConvFromOctets(DIGIT_T a[], int ndigits, const unsigned char *c, int nbytes)
/* Converts nbytes octets into big digit a of max size ndigits
   Returns actual number of digits set (may be larger than mpSizeof)
*/
{
    //mpFunc=&_mpFunc;  //-------------------------------
    int i;
    int j, k;
    DIGIT_T t;

    _mpSetZero(a, ndigits);

    /* Read in octets, least significant first */
    /* i counts into big_d, j along c, and k is # bits to shift */
    for (i = 0, j = nbytes - 1; i < ndigits && j >= 0; i++)
    {
        t = 0;
        for (k = 0; j >= 0 && k < BITS_PER_DIGIT; j--, k += 8)
        {
            t |= ((DIGIT_T)c[j]) << k;
        }
        a[i] = t;
    }

    return i;
}

int _mpConvToOctets(const DIGIT_T a[], int ndigits, unsigned char *c, int nbytes)
/* Convert big digit a into string of octets, in big-endian order,
   padding on the left to nbytes or truncating if necessary.
   Return number of octets required excluding leading zero bytes.
*/
{
    int j, k, len;
    DIGIT_T t;
    int i, noctets, nbits;

    nbits = _mpBitLength(a, ndigits);
    noctets = (nbits + 7) / 8;

    len = (int)nbytes;

    for (i = 0, j = len - 1; i < ndigits && j >= 0; i++)
    {
        t = a[i];
        for (k = 0; j >= 0 && k < BITS_PER_DIGIT; j--, k += 8)
        {
            c[j] = (unsigned char)(t >> k);
        }
    }

    for ( ; j >= 0; j--)
    {
        c[j] = 0;
    }

    return (int)noctets;
}
int RSA2048SWdecrypt(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_E, unsigned char *Sim_SignOut)
{
    DIGIT_T Sim_Signature[RSA_KEY_DIGI_LEN];
    DIGIT_T Sim_N[RSA_KEY_DIGI_LEN];
    DIGIT_T Sim_E[RSA_KEY_DIGI_LEN];

    _mpSetZero(Sim_Signature, RSA_KEY_DIGI_LEN);
    _mpSetZero(Sim_N, RSA_KEY_DIGI_LEN);
    _mpSetZero(Sim_E, RSA_KEY_DIGI_LEN);
    _mpSetZero((DIGIT_T *)Sim_SignOut, RSA_KEY_DIGI_LEN);

    mpFunc->ConvFromOctets(Sim_Signature, RSA_KEY_DIGI_LEN, Signature, RSA_KEY_DIGI_LEN); //sizeof(Signature)
    mpFunc->ConvFromOctets(Sim_N, RSA_KEY_DIGI_LEN, PublicKey_N, RSA_KEY_DIGI_LEN); //sizeof(PublicKey_N)
    mpFunc->ConvFromOctets(Sim_E, RSA_KEY_DIGI_LEN, PublicKey_E, 4); //sizeof(PublicKey_E)
    mpFunc->ModExp((DIGIT_T *)Sim_SignOut, Sim_Signature, Sim_E, Sim_N, RSA_KEY_DIGI_LEN);
    return 0;
}
#endif
int rsa_main(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_E, unsigned char *Sim_SignOut)
{
    UBOOT_TRACE("IN\n");
    int ret = -1;
    #if defined(CONFIG_SECURE_HW_IP) || defined(CONFIG_SECURE_SMC_HW_IP)
    //AESDMA
    ret = RSA2048HWdecrypt(Signature,PublicKey_N,PublicKey_E,Sim_SignOut);
    #else
    //SW
    ret = RSA2048SWdecrypt(Signature,PublicKey_N,PublicKey_E,Sim_SignOut);
    #endif
    if(-1 == ret)
    {
        UBOOT_ERROR("RSA Decrypt Error\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
	return ret;
}
#undef CRYPTO_RSA_C
