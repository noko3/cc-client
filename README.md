# ChanCity [The Game] client
ChanCity client is a proxy between CC server and CC frontend.

Current milestone: multiplayer cube pushing game client

## DOWANT

The client must implement the following:
1. client<->server API
2. client<->frontend API
3. Networking (using enet)
4. The Game itself:
    4.1. Object tracker
    4.2. Audio back-end
    4.3. Geometry generator
    4.4. Physics back-end
	4.4.1. Soft body dynamics
	4.4.2. Object destruction (fracture)
	4.4.3. Various ballistics
	4.4.4. 3D multipolygon physbox collisions
    4.5. Time, day/night, year, weather cycle
5. Lua or Python scripting interfaces (need to decide quick or else it will be Lua AND Python)
6. Chemical reaction/process engine
    6.1. Particle system
