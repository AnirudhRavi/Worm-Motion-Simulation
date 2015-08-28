**Program**: Simulating motion dynamics of a worm using a 3 mass-2 spring system

**Author**: Anirudh Ravi

**About**:

*Scene*:

The scene consists of a grass field having 2 flower pots and the worm’s house along with a path leading to the house.

*Worm*

The worm is drawn using 3 ellipsoids. An orange torus ring is drawn as the worm’s scales so as to enhance the worm’s look.

**Algorithm**:

*Modeling the 3 mass - 2 spring system*:

For implementing the motion of a worm a 3 mass - 2 spring system was implemented. The masses are implemented as ellipsoids whose position and velocity vectors are calculated along the X direction while the Y and Z coordinates for velocity and position are kept constant. The force acting on the masses was calculated using the formula:

					- f = k*(L - l) - D*dl/dt 

(f = Force along spring direction, k = Spring force constant, D = Damping force, l = Current length of the spring, L = Minimum energy spring length)

The velocity and position vectors of each mass are updated based on the acceleration acting on the particle. The acceleration was computed using the formula a = f/m. Based on acceleration value, the updated values of position and velocity vectors are:

					- V(new) = V(prev) + a*dt
					- P(new) = P(prev) + V(new)*dt

dt was maintained as 0.5 milliseconds for simulating the motion of the worm.

*Translation using directional friction*:

Due to the force acting on mass A the spring between A and B expands because of which there will be backward friction on A resulting in mass A remaining stationary while B moves. Now the spring between B and C expands resulting in a backward friction on B making B stationary while C moves. Due to this friction there will be translation along X axis of the spring mass - system.

Directional friction is calculated using by using a unit vector known as Spine Vector (S). This vector gives the direction of motion of the worm. It is computed using the formula:

					- S = (XC-XA)/(|XA-XC|)

The velocity of the individual particles is set as:

					- If S*V < 0.0 then V = V - S(S*V)

The above expression prevents backward sliding of the worm.

Since motion of the worm is defined only along X axis:

					- V = V - S(S*V) = 0; If S*V < 0.0

**References**:

The following papers were very useful for understanding the physics involved while simulating a worm:

1.    “The Motion Dynamics of Snakes and Worms” by Gavin S.P. Miller

2.    The Siggraph paper related to snake and worm motion dynamics by G. Scott Owen 

The following websites were useful while implementing mass – spring systems:

1.    Ryan’s Blog: Implementing a Mass Spring system: http://www.ryanmwright.com/2009/10/03/implementing-a-mass-spring-system/

2.    Spring mass system Alexandra Lab: http://cg.alexandra.dk/tag/spring-mass-system/

**Executing**:

g++ ProgramName -lGL -lGLU -lglut -lm -o ExecutableName

./ExecutableName
