// This sketch implements a menu system over the Serial port
// Pretends to be a battery charger that can charge/discharge/etc
// multiple types of batteries

struct MenuOpt;
struct Menu;
struct FuncOpt;
void showMenu();

Menu* head;

struct MenuOpt {
    MenuOpt(const char* nm, const char* desc)
        : name(nm)
        , description(desc)
        {}
    MenuOpt(){}
    const char* name;
    const char* description;
    virtual void select() = 0;
};

struct Menu : public MenuOpt {
    Menu(const char* nm, const char* desc,
         size_t N, MenuOpt** opts, Menu* prev)
        : MenuOpt(nm, desc)
        , numOptions(N)
        , options(opts)
        , prevMenu(prev)
        {}
    Menu(){}
    size_t numOptions;
    MenuOpt** options; // Array of options
    virtual void select() {
        head = this;
    }
    Menu* prevMenu;
};

struct FuncOpt : public MenuOpt {
    FuncOpt(const char* nm, const char* desc,
            void (* function)(void))
        : MenuOpt(nm, desc)
        , func(function)
        {}
    FuncOpt(){}
    virtual void select() {
        this->func();
    }
    void (* func)(void);
};

char getSelectChar(size_t i) {
    if (i<=10) return i+'0';
    if (i<=36) return i+'a';
    if (i<=62) return i+'A';
    else return '@';
}

void showMenu() {
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println(F("For help, type ?\r\n"
                     "To select an option, press the character in parenthesus"));
    for (size_t i=0; i < head->numOptions; i++) {
        Serial.write('(');
        Serial.write(getSelectChar(i));
        Serial.write(')');
        Serial.write(' ');
        Serial.print(head->options[i]->name);
        Serial.write('\t');
    }
    Serial.println();
}

void DischargeLipo() {
    Serial.println(F("Discharging LiPo"));
}
FuncOpt dischargeLipo ("Discharge", "Discharges a LiPo battery"
                       " so it can be stored", DischargeLipo);
void ChargeLipo() {
    Serial.println(F("Charging LiPo"));
}
FuncOpt chargeLipo ("Charge", "Charges a LiPo battery to run crap",
                    ChargeLipo);
void ExplodeLipo() {
    Serial.println(F("Exploding LiPo"));
}
FuncOpt explodeLipo ("Blow Up", "Blows up a Lithium Ion Polymer battery",
                     ExplodeLipo);
MenuOpt* lipoopts[] = {&chargeLipo, &dischargeLipo, &explodeLipo};
extern Menu chargeBat;
Menu lipo ("LiPo", "Manage a Lithium-Ion Polymer battery",
           3, lipoopts, &chargeBat);


void DischargeLead() {
    Serial.println(F("Discharging Lead Acid Battery"));
}
FuncOpt dischargeLead ("Discharge", "Discharges a Lead Acid battery"
                       " so it can be stored", DischargeLead);
void ChargeLead() {
    Serial.println(F("Charging Lead Acid Battery"));
}
FuncOpt chargeLead ("Charge", "Charges a LiPo battery to run",
                    ChargeLead);
MenuOpt* leadopts[] = {&chargeLead, &dischargeLead};
Menu lead ("Lead", "Manage a Lead Acid battery",
           2, leadopts, &chargeBat);


MenuOpt* chargeBatOpts[] = {&lipo, &lead};
extern Menu mainmenu;
Menu chargeBat("Charge", "Charge or Discharge a battery",
               2, chargeBatOpts, &mainmenu);
MenuOpt* mainMenuOpts[] = {&chargeBat};
Menu mainmenu("Main", "Use your charger", 1, mainMenuOpts, NULL);


int main() {
    init(); // Initialize the timers and stuff for arduino

    head = &mainmenu;
    
    Serial.begin(115200);
    showMenu();

    while(1) {
        if (Serial.available()) {
            size_t opt = Serial.read() - '0'; // TODO: Make this work for a-z
            head->options[opt]->select();
            showMenu();
        }
    }
}
//void setup() {}
//void loop() {}
