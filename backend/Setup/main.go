package main

import (
	"fmt"
	"log"
	"os/exec"

	pp "github.com/k0kubun/pp/v3"
	sdk "github.com/mainflux/mainflux/pkg/sdk/go"
)

const (
	defURL   string = "http://localhost"
	nMessage int    = 10
	name     string = "omicron"
)

func main() {
	sdkConf := sdk.Config{
		AuthURL:         defURL,
		ThingsURL:       defURL,
		UsersURL:        defURL,
		ReaderURL:       defURL,
		HTTPAdapterURL:  fmt.Sprintf("%s/http", defURL),
		BootstrapURL:    defURL,
		CertsURL:        defURL,
		MsgContentType:  sdk.ContentType(string(sdk.CTJSONSenML)),
		TLSVerification: false,
	}
	mfsdk := sdk.NewSDK(sdkConf)
	adminUser := sdk.User{
		Email:    "omicron@students.jkuat.ac.ke",
		Password: "eRpersAnylAuriEnEYer",
	}
	admintoken, err := mfsdk.CreateToken(adminUser)
	if err != nil {
		log.Fatal(err)
	}

	channel := sdk.Channel{
		Name: fmt.Sprintf("%sChannel", name),
	}
	channelID, err := mfsdk.CreateChannel(channel, admintoken)
	if err != nil {
		log.Fatal(err)
	}
	channel, err = mfsdk.Channel(channelID, admintoken)
	if err != nil {
		log.Fatal(err)
	}

	thing := sdk.Thing{
		Name: fmt.Sprintf("%sThing", name),
	}
	thingID, err := mfsdk.CreateThing(thing, admintoken)
	if err != nil {
		log.Fatal(err)
	}
	thing, err = mfsdk.Thing(thingID, admintoken)
	if err != nil {
		log.Fatal(err)
	}

	connection := sdk.ConnectionIDs{
		ChannelIDs: []string{channel.ID},
		ThingIDs:   []string{thing.ID},
	}
	if err := mfsdk.Connect(connection, admintoken); err != nil {
		log.Fatal(err)
	}

	SendMessage(channel.ID, thing.ID, thing.Key, mfsdk)

	fmt.Printf("User Details\n\tUser Email: %s\n\tUser password: %s\n\tUser Token:%s\n", adminUser.Email, adminUser.Password, admintoken)
	fmt.Printf("Messaging Topic\n\tPublish/Subscribe Topic: channels/%s/messages\n", channelID)
	pp.Println(thing)
	pp.Println(channel)
}

func SendMessage(chanID, thingID, thingKey string, sdk sdk.SDK) {
	// 	[
	//   {"bn":"mobile","n":"pitch","u":"m/s","v":distance},
	//   {"n":"yaw","u":"rad","v":degree}
	// ]
	for p := 0; p < nMessage; p++ {
		msg1 := fmt.Sprintf("[{\"bn\":\"http-app\",\"n\":\"pitch\",\"u\":\"m/s\",\"v\":%d}, {\"n\":\"yaw\",\"u\":\"rad\",\"v\":%d}]", p, p)
		cmd1 := exec.Command("curl", "-s", "-S", "-i", "-X", "POST", "-H", "Content-Type: application/senml+json", "-H", fmt.Sprintf("Authorization: Thing %s", thingKey), fmt.Sprintf("http://localhost/http/channels/%s/messages", chanID), "-d", msg1)
		if _, err := cmd1.Output(); err != nil {
			log.Fatal(err)
		}
		msg2 := fmt.Sprintf("[{\"bn\":\"mqtt-app\",\"n\":\"pitch\",\"u\":\"m/s\",\"v\":%d}, {\"n\":\"yaw\",\"u\":\"rad\",\"v\":%d}]", p, p)
		cmd2 := exec.Command("mosquitto_pub", "-u", thingID, "-P", thingKey, "-t", fmt.Sprintf("channels/%s/messages", chanID), "-h", "localhost", "-m", msg2)
		if _, err := cmd2.Output(); err != nil {
			log.Fatal(err)
		}
	}
}
