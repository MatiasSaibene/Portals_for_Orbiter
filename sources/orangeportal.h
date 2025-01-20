#pragma once

#include <cstddef>
#include <optional>
#ifndef __ORANGE_PORTAL_H
#define __ORANGE_PORTAL_H
#define STRICT 1

#include "HEADERS//Orbitersdk.h"
#include "HEADERS//OrbiterAPI.h"
#include "HEADERS//VesselAPI.h"

const VECTOR3 FORWARD_DIRECTION = {0, 0, 0};

const VECTOR3 orange_DCK1_Location = {4.0000, 0.0000, 0.0000};
const VECTOR3 orange_DCK2_Location = {0.0000, -4.0000, 0.0000};

const VECTOR3 OrangePortal_DOCK_ROT = {0, 0, -1};

static const int ntdvtx = 4;
static TOUCHDOWNVTX tdvtx[ntdvtx]{
    {_V(0.9953, 0.0023, -1.0000), 1635, 571, 0.0, 0.0},
    {_V(-1.0047, 0.0023, -1.0000), 1635, 571, 0.0, 0.0},
    {_V(-1.0047, 0.0023, 1.0000), 1635, 571, 0.0, 0.0},
    {_V(0.9953, 0.0023, 1.0000), 1635, 571, 0.0, 0.0}
};



class ORANGEPORTAL : public VESSEL4{

    public:

        ORANGEPORTAL(OBJHANDLE hVessel, int flightmodel);

        virtual ~ORANGEPORTAL();

        //void NotifyError(const char *error, const char *className, const char *type);

        void clbkSetClassCaps(FILEHANDLE cfg) override;
        void clbkLoadStateEx(FILEHANDLE scn, void *vs) override;
        void clbkPostCreation() override;
        void clbkPreStep(double simt, double simdt, double mjd) override;
        void clbkPostStep(double simt, double simdt, double mjd) override;
        void clbkSaveState(FILEHANDLE scn) override;

        VECTOR3 ReturnBluePortalPos();
        OBJHANDLE ReturnNearestVessel();
        void TeleportVesselToBluePortal();

        


    private:

        MESHHANDLE mhPortal;
        unsigned int uimesh_Portal;

        VESSELSTATUS2 vs2;

        DOCKHANDLE orange_dock1, orange_dock2;


};

#endif //!__ORANGE_PORTAL_H