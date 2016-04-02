#pragma once
#include <map>

// ģ��shared_ptrʵ��һ������ָ��
template <typename T>
class smart_ptr
{
public:
	smart_ptr();
	explicit smart_ptr(T*);
	smart_ptr(const smart_ptr&);

	~smart_ptr();

	T* get();

private:
	unsigned* m_use_count = nullptr;
	T* m_pobject = nullptr;
};

template <typename T>
smart_ptr::smart_ptr()
	:m_pobject(nullptr), m_use_count(new unsigned(0))
{
}


template <typename T>
smart_ptr::smart_ptr(T*p)
	:m_pobject(p), m_use_count(new unsigned(1))
{
}


template <typename T>
smart_ptr::smart_ptr(const smart_ptr& rhs)
{
	// �ݼ�������
	if (*m_use_count > 0)
	{
		*m_use_count--;
	}
	if (*m_use_count == 0)
	{
		delete m_pobject;
	}

	// �����Ҳ����
	*(rhs.m_use_count)++;

	m_pobject = rhs.m_pobject;
	m_use_count = rhs.m_use_count;
}


template <typename T>
smart_ptr::~smart_ptr()
{
	if (m_use_count > 0)
	{
		m_use_count--;
	}

	if (m_use_count == 0 && m_pobject)
	{
		delete m_pobject;
		m_pobject = nullptr;
	}
}


template <typename T>
T* smart_ptr::get()
{
	return m_pobject;
}
