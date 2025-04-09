#ifndef SPARKS_H_
#define SPARKS_H_

#include "geometry.h"

namespace game {

    // A set of particles that can be rendered
    class Sparks : public Geometry {

        public:
            Sparks(void);

            // Create the geometry (called once)
            void CreateGeometry(int num_particles);

            // Use the geometry
            void SetGeometry(GLuint shader_program);

    }; // class Particles
} // namespace game

#endif // SPARKS_H_
