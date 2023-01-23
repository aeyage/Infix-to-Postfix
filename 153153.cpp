#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

//define node structure
template <class Type>
struct nodeType 
{
    char info;
    nodeType<Type>* next;
};

//define class InfixToPostfix
template <class Type>
class InfixToPostfix 
{
    private:
        nodeType<Type>* infx;
        nodeType<Type>* pfx;
        stack<Type> s;

    public:
        //initialise pfx to an empty expression
        InfixToPostfix() { infx = pfx = NULL; } //default constructor 
        ~InfixToPostfix() //default destructor
        {
            nodeType<Type>* current = infx;
            while (current != NULL) 
            {
                nodeType<Type>* next = current->next;
                delete current;
                current = next;
            }

            current = pfx;
            while (current != NULL) 
            {
                nodeType<Type>* next = current->next;
                delete current;
                current = next;
            }
        }
        void getInfix(InfixToPostfix& i2p, string expression);
        void convertToPostfix();
        void showInfix();
        void showPostfix();
        bool precedence(char op1, char op2);
        int getWeight(char op);
        bool isRightAssociative(char op);
};

template <class Type>
void InfixToPostfix<Type>::getInfix(InfixToPostfix& i2p, string expression)
{
    nodeType<Type>* current = new nodeType<Type>;
    current->next = NULL;

    for (int i = 0; i < expression.length(); i++) 
    {
        current->info = expression[i];

        if (i2p.infx == NULL) 
        {
            i2p.infx = current;
        }
        else 
        {
            nodeType<Type>* p = i2p.infx; //p is used to iterate through the linked list of pfx
            while (p->next != NULL) 
            {
                p = p->next;
            }
            p->next = current;
        }
        current = new nodeType<Type>;
        current->next = NULL;
    }
}

template <class Type>
void InfixToPostfix<Type>::showInfix()
{
    nodeType<Type>* current = infx;
    cout << "Infix Expression: ";

    while (current != NULL) 
    {
        cout << current->info;
        current = current->next;
    }
    cout << endl;
}

template <class Type>
void InfixToPostfix<Type>::convertToPostfix()
{
    //get the next sym from infx
    nodeType<Type>* current = infx;
    while (current != NULL) 
    {
        if (isalnum(current->info)) //if sym is an operand, append sym to pfx
        {
            nodeType<Type>* newNode = new nodeType<Type>;
            newNode->info = current->info;
            newNode->next = NULL;

            if (pfx == NULL) 
            {
                pfx = newNode;
            }
            else 
            {
                nodeType<Type>* p = pfx;
                while (p->next != NULL) 
                {
                    p = p->next;
                }
                p->next = newNode;
            }
        }
        else if (current->info == '(') //if sym is (, push sym into the stack
        {
            s.push(current->info);
        }
        /*if sym is ), popand append all the symbols from the stack until the most
        recent left parenthesis.Pop and discard the left parenthesis.*/
        else if (current->info == ')') 
        {
            while (s.top() != '(') 
            {
                nodeType<Type>* newNode = new nodeType<Type>;
                newNode->info = s.top();
                newNode->next = NULL;
                s.pop();

                if (pfx == NULL) 
                {
                    pfx = newNode;
                }
                else 
                {
                    nodeType<Type>* p = pfx;
                    while (p->next != NULL) 
                    {
                        p = p->next;
                    }
                    p->next = newNode;
                }
            }
            s.pop();
        }
        /*if sym is an operator, pop and append all the operator from the stack to pfx that are
        above the most recent left parenthesis and have precedence greater than or equal to sym*/
        else 
        {
            while (!s.empty() && precedence(s.top(), current->info)) 
            {
                nodeType<Type>* newNode = new nodeType<Type>;
                newNode->info = s.top();
                newNode->next = NULL;
                s.pop();

                if (pfx == NULL) 
                {
                    pfx = newNode;
                }
                else 
                {
                    nodeType<Type>* p = pfx;
                    while (p->next != NULL) 
                    {
                        p = p->next;
                    }
                    p->next = newNode;
                }
            }
            s.push(current->info);
        }
        current = current->next;
    }
    /* After processing infx, some operators might be left in the stack. Pop and append to
    pfx everything from the stack. */
    while (!s.empty()) 
    {
        nodeType<Type>* newNode = new nodeType<Type>;
        newNode->info = s.top();
        newNode->next = NULL;
        s.pop();

        if (pfx == NULL) 
        {
            pfx = newNode;
        }
        else 
        {
            nodeType<Type>* p = pfx;
            while (p->next != NULL) 
            {
                p = p->next;
            }
            p->next = newNode;
        }
    }
}

//printing out the pfx
template <class Type>
void InfixToPostfix<Type>::showPostfix()
{
    nodeType<Type>* current = pfx;
    cout << "Postfix Expression: ";

    while (current != NULL) 
    {
        cout << current->info << " ";
        current = current->next;
    }
    cout << endl;
}

template <class Type>
bool InfixToPostfix<Type>::precedence(char op1, char op2)
{
    int op1Weight = getWeight(op1);
    int op2Weight = getWeight(op2);

    if (op1Weight == op2Weight) 
    {
        return isRightAssociative(op1) ? false : true; 
    }
    return op1Weight > op2Weight ? true : false;
}

template <class Type>
int InfixToPostfix<Type>::getWeight(char op)
{
    int weight = -1; //initialise the weight to a default value

    switch (op) 
    {
        case '-':
        case '+':
            weight = 1;
            break;
        case '/':
        case '*':
            weight = 2;
            break;
        case '^': 
            weight = 3; //for the purpose of checking right associativity
    }
    return weight;
}

template <class Type>
bool InfixToPostfix<Type>::isRightAssociative(char op) //check whether operator is right associative or not
{
    if (op == '^')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main() 
{
    InfixToPostfix<int> i2p;
    string expression;

    cout << "Enter the infix expression: ";
    getline(cin, expression);
    
    cout << endl;

    i2p.getInfix(i2p, expression);
    i2p.showInfix();
    i2p.convertToPostfix();
    i2p.showPostfix();

    return 0;
}