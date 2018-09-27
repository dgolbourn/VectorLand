#include "scene.h"

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>

#include <set>
#include <tuple>
#include <new>
#include <iostream>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//static bool operator==(aabb const& l, aabb const& r)
//{
//    return std::tie(l.min_corner.x, l.min_corner.y,
//                    l.max_corner.x, l.max_corner.y,
//                    l.z,
//                    l.ptr)
//           == std::tie(r.min_corner.x, r.min_corner.y,
//                       r.max_corner.x, r.max_corner.y,
//                       r.z,
//                       r.ptr);
//}
//
//static bool operator<(aabb const& l, aabb const& r)
//{
//    return l.z < r.z;
//}

static std::ostream& operator<<(std::ostream& os, const aabb& a)
{
    return os << "{\n\t\"min corner\" = \n\t{\n\t\t\"x\" = " << a.min_corner.x
           << ",\n\t\t\"y\" = " << a.min_corner.y
           << "\n\t},\n\t\"max corner\" = \n\t{\n\t\t\"x\" = " << a.max_corner.x
           << ",\n\t\t\"y\" = " << a.max_corner.y
           << "\n\t},\n\t\"z\" = " << a.z
           << ",\n\t\"ptr\" = " << a.ptr << "\n}";
}

BOOST_GEOMETRY_REGISTER_POINT_2D(corner, float, bg::cs::cartesian, x, y)

BOOST_GEOMETRY_REGISTER_BOX(aabb, corner, min_corner, max_corner)

template<typename Box> struct ptr
{
    Box const *p;
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

static std::ostream& operator<<(std::ostream& os, const AABB& a)
{
    return os << *(a.p);
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

int scene_add(scene *s, aabb const *r)
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

int scene_remove(scene *s, aabb const *r)
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

scene_list *scene_intersect(scene *s, aabb const *r)
{
    scene_list *l = nullptr;
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

aabb const *scene_list_next(scene_list *l)
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

void scene_list_destroy(scene_list *l)
{
    delete l;
}

scene *scene_init()
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

void scene_destroy(scene *s)
{
    delete s;
}

void example()
{
    scene *r = scene_init();

    aabb *x = new aabb;
    x->min_corner.x = 0;
    x->min_corner.y = 0;
    x->max_corner.x = 0.5f;
    x->max_corner.y = 0.5f;
    x->z = 100;
    x->ptr = nullptr;
    scene_add(r, x);
    for(unsigned i = 1 ; i < 100 ; ++i)
    {
        aabb *b = new aabb;
        b->min_corner.x = i;
        b->min_corner.y = i;
        b->max_corner.x = i + 0.5f;
        b->max_corner.y = i + 0.5f;
        b->z = 100-i;
        b->ptr = nullptr;
        scene_add(r, b);
    }
    scene_remove(r, x);
    aabb q;
    q.min_corner.x = 0.f;
    q.min_corner.y = 0.f;
    q.max_corner.x = 5.f;
    q.max_corner.y = 5.f;
    scene_list *l = scene_intersect(r, &q);
    std::cout << "spatial query box:" << std::endl;
    std::cout << q << std::endl;
    std::cout << "spatial query result:" << std::endl;
    while(aabb const *b = scene_list_next(l))
        std::cout << *b << std::endl;
}
