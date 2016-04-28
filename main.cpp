#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>

using namespace std;

Gtk::Entry* passwordField;
Gtk::Button* cancelButton;
Gtk::Button* mountButton;

void luksOpen(string dev, string luksName, const char* pwd, size_t pwdLength){
    string cmd = "cryptsetup luksOpen ";
    cmd.append(dev).append(" ").append(luksName);
    FILE *result = popen(cmd.c_str(), "w");
    // pray
    fwrite(pwd, 1, pwdLength, result);
}

void onMount(){
    string pwd = passwordField->get_text();
    // luksOpen
    luksOpen("/dev/sda8", "data", pwd.c_str(), pwd.length());
    luksOpen("/dev/sda9", "prog", pwd.c_str(), pwd.length());

    // mount
    system("sudo mount /dev/mapper/data /home/arne/Documents");
    system("sudo mount /dev/mapper/progs /home/arne/Programs");

    // pray
    exit(0);
}

void onCancel(){
    exit(0);
}

bool keyboardPressed(GdkEventKey* event){
    switch(event->keyval){
        case GDK_KEY_Escape:
            onCancel();
            break;
        case GDK_KEY_Return:
            onMount();
            break;
        default:
            return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "de.oerntec.openencrypteddrives");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("dialog.glade");
    Gtk::Window* win;
    builder->get_widget("all", win);

    // get widgets
    builder->get_widget("passwordField", passwordField);
    builder->get_widget("cancelButton", cancelButton);
    builder->get_widget("mountButton", mountButton);

    // button handling
    cancelButton->signal_clicked().connect(sigc::ptr_fun(onCancel));
    mountButton->signal_clicked().connect(sigc::ptr_fun(onMount));

    // keyboard handling
    win->add_events(Gdk::KEY_PRESS_MASK);
    win->signal_key_release_event().connect(sigc::ptr_fun(keyboardPressed));

    return app->run(*win);
}