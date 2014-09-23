/*
 * File:   chat_test.c
 * Author: Matthias
 *
 * Created on Sep 16, 2014, 11:05:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>


/*
 * CUnit Test Suite
 */

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

void createSocket()
{
  int test = create_socket(SOCK_DGRAM, IPPROTO_UDP);
  CU_ASSERT( test != -1 );
}

void createUDPConnection() 
{
  Connection* test;
  test = new_connection(1);
  CU_ASSERT( test->port != -1);
}

void runTest()
{
  run_chat_coordinator();
  CU_PASS("run method was able to run");
}

void startTest()
{
  char* test = "test";
  int test_ret  = my_start(test);
  CU_ASSERT(test_ret > -1);
}

void findTest()
{
  char* test = "test";
  int result;
  result = my_find(test);
  CU_ASSERT(result != -1);
}

void dontFindTest()
{
  char* test = "foo";
  int result;
  result = my_find(test);
  CU_ASSERT(result == -1);
}

void startSessionTest()
{
  start_new_session(-1);
  CU_PASS("Was able to create a new process");
}

int main() {
  CU_pSuite pSuite = NULL;

  /* Initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* Add a suite to the registry */
  pSuite = CU_add_suite("chat_test", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Add the tests to the suite */
  if (
       (NULL == CU_add_test(pSuite, "createSocket", createSocket))               ||
       (NULL == CU_add_test(pSuite, "createUDPConnection", createUDPConnection)) ||
       (NULL == CU_add_test(pSuite, "startTest", startTest))                     ||
       (NULL == CU_add_test(pSuite, "findTest", findTest))                       ||
       (NULL == CU_add_test(pSuite, "dontFindTest", dontFindTest))               ||
       (NULL == CU_add_test(pSuite, "startSessionTest", startSessionTest))
      ) 
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
