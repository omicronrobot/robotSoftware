# robotSoftware

This is the robot software it contains software for controlling the robot which are:

- Handmotion controller.
- Wifi client for the mobile platform.
- Main MCU for the mobile platform.
- Backend application for different interface control. MQTT, HTTP and WebSocket.

The messaging scheme is guided by this protocol buffer:

```proto
service ThingsService {
  rpc SendMessage(OmicronMessage) returns (OmicronResponse) {}
}

message OmicronMessage {
    string pitch     = 1; // The pitch for moving back and forth. Forward and backward direction
    string roll      = 2; // Roll used to move the robot sideways. Left and Right direction
    string yaw       = 3; // Yaw used to control the direction of the robot. Rotation about perpendicular axis to the floor
    string timestamp = 4; // Timestamp used to denote when the message was sent
    string publisher = 5; // The publisher of the message. It can be handmotion or mobile
    string protocol  = 6; // The protocol used to send the message. It can be websocket, http or mqtt
}

// TODO add respose to robot. To indicate if it is in motion on stationary
message OmicronResponse {}
```

## TODO

- [ ] Setup CI/CD to build the setup go file and publish to releases.
- [ ] Create 3 thing IDs for handmotion client, mobile application client and mobile platform and connect the to one channel.
- [ ] Setup CI/CD for linting and building Setup files.
