#include "orangeportal.h"
#include <cstddef>
#include <cstring>

//Constructor
ORANGEPORTAL::ORANGEPORTAL(OBJHANDLE hVessel, int flightmodel):VESSEL4(hVessel, flightmodel){

    mhPortal = NULL;

    uimesh_Portal = 0;

    vs2 = {0};

    orange_dock1 = NULL;

    orange_dock2 = NULL;
}

//Destructor
ORANGEPORTAL::~ORANGEPORTAL(){

}

void ORANGEPORTAL::clbkSetClassCaps(FILEHANDLE cfg){

    const char *meshname = "orange_portal";

    mhPortal = oapiLoadMeshGlobal(meshname);
    uimesh_Portal = AddMesh(mhPortal);

    SetSize(2);
    SetEmptyMass(50);
    SetTouchdownPoints(tdvtx, ntdvtx);

    orange_dock1 = CreateDock(orange_DCK1_Location, FORWARD_DIRECTION, OrangePortal_DOCK_ROT);

    orange_dock2 = CreateDock(orange_DCK1_Location, FORWARD_DIRECTION, OrangePortal_DOCK_ROT);    

}

VECTOR3 ORANGEPORTAL::ReturnBluePortalPos() {

    VECTOR3 emptyvector = _V(0, 0, 0);

    OBJHANDLE blue_portal = oapiGetVesselByName("BLUEPORTAL");
    if (!blue_portal) {
        oapiWriteLog("BLUEPORTAL not found!");
        return emptyvector;
    }

    // Obtener la interfaz del portal naranja
    VESSEL *blue_portal_vessel = oapiGetVesselInterface(blue_portal);
    if (!blue_portal_vessel) {
        oapiWriteLog("Failed to get interface for BLUEPORTAL!");
        return emptyvector;
    }

    // Obtener la referencia gravitacional (cuerpo central)
    OBJHANDLE ref_body = blue_portal_vessel->GetGravityRef();
    if (!ref_body) {
        oapiWriteLog("Reference body not found!");
        return emptyvector;
    }

    // Obtener la posición global del portal azul y su cuerpo central
    VECTOR3 blue_portal_global, ref_body_global;
    oapiGetGlobalPos(blue_portal, &blue_portal_global);
    oapiGetGlobalPos(ref_body, &ref_body_global);

    // Calcular la posición relativa al cuerpo central
    VECTOR3 blue_portal_relative = blue_portal_global - ref_body_global;

    oapiWriteLogV("Orange Portal relative position: (%f, %f, %f)", 
                  blue_portal_relative.x, blue_portal_relative.y, blue_portal_relative.z);

    return blue_portal_relative;

}



OBJHANDLE ORANGEPORTAL::ReturnNearestVessel() {
    VECTOR3 vslPos;
    OBJHANDLE nearestVessel = nullptr;
    char vesselName[256];

    double minDistance = std::numeric_limits<double>::max(); //Starts with the maximum distance possible.

    for (int i = 0; i < oapiGetVesselCount(); i++) {
        OBJHANDLE hVessel = oapiGetVesselByIndex(i);

        //Ignore own vessel (portal vessel)
        if (hVessel == GetHandle()) {
            continue;
        }

        //Get relative position from vessel to portal vessel
        oapiGetRelativePos(GetHandle(), hVessel, &vslPos);
        double targetDistance = length(vslPos);

        //Update index if is in the target radius of 2.1m
        if (targetDistance < 2.1 && targetDistance < minDistance) {
            minDistance = targetDistance;
            nearestVessel = hVessel;
            oapiGetObjectName(hVessel, vesselName, sizeof(vesselName));
            oapiWriteLogV("Nearest %s vessel found", vesselName);
        }
    }

    return nearestVessel;

}

void ORANGEPORTAL::TeleportVesselToBluePortal() {

    OBJHANDLE teleport_target = ReturnNearestVessel();
    VECTOR3 blue_portal_pos = ReturnBluePortalPos();

    if (teleport_target != nullptr) {
        // Obtener la interfaz de la nave objetivo
        VESSEL *target_vessel = oapiGetVesselInterface(teleport_target);
        if (target_vessel == nullptr) {
            return;
        }

        // Obtener el estado actual de la nave
        memset(&vs2, 0, sizeof(vs2));
        vs2.version = 2;
        target_vessel->GetStatusEx(&vs2);

        // Cambiar la posición global
        vs2.rpos = blue_portal_pos;

        // Asegurar que la velocidad sea cero relativa al portal naranja
        vs2.rvel = _V(0, 0, 0); // Puede ajustarse según el contexto.

        // Aplicar el nuevo estado a la nave objetivo
        target_vessel->DefSetStateEx(&vs2);

        // Log para confirmar la teletransportación
        oapiWriteLog("Vessel teleported to Blue Portal!");
    }

}


void ORANGEPORTAL::clbkLoadStateEx(FILEHANDLE scn, void *vs){

    char *line;

	while(oapiReadScenario_nextline(scn,line)){

		ParseScenarioLineEx(line,vs);

	}

}

void ORANGEPORTAL::clbkPostCreation(){

}

void ORANGEPORTAL::clbkPreStep(double simt, double simdt, double mjd){

    ReturnNearestVessel();
    TeleportVesselToBluePortal();

}

void ORANGEPORTAL::clbkPostStep(double simt, double simdt, double mjd){

    return;

}

void ORANGEPORTAL::clbkSaveState(FILEHANDLE scn){

    SaveDefaultState(scn);

}

DLLCLBK void InitModule(MODULEHANDLE hModule){

}

DLLCLBK void ExitModule(MODULEHANDLE *hModule){

}

///////////////Vessel initialization

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel){
    
	return new ORANGEPORTAL(hvessel, flightmodel);

}

/////////////Vessel memory cleanup
DLLCLBK void ovcExit(VESSEL *vessel){
    
	if(vessel) delete(ORANGEPORTAL*)vessel;
	
}