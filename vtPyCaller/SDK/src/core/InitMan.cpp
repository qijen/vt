#include "StdAfx.h"
#include "InitMan.h"
#include "vt_python_funcs.h"
//#include "VSLManagerSDK.h"
/*#include <iostream>
#include "pyembed.h"
*/

extern vt_python_man *pym;
//////////////////////////////////////////////////////////////////////////
vt_python_man::~vt_python_man(){}
//////////////////////////////////////////////////////////////////////////

vt_python_man*
vt_python_man::GetPyMan()
{

	return pym;
}


//************************************
// Method:    PostProcess
// FullName:  vt_python_man::PostProcess
// Access:    public 
// Returns:   CKERROR
// Qualifier:
//************************************
CKERROR
vt_python_man::PostProcess()
{
	if (m_stdErr.Length())
	{
		
		m_Context->OutputToConsoleEx("\nPyErr:%s",m_stdErr.Str(),FALSE);
		m_stdErr ="";
	}
	
	if (m_stdOut.Length())
	{
		m_Context->OutputToConsoleEx("\nPyOut:%s",m_stdOut.Str(),FALSE);
		m_stdOut="";
		
	}
	return CK_OK;
}

//************************************
// Method:    PreProcess
// FullName:  vt_python_man::PreProcess
// Access:    public 
// Returns:   CKERROR
// Qualifier:
//************************************
CKERROR
vt_python_man::PreProcess()
{
	//m_Context->OutputToConsoleEx("Py std::error : %s",m_stdErr);
	//m_Context->OutputToConsoleEx("Py std::out : %s",m_stdOut);
	return CK_OK;
}

CKERROR
vt_python_man::PostClearAll(){
	return CK_OK;
}
//////////////////////////////////////////////////////////////////////////

CKERROR vt_python_man::OnCKEnd(){

	return CK_OK;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR vt_python_man::OnCKReset(){

	/*if(py)
	{
		delete py;
	
		py = NULL;
	}*/
	ClearModules();
	DestroyPython();
	return CK_OK;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CKERROR vt_python_man::OnCKPlay()
{
	
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR vt_python_man::OnCKInit(){
	
	m_Context->ActivateManager((CKBaseManager*) this,true);	
/*
	int argc = 1;
	char *cmd =GetCommandLineA();
	char *c1[1];
	c1[0]=new char[5];
	strcpy(c1[0],cmd);*/
	//py = new Python(argc,c1);

	/*if(py)
	{
		delete py;
		py = NULL;
	}*/
	//RegisterVSL();
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
