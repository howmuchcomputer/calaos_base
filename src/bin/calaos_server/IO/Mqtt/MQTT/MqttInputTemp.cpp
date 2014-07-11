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
void _suback_received_cb(void *data, EMQTT_SN_ERROR_TYPE state);
void _connect_received_cb(void *data, EMqtt_Sn_Client *client, EMqtt_Sn_CONNECTION_STATE connection_state);

using namespace Calaos;

REGISTER_INPUT(MqttInputTemp)

MqttInputTemp::MqttInputTemp(Params &p):
    InputTemp(p),
    port(1883)
{
  
  //    address = get_param("mqtt_addr");
    address = "::";

    if (get_params().Exists("mqtt_port"))
        Utils::from_string(get_param("mqtt_port"), port);
    mqtt_id = get_param("mqtt_id");
    mqtt_args = get_param("mqtt_args");

    client = MqttMap::Instance(address, port, _connect_received_cb, (void*)this, 10.0);

    cDebugDom("input") << get_param("id") << ": MQTT_ID : " << mqtt_id;
}

MqttInputTemp::~MqttInputTemp()
{

#ifdef HAVE_EMQTT
    cDebugDom("input") << get_param("id") << ": MQTT_ID : " << mqtt_id;
#endif

}



void MqttInputTemp::readValue()
{
  val = roundValue(val);
  if (val != value)
  {
    value = val;
    emitChange();
    cInfoDom("input") << " Ok value :  " << val;
  }

}





void _topic_received_cb(void *data, EMqtt_Sn_Client *client, const char *topic, const char *value)
{

  MqttInputTemp *temp = (MqttInputTemp *)data;

  printf("TOPIC : %s\n", topic);
  printf("MSG : %s\n", value);

  from_string(value, temp->val);
  temp->readValue();

}


void _suback_received_cb(void *data, EMQTT_SN_ERROR_TYPE state)
{
    printf("SUBACK : %d\n", state);
}


void _connect_received_cb(void *data, EMqtt_Sn_Client *client, EMqtt_Sn_CONNECTION_STATE connection_state)
{
  printf("State: %d\n",connection_state);
  if(connection_state == CONNECTION_ACCEPTED){
    emqtt_sn_client_subscribe(client,"temp", _topic_received_cb, _suback_received_cb, data);
  }
}
