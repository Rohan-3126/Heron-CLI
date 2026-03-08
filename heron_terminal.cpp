#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <fstream>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_CMD "cls"
#else
    #define CLEAR_CMD "clear"
#endif

using namespace std;

/* ===============================
   CONFIG
================================ */

const string LOG_FILE = "heron.log";

const string VERSION = "v1.3.0";
const string BUILD   = "2026.01";

const double Xcoord = 31.7683;
const double Ycoord = 35.2137;

const string status_state   = "ACTIVE";
const string telemetry_mode = "REAL + DECOY READY";

/* ===============================
   GLOBALS
================================ */

chrono::steady_clock::time_point start_time;

/* ===============================
   COLORS (semantic)
================================ */

const string RESET  = "\033[0m";
const string GREEN  = "\033[32m";  // healthy
const string RED    = "\033[31m";  // errors
const string CYAN   = "\033[36m";  // info/metrics
const string YELLOW = "\033[33m";  // configuration/mode

/* ===============================
   UTILS
================================ */

void clear_screen() {
    system(CLEAR_CMD);
}

string current_time_string() {
    time_t now = time(nullptr);
    string t = ctime(&now);
    t.pop_back();
    return t;
}

string uptime_string() {
    auto now = chrono::steady_clock::now();
    long seconds =
        chrono::duration_cast<chrono::seconds>(now - start_time).count();

    int hrs  = seconds / 3600;
    int mins = (seconds % 3600) / 60;
    int secs = seconds % 60;

    return to_string(hrs) + "h "
         + to_string(mins) + "m "
         + to_string(secs) + "s";
}

/* ===============================
   LOGGING
================================ */

void log_event(const string& msg) {
    ofstream file(LOG_FILE, ios::app);
    file << "[" << current_time_string() << "] " << msg << "\n";
}

void clear_log() {
    ofstream file(LOG_FILE, ios::trunc);
}

/* ===============================
   BOOT SEQUENCE
================================ */

void boot_sequence() {

#ifdef _WIN32
    system(""); // enable ANSI
#endif

    clear_screen();

    cout << CYAN << "Initializing HERON Control Module...\n" << RESET;
    this_thread::sleep_for(chrono::milliseconds(2000));

    cout << "Loading telemetry mods............ OK\n";
    this_thread::sleep_for(chrono::milliseconds(1500));
    cout << "Starting logging service.......... OK\n";
    this_thread::sleep_for(chrono::milliseconds(1500));
    cout << "Syncing system clock.............. OK\n";
    this_thread::sleep_for(chrono::milliseconds(1500));
    
    cout << GREEN << "\nSystem Ready.\n" << RESET;
    this_thread::sleep_for(chrono::milliseconds(2000));

    log_event("System boot completed");

    this_thread::sleep_for(chrono::milliseconds(400));
}

/* ===============================
   DISPLAY
================================ */

void render_dashboard() {

    clear_screen();

    cout << CYAN;
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║              HERON TERMINAL MODULE           ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << RESET;

    cout << " Version        : " << VERSION
         << " | Build " << BUILD << "\n";

    cout << CYAN   << " System Time    : " << current_time_string() << RESET << "\n";

    cout << GREEN  << " Status         : " << status_state << RESET << "\n";

    cout << YELLOW << " Mode           : " << telemetry_mode << RESET << "\n";

    cout << CYAN   << " Coordinates    : " << "(" << Xcoord << "°N , "<< Ycoord << "°E" << ")"<< RESET << "\n";

    cout << GREEN  << " Heartbeat      : OK" << RESET << "\n";

    cout << "\n--------------- commands ----------------\n";
    cout << " help     | status | coords | mode | uptime\n";
    cout << " modelgui | clslog | clear  | last | exit \n";
    cout << "-----------------------------------------\n";
}

/* ===============================
   MAIN
================================ */

int main() {

#ifdef _WIN32
    system("chcp 65001 > nul");
#endif


    start_time = chrono::steady_clock::now();

    boot_sequence();

    string cmd;
    string last_cmd = "-";

    log_event("HERON terminal started");

    while (true) {

        render_dashboard();

        cout << "\n> ";
        getline(cin, cmd);

        if (cmd.empty())
            continue;

        log_event("CMD: " + cmd);

        if (cmd == "help") {
    
        cout << CYAN;
        cout << "\n========== HERON COMMAND INTERFACE ==========\n";
        cout << RESET;
    
        cout << GREEN << " SYSTEM\n" << RESET;
        cout << "  status    : show module health\n";
        cout << "  uptime    : show system running time\n";
        cout << "  mode      : show telemetry mode\n";
        cout << "  coords    : show current coordinates\n\n";
    
        cout << YELLOW << " CONTROL\n" << RESET;
        cout << "  modelgui  : launch telemetry dashboard GUI\n";
        cout << "  clear     : clear terminal screen\n";
        cout << "  clslog    : clear system log file\n\n";
    
        cout << RED << " POWER\n" << RESET;
        cout << "  exit      : shutdown HERON module\n\n";
    
        cout << CYAN;
        cout << "============================================\n";
        cout << RESET;
        }


        else if (cmd == "status") {
            cout << GREEN << "\nSystem ACTIVE\n" << RESET;
        }

        else if (cmd == "coords") {
            cout << CYAN << "\nCoordinates → X=" << Xcoord
                 << "  Y=" << Ycoord << RESET << "\n";
        }

        else if (cmd == "mode") {
            cout << YELLOW << "\nTelemetry Mode → "
                 << telemetry_mode << RESET << "\n";
        }

        else if (cmd == "uptime") {
            cout << CYAN << "\nUptime → "
                 << uptime_string() << RESET << "\n";
        }

        else if (cmd == "last") {
            cout << CYAN << "\nLast command → "
                 << last_cmd << RESET << "\n";
        }

        else if (cmd == "modelgui") {
            cout << "\nLaunching GUI...\n";

#ifdef _WIN32
            system("python main.py");
#else
            system("python3 main.py");
#endif
        }

        else if (cmd == "clslog") {
            clear_log();
            cout << RED << "\nLog cleared.\n" << RESET;
        }

        else if (cmd == "clear") {
            clear_screen();
        }

	else if (cmd == "exit") {
    	log_event("System shutdown");
	
    	clear_screen();
	
   	 cout << RED;
   	 cout << "=====================================\n";
    	 cout << "   HERON TERMINAL OFFLINE\n";
    	 cout << "   Session safely terminated.\n";
    	 cout << "=====================================\n";
    	 cout << RESET;
	 
	 this_thread::sleep_for(chrono::milliseconds(3000));
	 clear_screen();
 	
   	 break;
	}


        else {
            cout << RED << "\nUnknown command.\n" << RESET;
        }

        last_cmd = cmd;

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}
