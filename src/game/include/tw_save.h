#ifndef TW_SAVE_H
#define TW_SAVE_H

#include <iostream>

using namespace std;

class TWSave{
    public:
        TWSave(int n_levels);
		~TWSave();

		bool is_unlocked(int level);
		bool is_cleared(int level);

		void set_unlocked(int level);
		void set_cleared(int level);
		void set_record(int level, int n_cols, int n_enemies);
		void update();
		void increase_times_played(int level);

		int max_collectables(int level);
		int max_enemies(int level);
		int record_collectables(int level);
		int record_enemies(int level);
		int times_played(int level);

	private:
		typedef struct _ls{
			bool unlocked = false;
			bool cleared = false;
			int max_collectables = 0;
			int max_enemies = 0;
			int record_collectables = 0;
			int record_enemies = 0;
			int times_played = 0;
		}Level_stat;

		int n_levels;

		Level_stat *m_level_info;

		FILE *save;
};

#endif
