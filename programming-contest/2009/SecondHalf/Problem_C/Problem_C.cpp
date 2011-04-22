#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <math.h>

#include "TwoOpt.h"
#include "RNG.h"

using namespace std; 

// ----------------------------------------------------------------------------------

class Coordinates
{
    public:
        double x_;
        double y_;
};

// ----------------------------------------------------------------------------------

class PathProcesser
{
    public:
        map< unsigned int , Coordinates > cities_;
        map< unsigned int , map<unsigned int,unsigned int> > distance_;

    public:

        unsigned int calculateCost(const vector<unsigned int> path)
        {
            unsigned int cost = 0, city1, city2, i, dist;
            double distDouble;

            for( i = 0; i < path.size() -1; i++)
            {
                city1 = path[i];
                city2 = path[i + 1];
                dist = distance_[city1][city2];

                if ( dist != 0 )
                {
                    cost += dist;
                    continue;
                }
                else
                {
                    distDouble
                        = sqrt( ( cities_[city1].x_ - cities_[city2].x_ ) * ( cities_[city1].x_ - cities_[city2].x_ ) 
                                + ( cities_[city1].y_ - cities_[city2].y_ ) * ( cities_[city1].y_ - cities_[city2].y_ ) );


                    dist = distance_[city1][city2] = distance_[city1][city2] = static_cast<unsigned int>( distDouble );
                    cost += dist;
                    //cout << "distance betweeen (" << city1 << " , " << city2 << ") is : " << dist << endl;
                } // else

            } // end - for

            // Add one more path
            city1 = path[0];
            city2 = path[i];
            dist = distance_[city1][city2];

            if ( dist != 0 )
            {
                cost += dist;
            }
            else
            {
                distDouble
                    = sqrt( ( cities_[city1].x_ - cities_[city2].x_ ) * ( cities_[city1].x_ - cities_[city2].x_ ) 
                            + ( cities_[city1].y_ - cities_[city2].y_ ) * ( cities_[city1].y_ - cities_[city2].y_ ) );


                dist = distance_[city1][city2] = distance_[city1][city2] = static_cast<unsigned int>( distDouble );
                cost += dist;

                //cout << "distance betweeen (" << city1 << " , " << city2 << ") is : " << dist << endl;
            } // else

            return cost;

        }

};

// ----------------------------------------------------------------------------------

class Travel
{
    public:
        unsigned int cost_;
        vector<unsigned int> path_;

    public:
        Travel() : cost_(0) {};
        void print()
        {

            cout << "Travel Cost : " << cost_ << endl;
            cout << "Travel Path : ";
            for(size_t i = 0; i < path_.size(); i++)
                cout << path_[i] << " ";
            cout << endl;
        }


};


// ----------------------------------------------------------------------------------

int main(int argc, char** argv)
{

    // ----------------------------------- [ check execution parameters]
    if (argc != 3)
    {
        cout << "USage : Problem_A <input_file> <output_file>\n";
        return -1;
    } // end - if

    // ----------------------------------- [ Open input file ]
    
    ifstream fpin;
    ofstream fpout;

    fpin.open(argv[1]);
    if (!fpin.is_open()) {
        cout << argv[1] << " is not opened" << endl;
        return -1;
    } // end - if

    fpout.open(argv[2]);





    // ----------------------------------- [ Set variables ]
    Coordinates cityCoordinate;
    unsigned int cityNo;
    PathProcesser pathProcessor;
    Travel bestTravel;




    // Get input data and calculate cost
    while(fpin.good())
    {
        fpin >> cityNo >> cityCoordinate.x_ >> cityCoordinate.y_;

        if (!fpin.good())
            break;

        //cout << cityNo << " " << x << " " << y << endl;

        bestTravel.path_.push_back( cityNo );
        pathProcessor.cities_[ cityNo ] = cityCoordinate;

    } // end - while

    bestTravel.cost_ = pathProcessor.calculateCost( bestTravel.path_ );

    double T = 100.0;
    double exponential;
    unsigned int changedCost;
    std::vector<unsigned int> changedPath;

    int count = 0;
    while( T > 1.0 )
    {
        for(unsigned int k = 0; k < 100; k++)
        {
            TwoOpt<unsigned int>::getNeighbor( bestTravel.path_ , changedPath );
            changedCost = pathProcessor.calculateCost( changedPath );

            // Computing probability : exp ( -dE / T )
            exponential = exp( (-bestTravel.cost_ + changedCost) / T );

            // Changing state
            if ( exponential > RNG::getInstance().getDbRN() )
            {
                if (changedCost < bestTravel.cost_ )
                {
                    //cout << "exponential = " << exponential << endl;
                    bestTravel.cost_ = changedCost;
                    bestTravel.path_ = changedPath;
                }
            }

            count++;
        } // end - for

        // Change the temporature
        T = T - 1.0;

    } // end - while()



    //cout << "Total Count : " << count << endl;


    // Output result
    fpout << bestTravel.cost_ << endl;
    for(size_t i = 0; i < bestTravel.path_.size(); i++)
        fpout << bestTravel.path_[i] << " ";



    fpin.close();
    fpout.close();

    return 0;

} // end - main
