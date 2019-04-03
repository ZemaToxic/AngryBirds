class ElasticBody 
{
public:
	b2Body* bodies[64];
    b2Body*  m_ground;
	b2Body*  m_elev;
	b2PrismaticJoint* m_joint_elev;
	/// Main...
	ElasticBody()
	{
	
	}
	///  Apply dynamic forces (springs) and check elevator state
	void Step(Settings* settings)
	{
		Test::Step(settings);
		for (int i=0; i<8; ++i){
			for (int j=0; j<8; ++j){

				b2Vec2 up(0.0f, 0.5f);

				float32 spring = 500.0f;
				float32 damp = 5.0f;

					AddSpringForce(*(bodies[ind]),up,*(bodies[indd]),down,0.5f*spring,damp,0.0f);
			
	}
   /// Add a spring force
   void AddSpringForce(b2Body& bA, b2Vec2& localA, b2Body& bB, b2Vec2& localB, float32 k, float32 friction, float32 desiredDist)
   {
        b2Vec2 pA = bA.GetWorldPoint(localA);
        b2Vec2 pB = bB.GetWorldPoint(localB);
        b2Vec2 diff = pB - pA;
        //Find velocities of attach points
        b2Vec2 vA = bA.GetLinearVelocity() - b2Cross(bA.GetWorldVector(localA), bA.GetAngularVelocity());
        b2Vec2 vB = bB.GetLinearVelocity() - b2Cross(bB.GetWorldVector(localB), bB.GetAngularVelocity());
        b2Vec2 vdiff = vB-vA;
        float32 dx = diff.Normalize(); //normalizes diff and puts length into dx
        float32 vrel = vdiff.x*diff.x + vdiff.y*diff.y;
        float32 forceMag = -k*(dx-desiredDist) - friction*vrel;
        diff *= forceMag; // diff *= forceMag
        bB.ApplyForce(diff, bA.GetWorldPoint(localA));
        diff *= -1.0f;
        bA.ApplyForce(diff, bB.GetWorldPoint(localB));
    }
};