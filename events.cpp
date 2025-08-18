#include <bits/stdc++.h>
using namespace std;

// ---------------- Event Class ----------------
class Event {
public:
    int id;
    string name, date, time, type, location, recurrence;

    void print() const {
        cout << "[" << id << "] " << name << " on " << date << " " << time
             << " (" << type << ") at " << location
             << " [" << recurrence << "]\n";
    }
};

// ---------------- Event Manager Class ----------------
class EventManager {
    vector<Event> events;
    int nextId = 1;
    bool isAdmin = false;

    // Helpers
    bool isValidDate(const string &d) { return d.size()==10 && d[2]=='-' && d[5]=='-'; }
    bool isValidTime(const string &t) { return t.size()==5 && t[2]==':'; }

    bool hasConflict(const string &date, const string &time) {
        for (auto &e: events) if (e.date==date && e.time==time) return true;
        return false;
    }

    void saveEvents() {
        ofstream f("events.csv");
        for (auto &e: events)
            f<<e.id<<","<<e.name<<","<<e.date<<","<<e.time<<","
             <<e.type<<","<<e.location<<","<<e.recurrence<<"\n";
    }

    void loadEvents() {
        ifstream f("events.csv"); string line;
        while (getline(f,line)) {
            stringstream ss(line); Event e; string id;
            getline(ss,id,','); e.id=stoi(id);
            getline(ss,e.name,','); getline(ss,e.date,','); getline(ss,e.time,',');
            getline(ss,e.type,','); getline(ss,e.location,','); getline(ss,e.recurrence,',');
            events.push_back(e); nextId=max(nextId,e.id+1);
        }
    }

public:
    EventManager() { loadEvents(); }

    // ---------- Features ----------
    void adminLogin() {
        string pass; cout<<"Password: "; getline(cin,pass);
        if (pass=="admin123") { isAdmin=true; cout<<"[+] Admin logged in!\n"; }
        else cout<<"Wrong password!\n";
    }

    void addEvent() {
        if (!isAdmin) { cout<<"[!] Admin only.\n"; return; }
        Event e; e.id=nextId++;
        cout<<"Name: "; getline(cin,e.name);
        cout<<"Date (DD-MM-YYYY): "; getline(cin,e.date);
        if (!isValidDate(e.date)) { cout<<"Invalid date!\n"; return; }
        cout<<"Time (HH:MM): "; getline(cin,e.time);
        if (!isValidTime(e.time)) { cout<<"Invalid time!\n"; return; }
        if (hasConflict(e.date,e.time)) { cout<<"Conflict! Pick another slot.\n"; return; }
        cout<<"Type: "; getline(cin,e.type);
        cout<<"Location: "; getline(cin,e.location);
        cout<<"Recurrence (none/weekly/monthly): "; getline(cin,e.recurrence);

        events.push_back(e); saveEvents();
        cout<<"Event added!\n";
    }

    void editEvent() {
        if (!isAdmin) { cout<<"[!] Admin only.\n"; return; }
        int id; cout<<"Enter Event ID: "; cin>>id; cin.ignore();
        for (auto &e: events) if (e.id==id) {
            cout<<"Editing "<<e.name<<"\n";
            cout<<"New Name ("<<e.name<<"): "; getline(cin,e.name);
            cout<<"New Date ("<<e.date<<"): "; getline(cin,e.date);
            cout<<"New Time ("<<e.time<<"): "; getline(cin,e.time);
            cout<<"New Type ("<<e.type<<"): "; getline(cin,e.type);
            cout<<"New Location ("<<e.location<<"): "; getline(cin,e.location);
            cout<<"New Recurrence ("<<e.recurrence<<"): "; getline(cin,e.recurrence);
            saveEvents(); cout<<"Updated!\n"; return;
        }
        cout<<"Not found!\n";
    }

    void deleteEvent() {
        if (!isAdmin) { cout<<"[!] Admin only.\n"; return; }
        int id; cout<<"Enter Event ID: "; cin>>id; cin.ignore();
        auto it=find_if(events.begin(),events.end(),[&](auto &e){return e.id==id;});
        if (it!=events.end()) { events.erase(it); saveEvents(); cout<<"Deleted!\n"; }
        else cout<<"Not found!\n";
    }

    void viewDay() {
        string d; cout<<"Enter date: "; getline(cin,d);
        vector<Event> day;
        for (auto &e: events) if (e.date==d) day.push_back(e);
        sort(day.begin(),day.end(),[](auto &a, auto &b){return a.time<b.time;});
        if (day.empty()) cout<<"No events.\n";
        else for (auto &e: day) e.print();
    }

    void searchEvent() {
        string k; cout<<"Keyword: "; getline(cin,k);
        transform(k.begin(),k.end(),k.begin(),::tolower);
        for (auto &e: events) {
            string n=e.name, t=e.type;
            transform(n.begin(),n.end(),n.begin(),::tolower);
            transform(t.begin(),t.end(),t.begin(),::tolower);
            if (n.find(k)!=string::npos || t.find(k)!=string::npos) e.print();
        }
    }

    void analytics() {
        map<string,int> typeCount;
        for (auto &e: events) typeCount[e.type]++;
        cout<<"Total: "<<events.size()<<"\nBy type:\n";
        for (auto &p: typeCount) cout<<"  "<<p.first<<": "<<p.second<<"\n";
    }

    void sendReminders() {
        if (!isAdmin) { cout<<"[!] Admin only.\n"; return; }
        for (auto &e: events)
            cout<<"Reminder: "<<e.name<<" on "<<e.date<<" "<<e.time<<"\n";
    }
};

// ---------------- Main with switch ----------------
int main() {
    EventManager mgr;
    int choice;
    do {
        cout<<"\n===== Smart Event Manager =====\n";
        cout<<"1. Add Event\n2. Edit Event\n3. Delete Event\n";
        cout<<"4. View by Day\n5. Search Event\n6. Analytics\n";
        cout<<"7. Send Reminders\n8. Admin Login\n9. Exit\nChoice: ";
        cin>>choice; cin.ignore();

        switch(choice) {
            case 1: mgr.addEvent(); break;
            case 2: mgr.editEvent(); break;
            case 3: mgr.deleteEvent(); break;
            case 4: mgr.viewDay(); break;
            case 5: mgr.searchEvent(); break;
            case 6: mgr.analytics(); break;
            case 7: mgr.sendReminders(); break;
            case 8: mgr.adminLogin(); break;
            case 9: cout<<"Goodbye!\n"; break;
            default: cout<<"Invalid choice!\n";
        }
    } while(choice!=9);
    return 0;
}
