#pragma once
// Disable warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
DISABLE_WARNINGS_POP()
#include <framework/mesh.h>
#include <framework/ray.h>
#include <span>

void drawMeshWithColors(const Mesh& mesh, std::vector<glm::vec3> colors);
void drawAxis(); 
void draw_regions(std::vector<Vertex>& vertices); 
void draw_normal_rays(std::vector<Ray>& normals); 
void draw_laplace_rays(std::vector<Ray>& laplace);