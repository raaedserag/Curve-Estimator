#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <string>
#include<sstream> 
using namespace std ;
//structures
struct spline {
	double a ;
	double b ;
	double c ;
	double d ;
} ;
//ProtoTypes
double f_fac(double a , double b ) ;
double enter_dx(int x , double input = 0) ;
double enter_dy(int y , double input = 0) ;
int  enter_n() ;
double rhs(double h , double a , double b , double c ) ;
double c(double a , double b , double c ) ;
double b(double h , double a , double b , double c , double d)  ;
double d(double h , double a , double b) ;
int calc_range(int n , double *arr , double x);
double calc_x(int i , spline *sp , double x  , double *xi) ;
// main
void main() {
	//declare "n" number of inputs 
	cout<<"Please enter the number of inputs you have"<<endl ;
	int n = enter_n() ;
	// declaring reqired arrays 
	double *x = new double [n] ;
	double *y = new double [n] ;
	double *f = new double [n] ;
	double *r = new double [n] ;
	double *r2 = new double [n] ;
	spline *s = new spline [n] ; // the array which can hold the spline functions variables
	//input
	for ( int i = 0 ; i<n ; i++) {
		cout<<"Please Enter The Element: X["<<i<<"] "<<endl ;
		x[i] = enter_dx(i) ;
		cout<<"Please Enter The Element: Y["<<i<<"] "<<endl ;
		y[i] = enter_dy(i) ;
	}
	double h ; // the interval
	h= x[1] - x[0] ;
	//--------------------------------------------------------------//
	// "F" Factor
	f[0] = 0 ;
	f[1] = 1 ;
	for ( int i = 2 ; i<=(n-1) ; i++) {
		f[i] = f_fac(f[i-1] , f[i-2] ) ;
	}
	//--------------------------------------------------------------//
	//"R.H.S" Factor, r will be swapped to calculate c1 constant , r2 won't be swapped to calculate c constants
	r[0] = 0 ;
	r[n-1] = 0 ;
	r2[0] = 0 ;
	r2[n-1] = 0 ;
	for ( int i = 1 ; i<=(n-2) ; i++) {
		r[i] = rhs(h ,y[i-1] , y[i] , y[i+1] ) ;
		r2[i] = rhs(h ,y[i-1] , y[i] , y[i+1] ) ;
	}
	//Swapping array of "r"
	int ev  ; // ev is a parameter to c1 calculation in the next step , don't care now ! 
	int m ;
	if ( (n % 2) == 0) {//length n is even 
		m = n/2 ;
		ev = 0 ; 
	}
	else { //length n is odd
		m = (n-1)/2 ;
		ev = 1 ;
	}
	double temp ; 
	for (int i = 0 ; i<m ; i ++ ) { 
		temp = r[i] ;
		r[i] = r[n-i-1] ;
		r[n-i-1] = temp ;
	}
	//--------------------------------------------------------------//
	//Calculating "C1" , the key for all next "c" constants !
	double c1 = 0 ;
	for(int i = 1 ; i<=(n-2) ; i++)
	{
	c1 += pow(-1 , (i+ev)) * f[i] * r[i] / f[n-1] ;
	}
	//--------------------------------------------------------------//
	//CREATING S[i] , the final functions which calculate the values of any entered data
	//calculate "c"
	s[0].c = 0 ;
	s[1].c = c1 ;
	for ( int i = 2 ; i<n ; i++) 
	{
		s[i].c = c(r2[i-1] , s[i-1].c , s[i-2].c ) ;
	}
	//--------------------------------------------------------------//
	// a[i] = y[i]
	for ( int i = 0 ; i<(n-1) ; i++)
	{
		s[i].a = y[i] ;
	}
	//--------------------------------------------------------------//
	//Calculate "b"
	for ( int i = 0 ; i<n ; i++)
	{
		s[i].b = b(h , y[i+1] , y[i] , s[i+1].c , s[i].c ) ;
	}
	//--------------------------------------------------------------//
	// calculate "d"
	for ( int i = 0 ; i<n ; i++)
	{
		s[i].d = d(h , s[i+1].c , s[i].c ) ;
	}
	//Printing "S[i]" , {s[i] = a[i] + b[i](x-x[i]) + c[i]((x-x[i])^2) + d[i]((x-x[i])^3) }
	for ( int i = 0 ; i<(n-1) ; i++)
	{
		cout<<"s["<<i<<"] = "<<s[i].a<<" + "<<s[i].b<<"(x-"<<x[i]<<") + "<<s[i].c<<"((x-"<<x[i]<<")^2) + "<<s[i].d<<"((x-"<<x[i]<<")^3)"<<endl ;
	}
	//--------------------------------------------------------------//
	//calculate a value of x
	cout<<"please enter a value of 'x' "<<endl ; 
	double x_value ;
	cin>>x_value ; 
	if (x_value != 0  ) { //want returns the value of x if the user entered a value, or return 0 else.
		int range = calc_range(n , x , x_value ) ;
		double y_value ;
		y_value = calc_x(range , s , x_value , x ) ;
		cout<<"y = "<<y_value<<endl ;
	}
	cout<<"the entered x isn't in the range : ("<<x[0]<<","<<x[n-1]<<")"<<endl ;
	int hold ; cin>>hold ;
	//--------------------------------------------------------------//
	//Deleting dynamic memories
	delete [n] x ;
	delete [n] y ;
	delete [n] f ;
	delete [n] r ;
	delete [n] r2 ;
	delete [n] s ;
	//--------------------------------------------------------------//
}
//if yes, calculate the range of 'x'
int calc_range(int n , double *arr , double x ) {
	cout<<"please enter a value of 'x' in the range : ("<<arr[0]<<" , "<<arr[n-1]<<")"<<endl ;
	cin>> x ;
	for (int i = 0 ; i<n ; i++)
	{
		if ( x > arr[i] ) {
			return i ;
		}
	}
	return 0 ;
}
//calculate the value of 'x'
double calc_x(int i , spline *sp , double x  , double *xi) {
	return ( (sp[i].a) + ((sp[i].b)*((x)-(xi[i]))) + ((sp[i].c) *(pow( ((x) - (xi[i])), 2))) + ((sp[i].d) *(pow( ((x) - (xi[i])), 3))) ) ;
}
//then calculate
//CHECK INPUTS
// X Input 
double enter_dx(int x , double input ) { ;
	cin>>input ;
	if(!cin) 
	{
		// user didn't input a number
		cout<<"Please enter only numeric input for the element X["<<x<<"] "<<endl ;
		cin.clear(); // reset failbit
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		enter_dx(x) ;
	}
	return input ;
}
// Y Input 
double enter_dy(int y , double input  ) {
	cin>>input ;
	if (!cin) 
	{
		// user didn't input a number
		cout<<"Please enter only numeric input for the element Y["<<y<<"] "<<endl ;
		cin.clear(); // reset failbit
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		// next, request user reinput
		enter_dy(y) ;
	}
	return input ;
}
// N Input 
int  enter_n() {
	int  input ;
	cin>>input ;
	if (!cin) 
	{
		// user didn't input a number
		cout<<"Please enter a valid number of inputs ! "<<endl ;
		cin.clear(); // reset failbit
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		// next, request user reinput
		enter_n() ;
	}
	return input ;
}
/* To get the "C" constants we should first calculate "C1" in terms of (F, R.H.S) while "C0" = 0 .
Then we can calculate # C constants = (n-2) with carelessness the value of "C[n-1]" which equal 0 according to the theory of "Interpolation with cubic spline".
*/
// FUNCTION of calculating "R.H.S" factors, { R[i] = (3/h^2) (Y[i-1] -2Y[i] +Y[i+1] }, R[0] = 0 .
double rhs(double h , double a , double b , double c ) {
	return  (3/(h * h )) * (a - (b) - (b) + c) ;
}
// FUNCTION of calculating "F" factors, { F[i] = 4F[i-1] - F[i-2] }, F[0] = 0 , F[1] = 1 .
double f_fac(double a , double b ) {
		return (4*a) - (b) ;
}
// FUNCTION of calculating "c" factors, { c[i] = r2[i-1] - 4c[i-1] - c[i-2] }, c[0] = 0 , c[1] = already calculated .
double c(double a , double b , double c ){
	return (a - (4*b) - c ) ;
}
// FUNCTION of calculating "b" factors, { b[i] = ((y[i+1]-y[i])/h) - ((h/3)* (c[i+1] + 2c[i])) }
double b(double h , double a , double b , double c , double d) 
{
	return ((( a - b ) / h )-((h/3))*(c + (2*d) )) ; 
}
// FUNCTION of calculating "d" factors, { d[i] = (c[i+1] - c[i])/(3*h) }
double d(double h , double a , double b) {
	return ((a-b)/(3*h)) ;
}