///////////////////////////////////////////////////////////
//  dSleepingSettings.h
//  Implementation of the Class dSleepingSettings
//  Created on:      18-Jan-2008 17:15:31
///////////////////////////////////////////////////////////

#if !defined(EA_7D481C15_2B93_4925_B66E_6FAF4DED2A2A__INCLUDED_)
#define EA_7D481C15_2B93_4925_B66E_6FAF4DED2A2A__INCLUDED_

namespace vtAgeia
{


class pSleepingSettings
{

public:
	pSleepingSettings()
	{
		m_AngularThresold = 0.1f;
		m_LinearThresold = 0.1f;
		m_SleepSteps = 0 ; 
		m_AutoSleepFlag = 1;
		m_SleepTime = 0.0f;
	}
	virtual ~pSleepingSettings(){}
	
	int SleepSteps() const { return m_SleepSteps; }
	void SleepSteps(int val) { m_SleepSteps = val; }
	
	float AngularThresold() const { return m_AngularThresold; }
	void AngularThresold(float val) { m_AngularThresold = val; }
	int AutoSleepFlag() const { return m_AutoSleepFlag; }
	void AutoSleepFlag(int val) { m_AutoSleepFlag = val; }

	float LinearThresold() const { return m_LinearThresold; }
	void LinearThresold(float val) { m_LinearThresold = val; }



	float m_AngularThresold;
	int m_AutoSleepFlag;
	float m_LinearThresold;
	int m_SleepSteps;
	float m_SleepTime;

};

}
#endif // !defined(EA_7D481C15_2B93_4925_B66E_6FAF4DED2A2A__INCLUDED_)
