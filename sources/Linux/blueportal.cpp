#include "blueportal.h"
#include <cstddef>
#include <cstring>

//Constructor
BLUEPORTAL::BLUEPORTAL(OBJHANDLE hVessel, int flightmodel):VESSEL4(hVessel, flightmodel){

    mhPortal = NULL;

    uimesh_Portal = 0;

    vs2 = {0};

    l1 = nullptr;

    l2 = nullptr;

    blue_dock1 = nullptr;

    blue_dock2 = nullptr;

    prp = nullptr;

}

//Destructor
BLUEPORTAL::~BLUEPORTAL(){

    if(mhPortal != nullptr){
        
        mhPortal = nullptr;

    }

    uimesh_Portal = 0;

    vs2 = {0};

    if(blue_dock1 != nullptr){

        blue_dock1 = nullptr;

    }

    if(blue_dock2 != nullptr){

        blue_dock2 = nullptr;

    }

}

void BLUEPORTAL::clbkSetClassCaps(FILEHANDLE cfg){

    prp = CreatePropellantResource(PORTAL_FUEL_MASS);

    const char *meshname = "blue_portal";

    mhPortal = oapiLoadMeshGlobal(meshname);
    uimesh_Portal = AddMesh(mhPortal);

    SetSize(4);
    SetEmptyMass(50);
    SetTouchdownPoints(tdvtx, ntdvtx);

    blue_dock1 = CreateDock((blue_DCK1_Location), FORWARD_DIRECTION, BluePortal_DOCK_ROT);

    blue_dock2 = CreateDock((blue_DCK2_Location), FORWARD_DIRECTION, BluePortal_DOCK_ROT);

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

VECTOR3 BLUEPORTAL::ReturnOrangePortalPos() {
    VECTOR3 emptyvector = _V(0, 0, 0);

    OBJHANDLE orange_portal = oapiGetVesselByName("ORANGEPORTAL");
    if (!orange_portal) {
        oapiWriteLog("ORANGEPORTAL not found!");
        return emptyvector;
    }

    //Get Orange Portal vessel interface
    VESSEL *orange_portal_vessel = oapiGetVesselInterface(orange_portal);
    if (!orange_portal_vessel) {
        oapiWriteLog("Failed to get interface for ORANGEPORTAL!");
        return emptyvector;
    }

    //Get gravitational reference body
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

    /* oapiWriteLogV("Orange Portal relative position: (%f, %f, %f)", 
                  orange_portal_relative.x, orange_portal_relative.y, orange_portal_relative.z); */

    //Apply some shift
    orange_portal_relative.z += 10;

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
        //vs2.rvel = _V(0, 0, 0);

        // Aplicar el nuevo estado
        target_vessel->DefSetStateEx(&vs2);
        target_vessel->AddForce(_V(0, 0, 1), _V(0, 0, 0.01));

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

    return;

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