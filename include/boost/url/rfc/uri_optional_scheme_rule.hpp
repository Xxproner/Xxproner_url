//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/url
//

#ifndef BOOST_URL_RFC_URI_OPTIONAL_SCHEME_RULE_HPP
#define BOOST_URL_RFC_URI_OPTIONAL_SCHEME_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error_types.hpp>
#include <boost/url/url_view.hpp>

namespace boost {
namespace urls {

namespace implementation_defined {

struct strequal_rule_t
{
    using value_type = core::string_view;

    strequal_rule_t(value_type);

    auto 
    parse(
        char const*& it,
        char const* end
            ) const noexcept ->
        system::result<value_type>;
private:
    const value_type str;
};


struct uri_optional_scheme_rule_t
{
    using value_type = url_view;

    BOOST_URL_DECL
    auto
    parse(
        char const*& it,
        char const* end
            ) const noexcept ->
        system::result<value_type>;
};
} // implementation_defined

/** Rule for URI-optional-scheme

    @par Value Type
    @code
    using value_type = url_view;
    @endcode

    @par Example
    Rules are used with the function @ref grammar::parse.
    @code
    system::result< url_view > rv = grammar::parse( "example.com/index.html?id=1", uri_optional_scheme_rule );
    @endcode

    @par BNF
    @code

    URI-optional-scheme = [ scheme "://" ] authority md-hier-part [ "?" query ] [ "#" fragment]

    md-hier-part    = path-abempty
                    / path-absolute
                    / path-empty
    @endcode

    @par Specification
        rfc extension

    @see
        @ref grammar::parse,
        @ref url_view.
*/
BOOST_INLINE_CONSTEXPR implementation_defined::uri_optional_scheme_rule_t uri_optional_scheme_rule{};

} // urls
} // boost

#endif
