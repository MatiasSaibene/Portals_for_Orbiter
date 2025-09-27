#pragma once
#ifndef PORTAL_HPP_
#define PORTAL_HPP_

#include "HEADERS//Orbitersdk.h"
#include "HEADERS//MFDAPI.h"

static struct{
    int mode;
} g_PortalMFD;

class PortalMFD : public MFD2{

    public:
        PortalMFD(int w, int h, VESSEL *vessel);
        ~PortalMFD() override;

        bool ConsumeKeyBuffered(int key) override;
        bool ConsumeButton(int bt, int event) override;
        const char *ButtonLabel(int bt) override;
        int ButtonMenu(const MFDBUTTONMENU **menu) const override;
        bool Update(oapi::Sketchpad *skp) override;
        static OAPI_MSGTYPE MsgProc(MFD_msg, MfdId mfd, MFDMODEOPENSPEC *spec, VESSEL *vessel);

        bool SetRefBody(const std::string refbody);
        bool SetBase(const std::string base);
        bool DeletePortals();
        bool OpenPortals();

    private:

        VECTOR3 currentRelPos;

        double longitude, latitude, altitude;

        VECTOR3 selectedBasePos;

        OBJHANDLE currentShip;

        OBJHANDLE selectedBody;

        OBJHANDLE selectedBase;

        std::string planet, base;

        bool portalcreated;

};

#endif //!_PORTAL_HPP_