#ifndef __CC_WEAK_PTR_H__
#define __CC_WEAK_PTR_H__

#include "base/CCRef.h"
#include "base/ccMacros.h"
#include <functional>
#include <type_traits>

NS_CC_BEGIN

template <typename T>
class WeakPtr
{
public:
    WeakPtr()
        :_counter(nullptr)
    {
    }

    WeakPtr(WeakPtr<T> && other)
    {
        _counter = other._counter;
        other._counter = nullptr;
    }

    WeakPtr(T* ptr)
    {
        _counter = ptr ? ptr->getWeakPtrCounter() : nullptr;
        CC_SAFE_RETAIN(_counter);
    }

    WeakPtr(std::nullptr_t ptr)
        :_counter(nullptr)
    {
    }

    WeakPtr(const WeakPtr<T> & other)
        : _counter(other._counter)
    {
        CC_SAFE_RETAIN(_counter);
    }

    ~WeakPtr()
    {
		CC_SAFE_RELEASE(_counter);
    }

    WeakPtr<T>& operator = (const WeakPtr<T>& other)
    {
        if (other._counter != _counter)
        {
            CC_SAFE_RELEASE(_counter);
            _counter = other._counter;
            CC_SAFE_RETAIN(_counter);
        }
        
        return *this;
    }

    WeakPtr<T>& operator = (WeakPtr<T>&& other)
    {
        if (&other != this)
        {
            CC_SAFE_RELEASE(_counter);
            _counter = other._counter;
            other._counter = nullptr;
        }
        
        return *this;
    }

    WeakPtr<T>& operator = (T* other)
    {
        if (!other)
        {
            CC_SAFE_RELEASE_NULL(_counter);
        }
        else if (_counter != other->getWeakPtrCounter())
        {
            CC_SAFE_RELEASE(_counter);
            _counter = other->getWeakPtrCounter();
            CC_SAFE_RETAIN(_counter);
        }
        
        return *this;
    }

    WeakPtr<T> & operator = (std::nullptr_t other)
    {
        CC_SAFE_RELEASE_NULL(_counter);
        return *this;
    }

    operator T* () const
    {
        return _counter ? static_cast<T*>(_counter->_target) : nullptr;
    }
    
    T& operator * () const
    {
        CCASSERT(_counter && _counter->_target, "Attempt to dereference a null pointer!");
        return *static_cast<T*>(_counter->_target);
    }
    
    T* operator->() const
    {
        CCASSERT(_counter && _counter->_target, "Attempt to dereference a null pointer!");
        return static_cast<T*>(_counter->_target);
    }
    
    T* get() const
    {
        return _counter ? static_cast<T*>(_counter->_target) : nullptr;
    }

    bool operator == (const WeakPtr<T>& other) const { return get() == other.get(); }
    
    bool operator == (const T* other) const { return get() == other; }
    
    bool operator == (typename std::remove_const<T>::type* other) const { return get() == other; }
    
    bool operator == (const std::nullptr_t other) const { return get() == other; }
    
    
    bool operator != (const WeakPtr<T>& other) const { return get() != other.get(); }
    
    bool operator != (const T* other) const { return get() != other; }
    
    bool operator != (typename std::remove_const<T>::type* other) const { return get() != other; }
    
    bool operator != (const std::nullptr_t other) const { return get() != other; }
    
    
    bool operator > (const WeakPtr<T>& other) const { return get() > other.get(); }
    
    bool operator > (const T* other) const { return get() > other; }
    
    bool operator > (typename std::remove_const<T>::type* other) const { return get() > other; }
    
    
    bool operator < (const WeakPtr<T>& other) const { return get() < other.get(); }
    
    bool operator < (const T* other) const { return get() < other; }
    
    bool operator < (typename std::remove_const<T>::type* other) const { return get() < other; }
    
        
    bool operator >= (const WeakPtr<T>& other) const { return get() >= other.get(); }
    
    bool operator >= (const T* other) const { return get() >= other; }
    
    bool operator >= (typename std::remove_const<T>::type* other) const { return get() >= other; }
    
        
    bool operator <= (const WeakPtr<T>& other) const { return get() <= other.get(); }
    
    bool operator <= (const T* other) const { return get() <= other; }
    
    bool operator <= (typename std::remove_const<T>::type* other) const { return get() <= other; }

    operator bool() const { return get() != nullptr; }
        
    void reset()
    {
        CC_SAFE_RELEASE_NULL(_counter);
    }
        
    void swap(WeakPtr<T>& other)
    {
        if (&other != this)
        {
            Ref::WeakPtrCounter* tmp = _counter;
            _counter = other._counter;
            other._counter = tmp;
        }
    }

private:
	Ref::WeakPtrCounter* _counter;

    /*
    Note: Removed the following assertion because it blocks class T from holding a WeakPtr<T>, e.g.
    class T : public cocos2d::Ref {cocos2d::WeakPtr<T> _member;}; // does not compile
    */
    // static_assert(std::is_base_of<Ref, typename std::remove_const<T>::type>::value, "T must be derived from Ref");
};

template <class T> inline
WeakPtr<T> makeWeak(T* ptr)
{
    return WeakPtr<T>(ptr);
}

template<class T> inline
bool operator<(const WeakPtr<T>& r, std::nullptr_t)
{
    return std::less<T*>()(r.get(), nullptr);
}

template<class T> inline
bool operator<(std::nullptr_t, const WeakPtr<T>& r)
{
    return std::less<T*>()(nullptr, r.get());
}

template<class T> inline
bool operator>(const WeakPtr<T>& r, std::nullptr_t)
{
    return nullptr < r;
}

template<class T> inline
bool operator>(std::nullptr_t, const WeakPtr<T>& r)
{
    return r < nullptr;
}

template<class T> inline
bool operator<=(const WeakPtr<T>& r, std::nullptr_t)
{
    return !(nullptr < r);
}

template<class T> inline
bool operator<=(std::nullptr_t, const WeakPtr<T>& r)
{
    return !(r < nullptr);
}

template<class T> inline
bool operator>=(const WeakPtr<T>& r, std::nullptr_t)
{
    return !(r < nullptr);
}

template<class T> inline
bool operator>=(std::nullptr_t, const WeakPtr<T>& r)
{
    return !(nullptr < r);
}

/**
 * Cast between WeakPtr types statically.
 */
template<class T, class U> WeakPtr<T> static_pointer_cast(const WeakPtr<U>& r)
{
    return WeakPtr<T>(static_cast<T*>(r.get()));
}

/**
 * Cast between WeakPtr types dynamically.
 */
template<class T, class U> WeakPtr<T> dynamic_pointer_cast(const WeakPtr<U>& r)
{
    return WeakPtr<T>(dynamic_cast<T*>(r.get()));
}

NS_CC_END

/// @endcond
#endif 
