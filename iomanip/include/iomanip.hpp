/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__
#include <iostream>
//{ endm manipulator
inline std::ostream& endm(std::ostream& os)
{
	return os << "[eol]\n";
}
//}

//{ squares manipulator
struct squares_manip 
{
    squares_manip(std::ostream & os_):os(os_){}

    template<typename T>
    friend std::ostream& operator<<(squares_manip const& s, T const& rhs) 
	{
        return s.os << "[" << rhs << "]";
    }

private:
    std::ostream & os;
};

inline struct squares_creator {} squares;

inline squares_manip operator<<(std::ostream & os, squares_creator) 
{
    return squares_manip(os);
}
//}

//{ add manipulator
inline struct add_manip 
{
	struct first_arg 
	{
		std::ostream& os;
	};

	friend first_arg operator<< (std::ostream& os, add_manip)
	{
		return {os};
	}

	template<typename T>
	struct second_arg 
	{
		std::ostream& os;
		T value;
	};

	template<typename T>
	friend second_arg<T> operator<< (first_arg fst, T&& fstvalue)
	{
		return {fst.os, fstvalue};
	}
} add;

template<typename T>
std::ostream& operator<< (add_manip::second_arg<T> snd, T&& sndvalue) 
{
	return snd.os << (snd.value + sndvalue);
}
//}

#endif // __IOMANIP_HPP__
