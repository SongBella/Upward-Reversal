#include "StdAfx.h"
#include "NNshell.h"
#include "NNshellDoc.h"
#include "NNshellView.h"
#include "FeatureEnum.h"
#import "mysdo.dll"
using namespace mysdoLib;
#include "FundSim.h"
#include "AStockFeatures.h"
#include "PrintfDlg.h"
#include <stdio.h>
#include <stdlib.h>		
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>		
#include "TensorflowInput.h"
#include "CSpreadSheet.h"
#include "DBUpdate.h"
using namespace std;

char stock_code[MAX_STOCK][7];
char stock_name[MAX_STOCK][70];
int stock_count = 0;

int close[MAX_STOCK];
int open[MAX_STOCK];
int high[MAX_STOCK];
int low[MAX_STOCK];
int volume[MAX_STOCK];


char my_stock_code[MAX_MY_STOCK][7];
char my_stock_name[MAX_MY_STOCK][70];
int my_stock_count = 0;
int num_days[MAX_MY_STOCK];

char date_string[MAX_MY_STOCK][MAX_TXT_DB_DATES][10];
int m_open[MAX_MY_STOCK][MAX_TXT_DB_DATES], m_high[MAX_MY_STOCK][MAX_TXT_DB_DATES], m_low[MAX_MY_STOCK][MAX_TXT_DB_DATES],
m_close[MAX_MY_STOCK][MAX_TXT_DB_DATES], m_volume[MAX_MY_STOCK][MAX_TXT_DB_DATES];

double ma5[MAX_MY_STOCK][MAX_TXT_DB_DATES],  ma10[MAX_MY_STOCK][MAX_TXT_DB_DATES],  ma20[MAX_MY_STOCK][MAX_TXT_DB_DATES],  ma60[MAX_MY_STOCK][MAX_TXT_DB_DATES],  ma120[MAX_MY_STOCK][MAX_TXT_DB_DATES];
double vma5[MAX_MY_STOCK][MAX_TXT_DB_DATES], vma10[MAX_MY_STOCK][MAX_TXT_DB_DATES], vma20[MAX_MY_STOCK][MAX_TXT_DB_DATES], vma60[MAX_MY_STOCK][MAX_TXT_DB_DATES], vma120[MAX_MY_STOCK][MAX_TXT_DB_DATES];

double disp5[MAX_MY_STOCK][MAX_TXT_DB_DATES], disp10[MAX_MY_STOCK][MAX_TXT_DB_DATES], disp20[MAX_MY_STOCK][MAX_TXT_DB_DATES], 
			disp60[MAX_MY_STOCK][MAX_TXT_DB_DATES], disp120[MAX_MY_STOCK][MAX_TXT_DB_DATES];
double vdisp5[MAX_MY_STOCK][MAX_TXT_DB_DATES], vdisp10[MAX_MY_STOCK][MAX_TXT_DB_DATES], vdisp20[MAX_MY_STOCK][MAX_TXT_DB_DATES], 
			vdisp60[MAX_MY_STOCK][MAX_TXT_DB_DATES], vdisp120[MAX_MY_STOCK][MAX_TXT_DB_DATES];

double grad5[MAX_MY_STOCK][MAX_TXT_DB_DATES], grad10[MAX_MY_STOCK][MAX_TXT_DB_DATES], grad20[MAX_MY_STOCK][MAX_TXT_DB_DATES], 
			grad60[MAX_MY_STOCK][MAX_TXT_DB_DATES], grad120[MAX_MY_STOCK][MAX_TXT_DB_DATES];
double vgrad5[MAX_MY_STOCK][MAX_TXT_DB_DATES], vgrad10[MAX_MY_STOCK][MAX_TXT_DB_DATES], vgrad20[MAX_MY_STOCK][MAX_TXT_DB_DATES], 
			vgrad60[MAX_MY_STOCK][MAX_TXT_DB_DATES], vgrad120[MAX_MY_STOCK][MAX_TXT_DB_DATES];

int score[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int score1[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int score2[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int score3[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int aux_score[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int vscore[MAX_MY_STOCK][MAX_TXT_DB_DATES];

int reset_start[1600][50];
int reset_end[1600][50];
int reset_interval[1600][50];
int reset_count[1600];

bool is_trainable[1600][1500];

int idx_arr[5][5] = { { 0,0,0,0,0 },{ -1,0,0,0,0 },{ -2,-3,0,0,0 },
{ -4,-5,-6,0,0 },{ -7,-8,-9,-10,-11 } };
int len_arr[5] = { 1,1,2,3,5 };

enum {
     UP5,
     UP10,
     UP20,
     UP60,
     UP120,
     ON5_10,
     ON5_20,
     ON5_60,
     ON5_120,
     ON10_20,
     ON10_60,
     ON10_120,
     ON20_60,
     ON20_120,
     ON60_120,
     ONC_5,
     ONC_10,
     ONC_20,
     ONC_60,
     ONC_120
};

char event_string[20][15] = {
	 "U5",
     "U10",
     "U20",
     "U60",
     "U120",
     "5_10",
     "5_20",
     "5_60",
     "5_120",
     "10_20",
     "10_60",
     "10_120",
     "20_60",
     "20_120",
     "60_120",
     "On5",
     "On10",
     "On20",
     "On60",
     "On120"
};

enum {
     Up5,
     Up10,
     Up20,
     Up120,
     On5_10,
     On5_20,
     On5_120,
     On10_20,
     On10_120,
     On20_120,
     OnC_5,
     OnC_10,
     OnC_20,
     OnC_120
};

char event_string1[20][15] = {
	 "U5",
     "U10",
     "U20",
     "U120",
     "5_10",
     "5_20",
     "5_120",
     "10_20",
     "10_120",
     "20_120",
     "On5",
     "On10",
     "On20",
     "On120"
};

enum {
     up5,
     up10,
     up20,
     up60,
     on5_10,
     on5_20,
     on5_60,
     on10_20,
     on10_60,
     on20_60,
     onC_5,
     onC_10,
     onC_20,
     onC_60,
};

char event_string2[20][15] = {
	 "U5",
     "U10",
     "U20",
     "U60",
     "5_10",
     "5_20",
     "5_60",
     "10_20",
     "10_60",
     "20_60",
     "On5",
     "On10",
     "On20",
     "On60"
};


enum {
     U5,
     U10,
     U20,
     O5_10,
     O5_20,
     O10_20,
     OC_5,
     OC_10,
     OC_20,
};

char event3_string[20][15] = {
	 "U5",
     "U10",
     "U20",
     "5_10",
     "5_20",
     "10_20",
     "On5",
     "On10",
     "On20",
};


enum {
     BU5,
     BU10,
     BU20,
     BU60,
     BU120,
     BD5,
     BD10,
     BD20,
     BD60,
     BD120,
     SUP5,
     SUP10,
     SUP20,
     SUP60,
     SUP120,
     RES5,
     RES10,
     RES20,
     RES60,
     RES120
};

char aux_event_string[20][15] = {
     "BU5",
     "BU10",
     "BU20",
     "BU60",
     "BU120",
     "BD5",
     "BD10",
     "BD20",
     "BD60",
     "BD120",
     "S5",
     "S10",
     "S20",
     "S60",
     "S120",
     "R5",
     "R10",
     "R20",
     "R60",
     "R120"
};


enum {
     VUP5,
     VUP20,
     VUP60,
     VUP120,
     VON5_20,
     VON5_60,
     VON5_120,
     VON20_60,
     VON20_120,
     VON60_120,
     VONC_5,
     VONC_20,
     VONC_60,
     VONC_120
};

char vevent_string[14][15] = {
	 "VU5",
     "VU20",
     "VU60",
     "VU120",
     "V5_20",
     "V5_60",
     "V5_120",
     "V20_60",
     "V20_120",
     "V60_120",
     "VOn5",
     "VOn20",
     "VOn60",
     "VOn120"
};

bool event0[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_EVENT];
bool event1[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_EVENT1];
bool event2[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_EVENT2];
bool event3[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_EVENT3];

bool aux_event[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_AUX_EVENT];
bool vevent[MAX_MY_STOCK][MAX_TXT_DB_DATES][MAX_VEVENT];

int state[MAX_MY_STOCK][MAX_TXT_DB_DATES];
int vstate[MAX_MY_STOCK][MAX_TXT_DB_DATES];


int Open=0, High=0, Low=0, Close=0, Higher=0, Lower=0;
double MA5=0, MA10=0, MA20=0, MA60=0, MA120=0;
bool Red=false, Blue=false;

bool volume_cliff[MAX_MY_STOCK][MAX_TXT_DB_DATES];
bool ma10_cliff[MAX_MY_STOCK][MAX_TXT_DB_DATES];
bool ma10_valley[MAX_MY_STOCK][MAX_TXT_DB_DATES];

double rate(int a, int b) 
{
	if (a==0 || b==0) return 0.0f;
	else return((double)(a-b) / (double)b * 100.0f);
}

double rate_d(double a, double b) 
{
	if (a==0.0f || b==0.0f) return 0.0f;
	else return((a-b) / b * 100.0f);
}

bool red(int i, int j) {
	if (m_close[i][j] > m_open[i][j]) return true;
	else return false;
}

bool thin(int i, int j) {
	if (red(i, j)) {
		if ((double)m_close[i][j] < 1.008f * (double)m_open[i][j]) return true;
		else return false;
	}
	else {
		if ((double)m_open[i][j] < 1.008f * (double)m_close[i][j]) return true;
		else return false;
	}
}


bool overlap(int h1, int l1, int h2, int l2) {
	if (l1 >= h2) return false;
	else if (h1 <= l2) return false;
	else return true;
}

int num_friends(int i, int j) {
	if (!red(i, j)) return 0;
	int count=0;
	for (int k=j-1; k > j-18; k--) {
		if (!red(i, k)) continue;
		if (overlap(m_close[i][j], m_open[i][j], m_close[i][k], m_open[i][k])) {
			count++;
		}
	}
	return count;
}

void calculate_indicators(int start, int end)
{
	for (int i=start; i < end; i++) {
		for (int j=0; j < num_days[i]; j++) {
			if (j < 4) { ma5[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 5; k++) {
				sum += m_close[i][j - k];
			}
			ma5[i][j] = (double)sum / 5.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 9) { ma10[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 10; k++) {
				sum += m_close[i][j - k];
			}
			ma10[i][j] = (double)sum / 10.0f;
		}


		for (int j=0; j < num_days[i]; j++) {
			if (j < 19) { ma20[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 20; k++) {
				sum += m_close[i][j - k];
			}
			ma20[i][j] = (double)sum / 20.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 59) { ma60[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 60; k++) {
				sum += m_close[i][j - k];
			}
			ma60[i][j] = (double)sum / 60.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 119) { ma120[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 120; k++) {
				sum += m_close[i][j - k];
			}
			ma120[i][j] = (double)sum / 120.0f;
		}

		// Disparity
		for (int j=0; j < num_days[i]; j++) {
			if (ma5[i][j] == 0.0f) {	disp5[i][j] = 0.0f; continue; }
			disp5[i][j] = ((double)m_close[i][j] - ma5[i][j]) / m_close[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (ma10[i][j] == 0.0f) { disp10[i][j] = 0.0f; continue; }
			disp10[i][j] = ((double)m_close[i][j] - ma10[i][j]) / m_close[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (ma20[i][j] == 0.0f) { disp20[i][j] = 0.0f; continue; }
			disp20[i][j] = ((double)m_close[i][j] - ma20[i][j]) / m_close[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (ma60[i][j] == 0.0f) { disp60[i][j] = 0.0f; continue; }
			disp60[i][j] = ((double)m_close[i][j] - ma60[i][j]) / m_close[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (ma120[i][j] == 0.0f) { disp120[i][j] = 0.0f; continue; }
			disp120[i][j] = ((double)m_close[i][j] - ma120[i][j]) / m_close[i][j] * 100.0f;
		}

		// Gradient
		for (int j=1; j < num_days[i]; j++) {
			if (ma5[i][j] == 0.0f) { grad5[i][j] = 0.0f; continue; }
			grad5[i][j] = (ma5[i][j] - ma5[i][j - 1]) / ma5[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (ma10[i][j] == 0.0f) { grad10[i][j] = 0.0f; continue; }
			grad10[i][j] = (ma10[i][j] - ma10[i][j - 1]) / ma10[i][j] * 100.0f;
		for (int j=1; j < num_days[i]; j++) {
			if (ma20[i][j] == 0.0f) { grad20[i][j] = 0.0f; continue; }
			grad20[i][j] = (ma20[i][j] - ma20[i][j - 1]) / ma20[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (ma60[i][j] == 0.0f) { grad60[i][j] = 0.0f; continue; }
			grad60[i][j] = (ma60[i][j] - ma60[i][j - 1]) / ma60[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (ma120[i][j] == 0.0f) { grad120[i][j] = 0.0f; continue; }
			grad120[i][j] = (ma120[i][j] - ma120[i][j - 1]) / ma120[i][j] * 100.0f;
		}



		// Volume Moving Average
		for (int j=0; j < num_days[i]; j++) {
			if (j < 4) { vma5[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 5; k++) {
				sum += m_volume[i][j - k];
			}
			vma5[i][j] = (double)sum / 5.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 9) { vma10[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 10; k++) {
				sum += m_volume[i][j - k];
			}
			vma10[i][j] = (double)sum / 10.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 19) { vma20[i][j] = 0.0f; continue; }
			int sum = 0;
			for (int k=0; k < 20; k++) {
				sum += m_volume[i][j - k];
			}
			vma20[i][j] = (double)sum / 20.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 59) { vma60[i][j] = 0.0f; continue; }
			double sum = 0; 
			for (int k=0; k < 60; k++) {
				sum += (double)m_volume[i][j - k];
			}
			vma60[i][j] = sum / 60.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 119) { vma120[i][j] = 0.0f; continue; }
			double sum = 0;
			for (int k=0; k < 120; k++) {
				sum += (double)m_volume[i][j - k];
			}
			vma120[i][j] = sum / 120.0f;
		}


		// Volume Disparity
		for (int j=0; j < num_days[i]; j++) {
			if (vma5[i][j] == 0.0f) { vdisp5[i][j] = 0.0f; continue; }
			vdisp5[i][j] = ((double)m_volume[i][j] - vma5[i][j]) / m_volume[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (vma10[i][j] == 0.0f) { vdisp10[i][j] = 0.0f; continue; }
			vdisp10[i][j] = ((double)m_volume[i][j] - vma10[i][j]) / m_volume[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (vma20[i][j] == 0.0f) { vdisp20[i][j] = 0.0f; continue; }
			vdisp20[i][j] = ((double)m_volume[i][j] - vma20[i][j]) / m_volume[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (vma60[i][j] == 0.0f) { vdisp60[i][j] = 0.0f; continue; }
			vdisp60[i][j] = ((double)m_volume[i][j] - vma60[i][j]) / m_volume[i][j] * 100.0f;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (vma120[i][j] == 0.0f) { vdisp120[i][j] = 0.0f; continue; }
			vdisp120[i][j] = ((double)m_volume[i][j] - vma120[i][j]) / m_volume[i][j] * 100.0f;
		}

		// Volume Gradient
		for (int j=1; j < num_days[i]; j++) {
			if (vma5[i][j] == 0.0f) { vgrad5[i][j] = 0.0f; continue; }
			vgrad5[i][j] = (vma5[i][j] - vma5[i][j - 1]) / vma5[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (vma10[i][j] == 0.0f) { vgrad10[i][j] = 0.0f; continue; }
			vgrad10[i][j] = (vma10[i][j] - vma10[i][j - 1]) / vma10[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (vma20[i][j] == 0.0f) { vgrad20[i][j] = 0.0f; continue; }
			vgrad20[i][j] = (vma20[i][j] - vma20[i][j - 1]) / vma20[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (vma60[i][j] == 0.0f) { vgrad60[i][j] = 0.0f; continue; }
			vgrad60[i][j] = (vma60[i][j] - vma60[i][j - 1]) / vma60[i][j] * 100.0f;
		}
		for (int j=1; j < num_days[i]; j++) {
			if (vma120[i][j] == 0.0f) { vgrad120[i][j] = 0.0f; continue; }
			vgrad120[i][j] = (vma120[i][j] - vma120[i][j - 1]) / vma120[i][j] * 100.0f;
		}
	}
}

void make_simple_variables(int i, int j) {
	Open = m_open[i][j];
	High = m_high[i][j];
	Low = m_low[i][j];
	Close = m_close[i][j];

	Red = Blue = false;
	if (Open < Close) Red = true;
	else Blue = true;

	if (Red) {
		Higher = Close; Lower = Open;
	}
	else {
		Higher = Open; Lower = Close;
	}

	MA5 = ma5[i][j];
	MA10 = ma10[i][j];
}

void calculate_global_scores(int start, int end) {
	//---------------------------
	// Calculate global scores
	//--------------------------
	for (int i=start; i < end; i++) {

		for (int j=120; j < num_days[i]; j++) { // for all days
						
			score[i][j] = 20;
			if (grad5[i][j] < 0) { 
				score[i][j]--;
				event0[i][j][UP5] = false;
			}
			else event0[i][j][UP5] = true;

			if (grad10[i][j] < 0) { 
				score[i][j]--;
				event0[i][j][UP10] = false;
			}
			else event0[i][j][UP10] = true;
		
			if (grad20[i][j] < 0) { 
				score[i][j]--;
				event0[i][j][UP20] = false;
			}
			else event0[i][j][UP20] = true;
			
			if (grad60[i][j] < 0) { 
				score[i][j]--;
				event0[i][j][UP60] = false;
			}
			else event0[i][j][UP60] = true;
			
			if (grad120[i][j] < 0) { 
				score[i][j]--;
				event0[i][j][UP120] = false;
			}
			else event0[i][j][UP120] = true;


			if (ma5[i][j] < ma10[i][j]) { 
				score[i][j]--;
				event0[i][j][ON5_10] = false;
			}
			else event0[i][j][ON5_10] = true;

			if (ma5[i][j] < ma20[i][j]) { 
				score[i][j]--;
				event0[i][j][ON5_20] = false;
			}
			else event0[i][j][ON5_20] = true;

			if (ma5[i][j] < ma60[i][j]) { 
				score[i][j]--;
				event0[i][j][ON5_60] = false;
			}
			else event0[i][j][ON5_60] = true;

			if (ma5[i][j] < ma120[i][j]) { 
				score[i][j]--;
				event0[i][j][ON5_120] = false;
			}
			else event0[i][j][ON5_120] = true;


			if (ma10[i][j] < ma20[i][j]) { 
				score[i][j]--;
				event0[i][j][ON10_20] = false;
			}
			else event0[i][j][ON10_20] = true;

			if (ma10[i][j] < ma60[i][j]) { 
				score[i][j]--;
				event0[i][j][ON10_60] = false;
			}
			else event0[i][j][ON10_60] = true;

			if (ma10[i][j] < ma120[i][j]) { 
				score[i][j]--;
				event0[i][j][ON10_120] = false;
			}
			else event0[i][j][ON10_120] = true;


			
			if (ma20[i][j] < ma60[i][j]) { 
				score[i][j]--;
				event0[i][j][ON20_60] = false;
			}
			else event0[i][j][ON20_60] = true;

			if (ma20[i][j] < ma120[i][j]) { 
				score[i][j]--;
				event0[i][j][ON20_120] = false;
			}
			else event0[i][j][ON20_120] = true;



			if (ma60[i][j] < ma120[i][j]) { 
				score[i][j]--;
				event0[i][j][ON60_120] = false;
			}
			else event0[i][j][ON60_120] = true;


			
			if (m_close[i][j] < ma5[i][j]) { 
				score[i][j]--;
				event0[i][j][ONC_5] = false;
			}
			else event0[i][j][ONC_5] = true;

			if (m_close[i][j] < ma10[i][j]) { 
				score[i][j]--;
				event0[i][j][ONC_10] = false;
			}
			else event0[i][j][ONC_10] = true; 

			if (m_close[i][j] < ma20[i][j]) { 
				score[i][j]--;
				event0[i][j][ONC_20] = false;
			}
			else event0[i][j][ONC_20] = true;

			if (m_close[i][j] < ma60[i][j]) { 
				score[i][j]--;
				event0[i][j][ONC_60] = false;
			}
			else event0[i][j][ONC_60] = true;

			if (m_close[i][j] < ma120[i][j]) { 
				score[i][j]--;
				event0[i][j][ONC_120] = false;
			}
			else event0[i][j][ONC_120] = true;

		}
	}
}

void calculate_four_scores1(int start, int end) {
	//---------------------------
	// Calculate global scores
	//--------------------------
	for (int i=start; i < end; i++) {

		for (int j=120; j < num_days[i]; j++) { // for all days

			score1[i][j] = 14;
			
			if (grad5[i][j] < 0) { 
				score1[i][j]--;
				event1[i][j][Up5] = false;
			}
			else event1[i][j][Up5] = true;

			if (grad10[i][j] < 0) { 
				score1[i][j]--;
				event1[i][j][Up10] = false;
			}
			else event1[i][j][Up10] = true;
		
			if (grad20[i][j] < 0) { 
				score1[i][j]--;
				event1[i][j][Up20] = false;
			}
			else event1[i][j][Up20] = true;
			
			if (grad120[i][j] < 0) { 
				score1[i][j]--;
				event1[i][j][Up120] = false;
			}
			else event1[i][j][Up120] = true;


			if (ma5[i][j] < ma10[i][j]) { 
				score1[i][j]--;
				event1[i][j][On5_10] = false;
			}
			else event1[i][j][On5_10] = true;

			if (ma5[i][j] < ma20[i][j]) { 
				score1[i][j]--;
				event1[i][j][On5_20] = false;
			}
			else event1[i][j][On5_20] = true;

			if (ma5[i][j] < ma120[i][j]) { 
				score1[i][j]--;
				event1[i][j][On5_120] = false;
			}
			else event1[i][j][On5_120] = true;


			if (ma10[i][j] < ma20[i][j]) { 
				score1[i][j]--;
				event1[i][j][On10_20] = false;
			}
			else event1[i][j][On10_20] = true;

			if (ma10[i][j] < ma120[i][j]) { 
				score1[i][j]--;
				event1[i][j][On10_120] = false;
			}
			else event1[i][j][On10_120] = true;


			
			if (ma20[i][j] < ma120[i][j]) { 
				score1[i][j]--;
				event1[i][j][On20_120] = false;
			}
			else event1[i][j][On20_120] = true;


			
			if (m_close[i][j] < ma5[i][j]) { 
				score1[i][j]--;
				event1[i][j][OnC_5] = false;
			}
			else event1[i][j][OnC_5] = true;

			if (m_close[i][j] < ma10[i][j]) { 
				score1[i][j]--;
				event1[i][j][OnC_10] = false;
			}
			else event1[i][j][OnC_10] = true; 

			if (m_close[i][j] < ma20[i][j]) { 
				score1[i][j]--;
				event1[i][j][OnC_20] = false;
			}
			else event1[i][j][OnC_20] = true;

			if (m_close[i][j] < ma120[i][j]) { 
				score1[i][j]--;
				event1[i][j][OnC_120] = false;
			}
			else event1[i][j][OnC_120] = true;


		}
	}
}

void calculate_four_scores2(int start, int end) {
	//---------------------------
	// Calculate global scores
	//--------------------------
	for (int i=start; i < end; i++) {

		for (int j=120; j < num_days[i]; j++) { // for all days

			score2[i][j] = 14;
			
			if (grad5[i][j] < 0) { 
				score2[i][j]--;
				event2[i][j][up5] = false;
			}
			else event2[i][j][up5] = true;

			if (grad10[i][j] < 0) { 
				score2[i][j]--;
				event2[i][j][up10] = false;
			}
			else event2[i][j][up10] = true;
		
			if (grad20[i][j] < 0) { 
				score2[i][j]--;
				event2[i][j][up20] = false;
			}
			else event2[i][j][up20] = true;
			
			if (grad60[i][j] < 0) { 
				score2[i][j]--;
				event2[i][j][up60] = false;
			}
			else event2[i][j][up60] = true;
			


			if (ma5[i][j] < ma10[i][j]) { 
				score2[i][j]--;
				event2[i][j][on5_10] = false;
			}
			else event2[i][j][on5_10] = true;

			if (ma5[i][j] < ma20[i][j]) { 
				score2[i][j]--;
				event2[i][j][on5_20] = false;
			}
			else event2[i][j][on5_20] = true;

			if (ma5[i][j] < ma60[i][j]) { 
				score2[i][j]--;
				event2[i][j][on5_60] = false;
			}
			else event2[i][j][on5_60] = true;

			if (ma10[i][j] < ma20[i][j]) { 
				score2[i][j]--;
				event2[i][j][on10_20] = false;
			}
			else event2[i][j][on10_20] = true;

			if (ma10[i][j] < ma60[i][j]) { 
				score2[i][j]--;
				event2[i][j][on10_60] = false;
			}
			else event2[i][j][on10_60] = true;


			
			if (ma20[i][j] < ma60[i][j]) { 
				score2[i][j]--;
				event2[i][j][on20_60] = false;
			}
			else event2[i][j][on20_60] = true;


			
			if (m_close[i][j] < ma5[i][j]) { 
				score2[i][j]--;
				event2[i][j][onC_5] = false;
			}
			else event2[i][j][onC_5] = true;

			if (m_close[i][j] < ma10[i][j]) { 
				score2[i][j]--;
				event2[i][j][onC_10] = false;
			}
			else event2[i][j][onC_10] = true; 

			if (m_close[i][j] < ma20[i][j]) { 
				score2[i][j]--;
				event2[i][j][onC_20] = false;
			}
			else event2[i][j][onC_20] = true;

			if (m_close[i][j] < ma60[i][j]) { 
				score2[i][j]--;
				event2[i][j][onC_60] = false;
			}
			else event2[i][j][onC_60] = true;

		}
	}
}

void calculate_three_scores(int start, int end) {
	//---------------------------
	// Calculate global scores
	//--------------------------
	for (int i=start; i < end; i++) {

		for (int j=120; j < num_days[i]; j++) { // for all days
			score3[i][j] = 9;
			if (grad5[i][j] < 0) { 
				score3[i][j]--;
				event3[i][j][U5] = false;
			}
			else event3[i][j][U5] = true;

			if (grad10[i][j] < 0) { 
				score3[i][j]--;
				event3[i][j][U10] = false;
			}
			else event3[i][j][U10] = true;
		
			if (grad20[i][j] < 0) { 
				score3[i][j]--;
				event3[i][j][U20] = false;
			}
			else event3[i][j][U20] = true;



			if (ma5[i][j] < ma10[i][j]) { 
				score3[i][j]--;
				event3[i][j][O5_10] = false;
			}
			else event3[i][j][O5_10] = true;

			if (ma5[i][j] < ma20[i][j]) { 
				score3[i][j]--;
				event3[i][j][O5_20] = false;
			}
			else event3[i][j][O5_20] = true;



			if (ma10[i][j] < ma20[i][j]) { 
				score3[i][j]--;
				event3[i][j][O10_20] = false;
			}
			else event3[i][j][O10_20] = true;


			
			if (m_close[i][j] < ma5[i][j]) { 
				score3[i][j]--;
				event3[i][j][OC_5] = false;
			}
			else event3[i][j][OC_5] = true;

			if (m_close[i][j] < ma10[i][j]) { 
				score3[i][j]--;
				event3[i][j][OC_10] = false;
			}
			else event3[i][j][OC_10] = true; 

			if (m_close[i][j] < ma20[i][j]) { 
				score3[i][j]--;
				event3[i][j][OC_20] = false;
			}
			else event3[i][j][OC_20] = true;

		}
	}
}

void calculate_states(int start, int end) {
	//-----------------------------
	// calculate(make) an integer
	//-----------------------------
	for (int i=start; i < end; i++) {
		for (int j=120; j < num_days[i]; j++) { // for all days
			state[i][j] = 0;
			for (int k=0; k < MAX_EVENT; k++) {
				if (event0[i][j][k]) {
					state[i][j] += (int)pow((long double)2, k);
				}
			}
		}
	}
}

void calculate_auxiliary_scores(int start, int end) {
	//------------------------------
	// calculate auxiliary scores 
	//------------------------------
	for (int i=start; i < end; i++) {
		for (int j=120; j < num_days[i]; j++) { // for all days
			aux_score[i][j] = 0;
			double open = (double)m_open[i][j];
			double high = (double)m_high[i][j];
			double low = (double)m_low[i][j];
			double close = (double)m_close[i][j];
			bool red = false;
			bool blue = false;

			if (open <= close) red = true;
			else blue = true;

			double higher = 0.0f;
			double lower = 0.0f;

			if (red) {
				higher = close; lower = open;
			}
			else {
				higher = open; lower = close;
			}

			high *= 1.01f; low *= 0.99f;

			if ((double)ma5[i][j] > higher && (double)ma5[i][j] < high) {
				aux_score[i][j]++;
				aux_event[i][j][RES5] = true;
			}
			else aux_event[i][j][RES5] = false;

			if ((double)ma10[i][j] > higher && (double)ma10[i][j] < high) {
				aux_score[i][j]++;
				aux_event[i][j][RES10] = true;
			}
			else aux_event[i][j][RES10] = false;

			if ((double)ma20[i][j] > higher && (double)ma20[i][j] < high) {
				aux_score[i][j]++;
				aux_event[i][j][RES20] = true;
			}
			else aux_event[i][j][RES20] = false;

			if ((double)ma60[i][j] > higher && (double)ma60[i][j] < high) {
				aux_score[i][j]++;
				aux_event[i][j][RES60] = true;
			}
			else aux_event[i][j][RES60] = false;

			if ((double)ma120[i][j] > higher && (double)ma120[i][j] < high) {
				aux_score[i][j]++;
				aux_event[i][j][RES120] = true;
			}
			else aux_event[i][j][RES120] = false;



			if ((double)ma5[i][j] > low && (double)ma5[i][j] < lower) {
				aux_score[i][j]++;
				aux_event[i][j][SUP5] = true;
			}
			else aux_event[i][j][SUP5] = false;

			if ((double)ma10[i][j] > low && (double)ma10[i][j] < lower) {
				aux_score[i][j]++;
				aux_event[i][j][SUP10] = true;
			}
			else aux_event[i][j][SUP10] = false;

			if ((double)ma20[i][j] > low && (double)ma20[i][j] < lower) {
				aux_score[i][j]++;
				aux_event[i][j][SUP20] = true;
			}
			else aux_event[i][j][SUP20] = false;

			if ((double)ma60[i][j] > low && (double)ma60[i][j] < lower) {
				aux_score[i][j]++;
				aux_event[i][j][SUP60] = true;
			}
			else aux_event[i][j][SUP60] = false;

			if ((double)ma120[i][j] > low && (double)ma120[i][j] < lower) {
				aux_score[i][j]++;
				aux_event[i][j][SUP120] = true;
			}
			else aux_event[i][j][SUP120] = false;


			if ((double)ma5[i][j] >= open && (double)ma5[i][j] <= close) {
				aux_score[i][j]++;
				aux_event[i][j][BU5] = true;
			}
			else aux_event[i][j][BU5] = false;

			if ((double)ma10[i][j] >= open && (double)ma10[i][j] <= close) {
				aux_score[i][j]++;
				aux_event[i][j][BU10] = true;
			}
			else aux_event[i][j][BU10] = false;

			if ((double)ma20[i][j] >= open && (double)ma20[i][j] <= close) {
				aux_score[i][j]++;
				aux_event[i][j][BU20] = true;
			}
			else aux_event[i][j][BU20] = false;

			if ((double)ma60[i][j] >= open && (double)ma60[i][j] <= close) {
				aux_score[i][j]++;
				aux_event[i][j][BU60] = true;
			}
			else aux_event[i][j][BU60] = false;

			if ((double)ma120[i][j] >= open && (double)ma120[i][j] <= close) {
				aux_score[i][j]++;
				aux_event[i][j][BU120] = true;
			}
			else aux_event[i][j][BU120] = false;


			if ((double)ma5[i][j] >= close && (double)ma5[i][j] <= open) {
				aux_score[i][j]++;
				aux_event[i][j][BD5] = true;
			}
			else aux_event[i][j][BD5] = false;

			if ((double)ma10[i][j] >= close && (double)ma10[i][j] <= open) {
				aux_score[i][j]++;
				aux_event[i][j][BD10] = true;
			}
			else aux_event[i][j][BD10] = false;

			if ((double)ma20[i][j] >= close && (double)ma20[i][j] <= open) {
				aux_score[i][j]++;
				aux_event[i][j][BD20] = true;
			}
			else aux_event[i][j][BD20] = false;

			if ((double)ma60[i][j] >= close && (double)ma60[i][j] <= open) {
				aux_score[i][j]++;
				aux_event[i][j][BD60] = true;
			}
			else aux_event[i][j][BD60] = false;

			if ((double)ma120[i][j] >= close && (double)ma120[i][j] <= open) {
				aux_score[i][j]++;
				aux_event[i][j][BD120] = true;
			}
			else aux_event[i][j][BD120] = false;

		}
	}
}

void calculate_global_volume_scores(int start, int end) {
	//---------------------------
	// Calculate volume global scores
	//--------------------------
	for (int i=start; i < end; i++) {


		for (int j=120; j < num_days[i]; j++) { // for all days
			vscore[i][j] = 14;
			if (vgrad5[i][j] < 0) { 
				vscore[i][j]--;
				vevent[i][j][VUP5] = false;
			}
			else vevent[i][j][VUP5] = true;
		
			if (vgrad20[i][j] < 0) { 
				vscore[i][j]--;
				vevent[i][j][VUP20] = false;
			}
			else vevent[i][j][VUP20] = true;
			
			if (vgrad60[i][j] < 0) { 
				vscore[i][j]--;
				vevent[i][j][VUP60] = false;
			}
			else vevent[i][j][VUP60] = true;
			
			if (vgrad120[i][j] < 0) { 
				vscore[i][j]--;
				vevent[i][j][VUP120] = false;
			}
			else vevent[i][j][VUP120] = true;


			if (vma5[i][j] < vma20[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON5_20] = false;
			}
			else vevent[i][j][VON5_20] = true;

			if (vma5[i][j] < vma60[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON5_60] = false;
			}
			else vevent[i][j][VON5_60] = true;

			if (vma5[i][j] < vma120[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON5_120] = false;
			}
			else vevent[i][j][VON5_120] = true;

			
			if (vma20[i][j] < vma60[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON20_60] = false;
			}
			else vevent[i][j][VON20_60] = true;

			if (vma20[i][j] < vma120[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON20_120] = false;
			}
			else vevent[i][j][VON20_120] = true;



			if (vma60[i][j] < vma120[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VON60_120] = false;
			}
			else vevent[i][j][VON60_120] = true;


			
			if (m_volume[i][j] < vma5[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VONC_5] = false;
			}
			else vevent[i][j][VONC_5] = true;

			if (m_volume[i][j] < vma20[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VONC_20] = false;
			}
			else vevent[i][j][VONC_20] = true;

			if (m_volume[i][j] < vma60[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VONC_60] = false;
			}
			else vevent[i][j][VONC_60] = true;

			if (m_volume[i][j] < vma120[i][j]) { 
				vscore[i][j]--;
				vevent[i][j][VONC_120] = false;
			}
			else vevent[i][j][VONC_120] = true;

		}
	}
}

void calculate_volume_states(int start, int end) {
	//-----------------------------
	// calculate(make) an integer
	//-----------------------------
	for (int i=start; i < end; i++) {
		for (int j=120; j < num_days[i]; j++) { // for all days
			vstate[i][j] = 0;
			for (int k=0; k < MAX_VEVENT; k++) {
				if (vevent[i][j][k]) {
					vstate[i][j] += (int)pow((long double)2, k);
				}
			}
		}
	}
}

void make_virtual_log(char *code, char *date) {
	for (int i=0; i < my_stock_count; i++) {
		int count=0;
		if (!strcmp(code, my_stock_code[i])) {
			while (true) {
				strcpy_s(date_string[my_stock_count][count], 
					   date_string[i][count]);
				m_open[my_stock_count][count] = m_open[i][count];
				m_high[my_stock_count][count] = m_high[i][count];
				m_low[my_stock_count][count] = m_low[i][count];
				m_close[my_stock_count][count] = m_close[i][count];
				m_volume[my_stock_count][count] = m_volume[i][count];
				count++;
				if (!strcmp(date_string[my_stock_count][count - 1], 
					   date)) break;
			}
			strcpy_s(date_string[my_stock_count][count], "20170101");
			m_open[my_stock_count][count] = 
			m_low[my_stock_count][count] = 
				m_close[my_stock_count][count - 1];
			m_high[my_stock_count][count] = 
			m_close[my_stock_count][count] = 
				(int)(1.10f * (double)m_close[my_stock_count][count - 1]);
			m_volume[my_stock_count][count] = 
				(int)(2.1f * 
					( ((double)vma20[i][count - 1] + 
					  (double)vma60[i][count - 1])/2.0f ));
			count++;
			num_days[my_stock_count] = count;
			break;
		}
	}
}

int i_index=0; int j_index=0;
int make_index(char *a, char *b) {
	for (int i=0; i < my_stock_count; i++) {
		if (strcmp(my_stock_code[i], a)) continue;		
		for (int j=120; j < num_days[i]; j++) {
			if (strcmp(date_string[i][j], b)) continue;
			i_index = i; j_index = j;
			return 1;
		}
	}
	return 0;
}

int big_vol_dist(int a, int b) {
	for (int i=0; i < 70; i++) {
		if ((double)m_volume[a][b-i] > 5.0f*vma60[a][b-i] &&
			(double)m_volume[a][b-i] > 5.0f*vma120[a][b-i]) return i;
	}
	return -1;
}

bool volatille(int i, int j) {
	int a=0;
	for (int k=1; k < 90; k++) {
		if (rate(m_high[i][j-k], m_close[i][j-k-1]) > 12.0f)
			return true;
	}
	return false;
}

bool affordable_trading_money(int i, int j) {
	if ((double)m_close[i][j] * vma120[i][j] < 500000000.0f)
		return false;
	else return true;
}

bool rest_on_20(int i, int j) {
	if (!volatille(i, j)) return false;
	if (!affordable_trading_money(i, j)) return false;

	if (disp5[i][j] < 0) return false;
	if (disp10[i][j] < 0) return false;
	if (disp20[i][j] < 0) return false;
	if (rate((int)ma5[i][j], (int)ma20[i][j]) < 0) return false;
	if (rate((int)ma10[i][j], (int)ma20[i][j]) < 0) return false;
	if (grad20[i][j] < 0.3f) return false;
	
	int count = 0;
	double h = 1.03f * (double)m_close[i][j];
	double l = 0.97f * (double)m_close[i][j];
	for (int k=1; k <16; k++) {
		if (m_open[i][j-k] >= l && m_open[i][j-k] <= h) count++;
		if (m_high[i][j-k] >= l && m_high[i][j-k] <= h) count++;
		if (m_low[i][j-k] >= l && m_low[i][j-k] <= h) count++;
		if (m_close[i][j-k] >= l && m_close[i][j-k] <= h) count++;
	}
	if (count >= 30) return true;
	else return false;
}

bool caos_nabi(int i, int j) {
	if (score[i][j] != 0) return false;

	if (disp5[i][j] > -6.8f) return false;
	if (disp5[i][j-1] > 0.0f) return false;
	if (disp5[i][j-2] > 0.0f) return false;
	if (disp5[i][j-3] > 0.0f) return false;
	if (disp5[i][j-4] > 0.0f) return false;

	if (disp10[i][j-1] > 0.0f) return false;
	if (disp10[i][j-2] > 0.0f) return false;
	if (disp10[i][j-3] > 0.0f) return false;
	if (disp10[i][j-4] > 0.0f) return false;

	if (disp20[i][j-1] > 0.0f) return false;
	if (disp20[i][j-2] > 0.0f) return false;
	if (disp20[i][j-3] > 0.0f) return false;
	if (disp20[i][j-4] > 0.0f) return false;

	return true;
}

bool hided_at_100_highest(int i, int j) {
	if (j < 100) return false;
	int c = m_close[i][j];
	int h = m_high[i][j];
	int c_count=0, h_count=0;
	for (int k=1; k < 20; k++) {
		if (m_close[i][j-k] > h) h_count++;
	}
	if (h_count > 0) return false;
	c_count=0; h_count=0;
	for (int k=1; k < 100; k++) {
		if (m_close[i][j-k] > c) c_count++;
		if (m_close[i][j-k] > h) h_count++;
	}
	if (h_count == 1 && c_count > 1) return true;
	else return false;
}

int get_index(char *stock) {
	for (int i=0; i < my_stock_count; i++) {
		if (!strcmp(my_stock_code[i], stock)) return i;
	}
	return -1;
}

void calculate_volume_cliffs(int start, int end) {
	for (int i=start; i < end; i++) {
		if (num_days[i] < 240) {
			for (int j=0; j < num_days[i]; j++) {
				volume_cliff[i][j]=false;
			}
			continue;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 150) {
				volume_cliff[i][j]=false;
				continue;
			}
			if (m_volume[i][j] == 0) {
				volume_cliff[i][j]=false;
				continue;
			}
			if (vma120[i][j] > 0.85f * vma120[i][j-6] &&
				vma60[i][j] > 0.65f * vma60[i][j-6]) {
				volume_cliff[i][j]=false;	
				continue;
			}

			if (vma120[i][j] < 2.0f * vma20[i][j]) {
				volume_cliff[i][j]=false;	
				continue;
			}
			volume_cliff[i][j]=true;
		}
	}
}

void calculate_ma10_cliffs(int start, int end) {
	for (int i=start; i < end; i++) {
		if (num_days[i] < 240) {
			for (int j=0; j < num_days[i]; j++) {
				ma10_cliff[i][j]=false;
			}
			continue;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 150) {
				ma10_cliff[i][j]=false;
				continue;
			}
			if (m_volume[i][j] == 0) {
				ma10_cliff[i][j]=false;
				continue;
			}
			double sum=0;
			for (int k=j; k > j-28; k--) {
				sum+=grad10[i][k];
			}
			if (sum > -42.0f) continue;
			ma10_cliff[i][j]=true;
		}
	}
}

void calculate_ma10_valleys(int start, int end) {
	for (int i=start; i < end; i++) {
		if (num_days[i] < 240) {
			for (int j=0; j < num_days[i]; j++) {
				ma10_valley[i][j]=false;
			}
			continue;
		}
		for (int j=0; j < num_days[i]; j++) {
			if (j < 150) {
				ma10_valley[i][j]=false;
				continue;
			}
			if (m_volume[i][j] == 0) {
				ma10_valley[i][j]=false;
				continue;
			}
			bool running=true;
			for (int k=j; k > j-25; k--) {
				if(grad10[i][k] > 0) { running=false; break; }
			}
			if (!running) continue;
			ma10_valley[i][j]=true;
		}
	}
}

void set_trainable_dates() {
	for (int i = 0; i < my_stock_count; i++) {
		for (int j = 0; j < num_days[i]; j++) is_trainable[i][j] = true;
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int j = 0; j < 133; j++) is_trainable[i][j] = false;
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int j = 59; j < 119; j++) ma120[i][j] = ma60[i][j];
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int j = 0; j < 59; j++) {
			ma60[i][j] = ma60[i][0 + 59];
			ma120[i][j] = ma60[i][0 + 59];
		}
	}

	for (int i = 0; i < my_stock_count; i++) reset_count[i] = 0;

	for (int i = 0; i < my_stock_count; i++) {
		reset_start[i][0] = 0; reset_end[i][0] = 0; reset_count[i] = 1;
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int j = 0; j < num_days[i]; j++) {
			if (j >= num_days[i] - 2) continue;
			if (m_volume[i][j] != 0 && m_volume[i][j + 1] == 0 && m_volume[i][j + 2] == 0) {
				reset_start[i][reset_count[i]++] = j + 1;
			}
		}
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int k = 0; k < reset_count[i]; k++) {
			int j;
			for (j = reset_start[i][k] + 1; j < num_days[i]; j++) {
				if (m_volume[i][j] != 0) break;
			}
			reset_end[i][k] = j;
		}
	}

	for (int i = 0; i < my_stock_count; i++) {
		for (int k = 0; k < reset_count[i]; k++) {

		}
	}

	for (int i = 0; i < my_stock_count; i++) {	
		for (int k = 0; k < reset_count[i]; k++) {
			int last = 0;
			if (k == reset_count[i] - 1) last = num_days[i] - 1;
			else last = reset_start[i][k + 1] - 1;

			int interval = last - reset_end[i][k] + 1;

			reset_interval[i][k] = interval;

		}
	}

	for (int i = 0; i < my_stock_count; i++) {	
		for (int k = 0; k < reset_count[i]; k++) {
			if (reset_interval[i][k] < 133) {
				int a = reset_start[i][k];
				int b = 0;
				if (k == reset_count[i] - 1) b = num_days[i];
				else b = reset_start[i][k + 1];
				for (int l = a; l < b; l++) {
					is_trainable[i][l] = false;
				}
				continue;
			}
			int start = reset_end[i][k];
			int recover = start + 119;
			for (int l = start + 59; l < recover; l++) {
				ma120[i][l] = ma60[i][l];
			}
			for (int l = start; l < start + 59; l++) {
				ma60[i][l] = ma60[i][start + 59];
				ma120[i][l] = ma60[i][start + 59];
			}
			for (int l = reset_start[i][k]; l < reset_end[i][k] + 133; l++) {
				is_trainable[i][l] = false;
			}
		}
	}
}


void upward_reversal_feature_model(bool on, int start, int end, int mode) 
{
	if (!on) {
		return;
	}

	FILE *p;
	if (mode == 0) fopen_s(&p, "train.txt", "w");
	else if (mode == 1) fopen_s(&p, "test.txt", "w");
	else fopen_s(&p, "now.txt", "w");

	double tmp, max, min, para;
	int stochastic_max_price, stochastic_min_price;

	pPrintfDlg->printf2("upward_reversal_feature_model 수행 시작...");

	int dist[57] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
		42, 44, 46, 48, 50, 52, 54, 56, 58, 60,
		63, 66, 69, 72, 75, 78, 81, 84, 87, 90,
		94, 98, 102, 106, 110, 114, 120 };

	int dist2[30] = { 0, 1, 2, 3, 4, 5, 7, 9, 11, 13, 15,
		18, 21, 24, 27, 30, 34, 38, 42, 46, 50,
		55, 60, 65, 70, 75, 80, 85, 90, 95 };

	pPrintfDlg->printf2("make_new_feature_engine() 수행 시작...");

	for (int i = 0; i < my_stock_count; i++) {

		if (i % 2 != 0) continue; 

		if (num_days[i] - start < 133) continue;
		for (int j = num_days[i] - start; j < num_days[i] - end - 6; j++) {
			if (j < 0)	
				continue;	
			if (!is_trainable[i][j]) continue;
			if (m_volume[i][j] == 0) continue;
			if (m_volume[i][j - 5] == 0) continue;
			if (m_volume[i][j - 10] == 0) continue;
			if (m_volume[i][j - 15] == 0) continue;
			if (m_volume[i][j - 30] == 0) continue;
			if (num_days[i] - start <= 0) continue;
			int max_price = 0;
			for (int k = j - 1; k > j - 35; k--) {
				if (m_close[i][k] > max_price)
				{
					max_price = m_close[i][k];
				}
			}

			if (m_close[i][j] == 0) continue;
			if (m_close[i][j - 1] == 0) continue;
			if (m_close[i][j - 2] == 0) continue;
			if (m_close[i][j - 4] == 0) continue;
			if (m_close[i][j - 7] == 0) continue;
			if (m_close[i][j - 12] == 0) continue;
			if (m_close[i][j - 20] == 0) continue;
			if (m_close[i][j - 33] == 0) continue;
			if (m_close[i][j - 54] == 0) continue;
			if (m_close[i][j - 88] == 0) continue;
			if (m_close[i][j - 133] == 0) continue;
			if (m_open[i][j] == 0) continue;
			if (m_open[i][j - 1] == 0) continue;
			if (ma5[i][j] == 0.0f) continue;
			if (ma10[i][j] == 0.0f) continue;
			if (ma20[i][j] == 0.0f) continue;
			if (ma60[i][j] == 0.0f) continue;
			if (ma120[i][j] == 0.0f) continue;
			if (ma5[i][j - 1] == 0.0f) continue;
			if (ma10[i][j - 1] == 0.0f) continue;
			if (ma20[i][j - 1] == 0.0f) continue;
			if (ma60[i][j - 1] == 0.0f) continue;
			if (ma120[i][j - 1] == 0.0f) continue;
			if (vma5[i][j] == 0.0f) continue;
			if (vma20[i][j] == 0.0f) continue;
			if (vma60[i][j] == 0.0f) continue;
			if (vma120[i][j] == 0.0f) continue;
			if (vma5[i][j - 1] == 0.0f) continue;
			if (vma20[i][j - 1] == 0.0f) continue;
			if (vma60[i][j - 1] == 0.0f) continue;
			if (vma120[i][j - 1] == 0.0f) continue;
			if (vma60[i][j - 20] == 0.0f) continue;
			if (vma60[i][j - 60] == 0.0f) continue;
			if (vma60[i][j - 120] == 0.0f) continue;
			if (vma120[i][j - 20] == 0.0f) continue;
			if (vma120[i][j - 60] == 0.0f) continue;
			if (vma120[i][j - 120] == 0.0f) continue;
			if (vma20[i][j - 2] == 0.0f) continue; 
			if (m_volume[i][j - 1] == 0) continue;
			if (m_volume[i][j - 2] == 0) continue;
			if (m_volume[i][j - 3] == 0) continue; 
			if (m_close[i][j] == 0 && m_open[i][j] == 0 && m_high[i][j] == 0 && m_low[i][j] == 0 && m_volume[i][j] == 0) continue;

			stochastic_min_price = m_close[i][j];
			stochastic_max_price = m_close[i][j];

			for (int k = j - 1; k > j - 5; k--) {
				if (m_close[i][k] < stochastic_min_price)
				{
					stochastic_min_price = m_close[i][k];
				}
				if (m_close[i][k] > stochastic_max_price)
				{
					stochastic_max_price = m_close[i][k];
				}
			}

			double sum = 0.0f;

			for (int k = j; k > j - 40; k--) {
				sum += grad60[i][k];
			}
			tmp = sum;
			max = 45.0f;		min = -45.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			sum = 0.0f;
			for (int k = j; k > j - 40; k--) {
				sum += grad120[i][k];
			}
			tmp = sum;
			max = 35.0f;		min = -35.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);
			for (int k = 0; k < 30; k++) { 
				tmp = rate((int)vma60[i][j], (int)vma60[i][j - dist2[k]]);
				max = 275.0f;		min = -275.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			for (int k = 0; k < 30; k++) { 
				tmp = rate((int)vma120[i][j], (int)vma120[i][j - dist2[k]]);
				max = 275.0f;		min = -275.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}


			for (int k = 0; k < 55; k++) { 
				tmp = rate(m_close[i][j], m_close[i][j - dist[k]]);
				max = 175.0f;		min = -175.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			for (int k = 0; k < 55; k++) { 
				tmp = rate(m_close[i][j], m_close[i][j - dist[k]]);
				max = 35.0f;		min = -35.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}
			// disparity
			tmp = disp5[i][j];
			max = 18.0f;	min = -18.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp10[i][j];
			max = 22.0f;	min = -22.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp20[i][j];
			max = 27.0f;	min = -27.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp60[i][j];
			max = 32.0f;	min = -32.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);


			tmp = disp120[i][j];
			max = 37.0f;	min = -37.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			// gradient
			tmp = grad5[i][j];
			max = 4.5f;		min = -4.5f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad10[i][j];
			max = 3.5f;		min = -3.5f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad20[i][j];
			max = 2.8f;		min = -2.8f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad60[i][j];
			max = 2.2f;		min = -2.2f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad120[i][j];
			max = 2.0f;		min = -2.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			// disparity2
			tmp = disp5[i][j - 1];
			max = 28.0f;	min = -28.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp10[i][j - 1];
			max = 32.0f;	min = -32.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp20[i][j - 1];
			max = 37.0f;	min = -37.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp60[i][j - 1];
			max = 52.0f;	min = -52.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = disp120[i][j - 1];
			max = 57.0f;	min = -57.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			// gradient2
			tmp = grad5[i][j - 1];
			max = 6.5f;		min = -6.5f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad10[i][j - 1];
			max = 5.5f;		min = -5.5f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad20[i][j - 1];
			max = 4.8f;		min = -4.8f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad60[i][j - 1];
			max = 4.2f;		min = -4.2f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);

			tmp = grad120[i][j - 1];
			max = 4.0f;		min = -4.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para);


			//Volume disp
			for (int k = 0; k < 30; k++) { 	
				tmp = rate((int)vma60[i][j - dist2[k]], (int)vma120[i][j - dist2[k]]);
				max = 70.0f;		min = -70.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			for (int k = 0; k < 30; k++) {  	
				tmp = rate((int)vma20[i][j - dist2[k]], (int)vma60[i][j - dist2[k]]);
				max = 70.0f;		min = -70.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			for (int k = 0; k < 30; k++) { 
				tmp = rate((int)vma5[i][j - dist2[k]], (int)vma20[i][j - dist2[k]]);
				max = 70.0f;		min = -70.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			for (int k = 0; k < 30; k++) { 	
				tmp = rate(m_volume[i][j - dist2[k]], (int)vma5[i][j - dist2[k]]);
				max = 70.0f;		min = -70.0f;
				para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
				if (para < 0.0001f) para = 0.0001f;
				if (para > 0.9999f) para = 0.9999f;
				fprintf(p, "%.4f ", para);
			}

			// voulme gradient 
			tmp = vgrad5[i][j];
			max = 90.0f;		min = -500.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para); 

			tmp = vgrad10[i][j];
			max = 50.0f;		min = -100.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para); 

			tmp = vgrad20[i][j];
			max = 90.0f;		min = -200.0f;
			para = (0.9999f * (tmp - min) + 0.0001f * (max - tmp)) / (max - min);
			if (para < 0.0001f) para = 0.0001f;
			if (para > 0.9999f) para = 0.9999f;
			fprintf(p, "%.4f ", para); 

			if (mode == 2) tmp = -30.0f;
			else
				for (int day = 0; day < 12; day++){
					tmp += (rate(m_close[i][j + day +1], m_close[i][j-day])
				}

			tmp /= 100.0f;
			if (tmp > 1.0f) tmp = 1.0f;
			else if (tmp < -1.0f) tmp = -1.0f;
			if (mode != 0) {
				fprintf(p, "%.4f ", tmp);
				fprintf(p, "%s %s\n", my_stock_code[i], date_string[i][j]);
			}
			else if (mode == 0) {
				fprintf(p, "%.4f\n", tmp);
			}

		}
	}
	fclose(p);

	pPrintfDlg->printf2("done.");
}


#define	OLD_CSV_FORMAT

UINT TensorflowInputGeneratingThreadMain(LPVOID lParam)
{
	CNNshellView* pView = (CNNshellView *)lParam;
	CNNshellDoc* pDoc = pView->GetDocument();
	// pDoc->strFromTodayDate

	char fname[200];
	CString Dates[MAX_FUNDSIM_DATES];
	FILE *pFile, *p_file;;
	int ret;

	strcpy_s(fname, "\\FundsimTable.txt");
	fopen_s(&pFile, fname, "r");

	char cc[200];
	int DateCount = 0;
	CString cs;

	while(fscanf_s(pFile, "%s", cc, _countof(cc)) != EOF) {
		if (DateCount >= MAX_FUNDSIM_DATES) {
			break;
		}
		cs = cc;
		if (cs > pDoc->strFromTodayDate)
			continue;
		Dates[DateCount++] = cs;
	}
	fclose(pFile);


	pPrintfDlg->printf2("Date Count = %d\n", DateCount);
	pPrintfDlg->printf2("Dates[0]: %s, Dates[1]: %s", Dates[0], Dates[1]);

	
	strcpy_s(fname, "\\DBupdate\\");
	strcat_s(fname, Dates[0]);
	strcat_s(fname, "-코스피.csv");

	ret = fopen_s(&p_file, fname, "r");
	if (ret != S_OK) {
		pPrintfDlg->printf2("%s file not found or open error!", fname);
		return(-100); 
	}
	fclose(p_file);

#ifdef OLD_CSV_FORMAT
	::fclose(p_file);

	CSpreadSheet kseCsv(fname, ",", false);
	CStringArray Row;
	stock_count = 0;
	for (int i = 2; i <= kseCsv.GetTotalRows(); i++) {	
		if (stock_count >= MAX_STOCK) {
			return -30;
		}

		kseCsv.ReadRow(Row, i);		
		cs = Row.GetAt(0);			
		strcpy_s(cc, cs.GetBuffer()+1);	
		if (_tcslen(cc) != CODE_LEN)	
			continue;
		strcpy_s(stock_code[stock_count], cc);

		cs = Row.GetAt(1);			
			
		strcpy_s(cc, cs.GetBuffer());
		strcpy_s(stock_name[stock_count], cc);

		cs = Row.GetAt(2);			// close
		strcpy_s(cc, cs.GetBuffer());
		close[stock_count] = atoi(cc);



		cs = Row.GetAt(6);			// vol
		::strcpy(cc, cs.GetBuffer());
		volume[stock_count] = atoi(cc);


		cs = Row.GetAt(9);			// open
		::strcpy(cc, cs.GetBuffer());
		open[stock_count] = atoi(cc);

		cs = Row.GetAt(10);		// high
		::strcpy(cc, cs.GetBuffer());
		high[stock_count] = atoi(cc);

		cs = Row.GetAt(11);		// 저가
		::strcpy(cc, cs.GetBuffer());
		low[stock_count] = atoi(cc);		//low

		stock_count++;
	pPrintfDlg->printf2("%s 읽기 완료", fname); 

	strcpy_s(fname, "\\DBupdate\\");
	strcat_s(fname, Dates[0]);
	strcat_s(fname, "-코스닥.csv");

	ret = fopen_s(&p_file, fname, "r");
	if (ret != S_OK) {
		pPrintfDlg->printf2("%s file not found or open error!", fname); 
		return(-100); 
	}
	::fclose(p_file);

	CSpreadSheet kosdaqCsv(fname, ",", false);
	for (int i = 2; i <= kosdaqCsv.GetTotalRows(); i++) {	
		if (stock_count >= MAX_STOCK) {
			return -30;
		}

		kosdaqCsv.ReadRow(Row, i);	

		cs = Row.GetAt(0);			
		::strcpy(cc, cs.GetBuffer()+1);	
		if (_tcslen(cc) != CODE_LEN)	
			continue;
		strcpy_s(stock_code[stock_count], cc);

		cs = Row.GetAt(1);			// stock name
		::strcpy(cc, cs.GetBuffer());
		strcpy_s(stock_name[stock_count], cc);

		cs = Row.GetAt(2);			
		::strcpy(cc, cs.GetBuffer());
		close[stock_count] = atoi(cc);

		cs = Row.GetAt(6);			
		::strcpy(cc, cs.GetBuffer());
		volume[stock_count] = atoi(cc);

		cs = Row.GetAt(9);			
		::strcpy(cc, cs.GetBuffer());
		open[stock_count] = atoi(cc);

		cs = Row.GetAt(10);			
		::strcpy(cc, cs.GetBuffer());
		high[stock_count] = atoi(cc);

		cs = Row.GetAt(11);			 
		::strcpy(cc, cs.GetBuffer());
		low[stock_count] = atoi(cc);

		stock_count++;
	} 
#else
	char buf[2048];
	int idx;

	fgets(buf, 2047, p_file); 

	CDBUpdate *dbupdate_object = new CDBUpdate();	

	stock_count = 0;
	while (fgets(buf, 2047, p_file)) {	
		if (stock_count >= MAX_STOCK) {
			return -30;
		}

		idx = dbupdate_object->gettoken_csv(1, cc, buf);	
		if (_tcslen(cc) != CODE_LEN)	
			continue;
		strcpy_s(stock_code[stock_count], cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		strcpy_s(stock_name[stock_count], cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		close[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		volume[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		open[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		high[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		low[stock_count] = atoi(cc);		

		stock_count++;

	fclose(p_file);

	// 코스닥 디비를 읽는다.
	strcpy_s(fname, "\\DBupdate\\");
	strcat_s(fname, Dates[0]);
	strcat_s(fname, "-코스닥.csv");

	ret = fopen_s(&p_file, fname, "r");
	if (ret != S_OK) {
		pPrintfDlg->printf2("%s file not found or open error!", fname);
		return(-100);
	}

	fgets(buf, 2047, p_file); 

	while (fgets(buf, 2047, p_file)) {	
		if (stock_count >= MAX_STOCK) {
			return -30;
		}

		idx = dbupdate_object->gettoken_csv(1, cc, buf);	
		if (_tcslen(cc) != CODE_LEN)	
			continue;
		strcpy_s(stock_code[stock_count], cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);
		strcpy_s(stock_name[stock_count], cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		close[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		volume[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		open[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);	
		high[stock_count] = atoi(cc);

		idx = dbupdate_object->gettoken_csv(idx, cc, buf);
		low[stock_count] = atoi(cc);	

		stock_count++;
	} 

	fclose(p_file);
	delete dbupdate_object;
#endif // OLD_CSV_FORMAT


	strcpy_s(fname, "\\stockTable.txt");
	fopen_s(&p_file, fname, "r");
	if (p_file == NULL) {
		return -700;
	}
	my_stock_count = 0;
	while(fscanf_s(p_file, "%s", cc, _countof(cc)) != EOF) {
		cc[6] = '\0';
		strcpy_s(my_stock_code[my_stock_count++], cc);
		if (my_stock_count >= MAX_MY_STOCK) {
			return -80;
		}
	}
	fclose(p_file);
	
	for (int i=0; i < my_stock_count; i++) {
		for (int j=0; j < stock_count; j++) {
			if (!strcmp(stock_code[j], my_stock_code[i])) {
				strcpy_s(my_stock_name[i], stock_name[j]);
			}
		}
	}

	// read primitive data until yesterday from stock files
	for (int i=0; i < my_stock_count; i++) {
		char tmp[100];
		char a[30], b[30], c[30], d[30], e[30], f[30];

		strcpy_s(tmp, "\\Works\\NNshell바이너리\\재원TXT디비\\StockDB\\");	
		strcat_s(tmp, 100, my_stock_code[i]);
		strcat_s(tmp, 100, ".txt");


		FILE *p_file;
		fopen_s(&p_file, tmp, "r");
		if (p_file == NULL) {
			pPrintfDlg->printf2("%s : cannot find this file!", tmp);
			return(-300);
		}

		int day_count = 0;

		while (true) {
			if (day_count >= MAX_TXT_DB_DATES) {
				return -900;
			}

			if (fscanf_s(p_file, "%s", a, _countof(a)) == EOF) break;
			if (fscanf_s(p_file, "%s", b, _countof(b)) == EOF) break;
			if (fscanf_s(p_file, "%s", c, _countof(c)) == EOF) break;
			if (fscanf_s(p_file, "%s", d, _countof(d)) == EOF) break;
			if (fscanf_s(p_file, "%s", e, _countof(e)) == EOF) break;
			if (fscanf_s(p_file, "%s", f, _countof(f)) == EOF) break;

			if (strlen(a) != 8) {	// date string
				pPrintfDlg->printf2("File Format Error in Stock~!");
				exit(0);
			}
			strcpy_s(date_string[i][day_count], a);

			m_open[i][day_count] = atoi(b);
			m_high[i][day_count] = atoi(c);
			m_low[i][day_count] = atoi(d);
			m_close[i][day_count] = atoi(e);
			m_volume[i][day_count++] = atoi(f);

			cs = a;
			if (cs >= pDoc->strFromTodayDate)		
				break;								
		}
		num_days[i] = day_count; // num_days[i]
		fclose(p_file);
	}
	if (cs == pDoc->strFromTodayDate) { 
	}
	else {	
		for (int i=0; i < my_stock_count; i++) {
			if (strcmp(date_string[i][num_days[i] - 1], Dates[1])) {
				return(-500);
			}
		}
		for (int i=0; i < my_stock_count; i++) {

			if (strcmp(date_string[i][num_days[i] - 1], Dates[1])) continue;

			char tmp[100];
			strcpy_s(tmp, "\\StockDB\\");
			strcat_s(tmp, 100, my_stock_code[i]);
			strcat_s(tmp, 100, ".txt");

			FILE *p_file;
			fopen_s(&p_file, tmp, "w"); // write~!

			for (int j = 0; j < num_days[i]; j++)
				fprintf(p_file, "%s %d %d %d %d %d\n", date_string[i][j], m_open[i][j], m_high[i][j], m_low[i][j], m_close[i][j], m_volume[i][j]);
	
			// Here, num_days[i] are actually increased by 1
			int index=0;
			// We can use a previously found index, but...
			for (int j=0; j < stock_count; j++) {
				if (!strcmp(stock_code[j], my_stock_code[i])) {
					fprintf(p_file, "%s %d %d %d %d %d\n", Dates[0], open[j], high[j], low[j], close[j], volume[j]);
				}
			}
			fclose(p_file);
		}
		pPrintfDlg->printf2("done!");
	}

	strcpy_s(fname, "\\stockTable.txt");
	fopen_s(&p_file, fname, "r");
	if (p_file == NULL) {
		return -600;
	}

	my_stock_count = 0;
	while(fscanf_s(p_file, "%s", cc, _countof(cc)) != EOF) {
		cc[6] = '\0';
		strcpy_s(my_stock_code[my_stock_count++], cc);
		if (my_stock_count >= MAX_MY_STOCK) {
			return -90;
		}
	}	
	fclose(p_file);
	for (int i=0; i < my_stock_count; i++) {
		for (int j=0; j < stock_count; j++) {
			if (!strcmp(stock_code[j], my_stock_code[i])) {
				strcpy_s(my_stock_name[i], stock_name[j]);
			}
		}
	}

	for (int i=0; i < my_stock_count; i++) {
		char tmp[100];
		char a[30], b[30], c[30], d[30], e[30], f[30];
		int day_count;

		strcpy_s(tmp, "\\StockDB\\");
		strcat_s(tmp, 100, my_stock_code[i]);
		strcat_s(tmp, 100, ".txt");
		fopen_s(&p_file, tmp, "r");
		if (p_file == NULL) {
			continue;
		}

		day_count = 0;
		while (true) {
			if (fscanf_s(p_file, "%s", a, _countof(a)) == EOF) break;
			if (fscanf_s(p_file, "%s", b, _countof(b)) == EOF) break;
			if (fscanf_s(p_file, "%s", c, _countof(c)) == EOF) break;
			if (fscanf_s(p_file, "%s", d, _countof(d)) == EOF) break;
			if (fscanf_s(p_file, "%s", e, _countof(e)) == EOF) break;
			if (fscanf_s(p_file, "%s", f, _countof(f)) == EOF) break;

			strcpy_s(date_string[i][day_count], a);
			m_open[i][day_count] = atoi(b);
			m_high[i][day_count] = atoi(c);
			m_low[i][day_count] = atoi(d);
			m_close[i][day_count] = atoi(e);
			m_volume[i][day_count++] = atoi(f);

			cs = a;
			if (cs >= pDoc->strFromTodayDate)	
				break;	
		}
		num_days[i] = day_count; 
		fclose(p_file);
	}
	pPrintfDlg->printf2("done!");

	pPrintfDlg->printf2("이제 각종 피쳐들을 계산합니다...");

	calculate_indicators(0, my_stock_count);
	calculate_global_scores(0, my_stock_count);
	calculate_states(0, my_stock_count);
	calculate_four_scores1(0, my_stock_count);
	calculate_four_scores2(0, my_stock_count);
	calculate_three_scores(0, my_stock_count);
	calculate_auxiliary_scores(0, my_stock_count);
	calculate_global_volume_scores(0, my_stock_count);
	calculate_volume_states(0, my_stock_count);
	calculate_volume_cliffs(0, my_stock_count);
	calculate_ma10_cliffs(0, my_stock_count);
	calculate_ma10_valleys(0, my_stock_count);
	pPrintfDlg->printf2("done!");

	pPrintfDlg->printf2(" ");
	pPrintfDlg->printf2(" ");

	set_trainable_dates();

	//upward_reversal_feature_model
	upward_reversal_feature_model(1, 2000, 961, 0);
	upward_reversal_feature_model(1, 960, 501, 1);
	upward_reversal_feature_model(1, 500, 0, 2);

	
	return 0;
}
