#include <SFML/Graphics.hpp>
using namespace sf;

float offsetX=0, offsetY=0;

const int H=12;
const int W=40;

int ground =150;

//масив відображає карту гри
//B - wall
//0 - bonus
//40x12
String TileMap[H] =
{

"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B         0000                BBBB     B",
"B                                B     B",
"BBB                              B     B",
"B              BB                BB    B",
"B              BB                      B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};

class Player
{
public:

    float dx,dy; //швидкість
    FloatRect rect;  //x,y,widht,height
    bool onGround; // на землі чи ні
    Sprite sprite;
    float currentFrame;


    Player(Texture &image)
    {
        sprite.setTexture(image);
        rect=FloatRect(7*32,9*32,40,50);

        dx=dy=0.1;
        currentFrame=0; //номер поточного кадру
    }



    void update(float time)
    {
        //координата х
        rect.left += dx*time;

        //зіткнення по x
        Collision(0);


        //якщо ми не на землі, то падаємо з прискоренням
        if(!onGround) dy= dy+0.0005*time;

        //координата y
        rect.top += dy*time;

        onGround=false;

        //зіткнення по y
        Collision(1);


        currentFrame+=0.005*time; //швидкість анімації
        if(currentFrame >=6) currentFrame-=6; //6 кадрів

        //кожного разу квадратик текстури зсувається на 40
        if(dx>0) sprite.setTextureRect(IntRect(40*int(currentFrame),244,40,50));

        //щоб брати картинку дзеркально копіююємо її справа на ліво
        if(dx<0) sprite.setTextureRect(IntRect(40*int(currentFrame)+40,244,-40,50));

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx=0;
    }


    void Collision (int dir)
    {
        for(int i=rect.top/32; i<(rect.top+rect.height)/32; i++) //висота
           for(int j=rect.left/32; j<(rect.left+rect.width)/32; j++) //ширина
            {
               if(TileMap[i][j]=='B')
               {

                   if((dx>0) && (dir==0)) rect.left = j*32 - rect.width;
                   if((dx<0) && (dir==0)) rect.left = j*32+32;

                   if((dy>0) && (dir==1))
                   {
                       rect.top = i*32 - rect.height;
                       dy=0;
                       onGround=true;
                   }

                   if((dy<0) && (dir==1))
                   {
                       rect.top = i*32+32;
                       dy=0;
                   }

               }

               if(TileMap[i][j]=='0')
               {
                   TileMap[i][j] = ' ';
               }

            }
    }
};

int main()
{
     RenderWindow app (VideoMode(600,600), "Test!");

     Texture t;
     t.loadFromFile("D:\\CODE\\MarioSFML\\fang.png");

     float currentFrame = 0;

     Player p(t);

     //привязуєм швидкість переміщення героя до часу
     Clock clock;

    RectangleShape rectangle (Vector2f(32,32));

     while(app.isOpen())
     {
         //в time час що минув
         float time= clock.getElapsedTime().asMicroseconds();
         //кожного разу засікає час що минув
         clock.restart();

         //тут можна регули швидкість гри
         time=time/700;

         if (time>20) time=20;

         Event e;
         while(app.pollEvent(e))
         {
             if (e.type==Event::Closed)
             {
                app.close();
             }
         }


         if(Keyboard::isKeyPressed(Keyboard::Left))
         {
            p.dx= -0.1;
         }

         if(Keyboard::isKeyPressed(Keyboard::Right))
         {
             p.dx= 0.1;
         }

         if(Keyboard::isKeyPressed(Keyboard::Up))
         {
            //тільки якщо ми на землі
             if(p.onGround)
             {
                 p.dy=-0.35;
                 p.onGround=false;
             }
         }

         p.update(time);

         if (p.rect.left>300) offsetX = p.rect.left - 300;
         offsetY = p.rect.top - 200;

         app.clear(Color::White);

         //замальовуємо карту
         for(int i=0; i<H; ++i) //висота
            for(int j=0; j<W; ++j) //ширина
             {
                if(TileMap[i][j]=='B')
                 rectangle.setFillColor(Color::Black);

                 if(TileMap[i][j]=='0')
                 rectangle.setFillColor(Color::Green);

                 if(TileMap[i][j]==' ')
                 continue;

                 rectangle.setPosition(j*32-offsetX,i*32 - offsetY) ;
                 app.draw(rectangle);
            }

         app.draw(p.sprite);
         app.display();

    }
}
