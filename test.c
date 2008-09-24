#include <stdlib.h>
#include <stdio.h>
#include <check.h>

int main (void)
{
  int n;
  SRunner *sr;
  Suite *s = suite_create("Foo");
  TCase * tc = tcase_create("Bar");

  sr = srunner_create (s);

  srunner_run_all (sr, CK_VERBOSE);
  n = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
