// header files
#include <stdlib.h>
#include <stdio.h>
#include "Console_IO_Utility.h"
#include "File_Output_Utility.h"
#include "File_Input_Utility.h"

// global constants
    // enumerator for column indexes
typedef enum { COLUMN_A, COLUMN_B, COLUMN_C, COLUMN_D, COLUMN_E } TableColumns;
    // enumarator for filing type
typedef enum { MARRIED, SINGLE, NONE } FilingFlag;
    // conversion into percent rate
const double PERCENT_CONVERSION = 0.01;
    // tax table properties
const char TAX_TABLE_R_COUNT = 4;
const char TAX_TABLE_C_COUNT = 5;
    // on demand data files
const char MARRIED_TAX_DATA[] = "married.csv";
const char SINGLE_TAX_DATA[] =  "single.csv";
const int TAX_DATA_TITLE_SIZE = 60;
    // output table columns size
const int OUT_TABLE_COLUMN_SIZE = 15;

// A linked list node
typedef struct Node
    {
     int currentVal;
     struct Node *nextNode;
   } singleLinkNode;


// function prototypes

double calculateTax(double totalIncome,
												 double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT]);
double calculateTotal(singleLinkNode *linkedList);
void displayTaxTable(double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT],
														int partnerStatus);
void displayCorrectTable(double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT]);
int getTaxReturnType();
void printProgramTitle();
void skipOneRow();
singleLinkNode *uploadDataIntoLL(const char *fileName);
void uploadTaxTable( const char *fileName,
												double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT]);

int main( )
{
	//initialize variables, taxTable
	double totalIncome, totalTax;
  double taxArray[ TAX_TABLE_R_COUNT ][ TAX_TABLE_C_COUNT ];
	int partnerStatus = NONE;
  char userFileName[ STD_STR_LEN ];
  singleLinkNode *linkedList;

	//Show title
		//function: printProgramTitle()
	printProgramTitle();

	//Get input: Income
		//function: promptForDouble()
	promptForString( "Provide file containing your income: " , userFileName );

	//Get input: Filing type (married/single)
		//function: getTaxReturnType()
	partnerStatus = getTaxReturnType();

	//abort if wrong input
	if ( partnerStatus != NONE )
		{
			//Upload tax table based on filing status
				//function: uploadTaxTable()
			if ( partnerStatus == SINGLE )
				{
					uploadTaxTable( SINGLE_TAX_DATA, taxArray );
				}

			else
				{
					uploadTaxTable( MARRIED_TAX_DATA, taxArray );
				}

      //upload linked list
        //Function: uploadDataIntoLL()
      linkedList = uploadDataIntoLL( userFileName );

			//Display: tax table used for calculations
				//funftion: displayTaxTable()
			displayTaxTable( taxArray, partnerStatus );

      //calculate total income
        //function: calculateTotal()
      totalIncome = calculateTotal( linkedList );

      //calculate total tax
        //function: calculateTax()
      totalTax = calculateTax( totalIncome, taxArray );

      //display tax + income
			if (totalTax > 0)
				{
				//show total tax
        printf( "Total Income: %0.2f\n", totalIncome );
				printf( "Total tax   : %0.2f\n", totalTax );

				}
			//income was below zero
			else
				{
					printf( "Income you provided is invalid.\n\n" );
				}
		}

		//Wrong filing type
		else
			{
				printf( "\nYou provided wrong filing type.\n" );
				printf( "Program aborted.\n" );
			}

	//end program
	printf( "\nEnd Program\n" );
	return 0;

}
// function prototypes
/*
Name: printProgramTitle
Process: displays program title
Function Input/Parameters: none
Function Output/Parameters: none
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: title displayed as specified
Dependencies: Console IO Utilities tools
*/
void printProgramTitle()
	{
		//print the title
			//function: printString()
		printf( "\nImproved Arizona Tax Calculator\n" );
		printf( "=================================\n\n" );
	}

/*
Name: uploadTaxTable
Process: uploads table from file into 2d array
Function Input/Parameters: accepts name of the file which stores tax table
                           and reference to tax data array in which to store
                           tax data (use constants to set size of array)
Function Output/Parameters: none
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependencies: openInputFile, readDoubleFromFile, skipOneRow, readCharacterFromFile
*/
void uploadTaxTable(const char *fileName,
													double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT])
{
	//initialize variables
	int rows, cols;
	double currentVal;

	//open the file
		//function: openInputFile()
	if ( openInputFile( fileName ) )
		{
			//skip a row
				//funtion: skipOneRow()
			skipOneRow();

			//begin the row loop
			for ( rows = 0; rows < TAX_TABLE_R_COUNT; rows++ )
				{
					//get the first value
						//function: readDoubleFromFile
					currentVal = readDoubleFromFile();

					//put into the first column
					taxArray[rows][COLUMN_A] = currentVal;

				//bein the col loop at col B
				for ( cols = COLUMN_B; cols < TAX_TABLE_C_COUNT; cols++ )
					{
						//skip the commas
							//function: readCharacterFromFile
						readCharacterFromFile( COMMA );

						//get value
							//function: readDoubleFromFile()
						currentVal = readDoubleFromFile();

						//put the value into the array
						taxArray[rows][cols] = currentVal;
					}
				}
			//close file
				//function: closeInputFile()
			closeInputFile( fileName );
		}
		else
			{
					printf( "ERROR: File not found.\n" );
			}
}

/*
Name: skipOneRow
Process: skips one line of input file
Function Input/Parameters: none
Function Output/Parameters: none
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: none
Dependencies: readStringToDelimiterFromFile OR readStringToLineEndFromFile
*/
void skipOneRow()
{
	//create the buffer string
	char buffer[ TAX_DATA_TITLE_SIZE ];

	//read the entire first line
		//function: readStringToLineEndFromFile
	readStringToLineEndFromFile( buffer) ;
}

/*
Name: getTaxReturnType
Process: ask user for input and based on input assigns appropriate filing type
Function Input/Parameters: none
Function Output/Parameters: none
Function Output/Returned: returns filling type either SINGLE or MARRIED
Device Input/Keyboard: character either m/M/s/S
Device Output/Monitor: none
Dependencies: promptForCharacter
*/
int getTaxReturnType()
{
	//initialze variables
	char userInput;
	int partnerStatus;

	//get filing type
		//function: promptForCharacter()
	userInput = promptForCharacter( "Choose filing type(M/S)?: " );

	//set status to married
	if ( (userInput == 'm') || (userInput == 'M') )
		{
			partnerStatus = MARRIED;
		}

	//set status to single
	else if ( (userInput == 's') || (userInput == 'S') )
		{
			partnerStatus = SINGLE;
		}

	//no valid input
	else
		{
			partnerStatus = NONE;
		}

	//return partnerStatus
	return partnerStatus;

}

/*
Name: displayTaxTable
Process: based on filling type prints apropriate title for table
         and calls for table to be displayed
Function Input/Parameters: table containing tax data and filing type
Function Output/Parameters: none
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: title for table
Dependencies: displayCorrectTable, printf
*/
void displayTaxTable(double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT],
														int partnerStatus)
{
	//print an endline for display
		//function: printEndline()
	printEndline();

	//Show filing table status
		//function: printString()
	if ( partnerStatus == MARRIED )
		{
			printString("Table 1. Filing as MARRIED.\n");
		}
	else if ( partnerStatus == SINGLE )
		{
			printString( "Table 1. Filing as SINGLE.\n" );
		}

	//Show the table
		//function: displayCorrectTable()
	displayCorrectTable( taxArray );

	//print an endline for display
		//function: printEndline()
	printEndline();

}


/*
Name: displayCorrectTable
Process: displays tax data table
Function Input/Parameters: table containing tax data
Function Output/Parameters: none
Function Output/Returned: none
Device Input/Keyboard: none
Device Output/Monitor: iteratively prints table as shown in examples
Dependencies: printCharacter, printDoubleJustified, printf
*/
void displayCorrectTable(double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT])
{

	//initialize variables
	int rows, cols;
	double currentVal;

	//begin the row loop
	for ( rows = 0; rows < TAX_TABLE_R_COUNT; rows++ )
		{
			//print a pipe
				//function: printCharacter()
			printCharacter( PIPE );

		//bein the col loop
		for ( cols = COLUMN_A; cols < TAX_TABLE_C_COUNT; cols++ )
			{
				currentVal = taxArray[rows][cols];

				//print justfiied double
					//function: printDoubleJustified()
				printDoubleJustified( currentVal, PRECISION, OUT_TABLE_COLUMN_SIZE,
					 																				"CENTER" );
				//print a pipe
					//function: printCharacter()
				printCharacter( PIPE );

			}
			//print endline to display
				//function: printEndline()
			printEndline();
		}
}

/*
Name: calculateTax
Process: identifies correct tax bracket using tax data from array and income
         calculates tax using tax data from array as well as with
         formula presented below
         TT = ((TI + Col_C) * Col_D %) + Col_E
         , where
         TT - Total Tax
         TI - Total Income
         Col_C - COLUMN_C (same as maximum limit of previous tax bracket)
         Col_D - COLUMN_D (same as rate, must be converted into %)
         Col_E - COLUMN_E (same as max possible tax for all previous brackets)

Function Input/Parameters: tax data aray and total income
Function Output/Parameters: none
Function Output/Returned: total tax
Device Input/Keyboard: none
Device Output/Monitor: none
Dependencies: none
*/
double calculateTax(double totalIncome,
												double taxArray[TAX_TABLE_R_COUNT][TAX_TABLE_C_COUNT])
	{
		//initialzie variables
		double totalTax;
		int row = 0;

		//find largest value
		while( totalIncome > taxArray[row][COLUMN_B])
			{
			row++;
			}

		//Get tax amount
		totalTax = ((totalIncome + taxArray[row][COLUMN_C]) *
	 					(taxArray[row][COLUMN_D] * PERCENT_CONVERSION))
						 + (taxArray[row][COLUMN_E]);

		//return tax
		return totalTax;
	}

/*
Name: calculateTotal
Process: given header to linked lists, traverses list, accsses data (income)
         adds income to the total sum of incomes, returns total income
Function Input/Parameters: pointer to the head of income list
Function Output/Parameters: none
Function Output/Returned: total income
Device Input/Keyboard:  none
Device Output/Monitor: none
Dependencies: none
*/
// TODO
double calculateTotal(singleLinkNode *linkedList)
{
  //initialize variables
  double totalIncome = 0;

  // loop while linked list is not NULL
  while( linkedList != NULL )
  {
   //Add the current val to the total income
   totalIncome += linkedList -> currentVal;

   //grab next value in Linked list
   linkedList = linkedList -> nextNode;
   }

  //return income
  return totalIncome;
}

/*
Name: uploadDataIntoLL
Process: opens file for input, uploads values and assigns them linked list,
         closes files, returns head pointer to the list
Function Input/Parameters: file name and
Function Output/Parameters: none
Function Output/Returned: head pointer to the list
Device Input/Keyboard: none
Device Output/Monitor: none
Dependencies: malloc, sizeof, openInputFile, readIntegerFromFile,
        checkForEndOfInputFile, readCharacterFromFile, closeInputFile
*/
// TODO





singleLinkNode *UploadDataIntoLL( const char filename )
{

  //initialize variables
  singleLinkNode *headPtr = NULL;
  singleLinkNode *traversalPtr = NULL;
  singleLinkNode *tempPtr = NULL;

  if (openInputFile(filename))
  {
      traversalPtr = ( singleLinkNode* )malloc(sizeOf(singleLinkNode));

      //prime the loop
      traversalPtr -> currentVal = readIntegerFromFile();
      traversalPtr -> NextNode = NULL;

      traversalPtr = headPtr;

      //read delimiter
      readCharacterFromFile();

      while( !checkForEndOfInputFile() )
      {

        tempPtr = ( singleLinkNode* )malloc(sizeOf(singleLinkNode));

        //link the data
        traversalPtr -> nextNode = tempPtr;

        //get values
        traversalPtr -> currentVal = readIntegerFromFile();
        traversalPtr -> NextNode = NULL;

        traversalPtr = tempPtr;

        readCharacterFromFile();





      }


}
return headPtr


}









singleLinkNode *uploadDataIntoLL(const char *fileName)
    {
     // initialize variables
     singleLinkNode *traversalPtr = NULL, *headPtr = NULL, *tempPtr = NULL;

     // allocate memory to new node
     traversalPtr = (singleLinkNode*)malloc(sizeof(singleLinkNode));

     //Open the file
     if ( openInputFile(fileName) )
        {

       // prime the loop by reading in data
         // function: readIntegerFromFile
       traversalPtr -> currentVal = readIntegerFromFile();
       traversalPtr -> nextNode = NULL;

       // point head pointer to the same place as traversal pointer
       headPtr = traversalPtr;

       //Skip the comma
        //function: readCharacterFromFile()
       readCharacterFromFile();

       // loop while end of line is not reached
        // function: checkForEndOfInputFile
       while( !checkForEndOfInputFile() )
          {
           // allocate memory to new node (use traversal pointer)
           tempPtr = (singleLinkNode*) malloc(sizeof(singleLinkNode));

           //Grab next node
           traversalPtr -> nextNode = tempPtr;

           //grab the file value
            //function: readIntegerFromFile()
           tempPtr -> currentVal = readIntegerFromFile();
           tempPtr -> nextNode = NULL;

           //Assign tempPtr to traversalPtr
           traversalPtr = tempPtr;

           //Skip the comma
            //function: readCharacterFromFile()
           readCharacterFromFile();
          }

          //Close the input file
            //function: closeInputFile()
          closeInputFile();

         }
       // return head pointer
       return headPtr;
   }
