#include"edge_filters.h"
#include"common.h"
#include<iostream>

using namespace std;


void test_function(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void raw_proc(int16_t* src, string params_path, int32_t height, int32_t width, int16_t* dst);
