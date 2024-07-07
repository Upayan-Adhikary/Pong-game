#include <iostream>
#include <raylib.h>

using namespace std;

    float screen_width = 1920 ;
    float screen_height = 1080 ;

    int cpu_score = 0;
    int player_score = 0;

    class Ball
    {
        public:
        float x,y;
        int radius;
        int speed_x, speed_y;
        Color c;

            void Draw()
            {
                DrawCircle(x, y, radius, c);
            }

            void update()
            {
                x = x + speed_x;
                y = y + speed_y;

                
                if(x + radius >= GetScreenWidth() ) 
                {
                    player_score++;
                    ResetBall();
                }
                if( x - radius <= 0)
                {
                    cpu_score++;
                    ResetBall();
                }

                if(y + radius >= GetScreenHeight() || y - radius <= 0)
                {
                    speed_y = speed_y*(-1);
                }
                

            }

            void ResetBall()
            {
                x = GetScreenWidth()/2;
                y = GetScreenHeight()/2;

                int Val[2] = {-1,1};
                speed_x *= Val[GetRandomValue(0,1)];
                speed_y *= Val[GetRandomValue(0,1)];

            } 

    };

    class Paddle
    {

        protected:
        void LimitMovement(){

            if(y <= 0 ) 
            {
              y = 0;  
            }
            if(y >= (screen_height - height) ) 
            {
              y = (screen_height - height);  
            }

        }

        public:
        float x, y;
        float width, height;
        int speed;
        Color c;

        void Draw()
        {
            DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, c);
      
            
        }
        
        void update()
        {
            if(IsKeyDown(KEY_UP)){
                y = y - speed ;
            }

            if(IsKeyDown(KEY_DOWN)){
                y = y + speed ;
            }
            LimitMovement();            
        }

    };
 


    class Cpupad: public Paddle {

        public:

        void update(int ball_y)
        {
            if(y + height/2 > ball_y)
            {
                y = y - speed;
            }
            if(y + height/2 < ball_y)
            {
                y = y + speed;
            }
            LimitMovement();

        }
    };

   //
    Paddle Lpad;
   // 
    Ball ball;
   //
    Cpupad Cpad;


int main () {

    
    InitWindow(screen_width, screen_height, "A.I. PONG");
    InitAudioDevice();
    SetTargetFPS (120);

    //Value added to class variables
    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 8;
    ball.speed_y = 8;
    ball.c = WHITE;

    Lpad.width = 30;
    Lpad.height = 120;
    Lpad.x = 10;
    Lpad.y = (screen_height/2) - (Lpad.height/2); 
    Lpad.speed = 12;
    Lpad.c = GREEN;

    Cpad.width = 30;
    Cpad.height = 120;
    Cpad.x = screen_width - (Lpad.x + Lpad.width);
    Cpad.y = (screen_height/2) - (Cpad.height/2);
    Cpad.speed = 12;
    Cpad.c = BLUE;
    
    //MUSIC files load and play
    Music fxm, fxm2;
    fxm = LoadMusicStream("resources/New_mario.ogg");
    fxm2 = LoadMusicStream("resources/smb_bump2.ogg");
    PlayMusicStream(fxm);
    PlayMusicStream(fxm2);
    
    //GAME LOOP
    while(WindowShouldClose() == false)
    {
     
       UpdateMusicStream(fxm);
       BeginDrawing();
       ClearBackground(BLACK);
        
        //Line draw
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, YELLOW);
        
        //ball
        ball.Draw();
        ball.update();
           
        //Left rectangle
        Lpad.Draw();
        Lpad.update();
        
        
        //Right rectangle CPU
        Cpad.Draw();
        Cpad.update(ball.y);

        //Check for collision with paddles and then move the ball to a random x,y position
        int nval[2] = {-1,1};
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{Lpad.x, Lpad.y, Lpad.width, Lpad.height}))
        {
            UpdateMusicStream(fxm2);
            
            ball.speed_x *= nval[GetRandomValue(0,1)]; 
            ball.speed_y *= nval[GetRandomValue(0,1)]; 
            
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{Cpad.x, Cpad.y, Cpad.width, Cpad.height})) 
        {
            UpdateMusicStream(fxm2);

            ball.speed_x *= nval[GetRandomValue(0,1)]; 
            ball.speed_y *= nval[GetRandomValue(0,1)]; 
        }

        //Score Display
        DrawText(TextFormat("%i",player_score), screen_width/4 -20, 20, 80, RED);
        DrawText(TextFormat("%i",cpu_score), 3 * screen_width/4 -20, 20, 80, RED);


        EndDrawing();
    }

    UnloadMusicStream(fxm2);
    UnloadMusicStream(fxm);


    CloseAudioDevice();         
    CloseWindow();
    return 0;
}