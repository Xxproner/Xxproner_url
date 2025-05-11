//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/uri_optional_scheme_rule.hpp>

#include "test_rule.hpp"

#include <iostream>

namespace boost {
namespace urls {

class uri_optional_scheme_rule_test
{
public:
    void
    run()
    {
        // javadoc
        {
            system::result< url_view > rv = grammar::parse( "example.com/index.htm?id=1", uri_optional_scheme_rule );
            (void)rv;
        }

        auto const& t = uri_optional_scheme_rule;

        bad(t, "");
        bad(t, ":");
        bad(t, "http://#");
        bad(t, "http://x.y.z/?a=b&c=d&#");
        bad(t, "http://x.y.z/?a=b&c=d&#frag");
        bad(t, "http://x.y.z/#frag");
        bad(t, "http://%");
        bad(t, "http://?%");

        bad(t, "http:");
        bad(t, "http:x");
        bad(t, "http:x/");
        bad(t, "http:x/x");
        bad(t, "http:x//");
        bad(t, "http://");
        
        ok(t, "http://x");
        ok(t, "http://x.y.z");
        ok(t, "http://x.y.z/");
        ok(t, "http://x.y.z/?");
        ok(t, "http://x.y.z/?a");
        ok(t, "http://x.y.z/?a=");
        ok(t, "http://x.y.z/?a=b");
        ok(t, "http://x.y.z/?a=b&c=d");
        ok(t, "http://x.y.z/?a=b&c=d&");
        
        // scheme is not mantadory
        ok(t, "x.y.z/?a=b");
        ok(t, "x.y.z/?a=b&c=d");
        ok(t, "x.y.z/?a=b&c=d&");

        // magnet link
        ok(t, "magnet:?xt=urn:btih:d2474e86c");

        // reg-name might have ipv4 prefix
        ok(t, "http://192.168.0.1.3.a");
    }
};

TEST_SUITE(
    uri_optional_scheme_rule_test,
    "boost.url.optional_scheme_rule");

} // urls
} // boost
