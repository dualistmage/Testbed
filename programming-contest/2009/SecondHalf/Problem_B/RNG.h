#ifndef RNG_H_
#define RNG_H_

/**
 * @brief A simple random number generator.
 *
 * Use the linear congruential method to generate the recurrent numbers.
 * The formula is: r(n+1) = (a * r(n) + b) mod m; 
 * Where a = 16807, b = 0, and m = 0x7fffffff in this class.
 */
class RNG{
	unsigned int seed;
	unsigned int m;
	unsigned int a;
	
	RNG(unsigned int seed = 5) {
		this->seed = seed;
		m = 0x7fffffff;
		a = 16807;
	}
	
	RNG(const RNG&);
	void operator=(RNG&);
	
	unsigned int getRand(){
		seed = (seed * a) % m;
		
		return seed;
	}
public:
	/**
	 * @brief Get the RNG class instance
	 * 
	 * @return RNG instance reference
	 */
	static RNG& getInstance(){
		static RNG rng;
		
		return rng;
	}
	
	/**
	 * @brief Get a random number with double type in the specified range. The default range is between [0, 1) 
	 * 
	 * The generated random number r is: lowerBound <= r < upperBound. 
	 * 
	 * @param lowerBound The lower range bound
	 * @param upperBound The upper range bound 
	 */
	double getDbRN(double lowerBound=0, double upperBound=1){
		double d = getRand();
		double percent = d / m;
		return percent * (upperBound - lowerBound) + lowerBound;
	}
	
	/**
	 * @brief Get a random number with unsigned int type in the specified range.
	 * 
	 * The generated random number r is: lowerBound <= r < upperBound. 
	 * 
	 * @param lowerBound The lower range bound
	 * @param upperBound The upper range bound
	 */
	unsigned int getUiRN(unsigned int lowerBound, unsigned int upperBound){
		double rst = getDbRN(lowerBound, upperBound);
		return static_cast<unsigned int>(rst);
	}
};

#endif /*RNG_H_*/
