from flask import *
import json
import paho.mqtt.client as mqtt
import createToken


server = Flask(__name__)
broker_address="test.mosquitto.org"
print("creating new instance")


def getJSONData(path):
  with open(path) as json_file:
    data = json.load(json_file)

  return data

def write_json(new_data, filename):
    with open(filename,'r+') as file:
        
        file_data = json.load(file)
        file_data.append(new_data)
        file.seek(0)
        json.dump(file_data, file, indent = 4)
 
@server.route("/api/admin/login", methods=["POST"])  
def adminLogin():
    request_json = request.get_json() 
    username = str(request_json["username"])
    password = str(request_json["password"])

    adminsData = getJSONData('Database/admins.JSON')
    for user_info in adminsData:
      if username in user_info['username']:
        if user_info['password'] == password:
          return Response(json.dumps({
            "Token":createToken.login_token(username,"admin")
          }), status=201, mimetype="application/json")
    return Response("{status: False}", status=401, mimetype="application/json")

@server.route("/api/user/login", methods=["POST"]) 
def userLogin():
    request_json = request.get_json() 
    user_id = str(request_json["user_id"])
    card = str(request_json["card"])
    room_number = str(request_json["room_number"])
    
    usersData = getJSONData('Database/users.JSON')
    for user_info in usersData:
      if user_id in user_info['user_id']:
        if user_info['card'] == card and user_info['room_number'] == room_number:
          return Response(json.dumps({
            "Token": createToken.login_token(user_id, "user")
            
          }), status=201, mimetype="application/json")
    return Response("{status: False}", status=401, mimetype="application/json")

@server.route("/api/admin/register", methods=["POST"])  
def registerAdmin():
  
    request_json = request.get_json() 
    username = str(request_json["username"])
    password = str(request_json["password"])

    newAdmin = {"username":username,
     "password": password,
     
    }

    try:
      write_json(newAdmin, 'Database/admins.JSON')
    except:
      return Response("{status: False}", status=401, mimetype="application/json")

    return Response("{status: True}", status=201, mimetype="application/json")
    
@server.route("/api/admin/user/register", methods=["POST"])  
def register_user_by_admin():
  if createToken.check_role(request) == "admin":
    request_json = request.get_json()  
    card = str(request_json["card"])
    password = str(request_json["password"])
    room_number = str(request_json["room_number"])
    usersData = getJSONData('Database/users.JSON')

    newUser = {
      "user_id": str(len(usersData)+1),
      "card": card,
      "room_number":room_number,
      "password": password
    }

    try:
      write_json(newUser, 'Database/users.JSON')
    except:
      return Response("{status: False}", status=401, mimetype="application/json")

    return Response("{status: True}", status=201, mimetype="application/json")
  else:
     return Response("{status: False}", status=401, mimetype="application/json")

@server.route("/api/user/<userid>", methods=["GET"])  
def set_light_brightness(userid):
    
  if createToken.check_role(request) == "user" and createToken.check_user_info(request) == str(userid):
    usersData = getJSONData('Database/users.JSON')
    check_user = False
    for user_info in usersData:
      if user_info["user_id"] == userid:
         check_user = True
    if not check_user:
       return Response("{status: False}", status=401, mimetype="application/json")

    brightness = request.args.get("lights") 

    if int(brightness) < 0 or int(brightness) > 255 :
       return Response("{status: False}", status=400, mimetype="application/json")

    try:
      
      client = mqtt.Client("P1")
      print("connecting to broker")
      client.connect(broker_address)
      client.publish("smartOffice",brightness)
    except:
      return Response("{status: False}", status=401, mimetype="application/json")

    return Response("{status: True}", status=200, mimetype="application/json")
  else:
    return Response("{status: False}", status=401, mimetype="application/json")

@server.route("/api/admin/setlights", methods=["POST"])  
def set_lighting_time():
  if createToken.check_role(request) == "admin":
    request_json = request.get_json() 
    lightsOnTime = str(request_json["lightsOnTime"])
    lightsOffTime = str(request_json["lightsOffTime"])
  
    try:
      
      client = mqtt.Client("P1")
      print("connecting to broker")
      client.connect(broker_address)
      client.publish("smartOffice/lightsOnTime",lightsOnTime)
      client.publish("smartOffice/lightsOffTime",lightsOffTime)
    except:
      return Response("{status: False}", status=401, mimetype="application/json")

    return Response("{status: True}", status=200, mimetype="application/json")
  else:
    return Response("{status: False}", status=401, mimetype="application/json")



if __name__ == '__main__':
    server.run(debug=True)
