/**
\file
 Demo utilities
*/

#include "util.h"

#include <stdio.h>
#include <string.h>

GLuint G_time = 0;

/**
 * exit function, returns a nuber.
 */
void _exit(int status){
	__asm__ __volatile__ (
		"movl $1, %%eax;\n\t"
		"movl %0, %%ebx;\n\t"
		"int $0x80;\n\t"
		: /* no output operands */
		:"r"(status)
		: "%eax"
	);
	// never reached
	while(1);
}

/**
 * Wrapper around malloc function. If something goes wrong, exit.
 * Sets memory to zeroes.
 * Kernel frees program memory.
 * @param size size of memory for allocation
 * @return pointer to allocated memory
 */
void *checkMalloc(size_t size){
	void *tmp;
	if(!(tmp = malloc(size))){
		error("Memmory allocation error\n");
	}
	myMemset(tmp, 0, size);
	return tmp;
}

/**
 * Wrapper around realloc function. If something goes wrong, exit.
 * Kernel frees program memory.
 * @param ptr pointer to original place
 * @param pointer to size of new memory
 * @return pointer to allocated memory
 */
void *checkRealloc(void *ptr, size_t size){
	void *tmp;
	if (!(tmp = realloc(ptr, size))){
		error("Memmory allocation error\n");
	}
	return tmp;
}
/**
 * Duplicates array
 * @param ptr original array
 * @param size size of original array
 * @return pointer to copy of original array
 */
void *checkStrdup(void *ptr, size_t size){
	void *tmp;
	tmp = checkMalloc(size);
	myMemcpy(tmp, ptr, size);
	return tmp;
}

/**
 * fill memory with a constant byte
 * @param s target memory area
 * @param c constatn byte
 * @param size size of memory area
 */
void myMemset(void *s, GLint c, size_t size){
	uint8_t val = (uint8_t) (c & 0xff);
	char *dest = s;
	for(int i = 0; i < size; ++i){
		dest[i] = val;
	}
}

/**
 * copy memory area
 * @param dest destination memory area
 * @param source source memory area
 * @param size size of memory area
 */
void myMemcpy(void *dest, void *source, size_t size){
	char *d = dest;
	char *s = source;
	for(int i = 0; i < size; i++ ){
		d[i] = s[i];
	}
}

/**
 * Converts degrees to radians
 * @param deg input degrees
 * @return radian value of deg
 */
GLfloat deg2rad(GLfloat deg){
	return deg * (M_PI / 180.0f);
}


/**
 * sine function.
 * @param angle in radians
 * @return sin value
 */
GLfloat sinf(GLfloat angle){
	GLfloat result;
	__asm__ __volatile__ (
		"flds %1;\n\t"
		"fsin;\n\t"
		"fstps %0;\n\t" :
		"=m" (result) :
		"m" (angle)
	);
	return result;
}

/**
 * cosine function.
 * @param angle in radians
 * @return cos value
 */
GLfloat cosf(GLfloat angle){
	GLfloat result;
	__asm__ __volatile__ (
		"flds %1;\n\t"
		"fcos;\n\t"
		"fstps %0;\n\t"
		:"=m" (result)
		:"m" (angle)
	);
	return result;
}

/**
 * tangent function.
 * @param angle in radians
 * @return tan value
 */
GLfloat tanf(GLfloat angle){
	GLfloat s, c, tan;
	s = sinf(angle);
	c = cosf(angle);
	tan = s / c;
	return tan;
}

/**
 * square root function.
 * @param val value
 * @return square root of val
 */
GLfloat mySqrt(GLfloat val){
	GLfloat result ;
	__asm__ __volatile__(
		"flds %1;\n\t"
		"fsqrt;\n\t"
		"fstps %0;\n\t"
		:"=m" (result)
		:"m" (val)
	);
	return result;
}

/**
 * compute the absolute value of an float.
 * @param val value
 * @return absolute value of val
 */
GLfloat myFabs(GLfloat val){
	GLfloat result ;
	__asm__ __volatile__(
		"flds %1;\n\t"
		"fabs;\n\t"
		"fstps %0;\n\t"
		:"=m" (result)
		:"m" (val)
	);
	return result;
}

/**
 * Multiply two matrices
 * @param matrix1 first matrix
 * @param matrix2 second matrix
 * @param result matrix
 */
void multMatrices(GLfloat *matrix1, GLfloat *matrix2, GLfloat *result){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i * 4 + j] = matrix1[i * 4 + 0] * matrix2[0 * 4 + j] +
					    matrix1[i * 4 + 1] * matrix2[1 * 4 + j] +
					    matrix1[i * 4 + 2] * matrix2[2 * 4 + j] +
					    matrix1[i * 4 + 3] * matrix2[3 * 4 + j];
		}
	}
}
