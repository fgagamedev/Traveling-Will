#include "tw_portal.h"

TWPortal::TWPortal(){
	m_texture = resources::get_texture("1/portal.jpg");
}

TWPortal::~TWPortal(){
    physics::unregister_object(this);
}

double TWPortal::x(){
	return m_x;
}

double TWPortal::y(){
	return m_y;
}

double TWPortal::height(){ 
	return m_height; 
}

double TWPortal::width(){ 
	return m_width; 
}

shared_ptr <Texture> TWPortal::texture(){ 
	return m_texture; 
}

void TWPortal::update_self(unsigned now, unsigned last){
}

void TWPortal::draw_self(Canvas *canvas, unsigned now, unsigned last){
    canvas->draw(m_texture.get(), Rectangle(0, 0, 80, 340), 750, 90);
}

void TWPortal::set_x(double cx){ 
	m_x = cx; 
}

void TWPortal::set_y(double cy){ 
	m_y = cy; 
}

void TWPortal::set_height(double ch){ 
	m_height = ch; 
}

void TWPortal::register_self(int current_x){
	m_x = current_x;
	m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
	physics::register_object(this);
}

bool TWPortal::active() const{ 
	return true; 
}

pair <double, double> TWPortal::direction() const {
    pair<double, double> p(0, 0);
	return p;
}

const Rectangle &TWPortal::bounding_box() const {
	return m_bounding_box;
}

const list<Rectangle> &TWPortal::hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void TWPortal::on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last){
    printf("TWWill colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
}

