#include"jsoncpp.cpp"
#include<iostream>
#include<fstream>
#include<string>
#include"raw_params.h"


using namespace std;

void read_params(string path, COMMON_PARAMS*common_params, BLC_PARAMS* blc_params, AWB_PARAMS* awb_params) {

	Json::Reader reader;
	Json::Value root;

	ifstream in(path, ios::binary);

	if (!in.is_open())
	{
		cout << "Error opening parameter file" << endl;
		return;
	}

	if (reader.parse(in, root))
	{
		int pattern = root["common"]["bayer_pattern"].asInt();
		int white_level = root["common"]["white_level"].asInt();
		int img_height = root["common"]["img_height"].asInt();
		int img_width = root["common"]["img_width"].asInt();

		common_params->bayer_pattern = pattern;
		common_params->white_level = white_level;
		common_params->img_height = img_height;
		common_params->img_width = img_width;

		if (pattern == 0)
		{
			blc_params->black_level_r = root["blc"]["black_level"][0].asInt();
			blc_params->black_level_gr = root["blc"]["black_level"][1].asInt();
			blc_params->black_level_gb = root["blc"]["black_level"][2].asInt();
			blc_params->black_level_b = root["blc"]["black_level"][3].asInt();
		}
		else if (pattern == 1)
		{
			blc_params->black_level_r = root["blc"]["black_level"][1].asInt();
			blc_params->black_level_gr = root["blc"]["black_level"][0].asInt();
			blc_params->black_level_gb = root["blc"]["black_level"][3].asInt();
			blc_params->black_level_b = root["blc"]["black_level"][2].asInt();
		}
		else if (pattern == 2)
		{
			blc_params->black_level_r = root["blc"]["black_level"][2].asInt();
			blc_params->black_level_gr = root["blc"]["black_level"][3].asInt();
			blc_params->black_level_gb = root["blc"]["black_level"][0].asInt();
			blc_params->black_level_b = root["blc"]["black_level"][1].asInt();
		}
		else if (pattern == 3)
		{
			blc_params->black_level_r = root["blc"]["black_level"][3].asInt();
			blc_params->black_level_gr = root["blc"]["black_level"][2].asInt();
			blc_params->black_level_gb = root["blc"]["black_level"][1].asInt();
			blc_params->black_level_b = root["blc"]["black_level"][0].asInt();
		}

		float gain_0 = root["awb"]["gain"][0].asFloat();
		float gain_1 = root["awb"]["gain"][1].asFloat();
		float gain_2 = root["awb"]["gain"][2].asFloat();
		float gain_3 = root["awb"]["gain"][3].asFloat();

		if (pattern == 0)
		{
			awb_params->g_gain = 1.0;
			awb_params->r_gain = gain_0 / gain_1;
			awb_params->b_gain = gain_2 / gain_3;
		}
		else if (pattern == 1)
		{
			awb_params->g_gain = 1.0;
			awb_params->r_gain = gain_1 / gain_0;
			awb_params->b_gain = gain_3 / gain_2;
		}
		else if (pattern == 2)
		{
			awb_params->g_gain = 1.0;
			awb_params->r_gain = gain_3 / gain_2;
			awb_params->b_gain = gain_1 / gain_0;
		}
		else if (pattern == 3)
		{
			awb_params->g_gain = 1.0;
			awb_params->r_gain = gain_2 / gain_3;
			awb_params->b_gain = gain_0 / gain_1;
		}

	}
	else
	{
		cout << "parse error" << endl;
	}

	in.close();
}


