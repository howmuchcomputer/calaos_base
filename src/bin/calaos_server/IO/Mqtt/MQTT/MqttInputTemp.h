/******************************************************************************
 **  Copyright (c) 2007-2014, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos.
 **
 **  Calaos is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Calaos is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
#ifndef S_MqttInputTemp_H
#define S_MqttInputTemp_H

#include <InputTemp.h>
#include <MqttMap.h>


namespace Calaos
{

class MqttInputTemp : public InputTemp
{

protected:
    char* address;
    int port;
    std::string mqtt_id;
    std::string mqtt_args; 
    
    EMqtt_Sn_Client* client;
    
public:
    MqttInputTemp(Params &p);
    ~MqttInputTemp();

    double val;

    virtual void readValue();

};

}
#endif
