#include <iostream>
#include <set>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
using namespace std::placeholders;

enum class apple{ green, red }; // In C++11 to declare enum we need either class name or stuct to have a storngly typed enum and
                                // it is scoped.

//constexpr is an expression which will be evaluated at complie time itself
constexpr int cube(int x)
{
   return (x * x * x);
}

// when you assign __cm to any long double variable this user 
// defined literals get invoked and x value multiplied with 10. exe : long double height = 10__cm; // height = 100; ==> return 10 * 10;
constexpr long double operator""__cm(long double x) 
{
   return (x * 100); 
} 

class cpp11 // *final* keyword is used this class can not be inherited to any derived class in the same way for class functions too.
{

   private:
     
     size_t len = 0;   // in c++11 we can initialize class member variables in declaration itself
     // const long double *var = 10__cm;
     // static_assert(var < 100, "Error in long double defined literals");
     char *str = nullptr;
     
     // Template alias
     template<class value, class Alloc = allocator<value>>
     using greaterMultiSet = multiset<value, greater<value>, Alloc>;
     // the alias greaterMultiSet will intsert data in decending order by default.
     greaterMultiSet<int> x {4, 1, 3, 5, 8, 7, 9};

   protected:   
 
     //constexpr is an expression which will be evaluated at complie time itself
     const int cu = cube(10); // auto will be deduced to int type.
     // static_assert(cu < 1000, "Error in cube defined literals");
     static int sf;    // Here auto will be deduced to a data type based on aurgument type which we pass while calling the function. 

   public:

     cpp11() = default;   // forcing complier to generate default constructor though user defined ctor available.
     explicit cpp11(const initializer_list<string> &vec); // if we use explicit keyword we have to call the constructor explicitly 
                                 // (cpp11 c11 = 30) object creation will give error since its a implicit call (conversion call)

     cpp11(const cpp11&) = delete;  // if we delete default copy constructor function it can not be called. so in this class 
                                    // copy is not possible
     cpp11& operator=(const cpp11 &obj) =  delete;  // if we delete default copy assignment operator function it can not be called.
                                                    // so in this class assignment is not possible.

     cpp11(cpp11&&); // Move constructor here const not required as we make assignment then we change the rvalue to nullptr
     cpp11& operator=(cpp11&& obj); // Move assignment operator here const not required as we make assignment then we change
                                    // the rvalue to nullptr.

     ~cpp11(); // destructor

     // bind funtion is useful to pass a function parameter after a while based on other function (thread) result
     int multiple(int a, int b);
     
     // find data type
     template <typename T>
     void findArgumentType(T arg);

     // variadic template
     template <typename T1, typename ... Arg>
     void tempfun(T1 a, Arg ... arg);
     void tempfun(); // this empty function is mandatary to terminate the recursive call after the last argument.
      
     // use of decltype keyword to deduce to get exact type
     template <typename T1, typename T2>
     T2 funname1(T1 a, T2 b); 
     template <typename T1, typename T2>
     auto funname2(T1 a, T2 b) -> decltype(a+b);

     // virtual function, override denotes that this function is redefined in the derived classes.
     virtual void fun(); // override; 

     // usage of attributes => it will improve the code execution efficiency by reducing add instruction in ASCII level.
     __attribute__((always_inline)) auto add(auto i , auto j);

     // lambda function
     auto update(std::vector<int> & vec);

     // bit operation
     inline int bitoperation(auto number, int pos);
       
};

int cpp11::sf = 0;

/* constructor with initializer list */
cpp11::cpp11(const initializer_list<string> &vec)
{
  auto c = 0;

  for(auto i : vec)
  {   
     len = i.size();
     str = new char[len+1];
     

     for(auto cr : i)
     {
        
        str[c] = cr;
        c++;
     }
  }

  str[len+1] = '\0';
  cout<<"initializer list Len : "<<len<<" Name : "<<str<<endl;
}

/* Move copy constructor */
cpp11::cpp11(cpp11&& obj)
{
   len = obj.len;
   str = obj.str;
   
   obj.str = nullptr;
   obj.len = 0;
   cout<<"Move constructor Len : "<<len<<" Name : "<<str<<endl;
}
 
/* Move assignment operator */

cpp11& cpp11::operator=(cpp11 &&obj)
{
   if(this != &obj)
   {
     delete[] str;
     
     str = obj.str;
     len = obj.len;
   }

   obj.str = nullptr;
   obj.len = 0;

   cout<<"Move Assignment operator Len : "<<len<<" Name : "<<str<<endl;

   return *this;
}

/* Destructor */
cpp11::~cpp11()
{
   if(str != nullptr)
   {
      delete[] str; 
      str = nullptr;    
   }

   cout<<"~cpp11"<<endl;
}

// bind() example function
int cpp11::multiple(int a, int b)
{
   return a * b;
}

// Find argument type
template <typename T>
void cpp11::findArgumentType(T arg)
{
    cout<<"Argument type : "<<typeid(arg).name()<<"value : "<<arg<<endl;
}

// variadic template
template <typename T1, typename ... Arg>
void cpp11::tempfun(T1 a, Arg ... arg)
{
   cout<<" A : "<<a<<endl;
   tempfun(arg...);
}

void cpp11::tempfun()
{
   // this empty function is mandatary to terminate the recursive call after the last argument.
   for(auto i : x)
   {
      cout<<"Template Alias : "<<i<<endl;
   }
}

// without decltype keyword will always return T2 type. 
template <typename T1, typename T2>
T2 cpp11::funname1(T1 a, T2 b)
{
   return( a + b);
}

// use of decltype keyword to deduce to get exact type
template <typename T1, typename T2>
auto cpp11::funname2(T1 a, T2 b) -> decltype(a+b)
{
   return( a + b);
}

// Virtual function.
void cpp11::fun()
{
   cout<<"Base virtual function"<<endl;
} 

// usage of attributes
auto cpp11::add(auto a, auto b)
{
   return a + b;
}

// Lambda function to count odd numbers 
auto cpp11::update(std::vector<int> & vec)
{
   // Traverse the vector and increment mCounter if element is odd
   // this is captured by value inside lambda
   std::for_each(vec.begin(), vec.end(), [this](int element)
   {
      if(element % 2)
      {
	 ++sf; // Accessing member variable from outer scope
      }
   });

  return sf;
}

inline int cpp11::bitoperation(auto number, int pos)
{
   // set a bit using OR 
   number |= (1 << pos);
   cout<<" Setbit OR : "<<number<<endl;

   // clear a bit using AND 
   number = 7;
   number &= ~(1 << pos);
   cout<<"Clear bit : "<<number<<endl;
 
   // Check a bit using AND 
   number = 7;
   number &= (1 << pos);
   cout<<"Clear bit : "<<number<<endl;

   // toggle a bit using XOR
   number = 7;
   number ^= (1 << pos);
   cout<<"Toggle bit  : "<<number<<endl;

   return number;
}


int main()
{
   cpp11 Kbj; // this will invoke default constructor.
   cpp11 obj{"Leo Alexander"}; // this will invoke parameterized constructor.

   cpp11 Kcbj(move(obj)); // this will invoke move constructor.

   cpp11 abj{"Raghul Fernant"}; // this will invoke parameterized constructor.
   Kbj = move(abj); // this will invoke move assignment operator.
   
   // c++11 bind example
   // auto fun = std::bind(&Kbj.multiple, 10, _2);
   // cout<<"bind example : "<<fun(10)<<endl; 
   
   // Find argument type
   Kbj.findArgumentType(20.90);

   Kbj.tempfun(9, 9.9, "King to God");
  
   // without decltype keyword 
   cout<<"decltype funname1 : "<<Kbj.funname1(10.9, 10)<<endl;
   // with decltype keyword
   cout<<"decltype funname2 : "<<Kbj.funname2(10.9, 10)<<endl;
   
   // virtual function
   Kbj.fun();

   // use of attributes
   cout<<"attributes add : "<<Kbj.add(10.9, 10)<<endl;

   // lambda function to count odd number
   std::vector<int> vec = {11,1,8,9,5,2,3,7};
   cout<<"Lambda odd num counting : "<<Kbj.update(vec)<<endl;

   // bit operation
   cout<<"attributes add : "<<Kbj.bitoperation(9, 3)<<endl;

   return 0;
}
     
