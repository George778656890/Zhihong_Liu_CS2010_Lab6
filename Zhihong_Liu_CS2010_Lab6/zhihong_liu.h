#ifndef ZHIHONG_LIU_H
#define ZHIHONG_LIU_H

using namespace std;

using range = pair<int, int>;
using freelist_t = vector<range>;
using alloc = pair<range, int>;
using alloclist_t = vector<alloc>;

const int MIN_LEASE = 40;
const int MAX_LEASE = 70;
const int MIN_SIZE = 50;
const int MAX_SIZE = 350;
const int TIME_LIMIT = 1000;
const int MEMORY_SIZE = 1000;
const int REQUEST_INTERVAL = 10;

int mergingTimes = 0;

int satisfiedRequest = 0;
int unSatisfiedRequest = 0;
int totalRequest = 0;

int smallestBlockSize = 0;
int largestBlockSize = 0;
int averageBlockSize = 0;
vector<int> generatedBlockSize;


int shortestLeaseDuration = 0;
int longestLeaseDuration = 0;
int averageLeaseDuration = 0;
vector<int> generatedLeaseDuration;



void printFree(freelist_t& f) {
	cout << "Free List\n";
	for (auto e : f) {
		auto [start, size] = e;
		cout << "[ " << start << "," << size << " ]";
	}
	cout << endl;
}

void printAlloc(alloclist_t& a) {
	cout << "Alloc List\n";
	for (auto e : a) {
		auto [r, lease] = e;
		auto [start, size] = r;
		cout << "[ ( " << start << "," << size << ")," << lease << " ) ]";
	}
	cout << endl;
}

bool comp1(const range& a, const range& b) {//Compare the start index
	return a.first < b.first;  //Ascending
}

bool comp2(const alloc& a, const alloc& b) {//Compare the lease time
	return a.second < b.second; //Ascending
}

void checkLeaseExpiry(freelist_t& f, alloclist_t& a, int clock) {
	for (int i = 0; i < a.size(); ++i) {
		if (a[i].second == clock) {
			f.push_back(a[i].first);//Give back the memory to freeList
			a.erase(a.begin() + i);//Remove the expired "alloc object" from the "allocList"
		}
	}
}

//Use the random-generated "blockSize" to search in the freeList,return the index
int searchSuitableSizeInFreeList(freelist_t& f, int blockSize, int* NthRangeObject) {

	for (int i = 0; i < f.size(); ++i) {
		if (blockSize <= f[i].second) {
			*NthRangeObject = i;//Mark which Range object in the freeList is selected(function couldn't have two return values)
			return f[i].first;

		}
	}

	return -1;//No suitable size is found
}

//Adjust the "Range object in the freeList" after part of which is allocated to the "allocList"
void adjustRange(range& r, int blockSize) {
	r.first = r.first + blockSize;
	r.second = r.second - blockSize;
}

bool merge(freelist_t& f, int expectedSize) {

	++mergingTimes;//Statistical variable

	for (int i = 0; i < f.size() - 1; ++i) {
		if (((f[i].first + f[i].second) == f[i + 1].first) && ((f[i].second + f[i + 1].second) >= expectedSize)) {//Check adjacency and size
			f[i].second = f[i].second + f[i + 1].second;//merging by changing the size of the current "range object"
			f.erase(f.begin() + (i + 1)); // Remove the next range object which has been merged
			return true;
		}
	}
	return false;
}


#endif
