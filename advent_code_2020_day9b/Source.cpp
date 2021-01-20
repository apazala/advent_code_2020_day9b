#include <cstdio>
#include <cinttypes>
#include <unordered_map>

#define MAX_LINES 1000

int64_t values[MAX_LINES];
int64_t cumsum[MAX_LINES];

using namespace std;

int main()
{
	FILE *fin;

//#define DEBUG
#ifdef DEBUG
	char inputfile[] = "input1.txt";
	int preamble = 5;
#else
	char inputfile[] = "input.txt";
	int preamble = 25;
#endif // DEBUG



	fin = fopen(inputfile, "r");
	if (!fin) {
		fprintf(stderr, "Can't open input file\n");
		return 1;
	}

	int64_t val;
	int nvalues = 0;
	while (fscanf(fin, "%" SCNd64, &val) == 1) {
		values[nvalues] = val;
		nvalues++;
	}
	fclose(fin);

	unordered_map<int64_t, int> previous;
	int i = 0;
	int f;

	unordered_map<int64_t, int>::iterator ret;
	for (f = 0; f < preamble; f++) {
		val = values[f];
		ret = previous.find(val);
		if (ret == previous.end())
			previous[val] = 1;
		else
			ret->second++;
	}

	for (bool found = true; ; f++, i++) {
		val = values[f];

		found = false;
		for (int j = i; !found && j < f; j++) {
			int64_t cand1 = values[j];
			int64_t cand2 = val - cand1;
			if (cand2 < 0 || cand2 == cand1)continue;
			auto ret = previous.find(cand2);
			found = (ret != previous.end());
		}

		if (!found)
			break;

		//add f
		ret = previous.find(val);
		if (ret == previous.end())
			previous[val] = 1;
		else
			ret->second++;

		//delete i
		val = values[i];
		ret = previous.find(val);
		if (ret->second == 1)
			previous.erase(ret);
		else
			ret->second--;

	}
	//end of first step
	int64_t target = values[f];

	//compute cumulative sums
	unordered_map <int64_t, int> sum_index;
	int64_t sum = 0;
	for (int i = 0; i < nvalues; i++) {
		sum += values[i];
		cumsum[i] = sum;
		sum_index[sum] = i;
	}

	int istart=-1, iend;
	//first n-values?
	ret = sum_index.find(target);
	if (ret != sum_index.end()) {
		istart = 0;
		iend = ret->second;
	}
	else {
		for (int i = 0; istart == -1; i++) {
			//cumsum[iend]-cumsum[istart-1] == target; cucumsum[iend] == target + cumsum[istart-1]

			ret = sum_index.find(target+cumsum[i]);
			if (ret != sum_index.end()) {
				istart = i+1;
				iend = ret->second;
			}
		}
	}

	int64_t minval = INT64_MAX;
	int64_t maxval = INT64_MIN;

	for (int i = istart; i <= iend; i++) {
		if (values[i] < minval)
			minval = values[i];
		if (values[i] > maxval)
			maxval = values[i];
	}

	printf("%" PRId64 "\n", minval + maxval);

	return 0;
}