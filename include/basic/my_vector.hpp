//
// Created by kamil on 28.01.2020.
//
#ifndef my_vector2_HPP
#define my_vector2_HPP
#define SFML_USED
#ifdef SFML_USED
#include <SFML/System/Vector2.hpp>
#endif
#include <cmath>
#include <ostream>
template<typename T>
struct my_vector2
{
    T x,y;
    //Constructors
    my_vector2() {x=static_cast<T>(0);y=static_cast<T>(0);}
    my_vector2(T pos_x, T pos_y) {x=pos_x;y=pos_y;}
    //Implicit conversions
    template<typename U>
    my_vector2(const my_vector2<U>& vec): x(vec.x), y(vec.y) {}
    #ifdef SFML_USED
    template<typename U>
    my_vector2(const sf::Vector2<U>& vec): x(vec.x), y(vec.y) {}
    template<typename U>
    operator sf::Vector2<U>() const {return sf::Vector2<U>(x,y);}
    #endif
    //Assignment operators
    void operator+= (const my_vector2<T> v) {x+=v.x; y+=v.y;}
    void operator-= (const my_vector2<T> v) {x-=v.x; y-=v.y;}
    template<typename U>
    void operator*= (U a) {x*=a; y*=a;}
    template<typename U>
    void operator/= (U a) {x/=a; y/=a;}
    //Additional
    T sqr_length() const {return x*x+y*y;}
    T length() const {return std::sqrt(this->sqr_length());}
    void normalize() {if(sqr_length()!=0) *this/length();}
    //static constexpr my_vector2 zero=my_vector2(static_cast<T>(0),static_cast<T>(0));

};
//Arithmetic operators
template<typename T>
my_vector2<T> operator+(const my_vector2<T>& a) {return a;}
template<typename T>
my_vector2<T> operator-(const my_vector2<T>& a) {return my_vector2<T>(-a.x,-a.y);}
template<typename T>
my_vector2<T> operator+(const my_vector2<T>& a, const my_vector2<T>& b) {return my_vector2<T>(a.x+b.x,a.y+b.y);}
template<typename T>
my_vector2<T> operator-(const my_vector2<T>& a, const my_vector2<T>& b) {return my_vector2<T>(a.x-b.x,a.y-b.y);}
template<typename T, typename U>
my_vector2<T> operator*(U val, const my_vector2<T>& a) {return my_vector2<T>(val*a.x,val*a.y);}
template<typename T, typename U>
my_vector2<T> operator*(const my_vector2<T>& a, U val) {return my_vector2<T>(val*a.x,val*a.y);}
template<typename T, typename U>
my_vector2<T> operator/(const my_vector2<T>& a, U val) {return my_vector2<T>(a.x/val,a.y/val);}
//Comparison operators
template<typename T>
bool operator==(const my_vector2<T>& a, const my_vector2<T>& b) {return a.x == b.x && a.y == b.y;}
#if _cplusplus<=201703L
template<typename T>
bool operator!=(const my_vector2<T>& a, const my_vector2<T>& b) {return !(a==b);}
#endif
//Additional
template<typename T>
void distance(const my_vector2<T>& a, const my_vector2<T>& b) {return length(a-b);}
template<typename T>
my_vector2<T> dot_product(const my_vector2<T>& a, const my_vector2<T> b) {return a.x*b.x+a.y*b.y;}
template<typename T>
float cos_angle(const my_vector2<T>& a, const my_vector2<T> b) {return dot_product(a,b)/(length(a)*length(b));}
template<typename T>
float angle(const my_vector2<T>& a, const my_vector2<T> b) {return std::acos(cos_angle(a,b));}
template<typename T>
T length(const my_vector2<T>& a) {return a.length();}
template<typename T>
T sqr_length(const my_vector2<T>& a) {return a.sqr_length();}
template<typename T>
std::ostream& operator<< (std::ostream& stream, const my_vector2<T>& a) {stream << "(" << a.x << ", " << a.y << ")"; return stream;}
typedef my_vector2<int> my_vector2i;
typedef my_vector2<float> my_vector2f;
typedef my_vector2<unsigned> my_vector2u;


#endif //my_vector2_HPP
