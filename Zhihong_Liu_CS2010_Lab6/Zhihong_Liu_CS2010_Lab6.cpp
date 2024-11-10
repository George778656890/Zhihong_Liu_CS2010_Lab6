//Memory management simulation
#include<utility>
#include <iostream>
#include<vector>
#include <ctime> 
#include <cstdlib> 
#include<algorithm>
#include <numeric>
#include"zhihong_liu.h"

int main()
{
	std::srand(static_cast<unsigned int>(std::time(0)));//Seed value

	long int clock = 0;

	freelist_t freeList{ range(0,MEMORY_SIZE) };
	alloclist_t allocList;



	do {
		//Generate a memory request
		if (!(clock % REQUEST_INTERVAL)) {

			++totalRequest;//Statistical variable


			int blockSize = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
			int leaseDuration = rand() % (MAX_LEASE - MIN_LEASE) + MIN_LEASE;
			int leaseExpiry = clock + leaseDuration;

			generatedBlockSize.push_back(blockSize);        //Statistical variable
			generatedLeaseDuration.push_back(leaseDuration);//Statistical variable

		RetryAfterMerging:

			int NthRangeObject = -1;//Mark which Range object in the freeList is selected

			int assignedIndex = searchSuitableSizeInFreeList(freeList, blockSize, &NthRangeObject);

			if ((assignedIndex != -1) && (NthRangeObject != -1)) {

				++satisfiedRequest;//Statistical variable

				allocList.push_back(alloc(range(assignedIndex, blockSize), leaseExpiry));//Push the "alloc object" into the "allocList"

				if (freeList[NthRangeObject].second == blockSize) {//Size matches perfectly,remove the corresponding "Range Object" from "freeList"

					freeList.erase(freeList.begin() + NthRangeObject);
				}
				else
				{
					adjustRange(freeList[NthRangeObject], blockSize);  //Adjust the selected "Range Object"
				}

			}
			else {//Merging
				if (merge(freeList, blockSize)) {
					goto RetryAfterMerging;
				}
				else {//Merge fails
					++unSatisfiedRequest;//Statistical variable
				}



			}

		}


		checkLeaseExpiry(freeList, allocList, clock);

		sort(freeList.begin(), freeList.end(), comp1);
		sort(allocList.begin(), allocList.end(), comp2);

	} while (++clock < TIME_LIMIT);


	smallestBlockSize = *std::min_element(generatedBlockSize.begin(), generatedBlockSize.end());
	largestBlockSize = *std::max_element(generatedBlockSize.begin(), generatedBlockSize.end());
	averageBlockSize = (accumulate(generatedBlockSize.begin(), generatedBlockSize.end(), 0)) / generatedBlockSize.size();


	shortestLeaseDuration = *std::min_element(generatedLeaseDuration.begin(), generatedLeaseDuration.end());
	longestLeaseDuration = *std::max_element(generatedLeaseDuration.begin(), generatedLeaseDuration.end());
	averageLeaseDuration = (accumulate(generatedLeaseDuration.begin(), generatedLeaseDuration.end(), 0)) / generatedLeaseDuration.size();

	cout << "The total number of requests is       : " << totalRequest << endl;
	cout << "The number of satisfied requests is   : " << satisfiedRequest << endl;
	cout << "The number of unsatisfied requests is : " << unSatisfiedRequest << "\n\n";

	cout << "The smallest block size is            : " << smallestBlockSize << endl;
	cout << "The largest block size is             : " << largestBlockSize << endl;
	cout << "The average block size is             : " << averageBlockSize << "\n\n";

	cout << "The shortest lease duration is        : " << shortestLeaseDuration << endl;
	cout << "The longest lease duration is         : " << longestLeaseDuration << endl;
	cout << "The average lease duration is         : " << averageLeaseDuration << "\n\n";

	cout << "The number of times of merging is     : " << mergingTimes << "\n\n";


	printFree(freeList);
	cout << "\n" << string(120, '*') << "\n" << endl;
	printAlloc(allocList);


	return 0;
}