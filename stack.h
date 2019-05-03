#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstdlib>


struct Node
{
    std::string value;

    Node* link;

};


class stack
{
    public:

        void Push(std::string);
        void Pop();

        Node* top;

        void Traverse();

        stack() : top (NULL)
        { }

};


void stack::Push(std::string _value)
{
    Node* temp = new Node();

    temp->value = _value;

    temp->link = top;

    top = temp;

}//

void stack::Pop()
{
    Node* temp;

    if (top == NULL)
    { std::cout<< "Top is NULL." << std::endl; }
    else
    {
        temp = top;

        top = top->link;

        delete temp;
    }


}//


void stack::Traverse()
{
    if (top == NULL)
    {
        std::cout << "Empty." << std::endl;
        return;
    }

    Node* node = top;

    while (node != NULL)
    {
        std::cout << node->value << std::endl;

        node = node ->link;
    }

}//


#endif // STACK_H
