#ifndef _heap_h
#define _heap_h

#include <vector>
#include "error.h"

// this is the header file for a naive heap
using namespace std;

// default template, T is not a pointer
template <typename T> class Heap {
public:
   Heap(){}       	// default constructor
   Heap(vector<T> & _vector, bool _smaller_top = false);   // construct a heap with a vector using Floyd

   void insert(T);      // insert an element to the heap
   int size();          // return the size of the heap

   T& getTop();          // read the element at the top of the heap
   T delTop();          // delete and return the element at the top of the heap

   // set and get whether the smaller element or the bigger element goes to the top of the heap
   void setSmallerTop() {
	   isSmallerTop = true;
   }
   void setBiggerTop() {
	   isSmallerTop = false;
   }
   bool getIsSmallerTop() {
	   return isSmallerTop;
   }
   bool getIsBiggerTop() {
	   return !isSmallerTop;
   }

   // copy operator overloading
   Heap<T> & operator= (Heap<T>& copy) {
	   _elem = copy._elem;
	   isSmallerTop = copy.isSmallerTop;
	   return *this;
   }

protected:
   // percolate down until reaches the index n
   // return the positions of the element reaches
   int percolateDown(int i);
   // return the positions of the element reaches
   int percolateUp(int i);
   // Floyd's method for heap construction
   void heapify();

private:
   vector<T> _elem;      // stores the data of the heap as a vector

   bool isSmallerTop = true;

   // check whether _elem[i] is valid
   bool InHeap(int i){ return ((-1 < i) && (i < _elem.size())); }
   // return the index of th parent of i
   int Parent(int i){ return ((i - 1) >> 1); }
   // return the index of the left hand child of i
   int LChild(int i){ return (1 + (i << 1)); }
   // return the index of the right hand child of i
   int RChild(int i){ return ((1 + i) << 1); }
   // check if _elem[i] has Parent
   bool ParentValid(int i){ return (0 < i); }
   // check if _elem[i] has a left hand child
   bool LChildValid(int i){ return InHeap(LChild(i)); }
   // check if _elem[i] has a right hand child
   bool RChildValid(int i){ return InHeap(RChild(i)); }
   // return the largest one's index of _elem[i] and its two children
   // if no child exists, return i itseld
   int ProperParent(int i);

   // return the index of the element that should be nearer to the top
   // if two elements equals, return the first one's index
   int Upper(int i, int j){
	   if (isSmallerTop) {
		   return ((_elem[i] <= _elem[j]) ? i : j);
	   }
	   else {
		   return ((_elem[i] >= _elem[j]) ? i : j);
	   }
	}

	// return the index of the last non-leaf node
	int LastNonLeaf(){
		return (_elem.size() / 2 - 1);
	}


};


// specialized template, Tp is a pointer
template <typename Tp> class Heap<Tp *> {
public:
   Heap(){}              // default constructor
   Heap(vector<Tp *> & _vector, bool _smaller_top = false);   // construct a heap with a vector using Floyd

   void insert(Tp *);      // insert an element to the heap
   int size();          // return the size of the heap

   Tp* getTop();          // read the element at the top of the heap
   Tp* delTop();          // delete and return the element at the top of the heap

	// set and get whether the smaller element or the bigger element goes to the top of the heap
   void setSmallerTop() {
	   isSmallerTop = true;
   }
   void setBiggerTop() {
	   isSmallerTop = false;
   }
   bool getIsSmallerTop() {
	   return isSmallerTop;
   }
   bool getIsBiggerTop() {
	   return !isSmallerTop;
   }

   //// copy operator overloading
   //Heap<T> & operator= (Heap<T>& copy) {
	  // _elem = copy._elem;
	  // isSmallerTop = copy.isSmallerTop;
   //}

protected:
   // percolate down until reaches the index n
   // return the positions of the element reaches
   int percolateDown(int i);
   // return the positions of the element reaches
   int percolateUp(int i);
   // Floyd's method for heap construction
   void heapify();

private:
   vector<Tp *> _elem;      // stores the data of the heap as a vector

   bool isSmallerTop = true;

   // check whether _elem[i] is valid
   bool InHeap(int i){ return ((-1 < i) && (i < _elem.size())); }
   // return the index of th parent of i
   int Parent(int i){ return ((i - 1) >> 1); }
   // return the index of the left hand child of i
   int LChild(int i){ return (1 + (i << 1)); }
   // return the index of the right hand child of i
   int RChild(int i){ return ((1 + i) << 1); }
   // check if _elem[i] has Parent
   bool ParentValid(int i){ return (0 < i); }
   // check if _elem[i] has a left hand child
   bool LChildValid(int i){ return InHeap(LChild(i)); }
   // check if _elem[i] has a right hand child
   bool RChildValid(int i){ return InHeap(RChild(i)); }
   // return the largest one's index of _elem[i] and its two children
   // if no child exists, return i itseld
   int ProperParent(int i);
   // return the index that should be nearer to the top
   // if two elements equals, return the first one's index
   // access the element's value using the pointer
   int Upper(int i, int j){
	   if (isSmallerTop) {
		   return ( (_elem[i][0] <= _elem[j][0]) ? i : j);
	   }
	   else {
		   return ( (_elem[i][0] >= _elem[j][0]) ? i : j);
	   }
   }
	// return the index of the last non-leaf node
	int LastNonLeaf(){
		return (_elem.size() / 2 - 1);
	}


};


// implementation of the default Heap
// construct a heap using a vector
template <typename T> Heap<T>::Heap(vector<T> & _vector, bool _smaller_top = false){
   _elem = _vector;
   heapify();
   isSmallerTop = _smaller_top;
}

template <typename T> int Heap<T>::size(){
   return _elem.size();
}

template <typename T> T& Heap<T>::getTop(){
   if(_elem.size() == 0)
      error("heap does not have any element");
   return _elem[0];
}

template <typename T> int Heap<T>::percolateUp(int i){
   while(ParentValid(i)){
      int j = Parent(i);
      if(j == Upper(j, i)) break;
      // swap two elements
      {
         T temp = _elem[i];
         _elem[i] = _elem[j];
         _elem[j] = temp;
      }
      i = j;
   }
   return i;      // return the postion this element reaches
}

template <typename T> int Heap<T>::ProperParent(int i){
   if(RChildValid(i)){
      return Upper( Upper(i, LChild(i)), RChild(i) );
   }
   else if(LChildValid(i)){
      return Upper(i, LChild(i));
   }
   else{
      return i;
   }
}

template <typename T> int Heap<T>::percolateDown(int i){
   int j;
   while( i != ( j = ProperParent(i) ) ){
      // swap two elements
      {
         T temp = _elem[i];
         _elem[i] = _elem[j];
         _elem[j] = temp;
      }
      i = j;
   }
   return i;      // return the postion this element reaches
}

template <typename T> void Heap<T>::insert(T element){
   _elem.push_back(element);
   percolateUp(_elem.size() - 1);
}

template <typename T> T Heap<T>::delTop(){
   T maxElem = _elem[0];
   _elem[0] = _elem[_elem.size() - 1];
   _elem.pop_back();
   percolateDown(0);
   return maxElem;
}

template <typename T> void Heap<T>::heapify(){
   for(int i = LastNonLeaf(); i >= 0; i--){
      percolateDown(i);
   }
}














// implementation of the specialized Heap (Heap of pointers)
// It is the same with the default heap

// construct a heap using a vector
template <typename Tp> Heap<Tp *>::Heap(vector<Tp *> & _vector, bool _smaller_top = false){
   _elem = _vector;
   heapify();
   isSmallerTop = _smaller_top;
}

template <typename Tp> int Heap<Tp *>::size(){
   return _elem.size();
}

template <typename Tp> Tp * Heap<Tp *>::getTop(){
   if(_elem.size() == 0)
      error("heap does not have any element");
   return _elem[0];
}

template <typename Tp> int Heap<Tp *>::percolateUp(int i){
   while(ParentValid(i)){
      int j = Parent(i);
      if(j == Upper(j, i)) break;
      // swap two elements
      {
         Tp* temp = _elem[i];
         _elem[i] = _elem[j];
         _elem[j] = temp;
      }
      i = j;
   }
   return i;      // return the postion this element reaches
}

template <typename Tp> int Heap<Tp *>::ProperParent(int i){
   if(RChildValid(i)){
      return Upper( Upper(i, LChild(i)), RChild(i) );
   }
   else if(LChildValid(i)){
      return Upper(i, LChild(i));
   }
   else{
      return i;
   }
}

template <typename Tp> int Heap<Tp *>::percolateDown(int i){
   int j;
   while( i != ( j = ProperParent(i) ) ){
      // swap two elements
      {
         Tp* temp = _elem[i];
         _elem[i] = _elem[j];
         _elem[j] = temp;
      }
      i = j;
   }
   return i;      // return the postion this element reaches
}

template <typename Tp> void Heap<Tp *>::insert(Tp* element){
   _elem.push_back(element);
   percolateUp(_elem.size() - 1);
}

template <typename Tp> Tp * Heap<Tp *>::delTop(){
   Tp* maxElem = _elem[0];
   _elem[0] = _elem[_elem.size() - 1];
   _elem.pop_back();
   percolateDown(0);
   return maxElem;
}

template <typename Tp> void Heap<Tp *>::heapify(){
   for(int i = LastNonLeaf(); i >= 0; i--){
      percolateDown(i);
   }
}




#endif
