#include <pb_lib.hpp>

struct Config : _Config {};

extern Config cfg;

long swapbits(long);

bool simul_files_ra(bool);
bool simul_messages_ra();
bool simul_config_ra();

bool simul_files_ra2(bool);
bool simul_messages_ra2();
bool simul_config_ra2();

int convert_ra_msgs();
int convert_ra_files();
int convert_ra_menus();

int convert_qbbs_msgs();
int convert_qbbs_files();
int convert_qbbs_menus();

int convert_sbbs_msgs();
int convert_sbbs_files();
int convert_sbbs_users();

int convert_maximus();

bool convert_userfile();
bool convert_menus();

bool convert_msgareas();
bool convert_modem();
bool convert_pvtfiles();
bool convert_config();
bool convert_languages();

extern char ra_path[];
extern char qbbs_path[];
extern char sbbs_path[];
extern char maximus_path[];

extern char syspath[];


bool read_config();
