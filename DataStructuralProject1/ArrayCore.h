#include "Utility.h"
#include "AutoPtr.h"
#include "TypedDefineBegin.h"
#define _type_ T//别名

#define CHECK_RANGE(A) CHECK(if (A->m_data > A->m_data_end)\
{ ET(OutOfRange, "Array out of range",); })

//private
#define Template array_init
#define array_init(T, A, init_capacity) _TypedVar_(array_init, T)(A, init_capacity)
void _template_(Array(T) A, size_t capacity)
{
	if (capacity < 4) capacity = 4;
	capacity = up_pow2(size_t, capacity);
	A->m_capacity = capacity;
	A->m_data = MALLOC(T, capacity);
	A->m_end = A->m_data;
	A->m_data_end = A->m_data + A->m_capacity;
	A->m_size = 0;
}

#define _class_ Array(T)

//public construct
#define Template ArrayInit
 Array(T) _template_(size_t capacity, bool auto_release)
{
	 Array(T) A = MALLOC(_class_struct_, 1);
	array_init(T, A, capacity);
	if (auto_release) AutoRelease(A, _TypedVar_(ArrayRelease, T), false);
	return A;
}

//public destruct
#define Template ArrayRelease
void _template_(Array(T) A)
{
	free(A->m_data);
	free(A);
}

//private
#define Template array_memory_set
#define array_memory_set(T, A, size) _TypedVar_(array_memory_set, T)(A, size)
void _template_(Array(T) A, size_t new_size)
{
	T* new_base = REALLOC(T, A->m_data, new_size);
	if (new_base != NULL)
	{
		A->m_capacity = new_size;
		A->m_data = new_base;
		A->m_data_end = A->m_data + A->m_capacity;
		if (new_size < A->m_capacity) A->m_size = A->m_capacity;
	}
	else
		ET(OverFlow, "remalloc fail", );
}


//public
#define Template ArrayCapacitySet
void _template_(Array(T) A, size_t new_size)
{
	array_memory_set(T, A, new_size);
	A->m_end = A->m_data + A->m_size;
}


//private
#define Template array_extend
#define array_extend(T, A) _TypedVar_(array_extend, T)(A)
void _template_(Array(T) A)
{
	ArrayCapacitySet(T, A, A->m_capacity << 1);
}


//public
#define Template ArrayPushBack
void _template_(Array(T) A, const T val)
{
	if (A->m_size == A->m_capacity)
	{
		array_extend(T, A);
	}
	*(A->m_end) = val;
	A->m_end++;
	A->m_size++;
}


//public
#define Template ArrayPopBack
void _template_(Array(T) A)
{
	A->m_size--;
	A->m_end--;
	CHECK_RANGE(A);
}

//public
#define Template ArrayResize
void _template_(Array(T) A, size_t new_size)
{
	if (new_size > A->m_capacity || new_size <= A->m_capacity / 2)
		ArrayCapacitySet(T, A, new_size);
	A->m_size = new_size;
}

#define Template ArrayClear
void _template_(Array(T) A)
{
	A->m_size = 0;
	A->m_end = A->m_data;
}


//public
#define Template ArrayForEach
void _template_(Array(T) A, Status visit(T* val))
{
	for (int i = 0; i < A->m_size; i++)
	{
		if (visit(&A->m_data[i]) == Break) break;
	}
}



#define _class_ LoopArray(T)

#define m_data_ self->m_data
#define m_data_end_ self->m_data_end
#define m_size_ self->m_size
#define m_capacity_ self->m_capacity
#define m_begin_ self->m_begin
#define m_end_ self->m_end

//public construct fucntion
#define Template LoopArrayInit
LoopArray(T) _template_(size_t capacity, bool auto_release)
{
	LoopArray(T) loop = MALLOC(_class_struct_, 1);
	array_init(T, (Array(T)) loop, capacity);
	loop->m_begin = loop->m_data;
	loop->m_end = loop->m_data;
	if (auto_release) AutoRelease(loop, _TypedVar_(LoopArrayRelease,T), false);
	return loop;
}

//public destruct function
#define Template LoopArrayRelease
void _template_(_class_ self)
{
	free(self->m_data);
	free(self);
}


//public
#define Template LoopArrayForward
T* _template_(_class_ self, T* ptr)
{
	ptr++;
	if (ptr == m_data_end_)
		ptr = m_data_;
	return ptr;
}


//public
#define Template LoopArrayBackward
T* _template_(_class_ self, T* ptr)
{
	if (ptr == m_data_)
		ptr = m_data_end_;
	ptr--;
	return ptr;
}


//public
#define Template LoopArrayIncrease
void _template_(_class_ self, T** ptr)
{
	(*ptr)++;
	if (*ptr == m_data_end_)
		*ptr = m_data_;
}


//public
#define Template LoopArrayDecrease
void _template_(_class_ self, T** ptr)
{
	if (*ptr == m_data_)
		*ptr = m_data_end_;
	(*ptr)--;
}


//private
#define Template loopArray_capacity_set
#define loopArray_capacity_set(T, LA, size) _TypedVar_(loopArray_capacity_set, T)(LA, size)
void _template_(_class_ self, size_t new_size)
{
	if (new_size >= m_capacity_)
	{
		uint64_t begin_offset = m_begin_ - m_data_;
		uint64_t end_offset = m_end_ - m_data_;
		size_t p_size = m_size_;
		array_memory_set(T, (Array(T))self, new_size);
		m_begin_ = m_data_ + begin_offset;
		m_end_ = m_data_ + end_offset;
		//将后半部分移动到末端
		//data---end----begin---data end
		//0 1 2 3 4 5 6 7  8 size=5
		// |     |    	  |         |
		//      +2             +3
		if (m_begin_ >= m_end_)
		{
			size_t len1 = m_end_ - m_data_;
			size_t len2 = p_size - len1;
			memmove(m_data_end_ - len2, m_begin_, len2 * sizeof(T));
			m_begin_ = m_data_end_ - len2;
		}
	}
	else//缩容
	{
		T* tempArray = MALLOC(T, new_size);
		T* p = m_begin_;
		for (int i = 0; i < new_size; i++)
		{
			tempArray[i] = *p;
			LoopArrayIncrease(T, self, p);
		}
		free(m_data_);
		m_data_ = tempArray;
		m_capacity_ = new_size;
		m_data_end_ = m_data_ + m_capacity_;
		m_size_ = new_size;
		m_begin_ = m_data_;
		m_end_ = m_data_ + new_size;
	}
}


//private
#define Template loopArray_extend
#define loopArray_extend(T, LA) _TypedVar_(loopArray_extend, T)(LA)
void _template_(_class_ self)
{
	loopArray_capacity_set(T, self, m_capacity_ << 1);
}


//public function
#define Template LoopArrayPushBack
void _template_(_class_ self, T val)
{
	if (m_size_ == m_capacity_ - 1) loopArray_extend(T, self);
	*m_end_ = val;
	LoopArrayIncrease(T, self, m_end_);
	m_size_++;
}


//public
#define Template LoopArrayPopBack
void _template_(_class_ self)
{
	if (m_size_ == 0)
		ET(OutOfRange, "empty array", );
	LoopArrayDecrease(T, self, m_end_);
	m_size_--;
}


//public
#define Template LoopArrayPushFront
void _template_(_class_ self, T val)
{
	if (m_size_ == m_capacity_ - 1) loopArray_extend(T, self);
	LoopArrayDecrease(T, self, m_begin_);
	*m_begin_ = val;
	m_size_++;
}


//public
#define Template LoopArrayPopFront
void _template_(_class_ self)
{
	CHECK(if (m_size_ == 0)
		ET(OutOfRange, "empty array", ););
	LoopArrayIncrease(T, self, m_begin_);
	m_size_--;
}



#undef m_data_
#undef m_data_end_
#undef m_size_
#undef m_capacity_
#undef m_begin_
#undef m_end_

#include "TypedDefineEnd.h"
