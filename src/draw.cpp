#include "draw.h"
// Disable warnings in third-party code.
#include <framework/opengl_includes.h>
#include <iostream>
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#ifdef __APPLE__
#include <OpenGL/GLU.h>
#else
#ifdef WIN32
// Windows.h includes a ton of stuff we don't need, this macro tells it to include less junk.
#define WIN32_LEAN_AND_MEAN
// Disable legacy macro of min/max which breaks completely valid C++ code (std::min/std::max won't work).
#define NOMINMAX
// GLU requires Windows.h on Windows :-(.
#include <Windows.h>
#endif
#include <GL/glu.h>
#endif
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
DISABLE_WARNINGS_POP()
#include <algorithm>
#include <Windows.h>
#include <GL/glu.h>
#include "util.h" 
#include <framework/ray.h>

// TODO: Make this work with colours for different curvature 
void drawMeshWithColors(const Mesh& mesh, std::vector<glm::vec3> colors)
{
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : mesh.triangles) {
        for (int v = 0; v < 3; v++) {
            const auto& color = colors[triangle[v]];
            const auto& vertex = mesh.vertices[triangle[v]];

            glColor3fv(glm::value_ptr(color));
            glNormal3fv(glm::value_ptr(vertex.normal));
            glVertex3fv(glm::value_ptr(vertex.position));
        }
    }
    glEnd();
}

void drawAxis() 
{
    // RED x-axis
    glColor3f(1.0, 0.0, 0.0); 
    glBegin(GL_LINES);
    glVertex3f(-4.0, 0.0f, 0.0f);
    glVertex3f(4.0, 0.0f, 0.0f);
    glEnd();

    // GREEN y-axis
    glColor3f(0.0, 1.0, 0.0); 
    glBegin(GL_LINES);
    glVertex3f(0.0, -4.0f, 0.0f);
    glVertex3f(0.0, 4.0f, 0.0f);
    glEnd();

    // BLUE z-axis
    glColor3f(0.0, 0.0, 1.0); 
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0f, -4.0f);
    glVertex3f(0.0, 0.0f, 4.0f); 
    glEnd();
}

 static void draw_sphere_internal(const glm::vec3& center, float radius)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    const glm::mat4 transform = glm::translate(glm::identity<glm::mat4>(), center);
    glMultMatrixf(glm::value_ptr(transform));
    auto quadric = gluNewQuadric();
    gluSphere(quadric, radius, 40, 20);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void draw_sphere(const glm::vec3& center, float radius, const glm::vec3& color)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor4f(color.r, color.g, color.b, 1.0f);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    draw_sphere_internal(center, radius);
    glPopAttrib();
}

void draw_regions(std::vector<Vertex>& vertices)
{
    ColorRegion color; 
    for (Vertex& v : vertices) {
        switch (v.region) {
            case Region::IT: 
                draw_sphere(v.position, 1e-2f, color.c1); 
                continue; 
            case Region::OT: 
                draw_sphere(v.position, 1e-2f, color.c2);
                continue;
            case Region::SB:
                draw_sphere(v.position, 1e-2f, color.c3);
                continue; 
            case Region::FB:
                draw_sphere(v.position, 1e-2f, color.c4);
                continue;  
            case Region::SA:
                draw_sphere(v.position, 1e-2f, color.c5);
                continue;
            case Region::FA:
                draw_sphere(v.position, 1e-2f, color.c6);
                continue;
        } 
    }
}

// Draws a ray, mostly used for checking surface normals 
void drawRay(const Ray& ray, const glm::vec3& color)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    const glm::vec3 hitPoint = ray.origin + std::clamp(ray.t, 0.0f, 100.0f) * ray.direction;

    glColor3fv(glm::value_ptr(color));
    glVertex3fv(glm::value_ptr(ray.origin));
    glColor3fv(glm::value_ptr(color));
    glVertex3fv(glm::value_ptr(hitPoint));
    glEnd();
    glPopAttrib();
}

void draw_normal_rays(std::vector<Ray>& normals)
{
    for (auto& r : normals) {
        drawRay(r, glm::vec3(1, 0, 0)); 
    }
}

void draw_laplace_rays(std::vector<Ray>& laplace)
{
    for (auto& r : laplace) {
        drawRay(r, glm::vec3(0, 0, 0));
    }
}