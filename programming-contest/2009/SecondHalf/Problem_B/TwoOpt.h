#ifndef TWOOPT_H_
#define TWOOPT_H_

#include <vector>
#include <cstdlib>
#include <iostream>

#include "RNG.h"
/**
 * @brief Implement a two-opt move method to generate TSP neighbor solution.
 *
 */

template<typename T = unsigned int> 
class TwoOpt {
public:
	/**
	 * @brief Get a neighbor solution. 
	 * 
	 * @param origin The solution used to generate its neighbor solution.
	 * @param neighbor The generated neighbor solution.  
	 */
	static void getNeighbor(std::vector<T>& orig, std::vector<T>& neighbor) {
		if (orig.size() == 0) {
			neighbor.clear();
			return;
		}

		if (orig.size() != neighbor.size())
			neighbor.resize(orig.size());

		unsigned int size = orig.size();
		RNG& rng = RNG::getInstance();

		//randomly select two nodes
		unsigned int index1 = rng.getUiRN(0, size);
		unsigned int index2 = rng.getUiRN(0, size);
		int absValue = static_cast<int>(index1-index2);
		while (index1 == index2 || std::abs(absValue) == 1){
			index1 = rng.getUiRN(0, size);
			index2 = rng.getUiRN(0, size);
			absValue = static_cast<int>(index1-index2);
		}
		if (index1 > index2) {
			unsigned int temp = index1;
			index1 = index2;
			index2 = temp;
		}		

		//start two-opt move
		unsigned int index = 0;
		//start count from 0 to index1
		typename std::vector<T>::iterator ite1, ite2;
		ite1 = orig.begin();
		ite2 = ite1 + index1;
		while (ite1 != ite2) {
			neighbor[index] = *ite1;
			index++;
			ite1++;
		}
		neighbor[index] = *ite2;
		index++;

		//start count from index2 to index1+1
		ite1 = orig.begin() + index2;
		ite2 = orig.begin() + index1 + 1;
		while (ite1 != ite2) {
			neighbor[index] = *ite1;
			index++;
			ite1--;
		}
		neighbor[index] = *ite2;
		index++;

		//start count from index2+1 to end
		ite1 = orig.begin() + index2 + 1;
		while (ite1 != orig.end()) {
			neighbor[index] = *ite1;
			ite1++;
			index++;
		}
		
		//for debug
//		std::cout << "The two indexes: " << index1 << ", " << index2 << std::endl;
//		std::cout << "Before swap: " << std::endl;
//		for(unsigned int i=0; i<size; i++)
//			std::cout << orig[i] << " ";
//		std::cout << std::endl << "After swap: " << std::endl;
//		for(unsigned int i=0; i<size; i++)
//			std::cout << neighbor[i] << " ";
//		std::cout << std::endl;
	}
};
#endif /*TWOOPT_H_*/
