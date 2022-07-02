"""
Simulate the stars/planets/satellites motion in 2D space. Every two objects in the universe are attracted by the gravitational force

$$\vec{F_{ij}} = \frac{G m_i m_j}{r_{ij}^2} \frac{\vec{r_{ij}}}{\|r_{ij}\|}.$$ 

The force that acts on the object $i$ is the vectorial sum of the forces induced by all other (massive) objects

$$\vec{F_i} = \sum_{j \neq i} \vec{F_{ij}}$$

Use SI units, don't be concerned with the speed of the code - do not optimize!!!

Write function that takes any number of space objects (named tuples) as arguments (may not be a list of named tuples for any function!!!) plus the size of the time step and number of time steps. For each object it calculates the force caused by other objects (vector sum of attractive forces). It returns the dictionary with name of the object as a key and tuple of lists of coordinates (one list of x, one of y, every time step one item in list). 

Write a decorator that measures number of calling of each function and their runtime of the functions. The information should be printed to standard output in a form "function_name - number_of_calls - time units\n". The decorator takes optional parameter units which allows to specify time units for printing (default is ms). You can implement the unit measurement only for ns, ms, s, min, h and days.

Below is description of all steps for calculating the update. If you are unsure of precise interface see test script for examples of calling the function.
"""

import time #measuring time
import math
from collections import namedtuple

#Define universal gravitation constant
G = 6.67408e-11 #N-m2/kg2
SpaceObject = namedtuple('SpaceObject', 'name mass x y vx vy color')
Force = namedtuple('Force', 'fx fy')

def get_unit_factor(unit):
    switcher = {
        'ns': 10**9,
        'ms': 10**3,
        's': 1,
        'min': 1 / 60,
        'h': 1 / 60**2,
        'days': 1 / 60**2 / 24
    }
    return switcher.get(unit, "Invalid unit")


def logging(unit = 'ms'):
    def decorator(target):
        def wrapper(*args, **kwargs):
            wrapper.number_of_calls += 1

            start = time.time()
            ret = target(*args, **kwargs)
            end = time.time()
  
            factor = get_unit_factor(unit)   
            print(f'{target.__name__} - {wrapper.number_of_calls} - {(end - start) * factor} {unit}')
            return ret

        wrapper.number_of_calls = 0
        return wrapper

    return decorator


@logging(unit = 'ms')
def calculate_force(space_object, *other_space_objects):
    #input: one of the space objects (indexed as i in below formulas), other space objects (indexed as j, may be any number of them)
    #returns named tuple (see above) that represents x and y components of the gravitational force
    #calculate force (vector) for each pair (space_object, other_space_object):
    #|F_ij| = G*m_i*m_j/distance^2
    #F_x = |F_ij| * (other_object.x-space_object.x)/distance
    #analogous for F_y
    #for each coordinate (x, y) it sums force from all other space objects

    fx = fy = 0

    for other_object in other_space_objects:
        distance = math.sqrt( (space_object.x - other_object.x)**2 + (space_object.y - other_object.y)**2 )
        F_ij_abs = G * space_object.mass * other_object.mass / distance**2
        F_x = F_ij_abs * (other_object.x - space_object.x) / distance
        F_y = F_ij_abs * (other_object.y - space_object.y) / distance

        fx += F_x
        fy += F_y

    return Force(fx, fy)


@logging(unit = 's')
def update_space_object(space_object, force, timestep):
    #here we update coordinates and speed of the object based on the force that acts on it
    #input: space_object we want to update (evolve in time), force (from all other objects) that acts on it, size of timestep
    #returns: named tuple (see above) that contains updated coordinates and speed for given space_object
    #hint:
    #acceleration_x = force_x/mass
    #same for y
    #speed_change_x = acceleration_x * timestep
    #same for y
    #speed_new_x = speed_old_x + speed_change_x
    #same for y
    #x_final = x_old + speed_new_x * timestep
    
    acceleration_x = force.fx / space_object.mass
    speed_change_x = acceleration_x * timestep
    speed_new_x = space_object.vx + speed_change_x
    x_final = space_object.x + speed_new_x * timestep

    acceleration_y = force.fy / space_object.mass
    speed_change_y = acceleration_y * timestep
    speed_new_y = space_object.vy + speed_change_y
    y_final = space_object.y + speed_new_y * timestep

    return SpaceObject (
        name = space_object.name,
        mass = space_object.mass,
        x = x_final,
        y = y_final,
        vx = speed_new_x,
        vy = speed_new_y,
        color = space_object.color
    )


@logging(unit = 'ms')
def update_motion(timestep, *space_objects):
    #input: timestep and space objects we want to simulate (as named tuples above)
    #returns: list or tuple with updated objects
    #hint:
    #iterate over space objects, for given space object calculate_force with function above, update
    
    updated_space_objects = ()

    for space_object in space_objects:
        other_objects = tuple(x for x in space_objects if x != space_object)

        force = calculate_force(space_object, *other_objects)
        updated_object = update_space_object(space_object, force, timestep)

        updated_space_objects += updated_object,

    return updated_space_objects #(named tuple with x and y)


@logging()
def simulate_motion(timestep, number_of_timesteps, *space_objects):
    #generator that in every iteration yields dictionary with name of the objects as a key and tuple of coordinates (x first, y second) as values
    #input size of timestep, number of timesteps (integer), space objects (any number of them)

    for _ in range(number_of_timesteps):
        space_objects = update_motion(timestep, *space_objects)
        
        ret = {}
        for updated_object in space_objects:
            ret[updated_object.name] = (updated_object.x, updated_object.y)
        
        yield ret
