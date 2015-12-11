#ifndef __utility__
#define __utility__
#include <string>
#include <algorithm>
#include "allheaders.h"
#include <math.h>
#include <vector>
#include <map>

using namespace std;

const int scale_factor_line_layout = 10;

class Util {
public:
	static string codeToText(int n) {
		string s;
		if (n < 0) {
			s += "N";
			n = -n;
		}
		string t;
		do {
			t += 'a' + n%26;
			n /= 26;
		} while (n);
		reverse(t.begin(), t.end());
		return s + t;
	}
	static double getBoxaSkew(Boxa* boxa) {/*
		vector<vector<Box*> > near; //3 height nearby's neighbors
		Boxa* sort_boxa = boxaSort(boxa, L_SORT_BY_Y, L_SORT_INCREASING, NULL);
		for (int i = 0; i < sort_boxa->n; i++) {
			vector<Box*> v;
			Box* ib = boxaGetBox(sort_boxa, i, L_CLONE);
			v.push_back(ib);
			for (int j = i + 1; j < sort_boxa->n; j++) {
				Box* b = boxaGetBox(sort_boxa, i, L_CLONE);
				if (b->y - ib->y > 3*ib->h)
					break;
				if (b->w*b->h*3 < ib->w*ib->h || b->w*b->h > 3*ib->w*ib->h)
					continue;
				int dis = sqrt(double((b->y - ib->y)*(b->y - ib->y)
						+(b->x - ib->x)*(b->x - ib->x)));
				if (dis < 3*ib->h) {
					v.push_back(b);
				}
			}
			center = ib;
			sort(v.begin(), v.end(), comBoxNear);
			near.push_back(v);
		}

		boxaDestroy(&sort_boxa);
		return getAverageSkew(near);*/
		return 0.0;
	}

private:
	static double getAverageSkew(const vector<vector<Box*> >& m) {
		vector<int> vis(m.size());//mark whether the node has been visited or not
		vector<double> skews; //store every line's gradient
		map<double, int> skews_weight; //store how many boxs on that gradient
		map<Box*, int> boxid;//box* to its id
		for (unsigned int i = 0; i < m.size(); i++)
			boxid[m[i][0]] = i;
		double sk = 0.0;

		//find all the line in the page
		for (unsigned int i = 0; i < m.size(); i++) {
			vector<int> x, y;
			if (vis[i])
				continue;

			Box* p = m[i][0];
			x.push_back(p->x);
			y.push_back(p->y);
			while (1) {
				int pid = boxid[p];
				unsigned int j;
				for (j = 1; j < m[pid].size(); j++) {
					if (!vis[boxid[m[pid][j]]]) {
						vis[boxid[m[pid][j]]] = 1;
						x.push_back(m[pid][j]->x);
						y.push_back(m[pid][j]->y);
						p = m[pid][j];
						break;
					}
				}
				if (j == m[pid].size())
					break;
			}

			if (x.size() <= 5)
				continue;
			double a, b;
			LeastSquare(x, y, a, b);
			skews.push_back(a);
			skews_weight[a] = x.size();
			sk += a;
		}
		if (skews.size() == 0)
			return 1e10;
		return getSkew(skews, skews_weight);
	}

	static double getSkew(vector<double> skews, map<double, int> skews_weight) {
		sort(skews.begin(), skews.end());
		int weight_sum = skews_weight[skews[0]];
		int max_weight = 0;
		int max_begin = 0;
		int max_end = 0;
		int begin = 0;
		for (unsigned int i = 1; i < skews.size(); i++) {
			double t = fabs(skews[i-1]/skews[i]);
			if (t > 1.0/1.1 && t < 1.1) {
				weight_sum += skews_weight[skews[i]];
				if (weight_sum > max_weight) {
					max_weight = weight_sum;
					max_end = i+1;
					max_begin = begin;
				}
			} else {
				weight_sum = skews_weight[skews[i]];
				begin = i;
			}
		}
		double sum = 0.0;
		int weight = 0;
		for (int i = max_begin; i < max_end; i++) {
			sum += skews[i]*skews_weight[skews[i]];
			weight += skews_weight[skews[i]];
		}
		if (weight)
			return sum / weight;
		return 1e10;
	}
	static void LeastSquare(const vector<int>& x, const vector<int>& y, double& a, double& b)  {  
		double t1=0, t2=0, t3=0, t4=0;  
		for(unsigned int i=0; i<x.size(); ++i) {  
			t1 += x[i]*x[i];  
			t2 += x[i];  
			t3 += x[i]*y[i];  
			t4 += y[i];  
		}  
		a = 1.0 * (t3*x.size() - t2*t4) / (t1*x.size() - t2*t2);  // 求得β1   
		b = 1.0 * (t1*t4 - t2*t3) / (t1*x.size() - t2*t2);        // 求得β2  
	}  
	static Box* center;
	static bool comBoxNear(Box*a, Box*b) {
		int ax = a->x - center->x;
		int ay = a->y - center->y;
		int bx = b->x - center->x;
		int by = b->y - center->y;
		return ax*ax + ay*ay - bx*bx - by*by < 0;
	}
	static bool comBoxArea(Box* a, Box* b) {
		return a->w*a->h < b->w*b->h;
	}
};

#endif
