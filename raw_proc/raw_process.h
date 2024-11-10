#include"edge_filters.h"
#include"common.h"
#include<iostream>

#include"dpc.h"
#include"raw_nr.h"
#include"blc.h"
#include"lsc.h"
#include"wbc.h"
#include"gtm.h"
#include"ltm.h"
#include"cac.h"
#include"demosaic.h"


using namespace std;


void test_function(int16_t* src, int32_t height, int32_t width, int16_t* dst);

void raw_proc(uint16_t* src, string params_path, int32_t height, int32_t width, uint8_t* dst);
