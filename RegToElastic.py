
import requests
import json


dataTechnoDrink = {
    "mappings": {
        "reading": {
            "properties": {
                "sound": {"type": "float"},
                "temperature": {"type": "float"},
                "lumens": {"type": "float"},
		"led": {"type": "float"},
		"timestamp": {"type": "date", "format":"epoch_millis"},
                "date": {"type": "date","format": "yyyy-MM-dd HH:mm:ss"}
            },
        }, 
    }
}
response = requests.put('http://192.168.5.111:9200/technodrink', data=json.dumps(dataTechnoDrink))
try:
    assert response.status_code is 200
except AssertionError:
    raise AssertionError("Your mapping was not created", response)
