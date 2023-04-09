#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;
using namespace sf;


int main()
{
    const int W = 1080;
    const int H = 1080;
    const int dr = 40;
    const int w = W/dr;
    const int h = H/dr;


    ///// SETUP /////

    RenderWindow window(VideoMode(W, H), "Snake Game");
    window.setFramerateLimit(60);

    RectangleShape snake(Vector2f(dr, dr));
    snake.setFillColor(Color(190, 18, 15));

    CircleShape point(dr/2);
    point.setFillColor(Color(90, 190, 15));


    int size = 3;
    char dir = 'L';
    vector<int> snake_x(size);
    vector<int> snake_y(size);
    for (int i = 0; i < size; i++) {
        snake_x[i] = (int)(w/2) + i;
        snake_y[i] = (int)(h/2);
    }

    vector<int> tsnake_x(size-1);
    vector<int> tsnake_y(size-1);
    for (int i = 0; i < size-1; i++) {
        tsnake_x[i] = snake_x[i];
        tsnake_y[i] = snake_y[i];
    }

    int point_x = rand() % (w);
    int point_y = rand() % (h);

    Clock clock;
    Time waiting_time = seconds(0.1);

    Font font;
    font.loadFromFile("assets/Amatic-Bold.ttf");
    Text text1;
    text1.setFont(font);
    text1.setCharacterSize(50);
    text1.setFillColor(Color::White);
    text1.setPosition(Vector2f(10, 0));

    Text text2;
    text2.setFont(font);
    text2.setCharacterSize(50);
    text2.setFillColor(Color::White);

    char str1[100];
    int score = 0;
    int speed = 4;

    char str2[100];

    int tms = 0;
    int max_score = 0;
    if (!fopen("assets/data.dat", "r"))
    {
        FILE *ms = fopen("assets/data.dat", "w+");
        fprintf(ms, "%d", 0);
        fclose(ms);
    }
    else
    {
        FILE *ms = fopen("assets/data.dat", "r+");
        fscanf(ms, "%d", &max_score);
        fclose(ms);
    }

    bool turned = true;


    ///// MAIN LOOP /////

    while (window.isOpen())
    {


        ///// CHECKING FOR EVENTS /////

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Right && dir != 'L' && turned == false) { 
                    dir = 'R';
                    turned = true;
                } else if (event.key.code == Keyboard::Left && dir != 'R' && turned == false) {
                    dir = 'L';
                    turned = true;
                } else if (event.key.code == Keyboard::Down && dir != 'U' && turned == false) {
                    dir = 'D';
                    turned = true;
                } else if (event.key.code == Keyboard::Up && dir != 'D' && turned == false) {
                    dir = 'U';
                    turned = true;
                }

                if (event.key.code == Keyboard::Num1) { waiting_time = seconds(0.25); speed = 1; }
                else if (event.key.code == Keyboard::Num2) { waiting_time = seconds(0.2); speed = 2; }
                else if (event.key.code == Keyboard::Num3) { waiting_time = seconds(0.15); speed = 3; }
                else if (event.key.code == Keyboard::Num4) { waiting_time = seconds(0.1); speed = 4; }
                else if (event.key.code == Keyboard::Num5) { waiting_time = seconds(0.05); speed = 5; }
            }
        }


        ///// DRAWING /////
        
        if (clock.getElapsedTime() >= waiting_time)
        {
            switch (dir)
            {
                case 'R': snake_x[0]++; if (snake_x[0] > w-1) snake_x[0] = 0; break;
                case 'L': snake_x[0]--; if (snake_x[0] < 0) snake_x[0] = w-1; break;
                case 'D': snake_y[0]++; if (snake_y[0] > h-1) snake_y[0] = 0; break;
                case 'U': snake_y[0]--; if (snake_y[0] < 0) snake_y[0] = h-1; break;
                default: break;
            }
            turned = false;

            for (int i = 0; i < size-1; i++)
            {
                snake_x[i+1] = tsnake_x[i];
                snake_y[i+1] = tsnake_y[i];

                tsnake_x[i] = snake_x[i];
                tsnake_y[i] = snake_y[i];
            }


            window.clear();


            ///// GOING THROUGH ALL GRID /////

            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    if (j == point_x && i == point_y) {
                        point.setPosition(j*dr, i*dr);
                        window.draw(point);
                    }

                    if (snake_x[0] == point_x && snake_y[0] == point_y)
                    {
                        snake_x.push_back(-2); snake_y.push_back(0);
                        tsnake_x.push_back(snake_x[size-1]); tsnake_y.push_back(snake_y[size-1]);
                        size++; score += 200;

                        FILE *ms = fopen("assets/data.dat", "r+");
                        fscanf(ms, "%d", &tms);
                        fclose(ms);
                        if (tms < score) {
                            FILE *ms = fopen("assets/data.dat", "w+");
                            fprintf(ms, "%d", score);
                            fclose(ms);
                            max_score = score;
                        }

                        if (size == (w * h)) {
                            snake_x.resize(3); snake_y.resize(3);
                            tsnake_x.resize(2); tsnake_y.resize(2);
                            size = 3; score = 0;
                        }
                    }

                    for (int k = 0; k < size; k++)
                    {
                        if (snake_x[k] == j && snake_y[k] == i) {
                            snake.setPosition(j*dr, i*dr);
                            window.draw(snake);
                        }

                        if (k != 0 && snake_x[0] == snake_x[k] && snake_y[0] == snake_y[k]) {
                            snake_x.resize(3); snake_y.resize(3);
                            tsnake_x.resize(2); tsnake_y.resize(2);
                            size = 3; score = 0;
                        }

                        while (snake_x[k] == point_x && snake_y[k] == point_y) {
                            point_x = rand() % (w);
                            point_y = rand() % (h);
                        }
                    }
                }
            }

            sprintf(str1, "Score: %d\nSpeed: %d", score, speed);
            text1.setString(str1);
            window.draw(text1);

            sprintf(str2, "Max score: %d", max_score);
            text2.setString(str2);
            text2.setPosition(Vector2f((W - text2.getGlobalBounds().width - 10), 0));
            window.draw(text2);


            window.display();
            clock.restart();
        }
    }

    return 0;
}
