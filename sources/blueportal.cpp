#include "blueportal.h"
#include <cstddef>
#include <cstring>

//Constructor
BLUEPORTAL::BLUEPORTAL(OBJHANDLE hVessel, int flightmodel):VESSEL4(hVessel, flightmodel){

    mhPortal = NULL;

    uimesh_Portal = 0;

    vs2 = {0};

}

//Destructor
BLUEPORTAL::~BLUEPORTAL(){

}

void BLUEPORTAL::clbkSetClassCaps(FILEHANDLE cfg){

    const char *meshname = "blue_portal";

    mhPortal = oapiLoadMeshGlobal(meshname);
    uimesh_Portal = AddMesh(mhPortal);

    SetSize(2);
    SetEmptyMass(50);
    SetTouchdownPoints(tdvtx, ntdvtx);

    blue_dock1 = CreateDock(blue_DCK1_Location, FORWARD_DIRECTION, BluePortal_DOCK_ROT);

    blue_dock2 = CreateDock(blue_DCK1_Location, FORWARD_DIRECTION, BluePortal_DOCK_ROT);    
    
}

VECTOR3 BLUEPORTAL::ReturnOrangePortalPos() {
    VECTOR3 emptyvector = _V(0, 0, 0);

    OBJHANDLE orange_portal = oapiGetVesselByName("ORANGEPORTAL");
    if (!orange_portal) {
        oapiWriteLog("ORANGEPORTAL not found!");
        return emptyvector;
    }

    // Obtener la interfaz del portal naranja
    VESSEL *orange_portal_vessel = oapiGetVesselInterface(orange_portal);
    if (!orange_portal_vessel) {
        oapiWriteLog("Failed to get interface for ORANGEPORTAL!");
        return emptyvector;
    }

    // Obtener la referencia gravitacional (cuerpo central)
    OBJHANDLE ref_body = orange_portal_vessel->GetGravityRef();
    if (!ref_body) {
        oapiWriteLog("Reference body not found!");
        return emptyvector;
    }

    // Obtener la posición global del portal naranja y su cuerpo central
    VECTOR3 orange_portal_global, ref_body_global;
    oapiGetGlobalPos(orange_portal, &orange_portal_global);
    oapiGetGlobalPos(ref_body, &ref_body_global);

    // Calcular la posición relativa al cuerpo central
    VECTOR3 orange_portal_relative = orange_portal_global - ref_body_global;

    oapiWriteLogV("Orange Portal relative position: (%f, %f, %f)", 
                  orange_portal_relative.x, orange_portal_relative.y, orange_portal_relative.z);

    return orange_portal_relative;
}

OBJHANDLE BLUEPORTAL::ReturnNearestVessel() {
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

void BLUEPORTAL::TeleportVesselToOrangePortal() {
    OBJHANDLE teleport_target = ReturnNearestVessel();
    VECTOR3 orange_portal_pos = ReturnOrangePortalPos();

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

        // Modificar la posición relativa al cuerpo central
        vs2.rpos = orange_portal_pos;

        // Ajustar velocidad relativa (puedes personalizar este vector)
        vs2.rvel = _V(0, 0, 0);

        // Aplicar el nuevo estado
        target_vessel->DefSetStateEx(&vs2);

        oapiWriteLog("Vessel successfully teleported to Orange Portal!");
    }
}


void BLUEPORTAL::clbkLoadStateEx(FILEHANDLE scn, void *vs){

    char *line;

	while(oapiReadScenario_nextline(scn,line)){

		ParseScenarioLineEx(line,vs);

	}

}

void BLUEPORTAL::clbkPostCreation(){

}

void BLUEPORTAL::clbkPreStep(double simt, double simdt, double mjd){

    ReturnNearestVessel();
    TeleportVesselToOrangePortal();

}

void BLUEPORTAL::clbkPostStep(double simt, double simdt, double mjd){

    return;

}

void BLUEPORTAL::clbkSaveState(FILEHANDLE scn){

    SaveDefaultState(scn);

}

DLLCLBK void InitModule(MODULEHANDLE hModule){

}

DLLCLBK void ExitModule(MODULEHANDLE *hModule){

}

///////////////Vessel initialization

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel){
    
	return new BLUEPORTAL(hvessel, flightmodel);

}

/////////////Vessel memory cleanup
DLLCLBK void ovcExit(VESSEL *vessel){
    
	if(vessel) delete(BLUEPORTAL*)vessel;
	
}