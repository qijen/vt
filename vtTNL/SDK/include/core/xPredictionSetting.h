#ifndef __xPredictionSetting_h_
#define __xPredictionSetting_h_

class xPredictionSetting
{
	public:
		xPredictionSetting()
		{
			m_MinSendTime =0.0f;
			m_MinDifference =0.0f;
			m_PredictionFactor = 0.0f;

		}
		
		float getMinSendTime() const { return m_MinSendTime; }
		void setMinSendTime(float val) { m_MinSendTime = val; }

		float getMinDifference() const { return m_MinDifference; }
		void setMinDifference(float val) { m_MinDifference = val; }
		float getPredictionFactor() const { return m_PredictionFactor; }
		void setPredictionFactor(float val) { m_PredictionFactor = val; }

	private:

		float m_MinSendTime;
		float m_MinDifference;
		float m_PredictionFactor;
		
		
};
#endif
