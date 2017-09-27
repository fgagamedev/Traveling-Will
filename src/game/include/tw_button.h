#ifndef TW_BUTTON_H
#define TW_BUTTON_H

#include <memory>
#include <iostream>
#include <ijengine/engine.h>
#include <ijengine/rectangle.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;

class TWButton : public GameObject, public GameEventsListener {
    public:
        TWButton(string label, string cur_level, double b_x, double b_y, string img, double b_w, double b_h);
        TWButton() {}
        ~TWButton();
        double w();
        double h();
        double x();
        double y();
        string level();
        string label();
        string texture();
        double percentage();
        double width();
        void set_texture(string btn_texture);
        void set_active(bool act);
        void set_x(int bx);

        bool on_event(const GameEvent& event);

        typedef enum { CLICKING, NOT_CLICKING } ClickState;
        typedef enum { HOVERING, NOT_HOVERING } HoverState;

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        static const int GAME_MOUSE_CLICK = 1 << 8;
        static const int GAME_MOUSE_MOTION = 1 << 10;
        ClickState m_click_state;
        HoverState m_hover_state;
        string m_label, m_texture_label;
        string m_img, m_action, m_level;
        double m_x, m_y, m_h, m_w, m_percentage;
        bool m_active;
        shared_ptr<Texture> m_texture;
};

#endif
