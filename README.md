# robot software

This is the robot software it contains software for controlling the robot which are:

- Hand motion controller.
- Wifi client for the mobile platform.
- Main MCU for the mobile platform.
- Backend application for different interface control. MQTT, HTTP, and WebSocket.

The messaging scheme is guided by this protocol buffer:

```proto
service OmicronService {
	rpc SendMessage(OmicronMessage) returns (OmicronResponse) {}
}

message OmicronMessage {
	float pitch     = 1; // The pitch for speed control of the robot. Speed is either max forward or zero or max revers. float -1 to +1.
	int   yaw       = 2; // Yaw used to control the direction of the robot. Rotation about perpendicular axis to the floor. Int 0 to 180.
	string metadata = 4; // The metadata of the message. It can be used to add any payload to the message for example the publisher.
}

// Message 0.6:165:handmotion 30Bytes

message OmicronResponse {}
```

The messages sent follow the [SenML](https://datatracker.ietf.org/doc/html/draft-ietf-core-senml#section-3) language rule and here is an example

```json
[
	{"bn":"mobile","n":"pitch","u":"m/s","v":pitch},
	{"n":"yaw","u":"rad","v":yaw}
]
```

A single array that represents a collection of measurements is carried by each SenML Pack. A sequence of SenML Records containing the fields listed below makes up this array. Base fields and ordinary fields are the two types of fields. Any SenML Record may contain either the standard fields or the base fields. The base fields apply to the entries in the Record as well as to every Record that comes after it, but not to the next Record with that base field. Any base field can be used. Any SenML Record can contain regular fields, and those fields only apply to that Record.

## Setup

1. Install [PlatformIO using their documentation](https://docs.platformio.org/en/latest/core/index.html)
2. Use Python3.7 - [Follow docs to install](https://linuxize.com/post/how-to-install-python-3-7-on-ubuntu-18-04/)
3. Install Make - Follow the [official documentation](https://www.gnu.org/software/make/)
4. Pull submodules

```bash
git submodule update --init --recursive
```
