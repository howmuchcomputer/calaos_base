/******************************************************************************
 **  Copyright (c) 2006-2014, Calaos. All Rights Reserved.
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

#include <MqttMap.h>

using namespace Calaos;



MqttMapManager MqttMap::mqttmaps;

MqttMap::MqttMap(char* addr, int p)
{
    address = addr;
    port = p;
   
}


MqttMap::~MqttMap()
{
#ifdef HAVE_EMQTT
    cInfoDom("Mqtt Map");
#endif
}



EMqtt_Sn_Client* MqttMap::Instance(char* addr, int p, EMqtt_Sn_Client_Connect_Cb connected_cb, void *data, double keepalive)
{
    for (uint i = 0;i < mqttmaps.maps.size();i++)
    {
        if (mqttmaps.maps[i]->get_addr() == addr &&
            mqttmaps.maps[i]->get_port() == p)
        {
            return mqttmaps.maps[i]->client;
        }
    }

    // Create a new mqtt mapping object
    MqttMap *mqttmap = new MqttMap(addr, p);
#ifdef HAVE_EMQTT
    mqttmap->client = emqtt_sn_client_add(addr, p, "Calaos");
    emqtt_sn_client_connect(mqttmap->client, connected_cb, data, 10.0);
#endif

    mqttmaps.maps.push_back(mqttmap);
    return mqttmaps.maps[mqttmaps.maps.size() - 1]->client;
}



void MqttMap::stopAllMqttMaps()
{
    std::for_each(mqttmaps.maps.begin(), mqttmaps.maps.end(), Delete());
    mqttmaps.maps.clear();
}
