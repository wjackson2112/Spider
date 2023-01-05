//
// Created by Will on 12/2/2022.
//

#ifndef SP_MAIN_MENU_H
#define SP_MAIN_MENU_H

#include "Scene.h"
#include "IInputReceiver.h"

class SPMainMenu : public Scene, IInputReceiver
{
public:
    SPMainMenu();
    ~SPMainMenu();

    // TODO: This should probably just have buttons in it instead of supporting the input directly
    //IInputReceiver
    void mouseInputCallback(double xpos, double ypos, MouseButton button, Action action, Modifier mods) override {}
    void keyInputCallback(Key key, int scancode, Action action, Modifier mods) override;
};

#endif //SP_MAIN_MENU_H
