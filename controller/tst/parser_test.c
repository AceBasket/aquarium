#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

extern void parser(char *str);

static void test_parse_load(void **state) {
    (void)state; /* unused */
    assert_int_equal(1, 1);
    parser("load test");
}

int main(void) {
    const struct CMUnitTest tests[] = { cmocka_unit_test(test_parse_load) };
    return cmocka_run_group_tests(tests, NULL, NULL);
}