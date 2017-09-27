#ifndef TW_HUD_H
#define TW_HUD_H

#include <map>
#include <vector>
#include <iostream>
#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>
#include <ijengine/game_object.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;
using std::map;

class TWHud : public GameObject {
    public:
        ~TWHud(){}

    protected:
        void update_self(unsigned now, unsigned last) = 0;
        void draw_self(Canvas *canvas, unsigned now, unsigned last) = 0;
        string m_current_level;
        vector< shared_ptr<Texture> > m_textures;
};


#endif
