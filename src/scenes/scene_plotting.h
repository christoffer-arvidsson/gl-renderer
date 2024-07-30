
#ifndef SCENES_SCENE_plotting_H_
#define SCENES_SCENE_plotting_H_

#include "../arena.h"
#include "../scene.h"

void plotting_scene_init(Region* allocator, Scene* scene);
void plotting_scene_render(Scene* scene);
void plotting_scene_update(Scene* scene, float t);
void plotting_scene_deinit(Scene* scene);

#endif  // SCENES_SCENE_plotting_H_
