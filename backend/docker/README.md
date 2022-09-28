# Docker Composition

This backend it built with Mainflux. Mainflux is modern, scalable, secure open source and patent-free IoT cloud platform written in Go. It accepts user and thing connections over various network protocols (i.e. HTTP, MQTT, WebSocket, CoAP), thus making a seamless bridge between them. It is used as the IoT middleware for building complex IoT solutions.

This will be a great addition to our project as mainflux accepts HTTP, MQTT and WebSocket connections which are used by our client applications. The Flutter Mobile Application will be able to connect to our backend using either HTTP, MQTT or WebSocket, the handmotion controller will be able to connect to our backend using HTTP and MQTT while the mobile platform will receive the data published by the flutter application client and hand motion application client by MQTT. This will ensure seamless intergration.

Configure environment variables and run Mainflux Docker Composition.

\*Note\*\*: `docker-compose` uses `.env` file to set all environment variables. Ensure that you run the command from the same location as .env file.

## Installation

1. Install Docker - Follow the [official documentation](https://docs.docker.com/compose/install/).
2. Install Make - Follow the [official documentation](https://www.gnu.org/software/make/)

## Usage

Run following commands from this directory.

```bash
make run
```

To pull docker images from a specific release you need to change the value of `OR_RELEASE_TAG` in `.env` before running these commands.

## Setup Mainflux

Install [omicron-linux](../Setup/build/omicron-linux) or [omicron-windows](../Setup/build/omicron-windows.exe) on your system. Omicron binary makes it easy to manage users, things, channels.

CLI can be downloaded as separate asset from [Setup folder](../Setup/) or it can be built with GNU Make tool:

Run the following commands

```bash
omciron-linux
```

## You can do it on you own by

Install mainflux-cli on your system. Mainflux CLI makes it easy to manage users, things, channels and messages.

CLI can be downloaded as separate asset from [project realeses](https://github.com/mainflux/mainflux/releases) or it can be built with GNU Make tool:

### Login Admin User

```bash
mainflux-cli users token admin@example.com 12345678
```

### Store the USERTOKEN

```bash
USERTOKEN=<token-after-login>
```

### Create thing

```bash
mainflux-cli things create '{"name":"myThing"}' $USERTOKEN

```

### Get thing Key

```bash
mainflux-cli things get <thing_id> $USERTOKEN
```

### Create Channel

```bash
mainflux-cli channels create '{"name":"myChannel"}' $USERTOKEN
```

### Connect Thing and Channel

```bash
mainflux-cli things connect <thing_id> <channel_id> $USERTOKEN
```

### Start listening to data

```bash
mosquitto_sub -u <thing_id> -P <thing_key> -t channels/<channel_id>/messages -h localhost
```

### Start publishing data

#### HTTP

```bash
curl -s -S -i -X POST -H "Content-Type: application/senml+json" -H "Authorization: Thing <thing_key>" http://localhost/http/channels/<channel_id>/messages -d '[{"bn":"omicron:","bt":1.276020076001e+09,"bu":"A","bver":5,"n":"voltage","u":"V","v":120.1}, {"n":"current","t":-5,"v":1.2}, {"n":"current","t":-4,"v":1.3}]'
```

#### MQTT

```bash
mosquitto_pub -u <thing_id> -P <thing_key> -t channels/<channel_id>/messages -h localhost -m '[{"bn":"some-base-name:","bt":1.276020076001e+09, "bu":"A","bver":5, "n":"voltage","u":"V","v":120.1}, {"n":"current","t":-5,"v":1.2}, {"n":"current","t":-4,"v":1.3}]'

```
