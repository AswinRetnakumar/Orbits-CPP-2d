#include <SFML/Graphics.hpp>
#include <chrono>
#include <math.h>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

float delta_T = 0.01;
float max_body_size = 50.0f;
float min_body_size = 2.0f;

class Acceleration
{
    public:

    float accel_x;
    float accel_y;

    Acceleration(float acel_x = 0.0 ,float acel_y = 0.0)
    {
        accel_x = acel_x;
        accel_y = acel_y;
    }
};

template <class T>
class Queue
{
    public:

    int capacity;
    std::vector <T> q;

    Queue(int cap = 10000)
    {
        capacity = cap;
    }

    void add(T ele)
    {
        if(q.size() == capacity)
        {
            q.erase(q.begin());
            
        }
        q.push_back(ele);
    }

};


class Velocity
{
    public:

    float velocity_x;
    float velocity_y;

    Velocity(float vel_x = 0.0, float vel_y = 0.0)
    {
        velocity_x = vel_x;
        velocity_y = vel_y;
    }

};

class Position
{
    public:

    float position_x;
    float position_y;

    Position(float pos_x = 400.0, float pos_y=400.0)
    {
        position_x = 400.0+pos_x;
        position_y = 400.0+pos_y;
    }

    Position operator -(Position const &obj)
    {
        Position result;
        result.position_x = position_x - obj.position_x;
        result.position_y = position_y - obj.position_y;

        return result;
    }

};

typedef class Acceleration acceleration;
typedef class Position position;
typedef class Velocity velocity;
//typedef class CelestialBodies celestials;

class CelestialBodies
{
    public:
    float mass;
    position pos;
    velocity vel;
    sf::CircleShape body;
    Queue <position> trace;

    CelestialBodies(float m, position p, velocity v, sf::Color color)
    {
        pos = p;
        vel = v;
        mass = m;
        body.setRadius(std::min(max_body_size, std::max(min_body_size, std::log(mass))));
        body.setPosition(pos.position_x, pos.position_y);
        body.setFillColor(color);
    }


    void set_velocity(acceleration acc)
    {
        vel.velocity_x -= acc.accel_x * delta_T;
        vel.velocity_y -= acc.accel_y * delta_T;

    }

    void set_position()
    {
        pos.position_x += vel.velocity_x * delta_T;
        pos.position_y += vel.velocity_y * delta_T;
        trace.add(pos);
        std::cout<<"\nx: "<<pos.position_x<<"  y: "<<pos.position_y;
        body.setPosition(pos.position_x, pos.position_y);
    }
};

CelestialBodies step(CelestialBodies body, acceleration a)
{
    body.set_velocity(a);
    body.set_position();
    
    return body;
}

/*
float get_angle(CelestialBodies body1, CelestialBodies body2)
{
    position pos1 = body1.pos;
    position pos2 = body2.pos;
    float theta = acos((pos1.position_x*pos2.position_x)/(pos1.position_x*pos2.position_x + pos1.position_y*pos2.position_y));
    std::cout<<"\ntheta: "<<180* theta/3.14;
    return theta;
}
*/


float get_angle(CelestialBodies body1, CelestialBodies body2)
{   /*
            pos1
            | 
            ||
         a  |||
            ||||
            ||||| pos2
              b
    */
    position pos1 = body1.pos;
    position pos2 = body2.pos;
    float a = pos1.position_y - pos2.position_y;
    float b = pos1.position_x - pos2.position_x;
    float theta = atan2(a,b); //angle in radians

    std::cout<<"\ntheta: "<<180* theta/3.14;
    return theta;
}


void render(std::vector<CelestialBodies> bodies)
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Planetary Orbits!");
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(int i=0; i<bodies.size(); i++)
        {   std::cout<<"\nBody "<<i;
            acceleration acc;
            window.draw(bodies[i].body);
            sf::CircleShape trace_pt(1.f);
            for(int t=0; t<bodies[i].trace.q.size(); t++)
            {
                trace_pt.setPosition(bodies[i].trace.q[t].position_x, bodies[i].trace.q[t].position_y);
                window.draw(trace_pt);
            }
            for(int j=0; j<bodies.size(); j++)
            {   
                if(j != i)
                {
                std::cout<<"\nBodies "<<i<<"  "<<j;
                position diff = bodies[i].pos - bodies[j].pos;
                float distance = sqrt(diff.position_x*diff.position_x + diff.position_y*diff.position_y);
                float force = (bodies[i].mass * bodies[j].mass)/ (distance*distance); //G is a constant, hence not needed...
                float theta = get_angle(bodies[i], bodies[j]);
                acc.accel_x += (force/bodies[i].mass )* cos(theta);
                acc.accel_y += (force/bodies[i].mass )* sin(theta);
                }

                
            }
            std::cout<<"\nAccel_x: "<<acc.accel_x<<" Accel_y: "<<acc.accel_y;

            bodies[i] = step(bodies[i], acc);
        }

        sleep_until(system_clock::now() + 0.001s);
        
        window.display();
    }
    
}


int main()
{
    std::vector<CelestialBodies> Bodies;
    position p(-120, 0);
    velocity v(5, 0);
    sf::Color c(200, 100, 0);
    CelestialBodies sun(100000, p, v, c);
    position p2(0, 200);
    sf::Color c2(0, 50, 220);
    velocity v2(-10, 65);
    position p3(0, 250);
    sf::Color c3(50, 100, 220);
    velocity v3(25, -20);

    position p4(0, 220);
    sf::Color c4(220, 20, 20);
    velocity v4(35, -1);

    CelestialBodies planet(10, p2, v2, c2);
    CelestialBodies planet2(1500, p3, v3, c3);
    Bodies.push_back(sun);
    Bodies.push_back(planet);
    Bodies.push_back(planet2);
    CelestialBodies planet3(2, p4, v4, c4);
    Bodies.push_back(planet3);




    render(Bodies);
    return 0;
}