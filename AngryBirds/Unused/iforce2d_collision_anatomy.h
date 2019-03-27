#ifndef IFORCE2D_COLLISION_ANATOMY_H
#define IFORCE2D_COLLISION_ANATOMY_H

#include <stdio.h>
#include "../Header/utils.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

class iforce2d_collision_anatomy : public Test
{
public:

    iforce2d_collision_anatomy() {

        //zero gravity
        m_world->SetGravity(b2Vec2(0,0));

        //bodies setup
        {
            //dynamic body
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;

            //fixture definition
            b2PolygonShape polygonShape;
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &polygonShape;
            fixtureDef.density = 1;

            //square body
            polygonShape.SetAsBox(3,3);//6x6 box
            bodyDef.position.Set(0, 20);
            bodyDef.angle = 45 * DEGTORAD;
            m_squareBody = m_world->CreateBody(&bodyDef);
            m_squareBody->CreateFixture(&fixtureDef);

            //triangle body
            b2Vec2 points[3];
            points[0].Set( 0, 0);
            points[1].Set( 2, 6);
            points[2].Set(-5, 3);
            polygonShape.Set( points, 3 );
            bodyDef.position.Set(-15, 24.1f);//corner vs corner
            //bodyDef.position.Set(-15, 23.5f);//corner vs edge
            bodyDef.angle = 0;
            m_triangleBody = m_world->CreateBody(&bodyDef);
            m_triangleBody->CreateFixture(&fixtureDef);
        }

        m_triangleBody->SetLinearVelocity( b2Vec2(30,0) );//corner vs corner
        //m_triangleBody->SetLinearVelocity( b2Vec2(30.2f,0) );//corner vs corner, reverse side
        //m_triangleBody->SetBullet(true);

        m_lastContact = NULL;
    }

    //contact listener functions
    void BeginContact(b2Contact* contact) {
        m_lastContact = contact;
        printf("BeginContact\n");
        fflush(stdout);
    }
    void EndContact(b2Contact* contact) {
        m_lastContact = contact;
        printf("EndContact\n");
        fflush(stdout);
    }
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
        m_lastContact = contact;
        if ( !m_paused || m_singleStep )
            printf("PreSolve\n");
        fflush(stdout);
    }
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
        m_lastContact = contact;
        printf("PostSolve\n");
        fflush(stdout);
    }

    void Step(Settings* settings)
    {
        m_paused = settings->pause;
        m_singleStep = settings->singleStep;

        if ( !m_paused || m_singleStep )
            m_lastContact = NULL;

        if ( !m_paused || m_singleStep )
            printf("Step\n");
        fflush(stdout);

        //run the default physics and rendering
        Test::Step(settings);

        if ( m_lastContact )
        {
            //normal manifold contains all info...
            int numPoints = m_lastContact->GetManifold()->pointCount;

            //...world manifold is helpful for getting locations
            b2WorldManifold worldManifold;
            m_lastContact->GetWorldManifold( &worldManifold );

            //draw collision normal
            float normalLength = 0.1f;
            b2Vec2 normalStart = worldManifold.points[0] - normalLength * worldManifold.normal;
            b2Vec2 normalEnd = worldManifold.points[0] + normalLength * worldManifold.normal;
            glBegin(GL_LINES);
            glColor3f(1,0,0);//red
            glVertex2f( normalStart.x, normalStart.y );
            glColor3f(0,1,0);//green
            glVertex2f( normalEnd.x, normalEnd.y );
            glEnd();

            //draw collision points
            glColor3f(1,1,0);
            glPointSize(6);
            glBegin(GL_POINTS);
            for (int i = 0; i < numPoints; i++)
                glVertex2f(worldManifold.points[i].x, worldManifold.points[i].y);
            glEnd();
        }

        //show some text in the main screen
        m_debugDraw.DrawString(5, m_textLine, "Use single step and watch the stdout terminal to see contact listener events");
        m_textLine += 15;

    }

    static Test* Create()
    {
        return new iforce2d_collision_anatomy;
    }

    //class member variables
    b2Body* m_squareBody;
    b2Body* m_triangleBody;

    //keep a reference to the last contact
    const b2Contact* m_lastContact;

    //just to avoid screeds of "PreSolve" showing in stdout when paused
    bool m_paused;
    bool m_singleStep;

};

//This is just to avoid conflicting names in other tutorial source files.
#undef Ball
#undef entityCategory

#endif
