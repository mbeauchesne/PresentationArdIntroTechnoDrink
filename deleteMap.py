import requests
import json

# You could use this snippet to delete your old data if you have an error.

response = requests.delete('http://192.168.5.111:9200/technodrink')
try:
    assert response.status_code is 200
except AssertionError:
    raise AssertionError("Your mapping was not deleted", response)
