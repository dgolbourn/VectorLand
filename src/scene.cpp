#include "scene.h"

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>

#include <set>
#include <tuple>
#include <new>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

BOOST_GEOMETRY_REGISTER_POINT_2D(corner, float, bg::cs::cartesian, x, y)

BOOST_GEOMETRY_REGISTER_BOX(aabb, corner, min_corner, max_corner)

template<typename Box> struct ptr
{
    Box const* p;
};

namespace boost
{
namespace geometry
{
namespace index
{
template <typename Box>
struct indexable< ptr<Box> >
{
    typedef ptr<Box> V;

    typedef Box const& result_type;
    result_type operator()(V const& v) const
    {
        return *(v.p);
    }
};
}
}
}

typedef ptr<aabb> AABB;

static bool operator==(AABB const& l, AABB const& r)
{
    return l.p == r.p;
}

static bool operator<(AABB const& l, AABB const& r)
{
    return l.p->z < r.p->z;
}

struct scene
{
    bgi::rtree<AABB, bgi::quadratic<16>> t;
};

struct scene_list
{
    std::set<AABB> s;
    std::set<AABB>::iterator i;
};

int scene_add(scene* s, aabb const* r)
{
    try
    {
        AABB R;
        R.p = r;
        s->t.insert(R);
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

int scene_remove(scene* s, aabb const* r)
{
    try
    {
        AABB R;
        R.p = r;
        s->t.remove(R);
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

scene_list* scene_intersect(scene* s, aabb const* r)
{
    scene_list* l = nullptr;
    try
    {
        l = new scene_list;
        s->t.query(bgi::intersects(r), std::inserter(l->s, l->s.end()));
        l->i = l->s.begin();
        return l;
    }
    catch(...)
    {
        delete l;
        return nullptr;
    }
}

aabb const* scene_list_next(scene_list* l)
{
    try
    {
        if(l->i != l->s.end())
            return l->i++->p;
    }
    catch(...)
    {
    }
    return nullptr;
}

void scene_list_destroy(scene_list* l)
{
    delete l;
}

scene* scene_init()
{
    try
    {
        return new scene;
    }
    catch(...)
    {
        return nullptr;
    }
}

void scene_destroy(scene* s)
{
    delete s;
}
