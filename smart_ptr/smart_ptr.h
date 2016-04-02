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
	smart_ptr& operator=(const smart_ptr&);

	~smart_ptr();
	// ��bool������ת��
	explicit operator bool() const

	T* get();

private:
	unsigned* m_use_count = nullptr;
	T* m_pobject = nullptr;
};

template <typename T>
smart_ptr::smart_ptr()
	:m_pobject(nullptr), m_use_count(new unsigned(1))
{
}


template <typename T>
smart_ptr::smart_ptr(T*p)
	:m_pobject(p), m_use_count(new unsigned(1))
{
}


template <typename T>
smart_ptr::smart_ptr(const smart_ptr& rhs)
	:m_pobject(rhs.m_pobject), m_use_count(rhs.m_use_count)
{
}


template <typename T>
smart_ptr& smart_ptr::operator =(const smart_ptr &rhs)
{
	// �����Ҳ������������ü���
	++*rhs.m_use_count;
	// �ݼ�����������ü���
	if (--*m_use_count == 0)
	{
		// �������Ķ���û�������û��ˣ����ͷŶ������ĳ�Ա
		delete m_pobject;
		delete m_use_count;
	}

	m_use_count = rhs.m_use_count;
	m_pobject = rhs.m_pobject;

	return *this; // ���ر�����
}


template <typename T>
smart_ptr::~smart_ptr()
{
	if (--(*m_use_count) == 0)
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


template <typename T>
smart_ptr::bool()
{
	return m_pobject != nullptr;
}

