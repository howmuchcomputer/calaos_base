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
#include <MqttInputLogic.h>
#include <MqttMap.h>
#include <IOFactory.h>


void _topic_received_input_logic_cb(void *data, EMqtt_Sn_Client *client, const char *topic, const char *value);
void _connect_received_input_logic_cb(void *data, EMqtt_Sn_Client *client, EMQTT_SN_CONNECTION_STATE connection_state);

using namespace Calaos;

REGISTER_INPUT(MqttInputLogic)

MqttInputLogic::MqttInputLogic(Params &p):
InputSwitch(p)
{
     if (get_params().Exists("host"))
	  from_string(get_param("host"), host);
     if (get_params().Exists("port"))
	  from_string(get_param("port"), port);
     if (get_params().Exists("topic"))
	  from_string(get_param("topic"), topic);

     client = MqttMap::Instance((char*)host.c_str(), port, _connect_received_input_logic_cb, (void*)this, 10.0);

     cDebugDom("input") << get_param("id") << ": topic : " <<  topic;

     setValueChanged([=] {
	       hasChanged();
	  });
}

MqttInputLogic::~MqttInputLogic()
{
#ifdef HAVE_EMQTT
     cDebugDom("input") << get_param("id") << ": topic : " << topic;
#endif
}

bool MqttInputLogic::readValue()
{
}

bool MqttInputLogic::setValueChanged(sigc::slot<void> slot)
{
     connection = event_signal.connect(slot);
     return true;
}

void MqttInputLogic::connected()
{
     emqtt_sn_client_subscribe(client, topic.c_str(), _topic_received_input_logic_cb, NULL, this);
}

void MqttInputLogic::emitChange(string val)
{
     if (value == false)
     {
	  value = true;
     }
     else
     {
	  value = false;
     }
     event_signal.emit();
}

void _topic_received_input_logic_cb(void *data, EMqtt_Sn_Client *client, const char *topic, const char *value)
{
     MqttInputLogic *input = (MqttInputLogic *)data;
     input->emitChange(value);
}

void _connect_received_input_logic_cb(void *data, EMqtt_Sn_Client *client, EMqtt_Sn_Connection_State connection_state)
{
     MqttInputLogic *input = (MqttInputLogic *)data;

     if(connection_state == CONNECTION_ACCEPTED)
     {
	  input->connected();

     }
}
