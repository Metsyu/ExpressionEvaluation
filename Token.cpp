#include "Token.h"

Token :: Token()
{
	type = INVALID;
	token = "";
	priority = -1;
}

Token :: Token(string s)
{
	set(s);
}

void Token :: set(string s)
{
	//Check for type Identifier
	if (isalpha(s[0]))
	{
		if (s.length() == 1)
		{
			type = Identifier;
			goto ELSE;
		}
		else
		{
			for (unsigned int i = 1; i < s.length(); i++)
			{
				if (isalnum(s[i]))
				{
					type = Identifier;
				}
				else
				{
					type = INVALID;
					goto ELSE;
				}
			}
			goto ELSE;
		}
	}
	
	//Check for type Integer
	if (isdigit(s[0]))
	{
		if (s.length() == 1)
		{
			type = Integer;
			goto ELSE;
		}
		else if (s.length() > 1 && s[0] == '0')
		{
			type = INVALID;
			goto ELSE;
		}
		else if (s.length() > 1 && s[0] != '0')
		{
			for (unsigned int i = 1; i < s.length(); i++)
			{
				if (isdigit(s[i]))
				{
					type = Integer;
				}
				else
				{
					type = INVALID;
					goto ELSE;
				}
			}
			goto ELSE;
		}
	}
	
	//Check for type Operators
	if (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
	{
		if (s.length() > 1)
		{
			type = INVALID;
			goto ELSE;
		}
		else if (s[0] == '+' || s[0] == '-')
		{
			type = Operators;
			goto POM;
		}
		else if (s[0] == '*' || s[0] == '/')
		{
			type = Operators;
			goto MOD;
		}
	}
	
	//Check for type EqualSign
	if (s[0] == '=')
	{
		if (s.length() > 1)
		{
			type = INVALID;
			goto ELSE;
		}
		else
		{
			type = EqualSign;
			goto ELSE;
		}
	}
	
	//Check for type OpenBrace
	if (s[0] == '(')
	{
		type = OpenBrace;
		goto OB;
	}
	
	//Check for type CloseBrace
	if (s[0] == ')')
	{
		type = CloseBrace;
		goto ELSE;
	}
	else
	{
		type = INVALID;
		goto ELSE;
	}
	
	//OpenBrace priority
	OB:
	priority = 0;
	goto END;
	
	//Plus or Minus priority
	POM:
	priority = 1;
	goto END;
	
	//Multiplication of Division priority
	MOD:
	priority = 2;
	goto END;
	
	//Everything Else priority
	ELSE:
	priority = -1;
	goto END;
	
	END:
	token = s;
}

int Token :: value() const
{
	//If the type is Integer
	if (type == Integer)
	{
		stringstream str(token);
		int i = 0;
		str >> i;
		return i;
	}
		
		//If the type is Identifier
	else if (type == Identifier)
	{
		return -1;
	}
		
		//If the type is anything else
	else
	{
		return -2;
	}
}

void Token :: display() const
{
	//Display type
	if (get_type() == 0)
	{
		cout << "type = Identifier" << endl;
	}
	else if (get_type() == 1)
	{
		cout << "type = Integer" << endl;
	}
	else if (get_type() == 2)
	{
		cout << "type = Operators" << endl;
	}
	else if (get_type() == 3)
	{
		cout << "type = EqualSign" << endl;
	}
	else if (get_type() == 4)
	{
		cout << "type = OpenBrace" << endl;
	}
	else if (get_type() == 5)
	{
		cout << "type = CloseBrace" << endl;
	}
	else if (get_type() == 6)
	{
		cout << "type = INVALID" << endl;
	}
	
	//Display the token and value of the token
	cout << "token = " << get_token() << " (value is " << value() << ")" << endl;
	
	//Display the priority
	cout << "priority = " << get_priority() << endl << endl;
}

Token_type Token :: get_type() const
{
	return type;
}

string Token :: get_token() const
{
	return token;
}

int Token :: get_priority() const
{
	return priority;
}
