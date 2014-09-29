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
#include <MqttInputTemp.h>
#include <MqttMap.h>
#include <IOFactory.h>

void _topic_received_cb(void *data, EMqtt_Sn_Client *client, const char *topic, const char *value);
void _connect_received_cb(void *data, EMqtt_Sn_Client *client, EMqtt_Sn_CONNECTION_STATE connection_state);

using namespace Calaos;

REGISTER_INPUT(MqttInputTemp)

MqttInputTemp::MqttInputTemp(Params &p):
InputTemp(p)
{
     if (get_params().Exists("host"))
	  from_string(get_param("host"), host);
     if (get_params().Exists("port"))
	  from_string(get_param("port"), port);
     if (get_params().Exists("topic"))
	  from_string(get_param("topic"), topic);

     client = MqttMap::Instance((char*)host.c_str(), port, _connect_received_cb, (void*)this, 10.0);

     cDebugDom("input") << get_param("id") << ": topic : " <<  topic;
}

MqttInputTemp::~MqttInputTemp()
{
#ifdef HAVE_EMQTT
     cDebugDom("input") << get_param("id") << ": topic : " << topic;
#endif
}

void MqttInputTemp::readValue()
{
}

void MqttInputTemp::connected()
{
     emqtt_sn_client_subscribe(client, topic.c_str(), _topic_received_cb, NULL, this);
}

void MqttInputTemp::setValue(double val)
{
     if (val != value)
     {
	  value = roundValue(val);
	  emitChange();
	  cInfoDom("input") << " Ok value :  " << value;
     }
}

void _topic_received_cb(void *data, EMqtt_Sn_Client *client, const char *topic, const char *value)
{
     MqttInputTemp *temp = (MqttInputTemp *)data;
     double val = 0.0;

     from_string(value, val);
     temp->setValue(val);
}

void _connect_received_cb(void *data, EMqtt_Sn_Client *client, EMqtt_Sn_CONNECTION_STATE connection_state)
{
     MqttInputTemp *temp = (MqttInputTemp *)data;

     if(connection_state == CONNECTION_ACCEPTED)
     {
	  temp->connected();
     }
}
