#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"

#include "xNetInterface.h"

#include "tnlRandom.h"
#include "tnlSymmetricCipher.h"
#include "tnlAsymmetricKey.h"

#include "xNetworkTypes.h"

extern xNetInterface* GetNetInterfaceServer();
extern xNetInterface *GetNetInterfaceClient();
extern void SetNetInterfaceClient(xNetInterface *cInterface);
extern void SetNetInterfaceServer(xNetInterface *cInterface);


void vtNetworkManager::SetClientNetInterface(xNetInterface *cinterface) {	SetNetInterfaceClient(cinterface);}
void vtNetworkManager::SetServerNetInterface(xNetInterface *cinterface){	 SetNetInterfaceServer(cinterface); }


xNetInterface* vtNetworkManager::GetServerNetInterface() { return GetNetInterfaceServer(); }
xNetInterface*vtNetworkManager::GetClientNetInterface(){ 	return GetNetInterfaceClient(); }


////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void  vtNetworkManager::DeleteServer(int flags)
{



}
