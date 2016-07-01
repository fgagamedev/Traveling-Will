#include "tw_result.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWResult::TWResult(const string &current_level, const string& next_level, const string audio_path, int){
	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = NOTHING;
	m_start = -1;

	FILE *result = fopen("result.dat", "rb");
	int v[1];

	if(not result){
		printf("Não foi possível abrir o arquivo result.dat\n");
		exit(1);
	}

	fread(&v[0], sizeof(int), 1, result);

	double max_col = 500;
	double percentage = (100.0 * v[0])/max_col;

	if(percentage >= MINIMUM_PERCENTAGE){
		final_texture = resources::get_texture(m_current_level + "/ganhou.png");
	}else{
		final_texture = resources::get_texture(m_current_level + "/perdeu.png");
	}

	event::register_listener(this);
}

TWResult::~TWResult(){
	event::unregister_listener(this);
}

bool TWResult::done() const{
	return m_done;
}

string TWResult::next() const{
	return m_next;
}

string TWResult::audio() const{
	return m_audio;
}

bool TWResult::on_event(const GameEvent& event){
	return false;
}

void TWResult::update_self(unsigned, unsigned){
	if(m_state == SELECTING){
		m_state = RUNNING;
		m_done = true;
	}
}

void TWResult::draw_self(Canvas *canvas, unsigned, unsigned){
	canvas->clear();
	
	canvas->draw(final_texture.get(), Rectangle(0, 0, 852, 480), 0, 0);
}
