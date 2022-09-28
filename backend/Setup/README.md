# Setup Mainflux

This program is used to setup mainflux system for use by omicron robot. It logins in as the admin user, creates a thing, creates a channel and connects the thing to the channel. In this case we can use a thing to publish messages to the channel. For start we will use on thing to be the 2 clients that will be publishing to the channel, that is the hand motion client and mobile application client and 1 client that will be subscribing from the channel that is the mobile platform.

## Setup

If you want to work on the code you need to be familiar with golang.

1. Install go - Follow [official documentation](https://go.dev/dl/)
2. Install make - Follow [official documentation](https://www.gnu.org/software/make/)
3. Make changes to the code
4. Build the code

Make is used to build the code for different platforms whilst golang is used to write the code that interacts with mainflux system

## Run

To run the system make sure you have started the docker service

```bash
make run
```

or

```bash
./build/omicron-linux
```

The printed thing and channel information will be used when connecting the systems. Make sure you store the thing ID, thing KEY as they are used for authentication purposes. The thingID is the username and thingKey is the password. Also store the channel ID as it is the topic where we will be publishing data and subscribing data from.
