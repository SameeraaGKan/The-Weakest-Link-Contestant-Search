//import all teh libraries necessary for the program
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

//Funstions prototypes
unsigned long int arraySize(string ansFile); //get size of the array
double arrayMean(double *totalScoresPtr, unsigned long int count);
double arrayMedian(double *totalScoresPtr, unsigned long int count);
void arrayMode(double *totalScoresPtr, unsigned long int count);
void identifyMissedQuestions(int *missedQuestionsPtr, unsigned long int size, unsigned long int count);


int main() 
{
    //variables to store the file name and the number of questions
    ifstream inFile1, inFile2;
    string contFile, ansFile;

    cout << "Enter name of answer file: ";
    getline(cin, ansFile);
    cout << "Enter the name of the Contestant file: ";
    getline(cin, contFile);

    inFile1.open(contFile);
    inFile2.open(ansFile);

    if (inFile1.fail())
    {
        cout << "Error opening Contestant's info file." << endl;
        return 1;
    }

    if (inFile2.fail())
    {
        cout << "Error opening Answer file." << endl;
        return 1;
    }

    // Getting the size of the array
    unsigned long int size = arraySize(ansFile);

    // Dynamically allocate memory for the arrays

    //pointer to store the answers in from the answers file
    string *corrAnsPtr = new string[size * size ];
    //pointer which dynamically allocates memory to store the subscripts at which the answers are wrong.
    int *wrongAnsPtr = new int[size];
    //pointer which dynamically allocates memory to store the answers that are wrong.
    string *incorrectAnswersPtr = new string[size * size];
    //pointer which dynamically allocates memory to store the answers that are correct.
    string *correctAnswersPtr = new string[size * size];
    //pointer which will point to the array of the contestants answers
    string *contArrayPtr = new string[size * size];
    //pointer which dynamicaly allocates memory for the total scores array
    double *totalScoresPtr = new double[size * size];
    // new pointer to keep track of missed questions
    int *missedQuestionsPtr = new int[size * size]; 

    // Initialize missedQuestionsPtr to zeros
    for (unsigned long int i = 0; i < size; i++)
    {
        missedQuestionsPtr[i] = 0;
    }

    // Read correct answers from file into the array
    for (unsigned long int i = 0; i < size; i++)
    {
        inFile2 >> corrAnsPtr[i];
    }

    // Process contestant data
    string line;
    long unsigned int count = 0;
    while (getline(inFile1, line))
    {
        // Extract contestant ID and answers
        unsigned long int spacePos = line.find(' ');
        string contID = line.substr(0, spacePos);
        string contAnswers = line.substr(spacePos + 1);
         count++;

        // Read contestant answers from string into the array
        unsigned long int index = 0;
        unsigned long int pos = 0;
        while (pos < contAnswers.length())
        {
            spacePos = contAnswers.find(' ', pos);
            if (spacePos == string::npos)
            {
                spacePos = contAnswers.length();
            }
            contArrayPtr[index++] = contAnswers.substr(pos, spacePos - pos);
            pos = spacePos + 1;
        }

        // Compare correct answers with contestant answers
        unsigned long int numWrongAns = 0;
        double score = 0.0;


        for (unsigned long int i = 0; i < size; i++)
        {
            if (corrAnsPtr[i] != contArrayPtr[i])
            {
                //if the correct answer doesnt match the contestant answer
                // then that particular question is wrong, and it is added to the wrongAnsPtr
                wrongAnsPtr[numWrongAns] = static_cast<int>(i) + 1;
                incorrectAnswersPtr[numWrongAns] = contArrayPtr[i];
                correctAnswersPtr[numWrongAns] = corrAnsPtr[i];
                // Increment the count of the missed question
                missedQuestionsPtr[i]++;
                numWrongAns++;
            }

        }

        if (numWrongAns > 0)
        {

            score = static_cast<double>((size - numWrongAns))/ static_cast<double>(size) * 100.0;

            //scoresPtr += score;
            cout << contID << " - " << fixed << showpoint 
                 << setprecision(2) << score << endl;

            // Output wrong answers subsripts 
            for (unsigned long int i = 0; i < numWrongAns; i++)
            {
                cout << wrongAnsPtr[i] << " ";
            }
            cout << endl;

            // Output incorrect answers
            for (unsigned long int i = 0; i < numWrongAns; i++)
            {
                if(wrongAnsPtr[i] >= 10)
                {
                    cout << setw(2) << right << incorrectAnswersPtr[i] << " ";
                }
                else
                {
                    cout << incorrectAnswersPtr[i] << " ";
                }
            }
            cout << endl;

            // Output correct answers
            for (unsigned long int i = 0; i < numWrongAns; i++)
            {
                if(wrongAnsPtr[i] >= 10)
                {
                    cout << setw(2) << right << correctAnswersPtr[i] << " ";
                }
                else
                {
                    cout << correctAnswersPtr[i] << " ";
                }
            }
            cout << endl;

        }
        else if (numWrongAns == 0)
        {
            score = 100;
            cout << contID << " - " << fixed << showpoint << setprecision(2) << score << endl;
        }
        cout << endl; 

        totalScoresPtr[count - 1] = score; // Store the score in the array

        //++indexScores;

    }



    //call the arrayMean function to get the mean
    double mean = arrayMean(totalScoresPtr, count);

    //call the arrayMedian function to get the median
    double median = arrayMedian(totalScoresPtr, count);

    cout << "Mean: " << mean << endl;
    cout << "Median: " << median << endl;

    //call the arrayMode function to get the mode
    arrayMode(totalScoresPtr, count);
    cout << endl;

    //call the identifyMissedQuestions function to get the missed questions
    identifyMissedQuestions(missedQuestionsPtr, size, count);



    // Deallocate memory for the dynamically allocated arrays
    delete[] corrAnsPtr;
    delete[] contArrayPtr;
    delete[] wrongAnsPtr;
    delete[] incorrectAnswersPtr;
    delete[] correctAnswersPtr;
    delete[] totalScoresPtr;
    delete[] missedQuestionsPtr;

    // Close the input files
    inFile1.close();
    inFile2.close();

    return 0;
}

/*
    The arraySize function goes through the answers file to get the 
    size of the array. 
    parameters: it takes the name of the answer file
    return: it returns the size of the array
*/
unsigned long int arraySize(string ansFile)
{
    ifstream infile;
    string corrAns;
    unsigned long int numAns = 0;
    infile.open(ansFile);
    while (infile >> corrAns)
    {
        numAns++;
    }
     //close the file
    infile.close();
    return numAns;
}

/*
    The arrayMean function goes through the totalScores array to get the
    mean of the array. the mean of the array is the sum of the array divided
    by the name of the array.
    parameters: it takes the totalScores array and the size of the array
    return: it returns the mean of the array
*/

double arrayMean(double *totalScoresPtr, unsigned long int count)
{
    double mean = 0.0;
    double totalScore = 0.0;
    //int numCont = 4;
    for(unsigned long int i = 0; i < count; ++i)
    {
        //cout << totalScoresPtr[i] << " ";
        totalScore += totalScoresPtr[i];
    }
    mean = totalScore / static_cast<double>(count);

    return mean;
}

/*
    The arrayMedian function goes through the totalScores array to get the 
    median of the array. The median of the array is the middle value of the
    array, but before that the array needs to be sorted. If the number of 
    elements is even, the median is the average of the two middle elements.
    but if the number of elements is odd, the median is the middle element.
    parameters: it takes the totalScores array and the number of contestants.
    return: it returns the median of the array

*/
double arrayMedian(double *totalScoresPtr, unsigned long int count)
{
    double median = 0.0;
    sort(totalScoresPtr, totalScoresPtr + count);
    if (count % 2 == 0)
    {
        median = (totalScoresPtr[count / 2 - 1] + totalScoresPtr[count / 2]) / 2;
    }
    else if(count % 2 != 0)
    {
        median = totalScoresPtr[count / 2];
    }

    return median;
}

/*
    The arrayMode function goes through the totalScores array to get the
    mode of the array. The mode of the array is the value that occurs most, 
    but if there isnt a mode then it returns all the values that occur, if there
    are multiple modes, it returns all the modes.
    parameters: it takes the totalScores array and the number of contestants.

*/

void arrayMode(double *totalScoresPtr, unsigned long int count)
{
    // Find the maximum score to determine the size of the count vector
    double maxScore = *std::max_element(totalScoresPtr, totalScoresPtr + count);

    // Create a vector to store the count of each score
    std::vector<int> scoreCounts(static_cast<int>(maxScore) + 1, 0);

    // Count occurrences of each score
    for (unsigned long int i = 0; i < count; ++i)
    {
        // Increment the count for the current score
        scoreCounts[static_cast<int>(totalScoresPtr[i])]++;
    }

    // Find the score with the highest count (mode)
    std::vector<double> modes;
    int maxCount = 0;

    for (unsigned long int i = 0; i < scoreCounts.size(); ++i)
    {
        if (scoreCounts[i] > maxCount)
        {
            maxCount = scoreCounts[i];
            modes.clear();
            modes.push_back(static_cast<double>(i));
        }
        else if (scoreCounts[i] == maxCount)
        {
            modes.push_back(static_cast<double>(i));
        }
    }

    // Check if there is a mode
    if (modes.size() == 1)
    {
        cout << fixed << showpoint << setprecision(2) << "Mode: " << modes[0] << endl;
    }
    else
    {
        // Output the modes
        cout << "Mode: ";
        for (size_t i = 0; i < modes.size(); ++i)
        {
            cout << fixed << showpoint << setprecision(2) << modes[i];
            if (i < modes.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

/*
    The identifyMissedQuestions function goes through the missedQuestions array
    to get the missed questions of the array. The missed questions of the array
    is the number of questions that the contestant missed. it calculates the 
    percentage of the questions that the contestant missed, if the percent is greater
    or equal to 60%, then it displays the percent.
    parameters: it takes the missedQuestions array, the size of the array,
    and the number of contestants.
    retuen: it returns the missed questions of the array
*/

void identifyMissedQuestions(int *missedQuestionsPtr, unsigned long int size, unsigned long int count)
{
    // Iterate through missedQuestionsPtr to identify questions missed by more than 60% of the contestants
    cout << "MOST MISSED QUESTIONS" << endl;
    for (unsigned long int i = 0; i < size; ++i)
    {
        double percentage = static_cast<double>(missedQuestionsPtr[i]) / static_cast<double>(count) * 100.0;
        if (percentage >= 60.0)
        {
            cout << i+1 << "\t" << percentage << "%" << endl;
        }
    }
}
