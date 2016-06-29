#ifndef TW_RESULT_H
#define TW_RESULT_H

#include "tw_level.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWResult : public TWLevel{
	public:
		TWResult(const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
		~TWResult();

		bool done() const;
		string next() const;
		string audio() const;

		bool on_event(const GameEvent& event);

	protected:
		void update_self(unsigned, unsigned last);
		void draw_self(Canvas *canvas, unsigned, unsigned last);
};

#endif
