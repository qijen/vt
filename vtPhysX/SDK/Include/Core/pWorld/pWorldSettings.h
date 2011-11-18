#if !defined(EA_45F10106_0809_49c3_8B43_1556D139853F__INCLUDED_)
#define EA_45F10106_0809_49c3_8B43_1556D139853F__INCLUDED_

namespace vtAgeia
{

	class pWorldSettings
	{

		
	public:

		
		pWorldSettings();
		
		VxVector getGravity() const{

			return  m_Gravity;
		}
		void setGravity(VxVector val){
			m_Gravity = val;
		}
		float getSkinWith() const{
			return m_SkinWith;
		}
		void setSkinWith(float val)
		{
			m_SkinWith = val;
		}

		bool isFixedTime() const { return fixedTime; }
		void setFixedTime(bool val) { fixedTime = val; }
		
		int getNumIterations() const { return numIterations; }
		void setNumIterations(int val) { numIterations = val; }
		
		float getFixedTimeStep() const { return fixedTimeStep; }
		void setFixedTimeStep(float val) { fixedTimeStep = val; }
		int getSceneFlags() const { return sceneFlags; }
		void setSceneFlags(int val) { sceneFlags = val; }

		int getPhysicFlags() const { return physicFlags; }
		void setPhysicFlags(int val) { physicFlags = val; }


	protected:

		float m_SkinWith;
		VxVector m_Gravity;

		float fixedTimeStep;
		int numIterations;
		bool fixedTime;
		int sceneFlags;
		int physicFlags;
		
	};

	
}
#endif // !defined(EA_45F10106_0809_49c3_8B43_1556D139853F__INCLUDED_)
