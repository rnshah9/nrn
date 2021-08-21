#include <../../nrnconf.h>
/* a fake change */
/* /local/src/master/nrn/src/oc/math.cpp,v 1.6 1999/07/16 13:43:10 hines Exp */

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include "nrnmpiuse.h"
#include "ocfunc.h"
# include	"hoc.h"
#if NRN_ARCH_INDEP_EXP_POW
#include <mpfr.h>
#endif


#define EPS hoc_epsilon
#define MAXERRCOUNT 5

int hoc_errno_count;

#if _CRAY
#define log logl
#define log10 log10l
#define exp expl
#define sqrt sqrtl
#define pow powl
#endif

static double errcheck(double, const char *);

void hoc_atan2(void) {
    double d;
    d = atan2(*hoc_getarg(1), *hoc_getarg(2));
    hoc_ret();
    hoc_pushx(d);
}

double Log(double x) {
    return errcheck(log(x), "log");
}

double Log10(double x) {
    return errcheck(log10(x), "log10");
}

#if NRN_ARCH_INDEP_EXP_POW

static double accuracy32(double val) {
    int ex;
    double mant = frexp(val, &ex);
    // round to about 32 bits after .
    double prec = 4294967296.0;
    double result = mant*prec;
    result = round(result);
    result /= prec;
    result = ldexp(result, ex);
    return result;
}

static double pow_arch_indep(double x, double y) {
  mpfr_prec_t prec = 53;
  mpfr_rnd_t rnd = MPFR_RNDN;

  mpfr_t x_, y_;
  mpfr_init2(x_, prec);
  mpfr_init2(y_, prec);

  mpfr_set_d(x_, x, rnd);
  mpfr_set_d(y_, y, rnd);

  mpfr_pow(x_, x_, y_, rnd);

  double r = mpfr_get_d(x_, rnd);

  mpfr_clear(x_);
  mpfr_clear(y_);

  return r;
}

static double exp_arch_indep(double x) {
  mpfr_prec_t prec = 53;
  mpfr_rnd_t rnd = MPFR_RNDN;

  mpfr_t x_;
  mpfr_init2(x_, prec);
  mpfr_set_d(x_, x, rnd);
  mpfr_exp(x_, x_, rnd);
  double r = mpfr_get_d(x_, rnd);
  mpfr_clear(x_);
  return r;
}

static double pow_precision32(double x, double y) {
    return accuracy32(pow(x,y));
}  

static double exp_precision32(double x) {
    return accuracy32(exp(x));
}  

static double (*pow_ptr)(double x, double y) = pow;
static double (*pow_ieee_ptr)(double x, double y) = pow; // avoid error! Maybe because of c++ overloading.
static double (*exp_ptr)(double x) = exp;

int nrn_use_exp_pow_precision(int style) {
  int s = 0;
  if (style == 0) { // default IEEE
    pow_ptr = pow;
    exp_ptr = exp;
  }else if (style == 1) { // 53 bit mpfr
    pow_ptr = pow_arch_indep;
    exp_ptr = exp_arch_indep;
  }else if (style == 2) { // 32 bit truncation
    pow_ptr = pow_precision32;
    exp_ptr = exp_precision32;
  }
  if (pow_ptr == pow_ieee_ptr) { s = 0; }
  if (pow_ptr == pow_arch_indep) { s = 1; }
  if (pow_ptr == pow_precision32) { s = 2; }
  nrn_assert(s >= 0 && s <= 2);
  return s;
}

#endif //NRN_ARCH_INDEP_EXP_POW

void hoc_use_exp_pow_precision() {
  int style = 0;
  if (ifarg(1)) {
    style = chkarg(1, 0.0, 2.0);
  }
#if NRN_ARCH_INDEP_EXP_POW
  style = nrn_use_exp_pow_precision(style);
#else
  style = 0;
#endif // NRN_ARCH_INDEP_EXP_POW
  hoc_ret();
  hoc_pushx(double(style));
}

// Try to overcome difference between linux and windows.
// by rounding mantissa to 32 bit accuracy or using
// hopefully arch independent mpfr for exp and pow.

extern "C" double hoc_pow(double x, double y) {
#if NRN_ARCH_INDEP_EXP_POW
    return (*pow_ptr)(x, y);
#else
    return pow(x, y);
#endif
}

/* used by nmodl and other c, c++ code */
extern "C" double hoc_Exp(double x) {
    if (x < -700.) {
        return 0.;
    } else if (x > 700 && nrn_feenableexcept_ == 0) {
        errno = ERANGE;
        if (++hoc_errno_count < MAXERRCOUNT) {
            fprintf(stderr, "exp(%g) out of range, returning exp(700)\n", x);
        }
        if (hoc_errno_count == MAXERRCOUNT) {
            fprintf(stderr, "No more errno warnings during this execution\n");
        }
        return exp(700.);
    }

#if NRN_ARCH_INDEP_EXP_POW
    return (*exp_ptr)(x);
#else
    return exp(x);
#endif
}

/* used by interpreter */
double hoc1_Exp(double x) {
    if (x < -700.) {
        return 0.;
    } else if (x > 700) {
        errno = ERANGE;
        return errcheck(exp(700.), "exp");
    }
    return errcheck(exp(x), "exp");
}

double Sqrt(double x) {
    return errcheck(sqrt(x), "sqrt");
}

double Pow(double x, double y) {
    return errcheck(hoc_pow(x, y), "exponentiation");
}

double integer(double x) {
    if (x < 0) {
        return (double) (long) (x - EPS);
    } else {
        return (double) (long) (x + EPS);
    }
}

double errcheck(double d, const char *s)    /* check result of library call */
{
    if (errno == EDOM) {
        errno = 0;
        hoc_execerror(s, "argument out of domain");
    } else if (errno == ERANGE) {
        errno = 0;
#if 0
        hoc_execerror(s, "result out of range");
#else
        if (++hoc_errno_count > MAXERRCOUNT) {
        } else {
            hoc_warning(s, "result out of range");
            if (hoc_errno_count == MAXERRCOUNT) {
                fprintf(stderr, "No more errno warnings during this execution\n");
            }
        }
#endif
    }
    return d;
}

int hoc_errno_check(void) {
    int ierr;
#if LINDA
    static parallel_eagain = 0;
#endif

#if 1
    errno = 0;
    return 0;
#else
    if (errno) {
        if (errno == EAGAIN) {
            /* Ubiquitous on many systems and it seems not to matter */
            errno = 0;
            return 0;
        }
#if BLUEGENE
                 if (errno == ENOSYS) {
                        errno = 0;
                        return 0;
                 }
#endif
#if !defined(MAC) || defined(DARWIN)
        if (errno == ENOENT) {
            errno = 0;
            return 0;
        }
#endif
        if (++hoc_errno_count > MAXERRCOUNT) {
            errno = 0;
            return 0;
        }
#if defined(CYGWIN)
        if (errno == EBUSY) {
            errno = 0;
            return 0;
        }
#endif
        switch (errno) {
        case EDOM:
fprintf(stderr, "A math function was called with argument out of domain\n");
            break;
        case ERANGE:
fprintf(stderr, "A math function was called that returned an out of range value\n");
            break;
#if LINDA
/* regularly set by eval() and perhaps other linda commands */
                case EAGAIN:
                if (parallel_eagain++ == 0) {
                perror("oc");
fprintf(stderr, "oc: This error occurs often from LINDA and thus will not be further reported.\n");
                }
            break;
#endif
        default:
            perror("oc");
            break;
        }
        if (hoc_errno_count == MAXERRCOUNT) {
fprintf(stderr, "No more errno warnings during this execution\n");
        }
    }
    ierr = errno;
    errno = 0;
    return ierr;
#endif
}

