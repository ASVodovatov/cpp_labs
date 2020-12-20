/* Variant classes.
 * @file
 * @date 2018-08-12
 * @author Anonymous
 */

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>

#include <boost/variant.hpp>

//{ number
struct number : public std::variant<int,float>
{
    using std::variant<int,float>::variant;
};
//}

//{ array
struct array : public std::vector<number>
{
    using std::vector<number>::vector;
};
//}

//{ recursive_array
struct recursive_array : public std::vector<std::variant<number,std::shared_ptr<recursive_array>>>
{
    using std::vector<std::variant<number,std::shared_ptr<recursive_array>>>::vector;
};
//}

//{ recursive_array2
struct recursive_array2 : public std::vector<std::variant<number,boost::recursive_wrapper<recursive_array2>>>
{
    using std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>::vector;
};
//}

//{ variant_decorator
template<typename... Types>
struct variant_decorator : public std::variant<Types...>
{
    using std::variant<Types...>::variant;  

    template<typename R> 
    R& as()
    { 
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<R>, Types>...>)
        {
            return std::get<boost::recursive_wrapper<R>>(*this).get();
        }
        else
        {
            return std::get<R>(*this);
        }
    }

    
    template<typename R>
    R as() const
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<R>, Types>...>)
        {
            return std::get<boost::recursive_wrapper<R>>(*this).get();
        }
        else
        {
            return std::get<R>(*this);
        }
    }
};
//}

//{ recursive_map
struct recursive_map : public std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>
{
    using std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>::map;
};
//}

#endif // __VARIANT_HPP__
