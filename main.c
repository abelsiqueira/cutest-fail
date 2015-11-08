#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cutest.h"
#include <dlfcn.h>

void * lib_handle;

// Dummy elfun
void elfun_(double *a, double *b, double *c, integer *d, integer *e,
            integer *f, integer *g, integer *h, integer *i, integer *j,
            integer *k, integer *l, integer *m, integer *n, integer *o,
            integer *p, integer *q, integer *r, integer *s, integer *t,
            integer *u, integer *v, integer *x) {
  void (*elfun) (double *, double *b, double *, integer *, integer *,
            integer *, integer *, integer *, integer *, integer *,
            integer *, integer *, integer *, integer *, integer *,
            integer *, integer *, integer *, integer *, integer *,
            integer *, integer *, integer *);
  printf("> elfun\n");
  elfun = dlsym(lib_handle, "elfun_");
  (*elfun)(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,x);
}

void group_(double *a, integer *b, double *c, double *d, integer *e,
            integer *f, integer *g, integer *h, integer *i, integer *j,
            integer *k, integer *l, integer *m, integer *n, integer *o) {
  void (*group)(double *a, integer *b, double *c, double *d, integer *e,
            integer *f, integer *g, integer *h, integer *i, integer *j,
            integer *k, integer *l, integer *m, integer *n, integer *o);
  printf("> group\n");
  group = dlsym(lib_handle, "group_");
  (*group)(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o);
}

int MAINENTRY() {
  double *x, *bl, *bu, f = 0.0;
  double *y, *cl, *cu, *c;
  logical *equatn, *linear;
  integer efirst = 0, lfirst = 0, nvfirst = 0;
  char pname[10], *vnames, *cnames;
  char fname[20] = "OUTSDIFHS4.d";
  integer nvar = 0, ncon = 0;
  integer funit = 42, ierr = 0, fout = 6, io_buffer = 11, status;
  char *error;

  int i;

  // First HS4
  {
    lib_handle = dlopen("libHS4.so", RTLD_LAZY | RTLD_LOCAL);
    if (!lib_handle) {
      fprintf(stderr, "lib_handle = 0: %s\n", dlerror());
      exit(1);
    } else if ((error = dlerror()) != NULL) {
      fprintf(stderr, "error != 0: %s\n", error);
      exit(1);
    } else {
      fputs("ok\n", stdout);
    }
    FORTRAN_open(&funit, fname, &ierr);
    CUTEST_cdimen(&status, &funit, &nvar, &ncon);

    x  = (doublereal *) malloc(sizeof(doublereal) * nvar);
    bl = (doublereal *) malloc(sizeof(doublereal) * nvar);
    bu = (doublereal *) malloc(sizeof(doublereal) * nvar);
    vnames = (char *) malloc(10 * nvar);

    CUTEST_usetup(&status, &funit, &fout, &io_buffer, &nvar, x, bl, bu);
    CUTEST_unames(&status, &nvar, pname, vnames);

    pname[9] = 0;
    printf("%9s\n", pname);
    printf("Number of variables: %d\n", nvar);
    printf("Number of constraints: %d\n", ncon);

    CUTEST_ufn(&status, &nvar, x, &f);

    printf("f = %lf\n", f);
    printf("x =");
    for (i = 0; i < nvar; i++)
      printf(" %lf", x[i]);
    printf("\n");

    free(x);
    free(bl);
    free(bu);

    FORTRAN_close(&funit, &ierr);
    CUTEST_uterminate(&status);
    dlclose(lib_handle);
    if ((error = dlerror()) != NULL) {
      fprintf(stderr, "error != 0: %s\n", error);
      exit(1);
    }
  }

  // Second HS32
  {
    lib_handle = dlopen("libHS32.so", RTLD_LAZY | RTLD_LOCAL);
    if (!lib_handle) {
      fprintf(stderr, "lib_handle = 0: %s\n", dlerror());
      exit(1);
    } else if ((error = dlerror()) != NULL) {
      fprintf(stderr, "error != 0: %s\n", error);
      exit(1);
    } else {
      fputs("ok\n", stdout);
    }
    strcpy(fname, "OUTSDIFHS32.d");
    FORTRAN_open(&funit, fname, &ierr);
    CUTEST_cdimen(&status, &funit, &nvar, &ncon);

    x  = (doublereal *) malloc(sizeof(doublereal) * nvar);
    bl = (doublereal *) malloc(sizeof(doublereal) * nvar);
    bu = (doublereal *) malloc(sizeof(doublereal) * nvar);
    y  = (doublereal *) malloc(sizeof(doublereal) * ncon);
    cl = (doublereal *) malloc(sizeof(doublereal) * ncon);
    cu = (doublereal *) malloc(sizeof(doublereal) * ncon);
    c  = (doublereal *) malloc(sizeof(doublereal) * ncon);

    equatn = (logical *) malloc(sizeof(logical) * ncon);
    linear = (logical *) malloc(sizeof(logical) * ncon);
    vnames = (char *) malloc(10 * nvar);
    cnames = (char *) malloc(10 * ncon);

    CUTEST_csetup(&status, &funit, &fout, &io_buffer, &nvar, &ncon, x, bl, bu,
        y, cl, cu, equatn, linear, &efirst, &lfirst, &nvfirst);
    CUTEST_cnames(&status, &nvar, &ncon, pname, vnames, cnames);

    pname[9] = 0;
    printf("%9s\n", pname);
    printf("Number of variables: %d\n", nvar);
    printf("Number of constraints: %d\n", ncon);

    CUTEST_cfn(&status, &nvar, &ncon, x, &f, c);

    printf("f = %lf\n", f);
    printf("x =");
    for (i = 0; i < nvar; i++)
      printf(" %lf", x[i]);
    printf("\n");

    free(x);
    free(bl);
    free(bu);
    free(y);
    free(cl);
    free(cu);
    free(c);
    free(equatn);
    free(linear);

    FORTRAN_close(&funit, &ierr);
    CUTEST_cterminate(&status);
    dlclose(lib_handle);
  }

  return 0;
}
