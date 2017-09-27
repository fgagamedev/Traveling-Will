#include "tw_will.h"
#include "tw_playable_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWPlayableLevel::TWPlayableLevel(const string &current_level, const string& next_level, const string audio_path, 
int audio_duration) : 
	m_is_punching(false), level_started(false), level_finished(false),
    m_audio_duration(audio_duration), m_audio_counter(0),
    m_punch_counter(0), n_collectables(0), n_enemies(0), m_y_speed(0),
    m_camera_x(0), m_camera_y(0), m_reverse_camera_x(0), m_reverse_camera_y(480){

    // printf("Entrando em construtor\n");

    if(current_level == "3"){
        audio::set_audio_volume(0.7);
    }

	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = RUNNING;
	m_audio_start = 0;
    counting = 0;
	m_start = -1;

    m_collectable_status = new TWCollectableStatus(m_current_level);
    add_child(m_collectable_status);

    m_floor_texture = resources::get_texture(m_current_level + "/floor.png");

	//Read level design from txt
	fstream level_design("res/" + m_current_level + "/level_design.txt");

	if(not level_design.is_open()){
		printf("Level design txt not available\n");
		exit(0);
	}

	int tempo;

	level_design >> tempo;

	m_x_speed = tempo * 4 * 36 / 60000.0;
    //m_x_speed = 0.284; //FIXME
    printf("tempo: %d, m_x_speed: %f\n", tempo, m_x_speed);

    level_design >> n_screens >> n_backgrounds;

    for(int i = 0; i < n_screens; ++i){
        int ph, e_present, c_present;
        int et = INVALID, eh = INVALID, ch = INVALID;

        level_design >> ph >> e_present;

        if(e_present){
            level_design >> et >> eh;
        }

        level_design >> c_present;

        if(c_present){
            level_design >> ch;
        }

        auto p = new TWPlatform(m_current_level, ph, et, eh, e_present, ch, c_present, 0);
        platforms.push_back(p);

    }

    // sets correct sprite of platform
    int tam = platforms.size();
    for(int i = 0; i < (tam-1); i++){
        if(i == 0) continue;

        auto cur_plat = platforms[i];
        auto prev_plat = platforms[i-1];
        auto next_plat = platforms[i+1];

        auto cur_h = cur_plat->height();
        auto prev_h = prev_plat->height();
        auto next_h = next_plat->height();

        if(prev_h != cur_h && cur_h != next_h) cur_plat->set_texture(1);
        if(prev_h != cur_h && cur_h == next_h) cur_plat->set_texture(2);
        if(prev_h == cur_h && cur_h != next_h) cur_plat->set_texture(3);
    }

    m_progress_bar = new TWProgressBar(m_current_level, n_screens*1.0);
    add_child(m_progress_bar);

    m_portal_start = new TWPortal(53, 355, m_x_speed, START);
    add_child(m_portal_start);

    m_portal_end[0] = new TWPortal((n_screens-2) * 36, 430 - PORTAL_HEIGHT, m_x_speed, END);
    add_child(m_portal_end[0]);

    m_portal_end[1] = new TWPortal((n_screens-2) * 36, 480 - platforms[n_screens-3]->height() - PORTAL_HEIGHT, m_x_speed, END);
    add_child(m_portal_end[1]);

    for(int i = 0; i < n_backgrounds; ++i){
        m_background[i] = resources::get_texture(m_current_level + "/background_" + to_string(i) + ".png");
    }

    for(int i = 0; i < NUMBER_OF_SECTIONS; ++i){
        auto p = platforms[i];
        p->set_x(i * PLATFORM_SIZE);
        add_child(p);
        p->register_objects(i * PLATFORM_SIZE);
    }

    level_design.close();
    
    //Sets initial will height based on level design
    m_floor = 480 - platforms[0]->height() - WILL_HEIGHT;

    m_will = new TWWill(63, 430 - WILL_HEIGHT);
    add_child(m_will);

    m_start = -1;

    int n_levels = 6;
    m_save = new TWSave(n_levels);
    m_tutorial = nullptr;
    m_triple_jump = nullptr;

    if(m_save->times_played(1) == 0 && m_current_level == "1"){
        m_tutorial = new TWTutorial("jump", "tutorial/tutorial-jump.png");
        m_triple_jump = new TWTutorial("triple-jump", "tutorial/tutorial-triple-jump.png");
        add_child(m_triple_jump);
        add_child(m_tutorial);
    }
    else if(m_save->times_played(2) == 0 && m_current_level == "2"){
        m_tutorial = new TWTutorial("slide", "tutorial/tutorial-slide.png");
        add_child(m_tutorial);
    }
    else if(m_save->times_played(3) == 0 && m_current_level == "3"){
        m_tutorial = new TWTutorial("punch", "tutorial/tutorial-punch.png");
        add_child(m_tutorial);
    }

    event::register_listener(this);

    physics::set_collision_mode(physics::Mode::ONE_TO_ALL, m_will);
}

TWPlayableLevel::~TWPlayableLevel(){
    event::unregister_listener(this);
}

bool TWPlayableLevel::done() const{
    return m_done;
}

string TWPlayableLevel::next() const{
    return m_next;
}

string TWPlayableLevel::audio() const{
    return m_audio;
}

bool TWPlayableLevel::on_event(const GameEvent& event){
    if(event.id() == GAME_EVENT_DEBUG){
        printf("counting = %d\n", counting);
        return true;
    }

	return false;
}

void TWPlayableLevel::set_done(bool is_done, bool game_over){
	FILE *result = fopen("result.dat", "wb");

	if (not result){
		printf("Não foi possível abrir o arquivo result.dat\n");
		exit(1);
	}

	int v[5];

	v[0] = atoi(m_current_level.c_str());
	v[1] = m_will->collectables();
	v[2] = m_will->enemies();
    v[3] = game_over;
    v[4] = atoi(m_current_level.c_str());

	fwrite(&v[0], sizeof(int), 5, result);
	fclose(result);

	m_done = is_done;
}

void TWPlayableLevel::update_self(unsigned now, unsigned last){
    if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

    if(m_tutorial){
        if(m_will->state() == JUMPING && m_tutorial->label() == "jump"){
            m_tutorial->set_active(false);
        }
        else if(m_will->state() == SLIDING && m_tutorial->label() == "slide"){
            m_tutorial->set_active(false);
        }
        else if(m_will->punching() && m_tutorial->label() == "punch"){
            m_tutorial->set_active(false);
        }
    }

    if(m_triple_jump){
        if(m_will->triple_jump() && m_triple_jump->active()){
            m_triple_jump->set_active(false);
        }
    }

    update_counters(now);

    update_platforms_position();

    physics::do_collisions(now, last);

    test_floor(now);

    m_start = now;
}

void TWPlayableLevel::test_floor(unsigned now){
    //Start jump if TWWill is at the end of a cliff
    if(m_will->y() < m_floor && m_will->state() != JUMPING && m_will->state() != FALLING){
        m_will->set_state(FALLING);
        m_will->set_y_speed(0);
    }

    //Calculate jump speed and stop jump if hits the ground
    if(m_will->state() == JUMPING){
        m_will->update_y_speed((now - m_start)/300.0 * 0.5);

        if(m_will->speed() >= 0.001){
            m_will->set_state(FALLING);
        }
    }

    if(m_will->state() == FALLING){
        m_will->update_y_speed((now - m_start)/300.0 * 0.5);

        if(m_will->y() > m_floor){
            m_will->set_y(m_floor);
            m_will->set_y_speed(0);
            m_will->set_state(RUNNING);
            m_will->set_jump_counter(0);
        }
    }
}

void TWPlayableLevel::update_platforms_position(){
    int height, current_x;

    for(int i = 0; i < NUMBER_OF_SECTIONS; ++i){
        platforms[i]->set_x(m_reverse_camera_x + PLATFORM_SIZE*i);
    }


    for(int i = 0; i < NUMBER_OF_SECTIONS; ++i){
        current_x = platforms[i]->x();
        height = platforms[i]->height();

        if(current_x >= m_will->x() && current_x + 142 <= m_will->x() + WILL_WIDTH){
            m_floor = min(480.0 - height - WILL_HEIGHT, m_floor);
        }

        if(current_x >= m_will->x() && current_x <= m_will->x() + 30){
            m_floor = 480.0 - height - WILL_HEIGHT;
        }
    }

    if(m_will->y() >= m_floor + 5){
        m_floor = 430 - WILL_HEIGHT;
    }
}

void TWPlayableLevel::update_counters(unsigned now){
    if(not level_finished){
        m_audio_counter = now - m_audio_start;
    }

    m_camera_x += ((now - m_start) * m_x_speed);
    m_reverse_camera_x -= ((now - m_start) * m_x_speed);

    if(m_reverse_camera_x < -PLATFORM_SIZE){
        m_reverse_camera_x += PLATFORM_SIZE;
        destroy_child(platforms[0]);
        platforms.pop_front();
        ++counting;

		if(platforms.size() == NUMBER_OF_SECTIONS){
			m_x_speed = 0;
			m_portal_end[0]->set_x_speed(0);
            m_portal_end[1]->set_x_speed(0);
            m_will->set_x_speed(1/1.7);
		}

        platforms[NUMBER_OF_SECTIONS-1]->set_x(852);
        platforms[NUMBER_OF_SECTIONS-1]->register_objects(852);
        add_child(platforms[NUMBER_OF_SECTIONS-1]);
    }

    m_collectable_status->update_collectable_counter(m_will->collectables());
    m_progress_bar->update_current_length(counting + 25);
}

void TWPlayableLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();

    int divisor = 1 << (n_backgrounds - 1);
    for(int i = 0; i < n_backgrounds; ++i){
        canvas->draw(m_background[i].get(), Rectangle(fmod(m_camera_x/divisor, 1704), 0, 852, 480), 0, 0);
        divisor /= 2;
    }
}
