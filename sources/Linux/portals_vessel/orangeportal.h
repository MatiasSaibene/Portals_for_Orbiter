#pragma once

#include <cstddef>
#include <optional>
#ifndef __ORANGE_PORTAL_H
#define __ORANGE_PORTAL_H
#define STRICT 1

#include "HEADERS//Orbitersdk.h"
#include "HEADERS//OrbiterAPI.h"
#include "HEADERS//VesselAPI.h"

const double PORTAL_FUEL_MASS = 5;

const double PORTAL_TH = 500;

const double PORTAL_ISP = 5000;

const VECTOR3 FORWARD_DIRECTION = {0, 0, 0};

const VECTOR3 RCS0_Location = {-2.6182, -0.0015, 2.5853};

const VECTOR3 RCS1_Location = {2.5818, -0.0015, 2.5853};

const VECTOR3 RCS2_Location = {2.5818, -0.0015, -2.6147};

const VECTOR3 RCS3_Location = {-2.6182, -0.0015, -2.6147};

const VECTOR3 RCS4_Location = {-0.0182, -0.0015, -2.6147};

const VECTOR3 RCS5_Location = {-0.0182, -0.0015, 2.5853};

const VECTOR3 orange_DCK1_Location = {4.0000, 0.0000, 0.0000};
const VECTOR3 orange_DCK2_Location = {0.0000, -4.0000, 0.0000};

const VECTOR3 Spot1_Location = {0.1805, 2.5983, -0.2633};

const VECTOR3 Spot2_Location = {0.1805, 2.5983, 0.2567};

const VECTOR3 OrangePortal_DOCK_ROT = {1, 0, 0};

static const int ntdvtx = 4;
static TOUCHDOWNVTX tdvtx[ntdvtx]{
    {_V(2.5885, -2.5955, 2.5850), 1635, 571, 3.0, 3.0},
    {_V(2.5885, -2.5955, -2.6150), 1635, 571, 3.0, 3.0},
    {_V(-2.6115, -2.5955, -2.6150), 1635, 571, 3.0, 3.0},
    {_V(-2.6115, -2.5955, 2.5850), 1635, 571, 3.0, 3.0}
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

        LightEmitter *l1, *l2;

        COLOUR4 col_d = {255, 102, 0};
        COLOUR4 col_s = {255, 102, 0};
        COLOUR4 col_a = {255, 102, 0};

        THRUSTER_HANDLE th_rcs[14], th_group[4];

        PROPELLANT_HANDLE prp;

        DOCKHANDLE orange_dock1, orange_dock2;


};

#endif //!__ORANGE_PORTAL_H