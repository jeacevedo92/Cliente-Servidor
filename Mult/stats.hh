#include <bits/stdc++.h>

using namespace std;

template <typename T> double arithmeticMean(const vector<T> &vi){

	double cont = 0;

	for (const T &t : vi){
		cont += t;
	}

	return (cont/vi.size());

}

template <typename T> double StandardDeviation(const vector<T> &vi){

	double median = arithmeticMean(vi);
	double cont=0;

	for (int i = 0; i < vi.size() ; ++i){
		cont += pow((vi[i]-median),2);
	}

	return sqrt(cont/vi.size());
}
