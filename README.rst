VectorLand Documentation
========================
VectorLand is a 2D planar vector based rendering engine

C API Reference
=================

Packages
--------

``scene.h``
+++++++++++

The scene library is where you register the bounding boxes of all the drawable entities using ``scene_add()``. 

When it comes time to render them you can query which bounding boxes are currently visible with ``scene_intersect()``.

Bounding boxes are returned in rendering order based on their ``z`` axis.

Here is an example of how to use this library:

.. code-block:: c

    struct scene *r = scene_init();

    /* populate r */
    struct aabb x;
    x.min_corner.x = 0.f;
    x.min_corner.y = 0.f;
    x.max_corner.x = .5f;
    x.max_corner.y = .5f;
    x.z = 0.f;
    x.ptr = 0.f;
    scene_add(r, &x);

    struct aabb y;
    y.min_corner.x = 1.f;
    y.min_corner.y = 1.f;
    y.max_corner.x = 1.5f;
    y.max_corner.y = 1.5f;
    y.z = -1.f;
    y.ptr = 0.f;
    scene_add(r, &y);

    /* remove from r */
    scene_remove(r, &x);

    /* now query which AABBs are visible in a given region */
    struct aabb q;
    q.min_corner.x = 0.f;
    q.min_corner.y = 0.f;
    q.max_corner.x = 5.f;
    q.max_corner.y = 5.f;
    struct scene_list *l = scene_intersect(r, &q);
    struct aabb const *b = 0;
    while(b = scene_list_next(l))
    {
    	/* consume the bounding box data here */
    }
    scene_list_destroy(l);

    scene_destroy(r);


.. data:: corner

    a point denoting a corner of a bounding box

    :type: struct
	:param x: abscissa
	:param y: ordinate
	:type x: float
	:type y: float


.. data:: aabb

    A bounding box

    :type: struct
	:param min_corner: min corner
	:param max_corner: max corner	
	:param z: rendering order
	:param ptr: user data
	:type x: ``corner``
	:type y: ``corner``	
	:type z: float
	:type ptr: void pointer

.. data:: scene

	reference to a scene

	:type: incomplete type


.. data:: scene_list

	reference to a scene

	:type: incomplete type


.. function:: scene_init()

    create a new scene

    :return: new scene on success else 0
    :rtype: ``scene`` pointer


.. function:: scene_destroy(scene)

	destroy the given scene

	:param scene: this scene
	:type scene: ``scene`` pointer
	:rtype: none


.. function:: scene_add(scene, aabb)

	add a bounding box to the scene

	:param scene: this scene
	:type scene: ``scene`` pointer
	:param aabb: the bounding box to add
	:type aabb: ``aabb`` pointer	
	:return: 0 on success, -1 otherwise
	:rtype: integer


.. function:: scene_remove(scene, aabb)

	remove the bounding box from the scene

	:param scene: this scene
	:type scene: ``scene`` pointer
	:param aabb: the bounding box to remove
	:type aabb: ``aabb`` pointer	
	:return: 0 on success, -1 otherwise
	:rtype: integer


.. function:: scene_intersect(scene, aabb)

	find all bounding boxes that intersect the given region

	:param scene: this scene
	:type scene: ``scene`` pointer
	:param aabb: the bounding box to intersect
	:type aabb: ``aabb`` pointer	
	:return: ``scene_list`` on success, 0 otherwise
	:rtype: ``scene_list`` pointer


.. function:: scene_list_next(scene_list)

	get the next bounding box in the scene list

	:param scene_list: this scene list
	:type scene_list: ``scene_list`` pointer
	:return: bounding box if present, 0 otherwise
	:rtype: ``aabb`` pointer


.. function:: scene_list_destroy(scene_list)

	destroy the given scene list

	:param scene_list: this scene list
	:type scene_list: ``scene_list`` pointer
	:rtype: none


License
=======

Copyright (c) 2018 Diane Marigold

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
