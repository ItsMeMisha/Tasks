#include <stdint.h>
 
/*! This function is realization of Murmurhash2 with seed = 0 (light version)
*   
*	@param BeginPointer - pointer to the begin of the counting area of memory
*	@param size - size in bytes of the area of memory
* 
*	@return hash 
*/

unsigned MurmurHash2Light (void* BeginPointer, size_t size) {

	assert (BeginPointer);

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int hash = size;

	uint8_t* data = (uint8_t*) BeginPointer;
	unsigned int k = 0;

	while (size >= 4) {

	    k  = data[0];
		k |= data[1] << 8;
		k |= data[2] << 16;
    	k |= data[3] << 24;

    	k *= m;
    	k ^= k >> r;
    	k *= m;

    	hash *= m;
     	hash ^= k;

     	data += 4;
     	size -= 4;
	}

	switch (size) {

    case 3:
    	hash ^= data[2] << 16;

    case 2:
    	hash ^= data[1] << 8;

    case 1:
    	hash ^= data[0];
    	hash *= m;

	};
	
	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

	return hash;

}  
