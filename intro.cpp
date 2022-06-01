#include <SFML/Graphics.hpp>
#include <chrono>
#include <math.h>
#include <thread>
#include <iostream>


struct CirclePosition {
    sf::CircleShape circle;
    float t;
};

class CircleCentre{
    public:
        float x;
        float y;
        CircleCentre(float a, float b)
        {
            x = a;
            y = b;
        }
};

typedef struct CirclePosition circleposition;
typedef class CircleCentre centre;


circleposition Rotate(sf::CircleShape, centre, float, float);


int main()
{   
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Rotation aroud center!");
    sf::CircleShape circle(20.f);    
    centre c(400.0, 400.0);

    circleposition position;
    float t = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        position = Rotate(circle, c, 200.f, t);
        t = position.t;
        sleep_until(system_clock::now() + 25ms);


        window.clear();
        window.draw(position.circle);
        window.display();
    }

    return 0;
}


circleposition Rotate(sf::CircleShape circle, centre c, float rotation_radius, float t)
{
    //circle position of time is x = rcos(2pi(t-t0)/T), y = rsin(2pi(t-t0)/T)
    circleposition position;

    float T = 5;
    float t0 = 0;

    float increment = T/50;
    t += increment;
    float x = c.x + rotation_radius*cos(6.28*(t-t0)/T);
    float y = c.y + rotation_radius*sin(6.28*(t-t0)/T);
    circle.setPosition(x, y);
    position.circle = circle;
    position.t = t;

    return position;
}

