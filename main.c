#include <stdio.h>
#include <string.h>
#include "cutest.h"

int MAINENTRY() {
  double *x, *bl, *bu, f;
  double *y, *cl, *cu, *c;
  logical *equatn, *linear;
  integer efirst = 0, lfirst = 0, nvfirst = 0;
  char pname[10], *vnames, *cnames;
  char fname[20] = "OUTSDIFHS4.d";
  integer nvar = 0, ncon = 0;
  integer funit = 42, ierr = 0, fout = 6, io_buffer = 11, status;

  int i;

  // First HS4
  {
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
  }

  // Second HS32
  {
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
  }

  return 0;
}
