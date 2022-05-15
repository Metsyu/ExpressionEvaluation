#include "Expression.h"

//Default Constructor
Expression :: Expression()
{
	original.clear();
	tokenized.clear();
	postfix.clear();
	valid = false;
	type = Bad;
}

//Constructor
Expression :: Expression(const string& s)
{
	set(s);
}

//It tokenizes the input string and sets original, valid, type, postfix, prefix accordingly
void Expression :: set(const string& s)
{
	//Set s to original
	original = s;
	
	//Make sure tokenized is empty
	tokenized.clear();
	
	//Initialize variables to keep track of desired tokens
	string track;
	string str;
	
	//Loop through s
	for (unsigned int i = 0; i < s.length(); i++)
	{
		//If the current element of s is empty
		if (s[i] == ' ')
		{
			//If track is not empty, add track to tokenized and set track to empty
			if (!track.empty())
			{
				tokenized.push_back(Token(track));
				track = "";
			}
		}
		
		//If the current element of s is a special token
		else if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '(' || s[i] == ')' || s[i] == '=' || s[i] == '*')
		{
			//If track is not empty, add track to tokenized
			if (!track.empty())
			{
				tokenized.push_back(Token(track));
			}
			
			//Set current element of s to str, then add str to tokenized and set track to empty
			str = s[i];
			tokenized.push_back(Token(str));
			track = "";
		}
		
		//If current element is a number or letter, add the current element of s to track
		else
		{
			track += s[i];
			if(i == s.length()-1)
			{
				tokenized.push_back(Token(track));
			}
		}
	}
	
	//Set valid and type
	set_valid_type();
	
	if (type == Arithmetic)
	{
		//Set the postfix conversion
		set_postfix();
		
		//Set the prefix conversion
		set_prefix();
	}
}

//Converts the infix expression to a postfix expression and sets postfix accordingly
void Expression :: set_postfix()
{
	//Initialize a stack to hold operators
	stack<Token> ops;
	
	//Iterate through all of tokenized
	for (unsigned int i = 0; i < tokenized.size(); i++)
	{
		//If the current token is an Identifier of Integer, add it to postfix
		if (tokenized.at(i).get_type() == Identifier || tokenized.at(i).get_type() == Integer)
			postfix.push_back(tokenized.at(i));
			
		//If the current token is an OpenBrace, add it to ops
		else if (tokenized.at(i).get_type() == OpenBrace)
			ops.push(tokenized.at(i));
			
		//If the current token is a CloseBrace
		else if (tokenized.at(i).get_type() == CloseBrace)
		{
			//While the top of ops is not an OpenBrace, add it to postfix, then remove it from ops
			while (ops.top().get_type() != OpenBrace)
			{
				postfix.push_back(ops.top());
				ops.pop();
			}
			
			//Remove the top of ops
			ops.pop();
		}
			
		//If the current token is an Operator
		else
		{
			//While ops isn't empty and the priority of the top of ops is greater than
			//or equal to the priority of the current token, add the top of ops to postfix,
			//then remove it from ops
			while (!ops.empty() && ops.top().get_priority() >= tokenized.at(i).get_priority())
			{
				postfix.push_back(ops.top());
				ops.pop();
			}
			
			//Add the current token to ops
			ops.push(tokenized.at(i));
		}
	}
	
	//While ops is not empty, add the top of ops to postfix, then remove it from ops
	while (!ops.empty())
	{
		postfix.push_back(ops.top());
		ops.pop();
	}
}

//Converts the infix expression to a prefix expression and sets prefix accordingly
void Expression :: set_prefix()
{
	//Initialize a stack to hold operators
	stack<Token> ops;
	
	//Initialize a deque to temporarily hold the conversion
	deque<Token> temp_prefix;
	
	//Reverse Tokenized
	reverse(tokenized.begin(), tokenized.end());
	
	//Iterate through all of tokenized
	for (unsigned int i = 0; i < tokenized.size(); i++)
	{
		//If the current token is an Identifier of Integer, add it to temp_prefix
		if (tokenized.at(i).get_type() == Identifier || tokenized.at(i).get_type() == Integer)
			temp_prefix.push_front(tokenized.at(i));
			
		//If the current token is an CloseBrace, add it to ops
		else if (tokenized.at(i).get_type() == CloseBrace)
			ops.push(tokenized.at(i));
		
		//If the current token is a OpenBrace
		else if (tokenized.at(i).get_type() == OpenBrace)
		{
			//While the top of ops is not an CloseBrace, add it to temp_prefix, then remove it from ops
			while (ops.top().get_type() != CloseBrace)
			{
				temp_prefix.push_front(ops.top());
				ops.pop();
			}
			
			//Remove the top of ops
			ops.pop();
		}
		
		//If the current token is an Operator
		else
		{
			//While ops isn't empty and the priority of the top of ops is greater than
			//the priority of the current token, add the top of ops to temp_prefix,
			//then remove it from ops
			while (!ops.empty() && ops.top().get_priority() > tokenized.at(i).get_priority())
			{
				temp_prefix.push_front(ops.top());
				ops.pop();
			}
			
			//Add the current token to ops
			ops.push(tokenized.at(i));
		}
	}
	
	//While ops is not empty, add the top of ops to temp_prefix, then remove it from ops
	while (!ops.empty())
	{
		temp_prefix.push_front(ops.top());
		ops.pop();
	}
	
	//While temp_prefix is not empty, add the front of temp_prefix to prefix, then remove it from temp_prefix
	while (!temp_prefix.empty())
	{
		prefix.push_back(temp_prefix.front());
		temp_prefix.pop_front();
	}
	
	//Reverse tokenized (Return it to its original order)
	reverse(tokenized.begin(), tokenized.end());
}

//Reads through the postfix expression and parenthesizes the expression in infix format
void Expression :: parenthesize()
{
	//If Expression is invalid
	if (!valid)
		cout << "Cannot parenthesize " << original
		     << " which is an invalid expression.\n";
	
	//If Expression is Assignment type
	else if (type == Assignment)
		cout << "Cannot parenthesize " << original
		     << ". This is an assignment expression, not an arithmetic expression.\n";
	
	//If Expression is Arithmetic type
	else if (type == Arithmetic)
	{
		//Initialize a temporary string to hold expressions
		string temp_str;
		
		//Initialize a temporary stack to hold operators
		stack<string> operators;
		
		//Initialize a temporary stack to hold operands
		stack<string> operands;
		
		//Iterate through postfix
		for (unsigned int i = 0; i < postfix.size(); i++)
		{
			//If the current token is an Integer, add it to operands
			if (postfix.at(i).get_type() == Integer || postfix.at(i).get_type() == Identifier)
				operands.push(postfix.at(i).get_token());
			//If the current token is an operator
			else
			{
				//Add the token to operators
				operators.push(postfix.at(i).get_token());
				
				//Add the top of operands to the front of temp_str
				temp_str.insert(0, operands.top());
				operands.pop();
				
				//Add the top of operators to the front of temp_str
				temp_str.insert(0, operators.top());
				operators.pop();
				
				//Add the top of operands to the front of temp_str
				temp_str.insert(0, operands.top());
				operands.pop();
				
				//Add braces to the front and back of the string
				temp_str.insert(0, "(");
				temp_str.append(")");
				
				//Push the result to operands and clear temp_str
				operands.push(temp_str);
				temp_str.clear();
			}
		}
		
		cout << "Fully parenthesizing " << original << " results in: " << operands.top() << endl;
	}
}

//Sets the valid and type members accordingly
void Expression :: set_valid_type()
{
	enum States {expect_operand, expect_operator, done};
	
	//Bool to keep track if an = is encountered
	bool eqtrue = false;
	
	//Expecting operand for the first Token
	States state = expect_operand;
	
	//Int to keep track of Braces
	int pcount = 0;
	
	//Set valid to true in the beginning
	valid = true;
	
	//Iterate through tokenized while state doesn't equal done
	for (unsigned int i = 0; i < tokenized.size() && state != done; i++)
	{
		//Switch for states
		switch (state)
		{
			//Case when state is expect_operand
			case expect_operand:
				
				//If current token type is OpenBrace, iterate pcount, no change in state
				if (tokenized.at(i).get_type() == OpenBrace)
					pcount++;
				
				//If current token type is an Integer or Identifier, change state to expect_operator
				else if (tokenized.at(i).get_type() == Integer || tokenized.at(i).get_type() == Identifier)
					state = expect_operator;
				
				//If none of the above, the expression is invalid, set valid to false and state to done
				else
				{
					valid = false;
					state = done;
				}
				break;
				
			//Case when state is expect_operand
			case expect_operator:
				
				//If current token type is CloseBrace, subtract 1 from pcount,
				//no change in state unless pcount < 0
				if (tokenized.at(i).get_type() == CloseBrace)
				{
					pcount--;
					
					//If pcount < 0, the expression is invalid, set valid to false and state to done
					if (pcount < 0)
					{
						valid = false;
						state = done;
					}
				}
				
				//If the current token type is EqualSign,
				//set eqtrue to true and change state to expect_operand
				else if (tokenized.at(i).get_type() == EqualSign)
				{
					eqtrue = true;
					state = expect_operand;
				}
				
				//If the current token type is Operators, change state to expect_operand
				else if (tokenized.at(i).get_type() == Operators)
					state = expect_operand;
					
				//If none of the above, the expression is invalid, set valid to false and state to done
				else
				{
					valid = false;
					state = done;
				}
			
			//Default case (Should never be reached)
			default:
				break;
		}
	}
	
	//If Braces don't match, it is an invalid Expression
	if (pcount != 0)
		valid = false;
	
	//If the Expression doesn't end with an operand, it is an invalid Epression
	if (state == expect_operand)
		valid = false;
	
	//If valid is true, set types
	if (valid)
	{
		//If eqtrue is true
		if (eqtrue)
		{
			//If the size of tokenized is 3, the first Token type is an Identifier,
			//and the last Token type is an Integer, set type to Assignment
			if (tokenized.size() == 3 && tokenized.at(0).get_type() == Identifier
				&& tokenized.at(2).get_type() == Integer)
			{
				type = Assignment;
			}
			
			//Else the Expression is invalid
			else
				valid = false;
		}
		
		//If eqtrue is false, set type to Arithmetic
		else
		{
			type = Arithmetic;
		}
	}
	else
	{
		type = Bad;
	}
}

//Outputs the values of the expression fields, one field per line.
void Expression :: display() const
{
	//Display original string
	cout << "original = " << get_original() << endl;
	
	//Display the contents of the tokenized vector
	cout << "tokenized = ";
	for (unsigned int i = 0; i < tokenized.size(); i++)
	{
		cout << tokenized.at(i).get_token() << "; ";
	}
	
	//Display the number of tokens contained in tokenized
	cout << endl << "number of tokens = " << tokenized.size() << endl;
	
	//Display the contents of the postfix vector
	cout << "postfix = ";
	for (unsigned int i = 0; i < postfix.size(); i++)
	{
		cout << postfix.at(i).get_token() << ' ';
	}
	
	//Display the contents of the prefix vector
	cout << endl << "prefix = ";
	for (unsigned int i = 0; i < prefix.size(); i++)
	{
		cout << prefix.at(i).get_token() << ' ';
	}
	
	//Display valid
	cout << endl << "valid = ";
	if (valid)
		cout << "true\n";
	else
		cout << "false\n";
	
	//Display type
	if (type == 0)
	{
		cout << "type = Assignment\n\n";
	}
	else if (type == 1)
	{
		cout << "type = Arithmetic\n\n";
	}
	else if (type == 2)
	{
		cout << "type = Bad\n\n";
	}
}

//Outputs postfix vector
void Expression :: display_postfix() const
{
	//If Expression is invalid
	if (!valid)
		cout << "No postfix " << original
		     << " which is an invalid expression.\n";
		
	//If Expression is Assignment type
	else if (type == Assignment)
		cout << "No postfix of " << original
		     << ". This is an assignment expression, not an arithmetic expression.\n";
		
	//If Expression is Arithmetic type
	else if (type == Arithmetic)
	{
		//Display the contents of the postfix vector
		cout << "Postfix of " << original << " is: ";
		for (unsigned int i = 0; i < postfix.size(); i++)
		{
			cout << postfix.at(i).get_token() << ' ';
		}
		cout << endl;
	}
}

//Outputs the prefix vector
void Expression :: display_prefix() const
{
	//If Expression is invalid
	if (!valid)
		cout << "No prefix " << original
		     << " which is an invalid expression.\n";
		
	//If Expression is Assignment type
	else if (type == Assignment)
		cout << "No prefix of " << original
		     << ". This is an assignment expression, not an arithmetic expression.\n";
		
	//If Expression is Arithmetic type
	else if (type == Arithmetic)
	{
		//Display the contents of the prefix vector
		cout << "Prefix of " << original << " is: ";
		for (unsigned int i = 0; i < prefix.size(); i++)
		{
			cout << prefix.at(i).get_token() << ' ';
		}
		cout << endl;
	}
}

//Evaluates the expression and outputs the result
void Expression :: evaluate(const map<string, int>& IDs) const
{
	//If Expression is invalid
	if (!valid)
		cout << "Cannot evaluate " << original
		     << " which is an invalid expression.\n";
		
	//If Expression is Assignment type
	else if (type == Assignment)
		cout << "Cannot evaluate " << original
			 << ". This is an assignment expression, not an arithmetic expression.\n";
		
	//If Expression is Arithmetic type
	else if (type == Arithmetic)
	{
		//Initialize a stack of ints to keep track of what is evaluated
		stack<int> eval;
		for (unsigned int i = 0; i < postfix.size(); i++)
		{
			//If the current Token an integer,
			//convert the string to an int, then push it to eval
			if (postfix.at(i).get_type() == Integer)
			{
				stringstream str(postfix.at(i).get_token());
				int convert = 0;
				str >> convert;
				eval.push(convert);
			}
			
			//If the current Token is an Identifier
			else if (postfix.at(i).get_type() == Identifier)
			{
				//If the Identifier is undefined, exit the function
				if (IDs.find(postfix.at(i).get_token()) == IDs.end())
				{
					cout << original << " has no result, some variable is undefined.\n";
					return;
				}
				
				//Else push the mapped value to eval
				else
					eval.push(IDs.at(postfix.at(i).get_token()));
			}
			
			//If the current Token is an operator
			else
			{
				//Set y to the first operand, and remove from stack
				int y = eval.top();
				eval.pop();
				
				//Set x to the second operand, and remove from the stack
				int x = eval.top();
				eval.pop();
				
				//If the operator is an +, add y to x, then push the result to the stack
				if (postfix.at(i).get_token()[0] == '+')
					eval.push(x + y);
				
				//If the operator is an -, subtract y from x, then push the result to the stack
				else if (postfix.at(i).get_token()[0] == '-')
					eval.push(x - y);
					
				//If the operator is an *, multiply x and y, then push the result to the stack
				else if (postfix.at(i).get_token()[0] == '*')
					eval.push(x * y);
					
				//If the operator is an /, divide x and y, then push the result to the stack
				else
				{
					if (y == 0)
					{
						cout << "Sorry, you can't divide by 0.\n";
						return;
					}
					else
						eval.push(x / y);
				}
				
			}
		}
		
		//Print the result
		cout << original << " = " << eval.top() << endl;
	}
}

//Returns the original input
string Expression :: get_original() const
{
	return original;
}

//Returns the tokenized vector
vector<Token> Expression :: get_tokenized() const
{
	return tokenized;
}

//Returns valid
bool Expression :: get_valid() const
{
	return valid;
}

//Returns type
Exp_type Expression :: get_type() const
{
	return type;
}
