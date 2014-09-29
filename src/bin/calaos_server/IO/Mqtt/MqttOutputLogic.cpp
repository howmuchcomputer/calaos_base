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
#include <MqttOutputLogic.h>
#include <MqttMap.h>
#include <IOFactory.h>

void _connect_received_output_logic_cb(void *data, EMqtt_Sn_Client *client, EMQTT_SN_CONNECTION_STATE connection_state);

using namespace Calaos;

REGISTER_OUTPUT(MqttOutputLogic)

MqttOutputLogic::MqttOutputLogic(Params &p):
OutputLight(p)
{
     if (get_params().Exists("host"))
	  from_string(get_param("host"), host);
     if (get_params().Exists("port"))
	  from_string(get_param("port"), port);
     if (get_params().Exists("topic"))
	  from_string(get_param("topic"), topic);

     connected = false;

     client = MqttMap::Instance((char*)host.c_str(), port, _connect_received_output_logic_cb, (void*)this, 10.0);

     cDebugDom("input") << get_param("id") << ": topic : " <<  topic;
}

MqttOutputLogic::~MqttOutputLogic()
{
#ifdef HAVE_EMQTT
     cDebugDom("output") << get_param("id") << ": topic : " << topic;
#endif
}

bool MqttOutputLogic::set_value_real(bool val)
{
     if (connected == true)
     {
	  if (val == true)
	  {
	       emqtt_sn_client_publish(client, topic.c_str(), "ON", NULL, NULL);
	  }
	  else
	  {
	       emqtt_sn_client_publish(client, topic.c_str(), "OFF", NULL, NULL);
	  }
     }
     return true;
}

void _connect_received_output_logic_cb(void *data, EMqtt_Sn_Client *client, EMQTT_SN_CONNECTION_STATE connection_state)
{
     MqttOutputLogic *output = (MqttOutputLogic *)data;

     if (connection_state == EMQTT_SN_CONNECTION_ACCEPTED)
     {
	  output->connected = true;
     }
}
