#ifndef __vtDistributedProperty_h_
#define __vtDistributedProperty_h_

class vtDistributedProperty
{
	public:

		vtDistributedProperty();
		vtDistributedProperty(int type);
		virtual ~vtDistributedProperty();

		XString GetName() const { return m_Name; }
		void SetName(XString val) { m_Name = val; }

		DWORD GetType() const { return m_Type; }
		void SetType(DWORD val) { m_Type = val; }

	protected:
		XString m_Name;
		DWORD m_Type;
		
	private:
};
#endif