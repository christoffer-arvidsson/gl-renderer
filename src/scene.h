#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "line.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

typedef struct Scene Scene;

typedef void (*scene_init)(Region *allocator, Scene *scene);
typedef void (*scene_render)(Scene *scene);
typedef void (*scene_update)(Scene *scene, float t);
typedef void (*scene_deinit)(Scene *scene);

struct Scene {
  GLFWwindow *window;

  PerspectiveCamera camera;

  void *state;

  scene_init init_fn;
  scene_render render_fn;
  scene_update update_fn;
  scene_deinit deinit_fn;
};

#endif // SCENE_H_
