/* Test file for strided_range task.
 * @file
 * @date 2019-08-29
 * @author Anonymous
 */

#include <boost/range/iterator_range.hpp>
#include <vector>

#include <catch2/catch.hpp>

#include <strided_range.hpp>

// Two checks with different strides.
TEST_CASE("strided_range::stride")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	SECTION("stride = 7 width = 5")
	{
		auto a = make_strided_range(vec, 5, 7);
		CHECK(a == std::vector{1, 2, 3, 4, 5, 7, 8, 9, 1});
	}
	SECTION("stride = 4 width = 5")
	{
		auto a = make_strided_range(vec, 5, 4);
		CHECK(a == std::vector{1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1});
	}
}

// Two checks with different widths.
TEST_CASE("strided_range::width")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	SECTION("stride = 4 width = 2")
	{
		auto a = make_strided_range(vec, 2, 4);
		CHECK(a == std::vector{1, 2, 5, 0, 8, 9});
	}

	SECTION("stride = 4 width = 3")
	{
		auto a = make_strided_range(vec, 3, 4);
		CHECK(a == std::vector{1, 2, 3, 5, 0, 6, 8, 9, 1});
	}

}

TEST_CASE("strided_range::stride_equal_width")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	SECTION("stride = 5 width = 5")
	{
		auto a = make_strided_range(vec, 5, 5);
		CHECK(a == std::vector{1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1});
	}

}

// Use boost::size.
TEST_CASE("strided_range::distance")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	SECTION("stride = 5 width = 5")
	{
		auto a = make_strided_range(vec, 5, 5);
		CHECK(boost::size(a) == 11);
	}
	SECTION("stride = 4 width = 5")
	{
		auto a = make_strided_range(vec, 5, 4);
		CHECK(boost::size(a) == 11);
	}
	SECTION("stride = 4 width = 2")
	{
		auto a = make_strided_range(vec, 2, 4);
		CHECK(boost::size(a) == 6);
	}
}

// Make strided_range from another strided_range.
TEST_CASE("strided_range::from_strided")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	SECTION("stride = 4 width = 2")
	{
		auto a = make_strided_range(vec, 2, 4);
		auto b = make_strided_range(a, 2, 4);
		CHECK(b == std::vector<int>({1, 2, 8, 9}));
	}
}

// Provide example with shifted data. It's like a window in the image. Hint: boost::make_iterator_range.
TEST_CASE("strided_range::shift")
{
	int image_stride = 8;
	int image_width = 6;

	std::vector<int> vec =
	{ //---width---— -stride-
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 0, 0,
	0, 0, 5, 6, 7, 8, 0, 0,
	0, 0, 9, 10, 11, 12, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
	};

	auto image_strided = make_strided_range(vec, image_width, image_stride);

	int x0 = 2;
	int y0 = 1;

	int window_width = 4;
	int window_height = 3;

	auto window_begin = image_strided.begin();
	std::advance(window_begin, x0 + y0 * image_width);
	auto window_end = image_strided.begin();
	std::advance(window_end, x0 + y0 * image_width + window_width + (window_height-1) * image_width);

	auto window_not_straded_range = boost::make_iterator_range(window_begin, window_end);

	auto window_range = make_strided_range(window_not_straded_range, window_width, image_width);

	SECTION("window")
	{	
		CHECK(window_range == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12}));
	}
}

TEST_CASE("strided_range::corner_case1")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	CHECK(boost::size(make_strided_range(vec, 0, 4)) == 0);
}

TEST_CASE("strided_range::corner_case2")
{
	std::vector<int> vec = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 1};
	CHECK(boost::size(make_strided_range(vec, 0, -1)) == 0);
}