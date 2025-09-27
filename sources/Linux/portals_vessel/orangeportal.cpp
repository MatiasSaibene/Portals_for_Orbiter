#include "orangeportal.h"
#include <cstddef>
#include <cstring>

//Constructor
ORANGEPORTAL::ORANGEPORTAL(OBJHANDLE hVessel, int flightmodel):VESSEL4(hVessel, flightmodel){

    mhPortal = NULL;

    uimesh_Portal = 0;

    vs2 = {0};

    l1 = nullptr;

    l2 = nullptr;

    orange_dock1 = NULL;

    orange_dock2 = NULL;

    prp = nullptr;

}

//Destructor
ORANGEPORTAL::~ORANGEPORTAL(){

    if(mhPortal != nullptr){
        
        mhPortal = nullptr;

    }

    uimesh_Portal = 0;

    vs2 = {0};

    if(orange_dock1 != nullptr){

        orange_dock1 = nullptr;

    }

    if(orange_dock2 != nullptr){

        orange_dock2 = nullptr;

    }
    
}

void ORANGEPORTAL::clbkSetClassCaps(FILEHANDLE cfg){

    prp = CreatePropellantResource(PORTAL_FUEL_MASS);

    const char *meshname = "orange_portal";

    mhPortal = oapiLoadMeshGlobal(meshname);
    uimesh_Portal = AddMesh(mhPortal);

    SetSize(4);
    SetEmptyMass(50);
    SetTouchdownPoints(tdvtx, ntdvtx);

    orange_dock1 = CreateDock((orange_DCK1_Location), FORWARD_DIRECTION, OrangePortal_DOCK_ROT);

    orange_dock2 = CreateDock((orange_DCK2_Location), FORWARD_DIRECTION, OrangePortal_DOCK_ROT);

    l1 = AddSpotLight(Spot1_Location, _V(0.0000, 0.0000, -1.3000), 5, 1e-1, 0, 2e-1, 25*RAD, 45*RAD, col_d, col_s, col_a);

    l2 = AddSpotLight(Spot2_Location, _V(0.0000, 0.0000, 1.4000), 5, 1e-1, 0, 2e-1, 25*RAD, 45*RAD, col_d, col_s, col_a);

    th_rcs[0] = CreateThruster(RCS1_Location, _V(0, 1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[1] = CreateThruster(RCS1_Location, _V(0, -1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[2] = CreateThruster(RCS0_Location, _V(0, 1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[3] = CreateThruster(RCS0_Location, _V(0, -1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[4] = CreateThruster(RCS2_Location, _V(0, 1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[5] = CreateThruster(RCS2_Location, _V(0, -1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[6] = CreateThruster(RCS3_Location, _V(0, 1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[7] = CreateThruster(RCS3_Location, _V(0, -1, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[8] = CreateThruster(RCS1_Location, _V(-1, 0, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[9] = CreateThruster(RCS0_Location, _V(1, 0, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[10] = CreateThruster(RCS2_Location, _V(-1, 0, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[11] = CreateThruster(RCS3_Location, _V(1, 0, 0), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[12] = CreateThruster(RCS4_Location, _V(0, 0, 1), PORTAL_TH, prp, PORTAL_ISP);
    th_rcs[13] = CreateThruster(RCS5_Location, _V(0, 0, -1), PORTAL_TH, prp, PORTAL_ISP);

    th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[2];
	th_group[2] = th_rcs[5];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHUP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[3];
	th_group[2] = th_rcs[4];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHDOWN);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKLEFT);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKRIGHT);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_UP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_DOWN);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[11];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWLEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[10];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWRIGHT);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[10];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_LEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[11];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_RIGHT);

	CreateThrusterGroup (th_rcs+12, 1, THGROUP_ATT_FORWARD);
	CreateThrusterGroup (th_rcs+13, 1, THGROUP_ATT_BACK);

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

    /* oapiWriteLogV("Blue Portal relative position: (%f, %f, %f)", 
                  blue_portal_relative.x, blue_portal_relative.y, blue_portal_relative.z); */

    //Apply some shift
    blue_portal_relative.z += 10;

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
        //vs2.rvel = _V(0, 0, 0); // Puede ajustarse según el contexto.

        // Aplicar el nuevo estado a la nave objetivo
        target_vessel->DefSetStateEx(&vs2);
        target_vessel->AddForce(_V(0, 0, 1), _V(0, 0, 0.01));

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