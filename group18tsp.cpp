/*
 * Authors: Group 18 (Carlos Carrillo, Stephen Griffin, Marina Kaufman)
 * Description: CS 325 Project 4 - TSP
 * Due Date: December 2, 2016
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <climits>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <list>

using namespace std;


//Create struct variable for each city in graph
struct city {
	int id;
	double xcoord;
	double ycoord;
};


/* Source: stackoverflow.com/questions/589985/vectors-structs-and-stdfind/590010
 * Source was used to help find struct in stl::list based on id*/
struct find_id
{
	int id;
	find_id(int id) : id(id) {}
	bool operator () (const city& c) const
	{
		return c.id == id;
	}
};


//Function prototypes
vector <city> nearestTSP(vector <city> v);
list <city> insertionTSP(vector <city> v);
vector <city> TwoOptTSP(vector <city> v);

int main(int argc, char *argv[])
{
	//Starts timer
	clock_t timer;	
	timer = clock();

	//Add .tour to the name of the output file
	string inFile = argv[1];
	string outFile = inFile + ".tour";


        //Open input file
	std::fstream inputFile;
        inputFile.open(inFile);

	//Count number of lines with data
      	int numLines = 0;
	string line;
        if(inputFile.is_open())
        {
		while(getline(inputFile, line))
		{
			for(int i = 0; line[i] != '\0'; i++)
			{
				//If line is not all blankspace characters count as line
				if(!isspace(line[i]))
				{
					numLines++;
					break;
				}
			}
		}
        }

	//Go back to beginning of file
	inputFile.clear();
	inputFile.seekg(0, ios::beg);

	//Create vector of cities based on number of lines
        vector<city> cities;
        cities.resize(numLines);

	int j = 0;
	int a, b, c;
	//Assign info from each line into appropriate city variables
	while(inputFile >> a >> b >> c)
	{
		cities[j].id = a;
		cities[j].xcoord = b;
		cities[j].ycoord = c;
		j++;
	}

	//Close input file
	inputFile.close();
	
	//Create and open outputFile 
	ofstream outputFile;
	outputFile.open(outFile);

	//Call TSP function and assign its returned value to path
	vector<city> path = nearestTSP(cities);

	//****Did not use these for any actual test cases****//
	//vector<city> path = TwoOptTSP(cities);
	//list<city> tour = insertionTSP(cities);

	//Output total distance first
	//cout << "Total Distance:" << path[path.size() - 1].id << endl;
	outputFile << path[path.size() - 1].id << endl;
	//Then output the path the algorithm took
	for(int i = 0; i < path.size() - 1; i++)
	{
		outputFile << path[i].id << endl;
	}


	//****Used to print nearest insertion results****//
	//Output total distance first
	//cout << "size: " << tour.size() << endl;
	/*cout << "distance: " << tour.back().id << endl;
	//Then output the path the algorithm took
	list<city>::iterator it;
	for(it = tour.begin(); it != tour.end(); ++it)
	{
		cout << it->id << endl;
	}*/

	//Close output file
	outputFile.close();

	//End timer and print number of seconds
	timer = clock() - timer;
	printf("Time: %f seconds \n", ((float)timer)/CLOCKS_PER_SEC);

    	return 0;
}


/*
 * This function accepts a vector of struct city and uses the 
 * nearest-neighbor approximation algorithm to solve the TSP problem
 */
vector <city> nearestTSP(vector <city> v) {

	//initialize an empty vector to hold cities from 1 to n-1
	vector <city> V_cities = {};

	//Fill in vector with all cities excluding the first
	for (int i = 1; i <= v.size() - 1; i++) {
		V_cities.push_back(v[i]);
	}

	//create a vector with the starting city
	vector <city> U_cities = {};
	U_cities.push_back(v[0]);
	
	
	//To keep track of total distance traveled
	int totalDistance = 0;

	//Loop thhrough V_cities until all cities have been visited
	while (V_cities.size() != 0) {
		
		//take the most recently added city to U
		city u = U_cities[U_cities.size() - 1];

		//initialize the closest city arbitrarily as first city in V
		city closest = V_cities[0];

		//initialize the min distance
		int min_distance = static_cast<int>(sqrt( pow((V_cities[0].xcoord - u.xcoord), 2) + pow((V_cities[0].ycoord - u.ycoord), 2)) + 0.5);

		//Loop through all cities still in V and find nearest neighbor 
		for (int j = 1; j <= V_cities.size() - 1; j++)
		    {
				//calculate the closest city in V to city u
				int distance = static_cast<int>(sqrt(pow((V_cities[j].xcoord - u.xcoord), 2) + pow((V_cities[j].ycoord - u.ycoord), 2)) + 0.5);
				if (distance < min_distance) {
					min_distance = distance;
					closest = V_cities[j];
				}
		    }

		//add the closest city to U
		U_cities.push_back(closest);

		//Add new distance to total distance
		totalDistance += min_distance;

		//remove the closest city from V
		for (int k = 0; k < V_cities.size(); k++) {
			if (closest.id == V_cities[k].id) {
				V_cities.erase(V_cities.begin() + k);
			}
		}

	}

	//Add distance of final city back to original city after loop is completed
	int returnDistance = static_cast<int>(sqrt(pow((U_cities[0].xcoord - U_cities[U_cities.size()-1].xcoord), 2) 
	+ pow((U_cities[0].ycoord - U_cities[U_cities.size()-1].ycoord), 2))+0.5) ;
	
	totalDistance += returnDistance;	

	//Put total distance as last element in vector, with arbitrary x- and y-coords of 0
	U_cities.push_back({totalDistance, 0, 0});	
	return U_cities;
}


/********************************
 *
 * NOTE: Did not actually use the following two functions
 * to get any results. Just wanted to show we gave an effort
 * to try other algorithms and
 * improve on the nearest neighbor results. 
 * 
 * ******************************/


/*
 * This function implements a 2-OPT algorithm for the TSP problem
 */
vector <city> TwoOptTSP(vector <city> v) {

    int temp = 0;

    //initialize an empty vector to hold cities from 1 to n-1
    std::vector <city> V_cities = {};

    //fill in vector
    for (int i = 1; i <= v.size() - 1; i++) {
        V_cities.push_back(v[i]);
    }

    //create a vector with the starting city
    //vector <city> U_cities = {v.push_back[0]};
    std::vector <city> U_cities = {};
    U_cities.push_back(v[0]);

    //cout << "U: " << U_cities[0].id << endl;
    int totalDistance = 0;

    // Get tour size
    int size = V_cities.size();

    double new_distance = 0;

    // repeat until no improvement is made
    int improve = 0;


    while ( improve < 20 ) {

        //cout << "vsize: " << V_cities.size() << endl;

        //take the most recently added city to U
        city u = U_cities[U_cities.size() - 1];
        //cout << "city u: " << u.id << " " << u.xcoord << " " << u.ycoord << endl;

        ///initialize the closest city
        city closest = V_cities[0];

        //initialize the min distance
        int best_distance =sqrt( pow((V_cities[0].xcoord - u.xcoord), 2) + pow((V_cities[0].ycoord - u.ycoord), 2) );

        for (int i = 0; i < size - 1; i++ )
        {
            for (int j = i + 1; j < size; j++)
            {
                // Swap indixes in order to eliminate crossed paths
                temp = i;
                i = j;
                j = temp;

                // Get new distance
                new_distance = sqrt(pow((V_cities[i].xcoord - u.xcoord), 2) + pow((V_cities[i].ycoord - u.ycoord), 2));

                if ( new_distance < best_distance )
                {
                    // Improvement found so reset
                    improve = 0;
                    best_distance = new_distance;
                    closest = V_cities[j];
                }
            }
        }

        improve ++;

        //add the closest city to U
        U_cities.push_back(closest);
        //cout << "closest: " << closest.id << endl;

        //Get distance from closest to u and add to totalDistance
        new_distance = sqrt(pow((closest.xcoord - u.xcoord), 2) + pow((closest.ycoord - u.ycoord), 2));

        totalDistance += new_distance;

        //remove the closest city from V
        for (int k = 0; k < V_cities.size(); k++) {
            if (closest.id == V_cities[k].id) {
                V_cities.erase(V_cities.begin() + k);
            }
        }


    }



    //Put total distance as last element in vector, with arbitrary x- and y-coords of 0
    U_cities.push_back({totalDistance, 0, 0});


    return U_cities;
}

/*
 * This algorithm implements a nearest insertion algorithm for the TSP problem
 */
list <city> insertionTSP(vector <city> v) {
	
	//initialize an empty vector to hold cities from 1 to n-1
	vector <city> V_cities = {};

	//Fill in vector with all cities excluding the first
	for (int i = 1; i <= v.size() - 1; i++) {
		V_cities.push_back(v[i]);
	}

	//create a vector with the starting city
	list <city> U_cities = {};
	U_cities.push_back(v[0]);
	U_cities.push_back(v[0]);
	list<city>::iterator pos;
	pos = U_cities.begin();
	pos++;
	
	//To keep track of total distance traveled
	int totalDistance = 0;
	
	//take the most recently added city to U
	city u = U_cities.front();

	//initialize the closest city arbitrarily as first city in V
	city closest = V_cities[0];

	//initialize the min distance
	int min_distance = sqrt( pow((V_cities[0].xcoord - u.xcoord), 2) + pow((V_cities[0].ycoord - u.ycoord), 2) );
		
	//Loop through all cities still in V and find nearest neighbor 
	for (int j = 1; j <= V_cities.size() - 1; j++)
    	{
		//calculate the closest city in V to city u
		int distance = sqrt(pow((V_cities[j].xcoord - u.xcoord), 2) + pow((V_cities[j].ycoord - u.ycoord), 2));
		if (distance < min_distance) {
			min_distance = distance;
			closest = V_cities[j];
		}
	}

	//add the closest city to U
	//U_cities.push_back(closest);
	U_cities.insert(pos, closest);

	//Get distance from u to closest and back to u and add to totalDistance
	int newDistance = (sqrt(pow((closest.xcoord - u.xcoord), 2) + pow((closest.ycoord - u.ycoord), 2))) * 2; 
	totalDistance += newDistance;

	//remove the closest city from V
	for (int k = 0; k < V_cities.size(); k++) {
		if (closest.id == V_cities[k].id) {
			V_cities.erase(V_cities.begin() + k);
		}
	}

	/*
	for(pos = U_cities.begin(); pos != U_cities.end(); ++pos)
	{
		cout << pos->id << endl;
	}*/
	

	//Loop thhrough V_cities until all cities have been visited
	while (V_cities.size() != 0) {
		
		//take the most recently added city to U
		city u = U_cities.back();

		//initialize the closest city arbitrarily as first city in V
		city closestV = V_cities[0];
		city closestU = U_cities.front();

		//initialize the min distance
		int min_distance = sqrt( pow((closestV.xcoord - closestU.xcoord), 2) + pow((closestV.ycoord - closestU.ycoord), 2) );
		
		//Loop through all cities still in V and find nearest neighbor 
		for (int j = 0; j < V_cities.size(); j++)
		{
			for(pos = U_cities.begin(); pos != U_cities.end(); ++pos)
			{
				//calculate the closest city in V to city u
				int distance = sqrt(pow((V_cities[j].xcoord - pos->xcoord), 2) + pow((V_cities[j].ycoord - pos->ycoord), 2));
				if (distance < min_distance) {
					min_distance = distance;
					closestV = V_cities[j];
					closestU = *pos;
				}
			}
		}

		//Find id of closest city with iterator
		pos = find_if(U_cities.begin(), U_cities.end(), find_id(closestU.id));
		pos++;
		//Save city that was previously next
		city nextU = *pos;
		//Insert new city in between
	        U_cities.insert(pos, closestV);

		//Get new distance accounted for changes due to insertion
		int newDistance = (sqrt(pow((closestV.xcoord - closestU.xcoord), 2) + pow((closestV.ycoord - closestU.ycoord), 2)))
				 +(sqrt(pow((closestV.xcoord - nextU.xcoord), 2) + pow((closestV.ycoord - nextU.ycoord), 2)))
				 -(sqrt(pow((closestU.xcoord - nextU.xcoord), 2) + pow((closestU.ycoord - nextU.ycoord), 2))) ;
		totalDistance += newDistance;
		//cout << "new: " << newDistance << endl;
		//cout << "total: " << totalDistance << endl;

		//remove the closest city from V
		for (int k = 0; k < V_cities.size(); k++) {
			if (closestV.id == V_cities[k].id) {
				V_cities.erase(V_cities.begin() + k);
			}
		}

	}

	//Put total distance as last element in vector, with arbitrary x- and y-coords of 0
	U_cities.push_back({totalDistance, 0, 0});	
	
	return U_cities;
}
