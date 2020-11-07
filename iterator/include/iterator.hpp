/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

//{ image iterator
template<class Iterator>
class image_iterator: public boost::iterator_adaptor<image_iterator<Iterator>, Iterator>
{
    using ItAdaptor = boost::iterator_adaptor<image_iterator<Iterator>, Iterator>;
public:
    image_iterator(Iterator i_iterator, size_t i_width, size_t i_stride) : 
		ItAdaptor(i_iterator), 
        m_width(i_width), 
        m_stride(i_stride) 
    {   
    }
private:
	size_t m_width = 0;
    size_t m_stride = 0;
    ptrdiff_t m_currentPos = 0;

    friend class boost::iterator_core_access;
    typename ItAdaptor::reference dereference() const
    {
        return *this->base_reference();
    }

    void advance(typename ItAdaptor::difference_type n)
    {
        ptrdiff_t numRows = (m_currentPos + n) / ptrdiff_t(m_width);
        m_currentPos += n;
        if (m_currentPos < 0)
        {
            --numRows;
        }
        
		m_currentPos %= m_width;
        this->base_reference() += n + numRows * ptrdiff_t(m_stride - m_width);
    }

    void increment()
    {
        if (++m_currentPos >= ptrdiff_t(m_width))
        {
            m_currentPos = 0;
            this->base_reference() += m_stride - m_width + 1;
        }
        else
        {
            ++this->base_reference();
        }
    }

    void decrement()
    {
        if (--m_currentPos < 0)
        {
            m_currentPos = m_width - 1;
            this->base_reference() -= (m_stride - m_width + 1);
        }
        else
        {
            --this->base_reference();
        }
    }

    typename ItAdaptor::difference_type distance_to(image_iterator const& other) const
    {
        ptrdiff_t rawDistance = other.base() - this->base() - other.m_currentPos + m_currentPos;
        return other.base() - this->base() - (rawDistance / ptrdiff_t(m_stride)) * (m_stride - m_width);
    }

    bool equal(ItAdaptor const& other) const
    {
        return this->base() == other.base();
    }    
};
//}

template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
        data(stride * height),
        width_(width),
        stride_(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end()
    {
        return image_iterator(data.end(), width_, stride_);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end() const
    {
        return image_iterator(data.end(), width_, stride_);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
    size_t width_;
    size_t stride_;
};

#endif // __ITERATOR_HPP__
