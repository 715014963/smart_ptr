#pragma once
#include <map>

// �Լ�ģ��shared_ptrʵ��һ������ָ��
template <typename T>
class smart_ptr
{
public:
	smart_ptr();
	~smart_ptr();

private:
	unsigned m_use_count = 0;
};

template <typename T>
smart_ptr::smart_ptr()
{
}

template <typename T>
smart_ptr::~smart_ptr()
{
}