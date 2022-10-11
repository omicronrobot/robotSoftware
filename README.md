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
	float pitch     = 1; // The pitch for speed control of the robot. Speed is either max forward or zero or max revers. float -1 to +1.
	int   yaw       = 2; // Yaw used to control the direction of the robot. Rotation about perpendicular axis to the floor. Int 0 to 360.
	string metadata = 4; // The metadata of the message. It can be used to add any payload to the message for example the publisher.
}

// Message 0.6:270:handmotion 30Bytes

message OmicronResponse {}
```

## Setup

1. Install [PlatformIO using their documentation](https://docs.platformio.org/en/latest/core/index.html)
2. Use Python3.7 - [Follow docs to install](https://linuxize.com/post/how-to-install-python-3-7-on-ubuntu-18-04/)
2. Install Make - Follow [official documentation](https://www.gnu.org/software/make/)

## TODO

- [ ] Setup CI/CD to build the setup go file and publish to releases.
- [ ] Create 3 thing IDs for handmotion client, mobile application client and mobile platform and connect the to one channel.
- [ ] Setup CI/CD for linting and building Setup files.
