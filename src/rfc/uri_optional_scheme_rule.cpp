//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/url
//


#include <boost/url/detail/config.hpp>
#include "boost/url/rfc/detail/path_rules.hpp"
#include <boost/url/rfc/uri_optional_scheme_rule.hpp>
#include <boost/url/grammar/delim_rule.hpp>
#include <boost/url/grammar/tuple_rule.hpp>
#include <boost/url/grammar/optional_rule.hpp>
#include <boost/url/grammar/parse.hpp>
#include "detail/fragment_part_rule.hpp"
#include "detail/hier_part_rule.hpp"
#include "detail/query_part_rule.hpp"
#include "detail/scheme_rule.hpp"
#include <utility>

namespace boost {
namespace urls {


implementation_defined::strequal_rule_t::
strequal_rule_t(value_type str_)
    : str(str_)
{
}

auto
implementation_defined::strequal_rule_t::
parse(
    char const*& it,
    char const* const end
        ) const noexcept ->
    system::result<value_type>
{
    if ((it - end) >= str.length())
    {
        if (str == value_type(it, 3))
        {
            return str;
        }
    }

    return error::non_canonical;
}

auto
implementation_defined::uri_optional_scheme_rule_t::
parse(
    char const*& it,
    char const* const end
        ) const noexcept ->
    system::result<value_type>
{
    detail::url_impl u(detail::url_impl::from::string);
    u.cs_ = it;

    // [ scheme : "//" ]
    {
        auto optional_scheme_value_rv = grammar::parse(
            it, end,
            grammar::optional_rule(
                grammar::tuple_rule(
                    detail::scheme_rule(),
                    grammar::squelch(
                        strequal_rule_t("://")))
                    )
                );
        if(! optional_scheme_value_rv)
        {
            return optional_scheme_value_rv.error();
        }

        auto& optional_scheme_value = optional_scheme_value_rv.value();
        if (optional_scheme_value)
        {
            auto& scheme_value = *optional_scheme_value;
            u.apply_scheme(scheme_value.scheme);
        }

    }

    // authority
    {
        auto rv = grammar::parse(
            it, end, authority_rule);
        if (! rv)
            return rv.error();
        u.apply_authority(*rv);
    }

    // md-hier-part
    {
        struct md_hier_part_value_t
        {
            pct_string_view path;
            std::size_t segment_count = 0;
        };

        md_hier_part_value_t t{};
        if(it == end)
        {
            // path-empty
            return u.construct();
        }

        if(end - it == 1)
        {
            if(*it == '/')
            {
                // path-absolute
                t.path = make_pct_string_view_unsafe(
                    it, 1, 1);
                t.segment_count = 1;
                ++it;
                u.apply_path(t.path, t.segment_count);
                return u.construct();
            }

            return error::non_canonical;
        }

        if(it == end || (
                *it != '/' &&
                *it != '?' &&
                *it != '#'))
        {
            // path-empty
            return u.construct();
        }
        auto const it0 = it;
        std::size_t dn = 0;
        if(*it != '/')
        {
            auto rv = grammar::parse(
                it, end, detail::segment_rule);
            if(! rv)
                return rv.error();
            if(rv->empty())
                return u.construct();
            dn += rv->decoded_size();
            ++t.segment_count;
        }
        while(it != end)
        {
            if(*it == '/')
            {
                ++dn;
                ++it;
                ++t.segment_count;
                continue;
            }
            auto rv = grammar::parse(
                it, end, detail::segment_rule);
            if(! rv)
                return rv.error();
            if(rv->empty())
                break;
            dn += rv->decoded_size();
        }
        t.path = make_pct_string_view_unsafe(
            it0, it - it0, dn);

        u.apply_path(
            t.path,
            t.segment_count);
    }

    // [ "?" query ]
    {
        auto rv = grammar::parse(
            it, end, detail::query_part_rule);
        if(! rv)
            return rv.error();
        if(rv->has_query)
        {
            // map "?" to { {} }
            u.apply_query(
                rv->query,
                rv->count);
        }
    }

    // [ "#" fragment ]
    {
        auto rv = grammar::parse(
            it, end, detail::fragment_part_rule);
        if(! rv)
            return rv.error();
        if(rv->has_fragment)
            u.apply_frag(rv->fragment);
    }

    return u.construct();
}

} // urls
} // boost

