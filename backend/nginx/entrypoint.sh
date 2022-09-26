#!/bin/ash

if [ -z "$OR_MQTT_CLUSTER" ]
then
      envsubst '${OR_MQTT_ADAPTER_MQTT_PORT}' < /etc/nginx/snippets/mqtt-upstream-single.conf > /etc/nginx/snippets/mqtt-upstream.conf
      envsubst '${OR_MQTT_ADAPTER_WS_PORT}' < /etc/nginx/snippets/mqtt-ws-upstream-single.conf > /etc/nginx/snippets/mqtt-ws-upstream.conf
else
      envsubst '${OR_MQTT_ADAPTER_MQTT_PORT}' < /etc/nginx/snippets/mqtt-upstream-cluster.conf > /etc/nginx/snippets/mqtt-upstream.conf
      envsubst '${OR_MQTT_ADAPTER_WS_PORT}' < /etc/nginx/snippets/mqtt-ws-upstream-cluster.conf > /etc/nginx/snippets/mqtt-ws-upstream.conf
fi

envsubst '
    ${OR_USERS_HTTP_PORT}
    ${OR_THINGS_HTTP_PORT}
    ${OR_THINGS_AUTH_HTTP_PORT}
    ${OR_HTTP_ADAPTER_PORT}
    ${OR_NGINX_MQTT_PORT}
    ${OR_NGINX_MQTTS_PORT}
    ${OR_AUTH_HTTP_PORT}
    ${OR_WS_ADAPTER_PORT}' < /etc/nginx/nginx.conf.template > /etc/nginx/nginx.conf

exec nginx -g "daemon off;"
