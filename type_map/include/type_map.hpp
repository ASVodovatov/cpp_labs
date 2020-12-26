/* Map of types.
 * @file
 * @date 2018-08-25
 * @author Anonymous
 */

#ifndef __TYPE_MAP_HPP__
#define __TYPE_MAP_HPP__

#include <array>
#include <tuple>

namespace detail
{
    template<class T, class Tuple>
    struct type_to_index;

    template<class T, class... Ts>
    struct type_to_index<T, std::tuple<T, Ts...>>
    {
        constexpr static size_t v = 0;
    };

    template<class T, class U, class... Ts>
    struct type_to_index<T, std::tuple<U, Ts...>>
    {
        constexpr static size_t v = 1 + type_to_index<T, std::tuple<Ts...>>::v;
    };
}

//? What is wrong with ``*this[i]``?
//{
template<typename value_type, typename... Types>
class type_map : public std::array<value_type, sizeof...(Types)>
{	
public:
    using types = value_type;
 
    template<class T>
    value_type& as()
    {
        return this->at(detail::type_to_index<T,std::tuple<Types...>>::v);
    }

    template<class T>
    value_type as() const
    {
        return this->at(detail::type_to_index<T, std::tuple<Types...>>::v);
    }
};
//}

namespace std
{
    template<typename T, class TypeMap>
    typename TypeMap::types get(const TypeMap& tm)
    {
        //{ How to call ``as``?
        return tm.template as<T>();
        //}
    }

    template<typename T, class TypeMap>
    typename TypeMap::types& get(TypeMap& tm)
    {
        //{ How to call ``as``?
        return tm.template as<T>();
        //}
    }
}

#endif // __TYPE_MAP_HPP__
