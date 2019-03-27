#ifndef IFORCE2D_COLLISION_FILTERING_H
#define IFORCE2D_COLLISION_FILTERING_H

#include "../Header/utils.h"
#include <vector>

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

//This is just to avoid conflicting names in other tutorial source files.
#define Ball COLLISION_FILTERING_Ball
#define entityCategory COLLISION_FILTERING_entityCategory

enum entityCategory {
    BOUNDARY =          0x0001,
    FRIENDLY_SHIP =     0x0002,
    ENEMY_SHIP =        0x0004,
    FRIENDLY_AIRCRAFT = 0x0008,
    ENEMY_AIRCRAFT =    0x0010,
};

bool addedGroupIndex = false;

class Ball {
public:
    b2Body* m_body;
protected:
    //class member variables
    float m_radius;
    b2Color m_color;

public:
    Ball(b2World* world, float radius, b2Color color, uint16 categoryBits, uint16 maskBits) {
        m_color = color;
        m_body = NULL;
        m_radius = radius;

        //set up dynamic body, store in class variable
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.position.Set(0, 20);
        m_body = world->CreateBody(&myBodyDef);
        m_body->SetAngularDamping(1);
        m_body->SetLinearDamping(1);

        //add circle fixture
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0);
        circleShape.m_radius = m_radius; //use class variable
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape;
        myFixtureDef.density = 1;
        myFixtureDef.filter.categoryBits = categoryBits;
        myFixtureDef.filter.maskBits = maskBits;
        if ( !addedGroupIndex ) //a little hack to make just one fixture groupIndex of -1
            myFixtureDef.filter.groupIndex = -1;
        addedGroupIndex = true;
        m_body->CreateFixture(&myFixtureDef);
    }
    ~Ball() {}

    //in Ball class
    void renderAtBodyPosition() {

        //get current position from Box2D
        b2Vec2 pos = m_body->GetPosition();
        float angle = m_body->GetAngle();

        //call normal render at different position/rotation
        glPushMatrix();
        glTranslatef( pos.x, pos.y, 0 );
        glRotatef( angle * RADTODEG, 0, 0, 1 );
        glScalef( m_radius, m_radius, 1 );
        render();
        glPopMatrix();
    }

    void render() {

        glColor3f(m_color.r, m_color.g, m_color.b);

        //nose and eyes
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex2f( 0, 0 );
        glVertex2f(-0.5, 0.5 );
        glVertex2f( 0.5, 0.5 );
        glEnd();

        //mouth
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5,  -0.5 );
        glVertex2f(-0.16, -0.6 );
        glVertex2f( 0.16, -0.6 );
        glVertex2f( 0.5,  -0.5 );
        glEnd();

        //circle
        glBegin(GL_LINE_LOOP);
        for (float a = 0; a < 360 * DEGTORAD; a += 30 * DEGTORAD)
            glVertex2f( sinf(a), cosf(a) );
        glEnd();
    }
};

class iforce2d_collision_filtering : public Test
{
public:

    iforce2d_collision_filtering() {
        //a static body
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);
        b2Body* staticBody = m_world->CreateBody(&myBodyDef);

        //shape definition
        b2PolygonShape polygonShape;

        //fixture definition
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;

        myFixtureDef.filter.groupIndex = -1;

        //add four walls to the static body
        polygonShape.SetAsBox( 20, 1, b2Vec2(0, 0), 0);//ground
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 20, 1, b2Vec2(0, 40), 0);//ceiling
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 1, 20, b2Vec2(-20, 20), 0);//left wall
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 1, 20, b2Vec2(20, 20), 0);//right wall
        staticBody->CreateFixture(&myFixtureDef);

        //add ball entities to scene
        b2Color red(1,0,0);
        b2Color green(0,1,0);
        addedGroupIndex = false;

        //large and green are friendly ships
        for (int i = 0; i < 3; i++)
            m_balls.push_back( new Ball(m_world, 3, green,
                                        FRIENDLY_SHIP, //categoryBits = "I am a ..."
                                        BOUNDARY | FRIENDLY_SHIP | ENEMY_SHIP ) ); //maskBits = "I will collide with a ..."

        //large and red are enemy ships
        for (int i = 0; i < 3; i++)
            m_balls.push_back( new Ball(m_world, 3, red,
                                        ENEMY_SHIP,
                                        BOUNDARY | FRIENDLY_SHIP | ENEMY_SHIP ) );

        //small and green are friendly aircraft
        for (int i = 0; i < 3; i++)
            m_balls.push_back( new Ball(m_world, 1, green,
                                        FRIENDLY_AIRCRAFT,
                                        BOUNDARY | ENEMY_AIRCRAFT ) );

        //small and red are enemy aircraft
        for (int i = 0; i < 3; i++)
            m_balls.push_back( new Ball(m_world, 1, red,
                                        ENEMY_AIRCRAFT,
                                        BOUNDARY | FRIENDLY_AIRCRAFT ) );

        m_world->SetGravity(b2Vec2(0,0));

    }

    ~iforce2d_collision_filtering()
    {
        for (int i = 0; i < m_balls.size(); i++)
            delete m_balls[i];
    }

    void Step(Settings* settings)
    {
        //run the default physics and rendering
        Test::Step(settings);

        for (int i = 0; i < m_balls.size(); i++)
            m_balls[i]->renderAtBodyPosition();

        //show some text in the main screen
        //m_debugDraw.DrawString(5, m_textLine, "Now we have a foo test");
        //m_textLine += 15;

    }

    static Test* Create()
    {
        return new iforce2d_collision_filtering;
    }

    //class member variables
    std::vector<Ball*> m_balls;

};

//This is just to avoid conflicting names in other tutorial source files.
#undef Ball
#undef entityCategory

#endif
