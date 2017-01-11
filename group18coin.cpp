/*
 * Authors: Group 18 (Carlos Carrillo, Stephen Griffin, Marina Kaufman)
 * Description: CS 325 Project 2 - Coin Change
 */

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <climits>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>

using namespace std;

//Function prototypes
int changeSlow(vector <int> a, vector <int>& b, int);
vector<int> changegreedy(vector<int> a, int value, int size);
std::vector<int> changedp(std::vector<int> coins, int amount);


int main(int argc, char *argv[])
{
	//Parse .txt off of command line agrument
	string filename;
	for(int f = 0; f < strlen(argv[1]) - 4; f++)
	{
		filename += argv[1][f];
	}

	//Add .txt to inputFile and change.txt to end of outputFile
	string inFile = filename + ".txt";
	string outFile = filename + "change.txt";


        //Open input file and count number of lines with data
      	int numLines = 0;
	std::fstream inputFile;
        inputFile.open(inFile);
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

	//Put text from lines into array of strings
        //std::string myArray[numLines];
	string *myArray = new string[numLines];
	for(int i = 0; i < numLines; i++)
	{
		getline(inputFile, myArray[i]);
	}


	inputFile.close();


        //Create vector of vectors to hold the input from file
        std::vector<std::vector<int> > numbers;
        numbers.resize(numLines/2);
        std::string skip, keep;


	int sub = 0;
        //Loop through every other line (coin denominations)
        //of .txt file and skip any '[', ']', or ','
        for(int m = 0; m < numLines; m+=2)
        {
                std::istringstream iss (myArray[m]);
                while(std::getline(std::getline(iss, skip, '['), keep, ']'))
                {
                        std::istringstream iss2(keep);
                        while(std::getline(iss2, keep, ','))
                                //Convert string to int and add to vector
                                numbers[m-sub].push_back(stoi(keep));
				sub++;
	         }
        }

	//Loop through every other line (values)
	//And convert strings to ints
	int values[numLines/2];
	sub = 1;
	for(int k = 1; k < numLines; k+=2)
	{
		values[k-sub] = stoi(myArray[k]);
		sub++;
	}

	//Create outputFile with appropriate []change.txt name
	ofstream outputFile;
	outputFile.open(outFile);

	//Loop through each algorithm and write results to outputFile

	//changeSlow
	vector<int> slowresults;
	outputFile << "changeSlow algorithm:" << endl;
        for(int j = 0; j < numbers.size(); j++)
        {
		outputFile << "[";
		//Vector to pass in and store denominations
		vector<int> slowresults(values[j]+1, INT_MAX);
		int minCoins = changeSlow(numbers[j], slowresults, values[j]);
		for(int i = 0; i < numbers[j].size(); i++)
		{
			int count = 0;
			for(int k = slowresults.size() - numbers[j].size(); k < slowresults.size(); k++)
			{
				if(numbers[j][i] == slowresults[k])
				count++;
			}
		if(i == numbers[j].size() - 1)
			outputFile << count;
		else 
			outputFile << count << ", ";
	}
	outputFile << "]" << endl;	
	outputFile << minCoins << endl;
        }
	outputFile << endl;

	//changeGreedy
	outputFile << "changeGreedy algorithm:" << endl;
        for(int j = 0; j < numbers.size(); j++)
        {
		outputFile << "[";
		vector<int> greedyresults = changegreedy(numbers[j], values[j], numbers[j].size());
		//Number of individual coin denominations
		for(int i = 0; i < greedyresults.size()-1; i++)
		{
			if(i < greedyresults.size()-2)
				outputFile << greedyresults[i] << ", ";
			else
				outputFile << greedyresults[i] << "]";
		}
		outputFile << endl;
		//Minimum number of total coins
		outputFile << greedyresults[greedyresults.size()-1];
		outputFile << endl;
	}
	outputFile << endl;

	//changedp
 	outputFile << "changedp algorithm:" << endl;
        for(int j = 0; j < numbers.size(); j++)
        {
		outputFile << "[";
		vector<int> dpresults = changedp(numbers[j], values[j]);
		//Number of individual coin denominations
		for(int i = 0; i < dpresults.size()-1; i++)
		{
			if(i < dpresults.size()-2)
				outputFile << dpresults[i] << ", ";
			else
				outputFile << dpresults[i] << "]";
		}
		outputFile << endl;
		//Minimum number of total coins
		outputFile << dpresults[dpresults.size()-1];
		outputFile << endl;
        }
	outputFile << endl;

	delete [] myArray;

    	return 0;
}



/*
 * changeSlow solves the coin change problem using
 * a divide and conquer strategy
 */
int changeSlow(vector<int>a, vector<int> &b,  int amount) {
	//base case

	if (amount <= 0)
	{
		return 0;
	}

	//current number
	int coins = (int)INT_MAX;

	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] <= amount)
		{
			//keep current min or add a coin to the total if less than current
			coins = min(coins, changeSlow(a, b, amount - a[i]) + 1);
			}
	}

	//to keep track of used denominations for change
	vector<int> temp(amount + 1, INT_MAX);
	vector<int> solution(amount + 1);

	temp[0] = 0;
	for (int i = 1; i < amount + 1; i++)
	{
		for (int j = 0; j<a.size(); j++)
		{
			if (i >= a[j] && 1 + temp[i - a[j]] < temp[i])
			{
				temp[i] = 1 + temp[i - a[j]];
				solution[i] = j;
			}
		}


		//loop thru the result vector of denominations and push back to return from function
		int j = amount;
		while (j)
		{
			//cout << "  " << a[solution[j]];
			b.push_back(a[solution[j]]);
			j = j - a[solution[j]];
		}

	}

	return coins;
}


/*
 * changegreedy solves coin changes problem by always
 * taking the highest coin denomination possible
 * until value is 0
 */
vector<int> changegreedy(vector<int> a, int value, int size)
{
	int minCoins = 0;
	//Vector to keep track of number of each coing denomination
	//plus total number of coins
    	vector<int> eachCoin(size+1, 0);

	//Start at coin with highest value and decrement down
   	for(int i = size - 1; i >= 0; i--)
    	{
		//If current value is less than total remaining value
       		while(a[i] <= value)
        	{
            		minCoins++;		//Increase total number of coins
            		eachCoin[i]++;		//Increase that coin denomination
           		value = value - a[i];	//Decrease total value
        	}
   	 }

    	//Last element in vector is total number of minimum coins
	eachCoin[size] = minCoins;

    	return eachCoin;
}



/*
 * changedp solves the coin change problem
 * using dynamic programming
 */
std::vector<int> changedp(std::vector<int> coins, int amount){

    // Function Variables
    std::vector<int> table (amount+1, INT_MAX);  // Main table
    std::vector<int> tempVector (amount+1, 0);   // Storage vector
    std::vector<int> finalOutput;
    std::vector<int> tempVector2;
    std::vector<int> output;
    int tempValue = amount;
    int count = 0;  // Coins counter
    int subCount;
    int temp = 0;

    // In case of amount == 0
    table[0] = 0;
    tempVector[0] = 0;

    // Make table of all results
    // Walk through positive coin values
    for(int index = 1; index <= amount; index++){
        int subVal = amount;
        //iterate through all possible coin options
        for(int coin = coins.size()-1; coin >= 0; coin--){
            //Check if the coin is an option?
            if(coins[coin] <= index){
                subCount = table[index-coins[coin]];
                //Find minimum value
                if(subCount != INT_MAX && subCount+1 < table[index]){
                    table[index] = subCount+1;
                    //Store coin options here to keep track
                    tempVector[index] = coin;
                    if(subVal > coins[coin]){
                        subVal -= (subVal/coins[coin])*coins[coin];
                    }
                }
            }
        }
    }
    // Go back through the stored coin, match with original
    // array, and increase count based off of: Dynamic
    // Programming vs. Greedy Algorithms:
    // https://alaning.me/index.php/Dynamic_Programming_vs_Greedy_Algorithms

    // Only copy values greater than 0 to output vector.
    while(tempValue > 0){
        // Store coin value at the previous coin stored
        temp = coins[tempVector[tempValue]];
        output.push_back(temp);
        tempValue = tempValue - coins[tempVector[tempValue]];
    }

    // Set size for the finalOutput vector
    finalOutput.resize(coins.size());

    // Create the array/vector that will show the change result.
    int valueCount = 0;
    for (int i=0; i<coins.size(); i++){
        valueCount = 0;
        for (int j=0; j<output.size(); j++){
            if (coins[i] == output[j]){
                valueCount++;
                finalOutput[i] = valueCount;
            }
        }
    }
    // Attach/encrypt the # of coins needed to give change
    finalOutput.push_back(table[amount]);

    // Return array of coins used plus # of coins needed to give change.
    return finalOutput;

}


