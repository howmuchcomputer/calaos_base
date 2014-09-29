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
#ifndef S_MQTTMAP_H
#define S_MQTTMAP_H

#include <Calaos.h>
#include <Utils.h>

#include <EMqtt.h>

namespace Calaos
{


class MqttMap;
class MqttMapManager
{
public:
    ~MqttMapManager()
    {
        std::for_each(maps.begin(), maps.end(), Delete());
        maps.clear();
    }

    vector<MqttMap *> maps;
};


class MqttMap
{
protected: 
    EMqtt_Sn_Client *client;   
    char* address;
    int port;

    static MqttMapManager mqttmaps;

    MqttMap(char* addr, int p);

public:
    ~MqttMap();

    //Singleton
    static EMqtt_Sn_Client* Instance(char* addr, int p, EMqtt_Sn_Client_Connect_Cb connected_cb, void *data, double keepalive);
    static vector<MqttMap *> &get_maps() { return mqttmaps.maps; }
    static void stopAllMqttMaps();

    char* get_addr() { return address; }
    int get_port() { return port; }

};

}
#endif
