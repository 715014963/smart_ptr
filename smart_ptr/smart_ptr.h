#pragma once
#include <map>

// ģ��shared_ptrʵ��һ������ָ��
template <typename T>
class smart_ptr
{
public:
	smart_ptr();
	~smart_ptr();

private:
	unsigned m_use_count = 0;
	T* m_pobject = nullptr;
};

template <typename T>
smart_ptr::smart_ptr()
{
}

template <typename T>
smart_ptr::~smart_ptr()
{
	if (m_use_count == 0 && m_pobject)
	{
		delete m_pobject;
		m_pobject = nullptr;
	}
}
