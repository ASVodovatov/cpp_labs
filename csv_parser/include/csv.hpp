/* CSV parser.
 * @file
 * @date 2018-08-15
 * @author Anonymous
 */

#ifndef __CSV_HPP__
#define __CSV_HPP__

#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include "parser.hpp"
#include "quoted_string.hpp"

/* An example of CSV:
 *      kind,of,header
 *      abc,with space,"quote"
 *      "comma , inside",132,  spaces  dot
 *      empty,,
 *
 * CSV (comma separated value) EBNF specification (http://www.rfc-editor.org/rfc/rfc4180.txt)
 *      string := [^,\n]+
 *      quoted_string := " [^"]* "
 *      cell := quated_string | string
 *      row := cell (, cell)* \n
 *      header := row
 *      csv := header row*
 */

namespace types
{
    namespace csv
    {
        //? Which kind of types should I use to describe the CSV type?
        //{
        using csv = std::vector<std::vector<std::string>>;
        //}
    }
}

namespace parser
{
    namespace csv
    {
        //? Why I need a x3::no_skip here? Where is the original of the error?
        //? Where is BOOST_SPIRIT_DEFINE? Is it necessary?
        namespace x3 = boost::spirit::x3;

        //{ csv grammar
        auto string = x3::rule<class string, std::string>{} = x3::lexeme[+(x3::char_ - (x3::lit(',') | '"' | '\n') )];
        auto cell = x3::rule<class cell, std::string>{} = quoted_string | string;

        auto row = x3::rule<class row, std::vector<std::string>>{} = (cell % ',') >> -x3::no_skip["\n"];
        auto header = x3::rule<class row, std::vector<std::string>>{} = row;
        auto csv = x3::rule<class csv, types::csv::csv>{} = header >> *row;

        //BOOST_SPIRIT_DEFINE(csv);
        //}
    }
}

namespace literals
{
    namespace csv
    {
        //{ declare ``_csv`` literal

        types::csv::csv operator""_csv(const char* data, size_t)
        {
            return parser::load_from_string<types::csv::csv>(data, parser::csv::csv);
        }
        //}
        
    }
}

#endif // __CSV_HPP__
