#include "../log/log.c"
#include <stdio.h>


#define SPINNER         utf8_pat2
#define WORKING_TEXT    AC_RESETALL AC_GREEN "Working" AC_RESETALL
#define PART_TEXT       AC_RESETALL AC_YELLOW AC_REVERSED                    "Part way through" AC_RESETALL
#define CLOSE_TEXT      AC_RESETALL AC_BLUE "Getting close now" AC_RESETALL
#define DONE_TEXT       AC_RESETALL AC_GREEN AC_REVERSED "Done!" AC_RESETALL "\n"


#include "spin.h"

const int big_number = 1000000000;
#define LOCATION    SPINNER_TOP_RIGHT_DOWN_FIVE


int main(void) {
  spinner *s = spin_new(
    SPINNER,
    WORKING_TEXT
    "",
    UTF8_CHAR_WIDTH,
    LOCATION
    );
  int x = 0, working = 1;

  while (working) {
    if (x == (big_number * .25)) {
      spin_upd_msg(s, PART_TEXT);  // update the status message
      spin_drw(s);
    }
    if (x == (big_number * .75)) {
      spin_upd_msg(s, CLOSE_TEXT);
      spin_drw(s);
    }
    if (x % 9999999 == 0) {  // this is just to throttle it a bit
      spin_drw(s);
    }
    if (x++ > big_number) {
      working = 0;
    }
  }
  spin_del(s);   // cleanup
  printf(DONE_TEXT);
  return(0);
}
