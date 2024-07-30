#ifndef SCENES_SCENE_TEAPOT_H_
#define SCENES_SCENE_TEAPOT_H_

#include "../arena.h"
#include "../scene.h"

void teapot_scene_init(Region* allocator, Scene* scene);
void teapot_scene_render(Scene* scene);
void teapot_scene_update(Scene* scene, float t);
void teapot_scene_deinit(Scene* scene);

#endif  // SCENES_SCENE_TEAPOT_H_
