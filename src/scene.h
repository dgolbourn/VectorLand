#ifndef SCENE_H
#define SCENE_H
#ifdef __cplusplus
extern "C" {
#endif

struct corner
{
    float x;
    float y;
};

struct aabb
{
    struct corner min_corner;
    struct corner max_corner;
    float z;
    void *ptr;
};

struct scene;

int scene_add(struct scene *, struct aabb const *);
int scene_remove(struct scene *, struct aabb const *);
struct scene *scene_init();
void scene_destroy(struct scene *);

struct scene_list;

struct scene_list *scene_intersect(struct scene *, struct aabb const *);
struct aabb const *scene_list_next(struct scene_list *);
void scene_list_destroy(struct scene_list *);

void example();
#ifdef __cplusplus
}
#endif
#endif
