#include <SFML/Graphics.hpp>
#include <math.h>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

float px = 300.0f, py = 300.0f, pa = 0;
const int mapx = 8;
const int mapy = 8;
const int maps = 64;
int map[] =
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,1,1,0,1,
    1,0,1,0,1,0,0,1,
    1,0,1,0,1,1,0,1,
    1,0,1,0,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,1,1,1,1,1,1,1,
};

void drawMap2D(sf::RenderWindow& window) {
    int x, y, xo, yo;
    
    for (y = 0; y < mapy; y++) {
        for (x = 0; x < mapx; x++) {
            if (map[y * mapx + x] == 1) {
                sf::RectangleShape square(sf::Vector2f(maps, maps));
                square.setPosition(x * maps, y * maps);
                square.setFillColor(sf::Color::White);
                square.setOutlineThickness(1.f);
                    square.setOutlineColor(sf::Color(128,128,128));
                window.draw(square);
            }
            else {
                sf::RectangleShape square(sf::Vector2f(maps, maps));
                square.setPosition(x * maps, y * maps);
                square.setFillColor(sf::Color::Black);
                square.setOutlineThickness(1.f);
                square.setOutlineColor(sf::Color(128,128,128));
                window.draw(square);
            }
        }
    }
}

float dist(float ax, float ay, float bx, float by, float ang) {
    return(sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawRays3D(sf::RenderWindow& window) {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo,disT = 0;

    ra = pa - DR * 30;
    if (ra < 0) {
        ra += 2 * PI;
    }
    if (ra > 2 * PI) {
        ra -= 2 * PI;
    }
   //Horizontral
    for (r = 0; r < 60; r++) {
        dof = 0;
        float dish = 1000000, hx = px, hy = py;
        float hTan = -1 / tan(ra);
        if (ra > PI) {
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx       = (py - ry) * hTan + px;
            yo = -64;
            xo = -yo * hTan;
        }
        if (ra < PI) {
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * hTan + px;
            yo = 64;
            xo = -yo * hTan;
        }
        if (ra == 0 || ra == PI) {
            rx = px;
            ry = py;
            dof = 8;
        }
        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapx + mx;
            if (mp > 0 && mp < mapx * mapy && map[mp] == 1) {
                hx = rx;
                hy = ry;
                dish = dist(px, py, hx, hy, ra);
                dof = 8;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        //Vertical

        dof = 0;
        float disv = 1000000, vx = px, vy = py;
        float vTan = -tan(ra);
        if (ra > P2 && ra < P3) {
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * vTan + py;
            xo = -64;
            yo = -xo * vTan;
        }
        if (ra < P2 || ra > P3) {
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * vTan + py;
            xo = 64;
            yo = -xo * vTan;
        }
        if (ra == 0 || ra == PI) {
            rx = px;
            ry = py;
            dof = 8;
        }
        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapx + mx;
            if (mp > 0 && mp < mapx * mapy && map[mp] == 1) {
                vx = rx;
                vy = ry;
                disv = dist(px, py, vx, vy, ra);
                dof = 8;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        float lineH = (maps * 320) / disT;
        if (lineH > 320) {
            lineH = 320;
        }

        float lineO = 160 - lineH / 2;
        sf::RectangleShape line(sf::Vector2f(8, lineH));
        line.setPosition(r * 8 + 530, lineO);

        if (disv < dish) {
            rx = vx;
            ry = vy;
            disT = disv;
            line.setFillColor(sf::Color(240,0,0));
        }
        if (dish < disv) {
            rx = hx;
            ry = hy;
            disT = dish;
            line.setFillColor(sf::Color(200,0,0));
        }

        sf::Vertex linev[] = {
               linev[0].position = sf::Vector2f(px, py),
               linev[1].position = sf::Vector2f(rx,ry)
        };
        linev[0].color = sf::Color::Red;
        linev[1].color = sf::Color::Red;
        window.draw(linev, 2, sf::Lines);

        //3D
        float ca = pa - ra;
        if (ca < 0) {
            ca += 2 * PI;
        }
        if (ca > 2 * PI) {
            ca -= 2 * PI;
        }
        disT = disT * cos(ca);
        
        window.draw(line);

        ra += DR;
        if (ra < 0) {
            ra += 2 * PI;
        }
        if (ra > 2 * PI) {
            ra -= 2 * PI;
        }
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 512), "My window");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }   
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {   
            pa -= 0.01;
            if (pa < 0) {
                pa += 2 * PI;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            pa += 0.01;
            if (pa > 2*PI) {
                pa -= 2 * PI;
            } 
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {   
            float pdx = cos(pa) * 0.5;
            float pdy = sin(pa) * 0.5;
            px += pdx;
            py += pdy;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {   
            float pdx = cos(pa) * 0.5;
            float pdy = sin(pa) * 0.5;
            px -= pdx;
            py -= pdy;
        }

        window.clear();
        float pdx = cos(pa) * 5;
        float pdy = sin(pa) * 5;
        sf::RectangleShape player(sf::Vector2f(10, 10));
        player.setFillColor(sf::Color(255, 255, 0));
        player.setOrigin(5, 5);
        player.setPosition(px, py);

        sf::Vertex line[] = {
            line[0].position = sf::Vector2f(px, py),
            line[1].position = sf::Vector2f(px + pdx * 5, py + pdy * 5)
        };
        line[0].color = sf::Color::Yellow;
        line[1].color = sf::Color::Yellow;

        drawMap2D(window);
        window.draw(player);
        drawRays3D(window);
        window.draw(line, 2, sf::Lines);
        window.display();
       }
}