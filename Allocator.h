// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2013
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert> // assert
#include <new> // new
#include <stdexcept> // invalid_argument
#include <array>

// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T value_type;

        typedef int size_type;
        typedef std::ptrdiff_t difference_type;

        typedef value_type* pointer;
        typedef const value_type* const_pointer;

        typedef value_type& reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
        // ----
        // data
        // ----

        char a[N];

        // -----
        // valid
        // -----

        /**
* O(1) in space
* O(n) in time
* <your documentation>
*/

     bool valid () const {
            // <your code>
            int b = 0;
            while(b < N) {
            int i =  *reinterpret_cast<int*>(const_cast<char*>(&a[b])), j;
            if(i > 0) {
              j = *reinterpret_cast<int*>(const_cast<char*>(&a[b + i + 4]));}
            else {
              j =  *reinterpret_cast<int*>(const_cast<char*>(&a[b + (i * -1) + 4]));}
            if(i != j) {
		return false;
	    }
            if(j < 0) {
               b += j * -1 + 8; } 
 	    else { 
              b += j + 8; }
            }
            return true;}

   int& view (char& c) {
    return *reinterpret_cast<int*>(&c);}

    public:

        // ------------
        // constructors
        // ------------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        Allocator () {
            // <your code>
	    int size = N, smallestBlock = sizeof(T) + (2* sizeof(int));
            if(size < smallestBlock) {
              std::bad_alloc exception;
              throw exception;}
	    int* p1 = reinterpret_cast<int*>(&a[0]);
            *p1 = size - 8;
            int* p2 = reinterpret_cast<int*>(&a[size - 4]);
            *p2 = size - 8;
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Allocator (const Allocator<T>&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
* O(1) in space
* O(n) in time
* <your documentation>
* after allocation there must be enough space left for a valid block
* the smallest allowable block is sizeof(T) + (2 * sizeof(int))
* choose the first block that fits
*/
        pointer allocate (size_type n) {
            // <your code>
            pointer p;
            int b = 0;
            n = n * sizeof(T);
            while(b < N){ 
            int& sentinel = view(a[b]);
            if(sentinel >= n) {
              int* p1 = reinterpret_cast<int*>(&a[b]);
              *p1 = n * -1;
              int* p2 = reinterpret_cast<int*>(&a[b + n + 4]);
              *p2 = n * -1;
              int newb = b + n + 8;
              int smallestBlock = (sizeof(T) + (2 * sizeof(int)));
              if(  (N - newb) < smallestBlock  ) {
                n = n + (N - newb);
                a[b] = n * -1;
                a[b + n + 4] = n * -1;
                p = reinterpret_cast<value_type*>(&a[b + 4]);
                assert(valid());
                return p;
              }
              int newSent = N - newb - 8;
              a[newb] = newSent;
              a[newb + 4 + newSent] = newSent;
              p = reinterpret_cast<value_type*>(&a[b + 4]);
              assert(valid());
              return p;  
            }
            if(sentinel < 0) {
              b = b + 8 + (sentinel * -1);}
            else {
              b = b + 8 + sentinel;}
            }
            std::bad_alloc exception;
            throw exception;
            return 0;}

        // ---------
        // construct
        // ---------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void construct (pointer p, const_reference v) {
            //new (p) T(v); // uncomment!
            assert(valid());}

        // ----------
        // deallocate
        // ----------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
* after deallocation adjacent free blocks must be coalesced
*/
        void deallocate (pointer p, size_type = 0) {
            // <your code>
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void destroy (pointer p) {
            //p->~T(); // uncomment!
            assert(valid());}};

#endif // Allocator_h
