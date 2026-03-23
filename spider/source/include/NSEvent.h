//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/25/25.
//

#ifndef NS_EVENT_H
#define NS_EVENT_H

#include "IEventReceiver.h"

struct NSEvent : Event {
    enum
    {
        EVT_CANCEL = Event::EVT_EVENT_LAST,
        EVT_DEAL,
        EVT_UNDO,
        EVT_LEFT,
        EVT_RIGHT,
        EVT_UP,
        EVT_DOWN
    };
};

#endif //NS_EVENT_H
