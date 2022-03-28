#ifndef SPIN_H
#define SPIN_H
#include "spin_types.h"
/**/
#include "spin_constants.h"
/**/
#include "spin_locations.h"
/**/
#include "spin_defaults.h"
/**/

#include "../log/log.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile unsigned int spin_draws = 0;

typedef struct spinner {
  const char            *c;
  const char            *pat;
  char                  *msg;
  int                   charwidth;
  char                  *spinner_color;
  spinner_location      location;
  volatile unsigned int spin_draws;
  FILE                  *fp;
} spinner;


static void showcur(spinner *s, int show) {
  show ? fprintf(s->fp, CSI "?25h") : fprintf(s->fp, CSI "?25l");
}


spinner *spin_new(const char *pat, char *msg, int charwidth, spinner_location loc) {
  spinner *s = malloc(sizeof(spinner));

  s->pat           = pat;
  s->c             = s->pat;
  s->msg           = msg;
  s->charwidth     = charwidth;
  s->spinner_color = DEFAULT_SPINNER_COLOR;
  s->location      = loc;
  FILE *tfp;

  tfp   = fopen("/tmp/tfp.log", "a");
  s->fp = tfp;

  showcur(s, 0);
  return(s);
}


void spin_clr(spinner *s) {
  fflush(s->fp);

  for (int i = strlen(s->msg) + 2; i > 0; i--) {
    fprintf(s->fp, AC_CLS);
    for (int idx = 0; idx < s->location.y; idx++) {
      fprintf(s->fp, AC_DOWN(1));
    }
    for (int idx = 0; idx < abs(s->location.x); idx++) {
      (s->location.x > 0) ? fprintf(s->fp, AC_RIGHT(1)) : fprintf(s->fp, AC_LEFT(1));
    }
  }
}


void spin_drw(spinner *s) {
  spin_draws++;
  spin_clr(s);
  fprintf(s->fp, AC_RESETALL "<%lu> [%d] >%s " AC_RESETALL
          "",
          spin_draws,
          s->charwidth,
          s->msg
          );
  for (int i = 0; i < s->charwidth; i++) {
    fprintf(s->fp, "%c", s->c[i]);
  }
  if ((s->c += s->charwidth)[1] == '\0') {
    s->c = s->pat;
  }
}


void spin_upd_fp(spinner *s, FILE *fp) {
  s->fp = fp;
}


void spin_upd_location(spinner *s, spinner_location loc) {
  s->location = loc;
}


void spin_upd_msg(spinner *s, char *msg) {
  s->msg = msg;
}


void spin_del(spinner *s) {
  spin_clr(s);
  showcur(s, 1);
  free(s);
}

#endif // SPIN_H
