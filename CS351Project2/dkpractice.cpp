#include <iostream> 
#include <fstream>
using namespace std; 
  
// Returns the value of maximum profit 
int compareItems(int W, int wt[], int val[], int index, int** dp, string name[]) 
{ 
    // base condition 
    if (index < 0) 
        return 0; 
    if (dp[index][W] != -1) 
        return dp[index][W]; 
  
    if (wt[index] > W) { 
  
        // Store the value of function call 
        // stack in table before return 
        dp[index][W] = compareItems(W, wt, val, index - 1, dp, name); 
        return dp[index][W]; 
    } 
    else { 
        // Store value in a table before return 
        dp[index][W] = max(val[index] 
                           + compareItems(W - wt[index], wt, val, 
                                         index - 1, dp, name), 
                       compareItems(W, wt, val, index - 1, dp, name)); 
  
        // Return value of table after storing 
        return dp[index][W]; 
    } 
} 
  
int knapSack(int W, int wt[], int val[], int n, string name[]) 
{ 
    // double pointer to declare the 
    // table dynamically 
    int** dp; 
    dp = new int*[n]; 
  
    // loop to create the table dynamically 
    for (int i = 0; i < n; i++) 
        dp[i] = new int[W + 1]; 
  
    // loop to initially filled the 
    // table with -1 
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < W + 1; j++) 
            dp[i][j] = -1; 
    return compareItems(W, wt, val, n - 1, dp, name); 
} 
  
// Driver Code 
int main() 
{ 
    string inputFile = " ";
	ifstream filename;
    int W;  
	int n;
	string tempName = " ";
	int tempValue = 0;
	int tempWeight = 0;
	
	cout << "/****0-1 Knapsack Problem (Dyanmic Programming Edition)****/" << endl;
	cout << "What is the name of your txt file? ";
	cin >> inputFile;
	
	filename.open(inputFile);
	filename >> n >> W;
	int profit[n]; 
    int weight[n];
	string names[n];
	
	for(int i=0; i<n; i++){
		filename >> tempName;
		filename >> tempValue;
		filename >> tempWeight;
		
		weight[i] = tempWeight;
		profit[i] = tempValue;
		names[i] = tempName;
		
	}
	
    cout << knapSack(W, weight, profit, n, names) << endl;
	
    return 0; 
}