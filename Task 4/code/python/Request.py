import json
import requests
import paho.mqtt.client as mqtt

token = None

def main():
    
    print("hello")
    broker_address="test.mosquitto.org"
    print("creating new instance")
    client = mqtt.Client("P1")
    client.on_message=on_message 
    print("connecting to broker")
    client.connect(broker_address)

    print("Subscribing to topic","smartOffice/rfid")
    client.subscribe("smartOffice/rfid")
    client.loop_forever()

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
    resp = requests.post("/api/user/login" , json= json.dumps({"user_id":1,
    "code":str(message.payload.decode("utf-8")),
    "room_number":1}))  
    token = resp['Token']

if __name__ == '__main__':
    main()
  
