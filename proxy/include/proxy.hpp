/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>

template<class T>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}
 
    //{ describe proxy object
    class proxy: private std::lock_guard<std::mutex>
    {
    public:
        proxy(T* i_data, std::mutex& i_mutex): std::lock_guard<std::mutex>(i_mutex), m_data(i_data)
        {}
 
        T* operator -> () const {
            return m_data;
        }
 
    private:
        T* m_data;
    };
 
    proxy operator -> () const
    {
        return {ptr_, mutex_};
    }
    //}
 
private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
