import jwt


def login_token(username,role):

 return jwt.encode({"username":username,"role": role}, "secret", algorithm="HS256").decode('utf-8')




def check_role(req):

  try:
    # print(req.headers)
    token = req.headers['X-Access-Token']
    # encoded=token.split(" ")[1]
    result = jwt.decode(token, "secret", algorithms=["HS256"])
    # print(result)
  except Exception as e:
    print(e)
    return "guest"
  return result['role'] 


def check_user_info(req):
    try:
      # print(req.headers)
      token = req.headers['X-Access-Token']
      # encoded=token.split(" ")[1]
      result = jwt.decode(token, "secret", algorithms=["HS256"])
      # print(result)
    except Exception as e:
      print(e)
      return "guest"
    return result['username']


print(login_token("1", "user"))