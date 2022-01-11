#include "CoreHeaders.h"
#include "Vector.h"

namespace fw {

void vec2::Test()
{
    vec2 pos(10, 5);
    assert( pos == vec2(10,5) );

    vec2 other;
    other.Set( 1, 2 );
    assert( other == vec2(1,2) );

    vec2 newPos = pos + other;
    assert( newPos == vec2(11,7) );

    vec2 copyOfPos( pos );
    assert( copyOfPos == vec2(10,5) );

    copyOfPos += vec2(4, -12);
    assert( copyOfPos == vec2(14,-7) );

    vec2 negativeOfPos = -pos;
    assert( negativeOfPos == vec2(-10,-5) );

    vec2 scaledPos = 5.0f * pos;
    assert( scaledPos == vec2(50,25) );

    scaledPos = pos * 10.0f;
    assert( scaledPos == vec2(100,50) );
}

} // namespace fw
