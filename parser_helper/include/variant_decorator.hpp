/* Type decorator for variant.
 * @file
 * @date 2018-08-15
 * @author Anonymous
 */

#ifndef __VARIANT_DECORATOR_HPP__
#define __VARIANT_DECORATOR_HPP__

#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace x3 = boost::spirit::x3;

//? How to inherit x3::variant constructor & ``operator=``?
//? Why I need inherited constructor & ``operator=``?
//? How many constructors did I inherit?
//? Do I need my own constructor & ``operator=``? Why?
//{
template<typename... Types>
struct variant_decorator : public x3::variant<Types...>
{
    using x3::variant<Types...>::operator=;
    using x3::variant<Types...>::variant;
    //? Do I need a ``const``-version?
    template<class T>
    T& as()
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<T>, Types>...>)
        {
            return boost::get<boost::recursive_wrapper<T>>(*this).get();
        }
        else
        {
            return boost::get<T>(*this);
        }
    }

    template<class T>
    T as() const
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<T>, Types>...>)
        {
            return boost::get<boost::recursive_wrapper<T>>(*this).get();
        }
        else
        {
            return boost::get<T>(*this);
        }
    }
};

#endif // __VARIANT_DECORATOR_HPP__
