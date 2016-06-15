#include "tw_portal.h"

TWPortal(){

}

~TWPortal(){
}

double x(){

}

double y(){

}

double height(){ return m_height; }
double width() { return m_width; }
shared_ptr<Texture> texture() { retur m_texture; }

void update_self(unsigned now, unsigned last){
}

void draw_self(Canvas *canvas, unsigned now, unsigned last){
}

void set_x(double cx) { m_x = cx; }
void set_y(double cy) { m_y = cy; }
void set_height(double ch) { m_height = ch; }
void register_self(int current_x){
	m_x = current_x;
	m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
	physics::register_object(this);
}

bool active() const { return true; }
pair<double, double> direction() const {
    pair<double, double> p(0, 0);
	return p;
}

const Rectangle& bounding_box() const {
	return m_bounding_box;
}

const list<Rectangle>& hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last){
}

