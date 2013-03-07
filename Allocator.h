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
  b is the index that is used to go through array
  i and j are the pair of sentinels, if they don't match, return false
  increment b to index of next sentinel and loop until at end of array

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
  If size of array is too small, bad_alloc exception is thrown
  p1 and p2 are used to put in values of sentinels
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
  b is used as an iterator through the array.
  n becomes the number of bytes to be allocated.
  If the number the block being checked is large, memory will be allocated.
     If the space after allocation is too small, the leftover space will be allocated as well.
     p5 and p6 are used to change sentinels to be negative, pointer p is the first byte in the allocated block and is returned.
  p1 and p2 are used to change sentinels to be negative
  newb and newSent are the index and sentinel respectively for the new free block
  p3 and p4 are used to put in the new sentinels for the free block
  pointer p is the first byte in the allocated block and is returned.
  
  If b goes through whole array and doesn't find enough free space, bad_alloc exception is thrown

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
            int sentinel = view(a[b]);
            int smallestBlock = (sizeof(T) + (2 * sizeof(int)));
            if(sentinel >= n) {
               if(  (sentinel - n) < smallestBlock  ) {
                n = sentinel;
                int* p5 = reinterpret_cast<int*>(&a[b]);
                *p5 = n * -1;
                int* p6 = reinterpret_cast<int*>(&a[b + n + 4]);
                *p6 = n * -1;
                p = reinterpret_cast<value_type*>(&a[b + 4]);
                assert(valid());
                return p;
              }
              int* p1 = reinterpret_cast<int*>(&a[b]);
              *p1 = n * -1;
              int* p2 = reinterpret_cast<int*>(&a[b + n + 4]);
              *p2 = n * -1;
              int newb = b + n + 8;
              int newSent = sentinel - n - 8;
              int* p3 = reinterpret_cast<int*>(&a[newb]);
              *p3 = newSent;
              int* p4 = reinterpret_cast<int*>(&a[newb + 4 + newSent]);
              *p4 = newSent;
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
            new (p) T(v); // uncomment!
            assert(valid());}

        // ----------
        // deallocate
        // ----------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
  index is the index in the array that pointer p is pointing at
  p1 and p2 are used to change the sentinel values from negative to postive, freeing them
  Depending on where p is in the array, the left or right block is then checked to see if it is also free
     If so, free blocks are combined
 
* after deallocation adjacent free blocks must be coalesced
*/
        void deallocate (pointer p, size_type = 0) {
            // <your code>
            int index = reinterpret_cast<char*>(p) - &a[0];
            int sentinel = view(a[index - 4]);
            int* p1 = reinterpret_cast<int*>(&a[index - 4]);
            *p1 = sentinel * -1;
            int* p2 = reinterpret_cast<int*>(&a[index - sentinel]);
            *p2 = sentinel * -1;
            
            int pToEnd = &a[N-1] - reinterpret_cast<char*>(p);
            int smallestBlock = (sizeof(T) + (2 * sizeof(int)));
            if(index - 4 >= smallestBlock) {
               int leftSentinel = view(a[index-8]);
               if(leftSentinel > 0) {
                 int newSentinel = leftSentinel + (sentinel * -1) + 8; 
                 int* p3 = reinterpret_cast<int*>(&a[index - 12 - leftSentinel]);
                 *p3 = newSentinel;
                 *p2 = newSentinel;
               }
            }

            else if(pToEnd - 4 - (sentinel * -1) >= smallestBlock) {
              int rightSentinel = view(a[index - sentinel + 4]);
              if(rightSentinel > 0) {
                int newSentinel = rightSentinel + (sentinel * -1) + 8;
                int* p4 = reinterpret_cast<int*>(&a[index + (sentinel * -1) + rightSentinel + 8]);
                *p4 = newSentinel;
                *p1 = newSentinel;
              }
            }
            assert(valid());

        }

        // -------
        // destroy
        // -------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void destroy (pointer p) {
            p->~T(); // uncomment!
            assert(valid());}};

#endif // Allocator_h
