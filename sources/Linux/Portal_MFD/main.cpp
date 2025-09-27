#include <cstddef>
#include <cstring>
#include <string>
#define STRICT
#define ORBITER_MODULE
#include "main.hpp"
#include <format>

//Global variables
std::string targetRefBody;
std::string targetBase;

PortalMFD::PortalMFD(int w, int h, VESSEL *vessel) : MFD2(w, h, vessel){

    currentRelPos = _V(0, 0, 0);

    currentShip = nullptr;

    selectedBody = nullptr;

    selectedBase = nullptr;

    longitude = 0.0;

    latitude = 0.0;

    altitude = 0.0;

    selectedBasePos = _V(0, 0, 0);

    portalcreated = false;
}

PortalMFD::~PortalMFD(){
}

OAPI_MSGTYPE PortalMFD::MsgProc (MFD_msg msg, MfdId mfd, MFDMODEOPENSPEC *spec, VESSEL *vessel){
	
	switch (msg) {
	case OAPI_MSG_MFD_OPENEDEX:
		return (OAPI_MSGTYPE)new PortalMFD(spec->w, spec->h, vessel);
	}

	return 0;
}

bool PortalMFD::ConsumeButton(int bt, int event){

    if(!(event & PANEL_MOUSE_LBDOWN)){
        return false;
    }

    static const int btkey[4] = {
        OAPI_KEY_R,
        OAPI_KEY_T,
        OAPI_KEY_C,
        OAPI_KEY_O
    };

    if(bt < 4){
        return ConsumeKeyBuffered(btkey[bt]);
    } else {
        return false;
    }

}

bool PortalMFD::ConsumeKeyBuffered(int key){

    bool RefBodyInput(void *id, const char *str, void *data);
    bool BaseInput(void *id, const char *str, void *data);

    switch(key){
        case OAPI_KEY_R:
            oapiOpenInputBox(const_cast<char *>("Target celestial body"), RefBodyInput, 0, 20, (void *)this);
            return true;
        
        case OAPI_KEY_T:
            oapiOpenInputBox(const_cast<char *>("Target base"), BaseInput, 0, 20, (void *)this);
            return true;
        
        case OAPI_KEY_C:
            DeletePortals();
            return true;

        case OAPI_KEY_O:
            OpenPortals();
            return true;
    }
    
    return false;
}

const char *PortalMFD::ButtonLabel(int bt){

    static const char *label[4] = {
        "REF", "TGT", "CLS", "OPN"
    };

    return (bt < 4 ? const_cast<char *>(label[bt]) : 0);
}

int PortalMFD::ButtonMenu(const MFDBUTTONMENU **menu)const {

    static const MFDBUTTONMENU mnu[4] = {
        {"Select celestial body Reference", 0, 'R'},
        {"Select Target base", 0, 'T'},
        {"Close / delete portals", 0, 'C'},
        {"Open portals", 0, 'O'}
    };

    if(menu){
        *menu = mnu;
    }

    return 4;
}

bool PortalMFD::Update(oapi::Sketchpad *skp){

    std::string title = "PortalMFD";
    std::string portalstatus;

    skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::TOP);

    Title(skp, title.c_str());

    planet = std::format("Selected planet: {}", targetRefBody);

    base = std::format("Selected base: {}", targetBase);

    skp->Text(10, 50, planet.c_str(), planet.size());
    skp->Text(10, 100, base.c_str(), base.size());

    if(portalcreated){
        portalstatus = "PORTALS OPEN";
    } else {
        portalstatus = "PORTALS CLOSED";
    }

    skp->Text(100, 250, portalstatus.c_str(), portalstatus.size());

    return true;

}

bool RefBodyInput(void *id, const char *str, void *data){

    targetRefBody = str;

    if(!((PortalMFD *)data)->SetRefBody(targetRefBody)){
        targetRefBody.clear();
        return false;
    }

    return true;

}

bool BaseInput(void *id, const char *str, void *data){

    targetBase = str;

    if(!((PortalMFD *)data)->SetBase(targetBase)){
        targetBase.clear();
        return false;
    }

    return true;

}

bool PortalMFD::SetRefBody(const std::string refBody){

    selectedBody = oapiGetGbodyByName(refBody.c_str());

    if(selectedBody != nullptr){
        return true;
    } else {
        return false;
    }

}

bool PortalMFD::SetBase(const std::string base){

    selectedBase = oapiGetBaseByName(selectedBody, base.c_str());

    if(selectedBase != nullptr){
        return true;
    } else {
        return false;
    }

}

bool PortalMFD::OpenPortals(){

    if (!selectedBase) {
        oapiWriteLog("PortalMFD - ERROR: no base found");
        return false;
    }

    // 1) Obtener el planeta donde está la base (handle del cuerpo necesario)
    OBJHANDLE planet = oapiGetBasePlanet(selectedBase);
    if (!planet) {
        oapiWriteLog("PortalMFD - ERROR: base has no associated planet");
        return false;
    }

    // 2) Obtener lat/lon de la base y convertir a GLOBAL (usar el planeta correctamente)
    oapiGetBaseEquPos(selectedBase, &longitude, &latitude);
    altitude = 10.0; // metros sobre la superficie
    oapiEquToGlobal(planet, longitude, latitude, altitude, &selectedBasePos);

    // 3) Preparar VESSELSTATUS2 "base" copiándolo de una vessel válida en el mismo cuerpo
    VESSELSTATUS2 base_vs = {0}, ship_vs = {0};
    base_vs.version = 2;
    ship_vs.version = 2;

    VESSEL *portalgunship = oapiGetFocusInterface();
    if (portalgunship) {
        portalgunship->GetStatusEx(&ship_vs);
    }

    base_vs.rbody = selectedBody;
    base_vs.base = selectedBase;
    base_vs.surf_lat = latitude;
    base_vs.surf_lng = longitude;
    base_vs.surf_hdg = 0.0;
    base_vs.status = 1;

    // 4) Crear estados para Blue y Orange (copiando de estados válidos)
    VESSELSTATUS2 vs_blue = ship_vs;   // se usará la nave foco como base para el blue portal
    VESSELSTATUS2 vs_orange = base_vs; // vs_orange tiene rref coherente para el planeta de la base

    // Ajustes de spawn:
    if (portalgunship) {
        // Blue: 50 m sobre la nave foco
        vs_blue.rpos = ship_vs.rpos;
        vs_blue.rpos.z += 50.0;
        vs_blue.rvel = _V(0, 0, 0); // para pruebas (o ajustar a velocidad local)
    }

    // Orange: en la posición global de la base (ya calculada)
    //vs_orange.rpos = selectedBasePos;
    // Para evitar que salga disparado: usar la velocidad local tomada de base_vs
    // (normalmente base_vs.rvel corresponde a la velocidad apropiada en ese cuerpo)
    // Si querés que quede quieto relativo a la superficie, poner rvel = base_vs.rvel
    vs_orange.rvel = vs_blue.rvel;

    // 5) Crear las vessels con estados coherentes
    OBJHANDLE hBlue = oapiCreateVesselEx("BLUEPORTAL", "BluePortal", &vs_blue);
    portalcreated = true;
    if (!hBlue) {
        oapiWriteLog("PortalMFD - ERROR: Failed to create BLUEPORTAL");
        portalcreated = false;
        return false;
    }

    OBJHANDLE hOrange = oapiCreateVesselEx("ORANGEPORTAL", "OrangePortal", &vs_orange);
    if (!hOrange) {
        oapiWriteLog("PortalMFD - ERROR: Failed to create ORANGEPORTAL; deleting blue");
        oapiDeleteVessel(hBlue);
        return false;
    }

    oapiWriteLog("PortalMFD - INFO: Portals opened successfully on base.");
    return true;
}



bool PortalMFD::DeletePortals(){

    OBJHANDLE blueportal, orangeportal;

    blueportal = oapiGetVesselByName("BLUEPORTAL");

    orangeportal = oapiGetVesselByName("ORANGEPORTAL");

    if(!blueportal && !orangeportal){
        oapiWriteLog("BLUEPORTAL or ORANGEPORTAL not found");
        return false;
    } else if (blueportal && orangeportal){

        oapiDeleteVessel(blueportal);
        oapiDeleteVessel(orangeportal);

        portalcreated = false;

        blueportal = nullptr;
        orangeportal = nullptr;

    }
    
    return true;
}

DLLCLBK void InitModule(MODULEHANDLE hDLL){
    static char *name = const_cast<char *>("Portal MFD");
    MFDMODESPEC spec;

    spec.name = name;
    spec.key = OAPI_KEY_P;
    spec.msgproc = PortalMFD::MsgProc;

    g_PortalMFD.mode = oapiRegisterMFDMode(spec);
}

DLLCLBK void opcDLLExit(MODULEHANDLE hDLL){
    
    oapiUnregisterMFDMode(g_PortalMFD.mode);

}