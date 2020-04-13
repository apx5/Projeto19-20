#include <iostream>

template<typename T, int size> 
class Stack{
    T stack[size];
    int top{0};
public:
    void push(const T& elem){
        stack[top++] = elem;
    }
    void pop(); //se quisermos definir a função fora da classe, temos de a declarar na mesma dentro da classe
    
    //void pop(){ //função pop definida dentro da classe
    //    top--;
    //}

    const T& Top(){
        return stack[top-1];
    }
    bool isEmpty(){
        return top == 0;
    }

};

//se quisessemos escrever a função pop fora da declaração da classe teriamos

template<typename T, int size> void Stack<T, size>::pop(){
    top--;
}

int main(int argc, char const *argv[])
{
    Stack<int,10> s;
    s.push(2);
    s.push(4);
    s.push(3);
    while(!s.isEmpty()){
        std::cout << s.Top();
        s.pop();
        if(!s.isEmpty()) std::cout << ',';
        else std::cout << std::endl;
    }

    return 0;
}
