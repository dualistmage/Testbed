#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>

#include <math.h>

#include "TwoOpt.h"

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
    std::vector<unsigned int> changedPath;
    std::list< std::vector<unsigned int> > tabuList;
    std::vector< std::vector<unsigned int> > v;
    bool isPathInTheTabuList;

    unsigned int maxCost;
    unsigned int currentCost;
    unsigned int maxIndex;




    // Get input data and calculate cost
    while(fpin.good())
    {
        fpin >> cityNo >> cityCoordinate.x_ >> cityCoordinate.y_;

        if (!fpin.good())
            break;

        //cout << cityNo << " " << cityCoordinate.x_ << " " << cityCoordinate.y_ << endl;

        bestTravel.path_.push_back( cityNo );
        pathProcessor.cities_[ cityNo ] = cityCoordinate;

    } // end - while

    bestTravel.cost_ = pathProcessor.calculateCost( bestTravel.path_ );

    
    // Start Process
    for(size_t k = 0; k < 100; k++)
    {

        v.clear();

        while( v.size() < 10 )
        {
            TwoOpt<unsigned int>::getNeighbor( bestTravel.path_ , changedPath );
            isPathInTheTabuList = false;

            //DEBUG
            // cout << "Initial ChangedPath : ";
            // for(size_t abc=0; abc < changedPath.size(); abc++)
            //     cout << changedPath[abc] << " ";
            // cout << endl;


            // Find path in the tabuList.
            for(std::list< std::vector<unsigned int> >::iterator iter = tabuList.begin();
                    iter != tabuList.end(); iter++)
            {
                if (*iter == changedPath)
                {
                    // DEBUG
                    // cout << "Find in the tabu : (";
                    // for(size_t i = 0; i < changedPath.size(); i++)
                    //     cout << changedPath[i] << " ";
                    // cout << ", tabu ";
                    // for(size_t i = 0; i < iter->size(); i++)
                    //     cout << (*iter)[i] << " ";
                    // cout << ") - vSize " << v.size() << endl;

                    isPathInTheTabuList = true;
                    break;
                }
            } // end - for

            // If the list is in the tabu list, regenerate the path.
            if ( isPathInTheTabuList )
                break;


            // Insert new path into v and tabu list.
            v.push_back( changedPath );
            tabuList.push_back( changedPath );
            if (tabuList.size() > 10)
                tabuList.pop_front();

            //DEBUG
            // cout << "Inserted into v : ";
            // for(size_t abc=0; abc < changedPath.size(); abc++)
            //     cout << changedPath[abc] << " ";
            // cout << endl;

        } // end - while

        if (v.size() == 0)
            continue;

        // Find maximum cost
        maxCost = pathProcessor.calculateCost( v[0] );
        maxIndex = 0;
        for(size_t i = 1; i < v.size(); i++)
        {
            currentCost = pathProcessor.calculateCost( v[i] );
            if ( currentCost < maxCost )
            {
                maxCost = currentCost;
                maxIndex = i;
            } // end - if
        }

        // cout << endl << "v display - before" << endl;
        // for(size_t i = 0; i < v.size(); i++)
        //   {
        //       cout << "[" << i << "] : ";
        //       for(size_t j = 0; j < v[i].size(); j++)
        //           cout << v[i][j] << " ";
        //       cout << endl;
        //   }
        


            
        if ( maxCost < bestTravel.cost_ )
        {
            bestTravel.cost_ = maxCost;
            bestTravel.path_ = v[ maxIndex ];

            // DEBUG
            // cout << "Best Value is changed : (index = " << maxIndex << ") ";
            // for(size_t i = 0; i < v[ maxIndex ].size(); i++)
            //     cout << v[maxIndex][i] << " ";
            // cout << " - maxCost :" << maxCost << endl;
        }

    } // end - for


    // Output result
    fpout << bestTravel.cost_ << endl;
    for(size_t i = 0; i < bestTravel.path_.size(); i++)
        fpout << bestTravel.path_[i] << " ";



    fpin.close();
    fpout.close();

    return 0;

} // end - main
