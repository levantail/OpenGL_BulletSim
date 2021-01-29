# OpenGL_BulletSim (OUT OF DATE)

Bullet simulation with reflections from walls. Implemented in pure C++ using OpenGL, glew and glm math library.

When bullet collides with a wall it reflects from wall surface. Wall being destroyed after bullet successfully reflected.

Using imgui console adjust spawn properties for walls and bullets.

Every bullet upon spawn has randomized properties, such as:
- Location
- Time to spawn (after which time should appeared on scene and start moving)
- Life time (how long will stay alive)
- Speed

Every wall has randomized only rotation and location, but coud be simply extended.

UPD:
Simple Multhithread Pool added to increase performance.
