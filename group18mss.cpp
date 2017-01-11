/*
 * Authors: Group 18 (Carlos Carillo, Stephen Griffin, Marina Kaufman)
 * Description: CS 325 Project 1
 * Date: October 16, 2016
 */


#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

//Function prototypes
int *enumMaxSubarray(vector<int> array, int size, int n);
int *betterEnumMaxSubarray(vector<int> array, int size, int n);
int divAndCon(vector<int> arr, int start, int end);
int dynamicMSS (std::vector<int> array, int size);


int main()
{
	//Redirect stdout to file
        freopen("MSS_Results.txt", "w", stdout);

        //Open input file and put each line into string element in array
        std::fstream myFile;
        myFile.open("MSS_Problems.txt");
        std::string myArray[7];

        if(myFile.is_open())
        {
                for(int i = 0; i < 7; ++i)
                {
                        getline(myFile, myArray[i]) ;
                }
        }

        myFile.close();


        //Create vector of vectors to hold the seven provided arrays
        std::vector<std::vector<int> > numbers;
        numbers.resize(7);
        std::string skip, keep;


        //Loop through .txt file and skip any '[', ']', or ','
        for(int m = 0; m < 7; m++)
        {
                std::istringstream iss (myArray[m]);
                while(std::getline(std::getline(iss, skip, '['), keep, ']'))
                {
                        std::istringstream iss2(keep);
                        while(std::getline(iss2, keep, ','))
                                //Convert string to int and add to vector
                                numbers[m].push_back(std::stoi(keep));
                }
        }


	//Loop through vectors to display the intial array
	//And the results of each of the four algorithms for that array
        for(int j = 0; j < numbers.size(); j++)
        {
		cout << "***************************************" << endl;
		cout << endl;
		cout << "Input Array: [";
		for(int k = 0; k < numbers[j].size(); k++)
		{
			if(k < numbers[j].size() -1)
				cout << numbers[j][k] << ", ";
			else
				cout << numbers[j][k] << "]" << endl;
		} 
		cout << endl;		
 
        	enumMaxSubarray(numbers[j], numbers[j].size(), numbers[j].size() + 1);
        	betterEnumMaxSubarray(numbers[j], numbers[j].size(), numbers[j].size() + 1);
       		int dcMaxSum = divAndCon(numbers[j], 0, numbers[j].size());
        	cout << "Divide and Conquer Max Sum: " << dcMaxSum << endl;
		cout << endl;
                dynamicMSS(numbers[j], numbers[j].size());
		cout << endl;
        }


    	return 0;
}



/*
 * This function prints the max subarray and max sum of an array
 * using brute force enumeration
 */
int *enumMaxSubarray(vector<int> array, int size, int n){

    int sum = 0;
    int best = 0;
    int arraySize = 0;
    int start, end = 0;
    int currentArray [n];
    int *maxArray = new int[n];

    //Prevent output array store garbage if n > arraySize
    if (n > size-1) {
        for (int i=0; i<n; i++)
            currentArray [i] = 0;

        for (int i=0; i<size; i++)
            currentArray [i] = array[i]; }
    else {
        for (int i=0; i<n; i++)
            currentArray [i] = array[i]; }


    // Iterate through array
    for (int i = 0; i < n; i++) {           //First term in subarray.

        //Walk through array from i
        for (int j = i; j < n; j++) {       //Last term in subarray.

            //Reset the latest sum
            sum = 0;

            //Calculate new sum.
            for(int k = i; k <= j; k++)     //Current term in subarray
                sum += currentArray[k];

            //Store if there is a new max sum.
            if (sum > best) {
                best = sum;     //Highest sum so far
                start = i;      //Lowest bound index in MaxSubArray
                end = j;        //Highest bound index in MaxSubArray
            }
        }
    }

    int index = 0;
    cout << "Enumeartion Max subarray: [";
    for(int i=start; i<=end; i++) {
        maxArray[index] = currentArray[i];
        if(i < end)
           cout << maxArray[index] << ", ";
        else
           cout << maxArray[index] << "]" << endl;
        index++;
    }
    maxArray[index] = best;
    cout <<"Enumeration Max Sum: "<<best<<endl<<endl;
    //return best;
    return maxArray;


}

/*
 * This function prints the max subarray and max sum of an array
 * using enumeration, but improves the former by saving results
 * of subarrays.
 */
int *betterEnumMaxSubarray(vector<int> array, int size, int n){

    int sum = 0;
    int best = 0;
    int arraySize = 0;
    int start, end = 0;
    int currentArray [n];
    int *maxArray = new int[n];

    //Prevent output array store garbage if n > arraySize
    if (n > size-1) {
        for (int i=0; i<n; i++)
            currentArray [i] = 0;

        for (int i=0; i<size; i++)
            currentArray [i] = array[i]; }
    else {
        for (int i=0; i<n; i++)
            currentArray [i] = array[i]; }

    // Iterate through array
    for (int i = 0; i < n; i++){    //First term in subarray.

        //Reset the latest sum
        sum = 0;

        for (int j = i; j < n; j++){ //Last term in subarray.

            //Calculate new sum.
            sum += currentArray[j];

            //Store if there is a new max sum.
            if (sum > best) {
                best = sum;     //Highest sum so far
                start = i;      //Lowest bound index in MaxSubArray
                end = j;        //Highest bound index in MaxSubArray
            }
        }
    }

    int index = 0;
    cout << "Better Enumeartion Max subarray: [";
    for(int i=start; i<=end; i++) {
        maxArray[index] = currentArray[i];
        if(i < end)
           cout << maxArray[index] << ", ";
        else
           cout << maxArray[index] << "]" << endl;
        index++;
    }
    maxArray[index] = best;
    cout <<"Better Enumeration Max Sum: "<<best<<endl<<endl;
    //return best;
    return maxArray;



}


/*
 * This function prints the max subarray and sum of an array
 * using the divide and conquer method
 */
int divAndCon(vector<int> arr, int start, int end) {


	//counters
	int i;
	int sum = 0;

	//max values for left and right subarrays
	int maxLeft = 0;
	int maxRight = 0;
	int maxRecurse;



	//max values for suffix in left and prefix in right subarrays and their max sum value
	int combined = 0;
	int prefix = 0;
	int suffix = 0;

	//base case, the lengh of the array  ==1
	if (start == end) {
		return arr[end];
	}

	//find the midpoint
	int middle = (start + end) / 2;

	//divide subarray on the left recursively
	maxLeft = divAndCon(arr, start, middle);


	//divide subarray on the right recursively
	maxRight = divAndCon(arr, middle + 1, end);


	maxRecurse = max(maxLeft, maxRight);


	//find the max sum subarray suffix of left subarray
	for (i = middle - 1; i >= 0; i--)
	{
		sum += arr[i];
		if (sum > suffix) {
			suffix = sum;
		}
	}

	//find the max sum subarray prefix of right subarray
	sum = 0; // set counter to zero
	for (i = middle; i < end; i++) {
		sum += arr[i];
		if (sum > prefix) {
			prefix = sum;

		}
	}


	//find the max sum subarray of sufix of left array and prefix of right array
	combined = suffix + prefix;

	//Compare the combined max sum to the sums of left and right subarrays

	return max(combined, maxRecurse);

}

/*
 * This function prints the max subarray and max sum of an array
 * using dynamic (or bottom-up) programming.
 */
int dynamicMSS (std::vector<int> array, int size)
{
	//Set both currentMax and maxSoFar to first element
        int maxSoFar = array[0];
        int currentMax = array[0];

        int start = 0;
        int end = 0;
        int newStart = 0;

	//Loop through array starting at element 1
        for(int i = 1; i < size; i++)
        {
		//Add next element in array to saved currentMax
                int next = currentMax + array[i];

		//If it is positive
                if(next > 0)
                {
			//If currentMax was zero change start index, because
			//new subarray is starting
                        if(currentMax == 0)
                                newStart = i;
			//Set new currentMax
                        currentMax = next;
                }
		//Else set currentMax to zero, so program knows no new subarray was started
                else
                        currentMax = 0;

		//If current is greater than maxSoFar
		//Update maxSoFar and start and end indices
                if(currentMax > maxSoFar)
                {
                        maxSoFar = currentMax;
                        end = i;
                        start = newStart;
                }
        }

	//Print subarray and max sum
        std::cout << "Dynamic Max subarray: [";
        for(int j = start; j < end + 1; j++)
        {
                if(j < end)
                        std::cout << array[j] << ", ";
                else
                        std::cout << array[j] << "]" << std::endl;
        }

        std::cout << "Dynamic Max sum: " << maxSoFar << std::endl;


        return maxSoFar;
}
