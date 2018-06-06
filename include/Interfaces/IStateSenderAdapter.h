//
// Created by manoel on 01/06/18.
//

#ifndef VSS_VISION_ISENDCOMMANDADAPTER_H
#define VSS_VISION_ISENDCOMMANDADAPTER_H

#include <Domain/Robot.h>
#include <vector>
#include <Domain/Ball.h>

class IStateSenderAdapter {

public:

    virtual void createSocket() = 0;
    virtual void sendState(std::vector<vss::Robot>&, vss::Ball&) = 0;

};

#endif //VSS_VISION_ISENDCOMMANDADAPTER_H
