/*
 *
 * Author: Joseph Maldonado
 * ID#: 010880953
 *
 * This file contains the menu interface for Homework 6,
 * as well as other functionality for the Expression Evaluation Program.
 *
 */

#include "Expression.h"

using namespace std;

//Checks if the input ends with a ';'
bool checkInput(const string& input)
{
	//If a semicolon is present at the end of the input, return true, else return false
	if (input.c_str()[input.size()-1] == ';')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Stores the user input in a vector of Expressions, vec_input,
//and stores the Identifiers in a map, IDs
void storeInput(const string& input, vector<Expression>& vec_input, map<string, int>& IDs)
{
	//Make sure vec_input and IDs are empty
	vec_input.clear();
	IDs.clear();
	
	if (checkInput(input))
	{
		//Iterate through input
		for (unsigned int i = 0; i < input.length(); i++)
		{
			//Initialize a temporary string
			string str_temp;
			
			//Iterate through input until a semicolon is found
			for (unsigned int j = i; input[j] != ';'; j++)
			{
				//Add each element from input to str_temp
				str_temp.push_back(input[i]);
				
				//Set i equal to j + 1
				i = j + 1;
			}
			
			//Initialize a temporary Expression with str_temp
			Expression exp_temp(str_temp);
			
			//Add the temporary expression to vec_input
			vec_input.push_back(exp_temp);
			
			//If exp_temp is type Assignment, convert the value to an int, then insert it into IDs
			if (exp_temp.get_type() == Assignment)
			{
				stringstream str(exp_temp.get_tokenized().at(2).get_token());
				int convert = 0;
				str >> convert;
				IDs.insert(pair<string, int>(exp_temp.get_tokenized().at(0).get_token(), convert));
			}
		}
	}
}

//Appends the original user's input, then calls storeInput() again
void appendInput(string& input, vector<Expression>& vec_input, map<string, int>& IDs)
{
	//Initialize a temporary string
	string temp;
	
	//Set temp to the user's input
	cout << "Input:";
	getline(cin, temp);
	
	//Append input with temp
	input.append(temp);
	
	//Store the input within vec_input again
	storeInput(input, vec_input, IDs);
}

//Gets the user's input and erases all spaces from it
void getInput(string& input)
{
	cout << "Input:";
	getline(cin, input);
	
	//Erase spaces from input
	input.erase(remove(input.begin(), input.end(), ' '), input.end());
}

//Gets the user's desired action
void getAction(string& action)
{
	cout << "Action:";
	getline(cin, action);
}

//Displays the menu for the user to interact with and contains functionality for the evaluate,
//prefix, postfix, parenthesize, quit, append input, start over with a new input actions
void menu()
{
	cout << "This is an expression evaluation program, the instructions to use this program are as follows:\n";
	cout << "Input an expressions for processing using this format: '2+2; a=3; 3+a;'.\n\n";
	cout << "The expressions will then be tokenized and seperated by the semi-colon (';').\n\n";
	cout << "Once an expression is inputted, it can be manipulated by inputting '=' to evaluate the expressions,";
	cout << " '>' to output a postfix representation of the expressions, '<' to output a postfix representation";
	cout << " of the expressions, or 'f' to output a fully parenthesized version of the expressions.\n\n";
	cout << "If you would like to append your previous input, 'c' can be inputted. To start over with a new input, 's'";
	cout << " can be inputted. To quit the program, 'q' can be inputted.\n\n";
	
	//Initialize a string to hold user's input
	string input;
	
	//Initialize a string to hold user's input for action
	string action;
	
	//Initialize a vector of Expressions to hold input
	vector<Expression> vec_input;
	
	//Initialize a map of ints to hold the Identifiers
	map<string, int> IDs;
	
	//Bool to while loop
	bool quit = false;
	
	menuBEGIN:
	
	//Get user input
	getInput(input);
	
	//Store the input in vec_input
	storeInput(input, vec_input, IDs);

	do
	{
		//Get the action from user
		getAction(action);
		
		//Evaluate
		if (action == "=")
		{
			if (checkInput(input))
			{
				for (unsigned int i = 0; i < vec_input.size(); i++)
					vec_input.at(i).evaluate(IDs);
			}
			else
			{
				cout << "Missing a ; at the end of input, error.\n";
			}
		}
		
		//Prefix
		else if (action == ">")
		{
			if (checkInput(input))
			{
				for (unsigned int i = 0; i < vec_input.size(); i++)
					vec_input.at(i).display_prefix();
			}
			else
			{
				cout << "Missing a ; at the end of input, error.\n";
			}
		}
		
		//Postfix
		else if (action == "<")
		{
			if (checkInput(input))
			{
				for (unsigned int i = 0; i < vec_input.size(); i++)
					vec_input.at(i).display_postfix();
			}
			else
			{
				cout << "Missing a ; at the end of input, error.\n";
			}
		}
		
		//Parenthesize
		else if (action == "f")
		{
			if (checkInput(input))
			{
				for (unsigned int i = 0; i < vec_input.size(); i++)
					vec_input.at(i).parenthesize();
			}
			else
			{
				cout << "Missing a ; at the end of input, error.\n";
			}
		}
		
		//Quit
		else if (action == "q")
		{
			quit = true;
		}
		
		//Append Input
		else if (action == "c")
		{
			appendInput(input, vec_input, IDs);
		}
		
		//Start over with a new input
		else if (action == "s")
		{
			goto menuBEGIN;
		}
		
		//Wrong input for action
		else
		{
			cout << "Wrong input for the action! "
			        "Please type one of the following: "
			        "=, <, >, f(F), q(Q), c(C), s(S)\n";
		}
	}
	while (!quit);
}

int main()
{
	menu();
	
	return 0;
}