#pragma once 
// 常见的 预处理命令在头文件的最开始加入这条预处理指令，保证头文件只编译一次
// 还有一种方式 #ifndef #define #endif
// 区别是 #pragma once 指物理上的文件 #ifndef 指相同宏的文件
#include <functional>
// 一个头文件， 包含函数对象相关内容。
template <typename T>
class smart_ptr {
    public:
        smart_ptr();
        explicit smart_ptr(T*); 
        // explicit 唯一功能 声明构造函数要显式出现
        smart_ptr(const smart_ptr&);
        smart_ptr(T*, std::function<void(T*)>);
        // 表明传入一个函数对象，参数是一个返回 void（T*） 对象的函数
        smart_ptr& operator=(const smart_ptr&);
        T& operator*() const;
        // 尾部加上const ，表示不可修改对象，也不能调用非const函数防止修改对象。
        T* operator->() const;

        ~smart_ptr();
        explicit operator bool() const;

        bool unique();
        void reset();
        void reset(T*);
        void reset(T*, std::function<void(T*)>);
        T* release();
        T* get() const;

    private:
        static std::function<void(T*)> default_del;
        // 默认的删除器，是一个静态函数对象，比任何类的对象都先存在

        unsigned *m_p_use_count = nullptr; // 指针指向引用变量
        T* m_pt_object = nullptr;// 要管理的指针
        std::function<void(T*)> m_del = default_del; // 私有删除器



};

// 删除器 释放资源，重新指向nullptr
template <typename T> 
std::function<void(T*)> smart_ptr<T>::default_del = [](T* p) {delete p; p = nullptr;};

// 可变参数模板 c++11 typename / class 后跟 ... Args 变量可叫参数包
// 取参数只能展开
// template <class... T>
// void f(T... args)
// {    
//     cout << sizeof...(args) << endl; //打印变参的个数
// }

// f();        //0
// f(1, 2);    //2
// f(1, 2.5, "");    //3
template<typename T, typename ...Args>
smart_ptr<T> make_smart(Args &&... args) 
{
    smart_ptr<T> sp(new T(std::forward<Args>(args)...));
    return sp;
}
// c++ 11 逗号表达式，可以用来展开参数包。
// 加了...，就会依次展开参数包 
// 完美转发 forward c++ 11 ，可以理解为不断传入args参数包中的值


// 基础的构造函数 赋值构造函数 重载 = 构造函数 和 特有的加删除器的
template <typename T>
smart_ptr<T>::smart_ptr()
    : m_pt_object(nullptr), m_p_use_count(new unsigned(1)) 
{

}

template <typename T>
smart_ptr<T>::smart_ptr(T* p) 
    : m_pt_object(p), m_p_use_count(new unsigned(1))
{

}

template <typename T>
smart_ptr<T>::smart_ptr(T *p, std::function<void(T*)> del)
	:m_pt_object(p), m_p_use_count(new unsigned(1)), m_del(del)
{

}

template <typename T>
smart_ptr<T>::smart_ptr(const smart_ptr& rhs)
	:m_pt_object(rhs.m_pt_object), m_p_use_count(rhs.m_p_use_count), m_del(rhs.m_del)
{
	(*m_p_use_count)++;
}

template <typename T> 
smart_ptr<T> &smart_ptr<T>::operator=(const smart_ptr &rhs)  
{
    m_del = rhs.m_del;
    ++(*rhs.m_p_use_count);

    if (--(*m_p_use_count) == 0) {
        m_del(m_pt_object);
        delete m_p_use_count;
    } // 如果已经管理对象，则进行一次判断之前的管理对象
    m_p_use_count = rhs.m_p_use_count;
    m_pt_object = rhs.m_pt_object;
    return *this;
}

// 返回 smart_ptr 管理的对象
template <typename T>
T& smart_ptr<T>::operator*() const
{
    return *m_pt_object;
}

// 返回指向管理对象的值 ？ 
template <typename T>
T* smart_ptr<T>::operator->() const // 不全面 ？
{
	return &this->operator*();
}

template <typename T>
smart_ptr<T>::~smart_ptr()
{
	if (--(*m_p_use_count) == 0) // 如果会为 0 ， 则消解
	{
		m_del(m_pt_object);
		m_pt_object = nullptr;

		delete m_p_use_count; // delete 删的资源 而不是指针, 指针由栈 、 编译器管理
		m_p_use_count = nullptr;
	}
}

template <typename T>
bool smart_ptr<T>::unique()
{
	return *m_p_use_count == 1; // 计数 1 则唯一指向
}

template <typename T> 
void smart_ptr<T>::reset() 
{
    (*m_p_use_count)--;
    if (*m_p_use_count == 0) {
        m_del(m_pt_object);
    }
    m_pt_object = nullptr;
    *m_p_use_count = 1;  // 应该错了
    m_del = default_del;

}

template <typename T>
void smart_ptr<T>::reset(T* p)
{
	(*m_p_use_count)--;

	if (*m_p_use_count == 0)
	{
		m_del(m_pt_object);
	}

	m_pt_object = p;
	*m_p_use_count = 1;
	m_del = default_del;
}

template <typename T>
void smart_ptr<T>::reset(T *p, std::function<void(T*)> del)
{
	reset(p); // 体现了重用原则
	m_del = del;
}

template <typename T>
T* smart_ptr<T>::release()
{
	(*m_p_use_count)--;

	if (*m_p_use_count == 0)
	{
		*m_p_use_count = 1;
	}

	auto p = m_pt_object;
	m_pt_object = nullptr;

	return p;
}

template <typename T>
T* smart_ptr<T>::get() const // const 修饰细节
{
	return m_pt_object;
}

template <typename T>
smart_ptr<T>::operator bool() const
{
	return m_pt_object != nullptr;
}






    




